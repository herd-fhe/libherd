#include "herd/data_storage/local/local_data_storage.hpp"

#include <memory>

#include "herd/data_storage/local/local_data_table.hpp"
#include "herd/utils/csv_reader.hpp"


namespace herd::storage
{
	std::shared_ptr<DataTable> LocalDataStorage::create_table(std::string name, const std::vector<DataTable::ColumnParameters>& columns)
	{
		return LocalDataTable::make_shared(std::move(name), columns);
	}
}
