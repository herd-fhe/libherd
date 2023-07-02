#ifndef LIBHERD_I_CRYPTO_HPP
#define LIBHERD_I_CRYPTO_HPP

#include <memory>
#include <vector>

#include "herd/crypto/ciphertext.hpp"
#include "herd/crypto/keyset.hpp"


namespace herd::crypto
{
	class ICrypto
	{
	public:
		virtual ~ICrypto() = default;

		[[nodiscard]] virtual const Keyset& keyset() const noexcept = 0;
		[[nodiscard]] virtual std::unique_ptr<Ciphertext> encrypt(const std::vector<bool>& plaintext) const = 0;
	};
}

#endif //LIBHERD_I_CRYPTO_HPP
