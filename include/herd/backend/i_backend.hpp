#ifndef LIBHERD_I_BACKEND_HPP
#define LIBHERD_I_BACKEND_HPP

#include <vector>

#include "herd/common/uuid.hpp"
#include "herd/session/session.hpp"
#include "herd/utils/movable_function.hpp"


namespace herd
{
	class IBackend
	{
	public:
		virtual void connect() = 0;

		virtual SessionInfo create_session(const std::string& name) = 0;
		virtual void destroy_session(const common::UUID& uuid) = 0;
		virtual std::vector<SessionInfo> list_sessions() = 0;

		virtual utils::ProgressFuture<void> add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data) = 0;

		virtual std::unique_ptr<storage::DataStorage> create_session_storage(Session& session) = 0;
		virtual std::unique_ptr<executor::IExecutor> create_session_executor(Session& session) = 0;

		virtual std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataFrame>>, std::shared_ptr<storage::DataFrame>> create_data_frame(
				const common::UUID& session_uuid, const std::string& name,
				const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
				std::size_t row_count,
				utils::MovableFunction<bool(std::vector<std::byte>&)> next_row,
				std::size_t partitions
		) = 0;

		virtual std::vector<std::shared_ptr<storage::DataFrame>> list_data_frames(const common::UUID& session_uuid) = 0;

		virtual executor::JobInfo schedule_job(const common::UUID& session_uuid, const common::ExecutionPlan& plan, uint32_t concurrency_limit) = 0;

		virtual std::vector<executor::JobState> list_jobs(const common::UUID& session_uuid) = 0;
		virtual executor::JobInfo describe_job(const common::UUID& session_uuid, const common::UUID& uuid) = 0;
		virtual executor::JobState get_job_state(const common::UUID& session_uuid, const common::UUID& uuid) = 0;


		virtual ~IBackend() = default;
	};
}

#endif //LIBHERD_I_BACKEND_HPP
