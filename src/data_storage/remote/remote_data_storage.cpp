#include "herd/data_storage/remote/remote_data_storage.hpp"

#include <algorithm>
#include <memory>

#include "herd/backend/remote/remote_backend.hpp"
#include "herd/data_storage/remote/remote_data_table.hpp"
#include "herd/utils/csv_reader.hpp"


namespace herd::storage
{
	RemoteDataStorage::RemoteDataStorage(Session& session, RemoteBackend& backend)
	:   session_(session), backend_(backend)
	{
	}

	std::pair<utils::ProgressFuture<std::shared_ptr<DataTable>>, std::shared_ptr<DataTable>> RemoteDataStorage::populate_table_from_csv(std::istream& stream, std::string name, const std::vector<DataTable::ColumnParameters>& columns, common::SchemaType schema_type)
	{
		const auto& crypto = session_.crypto(schema_type);
		const auto row_count = utils::CSVReader::row_count(stream);

		std::vector<DataTable::column_type_key_type> column_types;
		column_types.reserve(columns.size());
		std::ranges::transform(columns, std::back_inserter(column_types), [](const auto column){ return column.type; });

		auto next_row_bytes = [&stream, column_types, &crypto](std::vector<std::byte>& row_bytes)
		{
			utils::CSVReader reader;
			const auto row = reader.read_row(stream);
			const auto encrypted_row = DataTable::encrypt_row(row, column_types, crypto);
			row_bytes.insert(std::begin(row_bytes), std::begin(encrypted_row), std::end(encrypted_row));
			return true;
		};

		return backend_.create_table(session_.uuid(), name, columns, schema_type, row_count, std::move(next_row_bytes));
	}

	std::unique_ptr<RemoteDataStorage> RemoteDataStorage::make_unique(Session& session, RemoteBackend& backend)
	{
		return std::make_unique<make_unique_enabler>(session, backend);
	}
}