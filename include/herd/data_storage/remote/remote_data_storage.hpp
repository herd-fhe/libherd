#ifndef LIBHERD_REMOTE_DATA_STORAGE_HPP
#define LIBHERD_REMOTE_DATA_STORAGE_HPP

#include "herd/data_storage/data_storage.hpp"
#include "herd/uuid.hpp"

namespace herd
{
	class RemoteBackend;
	class Session;
}

namespace herd::storage
{
	class RemoteDataStorage: public DataStorage
	{
	private:
		friend class herd::RemoteBackend;
		struct make_unique_enabler;

		Session& session_;
		RemoteBackend& backend_;

		explicit RemoteDataStorage(Session& session, RemoteBackend& backend);
		static std::unique_ptr<RemoteDataStorage> make_unique(
				Session& session, RemoteBackend& backend);

		std::pair<utils::ProgressFuture<std::shared_ptr<DataTable>>, std::shared_ptr<DataTable>> populate_table_from_csv(std::istream& stream, std::string name, const std::vector<DataTable::ColumnParameters> &columns, common::SchemaType schema_type) override;
	};

	struct RemoteDataStorage::make_unique_enabler: public RemoteDataStorage
	{
		template<typename... Args>
		requires std::constructible_from<RemoteDataStorage, Args...>
		explicit make_unique_enabler(Args&&... args)
			:   RemoteDataStorage(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_DATA_STORAGE_HPP
