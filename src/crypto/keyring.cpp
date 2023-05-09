#include "herd/crypto/keyring.hpp"

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto
{
	void crypto::Keyring::add_keyset(std::unique_ptr<IKeyset> keyset)
	{
		common::SchemaType type = keyset->get_schema_type();
		keyring_.insert({type, std::move(keyset)});
	}

	bool Keyring::contains_key(common::SchemaType key_type) const noexcept
	{
		return keyring_.contains(key_type);
	}

	std::unique_ptr<IKeyset>& Keyring::get_keyset(common::SchemaType key_type)
	{
		return keyring_.at(key_type);
	}
}