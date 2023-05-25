#ifndef LIBHERD_REMOTE_EXECUTOR_HPP
#define LIBHERD_REMOTE_EXECUTOR_HPP

#include <unordered_map>

#include "herd/executor/i_executor.hpp"
#include "herd/executor/remote/job.hpp"


namespace herd
{
	class RemoteBackend;
	class Session;
}

namespace herd::executor::remote
{
	class Executor: public IExecutor
	{
	public:
		std::shared_ptr<Job> schedule(const common::ExecutionPlan& plan) override;
		std::vector<std::shared_ptr<Job>> list_jobs(bool pending_only) override;

		void sync_cache() const;

	private:
		friend class herd::RemoteBackend;
		struct make_unique_enabler;

		Session& session_;
		RemoteBackend& backend_;

		Executor(Session& session, RemoteBackend& backend);
		static std::unique_ptr<Executor> make_unique(
				Session& session, RemoteBackend& backend
		);

		mutable std::unordered_map<common::UUID, std::shared_ptr<JobImpl>> jobs_;
	};

	struct Executor::make_unique_enabler: public Executor
	{
		template<typename... Args>
		explicit make_unique_enabler(Args&&... args)
		:	Executor(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_EXECUTOR_HPP
