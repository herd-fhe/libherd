#ifndef LIBHERD_XLSCC_COMPILER_HPP
#define LIBHERD_XLSCC_COMPILER_HPP

#include <filesystem>
#include <fstream>

#include "herd/translator/i_translator.hpp"
#include "herd/utils/temp_storage.hpp"


namespace herd::translator::xlscc::detail
{
	class Compiler: public ICompiler
	{
	public:
		Compiler(utils::TempStorage& temp_storage);
		~Compiler() override;

		common::Circuit translate(std::string_view source) override;

	private:
		utils::TempStorage& temp_storge_;
		std::filesystem::path toolset_dir_;

		std::string transpile_to_ir(const std::filesystem::path& ir_file_path);
		std::string	optimize_ir(const std::filesystem::path& ir_file_path);
		std::string booleanify_ir(const std::filesystem::path& ir_file_path);

		static std::string exec(const std::string& command);

		void find_toolset();
	};
}

#endif //LIBHERD_XLSCC_COMPILER_HPP
