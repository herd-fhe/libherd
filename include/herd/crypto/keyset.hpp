#ifndef LIBHERD_KEYSET_HPP
#define LIBHERD_KEYSET_HPP

#include <vector>
#include <filesystem>

#include "herd/common/model/schema_type.hpp"


namespace herd::crypto
{
	class Keyset
	{
	public:
		[[nodiscard]] virtual common::SchemaType get_schema_type() const noexcept = 0;
		[[nodiscard]] virtual std::vector<std::byte> cloud_key_to_bytes() const = 0;
		[[nodiscard]] virtual std::vector<std::byte> private_key_to_bytes() const = 0;
		void store_private_key_to_file(const std::filesystem::path& path) const;

		virtual ~Keyset() = default;
	};
}

#endif //LIBHERD_KEYSET_HPP
