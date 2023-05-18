#ifndef LIBHERD_I_TRANSLATOR_HPP
#define LIBHERD_I_TRANSLATOR_HPP

#include <memory>

#include "herd/translator/common/tree/circuit.hpp"


namespace herd::translator
{
	class ICompiler
	{
	public:

		virtual ~ICompiler() = default;

		virtual Circuit translate(std::string_view source) = 0;
	};
}

#endif //LIBHERD_I_TRANSLATOR_HPP
