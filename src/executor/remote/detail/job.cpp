#include "herd/executor/remote/detail/job.hpp"

namespace herd::executor::remote::detail
{
	JobImpl::JobImpl(const common::UUID& uuid, const common::ExecutionPlan& plan, common::JobStatus status, RemoteBackend& remote_backend)
	:	Job(uuid, plan, status),
		backend_(remote_backend)
	{
	}
}