#include "herd/executor/remote/job.hpp"

namespace herd::executor::remote
{
	JobImpl::JobImpl(const common::UUID& uuid, const common::ExecutionPlan& plan, common::JobStatus status, RemoteBackend& remote_backend)
	:	Job(uuid, plan, status),
		backend_(remote_backend)
	{
	}

	std::shared_ptr<JobImpl> JobImpl::make_shared(const common::UUID& uuid, const common::ExecutionPlan& plan, common::JobStatus status, RemoteBackend& remote_backend)
	{
		return std::make_shared<make_shared_enabler>(std::in_place, uuid, plan, status, remote_backend);
	}
}