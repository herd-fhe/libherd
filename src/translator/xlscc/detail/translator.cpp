#include "herd/translator/xlscc/detail/translator.hpp"


namespace herd::translator::xlscc::detail
{

	Translator::Translator()
	{
		setup_tmpdir();
	}

	Translator::~Translator()
	{
		clean_tmpdir();
	}

	std::shared_ptr<CircuitGraph> Translator::translate(std::string_view source)
	{
		static_cast<void>(source);
		return std::shared_ptr<CircuitGraph>();
	}


	void Translator::setup_tmpdir()
	{
		char dir_path[L_tmpnam];
		if(!std::tmpnam(dir_path))
		{
			throw std::runtime_error("Failed to create tmp directory path");
		}

		std::filesystem::path temp_dir(dir_path);

		std::filesystem::create_directory(temp_dir);
	}

	void Translator::clean_tmpdir()
	{
		std::filesystem::remove_all(tmp_dir);
	}

}