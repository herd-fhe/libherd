#ifndef LIBHERD_IMPORT_OPTIONS_HPP
#define LIBHERD_IMPORT_OPTIONS_HPP

#include <vector>

#include "herd/common/model/column_meta.hpp"
#include "herd/common/model/schema_type.hpp"


namespace herd::storage
{
	struct ImportOptions
	{
		std::string name{""};

		std::vector<common::ColumnMeta> columns{};
		common::SchemaType schema = common::SchemaType::NONE;

		std::size_t partition_count{1};

		ImportOptions& set_name(std::string frame_name) noexcept;
		ImportOptions& set_columns(std::vector<common::ColumnMeta> data_frame_columns) noexcept;
		ImportOptions& set_schema(common::SchemaType schema_type) noexcept;
		ImportOptions& set_partitions(std::size_t count) noexcept;
	};
}

#endif //LIBHERD_IMPORT_OPTIONS_HPP
