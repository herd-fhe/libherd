#include "herd/crypto/keyring.hpp"

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto
{
	void crypto::Keyring::add_keyset(std::unique_ptr<IKeyset>&& keyset)
	{
		const auto index = std::type_index(typeid(keyset));

		keyring_.insert({index, std::move(keyset)});
	}

	bool Keyring::contains_key(const std::type_index& key_type) const noexcept
	{
		return keyring_.contains(key_type);
	}

	const std::unique_ptr<IKeyset> &Keyring::get_keyset(const std::type_index &key_type) const
	{
		return keyring_.at(key_type);
	}
}