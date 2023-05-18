#ifndef LIBHERD_TEMP_STORAGE_HPP
#define LIBHERD_TEMP_STORAGE_HPP

#include <filesystem>
#include <string>


namespace herd::utils
{
	class TempStorage
	{
	public:
		TempStorage();
		~TempStorage();

		static void fill_file(const std::filesystem::path& path, std::string_view data);
		std::filesystem::path create_temp_file(const std::string& suffix="");

	private:
		std::filesystem::path temp_dir_;

		static std::string get_random_filename();

		void setup_tmpdir();
		void clean_tmpdir();
	};
}

#endif //LIBHERD_TEMP_STORAGE_HPP
