#ifndef LIBHERD_PATH_HPP
#define LIBHERD_PATH_HPP

#include <optional>
#include <filesystem>


namespace herd::utils
{
	std::filesystem::path get_herd_home();
	std::filesystem::path get_self_executable_dir_path();
}

#endif //LIBHERD_PATH_HPP
