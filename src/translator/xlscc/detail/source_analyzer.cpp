#include "herd/translator/xlscc/detail/source_analyzer.hpp"

#include <iostream>
#include <memory>

#include <llvm/Support/CrashRecoveryContext.h>
#include <clang/AST/ASTContext.h>
#include <llvm/Support/Host.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Parse/ParseAST.h>
#include <clang/Basic/FileManager.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Basic/Builtins.h>

#include "herd/common/native_type_mapping.hpp"

#include "herd/utils/string_utils.hpp"

#include "herd/translator/exception.hpp"


namespace
{
	constexpr const char* pragma_key = "#pragma";
	constexpr const char* hls_top_key = "hls_top";
}


namespace herd::translator::xlscc::detail
{
	SourceAnalyzer::SourceAnalyzer()
	{
		compiler_instance.getDiagnosticOpts().Warnings.emplace_back("no-unused-label");
		compiler_instance.getDiagnosticOpts().Warnings.emplace_back("no-unused-but-set-variable");
		compiler_instance.getDiagnosticOpts().Warnings.emplace_back("no-constant-logical-operand");
		compiler_instance.getDiagnosticOpts().Warnings.emplace_back("no-c++11-narrowing");
		compiler_instance.createDiagnostics();
		compiler_instance.createFileManager();
		compiler_instance.createSourceManager(compiler_instance.getFileManager());

		const auto triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
		auto target_options = std::make_shared<clang::TargetOptions>();
		target_options->Triple = triple.getTriple();
		auto target_info = clang::TargetInfo::CreateTargetInfo(compiler_instance.getDiagnostics(), target_options);
		compiler_instance.setTarget(target_info);

		auto& language_options = compiler_instance.getLangOpts();
		language_options.LangStd = clang::LangStandard::lang_cxx20;
		language_options.CPlusPlus = 1;
		language_options.CPlusPlus11 = 1;
		language_options.CPlusPlus14 = 1;
		language_options.CPlusPlus17 = 1;
		language_options.WChar = 1;
		language_options.Bool = 1;

		compiler_instance.getHeaderSearchOpts().UseStandardSystemIncludes = 0;


		compiler_instance.createPreprocessor(clang::TU_Complete);
		auto& preprocessor = compiler_instance.getPreprocessor();
		preprocessor.getBuiltinInfo().initializeBuiltins(preprocessor.getIdentifierTable(), preprocessor.getLangOpts());

		auto ast_consumer = std::make_unique<clang::ASTConsumer>();
		compiler_instance.setASTConsumer(std::move(ast_consumer));
		compiler_instance.createASTContext();
	}

	ProgramMetadata SourceAnalyzer::parse(std::string_view source)
	{
		auto main_source = llvm::MemoryBuffer::getMemBuffer(source, "main.cpp");

		const auto main_file_id = compiler_instance.getSourceManager().createFileID(std::move(main_source));
		compiler_instance.getSourceManager().setMainFileID(main_file_id);

		compiler_instance.getDiagnosticClient().BeginSourceFile(compiler_instance.getLangOpts());
		clang::ParseAST(compiler_instance.getPreprocessor(), &compiler_instance.getASTConsumer(), compiler_instance.getASTContext());
		compiler_instance.getDiagnosticClient().EndSourceFile();

		auto main = find_main_function();
		if(main == nullptr)
		{
			throw CompilerError("Invalid stage code. No top function");
		}

		ProgramMetadata metadata;
		const auto argument_count = main->getNumParams();
		metadata.inputs.reserve(argument_count);

		for(unsigned int i = 0; i < argument_count; ++i)
		{
			const auto argument = main->getParamDecl(i);
			if(argument->hasDefaultArg())
			{
				throw CompilerError("Default argument not supported for top function");
			}

			const auto type = argument->getType();
			if(!type->isStructureOrClassType() || type.isNull())
			{
				throw CompilerError("Top function argument must be a class/struct type");
			}

			const auto record_decl = type.getTypePtr()->getAsCXXRecordDecl();
			const auto argument_name = argument->getNameAsString();
			metadata.inputs.emplace_back(argument_name, process_struct_fields(record_decl));
		}

		const auto return_type = main->getReturnType();
		if(!return_type->isStructureOrClassType() || return_type.isNull())
		{
			throw CompilerError("Top function return type must be a class/struct type");
		}
		const auto output_record_decl = return_type.getTypePtr()->getAsCXXRecordDecl();

		metadata.output = process_struct_fields(output_record_decl);

		return metadata;
	}

	clang::FunctionDecl* SourceAnalyzer::find_main_function() const
	{
		const auto& ast_context = compiler_instance.getASTContext();
		const auto translation_unit = ast_context.getTranslationUnitDecl();
		auto iter = translation_unit->decls_begin();
		for(; iter != translation_unit->decls_end(); ++iter)
		{
			if(iter->isFunctionOrFunctionTemplate() && !iter->isTemplated())
			{
				const auto location = iter->getLocation();
				const auto& source_manager = compiler_instance.getSourceManager();
				const auto file = source_manager.getFileID(location);
				const auto function_line = source_manager.getSpellingLineNumber(location);
				if(function_line == 0)
				{
					continue ;
				}

				const auto pragma_file_entry = source_manager.getFileEntryForID(file);
				const auto pragma_line = function_line - 2;
				const auto pragma_location = source_manager.translateFileLineCol(pragma_file_entry, pragma_line, 0);

				assert(source_manager.getCharacterData(pragma_location)[0] == '\n');
				const auto line_start =  source_manager.getCharacterData(pragma_location) + 1;

				const auto next_line_start = source_manager.getCharacterData(location);

				const auto predicted_pragma_line = utils::next_line(line_start, next_line_start);
				const auto trimmed_predicted_pragma_line = utils::trim(predicted_pragma_line);
				if(!trimmed_predicted_pragma_line.starts_with(pragma_key))
				{
					continue;
				}

				const auto pragma_type = utils::trim({
						trimmed_predicted_pragma_line.begin() + strlen(pragma_key),
						trimmed_predicted_pragma_line.end()
				});

				if(pragma_type == hls_top_key)
				{
					return dynamic_cast<clang::FunctionDecl*>(*iter);
				}
			}
		}
		return nullptr;
	}

	struct_fields_t SourceAnalyzer::process_struct_fields(const clang::CXXRecordDecl* record_decl) const
	{
		if(!record_decl->isPOD())
		{
			throw CompilerError("Input type needs to be a POD type");
		}

		struct_fields_t fields;
		for(const clang::FieldDecl* const field_decl: record_decl->fields())
		{
			fields.emplace_back(process_struct_field(field_decl));
		}
		return fields;
	}

	StructField SourceAnalyzer::process_struct_field(const clang::FieldDecl* field_decl) const
	{
		const auto canonical_type = field_decl->getType()->getCanonicalTypeInternal();
		if(!canonical_type->isBooleanType() && !canonical_type->isArithmeticType())
		{
			throw CompilerError("Only boolean/arithmeticsupported");
		}

		if(const clang::Type* type = canonical_type.getTypePtr(); type->isBuiltinType())
		{
			const auto builtin_type = type->getAs<clang::BuiltinType>();
			const auto data_type = map_kind_to_data_type(builtin_type->getKind());
			assert(common::data_type_to_bit_width(data_type) == compiler_instance.getASTContext().getTypeSize(type->getCanonicalTypeInternal()));

			const auto name = field_decl->getNameAsString();
			return {name, data_type};
		}

		throw CompilerError("Not builtin type");
	}

	common::DataType SourceAnalyzer::map_kind_to_data_type(clang::BuiltinType::Kind kind) const
	{
		switch(kind)
		{
			using enum herd::common::DataType;

			case clang::BuiltinType::Kind::Bool:
				return BIT;
			case clang::BuiltinType::Kind::Char_U:
			case clang::BuiltinType::Kind::UChar:
			{
				return UINT8;
			}
			case clang::BuiltinType::Kind::Char_S:
			case clang::BuiltinType::Kind::SChar:
			{
				return INT8;
			}
			case clang::BuiltinType::Kind::UShort:
			{
				return UINT16;
			}
			case clang::BuiltinType::Kind::Short:
			{
				return INT16;
			}
			case clang::BuiltinType::Kind::UInt:
			{
				return UINT32;
			}
			case clang::BuiltinType::Kind::Int:
			{
				return INT32;
			}
			case clang::BuiltinType::Kind::ULong:
			case clang::BuiltinType::Kind::ULongLong:
			{
				return UINT64;
			}
			case clang::BuiltinType::Kind::Long:
			case clang::BuiltinType::Kind::LongLong:
			{
				return INT64;
			}
			default:
			{
				throw CompilerError("Unsupported arithmetic type");
			}
		}
	}

}