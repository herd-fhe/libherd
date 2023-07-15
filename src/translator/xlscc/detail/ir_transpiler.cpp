#include "herd/translator/xlscc/detail/ir_transpiler.hpp"
#include <cassert>

#include <unordered_map>

#include "herd/common/native_type_mapping.hpp"
#include "herd/translator/exception.hpp"
#include "herd/translator/xlscc/detail/source_analyzer.hpp"


namespace herd::translator::xlscc::detail
{
	common::Circuit transpile_ir_program(const ProgramDefinition& definition, const ProgramMetadata& metadata)
	{
		common::Circuit circuit;

		const auto& main = definition.main_function;

		if(main.header.first.size() != metadata.inputs[0].size() || main.header.second.size() != metadata.output.size())
		{
			throw CompilerError("Internal transpiler error. AST mismatch");
		}

		for(std::size_t i = 0; i < main.header.first.size(); ++i)
		{
			if(main.header.first[i] != common::data_type_to_bit_width(metadata.inputs[0][i].data_type))
			{
				throw CompilerError("Internal transpiler error. Structure type mismatch");
			}
		}
		for(std::size_t i = 0; i < main.header.second.size(); ++i)
		{
			if(main.header.second[i] != common::data_type_to_bit_width(metadata.output[i].data_type))
			{
				throw CompilerError("Internal transpiler error. Structure type mismatch");
			}
		}

		circuit.input.reserve(metadata.inputs[0].size());
		for(const auto& input_field: metadata.inputs[0])
		{
			circuit.input.emplace_back(input_field.data_type);
		}

		circuit.output.reserve(metadata.output.size());
		for(const auto& output_field: metadata.output)
		{
			circuit.output.emplace_back(output_field.name, output_field.data_type);
		}

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
				using enum herd::translator::xlscc::detail::OperationType;

				case TUPLE_INDEX:
					visit_tuple_index(args, output, indirect_input_access);
					break;
				case BIT_SLICE:
				{
					visit_bit_slice(args, output, graph, indirect_input_access, id_node_map);
					break;
				}
				case AND:
				case OR:
				{
					visit_binary(args, output, operation.type, graph, id_node_map);
					break;
				}
				case NOT:
				{
					visit_unary(args, output, operation.type, graph, id_node_map);
					break;
				}
				case LITERAL:
				{
					visit_literal(args, output, graph, id_node_map);
					break;
				}
				case CONCAT:
				{
					visit_concat(args, output, indirect_output_access);
					break;
				}
				case RETURN:
				{
					visit_return(args, graph, id_node_map, indirect_output_access);
					break;
				}
			}
		}

		return circuit;
	}

	void visit_tuple_index(
			const std::vector<unsigned int>& args, unsigned int output,
			std::unordered_map<unsigned int, unsigned int>& indirect_input_access
	)
	{
		indirect_input_access.emplace(output, args[0]);
	}

	void visit_bit_slice(
			const std::vector<unsigned int>& args, unsigned int output,
			common::DAG<common::node_t>& graph, const std::unordered_map<unsigned int, unsigned int>& indirect_input_access,
			std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(args[2] == 1);
		const auto input_index = indirect_input_access.at(args[0]);
		const auto iterator = graph.emplace(common::InputNode{input_index, args[1]});
		id_node_map.try_emplace(output, iterator);
	}

	void visit_binary(
			const std::vector<unsigned int>& args, unsigned int output, OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map)
	{
		const auto graph_operation = type == OperationType::AND
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
	}

	void visit_unary(
			const std::vector<unsigned int>& args, unsigned int output, OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(type == OperationType::NOT);
		const auto iterator = graph.emplace(common::OperationNode{common::Operation::NOT});
		id_node_map.try_emplace(output, iterator);

		assert(id_node_map.contains(args[0]));
		const auto& input = id_node_map[args[0]];
		graph.add_edge(input, iterator);
	}

	void visit_literal(
			const std::vector<unsigned int>& args, unsigned int output,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(args[0] == 0 || args[0] == 1);
		const bool value = args[0] == 1;
		const auto iterator = graph.emplace(common::ConstantNode{value});
		id_node_map.try_emplace(output, iterator);
	}

	void visit_concat(
			const std::vector<unsigned int>& args, unsigned int output,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	)
	{
		assert(!indirect_output_access.contains(output));
		indirect_output_access[output] = args;
	}

	void visit_return(
			const std::vector<unsigned int>& args,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	)
	{
		for(auto tuple_index = static_cast<unsigned int>(args.size()); const auto arg: args)
		{
			assert(indirect_output_access.contains(arg));
			const auto& value_bits = indirect_output_access[arg];
			--tuple_index;
			for(auto bit_index = static_cast<unsigned int>(value_bits.size()); const auto bit_id: value_bits)
			{
				assert(id_node_map.contains(bit_id));
				const auto& input = id_node_map[bit_id];
				--bit_index;
				graph.emplace(input, common::OutputNode{tuple_index, bit_index});

			}
		}
	}
}
