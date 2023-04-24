#ifndef LIBHERD_DATA_STORAGE_HPP
#define LIBHERD_DATA_STORAGE_HPP

#include <memory>
#include <fstream>
#include <unordered_map>
#include <filesystem>

#include "herd/data_storage/data_table.hpp"
#include "herd/utils/progress_future.hpp"
#include "herd_common/schema_type.hpp"


namespace herd::storage
{
	class DataStorage
	{
	public:
		virtual ~DataStorage() = default;

		[[nodiscard]] utils::ProgressFuture<std::shared_ptr<DataTable>> load_from_csv(const std::vector<DataTable::ColumnParameters>& columns, std::istream& stream, common::SchemaType schema_type, const std::string& table_name = "");

		[[nodiscard]] virtual const std::unordered_map<std::string, std::shared_ptr<DataTable>>& data_frames() const;
		[[nodiscard]] virtual std::shared_ptr<DataTable> data_frame_by_name(const std::string& name);

	protected:
		mutable std::unordered_map<std::string, std::shared_ptr<DataTable>> data_frames_;

		static void mark_as_not_alive(std::shared_ptr<DataTable>& data_frame);

	private:
		virtual std::pair<utils::ProgressFuture<std::shared_ptr<DataTable>>, std::shared_ptr<DataTable>> populate_table_from_csv(std::istream& stream, std::string name, const std::vector<DataTable::ColumnParameters>& columns, common::SchemaType schema_type) = 0;
	};
}

#endif //LIBHERD_DATA_STORAGE_HPP
