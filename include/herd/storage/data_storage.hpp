#ifndef LIBHERD_DATA_STORAGE_HPP
#define LIBHERD_DATA_STORAGE_HPP

#include <filesystem>
#include <fstream>
#include <memory>
#include <unordered_map>

#include "herd/common/model/schema_type.hpp"
#include "herd/storage/data_frame.hpp"
#include "herd/utils/progress_future.hpp"


namespace herd::storage
{
	class DataStorage
	{
	public:
		virtual ~DataStorage() = default;

		[[nodiscard]] utils::ProgressFuture<std::shared_ptr<DataFrame>> load_from_csv(const std::vector<common::ColumnMeta>& columns, std::istream& stream, common::SchemaType schema_type, const std::string& frame_name = "");

		[[nodiscard]] virtual const std::unordered_map<std::string, std::shared_ptr<DataFrame>>& data_frames() const;
		[[nodiscard]] virtual std::shared_ptr<DataFrame> data_frame_by_name(const std::string& name);

	protected:
		mutable std::unordered_map<std::string, std::shared_ptr<DataFrame>> data_frames_;

		static void mark_as_not_alive(std::shared_ptr<DataFrame>& data_frame);

	private:
		virtual std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> populate_frame_from_csv(std::istream& stream, std::string name, const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type) = 0;
	};
}

#endif //LIBHERD_DATA_STORAGE_HPP
