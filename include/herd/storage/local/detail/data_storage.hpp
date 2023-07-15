#ifndef LIBHERD_LOCAL_DATA_STORAGE_HPP
#define LIBHERD_LOCAL_DATA_STORAGE_HPP

#include "herd/storage/data_storage.hpp"


namespace herd::storage::local::detail
{
	class DataStorageImpl: public DataStorage
	{
	private:
		std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> populate_frame_from_csv(std::istream& stream,
																														 const std::string& name,
																														 const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
																														 std::size_t partitions) override;
	};
}
#endif //LIBHERD_LOCAL_DATA_STORAGE_HPP
