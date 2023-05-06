#include "herd/data_storage/remote/remote_data_table.hpp"

namespace herd::storage
{
	size_t RemoteDataTable::size() const
	{
		return rows_count_;
	}

	bool RemoteDataTable::empty() const
	{
		return rows_count_ != 0;
	}

	common::SchemaType RemoteDataTable::schema_type() const
	{
		return schema_type_;
	}

	RemoteDataTable::RemoteDataTable(
			common::UUID uuid, const std::string& name, size_t row_count,
			const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
			RemoteBackend& remote_backend)
		: DataTable(name, columns),
		uuid_(uuid), schema_type_(schema_type), rows_count_(row_count), backend_(remote_backend)
	{

	}

	std::shared_ptr<RemoteDataTable> RemoteDataTable::make_shared(common::UUID uuid, const std::string& name, size_t row_count, const std::vector<ColumnParameters>& columns, common::SchemaType schema_type, RemoteBackend& remote_backend)
	{
		return std::make_shared<make_shared_enabler>(uuid, name, row_count, columns, schema_type, remote_backend);
	}
}