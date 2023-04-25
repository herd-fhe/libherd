#ifndef LIBHERD_REMOTE_DATA_FRAME_HPP
#define LIBHERD_REMOTE_DATA_FRAME_HPP

#include "herd/data_storage/data_frame.hpp"
#include "herd/common/uuid.hpp"
#include "herd/common/model/schema_type.hpp"


namespace herd
{
	class RemoteBackend;
}

namespace herd::storage
{
	class RemoteDataFrame: public DataFrame
	{
	public:
		size_t size() const override;
		bool empty() const override;

		common::SchemaType schema_type() const;

	private:
		friend class herd::RemoteBackend;
		struct make_shared_enabler;

		common::UUID uuid_;
		common::SchemaType schema_type_;

		size_t rows_count_;

		RemoteBackend& backend_;

		bool uploaded_ = false;

		RemoteDataFrame(
				common::UUID uuid, const std::string& name, size_t row_count,
				const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
				RemoteBackend& remote_backend);
		static std::shared_ptr<RemoteDataFrame> make_shared(
				common::UUID uuid, const std::string& name, size_t row_count,
				const std::vector<ColumnParameters>& columns, common::SchemaType schema_type,
				RemoteBackend& remote_backend);
	};

	struct RemoteDataFrame::make_shared_enabler: public RemoteDataFrame {
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
			:
			RemoteDataFrame(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_REMOTE_DATA_FRAME_HPP
