#ifndef LIBHERD_I_BACKEND_HPP
#define LIBHERD_I_BACKEND_HPP

#include <vector>

#include "herd/session/session.hpp"
#include "herd/utils/movable_function.hpp"
#include "herd/common/uuid.hpp"


namespace herd
{
	class IBackend
	{
	public:
		virtual void connect() = 0;

		virtual SessionInfo create_session(const std::string& name) = 0;
		virtual void destroy_session(const common::UUID& uuid) = 0;
		virtual std::vector<SessionInfo> list_sessions() = 0;

		virtual utils::ProgressFuture<void> add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data) = 0;

		virtual std::unique_ptr<storage::DataStorage> create_session_storage(Session& session) = 0;
		virtual std::pair<utils::ProgressFuture<std::shared_ptr<storage::DataTable>>, std::shared_ptr<storage::DataTable>> create_table(
				const common::UUID& session_uuid, const std::string& name,
				const std::vector<storage::DataTable::ColumnParameters>& columns, common::SchemaType schema_type,
				std::size_t row_count,
				utils::MovableFunction<bool(std::vector<std::byte>&)> next_row
		) = 0;

		virtual ~IBackend() = default;
	};
}

#endif //LIBHERD_I_BACKEND_HPP
