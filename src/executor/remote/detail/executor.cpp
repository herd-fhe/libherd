#include "herd/executor/remote/detail/executor.hpp"

#include "herd/backend/remote/remote_backend.hpp"
#include "herd/executor/remote/detail/job.hpp"

namespace herd::executor::remote::detail
{
	std::shared_ptr<Job> Executor::schedule(const common::ExecutionPlan& plan, uint32_t concurrency_limit)
	{
		const auto job_info = backend_.schedule_job(session_.uuid(), plan, concurrency_limit);
		const auto job = std::make_shared<JobImpl>(job_info.uuid, plan, common::JobStatus::WAITING_FOR_EXECUTION, backend_);
		jobs_.try_emplace(job_info.uuid, job);

		return job;
	}

	std::vector<std::shared_ptr<Job>> Executor::list_jobs(bool pending_only)
	{
		sync_cache();

		std::vector<std::shared_ptr<Job>> output;
		for(const auto& [uuid, job]: jobs_)
		{
			if(!pending_only || (!job->ready() && !job->failed()))
			{
				output.emplace_back(job);
			}
		}

		return output;
	}

	void Executor::sync_cache() const
	{
		const auto remote_job_infos = backend_.list_jobs(session_.uuid());

		std::set<common::UUID> cache_job_uuids;
		for(const auto& [uuid, job]: jobs_)
		{
			cache_job_uuids.insert(uuid);
		}

		for(const auto& job_info: remote_job_infos)
		{
			if(!cache_job_uuids.contains(job_info.uuid))
			{
				const auto description = backend_.describe_job(session_.uuid(), job_info.uuid);
				jobs_.try_emplace(job_info.uuid, std::make_shared<JobImpl>(job_info.uuid, description.plan, job_info.status, backend_));
			}
			else
			{
				assert(jobs_.contains(job_info.uuid));
				const auto& job = jobs_[job_info.uuid];
				job->set_status(job_info.status);
			}
		}
	}

	Executor::Executor(herd::Session& session, herd::RemoteBackend& backend)
	:	session_(session), backend_(backend)
	{
	}
}