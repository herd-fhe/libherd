#ifndef LIBHERD_I_CRYPTO_HPP
#define LIBHERD_I_CRYPTO_HPP

#include <memory>
#include <vector>

#include "herd/crypto/ciphertext.hpp"


namespace herd::crypto
{
	class ICrypto
	{
	public:
		virtual ~ICrypto() = default;

		virtual std::unique_ptr<Ciphertext> encrypt(const std::vector<bool>& plaintext) const = 0;
	};
}

#endif //LIBHERD_I_CRYPTO_HPP
