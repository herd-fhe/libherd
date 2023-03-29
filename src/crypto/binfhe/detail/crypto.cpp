#include "herd/crypto/binfhe/detail/crypto.hpp"

#include <cassert>

#include <lwe-ciphertext-fwd.h>

#include "herd/crypto/binfhe/detail/ciphertext.hpp"


namespace herd::crypto::binfhe::detail
{
	namespace
	{
		KeysetImpl& validate_keyset(herd::crypto::IKeyset& keyset)
		{
			assert(keyset.get_schema_type() == common::SchemaType::BINFHE);
			return dynamic_cast<KeysetImpl&>(keyset);
		}
	}

	CryptoImpl::CryptoImpl(IKeyset& keyset)
	: keyset_(validate_keyset(keyset))
	{
	}

	std::unique_ptr<Ciphertext> CryptoImpl::encrypt(const std::vector<bool>& plaintext) const
	{
		std::vector<lbcrypto::LWECiphertext> encrypted_bits;
		encrypted_bits.reserve(plaintext.size());

		auto const& context = keyset_.context();
		auto const& private_key = keyset_.private_key();

		std::ranges::transform(
			plaintext, std::back_inserter(encrypted_bits),
			[&context, &private_key](bool bit)
			{
					return context.Encrypt(private_key, bit ? 1 : 0);
			}
        );

		return std::make_unique<CiphertextImpl>(std::move(encrypted_bits));
	}
}