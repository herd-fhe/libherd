#ifndef LIBHERD_REMOTE_DATA_STORAGE_HPP
#define LIBHERD_REMOTE_DATA_STORAGE_HPP

#include "herd/common/uuid.hpp"
#include "herd/storage/data_storage.hpp"


namespace herd
{
	class RemoteBackend;
	class Session;
}

namespace herd::storage::remote::detail
{
	class DataStorageImpl: public DataStorage
	{
	public:
		DataStorageImpl(Session& session, RemoteBackend& backend);

		[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<DataFrame>>& data_frames() const override;

		void sync_cache() const;

	private:
		friend class herd::RemoteBackend;

		Session& session_;
		RemoteBackend& backend_;

		[[nodiscard]] std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> populate_frame_from_csv(std::istream& stream,
																																	   std::string name,
																																	   const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
																																	   std::size_t partitions) override;
	};
}

#endif //LIBHERD_REMOTE_DATA_STORAGE_HPP
