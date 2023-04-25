#include <grpcpp/create_channel.h>

#include <utility>

#include "herd/backend/remote/detail/mapper.hpp"
#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"
#include "herd/data_storage/remote/remote_data_frame.hpp"


namespace herd
{
	namespace
	{
		constexpr const char* const AUTH_TOKEN_KEY = "authorization";
		constexpr const char* const BEARER_PREFIX = "Bearer ";
		constexpr std::size_t BLOCK_SIZE = 512;

		struct UploadKeyState
		{
			std::unique_ptr<grpc::ClientContext> context;
			std::unique_ptr<grpc::ClientWriter<proto::SessionAddKeyRequest>> writer;
			std::unique_ptr<proto::Empty> response;
		};

		struct UploadFrameState
		{
			std::unique_ptr<grpc::ClientContext> context;
			std::unique_ptr<grpc::ClientReaderWriterInterface<proto::DataFrameAddRequest, proto::DataFrameAddResponse>> reader_writer;
		};

		void do_upload_key(
				utils::ProgressPackagedTask<void()>::ProgressUpdateProxy& progress_proxy,
				UploadKeyState& state,
			    UUID session_uuid, common::SchemaType type, std::vector<std::byte> key_data
		)
		{
			progress_proxy.set_max_step(key_data.size()-1);
			//send metadata
			{
				proto::SessionAddKeyRequest request;
				const auto options_request = request.mutable_options();
				options_request->set_session_uuid(session_uuid.as_string());
				options_request->set_size(static_cast<uint32_t>(key_data.size()));
				options_request->set_type(mapper::to_proto(type));

				state.writer->Write(request);
			}

			for(uint64_t sent = 0; sent < key_data.size();)
			{
				proto::SessionAddKeyRequest request;
				const auto data_request = request.mutable_data();

				const size_t to_send = std::min(BLOCK_SIZE, key_data.size() - sent);
				data_request->mutable_blob()->resize(to_send);
			    std::memcpy(data_request->mutable_blob()->data(), reinterpret_cast<char*>(key_data.data()) + sent, to_send);
				state.writer->Write(request);

				sent += to_send;
				progress_proxy.step(to_send);
			}

			state.writer->WritesDone();
			state.writer->Finish();
		}

		UUID do_init_upload_frame(
				UploadFrameState& state,
				UUID session_uuid, const std::string& name,
				common::SchemaType type, const std::vector<storage::DataFrame::ColumnParameters>& columns,
				std::size_t row_count
		)
		{
			proto::DataFrameAddRequest request;
			const auto info_request = request.mutable_info();
			info_request->set_session_uuid(session_uuid.as_string());
			info_request->set_name(name);
			info_request->set_type(mapper::to_proto(type));

			const auto columns_proto = info_request->mutable_columns();
			columns_proto->CopyFrom(mapper::to_proto(columns));

			info_request->set_row_count(static_cast<uint32_t>(row_count));

			state.reader_writer->Write(request);

			proto::DataFrameAddResponse response;
			state.reader_writer->Read(&response);

			return UUID(response.metadata().uuid());
		}

		[[maybe_unused]] void do_upload_data_frame(utils::ProgressPackagedTask<std::shared_ptr<storage::DataFrame>()>::ProgressUpdateProxy& progress_proxy,
												   UploadFrameState& state,
								  std::size_t row_count, utils::MovableFunction<bool(std::vector<std::byte>&)> next_row)
		{
			progress_proxy.set_max_step(row_count);

			std::vector<std::byte> buffer;

			for(std::size_t i = 0; i < row_count; i += 64lu)
			{
				std::size_t current_block_size = std::min(64lu, row_count - i);

				buffer.clear();
				for(std::size_t j = 0; j < current_block_size; ++j)
				{
					if(!next_row(buffer))
					{
						throw std::runtime_error("Error while encrypting data");
					}
				}
				progress_proxy.step(current_block_size);

				proto::DataFrameAddRequest request;
				auto data = request.mutable_data();
				data->mutable_blob()->assign(reinterpret_cast<const char*>(buffer.data()), buffer.size());
				state.reader_writer->Write(request);
			}

			state.reader_writer->WritesDone();

			state.reader_writer->Finish();
		}
	}

	namespace mapper
	{
		TokenMetadataCredentialsPlugin::TokenMetadataCredentialsPlugin(const std::string& token)
			:token_(BEARER_PREFIX + token)
		{}

		bool TokenMetadataCredentialsPlugin::IsBlocking() const
		{
			return false;
		}

		grpc::Status TokenMetadataCredentialsPlugin::GetMetadata(
				grpc::string_ref service_url, grpc::string_ref method_name,
				const grpc::AuthContext& channel_auth_context, std::multimap<grpc::string, grpc::string>* metadata
		)
		{
			static_cast<void>(service_url);
			static_cast<void>(method_name);
			static_cast<void>(channel_auth_context);
			assert(metadata != nullptr);
			metadata->insert(std::make_pair(AUTH_TOKEN_KEY, token_));

			return grpc::Status::OK;
		}

		grpc::string TokenMetadataCredentialsPlugin::DebugString()
		{
			return "ConnectionToken(" + token_ + ")";
		}
	}

	RemoteBackend::RemoteBackendConnectionImpl::RemoteBackendConnectionImpl(RemoteBackend& backend, utils::ThreadPool& pool, const RemoteBackendConfig& config, std::string  token) noexcept
		: backend_(backend), pool_(pool), authentication_token_(std::move(token))
	{
		if(config.security.has_value())
		{
			grpc::SslCredentialsOptions ssl_credentials_options;
			ssl_credentials_options.pem_root_certs = config.security->pem_root_certs_path;
			ssl_credentials_options.pem_cert_chain = config.security->pem_cert_chain;
			ssl_credentials_options.pem_private_key = config.security->pem_private_key;

			credentials_ = grpc::SslCredentials(ssl_credentials_options);
		}
		else
		{
			credentials_ = grpc::experimental::LocalCredentials(LOCAL_TCP);
		}

		address_ = config.server.address + ":" + std::to_string(config.server.port);
		channel_ = grpc::CreateChannel(address_, credentials_);
	}

	void RemoteBackend::RemoteBackendConnectionImpl::connect()
	{
		create_stubs();
		authenticate();
	}

	void RemoteBackend::RemoteBackendConnectionImpl::create_stubs()
	{
		assert(channel_);

		auth_service_stub_ = herd::proto::Auth::NewStub(channel_);
		session_service_stub_ = herd::proto::Session::NewStub(channel_);
		storage_service_stub_ = herd::proto::Storage::NewStub(channel_);
	}

	void RemoteBackend::RemoteBackendConnectionImpl::authenticate()
	{
		assert(auth_service_stub_);
		using namespace herd::proto;

		AuthenticationToken auth_token;
		auth_token.set_authentication_token(authentication_token_);

		ConnectionToken connection_token;
		grpc::ClientContext client_context{};

		if(const auto status = auth_service_stub_->authorize_connection(&client_context, auth_token, &connection_token); !status.ok()) //todo: better error handling
		{
			throw RemoteConnectionError("Failed to authenticate");
		}

		connection_token_ = connection_token.token();
	}

	SessionInfo RemoteBackend::RemoteBackendConnectionImpl::create_session(const std::string& name)
	{
		grpc::ClientContext client_context{};
		setup_authenticated_context(client_context);

		proto::SessionCreateRequest request;
		request.set_name(name);

		proto::SessionInfo response;

		if(auto status = session_service_stub_->create_session(&client_context, request, &response); !status.ok())
		{
			//todo: only happy path properly handled by now
			throw RemoteConnectionError(status.error_message());
		}

		return SessionInfo{
				.name = name,
				.uuid = UUID(response.uuid())
		};
	}

	void RemoteBackend::RemoteBackendConnectionImpl::destroy_session(const UUID& session_uuid)
	{
		grpc::ClientContext client_context{};
		setup_authenticated_context(client_context);

		proto::SessionDestroyRequest request;
		request.set_uuid(session_uuid.as_string());

		proto::Empty response;

		if(auto status = session_service_stub_->destroy_session(&client_context, request, &response); !status.ok())
		{
			//todo: only happy path properly handled by now
			throw RemoteConnectionError(status.error_message());
		}
	}

	std::vector<SessionInfo> RemoteBackend::RemoteBackendConnectionImpl::list_sessions()
	{
		grpc::ClientContext client_context{};
		setup_authenticated_context(client_context);

		proto::Empty request;

		proto::SessionInfoList response;

		if(auto status = session_service_stub_->list_sessions(&client_context, request, &response); !status.ok())
		{
			//todo: only happy path properly handled by now
			throw RemoteConnectionError(status.error_message());
		}

		std::vector<SessionInfo> infos;
		infos.reserve(static_cast<std::size_t>(response.sessions().size()));

		std::ranges::transform(
				response.sessions(), std::back_inserter(infos),
				[](const auto& info)
				{
					return SessionInfo{
						info.name(),
						UUID(info.uuid())
					};
				}
	   );

		return infos;
	}

	void RemoteBackend::RemoteBackendConnectionImpl::setup_authenticated_context(grpc::ClientContext& context) const noexcept
	{
		context.set_credentials(
			grpc::MetadataCredentialsFromPlugin(
				std::make_unique<mapper::TokenMetadataCredentialsPlugin>(connection_token_)
			)
		);
	}

	utils::ProgressFuture<void> RemoteBackend::RemoteBackendConnectionImpl::add_key(const UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data)
	{
		assert(key_data.size() <= std::numeric_limits<uint32_t>::max());

		UploadKeyState state;
		state.context = std::make_unique<grpc::ClientContext>();
		setup_authenticated_context(*state.context);

		state.response = std::make_unique<proto::Empty>();

		state.writer = session_service_stub_->add_key(state.context.get(), state.response.get());

		auto task = utils::ProgressPackagedTask<void()>(
		[
			state=std::move(state),
			uuid=session_uuid,
			type=type,
			key_data=std::move(key_data)
		]
		(utils::ProgressPackagedTask<void()>::ProgressUpdateProxy updater) mutable
		{
			do_upload_key(updater, state, uuid, type, std::move(key_data));
		});

		auto future = task.get_future();

		pool_.execute(std::move(task));

		return future;
	}

	std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataFrame>>, std::shared_ptr<storage::DataFrame>> RemoteBackend::RemoteBackendConnectionImpl::create_data_frame(const UUID& session_uuid, const std::string& name, const std::vector<storage::DataFrame::ColumnParameters>& columns, common::SchemaType schema_type, std::size_t row_count, utils::MovableFunction<bool(std::vector<std::byte>&)> next_row)
	{
		UploadFrameState state;
		state.context = std::make_unique<grpc::ClientContext>();
		setup_authenticated_context(*state.context);

		state.reader_writer = storage_service_stub_->add_data_frame(state.context.get());

		const auto data_frame_uuid = do_init_upload_frame(state, session_uuid, name, schema_type, columns, row_count);

		auto data_frame = storage::RemoteDataFrame::make_shared(data_frame_uuid, name, row_count, columns, schema_type, backend_);
		auto task = utils::ProgressPackagedTask<std::shared_ptr<storage::DataFrame>()>(
				[
						state=std::move(state),
						next_row=std::move(next_row),
				        data_frame,
						row_count
			]
			(utils::ProgressPackagedTask<std::shared_ptr<storage::DataFrame>()>::ProgressUpdateProxy updater) mutable
			{
				do_upload_data_frame(updater, state, row_count, std::move(next_row));
				return data_frame;
			});

		auto future = task.get_future();
		pool_.execute(std::move(task));

		return std::make_pair(std::move(future), data_frame);
	}

	std::vector<std::shared_ptr<storage::DataFrame>> RemoteBackend::RemoteBackendConnectionImpl::list_data_frames(const UUID& session_uuid)
	{
		grpc::ClientContext client_context{};
		setup_authenticated_context(client_context);

		proto::DataFrameListRequest request;
		request.set_session_uuid(session_uuid.as_string());

		proto::DataFrameMetadataList response;

		if(auto status = storage_service_stub_->list_data_frames(&client_context, request, &response); !status.ok())
		{
			//todo: only happy path properly handled by now
			throw RemoteConnectionError(status.error_message());
		}

		std::vector<std::shared_ptr<storage::DataFrame>> data_frames;
		data_frames.reserve(static_cast<std::size_t>(response.dataframes().size()));

		std::ranges::transform(
				response.dataframes(), std::back_inserter(data_frames),
				[&](const auto& data_frame)
				{
					return storage::RemoteDataFrame::make_shared(
									UUID(data_frame.uuid()), data_frame.name(),
									data_frame.rows_count(),
									mapper::to_model(data_frame.columns()), mapper::to_model(data_frame.schema_type()),
									backend_);
				}
		);

		return data_frames;
	}
}