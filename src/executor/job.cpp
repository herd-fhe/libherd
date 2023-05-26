#include "herd/executor/job.hpp"


namespace herd::executor
{

	Job::Job(const common::UUID& uuid, const common::ExecutionPlan& plan, common::JobStatus status)
	:	uuid_(uuid), plan_(plan), status_(status)
	{
	}

	const common::UUID& Job::uuid() const
	{
		return uuid_;
	}

	bool Job::ready() const noexcept
	{
		return status_ == common::JobStatus::COMPLETED;
	}


	bool Job::failed() const noexcept
	{
		return status_ == common::JobStatus::FAILED;
	}

	const common::ExecutionPlan& Job::plan() const noexcept
	{
		return plan_;
	}

	void Job::set_status(common::JobStatus status) noexcept
	{
		status_ = status;
	}
}