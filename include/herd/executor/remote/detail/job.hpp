#ifndef LIBHERD_REMOTE_JOB_HPP
#define LIBHERD_REMOTE_JOB_HPP

#include <memory>

#include "herd/executor/job.hpp"


namespace herd
{
	class RemoteBackend;
}

namespace herd::executor::remote::detail
{
	class JobImpl: public Job
	{
	public:
		JobImpl(
				const common::UUID& uuid, const common::ExecutionPlan& plan,
				common::JobStatus status,
				RemoteBackend& remote_backend);

	private:
		friend class Executor;
		struct make_shared_enabler;

		RemoteBackend& backend_;

		void sync_status(common::JobStatus status) noexcept;
	};
}

#endif //LIBHERD_REMOTE_JOB_HPP
