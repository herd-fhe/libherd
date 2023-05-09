#ifndef LIBHERD_INPUT_NODE_HPP
#define LIBHERD_INPUT_NODE_HPP

#include <unordered_set>

#include "herd/operation.hpp"
#include "herd/translator/common/tree/node/abstract_node.hpp"


namespace herd::translator
{
	class InputNode: public AbstractNode
	{
	public:
		[[nodiscard]] virtual common::SchemaType value_type() const noexcept = 0;

		[[nodiscard]] std::vector<std::weak_ptr<AbstractNode>> children() const override
		{
			return children_;
		}

	private:
		std::vector<std::weak_ptr<AbstractNode>> children_;

		void add_child(std::weak_ptr<AbstractNode> child)
		{
			children_.emplace_back(std::move(child));
		}
	};

	class IOInputNode: public InputNode
	{
	public:
		[[nodiscard]] std::vector<std::shared_ptr<AbstractNode>> parents() const override
		{
			return parents_;
		}

		[[nodiscard]] size_t cost() const noexcept override
		{
			return 1;
		}

	protected:
		std::vector<std::shared_ptr<AbstractNode>> parents_;
	};
}

#endif //LIBHERD_INPUT_NODE_HPP
