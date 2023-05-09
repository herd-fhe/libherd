#ifndef LIBHERD_MOVABLE_TASK_HPP
#define LIBHERD_MOVABLE_TASK_HPP

#include <memory>
#include <utility>


namespace herd::utils
{
	namespace detail
	{
		class MovableTaskPimpl
		{
		public:
			virtual void invoke() = 0;
			virtual ~MovableTaskPimpl() = default;
		};

		template<typename F>
		class TaskPimplImpl: public MovableTaskPimpl
		{
		public:
			explicit TaskPimplImpl(F&& fun):
				fun_(std::move(fun)){};

			void invoke() final
			{
				fun_();
			}

		private:
			F fun_;
		};
	}

	class MovableTask
	{
	public:
		MovableTask() = default;

		MovableTask(MovableTask&&) = default;
		MovableTask& operator=(MovableTask&&) = default;

		template<typename F>
			requires(!std::same_as<std::decay_t<F>, MovableTask>)
		MovableTask(F&& fun)
		:	pimpl_(make_pimpl(std::forward<F>(fun)))
		{}

		void operator()()
		{
			pimpl().invoke();
		}

	private:
		std::unique_ptr<detail::MovableTaskPimpl> pimpl_;

		[[nodiscard]] detail::MovableTaskPimpl& pimpl() noexcept
		{
			return *pimpl_;
		}

		[[nodiscard]] const detail::MovableTaskPimpl& pimpl() const noexcept
		{
			return *pimpl_;
		}

		template<typename FF>
		static auto make_pimpl(FF&& fun)
		{
			using decay_FF = std::decay_t<FF>;
			using impl_t = detail::TaskPimplImpl<decay_FF>;
			return std::make_unique<impl_t>(std::forward<FF>(fun));
		}
	};
}

#endif //LIBHERD_MOVABLE_TASK_HPP
