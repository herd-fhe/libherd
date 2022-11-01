#ifndef LIBHERD_OPERATION_HPP
#define LIBHERD_OPERATION_HPP

#include <vector>
#include <cstdint>

#include "crypto/schema.hpp"


namespace herd
{
	enum class OperationType: uint8_t
	{
		AND,
		OR,
		NOT,
		XOR,
		NAND,
		MUX,
	};

	struct Operation
	{
		OperationType type;

		std::size_t cost;

		std::vector<crypto::SchemaType> inputs;
		crypto::SchemaType output;

		Operation(OperationType op_type, std::size_t op_cost, std::vector<crypto::SchemaType> op_input, crypto::SchemaType op_output)
		: type(op_type), cost(op_cost), inputs(std::move(op_input)), output(op_output) {};
	};
}


#endif //LIBHERD_OPERATION_HPP
