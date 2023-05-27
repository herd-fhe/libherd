#ifndef LIBHERD_I_EXECUTOR_HPP
#define LIBHERD_I_EXECUTOR_HPP

#include <memory>

#include "herd/common/model/executor/execution_plan.hpp"
#include "herd/executor/job.hpp"

namespace herd::executor
{
	class IExecutor
	{
	public:
		virtual ~IExecutor() = default;

		[[nodiscard]] virtual std::shared_ptr<Job> schedule(const common::ExecutionPlan& plan) = 0;
		[[nodiscard]] virtual std::vector<std::shared_ptr<Job>> list_jobs(bool pending_only) = 0;
	};
}

#endif //LIBHERD_I_EXECUTOR_HPP
