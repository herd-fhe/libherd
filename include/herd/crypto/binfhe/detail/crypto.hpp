#ifndef LIBHERD_BINFHE_CRYPTO_HPP
#define LIBHERD_BINFHE_CRYPTO_HPP

#include "herd/crypto/binfhe/detail/keyset.hpp"
#include "herd/crypto/i_crypto.hpp"


namespace herd::crypto::binfhe::detail
{
	class CryptoImpl final: public ICrypto
	{
	public:
		explicit CryptoImpl(IKeyset& keyset);

		std::unique_ptr<Ciphertext> encrypt(const std::vector<bool>& plaintext) const override;

	private:
		KeysetImpl& keyset_;
	};
}

#endif //LIBHERD_BINFHE_CRYPTO_HPP
