#ifndef LIBHERD_TREE_DUMP_HPP
#define LIBHERD_TREE_DUMP_HPP

#include "herd/translator/common/tree/circuit_graph.hpp"
#include <circuit_graph.pb.h>


namespace herd
{
	proto::CircuitGraph dump_tree(const herd::translator::CircuitGraph& graph);
}


#endif //LIBHERD_TREE_DUMP_HPP
