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

		if(main.header.first.size() != metadata.inputs.size() || main.header.second.size() != metadata.output.size())
		{
			throw CompilerError("Internal transpiler error. AST mismatch");
		}

		for(std::size_t i = 0; i < main.header.first.size(); ++i)
		{
			if(main.header.first[i].size() != metadata.inputs[i].fields.size())
			{
				throw CompilerError("Internal transpiler error. AST mismatch");
			}
		}

		for(std::size_t i = 0; i < main.header.first.size(); ++i)
		{
			for(std::size_t j = 0; j < main.header.first[i].size(); ++j)
			{
				if(main.header.first[i][j] != common::data_type_to_bit_width(metadata.inputs[i].fields[j].data_type))
				{
					throw CompilerError("Internal transpiler error. Structure type mismatch");
				}
			}
		}

		for(std::size_t i = 0; i < main.header.second.size(); ++i)
		{
			if(main.header.second[i] != common::data_type_to_bit_width(metadata.output[i].data_type))
			{
				throw CompilerError("Internal transpiler error. Structure type mismatch");
			}
		}

		circuit.inputs.resize(metadata.inputs.size());
		for(std::size_t index = 0; const auto& input_struct: metadata.inputs)
		{
			for(const auto& input_field: input_struct.fields)
			{
				circuit.inputs[index].emplace_back(input_field.data_type);
			}
			++index;
		}

		circuit.output.reserve(metadata.output.size());
		for(const auto& output_field: metadata.output)
		{
			circuit.output.emplace_back(output_field.name, output_field.data_type);
		}

		std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> indirect_input_access;
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
					visit_tuple_index(args, output, indirect_input_access, metadata);
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
			const operation_arguments_t& args, unsigned int output,
			std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>>& indirect_input_access,
			const ProgramMetadata& metadata
	)
	{
		assert(args.size() == 2);
		assert(std::holds_alternative<std::string>(args[0]));
		assert(std::holds_alternative<unsigned int>(args[1]));

		const auto& tuple_name = std::get<std::string>(args[0]);
		std::size_t tuple_name_index = 0;
		for(const auto& tuple: metadata.inputs)
		{
			if(tuple.name == tuple_name)
			{
				break;
			}
			++tuple_name_index;
		}
		assert(tuple_name_index != metadata.inputs.size());

		indirect_input_access.emplace(output, std::make_pair(tuple_name_index, std::get<unsigned int>(args[1])));
	}

	void visit_bit_slice(
			const operation_arguments_t& args, unsigned int output,
			common::DAG<common::node_t>& graph, const std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>>& indirect_input_access,
			std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(args.size() == 3);
		assert(std::holds_alternative<unsigned int>(args[0]));
		assert(std::holds_alternative<unsigned int>(args[1]));
		assert(std::holds_alternative<unsigned int>(args[2]));
		assert(std::get<unsigned int>(args[2]) == 1);

		const auto [tuple_index, tuple_field_index] = indirect_input_access.at(std::get<unsigned int>(args[0]));
		const auto iterator = graph.emplace(common::InputNode{tuple_index, tuple_field_index, std::get<unsigned int>(args[1])});
		id_node_map.try_emplace(output, iterator);
	}

	void visit_binary(
			const operation_arguments_t& args, unsigned int output, OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map)
	{
		const auto graph_operation = type == OperationType::AND
											 ? common::Operation::AND
											 : common::Operation::OR;
		const auto iterator = graph.emplace(common::OperationNode{graph_operation});
		id_node_map.try_emplace(output, iterator);
		for(const auto& arg: args)
		{
			assert(std::holds_alternative<unsigned int>(arg));
			assert(id_node_map.contains(std::get<unsigned int>(arg)));
			const auto& input = id_node_map[std::get<unsigned int>(arg)];
			graph.add_edge(input, iterator);
		}
	}

	void visit_unary(
			const operation_arguments_t& args, unsigned int output, [[maybe_unused]] OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(type == OperationType::NOT);
		const auto iterator = graph.emplace(common::OperationNode{common::Operation::NOT});
		id_node_map.try_emplace(output, iterator);

		assert(args.size() == 1);
		assert(std::holds_alternative<unsigned int>(args[0]));
		assert(id_node_map.contains(std::get<unsigned int>(args[0])));
		const auto& input = id_node_map[std::get<unsigned int>(args[0])];
		graph.add_edge(input, iterator);
	}

	void visit_literal(
			const operation_arguments_t& args, unsigned int output,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	)
	{
		assert(args.size() == 1);
		assert(std::holds_alternative<unsigned int>(args[0]));
		assert(std::get<unsigned int>(args[0]) == 0 || std::get<unsigned int>(args[0]) == 1);
		const bool value = std::get<unsigned int>(args[0]) == 1;
		const auto iterator = graph.emplace(common::ConstantNode{value});
		id_node_map.try_emplace(output, iterator);
	}

	void visit_concat(
			const operation_arguments_t& args, unsigned int output,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	)
	{
		assert(!indirect_output_access.contains(output));
		indirect_output_access[output] = {};
		for(const auto& arg: args)
		{
			assert(std::holds_alternative<unsigned int>(arg));
			indirect_output_access[output].emplace_back(std::get<unsigned int>(arg));
		}
	}

	void visit_return(
			const operation_arguments_t& args,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	)
	{
		for(auto tuple_index = static_cast<unsigned int>(args.size()); const auto& arg: args)
		{
			assert(std::holds_alternative<unsigned int>(arg));
			assert(indirect_output_access.contains(std::get<unsigned int>(arg)));
			const auto& value_bits = indirect_output_access[std::get<unsigned int>(arg)];
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
