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
	std::shared_ptr<DataTable> DataStorage::load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, std::istream& stream, const std::string& table_name)
	{
		static_cast<void>(stream);

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

		auto table = create_table(name, columns);

		populate_table_from_csv(table, stream);

		return table;
	}

	std::shared_ptr<DataTable> DataStorage::load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, const std::filesystem::path& path, const std::string& table_name)
	{
		if(!std::filesystem::exists(path))
		{
			throw std::runtime_error("File " + path.string() + " not found");
		}

		if(!std::filesystem::is_regular_file(path))
		{
			throw std::runtime_error(path.string() + "is not a regular file");
		}

		std::ifstream file(path);

		if(!file.good())
		{
			throw std::runtime_error("Could not open file " + path.string());
		}

		return load_from_csv(columns, file, table_name);
	}

	void DataStorage::populate_table_from_csv(const std::shared_ptr<DataTable> &table, std::istream &stream)
	{
		utils::CSVReader reader;

		while(stream.good())
		{
			const auto row = reader.read_row(stream);
			table->add_row(row);
		}

		table->flush_rows();
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