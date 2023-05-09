#ifndef LIBHERD_OUTPUT_NODE_HPP
#define LIBHERD_OUTPUT_NODE_HPP

#include "herd/translator/common/tree/node/abstract_node.hpp"


namespace herd::translator
{
	class OutputNode: public AbstractNode
	{
		std::shared_ptr<InputNode> input;

	public:
		[[nodiscard]] std::vector<std::shared_ptr<AbstractNode>> parents() const override
		{
			return {input};
		}

		[[nodiscard]] std::vector<std::weak_ptr<AbstractNode>> children() const override
		{
			return {};
		}

		[[nodiscard]] size_t cost() const noexcept override
		{
			return 1;
		}
	};
}

#endif //LIBHERD_OUTPUT_NODE_HPP
