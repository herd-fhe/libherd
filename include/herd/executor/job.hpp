#ifndef LIBHERD_JOB_HPP
#define LIBHERD_JOB_HPP

#include "herd/common/model/executor/execution_plan.hpp"
#include "herd/common/model/job.hpp"


namespace herd::executor
{
	struct JobInfo
	{
		common::UUID uuid;
		common::ExecutionPlan plan;
	};

	struct JobState
	{
		common::UUID uuid;
		common::JobStatus status;

		std::optional<unsigned long> current_stage;
		std::optional<std::string> message;
	};

	class Job
	{
	public:
		Job(const common::UUID& uuid, const common::ExecutionPlan& plan, common::JobStatus status);
		virtual ~Job() = default;

		[[nodiscard]] const common::UUID& uuid() const;

		[[nodiscard]] bool ready() const noexcept;
		[[nodiscard]] bool failed() const noexcept;
		[[nodiscard]] const common::ExecutionPlan& plan() const noexcept;

	protected:
		virtual void set_status(common::JobStatus status) noexcept;

	private:
		common::UUID uuid_;
		common::ExecutionPlan plan_;
		common::JobStatus status_;
	};
}

#endif //LIBHERD_JOB_HPP
