#include "herd/backend/remote/remote_backend.hpp"

#include <grpcpp/grpcpp.h>

#include <utility>

#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"
#include "herd/storage/remote/detail/data_storage.hpp"
#include "herd/executor/remote/detail/executor.hpp"


namespace herd
{
	RemoteBackend::RemoteBackend(const RemoteBackendConfig& config, std::string token)
	:	pool_(std::make_unique<utils::ThreadPool>(DEFAULT_THREADS)), pimpl_(*this, *pool_, config, std::move(token))
	{
	}

	void RemoteBackend::connect()
	{
		pimpl_->connect();
	}

	SessionInfo RemoteBackend::create_session(const std::string& name)
	{
		return pimpl_->create_session(name);
	}

	void RemoteBackend::destroy_session(const common::UUID& session_uuid)
	{
		return pimpl_->destroy_session(session_uuid);
	}

	std::vector<SessionInfo> RemoteBackend::list_sessions()
	{
		return pimpl_->list_sessions();
	}

	utils::ProgressFuture<void> RemoteBackend::add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data)
	{
		return pimpl_->add_key(session_uuid, type, std::move(key_data));
	}

	std::unique_ptr<storage::DataStorage> RemoteBackend::create_session_storage(Session& session)
	{
		return std::make_unique<storage::remote::detail::DataStorageImpl>(session, *this);
	}

	std::unique_ptr<executor::IExecutor> RemoteBackend::create_session_executor(Session& session)
	{
		return std::make_unique<executor::remote::detail::Executor>(session, *this);
	}

	std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataFrame>>, std::shared_ptr<storage::DataFrame>> RemoteBackend::create_data_frame(const common::UUID& session_uuid, const std::string& name, const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type, std::size_t row_count, utils::MovableFunction<bool(std::vector<std::byte>&)> next_row)
	{
		return pimpl_->create_data_frame(session_uuid, name, columns, schema_type, row_count, std::move(next_row));
	}
	
	std::vector<std::shared_ptr<storage::DataFrame>> RemoteBackend::list_data_frames(const common::UUID& session_uuid)
	{
		return pimpl_->list_data_frames(session_uuid);
	}

	executor::JobInfo RemoteBackend::schedule_job(const common::UUID& session_uuid, const common::ExecutionPlan& plan)
	{
		return pimpl_->schedule_job(session_uuid, plan);
	}

	std::vector<executor::JobState> RemoteBackend::list_jobs(const common::UUID& session_uuid)
	{
		return pimpl_->list_jobs(session_uuid);
	}

	executor::JobInfo RemoteBackend::describe_job(const common::UUID& session_uuid, const common::UUID& uuid)
	{
		return pimpl_->describe_job(session_uuid, uuid);
	}

	executor::JobState RemoteBackend::get_job_state(const common::UUID& session_uuid, const common::UUID& uuid)
	{
		return pimpl_->get_job_state(session_uuid, uuid);
	}

	RemoteBackend::~RemoteBackend() = default;
}