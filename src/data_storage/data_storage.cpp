#include "herd/data_storage/data_storage.hpp"

#include <fstream>

#include "herd/utils/csv_reader.hpp"


namespace
{
	std::string auto_table_name(std::size_t retry)
	{
		return "table_" + std::to_string(retry);
	}
}

namespace herd::storage
{
	utils::ProgressFuture<std::shared_ptr<DataTable>> DataStorage::load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, std::istream& stream, common::SchemaType schema_type, const std::string& table_name)
	{
		std::string name = table_name;
		if(name.empty())
		{
			size_t retry = 0;
			do
			{
				name = auto_table_name(retry);
				++retry;
			}
			while(data_frames_.contains(name));
		}

		auto [future, table] = populate_table_from_csv(stream, name, columns, schema_type);
		data_frames_.try_emplace(name, table);

		return std::move(future);
	}

	const std::unordered_map<std::string, std::shared_ptr<DataTable>> &DataStorage::data_frames() const
	{
		return data_frames_;
	}

	std::shared_ptr<DataTable> DataStorage::data_frame_by_name(const std::string& name)
	{
		const auto& frames_available = data_frames();
		if(const auto iter = frames_available.find(name); iter != std::end(frames_available))
		{
			return iter->second;
		}

		return nullptr;
	}


	void DataStorage::mark_as_not_alive(std::shared_ptr<DataTable>& data_frame)
	{
		data_frame->set_alive_status(false);
	}
}