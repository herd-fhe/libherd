#ifndef LIBHERD_BINFHE_HPP
#define LIBHERD_BINFHE_HPP

#include <memory>

#include "herd/crypto/keyset.hpp"


namespace herd::crypto::binfhe
{
	std::unique_ptr<Keyset> create_binfhe_keyset();
}
#endif //LIBHERD_BINFHE_HPP
