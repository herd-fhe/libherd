#include "herd/utils/path.hpp"

#include <cstdio>
#ifdef __linux__
	#include <unistd.h>

	static const char* herd_home_internal = "/usr/bin";

#else
	#error "System not supported"
#endif


namespace herd::utils
{

	std::filesystem::path get_herd_home()
	{
		if(const char* herd_home = std::getenv("HERD_HOME"))
		{
			std::filesystem::path herd_home_path = herd_home;
			if(!std::filesystem::exists(herd_home_path))
			{
				throw std::runtime_error("Provided HERD_HOME does not exist");
			}

			return herd_home_path;
		}

		return herd_home_internal;
	}

	std::filesystem::path get_self_executable_dir_path()
	{
		static auto self_dir = []() {
			char self_path[FILENAME_MAX];
			ssize_t count = readlink("/proc/self/exe", self_path, FILENAME_MAX);
			if(count <= 0)
			{
				throw std::runtime_error("Failed to find self directory");
			}
			std::filesystem::path self_dir_path(self_path);
			self_dir_path.remove_filename();

			return self_dir_path;
		}();

		return self_dir;
	}
}