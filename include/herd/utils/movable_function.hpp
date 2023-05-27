#ifndef LIBHERD_MOVABLE_FUNCTION_HPP
#define LIBHERD_MOVABLE_FUNCTION_HPP

#include <memory>
#include <utility>


namespace herd::utils
{
	namespace detail
	{
		template<typename Sig>
		class MovableFunctionIImpl;

		template<typename Ret, typename... Args>
		class MovableFunctionIImpl<Ret(Args...)>
		{
		public:
			virtual ~MovableFunctionIImpl() = default;
			virtual Ret invoke(Args&&... args) const = 0;
		};

		template<typename Fun, typename Sig>
		class MovableFunctionImpl;

		template<typename Fun, typename Ret, typename... Args>
		class MovableFunctionImpl<Fun, Ret(Args...)>: public MovableFunctionIImpl<Ret(Args...)>
		{
		public:
			template<typename T>
			MovableFunctionImpl(T&& fun)
			:	fun_(std::forward<T>(fun))
			{}

			Ret invoke(Args&&... args) const override
			{
				return fun_(std::forward<Args>(args)...);
			}

		private:
			Fun fun_;
		};

		template<typename Fun, typename... Args>
		class MovableFunctionImpl<Fun, void(Args...)>: private MovableFunctionIImpl<void(Args...)>
		{
		public:
			template<typename T>
			MovableFunctionImpl(T&& fun)
			:	fun_(std::forward<T>(fun))
			{}

			void invoke(Args&&... args) const override
			{
				fun_(std::forward<Args>(args)...);
			}

		private:
			Fun fun_;
		};
	}

	template<typename Sig>
	class MovableFunction;

	//https://stackoverflow.com/questions/25330716/move-only-version-of-stdfunction
	template<typename Ret, typename... Args>
	class MovableFunction<Ret(Args...)>
	{
	public:
		MovableFunction() = default;

		MovableFunction(MovableFunction&&) noexcept = default;
		MovableFunction& operator=(MovableFunction&&) noexcept = default;

		explicit operator bool() const
		{
			return static_cast<bool>(pimpl_);
		}

		Ret operator()(Args&&... args) const
		{
			return pimpl_->invoke(std::forward<Args>(args)...);
		}

		template<typename F>
			requires(!std::same_as<std::decay_t<F>, MovableFunction<Ret(Args...)>>)
		MovableFunction(F&& fun)
		:	pimpl_(make_pimpl(std::forward<F>(fun)))
		{}


	private:
		std::unique_ptr<detail::MovableFunctionIImpl<Ret(Args...)>> pimpl_;

		template<typename FF>
		static auto make_pimpl(FF&& fun)
		{
			using decay_FF = std::decay_t<FF>;
			using impl_t = detail::MovableFunctionImpl<decay_FF, Ret(Args...)>;
			return std::make_unique<impl_t>(std::forward<FF>(fun));
		}
	};
}

#endif //LIBHERD_MOVABLE_FUNCTION_HPP
