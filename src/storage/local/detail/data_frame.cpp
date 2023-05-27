#include "herd/storage/local/detail/data_frame.hpp"

#include <cassert>
#include <type_traits>

#include "herd/storage/local/type_pool_factory.hpp"
#include "herd/utils/csv_reader.hpp"


namespace
{
	template<typename Derived, typename Base>
	Derived assert_cast(Base type)
		requires std::is_pointer_v<Base> || std::is_pointer_v<Derived>
	{
		auto derived = dynamic_cast<Derived>(type);
		assert(derived);

		return derived;
	}
}

namespace herd::storage::local::detail
{
	DataFrameImpl::DataFrameImpl(const common::UUID& uuid, std::string name, const std::vector<ColumnParameters>& columns)
	:	DataFrame(uuid, std::move(name), columns)
	{
		pools_.reserve(columns.size());

		for(const auto& column: columns)
		{
			pools_.emplace_back(TypePoolFactory::create_pool(column.type));
		}
	}

	size_t DataFrameImpl::size() const
	{
		if(pools_.empty())
		{
			return 0;
		}
		else
		{
			return pools_.front()->size();
		}
	}

	bool DataFrameImpl::empty() const
	{
		if(pools_.empty())
		{
			return true;
		}
		else
		{
			return pools_.front()->empty();
		}
	}

	void DataFrameImpl::add_row(const utils::CSVRow& row)
	{
		for(const auto& [name, column]: columns())
		{
			const column_index_type index = column.index;
			const column_type_key_type type_key = column.type;

			switch(type_key)
			{
				using enum common::DataType;

				case BIT:
					assert_cast<TypePool<bool>*>(pools_[index].get())->emplace_back(row.get<bool>(index));
					break;
				case UINT8:
					assert_cast<TypePool<uint8_t>*>(pools_[index].get())->emplace_back(row.get<uint8_t>(index));
					break;
				case UINT16:
					assert_cast<TypePool<uint16_t>*>(pools_[index].get())->emplace_back(row.get<uint16_t>(index));
					break;
				case UINT32:
					assert_cast<TypePool<uint32_t>*>(pools_[index].get())->emplace_back(row.get<uint32_t>(index));
					break;
				case UINT64:
					assert_cast<TypePool<uint64_t>*>(pools_[index].get())->emplace_back(row.get<uint64_t>(index));
					break;
				case INT8:
					assert_cast<TypePool<int8_t>*>(pools_[index].get())->emplace_back(row.get<int8_t>(index));
					break;
				case INT16:
					assert_cast<TypePool<int16_t>*>(pools_[index].get())->emplace_back(row.get<int16_t>(index));
					break;
				case INT32:
					assert_cast<TypePool<int32_t>*>(pools_[index].get())->emplace_back(row.get<int32_t>(index));
					break;
				case INT64:
					assert_cast<TypePool<int64_t>*>(pools_[index].get())->emplace_back(row.get<int64_t>(index));
			}
		}
	}

	void DataFrameImpl::flush_rows()
	{
		//do nothing
	}
}