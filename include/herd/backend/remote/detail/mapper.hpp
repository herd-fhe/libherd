#ifndef LIBHERD_MAPPER_HPP
#define LIBHERD_MAPPER_HPP

#include <stdexcept>

#include <common.pb.h>

#include "herd_common/schema_type.hpp"


namespace herd
{

	struct MappingError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	namespace mapper
	{
		common::SchemaType to_model(proto::SchemaType type) noexcept;
		proto::SchemaType to_proto(common::SchemaType type);
	}
}

#endif //LIBHERD_MAPPER_HPP
