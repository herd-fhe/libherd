#include "herd/data_storage/local/local_data_storage.hpp"

#include <memory>

#include "herd/data_storage/local/local_data_table.hpp"
#include "herd/utils/csv_reader.hpp"


namespace herd::storage
{
	std::pair<utils::ProgressFuture<std::shared_ptr<DataTable>>, std::shared_ptr<DataTable>> LocalDataStorage::populate_table_from_csv(std::istream& stream, std::string name, const std::vector<DataTable::ColumnParameters>& columns, common::SchemaType schema_type)
	{
		if(schema_type != common::SchemaType::NONE)
		{
			throw std::runtime_error("Encryption not supported for local storage");
		}

		auto table = LocalDataTable::make_shared(common::UUID(), std::move(name), columns);

		utils::CSVReader reader;

		while(stream.good())
		{
			const auto row = reader.read_row(stream);
			table->add_row(row);
		}

		table->flush_rows();

		auto promise = utils::ProgressPromise<std::shared_ptr<DataTable>>();
		promise.set_max_step(1);
		promise.step(1);
		promise.set_value(table);

		return std::make_pair(promise.get_future(), table);
	}
}
