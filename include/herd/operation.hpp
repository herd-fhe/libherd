#ifndef LIBHERD_OPERATION_HPP
#define LIBHERD_OPERATION_HPP

#include <vector>
#include <cstdint>


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

	enum class OperandType: uint8_t
	{
		BINFHE
	};

	struct Operation
	{
		OperationType type;

		std::size_t cost;

		std::vector<OperandType> inputs;
		OperandType output;

		Operation(OperationType op_type, std::size_t op_cost, std::vector<OperandType> op_input, OperandType op_output)
		: type(op_type), cost(op_cost), inputs(std::move(op_input)), output(op_output) {};
	};
}


#endif //LIBHERD_OPERATION_HPP
