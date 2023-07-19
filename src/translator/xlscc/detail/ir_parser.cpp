#include "herd/translator/xlscc/detail/ir_parser.hpp"

#include <cassert>
#include <locale>
#include <map>
#include <functional>

#include "herd/utils/string_utils.hpp"


namespace herd::translator::xlscc::detail
{
	static const std::map<std::string, OperationType, std::less<>> operation_type_enum_mapping = {
			{"tuple_index", OperationType::TUPLE_INDEX},
			{"bit_slice", OperationType::BIT_SLICE},
			{"and", OperationType::AND},
			{"or", OperationType::OR},
			{"not", OperationType::NOT},
			{"literal", OperationType::LITERAL},
			{"concat", OperationType::CONCAT},
	};

	const std::map<OperationType, std::function<operation_description_t(std::string_view)>> operation_type_arguments_mappers = {
			{OperationType::TUPLE_INDEX, parse_function_tuple_index_arguments},
			{OperationType::BIT_SLICE, parse_function_bit_slice_statement_arguments},
			{OperationType::AND, parse_function_variadic_statement_arguments},
			{OperationType::OR, parse_function_variadic_statement_arguments},
			{OperationType::NOT, parse_function_variadic_statement_arguments},
			{OperationType::LITERAL, parse_function_one_named_argument},
			{OperationType::CONCAT, parse_function_variadic_statement_arguments},
			{OperationType::RETURN, parse_function_variadic_statement_arguments}
	};

	ProgramDefinition parse_ir(std::string_view source)
	{
		enum class ParserState
		{
			FUNC_START_SEARCHING,
			FUNC_END_SEARCHING
		};

		ParserState state = ParserState::FUNC_START_SEARCHING;
		auto current_line_start = std::cbegin(source);

		std::string_view::const_iterator function_begin;
		std::string_view::const_iterator function_end;
		bool function_found = false;

		do
		{
			auto line = utils::next_line(current_line_start, std::cend(source));
			if (std::cbegin(line) == std::cend(source))
			{
				break;
			}

			current_line_start = std::cend(line);
			line = utils::trim(line);

			switch(state)
			{
				using enum ParserState;
				case FUNC_START_SEARCHING:
				{
					if(line.starts_with("top fn"))
					{
						function_begin = std::cbegin(line);
						state = FUNC_END_SEARCHING;
					}
					break;
				}
				case FUNC_END_SEARCHING:
				{
					if(line == "}")
					{
						function_found = true;
						function_end = std::cend(line);
					}
					break;
				}
			}
		}
		while(!function_found);

		assert(function_found);

		const auto function = std::string_view(function_begin, function_end);

		ProgramDefinition definition;
		definition.main_function = parse_function(function);

		return definition;
	}

	FunctionDefinition parse_function(std::string_view source)
	{
		FunctionDefinition definition;

		enum class ParserState
		{
			HEADER,
			INSTRUCTIONS,
			END
		};

		ParserState state = ParserState::HEADER;
		auto current_line_start = std::cbegin(source);

		do
		{
			auto line = utils::next_line(current_line_start, std::cend(source));
			if(std::cbegin(line) == std::cend(source))
			{
				break;
			}
			current_line_start = std::cend(line);
			line = utils::trim(line);

			switch(state)
			{
				using enum ParserState;

				case HEADER:
				{
					definition.header = parse_function_header(line);
					state = INSTRUCTIONS;
					break;
				}
				case INSTRUCTIONS:
				{
					if(!line.starts_with('}'))
					{
						definition.operations.emplace_back(parse_function_statement(line));
					}
					else
					{
						state = END;
					}
					break;
				}
				case END:
				{
					break;
				}
			}
		}
		while(true);

		return definition;
	}

	function_header_t parse_function_header(std::string_view source)
	{
		enum class ParserState
		{
			ARGUMENTS_START,
			ARGUMENTS_END,
			RETURN_START,
			RETURN_END,
			END
		};

		ParserState state = ParserState::ARGUMENTS_START;
		bool parsing_completed = false;

		std::string_view::const_iterator signature_begin;
		std::string_view output_signature;

		std::vector<type_bits_list_t> inputs;

		for(auto iter = std::cbegin(source); iter != std::cend(source); ++iter)
		{
			switch(state)
			{
				using enum ParserState;

				case ParserState::ARGUMENTS_START:
				{
					if(*iter == ':')
					{
						signature_begin = iter;
						state = ParserState::ARGUMENTS_END;
					}
					else if(*iter == '>')
					{
						state = ParserState::RETURN_START;
					}
					break;
				}
				case ParserState::ARGUMENTS_END:
				{
					if(*iter == ')')
					{
						inputs.emplace_back(parse_type_list({signature_begin + 3, iter }));
						state = ParserState::ARGUMENTS_START;
					}
					break;
				}
				case ParserState::RETURN_START:
				{
					if(*iter == '(')
					{
						signature_begin = iter;
						state = ParserState::RETURN_END;
					}
					break;
				}
				case ParserState::RETURN_END:
				{
					if(*iter == ')')
					{
						output_signature = {signature_begin + 1, iter };
						state = ParserState::END;
					}
					break;
				}
				case END:
				{
					parsing_completed = true;
					break;
				}
			}
			if(parsing_completed)
			{
				break;
			}
		}

		assert(parsing_completed);
		const auto output_bit_sizes = parse_type_list(output_signature);

		return std::make_pair(inputs, output_bit_sizes);
	}

	std::vector<unsigned int> parse_type_list(std::string_view source)
	{
		enum class ParserState
		{
			ARGUMENTS_START,
			ARGUMENTS_END,
		};

		std::vector<unsigned int> args_bit_sizes;
		std::string_view::const_iterator value_begin;
		ParserState state = ParserState::ARGUMENTS_START;

		for(auto iter = std::cbegin(source); iter != std::cend(source); ++iter)
		{
			switch(state)
			{
				using enum ParserState;

				case ARGUMENTS_START:
				{
					if(*iter == '[')
					{
						value_begin = iter;
						state = ARGUMENTS_END;
					}
					break;
				}
				case ARGUMENTS_END:
				{
					if(*iter == ']')
					{
						const std::string_view bit_size_str = {value_begin + 1, iter};
						args_bit_sizes.emplace_back(utils::parse_num<uint8_t>(bit_size_str));

						state = ARGUMENTS_START;
					}
					break;
				}
			}
		}

		assert(state == ParserState::ARGUMENTS_START);

		return args_bit_sizes;
	}

	OperationDefinition parse_function_statement(std::string_view source)
	{
		OperationDefinition definition;

		const auto assignment_pos = source.find('=');
		const auto arguments_begin = std::begin(source) + source.find('(', assignment_pos) + 1;

		if(source.starts_with("ret "))
		{
			definition.type = OperationType::RETURN;
		}
		else
		{
			const auto operation_name_begin = std::begin(source) + assignment_pos;
			const std::string operation_name = { operation_name_begin + 2, arguments_begin - 1 };

			assert(operation_type_enum_mapping.contains(operation_name) == 1);

			definition.type = operation_type_enum_mapping.at(operation_name);
		}

		const auto arguments_end = std::begin(source) + source.find(')', assignment_pos);
		const std::string_view arguments = {arguments_begin, arguments_end};

		assert(operation_type_arguments_mappers.contains(definition.type) != 0);
		const auto [parsed_arguments, output_id] = operation_type_arguments_mappers.at(definition.type)(arguments);

		definition.arguments = parsed_arguments;
		definition.output = output_id;

		return definition;
	}

	operation_description_t parse_function_tuple_index_arguments(std::string_view source)
	{
		const auto tuple_name_pos = source.find('(') + 1;
		const auto tuple_name_end_pos = source.find(',', tuple_name_pos);
		const auto tuple_name = std::string(std::cbegin(source) + tuple_name_pos, std::cbegin(source) + tuple_name_end_pos);

		const auto tuple_index_pos = source.find('=', tuple_name_end_pos) + 1;
		const auto tuple_index_end_pos = source.find(',', tuple_index_pos);
		const auto tuple_index = utils::parse_num<unsigned int>(std::string_view(std::begin(source) + tuple_index_pos, std::begin(source) + tuple_index_end_pos));

		const auto id_pos = source.find('=', tuple_index_end_pos) + 1;
		const auto id = utils::parse_num<unsigned int>(source.substr(id_pos));

		return {{tuple_name, tuple_index}, id};
	}

	operation_description_t parse_function_bit_slice_statement_arguments(std::string_view source)
	{
		const auto tuple_pos = source.find('.') + 1;
		const auto tuple_end_pos = source.find(',', tuple_pos);
		const auto tuple = utils::parse_num<unsigned int>(std::string_view(std::begin(source) + tuple_pos, std::begin(source) + tuple_end_pos));

		const auto start_pos = source.find('=', tuple_end_pos) + 1;
		const auto start_end_pos = source.find(',', start_pos);
		const auto start = utils::parse_num<unsigned int>(std::string_view(std::begin(source) + start_pos, std::begin(source) + start_end_pos));

		const auto width_pos = source.find('=', start_end_pos) + 1;
		const auto width_end_pos = source.find(',', width_pos);
		const auto width = utils::parse_num<unsigned int>(std::string_view(std::begin(source) + width_pos, std::begin(source) + width_end_pos));

		const auto id_pos = source.find('=', width_end_pos) + 1;
		const auto id = utils::parse_num<unsigned int>(source.substr(id_pos));

		return {{tuple, start, width}, id};
	}

	operation_description_t parse_function_one_named_argument(std::string_view source)
	{
		const auto value_pos = source.find('=') + 1;
		const auto value_end_pos = source.find(',', value_pos);
		const auto value = utils::parse_num<unsigned int>(std::string_view(std::begin(source) + value_pos, std::begin(source) + value_end_pos));

		const auto id_pos = source.find('=', value_end_pos) + 1;
		const auto id = utils::parse_num<unsigned int>(source.substr(id_pos));

		return {{value}, id};
	}

	operation_description_t parse_function_variadic_statement_arguments(std::string_view source)
	{
		operation_arguments_t arguments;

		auto argument_pos = source.find('.');

		while(argument_pos != std::string::npos)
		{
			++argument_pos;

			const auto argument_end_pos = source.find(',', argument_pos);
			const auto argument = utils::parse_num<unsigned int>(source.substr(argument_pos, argument_end_pos - argument_pos));
			arguments.emplace_back(argument);

			argument_pos = source.find('.', argument_pos);
		}

		const auto id_pos = source.find('=') + 1;
		const auto id = utils::parse_num<unsigned int>(source.substr(id_pos));

		return {arguments, id};
	}
}