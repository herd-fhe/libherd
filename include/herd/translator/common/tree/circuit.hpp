#ifndef LIBHERD_CIRCUIT_HPP
#define LIBHERD_CIRCUIT_HPP

#include <vector>

#include "herd/common/dag.hpp"
#include "herd/common/model/data_type.hpp"
#include "herd/translator/common/tree/node/nodes.hpp"


namespace herd::translator
{
	struct Circuit
	{
		std::vector<unsigned int> input;
		std::vector<unsigned int> output;

		common::DAG<node_t> circuit_graph;
	};
}

#endif //LIBHERD_CIRCUIT_HPP
