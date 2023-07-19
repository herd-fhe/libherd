#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "herd/translator/xlscc/detail/ir_transpiler.hpp"


TEST(XlsccIrTranspiler, transpile_ir_program)
{
	using namespace herd::translator::xlscc::detail;

	ProgramDefinition definition;
	definition.main_function.header = {
			{{8}},
			{8, 8}
	};

	definition.main_function.operations = {
			OperationDefinition{OperationType::TUPLE_INDEX, {"input", 0U}, 50},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 7U, 1U}, 58},
			OperationDefinition{OperationType::LITERAL, {1U}, 47},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 6U, 1U}, 57},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 5U, 1U}, 56},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 4U, 1U}, 55},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 3U, 1U}, 54},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 2U, 1U}, 53},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 1U, 1U}, 52},
			OperationDefinition{OperationType::BIT_SLICE, {50U, 0U, 1U}, 51},
			OperationDefinition{OperationType::LITERAL, {0U}, 48},
			OperationDefinition{OperationType::AND, {58U, 47U}, 74},
			OperationDefinition{OperationType::AND, {57U, 47U}, 73},
			OperationDefinition{OperationType::AND, {56U, 47U}, 72},
			OperationDefinition{OperationType::AND, {55U, 47U}, 71},
			OperationDefinition{OperationType::AND, {54U, 47U}, 70},
			OperationDefinition{OperationType::AND, {53U, 47U}, 69},
			OperationDefinition{OperationType::AND, {52U, 47U}, 68},
			OperationDefinition{OperationType::AND, {51U, 47U}, 67},
			OperationDefinition{OperationType::OR, {58U, 48U}, 66},
			OperationDefinition{OperationType::OR, {57U, 47U}, 65},
			OperationDefinition{OperationType::OR, {56U, 48U}, 64},
			OperationDefinition{OperationType::OR, {55U, 47U}, 63},
			OperationDefinition{OperationType::OR, {54U, 48U}, 62},
			OperationDefinition{OperationType::OR, {53U, 47U}, 61},
			OperationDefinition{OperationType::OR, {52U, 48U}, 60},
			OperationDefinition{OperationType::OR, {51U, 47U}, 59},
			OperationDefinition{OperationType::NOT, {74U}, 82},
			OperationDefinition{OperationType::NOT, {73U}, 81},
			OperationDefinition{OperationType::NOT, {72U}, 80},
			OperationDefinition{OperationType::NOT, {71U}, 79},
			OperationDefinition{OperationType::NOT, {70U}, 78},
			OperationDefinition{OperationType::NOT, {69U}, 77},
			OperationDefinition{OperationType::NOT, {68U}, 76},
			OperationDefinition{OperationType::NOT, {67U}, 75},
			OperationDefinition{OperationType::CONCAT, {66U, 65U, 64U, 63U, 62U, 61U, 60U, 59U}, 83},
			OperationDefinition{OperationType::CONCAT, {82U, 81U, 80U, 79U, 78U, 77U, 76U, 75U}, 84},
			OperationDefinition{OperationType::RETURN, {83U, 84U}, 85},
	};

	const auto stage = transpile_ir_program(
			definition,
			ProgramMetadata{
					{
						{
							"input",
							{
								{"in", herd::common::DataType::UINT8}
							}
						}
					},
					{
							{"out_1", herd::common::DataType::UINT8},
							{"out_2", herd::common::DataType::UINT8}
					}
			}
		);

	const auto& circuit = stage.circuit_graph;

	EXPECT_EQ(circuit.source_nodes().size(), 10);
	EXPECT_EQ(circuit.sink_nodes().size(), 16);

	EXPECT_EQ(circuit.node_size(), 50);
	EXPECT_EQ(circuit.edges_size(), 56);

	ASSERT_EQ(stage.inputs.size(), 1);
	ASSERT_EQ(stage.inputs[0].size(), 1);
	EXPECT_EQ(stage.inputs[0][0], herd::common::DataType::UINT8);

	ASSERT_EQ(stage.output.size(), 2);
	EXPECT_EQ(stage.output[0].name, "out_1");
	EXPECT_EQ(stage.output[0].data_type, herd::common::DataType::UINT8);
	EXPECT_EQ(stage.output[1].name, "out_2");
	EXPECT_EQ(stage.output[1].data_type, herd::common::DataType::UINT8);
}