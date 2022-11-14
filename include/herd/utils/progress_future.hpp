#ifndef LIBHERD_PROGRESS_FUTURE_HPP
#define LIBHERD_PROGRESS_FUTURE_HPP

#include <cassert>
#include <future>

namespace herd::utils
{
	namespace detail
	{
		struct ProgressFutureState
		{
			std::atomic_uint64_t current_step_{0};
			std::atomic_uint64_t max_step_{0};
		};
	}

	template<typename Signature>
	class ProgressPackagedTask;

	template<typename Res>
	class ProgressFuture
	{
	private:
		template<typename> friend class ProgressPackagedTask;

		std::future<Res> future_;
		std::shared_ptr<detail::ProgressFutureState> state_;

		ProgressFuture(std::future<Res>&& fut, std::shared_ptr<detail::ProgressFutureState> state)
			:future_(std::move(fut)), state_(std::move(state))
		{}

	public:
		[[nodiscard]] uint64_t max_step() const noexcept
		{
			return state_->max_step_;
		}

		[[nodiscard]] uint64_t current_step() const noexcept
		{
			return state_->current_step_;
		}

		[[nodiscard]] double progress() const noexcept
		{
			return (static_cast<double>(current_step()) / static_cast<double>(max_step()))*100.0;
		}

		[[nodiscard]] bool valid() const noexcept
		{
			return future_.valid();
		}

		void wait() const
		{
			return future_.wait();
		}

		template<typename Rep, typename Period>
		std::future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const
		{
			return future_.wait_for(timeout_duration);
		}

		template<typename Clock, typename Duration>
		std::future_status wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const
		{
			return future_.wait_until(timeout_time);
		}
	};

	template<typename Res, typename... Args>
	class ProgressPackagedTask<Res(Args...)>
	{
	public:
		class ProgressUpdateProxy
		{
		public:
			void step(uint64_t step_count) noexcept
			{
				state_->current_step_ += step_count;
			}

			void set_max_step(uint64_t max_step) noexcept
			{
				state_->max_step_.store(max_step, std::memory_order_release);
			}

			[[nodiscard]] uint64_t current_step() const noexcept
			{
				return state_->current_step_.load(std::memory_order_acquire);
			}

			[[nodiscard]] uint64_t max_step() const noexcept
			{
				return state_->max_step_.load(std::memory_order_acquire);
			}

		private:
			template<typename> friend class ProgressPackagedTask;

			explicit ProgressUpdateProxy(std::shared_ptr<detail::ProgressFutureState> state)
				:state_(std::move(state))
			{}

			std::shared_ptr<detail::ProgressFutureState> state_;
		};

		std::shared_ptr<detail::ProgressFutureState> state_;
		std::packaged_task<Res(ProgressUpdateProxy, Args...)> task_;

		ProgressPackagedTask() noexcept = default;

		template<typename Fun>
		requires std::is_invocable_r_v<Res, Fun, ProgressUpdateProxy&, Args...>
		         && (!std::is_same_v<ProgressPackagedTask, std::remove_cvref<Fun>>)
		explicit ProgressPackagedTask(Fun&& fun)
			:task_(std::forward<Fun>(fun))
		{
			state_ = std::make_shared<detail::ProgressFutureState>();
		}

		[[nodiscard]] bool valid() const noexcept
		{
			return task_.valid();
		}

		ProgressFuture<Res> get_future()
		{
			return ProgressFuture<Res>(task_.get_future(), state_);
		}

		void operator()(Args... args)
		{
			task_(ProgressUpdateProxy(state_), std::forward<Args>(args)...);
		}
	};
}

#endif //LIBHERD_PROGRESS_FUTURE_HPP
