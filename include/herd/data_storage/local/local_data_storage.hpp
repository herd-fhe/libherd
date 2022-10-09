#ifndef LIBHERD_LOCAL_DATA_STORAGE_HPP
#define LIBHERD_LOCAL_DATA_STORAGE_HPP

#include "herd/data_storage/data_storage.hpp"


namespace herd::storage
{
	class LocalDataStorage : public DataStorage
	{
	private:
		[[nodiscard]] std::shared_ptr<DataTable> create_table(std::string name, const std::vector<DataTable::ColumnParameters>& columns) override;

	};
}
#endif //LIBHERD_LOCAL_DATA_STORAGE_HPP
