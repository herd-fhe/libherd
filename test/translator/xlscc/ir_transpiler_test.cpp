#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "herd/translator/xlscc/detail/ir_transpiler.hpp"


TEST(XlsccIrTranspiler, transpile_ir_program)
{
	using namespace herd::translator::xlscc::detail;

	ProgramDefinition definition;
	definition.main_function.header = {
			{8},
			{8, 8}
	};

	definition.main_function.operations = {
			OperationDefinition{OperationType::TUPLE_INDEX, {0}, 50},
			OperationDefinition{OperationType::BIT_SLICE, {50, 7, 1}, 58},
			OperationDefinition{OperationType::LITERAL, {1}, 47},
			OperationDefinition{OperationType::BIT_SLICE, {50, 6, 1}, 57},
			OperationDefinition{OperationType::BIT_SLICE, {50, 5, 1}, 56},
			OperationDefinition{OperationType::BIT_SLICE, {50, 4, 1}, 55},
			OperationDefinition{OperationType::BIT_SLICE, {50, 3, 1}, 54},
			OperationDefinition{OperationType::BIT_SLICE, {50, 2, 1}, 53},
			OperationDefinition{OperationType::BIT_SLICE, {50, 1, 1}, 52},
			OperationDefinition{OperationType::BIT_SLICE, {50, 0, 1}, 51},
			OperationDefinition{OperationType::LITERAL, {0}, 48},
			OperationDefinition{OperationType::AND, {58, 47}, 74},
			OperationDefinition{OperationType::AND, {57, 47}, 73},
			OperationDefinition{OperationType::AND, {56, 47}, 72},
			OperationDefinition{OperationType::AND, {55, 47}, 71},
			OperationDefinition{OperationType::AND, {54, 47}, 70},
			OperationDefinition{OperationType::AND, {53, 47}, 69},
			OperationDefinition{OperationType::AND, {52, 47}, 68},
			OperationDefinition{OperationType::AND, {51, 47}, 67},
			OperationDefinition{OperationType::OR, {58, 48}, 66},
			OperationDefinition{OperationType::OR, {57, 47}, 65},
			OperationDefinition{OperationType::OR, {56, 48}, 64},
			OperationDefinition{OperationType::OR, {55, 47}, 63},
			OperationDefinition{OperationType::OR, {54, 48}, 62},
			OperationDefinition{OperationType::OR, {53, 47}, 61},
			OperationDefinition{OperationType::OR, {52, 48}, 60},
			OperationDefinition{OperationType::OR, {51, 47}, 59},
			OperationDefinition{OperationType::NOT, {74}, 82},
			OperationDefinition{OperationType::NOT, {73}, 81},
			OperationDefinition{OperationType::NOT, {72}, 80},
			OperationDefinition{OperationType::NOT, {71}, 79},
			OperationDefinition{OperationType::NOT, {70}, 78},
			OperationDefinition{OperationType::NOT, {69}, 77},
			OperationDefinition{OperationType::NOT, {68}, 76},
			OperationDefinition{OperationType::NOT, {67}, 75},
			OperationDefinition{OperationType::CONCAT, {66, 65, 64, 63, 62, 61, 60, 59}, 83},
			OperationDefinition{OperationType::CONCAT, {82, 81, 80, 79, 78, 77, 76, 75}, 84},
			OperationDefinition{OperationType::RETURN, {83, 84}, 85},
	};

	const auto stage = transpile_ir_program(definition);

	const auto& circuit = stage.circuit_graph;

	EXPECT_EQ(circuit.source_nodes().size(), 10);
	EXPECT_EQ(circuit.sink_nodes().size(), 16);

	EXPECT_EQ(circuit.node_size(), 50);
	EXPECT_EQ(circuit.edges_size(), 56);
}