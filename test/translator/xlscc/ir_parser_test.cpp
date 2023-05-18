#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "herd/translator/xlscc/detail/ir_parser.hpp"


using namespace testing;

TEST(XlsccIrParser, parse_type_list_one_element)
{
	using namespace herd::translator::xlscc::detail;

	{
		EXPECT_THAT(parse_type_list("bits[32]"), ElementsAre(32));
	}

	{
		EXPECT_THAT(parse_type_list("bits[8]"), ElementsAre(8));
	}
}

TEST(XlsccIrParser, parse_type_list_two_elements)
{
	using namespace herd::translator::xlscc::detail;

	{
		EXPECT_THAT(parse_type_list("bits[32], bits[32]"), ElementsAre(32, 32));
	}

	{
		EXPECT_THAT(parse_type_list("bits[32], bits[16]"), ElementsAre(32, 16));
	}

	{
		EXPECT_THAT(parse_type_list("bits[16], bits[32]"), ElementsAre(16, 32));
	}

	{
		EXPECT_THAT(parse_type_list("bits[16], bits[32]"), ElementsAre(16, 32));
	}
}

TEST(XlsccIrParser, parse_type_list_many_elements)
{
	using namespace herd::translator::xlscc::detail;

	{
		EXPECT_THAT(parse_type_list("bits[32], bits[16], bits[32]"), ElementsAre(32, 16, 32));
	}

	{
		EXPECT_THAT(parse_type_list("bits[8], bits[16], bits[32], bits[16]"), ElementsAre(8, 16, 32, 16));
	}
}

TEST(XlsccIrParser, parse_type_list_one_ine_one_out)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto [input, output] = parse_function_header("top fn top_add3(input: (bits[32])) -> (bits[32]) {");

		EXPECT_THAT(input, ElementsAre(32));
		EXPECT_THAT(output, ElementsAre(32));
	}

	{
		const auto [input, output] = parse_function_header("top fn top_add3(input: (bits[8])) -> (bits[16]) {");

		EXPECT_THAT(input, ElementsAre(8));
		EXPECT_THAT(output, ElementsAre(16));
	}
}

TEST(XlsccIrParser, parse_function_one_named_argument)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto [arguments, output] = parse_function_one_named_argument("value=0, id=5");

		EXPECT_THAT(arguments, ElementsAre(0));
		EXPECT_EQ(output, 5);
	}

	{
		const auto [arguments, output] = parse_function_one_named_argument("input, index=1, id=34");

		EXPECT_THAT(arguments, ElementsAre(1));
		EXPECT_EQ(output, 34);
	}
}

TEST(XlsccIrParser, parse_function_bit_slice_statement_arguments)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto [arguments, output] = parse_function_bit_slice_statement_arguments("tuple_index.34, start=0, width=1, id=35");

		EXPECT_THAT(arguments, ElementsAre(34, 0, 1));
		EXPECT_EQ(output, 35);
	}

	{
		const auto [arguments, output] = parse_function_bit_slice_statement_arguments("tuple_index.25, start=0, width=1, id=26");

		EXPECT_THAT(arguments, ElementsAre(25, 0, 1));
		EXPECT_EQ(output, 26);
	}
}

TEST(XlsccIrParser, parse_function_variadic_statement_arguments)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto [arguments, output] = parse_function_variadic_statement_arguments("bit_slice.9, bit_slice.41, id=83");

		EXPECT_THAT(arguments, ElementsAre(9, 41));
		EXPECT_EQ(output, 83);
	}

	{
		const auto [arguments, output] = parse_function_variadic_statement_arguments("and.67, id=68");

		EXPECT_THAT(arguments, ElementsAre(67));
		EXPECT_EQ(output, 68);
	}

	{
		const auto [arguments, output] = parse_function_variadic_statement_arguments("and.353, and.344, and.335, and.326, and.317, and.308, and.299, and.290, id=356");

		EXPECT_THAT(arguments, ElementsAre(353, 344, 335, 326, 317, 308, 299, 290));
		EXPECT_EQ(output, 356);
	}
}

TEST(XlsccIrParser, parse_function_statement)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto definition = parse_function_statement("tuple_index.34: bits[32] = tuple_index(input, index=1, id=34)");

		EXPECT_EQ(definition.type, OperationType::TUPLE_INDEX);
		EXPECT_THAT(definition.arguments, ElementsAre(1));
		EXPECT_EQ(definition.output, 34);
	}

	{
		const auto definition = parse_function_statement("bit_slice.35: bits[1] = bit_slice(tuple_index.34, start=0, width=1, id=35)");

		EXPECT_EQ(definition.type, OperationType::BIT_SLICE);
		EXPECT_THAT(definition.arguments, ElementsAre(34, 0, 1));
		EXPECT_EQ(definition.output, 35);
	}

	{
		const auto definition = parse_function_statement("and.67: bits[1] = and(bit_slice.35, bit_slice.26, id=67)");

		EXPECT_EQ(definition.type, OperationType::AND);
		EXPECT_THAT(definition.arguments, ElementsAre(35, 26));
		EXPECT_EQ(definition.output, 67);
	}

	{
		const auto definition = parse_function_statement("or.69: bits[1] = or(bit_slice.35, bit_slice.26, id=69)");

		EXPECT_EQ(definition.type, OperationType::OR);
		EXPECT_THAT(definition.arguments, ElementsAre(35, 26));
		EXPECT_EQ(definition.output, 69);
	}

	{
		const auto definition = parse_function_statement("not.68: bits[1] = not(and.67, id=68)");

		EXPECT_EQ(definition.type, OperationType::NOT);
		EXPECT_THAT(definition.arguments, ElementsAre(67));
		EXPECT_EQ(definition.output, 68);
	}

	{
		const auto definition = parse_function_statement("literal.23: bits[1] = literal(value=0, id=23)");

		EXPECT_EQ(definition.type, OperationType::LITERAL);
		EXPECT_THAT(definition.arguments, ElementsAre(0));
		EXPECT_EQ(definition.output, 23);
	}

	{
		const auto definition = parse_function_statement("concat.360: bits[8] = concat(and.358, and.349, and.340, and.331, and.322, and.313, and.304, and.295, id=360)");

		EXPECT_EQ(definition.type, OperationType::CONCAT);
		EXPECT_THAT(definition.arguments, ElementsAre(358, 349, 340, 331, 322, 313, 304, 295));
		EXPECT_EQ(definition.output, 360);
	}

	{
		const auto definition = parse_function_statement("ret tuple.357: (bits[32], bits[32]) = tuple(concat.355, concat.356, id=357)");

		EXPECT_EQ(definition.type, OperationType::RETURN);
		EXPECT_THAT(definition.arguments, ElementsAre(355, 356));
		EXPECT_EQ(definition.output, 357);
	}
}

TEST(XlsccIrParser, parse_function)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto function = "top fn top_add3(input: (bits[8])) -> (bits[8], bits[8]) {\n"
							  "  tuple_index.50: bits[8] = tuple_index(input, index=0, id=50)\n"
							  "  bit_slice.58: bits[1] = bit_slice(tuple_index.50, start=7, width=1, id=58)\n"
							  "  literal.47: bits[1] = literal(value=1, id=47)\n"
							  "  bit_slice.57: bits[1] = bit_slice(tuple_index.50, start=6, width=1, id=57)\n"
							  "  bit_slice.56: bits[1] = bit_slice(tuple_index.50, start=5, width=1, id=56)\n"
							  "  bit_slice.55: bits[1] = bit_slice(tuple_index.50, start=4, width=1, id=55)\n"
							  "  bit_slice.54: bits[1] = bit_slice(tuple_index.50, start=3, width=1, id=54)\n"
							  "  bit_slice.53: bits[1] = bit_slice(tuple_index.50, start=2, width=1, id=53)\n"
							  "  bit_slice.52: bits[1] = bit_slice(tuple_index.50, start=1, width=1, id=52)\n"
							  "  bit_slice.51: bits[1] = bit_slice(tuple_index.50, start=0, width=1, id=51)\n"
							  "  literal.48: bits[1] = literal(value=0, id=48)\n"
							  "  and.74: bits[1] = and(bit_slice.58, literal.47, id=74)\n"
							  "  and.73: bits[1] = and(bit_slice.57, literal.47, id=73)\n"
							  "  and.72: bits[1] = and(bit_slice.56, literal.47, id=72)\n"
							  "  and.71: bits[1] = and(bit_slice.55, literal.47, id=71)\n"
							  "  and.70: bits[1] = and(bit_slice.54, literal.47, id=70)\n"
							  "  and.69: bits[1] = and(bit_slice.53, literal.47, id=69)\n"
							  "  and.68: bits[1] = and(bit_slice.52, literal.47, id=68)\n"
							  "  and.67: bits[1] = and(bit_slice.51, literal.48, id=67)\n"
							  "  or.66: bits[1] = or(bit_slice.58, literal.48, id=66)\n"
							  "  or.65: bits[1] = or(bit_slice.57, literal.47, id=65)\n"
							  "  or.64: bits[1] = or(bit_slice.56, literal.48, id=64)\n"
							  "  or.63: bits[1] = or(bit_slice.55, literal.47, id=63)\n"
							  "  or.62: bits[1] = or(bit_slice.54, literal.48, id=62)\n"
							  "  or.61: bits[1] = or(bit_slice.53, literal.47, id=61)\n"
							  "  or.60: bits[1] = or(bit_slice.52, literal.48, id=60)\n"
							  "  or.59: bits[1] = or(bit_slice.51, literal.47, id=59)\n"
							  "  not.82: bits[1] = not(and.74, id=82)\n"
							  "  not.81: bits[1] = not(and.73, id=81)\n"
							  "  not.80: bits[1] = not(and.72, id=80)\n"
							  "  not.79: bits[1] = not(and.71, id=79)\n"
							  "  not.78: bits[1] = not(and.70, id=78)\n"
							  "  not.77: bits[1] = not(and.69, id=77)\n"
							  "  not.76: bits[1] = not(and.68, id=76)\n"
							  "  not.75: bits[1] = not(and.67, id=75)\n"
							  "  concat.83: bits[8] = concat(or.66, or.65, or.64, or.63, or.62, or.61, or.60, or.59, id=83)\n"
							  "  concat.84: bits[8] = concat(not.82, not.81, not.80, not.79, not.78, not.77, not.76, not.75, id=84)\n"
							  "  ret tuple.85: (bits[8], bits[8]) = tuple(concat.83, concat.84, id=85)\n"
							  "}";

		const auto definition = parse_function(function);

		EXPECT_THAT(definition.header.first, ElementsAre(8));
		EXPECT_THAT(definition.header.second, ElementsAre(8, 8));

		EXPECT_EQ(definition.operations.back().type, OperationType::RETURN);
		EXPECT_THAT(definition.operations.back().arguments, ElementsAre(83, 84));
		EXPECT_EQ(definition.operations.back().output, 85);

		EXPECT_EQ(definition.operations.size(), 38);
	}
}

TEST(XlsccIrParser, parse_ir)
{
	using namespace herd::translator::xlscc::detail;

	{
		const auto function = "package my_package\n"
							  "\n"
							  "top fn top_add3(input: (bits[8])) -> (bits[8], bits[8]) {\n"
							  "  tuple_index.50: bits[8] = tuple_index(input, index=0, id=50)\n"
							  "  bit_slice.58: bits[1] = bit_slice(tuple_index.50, start=7, width=1, id=58)\n"
							  "  literal.47: bits[1] = literal(value=1, id=47)\n"
							  "  bit_slice.57: bits[1] = bit_slice(tuple_index.50, start=6, width=1, id=57)\n"
							  "  bit_slice.56: bits[1] = bit_slice(tuple_index.50, start=5, width=1, id=56)\n"
							  "  bit_slice.55: bits[1] = bit_slice(tuple_index.50, start=4, width=1, id=55)\n"
							  "  bit_slice.54: bits[1] = bit_slice(tuple_index.50, start=3, width=1, id=54)\n"
							  "  bit_slice.53: bits[1] = bit_slice(tuple_index.50, start=2, width=1, id=53)\n"
							  "  bit_slice.52: bits[1] = bit_slice(tuple_index.50, start=1, width=1, id=52)\n"
							  "  bit_slice.51: bits[1] = bit_slice(tuple_index.50, start=0, width=1, id=51)\n"
							  "  literal.48: bits[1] = literal(value=0, id=48)\n"
							  "  and.74: bits[1] = and(bit_slice.58, literal.47, id=74)\n"
							  "  and.73: bits[1] = and(bit_slice.57, literal.47, id=73)\n"
							  "  and.72: bits[1] = and(bit_slice.56, literal.47, id=72)\n"
							  "  and.71: bits[1] = and(bit_slice.55, literal.47, id=71)\n"
							  "  and.70: bits[1] = and(bit_slice.54, literal.47, id=70)\n"
							  "  and.69: bits[1] = and(bit_slice.53, literal.47, id=69)\n"
							  "  and.68: bits[1] = and(bit_slice.52, literal.47, id=68)\n"
							  "  and.67: bits[1] = and(bit_slice.51, literal.48, id=67)\n"
							  "  or.66: bits[1] = or(bit_slice.58, literal.48, id=66)\n"
							  "  or.65: bits[1] = or(bit_slice.57, literal.47, id=65)\n"
							  "  or.64: bits[1] = or(bit_slice.56, literal.48, id=64)\n"
							  "  or.63: bits[1] = or(bit_slice.55, literal.47, id=63)\n"
							  "  or.62: bits[1] = or(bit_slice.54, literal.48, id=62)\n"
							  "  or.61: bits[1] = or(bit_slice.53, literal.47, id=61)\n"
							  "  or.60: bits[1] = or(bit_slice.52, literal.48, id=60)\n"
							  "  or.59: bits[1] = or(bit_slice.51, literal.47, id=59)\n"
							  "  not.82: bits[1] = not(and.74, id=82)\n"
							  "  not.81: bits[1] = not(and.73, id=81)\n"
							  "  not.80: bits[1] = not(and.72, id=80)\n"
							  "  not.79: bits[1] = not(and.71, id=79)\n"
							  "  not.78: bits[1] = not(and.70, id=78)\n"
							  "  not.77: bits[1] = not(and.69, id=77)\n"
							  "  not.76: bits[1] = not(and.68, id=76)\n"
							  "  not.75: bits[1] = not(and.67, id=75)\n"
							  "  concat.83: bits[8] = concat(or.66, or.65, or.64, or.63, or.62, or.61, or.60, or.59, id=83)\n"
							  "  concat.84: bits[8] = concat(not.82, not.81, not.80, not.79, not.78, not.77, not.76, not.75, id=84)\n"
							  "  ret tuple.85: (bits[8], bits[8]) = tuple(concat.83, concat.84, id=85)\n"
							  "}";

		const auto definition = parse_ir(function);
		const auto& main = definition.main_function;

		EXPECT_THAT(main.header.first, ElementsAre(8));
		EXPECT_THAT(main.header.second, ElementsAre(8, 8));

		EXPECT_EQ(main.operations.back().type, OperationType::RETURN);
		EXPECT_THAT(main.operations.back().arguments, ElementsAre(83, 84));
		EXPECT_EQ(main.operations.back().output, 85);

		EXPECT_EQ(main.operations.size(), 38);
	}
}