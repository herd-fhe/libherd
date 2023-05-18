#include "herd/translator/xlscc/detail/compiler.hpp"

#include "herd/common/uuid.hpp"
#include "herd/translator/xlscc/detail/ir_parser.hpp"
#include "herd/translator/xlscc/detail/ir_transpiler.hpp"
#include "herd/utils/path.hpp"


namespace herd::translator::xlscc::detail
{

	Compiler::Compiler(utils::TempStorage& temp_storage)
	:	temp_storge_(temp_storage)
	{
		find_toolset();
	}

	Compiler::~Compiler()
	{
	}

	Circuit Compiler::translate(std::string_view source)
	{
		const auto source_path = temp_storge_.create_temp_file();
		utils::TempStorage::fill_file(source_path, source);
		auto temp_code = transpile_to_ir(source_path);

		const auto ir_path = temp_storge_.create_temp_file();
		utils::TempStorage::fill_file(ir_path, temp_code);
		temp_code = optimize_ir(ir_path);

		const auto optimized_ir_path = temp_storge_.create_temp_file();
		utils::TempStorage::fill_file(optimized_ir_path, temp_code);
		temp_code = booleanify_ir(optimized_ir_path);

		const auto definition = parse_ir(temp_code);
		auto stage_circuit = transpile_ir_program(definition);

		return stage_circuit;
	}

	std::string Compiler::transpile_to_ir(const std::filesystem::path& ir_file_path)
	{
		const auto command = toolset_dir_.string() + "/xlscc/xlscc " + ir_file_path.string();
		return exec(command);
	}


	std::string Compiler::optimize_ir(const std::filesystem::path& ir_file_path)
	{
		const auto command = toolset_dir_.string() + "/tools/opt_main " + ir_file_path.string();
		return exec(command);
	}

	std::string Compiler::booleanify_ir(const std::filesystem::path& ir_file_path)
	{
		const auto command = toolset_dir_.string() + "/tools/booleanify_main --ir_path=" + ir_file_path.string();
		return exec(command);
	}

	std::string Compiler::exec(const std::string& command)
	{
		std::unique_ptr<FILE, decltype(&pclose)> exec_pipe(popen(command.c_str(), "r"), pclose);
		if(!exec_pipe)
		{
			throw std::runtime_error("Failed to execute command: "+ command);
		}

		std::array<char, 64> temp_buffer;
		std::string output;
		while(fgets(temp_buffer.data(), temp_buffer.size(), exec_pipe.get()) != 0)
		{
			output += temp_buffer.data();
		}

		return output;
	}

	void Compiler::find_toolset()
	{
		toolset_dir_ = utils::get_herd_home();
	}
}