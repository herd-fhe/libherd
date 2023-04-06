#ifndef LIBHERD_I_KEYSET_HPP
#define LIBHERD_I_KEYSET_HPP

#include <vector>

#include "herd_common/schema_type.hpp"


namespace herd::crypto
{
	class IKeyset
	{
	public:
		[[nodiscard]] virtual common::SchemaType get_schema_type() const noexcept = 0;
		[[nodiscard]] virtual std::vector<std::byte> to_bytes() const = 0;

		virtual ~IKeyset() = default;
	};
}

#endif //LIBHERD_I_KEYSET_HPP
