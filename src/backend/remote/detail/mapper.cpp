#include "herd/backend/remote/detail/mapper.hpp"


namespace herd::detail
{
	crypto::SchemaType to_model(proto::SchemaType type) noexcept
	{
		switch(type)
		{
			case proto::BINFHE:
				return crypto::SchemaType::BINFHE;
			default:
				assert(false && "Proto schema, model mismatch");
				return static_cast<crypto::SchemaType>(0);
		}
	}

	proto::SchemaType to_proto(crypto::SchemaType type)
	{
		switch(type)
		{
			case crypto::SchemaType::BINFHE:
				return proto::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}
}