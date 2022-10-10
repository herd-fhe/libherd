#include "herd/data_storage/local/local_data_table.hpp"

#include <cassert>
#include <type_traits>

#include "herd/data_storage/local/type_pool_factory.hpp"
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

namespace herd::storage
{
	LocalDataTable::LocalDataTable(std::string name, const std::vector<ColumnParameters> &columns)
	: DataTable(std::move(name), columns)
	{
		pools_.reserve(columns.size());

		for(const auto& column: columns)
		{
			pools_.emplace_back(TypePoolFactory::create_pool(column.type));
		}
	}

	size_t LocalDataTable::size() const
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

	bool LocalDataTable::empty() const
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

	void LocalDataTable::add_row(const utils::CSVRow &row)
	{
		for(const auto& column: column_descriptors_)
		{
			const column_index_type index = column.second.index;
			const column_type_key_type type_key = column.second.type;

			switch(type_key)
			{
				case TypeKey::BIT:
					assert_cast<TypePool<bool> *>(pools_[index].get())->emplace_back(row.get<bool>(index));
					break;
				case TypeKey::UINT8:
					assert_cast<TypePool<uint8_t> *>(pools_[index].get())->emplace_back(row.get<uint8_t>(index));
					break;
				case TypeKey::UINT16:
					assert_cast<TypePool<uint16_t> *>(pools_[index].get())->emplace_back(row.get<uint16_t>(index));
					break;
				case TypeKey::UINT32:
					assert_cast<TypePool<uint32_t> *>(pools_[index].get())->emplace_back(row.get<uint32_t>(index));
					break;
				case TypeKey::UINT64:
					assert_cast<TypePool<uint64_t> *>(pools_[index].get())->emplace_back(row.get<uint64_t>(index));
					break;
				case TypeKey::INT8:
					assert_cast<TypePool<int8_t> *>(pools_[index].get())->emplace_back(row.get<int8_t>(index));
					break;
				case TypeKey::INT16:
					assert_cast<TypePool<int16_t> *>(pools_[index].get())->emplace_back(row.get<int16_t>(index));
					break;
				case TypeKey::INT32:
					assert_cast<TypePool<int32_t> *>(pools_[index].get())->emplace_back(row.get<int32_t>(index));
					break;
				case TypeKey::INT64:
					assert_cast<TypePool<int64_t> *>(pools_[index].get())->emplace_back(row.get<int64_t>(index));
			}
		}
	}

	void LocalDataTable::flush_rows()
	{
		//do nothing
	}

	std::shared_ptr<LocalDataTable> LocalDataTable::make_shared(std::string name, const std::vector<ColumnParameters>& columns)
	{
		return std::make_shared<make_shared_enabler>(std::move(name), columns);
	}
}