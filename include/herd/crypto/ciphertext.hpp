#ifndef LIBHERD_CIPHERTEXT_HPP
#define LIBHERD_CIPHERTEXT_HPP

#include <ostream>
#include <vector>

namespace herd::crypto
{
	class Ciphertext
	{
	public:
		virtual ~Ciphertext() = default;

		virtual void serialize(std::ostream& stream) const = 0;
	};
}

#endif //LIBHERD_CIPHERTEXT_HPP
