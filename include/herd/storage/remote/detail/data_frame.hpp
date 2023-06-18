#ifndef LIBHERD_REMOTE_DATA_FRAME_HPP
#define LIBHERD_REMOTE_DATA_FRAME_HPP

#include "herd/common/model/schema_type.hpp"
#include "herd/common/uuid.hpp"
#include "herd/storage/data_frame.hpp"


namespace herd
{
	class RemoteBackend;
}

namespace herd::storage::remote::detail
{
	class DataFrameImpl: public DataFrame
	{
	public:
		DataFrameImpl(
				common::UUID uuid, const std::string& name, size_t row_count, std::size_t partitions,
				const std::vector<common::ColumnMeta>& columns, common::SchemaType schema_type,
				RemoteBackend& remote_backend);

		[[nodiscard]] std::size_t partitions() const override;
		[[nodiscard]] size_t size() const override;
		[[nodiscard]] bool empty() const override;

		[[nodiscard]] common::SchemaType schema_type() const;

	private:
		friend class herd::RemoteBackend;
		struct make_shared_enabler;

		common::SchemaType schema_type_;

		std::size_t partitions_;
		std::size_t rows_count_;

		RemoteBackend& backend_;

		bool uploaded_ = false;
	};
}

#endif //LIBHERD_REMOTE_DATA_FRAME_HPP
