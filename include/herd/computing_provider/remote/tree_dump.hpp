#ifndef LIBHERD_TREE_DUMP_HPP
#define LIBHERD_TREE_DUMP_HPP

#include "herd/tree/circuit_graph.hpp"
#include "circuit_graph.pb.h"


namespace herd
{
	proto::CircuitGraph dump_tree(const herd::tree::CircuitGraph& graph);
}


#endif //LIBHERD_TREE_DUMP_HPP
