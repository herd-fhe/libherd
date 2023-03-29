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
			while(tables_.contains(name));
		}

		auto [future, table] = populate_table_from_csv(stream, name, columns, schema_type);
		tables_.try_emplace(name, table);

		return std::move(future);
	}

	const std::unordered_map<std::string, std::shared_ptr<DataTable>> &DataStorage::tables() const
	{
		return tables_;
	}

	std::shared_ptr<DataTable> DataStorage::table_by_name(const std::string& name)
	{
		if(const auto iter = tables_.find(name); iter != std::end(tables_))
		{
			return iter->second;
		}

		return nullptr;
	}

}