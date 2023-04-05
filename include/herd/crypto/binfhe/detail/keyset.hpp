#ifndef LIBHERD_BINFHE_KEYSET_HPP
#define LIBHERD_BINFHE_KEYSET_HPP

#include <binfhecontext.h>

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto::binfhe::detail
{
	class KeysetImpl: public IKeyset
	{
	public:
		KeysetImpl();

		[[nodiscard]] common::SchemaType get_schema_type() const noexcept override;
		[[nodiscard]] std::vector<std::byte> to_bytes() const override;

		const lbcrypto::BinFHEContext& context();
		const lbcrypto::LWEPrivateKey& private_key();

		void recreate_keys();

	private:
		lbcrypto::BinFHEContext context_;
		lbcrypto::LWEPrivateKey private_key_;

		lbcrypto::LWESwitchingKey switching_key_;
		lbcrypto::RingGSWACCKey refresh_key_;
	};
}

#endif //LIBHERD_BINFHE_KEYSET_HPP
