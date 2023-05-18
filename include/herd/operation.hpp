#ifndef LIBHERD_OPERATION_HPP
#define LIBHERD_OPERATION_HPP

#include <cstdint>
#include <vector>

#include "herd/common/model/schema_type.hpp"


namespace herd
{
	enum class Operation : uint8_t
	{
		AND,
		OR,
		NOT,
		XOR,
		NAND,
		MUX,
	};
}


#endif //LIBHERD_OPERATION_HPP
