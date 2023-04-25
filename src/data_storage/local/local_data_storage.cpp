#include "herd/data_storage/local/local_data_storage.hpp"

#include <memory>

#include "herd/data_storage/local/local_data_frame.hpp"
#include "herd/utils/csv_reader.hpp"


namespace herd::storage
{
	std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> LocalDataStorage::populate_frame_from_csv(std::istream& stream, std::string name, const std::vector<DataFrame::ColumnParameters>& columns, common::SchemaType schema_type)
	{
		if(schema_type != common::SchemaType::NONE)
		{
			throw std::runtime_error("Encryption not supported for local storage");
		}

		auto frame = LocalDataFrame::make_shared(UUID(), std::move(name), columns);

		utils::CSVReader reader;

		while(stream.good())
		{
			const auto row = reader.read_row(stream);
			frame->add_row(row);
		}

		frame->flush_rows();

		auto promise = utils::ProgressPromise<std::shared_ptr<DataFrame>>();
		promise.set_max_step(1);
		promise.step(1);
		promise.set_value(frame);

		return std::make_pair(promise.get_future(), frame);
	}
}
