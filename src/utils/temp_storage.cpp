#include "herd/utils/temp_storage.hpp"

#include <fstream>
#include <iostream>

#include "herd/common/uuid.hpp"


namespace herd::utils
{
	TempStorage::TempStorage()
	{
		setup_tmpdir();
	}

	TempStorage::~TempStorage()
	{
		clean_tmpdir();
	}

	void TempStorage::fill_file(const std::filesystem::path& path, std::string_view data)
	{
		std::ofstream file(path);
		file << data;
		file.close();
	}

	std::filesystem::path TempStorage::create_temp_file(const std::string& suffix)
	{
		std::string filename = get_random_filename() + suffix;
		while(std::filesystem::exists(temp_dir_ / filename))
		{
			filename = get_random_filename() + suffix;
		}

		return temp_dir_ / filename;
	}

	std::string TempStorage::get_random_filename()
	{
		return common::UUID().as_string();
	}

	void TempStorage::setup_tmpdir()
	{
		auto global_temp_dir = std::filesystem::temp_directory_path().string() + "/herd.XXXXXX";

		if(mkdtemp(global_temp_dir.data()) == nullptr)
		{
			throw std::runtime_error("Failed to create tmp directory path");
		}

		temp_dir_ = global_temp_dir;
	}

	void TempStorage::clean_tmpdir()
	{
		std::filesystem::remove_all(temp_dir_);
	}
}