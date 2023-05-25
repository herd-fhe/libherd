#ifndef LIBHERD_REMOTE_JOB_HPP
#define LIBHERD_REMOTE_JOB_HPP

#include <memory>

#include "herd/executor/job.hpp"


namespace herd
{
	class RemoteBackend;
}

namespace herd::executor::remote
{
	class JobImpl: public Job
	{
	private:
		friend class Executor;
		struct make_shared_enabler;

		RemoteBackend& backend_;

		JobImpl(
				const common::UUID& uuid, const common::ExecutionPlan& plan,
				common::JobStatus status,
				RemoteBackend& remote_backend);
		static std::shared_ptr<JobImpl> make_shared(
				const common::UUID& uuid, const common::ExecutionPlan& plan,
				common::JobStatus status,
				RemoteBackend& remote_backend);

		void sync_status(common::JobStatus status) noexcept;
	};

	struct JobImpl::make_shared_enabler: public JobImpl
	{
		template<typename... Args>
		explicit make_shared_enabler(std::in_place_t, Args&&... args)
		:	JobImpl(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_JOB_HPP
