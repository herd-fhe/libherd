#include "herd/crypto/binfhe/detail/ciphertext.hpp"

#include <iterator>


namespace herd::crypto::binfhe::detail
{
	CiphertextImpl::CiphertextImpl(std::vector<lbcrypto::LWECiphertext> encrypted_bits) noexcept
		:	encrypted_bits_(std::move(encrypted_bits))
	{
	}

	void CiphertextImpl::serialize(std::ostream& stream) const
	{
		for(const auto& encrypted_bit: encrypted_bits_)
		{
			lbcrypto::Serial::Serialize(encrypted_bit, stream, lbcrypto::SerType::BINARY);
		}
	}
}