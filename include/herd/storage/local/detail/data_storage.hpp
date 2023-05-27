#ifndef LIBHERD_LOCAL_DATA_STORAGE_HPP
#define LIBHERD_LOCAL_DATA_STORAGE_HPP

#include "herd/data_storage/data_storage.hpp"


namespace herd::storage::local::detail
{
	class DataStorageImpl: public DataStorage
	{
	private:
		std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> populate_frame_from_csv(std::istream& stream, std::string name, const std::vector<DataFrame::ColumnParameters>& columns, common::SchemaType schema_type) override;
	};
}
#endif //LIBHERD_LOCAL_DATA_STORAGE_HPP
