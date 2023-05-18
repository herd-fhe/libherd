#ifndef LIBHERD_NODES_HPP
#define LIBHERD_NODES_HPP

#include <cstdint>
#include <variant>

#include "herd/operation.hpp"


namespace herd::translator
{
	struct InputNode
	{
		unsigned int tuple_index;
		unsigned int bit_index;
	};

	struct OutputNode
	{
		unsigned int tuple_index;
		unsigned int bit_index;
	};

	struct ConstantNode
	{
		bool value;
	};

	struct OperationNode
	{
		Operation type;
	};

	using node_t = std::variant<InputNode, OutputNode, ConstantNode, OperationNode>;
}

#endif //LIBHERD_NODES_HPP
