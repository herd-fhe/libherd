#include "herd/storage/import_options.hpp"


namespace herd::storage
{
	ImportOptions& ImportOptions::set_name(std::string frame_name) noexcept
	{
		name = std::move(frame_name);
		return *this;
	}

	ImportOptions& ImportOptions::set_columns(std::vector<common::ColumnMeta> data_frame_columns) noexcept
	{
		columns = std::move(data_frame_columns);
		return *this;
	}

	ImportOptions& ImportOptions::set_schema(common::SchemaType schema_type) noexcept
	{
		schema = schema_type;
		return *this;
	}

	ImportOptions& ImportOptions::set_partitions(std::size_t count) noexcept
	{
		partition_count = count;
		return *this;
	}
}