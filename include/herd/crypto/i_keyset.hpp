#ifndef LIBHERD_I_KEYSET_HPP
#define LIBHERD_I_KEYSET_HPP

#include <vector>

#include "herd/crypto/schema.hpp"


namespace herd::crypto
{
	class IKeyset
	{
	public:
		[[nodiscard]] virtual SchemaType get_schema_type() const noexcept = 0;
		[[nodiscard]] virtual std::vector<std::byte> serialize_cloud_key() const = 0;

		virtual ~IKeyset() = default;
	};
}

#endif //LIBHERD_I_KEYSET_HPP
