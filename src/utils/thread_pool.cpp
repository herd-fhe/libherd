#include "herd/utils/thread_pool.hpp"

#include <algorithm>


namespace herd::utils
{
	ThreadPool::ThreadPool(std::size_t max_thread_count) noexcept
		:max_threads_(max_thread_count)
	{}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock queue_lock(pool_mutex_);
			closed_ = true;
		}
		consumers_cv_.notify_all();
		for(auto& thread: threads_)
		{
			thread->join();
		}
	}

	void ThreadPool::housekeeping()
	{
		auto to_remove = std::remove_if(
				std::begin(threads_), std::end(threads_),
				[](const auto& thread)
				{
					return thread->ended();
				}
		);

		std::for_each(to_remove, std::end(threads_), [](auto& thread) { thread->join(); });
		threads_.erase(to_remove, std::end(threads_));
	}

	ThreadPool::PooledThread::PooledThread(ThreadPool& pool) noexcept
		:pool_(pool)
	{}

	void ThreadPool::PooledThread::start()
	{
		thread_ = std::jthread([this, &pool=pool_]() {
			ThreadPool::PooledThread::thread_body((*this), pool);
		});
	}

	bool ThreadPool::PooledThread::ended() const noexcept
	{
		return ended_.test(std::memory_order_acquire);
	}

	void ThreadPool::PooledThread::join()
	{
		thread_.join();
	}
	void ThreadPool::PooledThread::thread_body(ThreadPool::PooledThread& thread, ThreadPool& pool)
	{
		MovableTask fun;
		while(true)
		{
			{
				std::unique_lock lock(pool.pool_mutex_);

				bool timed_out = !pool.consumers_cv_.wait_for(
						lock,
						TIMEOUT,
						[&queue=pool.queue_, &closed=pool.closed_]()
						{
							return !queue.empty() || closed;
						}
				);

				if(timed_out || pool.queue_.empty())
				{
					thread.ended_.test_and_set(std::memory_order_release);
					return;
				}
				else
				{
					fun = std::move(pool.queue_.front());
					pool.queue_.pop();
				}
			}

			fun();
		}
	}

}