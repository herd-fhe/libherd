#include "herd/backend/remote/detail/mapper.hpp"


namespace herd::mapper
{
	common::SchemaType to_model(proto::SchemaType type) noexcept
	{
		switch(type)
		{
			case proto::BINFHE:
				return common::SchemaType::BINFHE;
			default:
				assert(false && "Proto schema, model mismatch");
				return static_cast<common::SchemaType>(0);
		}
	}

	proto::SchemaType to_proto(common::SchemaType type)
	{
		switch(type)
		{
			case common::SchemaType::BINFHE:
				return proto::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}
}