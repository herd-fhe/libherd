#include <gtest/gtest.h>

#include "herd/translator/xlscc/detail/compiler.hpp"
#include "herd/utils/temp_storage.hpp"


TEST(XlssTranslator, translate_simple_function)
{
	herd::utils::TempStorage storage;

	std::string sample = "struct out_output\n"
						 "{\n"
						 "    char a; char b;\n"
						 "};\n"
						 "struct in_input\n"
						 "{\n"
						 "    char a;\n"
						 "};\n"
						 "#pragma hls_top\n"
						 "out_output top_add3(in_input input)\n"
						 "{\n"
						 "    return {~input.a | 0x01, input.a | 0b01010101};\n"
						 "}";
	herd::translator::xlscc::detail::Compiler translator(storage);

	auto stage_circuit = translator.translate(sample);

	const auto& circuit = stage_circuit.circuit_graph;

	EXPECT_EQ(circuit.source_nodes().size(), 10);
}