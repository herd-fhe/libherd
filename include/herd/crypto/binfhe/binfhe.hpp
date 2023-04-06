#ifndef LIBHERD_BINFHE_HPP
#define LIBHERD_BINFHE_HPP

#include <memory>

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto::binfhe
{
	std::unique_ptr<IKeyset> create_binfhe_keyset();
}
#endif //LIBHERD_BINFHE_HPP
