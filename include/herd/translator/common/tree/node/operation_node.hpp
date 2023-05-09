#ifndef LIBHERD_OPERATION_NODE_HPP
#define LIBHERD_OPERATION_NODE_HPP

#include <algorithm>
#include <cassert>
#include <memory>
#include <unordered_set>
#include <vector>

#include "herd/operation.hpp"
#include "herd/translator/common/tree/node/input_node.hpp"


namespace herd::translator
{
	class OperationNode: public InputNode
	{
		Operation operation_;
		std::vector<std::shared_ptr<InputNode>> inputs_;

	public:
		OperationNode(Operation operation, std::vector<std::shared_ptr<InputNode>> inputs)
		:	operation_(std::move(operation)), inputs_(std::move(inputs))
		{}

		[[nodiscard]] std::size_t cost() const noexcept override
		{
			return operation_.cost;
		}

		[[nodiscard]] common::SchemaType value_type() const noexcept override
		{
			return operation_.output;
		}

		[[nodiscard]] OperationType operation_type() const noexcept
		{
			return operation_.type;
		}
	};
}

#endif //LIBHERD_OPERATION_NODE_HPP
