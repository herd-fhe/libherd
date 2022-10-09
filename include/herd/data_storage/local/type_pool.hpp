#ifndef LIBHERD_LOCAL_TYPE_POOL_HPP
#define LIBHERD_LOCAL_TYPE_POOL_HPP

#include <vector>


namespace herd::storage
{
	class ITypePool
	{
	public:
		using size_type = std::size_t;

		[[nodiscard]] virtual bool empty() const = 0;
		[[nodiscard]] virtual size_type size() const = 0;

		virtual ~ITypePool() = default;
	};

	template<typename CellType>
	class TypePool: public ITypePool
	{
	public:
		using value_type = CellType;
		using const_reference = const value_type&;

		void emplace_back(CellType&& value);

		[[nodiscard]] const_reference at(size_type pos) const;
		[[nodiscard]] const_reference operator[](size_type pos) const;

		[[nodiscard]] bool empty() const override;
		[[nodiscard]] size_type size() const override;
	private:
		std::vector<CellType> values_;
	};

	template<typename CellType>
	void TypePool<CellType>::emplace_back(CellType &&value)
	{
		values_.emplace_back(value);
	}

	template<typename CellType>
	typename TypePool<CellType>::const_reference TypePool<CellType>::at(TypePool::size_type pos) const
	{
		return values_.at(pos);
	}

	template<typename CellType>
	typename TypePool<CellType>::const_reference TypePool<CellType>::operator[](TypePool::size_type pos) const
	{
		return values_[pos];
	}

	template<typename CellType>
	bool TypePool<CellType>::empty() const
	{
		return values_.empty();
	}

	template<typename CellType>
	typename TypePool<CellType>::size_type TypePool<CellType>::size() const
	{
		return values_.size();
	}
}

#endif //LIBHERD_DS_LOCAL_TYPE_POOL_HPP
