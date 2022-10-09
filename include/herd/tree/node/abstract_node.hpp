#ifndef LIBHERD_ABSTRACT_NODE_HPP
#define LIBHERD_ABSTRACT_NODE_HPP

#include <cstdint>


namespace herd::tree
{
	class AbstractNode
	{
	public:
		AbstractNode() = default;

		AbstractNode(AbstractNode&&) = default;
		AbstractNode& operator=(AbstractNode&&) = default;

		AbstractNode(const AbstractNode&) = delete;
		AbstractNode& operator=(const AbstractNode&) = delete;

		virtual ~AbstractNode() = 0;

		[[nodiscard]] virtual std::vector<std::shared_ptr<AbstractNode>> parents() const = 0;
		[[nodiscard]] virtual std::vector<std::weak_ptr<AbstractNode>> children() const = 0;

		[[nodiscard]] virtual size_t cost() const noexcept = 0;
	};

	AbstractNode::~AbstractNode() = default;
}

#endif //LIBHERD_ABSTRACT_NODE_HPP
