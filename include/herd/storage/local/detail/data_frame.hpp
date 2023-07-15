#ifndef LIBHERD_LOCAL_DATA_FRAME_HPP
#define LIBHERD_LOCAL_DATA_FRAME_HPP

#include "herd/storage/data_frame.hpp"
#include "herd/storage/local/type_pool.hpp"


namespace herd::storage::local::detail
{
	class DataFrameImpl: public DataFrame
	{
	public:
		DataFrameImpl(const common::UUID& uuid, const std::string& name, const std::vector<common::ColumnMeta>& columns);

		[[nodiscard]] std::size_t partitions() const override;
		[[nodiscard]] size_t size() const override;
		[[nodiscard]] bool empty() const override;

	private:
		std::vector<std::unique_ptr<ITypePool>> pools_;

		void add_row(const utils::CSVRow& row);
		void flush_rows();

		friend class DataStorageImpl;
	};
}

#endif //LIBHERD_LOCAL_DATA_FRAME_HPP
