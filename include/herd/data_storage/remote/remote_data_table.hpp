#ifndef LIBHERD_REMOTE_DATA_TABLE_HPP
#define LIBHERD_REMOTE_DATA_TABLE_HPP

#include "herd/data_storage/data_table.hpp"
#include "herd/uuid.hpp"
#include "herd_common/schema_type.hpp"


namespace herd
{
	class RemoteBackend;
}

namespace herd::storage
{
	class RemoteDataTable: public DataTable
	{
	public:
		size_t size() const override;
		bool empty() const override;

		common::SchemaType schema_type() const;

	private:
		friend class herd::RemoteBackend;
		struct make_shared_enabler;

		common::SchemaType schema_type_;

		size_t rows_count_;

		RemoteBackend& backend_;

		bool uploaded_ = false;

		RemoteDataTable(
				UUID uuid, const std::string& name, size_t row_count,
				const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
				RemoteBackend& remote_backend);
		static std::shared_ptr<RemoteDataTable> make_shared(
				UUID uuid, const std::string& name, size_t row_count,
				const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
				RemoteBackend& remote_backend);
	};

	struct RemoteDataTable::make_shared_enabler: public RemoteDataTable
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
			:   RemoteDataTable(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_DATA_TABLE_HPP
