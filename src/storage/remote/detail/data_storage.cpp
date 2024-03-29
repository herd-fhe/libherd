#include "herd/storage/remote/detail/data_storage.hpp"

#include <algorithm>
#include <memory>

#include "herd/backend/remote/remote_backend.hpp"
#include "herd/storage/remote/detail/data_frame.hpp"
#include "herd/utils/csv_reader.hpp"


namespace herd::storage::remote::detail
{
	DataStorageImpl::DataStorageImpl(Session& session, RemoteBackend& backend):
		session_(session), backend_(backend)
	{
	}

	std::pair<utils::ProgressFuture<std::shared_ptr<DataFrame>>, std::shared_ptr<DataFrame>> DataStorageImpl::populate_frame_from_csv(
			std::istream& stream,
			const std::string& name,
			const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
			std::size_t partitions)
	{
		const auto& crypto = session_.crypto(schema_type);
		const auto row_count = utils::CSVReader::row_count(stream);

		std::vector<DataFrame::column_type_key_type> column_types;
		column_types.reserve(columns.size());
		std::ranges::transform(columns, std::back_inserter(column_types), [](const auto column) { return column.type; });

		auto next_row_bytes = [&stream, column_types, &crypto](std::vector<std::byte>& row_bytes)
		{
			utils::CSVReader reader;
			const auto row = reader.read_row(stream);
			const auto encrypted_row = DataFrame::encrypt_row(row, column_types, crypto);
			row_bytes.insert(std::begin(row_bytes), std::begin(encrypted_row), std::end(encrypted_row));
			return true;
		};

		return backend_.create_data_frame(session_.uuid(), name, columns, schema_type, row_count, std::move(next_row_bytes), partitions);
	}

	const std::unordered_map<std::string, std::shared_ptr<DataFrame>>& DataStorageImpl::data_frames() const
	{
		sync_cache();
		return DataStorage::data_frames();
	}

	void DataStorageImpl::sync_cache() const
	{
		auto remote_data_frames = backend_.list_data_frames(session_.uuid());

		std::set<common::UUID> alive_data_frame_uuids;
		for(const auto& frame: remote_data_frames)
		{
			alive_data_frame_uuids.insert(frame->uuid());
		}

		std::set<common::UUID> cached_data_frame_uuids;
		for(const auto& [name, frame]: data_frames_)
		{
			cached_data_frame_uuids.insert(frame->uuid());
		}

		std::set<common::UUID> new_data_frame_uuids;
		std::ranges::set_difference(
				alive_data_frame_uuids,
				cached_data_frame_uuids,
				std::inserter(new_data_frame_uuids, std::begin(new_data_frame_uuids)),
				std::less<>());

		for(auto& [name, frame]: data_frames_)
		{
			if(!alive_data_frame_uuids.contains(frame->uuid()))
			{
				DataStorage::mark_as_not_alive(*frame);
			}
		}

		for(auto& frame: remote_data_frames)
		{
			if(new_data_frame_uuids.contains(frame->uuid()))
			{
				const auto name = frame->name();
				data_frames_.try_emplace(name, frame);
			}
		}
	}
}