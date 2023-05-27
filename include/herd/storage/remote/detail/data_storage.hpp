#ifndef LIBHERD_REMOTE_DATA_STORAGE_HPP
#define LIBHERD_REMOTE_DATA_STORAGE_HPP

#include "herd/common/uuid.hpp"
#include "herd/data_storage/data_storage.hpp"


namespace herd
{
	class RemoteBackend;
	class Session;
}

namespace herd::storage
{
	class RemoteDataStorage: public DataStorage
	{
	public:
		[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<DataFrame>>& data_frames() const override;

		void sync_cache() const;

	private:
		friend class herd::RemoteBackend;
		struct make_unique_enabler;

		Session& session_;
		RemoteBackend& backend_;

		RemoteDataStorage(Session& session, RemoteBackend& backend);
		static std::unique_ptr<RemoteDataStorage> make_unique(
				Session& session, RemoteBackend& backend);

		[[nodiscard]] std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> populate_frame_from_csv(std::istream& stream, std::string name, const std::vector<DataFrame::ColumnParameters>& columns, common::SchemaType schema_type) override;
	};

	struct RemoteDataStorage::make_unique_enabler: public RemoteDataStorage
	{
		template<typename... Args>
		explicit make_unique_enabler(Args&&... args)
		:	RemoteDataStorage(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_DATA_STORAGE_HPP
