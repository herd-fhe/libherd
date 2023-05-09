#ifndef LIBHERD_XLSCC_TRANSLATOR_HPP
#define LIBHERD_XLSCC_TRANSLATOR_HPP

#include <filesystem>

#include "herd/translator/i_translator.hpp"


namespace herd::translator::xlscc::detail
{
	class Translator: public ITranslator
	{
	public:
		Translator();
		~Translator() override;

		std::shared_ptr<CircuitGraph> translate(std::string_view source) override;

	private:
		std::filesystem::path tmp_dir;

		void setup_tmpdir();
		void clean_tmpdir();
	};
}

#endif //LIBHERD_XLSCC_TRANSLATOR_HPP
