#ifndef LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP
#define LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include <auth.grpc.pb.h>
#include <session.grpc.pb.h>
#include <storage.grpc.pb.h>
#include <execution.grpc.pb.h>

#include "herd/backend/remote/remote_backend.hpp"
#include "herd/session/session.hpp"


namespace herd
{
	namespace mapper
	{
		class TokenMetadataCredentialsPlugin: public grpc::MetadataCredentialsPlugin
		{
		public:
			explicit TokenMetadataCredentialsPlugin(const std::string& token);

			bool IsBlocking() const override;
			grpc::Status GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name, const grpc::AuthContext& channel_auth_context, std::multimap<grpc::string, grpc::string>* metadata) override;
			grpc::string DebugString() override;

		private:
			std::string token_;
		};
	}

	class RemoteBackend::RemoteBackendConnectionImpl
	{
	public:
		explicit RemoteBackendConnectionImpl(RemoteBackend& backend, utils::ThreadPool& pool, const RemoteBackendConfig& config, std::string token) noexcept;

		void connect();

		SessionInfo create_session(const std::string& name);
		void destroy_session(const common::UUID& session_uuid);
		std::vector<SessionInfo> list_sessions();

		utils::ProgressFuture<void> add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data);

		std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataFrame>>, std::shared_ptr<storage::DataFrame>> create_data_frame(
				const common::UUID& session_uuid, const std::string& name,
				const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
				std::size_t row_count,
				utils::MovableFunction<bool(std::vector<std::byte>&)> next_row);

		std::vector<std::shared_ptr<storage::DataFrame>> list_data_frames(const common::UUID& session_uuid);

		executor::JobInfo schedule_job(const common::UUID& session_uuid, const common::ExecutionPlan& plan);
		std::vector<executor::JobState> list_jobs(const common::UUID& session_uuid);
		executor::JobInfo describe_job(const common::UUID& session_uuid, const common::UUID& uuid);
		executor::JobState get_job_state(const common::UUID& session_uuid, const common::UUID& uuid);

	private:
		RemoteBackend& backend_;
		utils::ThreadPool& pool_;

		std::string address_;

		std::string authentication_token_;
		std::string connection_token_;

		std::shared_ptr<grpc::ChannelCredentials> credentials_;

		std::shared_ptr<grpc::Channel> channel_;

		std::unique_ptr<herd::proto::Auth::Stub> auth_service_stub_;
		std::unique_ptr<herd::proto::Session::Stub> session_service_stub_;
		std::unique_ptr<herd::proto::Storage::Stub> storage_service_stub_;
		std::unique_ptr<herd::proto::Execution::Stub> execution_service_stub_;

		void create_stubs();
		void authenticate();

		void setup_authenticated_context(grpc::ClientContext& context) const noexcept;
	};
}

#endif //LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP
