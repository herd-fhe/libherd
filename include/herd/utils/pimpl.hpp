#ifndef LIBHERD_PIMPL_HPP
#define LIBHERD_PIMPL_HPP

#include <memory>


namespace herd::utils
{
	template<typename T>
	class PImpl
	{
	public:
		PImpl();
		template<typename ...Args>
		explicit PImpl(Args&&...);
		~PImpl() = default;

		PImpl(PImpl&&) = default;
		PImpl& operator=(PImpl&&) = default;

		const T* operator->() const;
		T* operator->();

		const T& operator*() const;
		T& operator*();

	private:
		std::unique_ptr<T> pimpl_;
	};

	template<typename T>
	PImpl<T>::PImpl()
		:pimpl_(std::make_unique<T>())
	{}

	template<typename T>
	template<typename... Args>
	PImpl<T>::PImpl(Args&&... args)
		:pimpl_{std::make_unique<T>(std::forward<Args>(args)...)}
	{}

	template<typename T>
	const T* PImpl<T>::operator->() const
	{
		return pimpl_.get();
	}

	template<typename T>
	T* PImpl<T>::operator->()
	{
		return pimpl_.get();
	}

	template<typename T>
	const T& PImpl<T>::operator*() const
	{
		return *pimpl_;
	}

	template<typename T>
	T& PImpl<T>::operator*()
	{
		return *pimpl_;
	}


}

#endif //LIBHERD_PIMPL_HPP
