#ifndef LIBHERD_MAPPER_HPP
#define LIBHERD_MAPPER_HPP

#include <common.pb.h>

#include <stdexcept>

#include "herd/crypto/schema.hpp"


namespace herd
{

	struct MappingError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	namespace detail
	{
		crypto::SchemaType to_model(proto::SchemaType type) noexcept;
		proto::SchemaType to_proto(crypto::SchemaType type);
	}
}

#endif //LIBHERD_MAPPER_HPP
