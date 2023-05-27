#include "herd/storage/remote/detail/data_frame.hpp"

namespace herd::storage::remote::detail
{
	size_t DataFrameImpl::size() const
	{
		return rows_count_;
	}

	bool DataFrameImpl::empty() const
	{
		return rows_count_ != 0;
	}

	common::SchemaType DataFrameImpl::schema_type() const
	{
		return schema_type_;
	}

	DataFrameImpl::DataFrameImpl(
			common::UUID uuid, const std::string& name, size_t row_count,
			const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
			RemoteBackend& remote_backend)
		:	DataFrame(uuid, name, columns),
		schema_type_(schema_type), rows_count_(row_count), backend_(remote_backend)
	{
	}
}