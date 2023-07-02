#ifndef LIBHERD_KEYRING_HPP
#define LIBHERD_KEYRING_HPP

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "herd/common/model/schema_type.hpp"
#include "herd/crypto/keyset.hpp"


namespace herd::crypto
{
	struct KeyAlreadyAddedException: public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	class Keyring
	{
	public:
		void add_keyset(std::unique_ptr<Keyset> keyset);
		[[nodiscard]] std::unique_ptr<Keyset>& get_keyset(common::SchemaType key_type);
		[[nodiscard]] bool contains_key(common::SchemaType key_type) const noexcept;

	private:
		std::unordered_map<common::SchemaType, std::unique_ptr<Keyset>> keyring_;
	};
}

#endif //LIBHERD_KEYRING_HPP
