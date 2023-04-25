#ifndef LIBHERD_LOCAL_DATA_FRAME_HPP
#define LIBHERD_LOCAL_DATA_FRAME_HPP

#include "herd/data_storage/data_frame.hpp"
#include "herd/data_storage/local/type_pool.hpp"


namespace herd::storage
{
	class LocalDataFrame: public DataFrame
	{
	public:
		size_t size() const override;
		bool empty() const override;

	private:
		void add_row(const utils::CSVRow &row);
		void flush_rows();

		friend class LocalDataStorage;
		struct make_shared_enabler;

		LocalDataFrame(const UUID& uuid, std::string name, const std::vector<ColumnParameters>& columns);
		static std::shared_ptr<LocalDataFrame> make_shared(const UUID& uuid, std::string name, const std::vector<ColumnParameters>& columns);

		std::vector<std::unique_ptr<ITypePool>> pools_;

	};

	struct LocalDataFrame::make_shared_enabler: public LocalDataFrame {
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
				:
			LocalDataFrame(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_LOCAL_DATA_FRAME_HPP
