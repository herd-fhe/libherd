#ifndef LIBHERD_SOURCE_ANALYZER_HPP
#define LIBHERD_SOURCE_ANALYZER_HPP

#include <vector>
#include <string_view>
#include <filesystem>

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Basic/Builtins.h>

#include "herd/common/model/data_type.hpp"


namespace herd::translator::xlscc::detail
{
	struct StructField
	{
		std::string name;
		common::DataType data_type;
	};

	using struct_fields_t = std::vector<StructField>;
	using input_arguments_t = std::vector<struct_fields_t>;
	using output_argument_t = struct_fields_t;

	struct ProgramMetadata
	{
		input_arguments_t inputs;
		output_argument_t output;
	};

	class SourceAnalyzer
	{
	public:
		SourceAnalyzer();

		ProgramMetadata parse(std::string_view source);

	private:
		[[nodiscard]] clang::FunctionDecl* find_main_function() const;
		[[nodiscard]] struct_fields_t process_struct_fields(const clang::CXXRecordDecl* record_decl) const;
		[[nodiscard]] StructField process_struct_field(const clang::FieldDecl* field_decl) const;
		[[nodiscard]] common::DataType map_kind_to_data_type(clang::BuiltinType::Kind kind) const;

		clang::CompilerInstance compiler_instance;
	};
}

#endif //LIBHERD_SOURCE_ANALYZER_HPP
