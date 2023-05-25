#include "herd/translator/xlscc/detail/ir_transpiler.hpp"

#include <cassert>
#include <unordered_map>

namespace herd::translator::xlscc::detail
{
	common::Circuit transpile_ir_program(const ProgramDefinition& definition)
	{
		common::Circuit circuit;

		const auto& main = definition.main_function;
		circuit.input = main.header.first;
		circuit.output = main.header.second;

		std::unordered_map<unsigned int, unsigned int> indirect_input_access;
		std::unordered_map<unsigned int, std::vector<unsigned int>> indirect_output_access;
		std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator> id_node_map;

		auto& graph = circuit.circuit_graph;

		for(const auto& operation: main.operations)
		{
			const auto& args = operation.arguments;
			const auto& output = operation.output;

			switch(operation.type)
			{
				case OperationType::TUPLE_INDEX:
					indirect_input_access.emplace(output, args[0]);
					break;
				case OperationType::BIT_SLICE:
				{
					assert(args[2] == 1);
					const auto input_index = indirect_input_access[args[0]];
					const auto iterator = graph.emplace(common::InputNode{input_index, args[1]});
					id_node_map.try_emplace(output, iterator);
					break;
				}
				case OperationType::AND:
				case OperationType::OR:
				{
					const auto graph_operation = operation.type == OperationType::AND
														 ? common::Operation::AND
														 : common::Operation::OR;
					const auto iterator = graph.emplace(common::OperationNode{graph_operation});
					id_node_map.try_emplace(output, iterator);
					for(const auto arg: args)
					{
						assert(id_node_map.contains(arg));
						const auto& input = id_node_map[arg];
						graph.add_edge(input, iterator);
					}

					break;
				}
				case OperationType::NOT:
				{
						const auto iterator = graph.emplace(common::OperationNode{common::Operation::NOT});
						id_node_map.try_emplace(output, iterator);

						assert(id_node_map.contains(args[0]));
						const auto& input = id_node_map[args[0]];
						graph.add_edge(input, iterator);
						break;
				}
				case OperationType::LITERAL:
				{
					assert(args[0] == 0 || args[0] == 1);
					const bool value = args[0] == 1;
					const auto iterator = graph.emplace(common::ConstantNode{value});
					id_node_map.try_emplace(output, iterator);
					break;
				}
				case OperationType::CONCAT:
				{
					assert(!indirect_output_access.contains(output));
					indirect_output_access[output] = args;
					break;
				}
				case OperationType::RETURN:
				{
					for(const auto arg: args)
					{
						assert(indirect_output_access.contains(arg));
						const auto& value_bits = indirect_output_access[arg];
						for(const auto bit_id: value_bits)
						{
							assert(id_node_map.contains(bit_id));
							const auto& input = id_node_map[bit_id];
							graph.emplace(input, common::OutputNode{arg, bit_id});
						}
					}
					break;
				}
			}
		}

		return circuit;
	}
}
