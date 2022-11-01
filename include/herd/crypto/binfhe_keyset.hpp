#ifndef LIBHERD_BINFHE_KEYSET_HPP
#define LIBHERD_BINFHE_KEYSET_HPP

#include <memory>

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto::binfhe
{
	std::unique_ptr<IKeyset> create_binfhe_keyset();
}

#endif //LIBHERD_BINFHE_KEYSET_HPP
