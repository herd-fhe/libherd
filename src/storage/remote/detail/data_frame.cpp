#include "herd/storage/remote/detail/data_frame.hpp"

namespace herd::storage::remote::detail
{
	std::size_t DataFrameImpl::partitions() const
	{
		return partitions_;
	}

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
			common::UUID uuid, const std::string& name, std::size_t row_count, std::size_t partitions,
			const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
			RemoteBackend& remote_backend)
		:	DataFrame(uuid, name, columns),
		schema_type_(schema_type), partitions_(partitions), rows_count_(row_count), backend_(remote_backend)
	{
	}
}