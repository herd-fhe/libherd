#ifndef LIBHERD_TRANSLATOR_EXCEPTION_HPP
#define LIBHERD_TRANSLATOR_EXCEPTION_HPP

#include <stdexcept>


namespace herd::translator
{
	struct CompilerError: public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};
}

#endif //LIBHERD_TRANSLATOR_EXCEPTION_HPP
