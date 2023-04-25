#ifndef LIBHERD_MAPPER_HPP
#define LIBHERD_MAPPER_HPP

#include <stdexcept>

#include <common.pb.h>
#include <storage.pb.h>

#include "herd/data_storage/data_frame.hpp"
#include "herd/common/model/data_type.hpp"
#include "herd/common/model/schema_type.hpp"


namespace herd
{

	struct MappingError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	namespace mapper
	{
		[[nodiscard]] common::SchemaType to_model(proto::SchemaType type);
		[[nodiscard]] common::DataType to_model(proto::DataType data_type);
		[[nodiscard]] std::vector<storage::DataFrame::ColumnParameters> to_model(const google::protobuf::RepeatedPtrField<proto::ColumnDescriptor>& columns);

		[[nodiscard]] proto::SchemaType to_proto(common::SchemaType type);
		[[nodiscard]] proto::DataType to_proto(common::DataType data_type);
		[[nodiscard]] google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<storage::DataFrame::ColumnParameters>& columns);
	}
}

#endif //LIBHERD_MAPPER_HPP
