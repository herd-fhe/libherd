#ifndef LIBHERD_MOVABLE_TASK_HPP
#define LIBHERD_MOVABLE_TASK_HPP

#include <utility>
#include <memory>


namespace herd::utils
{
	namespace detail
	{
		class MovableFunctionPimpl
		{
		public:
			virtual void invoke() = 0;
			virtual ~MovableFunctionPimpl() = default;
		};

		template<typename F>
		class TaskPimplImpl: public MovableFunctionPimpl
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

	class MovableFunction
	{
	public:
		MovableFunction() = default;

		MovableFunction(MovableFunction&&) = default;
		MovableFunction& operator=(MovableFunction&&) = default;

		template<typename F>
			requires (!std::same_as<std::decay_t<F>, MovableFunction>)
		MovableFunction(F&& fun)
			:
			pimpl_(make_pimpl(std::forward<F>(fun)))
		{}

		void operator()()
		{
			pimpl().invoke();
		}

	private:
		std::unique_ptr<detail::MovableFunctionPimpl> pimpl_;

		[[nodiscard]] detail::MovableFunctionPimpl& pimpl() noexcept
		{
			return *pimpl_;
		}

		[[nodiscard]] const detail::MovableFunctionPimpl& pimpl() const noexcept
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
