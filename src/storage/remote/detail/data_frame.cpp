#include "herd/data_storage/remote/detail/remote_data_frame.hpp"

namespace herd::storage
{
	size_t RemoteDataFrame::size() const
	{
		return rows_count_;
	}

	bool RemoteDataFrame::empty() const
	{
		return rows_count_ != 0;
	}

	common::SchemaType RemoteDataFrame::schema_type() const
	{
		return schema_type_;
	}

	RemoteDataFrame::RemoteDataFrame(
			common::UUID uuid, const std::string& name, size_t row_count,
			const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
			RemoteBackend& remote_backend)
		:	DataFrame(uuid, name, columns),
		schema_type_(schema_type), rows_count_(row_count), backend_(remote_backend)
	{
	}

	std::shared_ptr<RemoteDataFrame> RemoteDataFrame::make_shared(common::UUID uuid, const std::string& name, size_t row_count, const std::vector<ColumnParameters>& columns, common::SchemaType schema_type, RemoteBackend& remote_backend)
	{
		return std::make_shared<make_shared_enabler>(uuid, name, row_count, columns, schema_type, remote_backend);
	}
}