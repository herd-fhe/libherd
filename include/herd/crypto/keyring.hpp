#ifndef LIBHERD_KEYRING_HPP
#define LIBHERD_KEYRING_HPP

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "herd/crypto/i_keyset.hpp"
#include "herd/common/model/schema_type.hpp"


namespace herd::crypto
{
	struct KeyAlreadyAddedException: public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	class Keyring
	{
	public:
		void add_keyset(std::unique_ptr<IKeyset> keyset);
		[[nodiscard]] std::unique_ptr<IKeyset>& get_keyset(common::SchemaType key_type);
		[[nodiscard]] bool contains_key(common::SchemaType key_type) const noexcept;

	private:
		std::unordered_map<common::SchemaType, std::unique_ptr<IKeyset>> keyring_;
	};
}

#endif //LIBHERD_KEYRING_HPP
