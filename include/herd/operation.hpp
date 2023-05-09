#ifndef LIBHERD_OPERATION_HPP
#define LIBHERD_OPERATION_HPP

#include <cstdint>
#include <vector>

#include "herd/common/model/schema_type.hpp"


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

		std::vector<common::SchemaType> inputs;
		common::SchemaType output;

		Operation(OperationType op_type, std::size_t op_cost, std::vector<common::SchemaType> op_input, common::SchemaType op_output)
		:	type(op_type), cost(op_cost), inputs(std::move(op_input)), output(op_output){};
	};
}


#endif //LIBHERD_OPERATION_HPP
