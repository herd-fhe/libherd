#include "herd/storage/data_storage.hpp"

#include <fstream>

#include "herd/utils/csv_reader.hpp"


namespace
{
	std::string auto_frame_name(std::size_t retry)
	{
		return "frame_" + std::to_string(retry);
	}
}

namespace herd::storage
{
	utils::ProgressFuture<std::shared_ptr<DataFrame>> DataStorage::import_from_csv(std::istream& stream, const ImportOptions& options)
	{
		std::string name = options.name;
		if(name.empty())
		{
			size_t retry = 0;
			do
			{
				name = auto_frame_name(retry);
				++retry;
			}
			while(data_frames_.contains(name));
		}

		auto [future, frame] = populate_frame_from_csv(stream, name, options.columns, options.schema, options.partition_count);
		data_frames_.try_emplace(name, frame);

		return std::move(future);
	}

	const std::unordered_map<std::string, std::shared_ptr<DataFrame>>& DataStorage::data_frames() const
	{
		return data_frames_;
	}

	std::shared_ptr<DataFrame> DataStorage::data_frame_by_name(const std::string& name)
	{
		const auto& frames_available = data_frames();
		if(const auto iter = frames_available.find(name); iter != std::end(frames_available))
		{
			return iter->second;
		}

		return nullptr;
	}


	void DataStorage::mark_as_not_alive(DataFrame& data_frame)
	{
		data_frame.set_alive_status(false);
	}
}