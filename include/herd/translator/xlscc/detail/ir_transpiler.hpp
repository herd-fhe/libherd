#ifndef LIBHERD_XLSCC_IR_TRANSPILER_HPP
#define LIBHERD_XLSCC_IR_TRANSPILER_HPP

#include "herd/common/model/circuit/circuit.hpp"
#include "herd/translator/xlscc/detail/ir_parser.hpp"
#include "source_analyzer.hpp"


namespace herd::translator::xlscc::detail
{
	[[nodiscard]] common::Circuit transpile_ir_program(const ProgramDefinition& definition, const herd::translator::xlscc::detail::ProgramMetadata& metadata);

	void visit_tuple_index(
			const std::vector<unsigned int>& args, unsigned int output,
			std::unordered_map<unsigned int, unsigned int>& indirect_input_access
	);
	void visit_bit_slice(
			const std::vector<unsigned int>& args, unsigned int output,
			common::DAG<common::node_t>& graph, const std::unordered_map<unsigned int, unsigned int>& indirect_input_access,
			std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	);
	void visit_binary(
			const std::vector<unsigned int>& args, unsigned int output, OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	);
	void visit_unary(
			const std::vector<unsigned int>& args, unsigned int output, OperationType type,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	);
	void visit_literal(
			const std::vector<unsigned int>& args, unsigned int output,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map
	);
	void visit_concat(
			const std::vector<unsigned int>& args, unsigned int output,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	);
	void visit_return(
			const std::vector<unsigned int>& args,
			common::DAG<common::node_t>& graph, std::unordered_map<unsigned int, decltype(common::Circuit::circuit_graph)::const_iterator>& id_node_map,
			std::unordered_map<unsigned int, std::vector<unsigned int>>& indirect_output_access
	);
}

#endif //LIBHERD_XLSCC_IR_TRANSPILER_HPP
