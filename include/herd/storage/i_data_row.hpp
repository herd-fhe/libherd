#ifndef LIBHERD_I_DATA_ROW_HPP
#define LIBHERD_I_DATA_ROW_HPP

#include "herd/data_storage/data_frame.hpp"


namespace herd::storage
{
	class IDataRow
	{
	public:
		using column_key_type = typename DataFrame::column_key_type;

		virtual ~IDataRow() = default;

	private:
		explicit IDataRow(const std::shared_ptr<DataFrame>& frame);

		std::weak_ptr<DataFrame> frame_;
	};
}

#endif //LIBHERD_I_DATA_ROW_HPP
