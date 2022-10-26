#include <utility>

#include "herd/data_storage/data_table.hpp"


namespace herd::storage
{
	DataTable::DataTable(std::string name, const std::vector<ColumnParameters> &columns)
	:name_(std::move(name))
	{
		for(std::size_t i = 0; const auto& column: columns)
		{
			column_descriptors_.try_emplace(
				column.name,
				i,
			    column.type
			);

			++i;
		}
	}

	const std::string &DataTable::name() const
	{
		return name_;
	}

	const std::map<DataTable::column_key_type, DataTable::ColumnDescriptor, std::less<>>& DataTable::columns() const
	{
		return column_descriptors_;
	}
}