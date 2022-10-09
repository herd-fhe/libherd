#ifndef LIBHERD_DATA_STORAGE_HPP
#define LIBHERD_DATA_STORAGE_HPP

#include <memory>
#include <istream>
#include <unordered_map>
#include <filesystem>

#include "herd/data_storage/data_table.hpp"


namespace herd::storage
{
	class DataStorage
	{
	public:
		virtual ~DataStorage() = default;

		[[nodiscard]] std::shared_ptr<DataTable> load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, std::istream& stream, const std::string& table_name = "");
		[[nodiscard]] std::shared_ptr<DataTable> load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, const std::filesystem::path& path, const std::string& table_name = "");

		[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<DataTable>>& tables() const;

		[[nodiscard]] std::shared_ptr<DataTable> table_by_name(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<DataTable>> tables_;

		[[nodiscard]] virtual std::shared_ptr<DataTable> create_table(std::string name, const std::vector<DataTable::ColumnParameters>& columns) = 0;
		static void populate_table_from_csv(const std::shared_ptr<DataTable>& table, std::istream& stream) ;
	};
}

#endif //LIBHERD_DATA_STORAGE_HPP
