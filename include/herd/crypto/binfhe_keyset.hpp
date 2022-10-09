#ifndef LIBHERD_BINFHE_KEYSET_HPP
#define LIBHERD_BINFHE_KEYSET_HPP

#include <binfhecontext.h>

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto::binfhe
{
	namespace detail
	{
		struct CloudKeyData
		{
			std::shared_ptr<lbcrypto::LWESwitchingKey> switching_key;
			std::shared_ptr<lbcrypto::RingGSWBTKey> refresh_key;
		};
	}

	class Keyset: public BaseKeyset<lbcrypto::BinFHEContext, lbcrypto::LWEPrivateKey, detail::CloudKeyData>
	{
	public:
		Keyset();

		void recreate_keys();
	};
}

#endif //LIBHERD_BINFHE_KEYSET_HPP
