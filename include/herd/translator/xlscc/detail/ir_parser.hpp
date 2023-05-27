#ifndef LIBHERD_XLSCC_IR_PARSER_HPP
#define LIBHERD_XLSCC_IR_PARSER_HPP

#include <string_view>
#include <vector>


namespace herd::translator::xlscc::detail
{
	using type_bits_list_t = std::vector<unsigned int>;
	using function_header_t = std::pair<type_bits_list_t, type_bits_list_t>;

	using operation_arguments_t = std::vector<unsigned int>;
	using operation_description_t = std::pair<operation_arguments_t, unsigned int>;

	enum class OperationType
	{
		TUPLE_INDEX,
		BIT_SLICE,
		AND,
		OR,
		NOT,
		LITERAL,
		CONCAT,
		RETURN
	};

	struct OperationDefinition
	{
		OperationType type;
		operation_arguments_t arguments;
		unsigned int output;
	};

	struct FunctionDefinition
	{
		function_header_t header;
		std::vector<OperationDefinition> operations;
	};

	struct ProgramDefinition
	{
		FunctionDefinition main_function;
	};

	[[nodiscard]] ProgramDefinition parse_ir(std::string_view source);

	[[nodiscard]] FunctionDefinition parse_function(std::string_view source);

	[[nodiscard]] function_header_t parse_function_header(std::string_view source);
	[[nodiscard]] type_bits_list_t parse_type_list(std::string_view source);

	[[nodiscard]] OperationDefinition parse_function_statement(std::string_view source);

	[[nodiscard]] operation_description_t parse_function_bit_slice_statement_arguments(std::string_view source);
	[[nodiscard]] operation_description_t parse_function_one_named_argument(std::string_view source);
	[[nodiscard]] operation_description_t parse_function_variadic_statement_arguments(std::string_view source);


}

#endif //LIBHERD_XLSCC_IR_PARSER_HPP
