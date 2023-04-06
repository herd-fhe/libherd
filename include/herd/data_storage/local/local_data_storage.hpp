#ifndef LIBHERD_LOCAL_DATA_STORAGE_HPP
#define LIBHERD_LOCAL_DATA_STORAGE_HPP

#include "herd/data_storage/data_storage.hpp"


namespace herd::storage
{
	class LocalDataStorage : public DataStorage
	{
	private:
		std::pair<utils::ProgressFuture<std::shared_ptr<DataTable>>, std::shared_ptr<DataTable>> populate_table_from_csv(std::istream& stream, std::string name, const std::vector<DataTable::ColumnParameters>& columns, common::SchemaType schema_type) override;
	};
}
#endif //LIBHERD_LOCAL_DATA_STORAGE_HPP
