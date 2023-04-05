#ifndef LIBHERD_BINFHE_CIPHERTEXT_HPP
#define LIBHERD_BINFHE_CIPHERTEXT_HPP

#include <vector>

#include <binfhe/lwe-ciphertext-fwd.h>
#include <binfhecontext-ser.h>

#include "herd/crypto/ciphertext.hpp"


namespace herd::crypto::binfhe::detail
{
	class CiphertextImpl: public herd::crypto::Ciphertext
	{
	public:
		explicit CiphertextImpl(std::vector<lbcrypto::LWECiphertext> encrypted_bits) noexcept;

		void serialize(std::ostream& stream) const override;

	private:
		std::vector<lbcrypto::LWECiphertext> encrypted_bits_;
	};
}

#endif //LIBHERD_BINFHE_CIPHERTEXT_HPP
