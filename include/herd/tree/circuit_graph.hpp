#ifndef LIBHERD_CIRCUIT_GRAPH_HPP
#define LIBHERD_CIRCUIT_GRAPH_HPP

#include <vector>
#include <memory>

#include "herd/tree/node/input_node.hpp"
#include "herd/tree/node/output_node.hpp"


namespace herd::tree
{
	class CircuitGraph
	{
	public:
		[[nodiscard]] const std::vector<std::shared_ptr<AbstractNode>>& input_nodes() const
		{
			return in_nodes_;
		}

		[[nodiscard]] const std::vector<std::shared_ptr<AbstractNode>>& output_nodes() const
		{
			return out_nodes_;
		}

	private:
		std::vector<std::shared_ptr<AbstractNode>> in_nodes_;
		std::vector<std::shared_ptr<AbstractNode>> out_nodes_;
	};
}

#endif //LIBHERD_CIRCUIT_GRAPH_HPP