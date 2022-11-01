#include <grpcpp/create_channel.h>

#include <utility>

#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"


namespace herd
{
	namespace detail
	{
		namespace
		{
			constexpr const char* const AUTH_TOKEN_KEY = "Authorization";
			constexpr const char* const BEARER_PREFIX = "Bearer ";
		}

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

	RemoteBackend::RemoteBackendConnectionImpl::RemoteBackendConnectionImpl(const RemoteBackendConfig& config, std::string  token) noexcept
		:authentication_token_(std::move(token))
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

	void RemoteBackend::RemoteBackendConnectionImpl::destroy_session(const UUID& uuid)
	{
		grpc::ClientContext client_context{};
		setup_authenticated_context(client_context);

		proto::SessionDestroyRequest request;
		request.set_uuid(uuid.as_string());

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
				std::make_unique<detail::TokenMetadataCredentialsPlugin>(connection_token_)
			)
		);
	}
}