#include "herd/crypto/binfhe/binfhe.hpp"

#include "herd/crypto/binfhe/detail/keyset.hpp"


namespace herd::crypto::binfhe
{
	std::unique_ptr<Keyset> create_binfhe_keyset()
	{
		return std::make_unique<detail::KeysetImpl>();
	}
}