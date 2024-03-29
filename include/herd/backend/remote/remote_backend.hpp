#ifndef LIBHERD_REMOTE_BACKEND_HPP
#define LIBHERD_REMOTE_BACKEND_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "herd/backend/i_backend.hpp"
#include "herd/storage/data_frame.hpp"
#include "herd/session/session.hpp"
#include "herd/utils/movable_function.hpp"
#include "herd/utils/pimpl.hpp"
#include "herd/utils/thread_pool.hpp"


namespace herd
{
	namespace storage
	{
		class RemoteDataFrame;
	}

	struct RemoteConnectionError: public std::runtime_error
	{
		using runtime_error::runtime_error;
	};

	struct RemoteBackendConfig
	{
		struct ServerConfig
		{
			std::string address;
			uint16_t port;
		};

		struct Credentials
		{
			std::string pem_root_certs_path;
			std::string pem_private_key;
			std::string pem_cert_chain;
		};

		ServerConfig server;
		std::optional<Credentials> security;
	};

	class RemoteBackend final: public IBackend
	{
	public:
		static constexpr std::size_t DEFAULT_THREADS = 4;

		explicit RemoteBackend(const RemoteBackendConfig& config, std::string token);
		~RemoteBackend() override;

		void connect() override;

		SessionInfo create_session(const std::string& name) override;
		void destroy_session(const common::UUID& session_uuid) override;
		std::vector<SessionInfo> list_sessions() override;

		utils::ProgressFuture<void> add_key(
				const common::UUID& session_uuid,
				common::SchemaType type, std::vector<std::byte>&& key_data) override;

		std::unique_ptr<storage::DataStorage> create_session_storage(Session& session) override;
		std::unique_ptr<executor::IExecutor> create_session_executor(Session& session) override;

		std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataFrame>>, std::shared_ptr<storage::DataFrame>> create_data_frame(
				const common::UUID& session_uuid, const std::string& name,
				const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
				std::size_t row_count,
				utils::MovableFunction<bool(std::vector<std::byte>&)> next_row,
				std::size_t partitions) override;

		std::vector<std::shared_ptr<storage::DataFrame>> list_data_frames(const common::UUID& session_uuid) override;

		executor::JobInfo schedule_job(const common::UUID& session_uuid, const common::ExecutionPlan& plan, uint32_t concurrency_limit) override;
		std::vector<executor::JobState> list_jobs(const common::UUID& session_uuid) override;
		executor::JobInfo describe_job(const common::UUID& session_uuid, const common::UUID& uuid) override;
		executor::JobState get_job_state(const common::UUID& session_uuid, const common::UUID& uuid) override;

	private:
		class RemoteBackendConnectionImpl;

		std::unique_ptr<utils::ThreadPool> pool_{std::make_unique<utils::ThreadPool>(DEFAULT_THREADS)};
		utils::PImpl<RemoteBackendConnectionImpl> pimpl_;
	};
}

#endif //LIBHERD_REMOTE_BACKEND_HPP
