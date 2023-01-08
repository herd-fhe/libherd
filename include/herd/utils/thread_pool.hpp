#ifndef LIBHERD_THREAD_POOL_HPP
#define LIBHERD_THREAD_POOL_HPP

#include <concepts>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "herd/utils/movable_task.hpp"
#include "herd/utils/progress_future.hpp"


namespace herd::utils
{
	struct ThreadPoolClosed: public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	class ThreadPool
	{
	public:
		static constexpr auto TIMEOUT = std::chrono::seconds(5);

		explicit ThreadPool(std::size_t max_thread_count = std::jthread::hardware_concurrency()) noexcept;

		~ThreadPool();

		template<typename Signature>
		void execute(ProgressPackagedTask<Signature>&& task)
		{
			std::unique_lock queue_lock(pool_mutex_);
			if(closed_)
			{
				throw ThreadPoolClosed("Internal queue already closed");
			}

			housekeeping();
			queue_.push(std::move(task));

			if(queue_.size() >= threads_.size() + 1 && threads_.size() < max_threads_)
			{
				const auto& thread_iter = threads_.emplace_back(std::make_unique<PooledThread>(*this));
				thread_iter->start();
			}
			else
			{
				queue_lock.unlock();
				consumers_cv_.notify_one();
			}
		}

	private:
		class PooledThread
		{
		private:
			std::atomic_flag ended_;
			std::jthread thread_;
			ThreadPool& pool_;

		public:
			explicit PooledThread(ThreadPool& pool) noexcept;

			void start();

			[[nodiscard]] bool ended() const noexcept;
			void join();

			static void thread_body(ThreadPool::PooledThread& thread, ThreadPool& pool);
		};

		enum class QueueOpState : uint8_t
		{
			SUCCESS,
			CLOSED,
			TIMEOUT
		};

		size_t max_threads_;

		bool closed_{false};
		mutable std::mutex pool_mutex_;
		std::condition_variable consumers_cv_;
		std::queue<MovableFunction> queue_;

		std::vector<std::unique_ptr<PooledThread>> threads_;

		void housekeeping();
	};
}

#endif //LIBHERD_THREAD_POOL_HPP
