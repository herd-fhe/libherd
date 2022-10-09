#ifndef LIBHERD_I_DATA_ROW_HPP
#define LIBHERD_I_DATA_ROW_HPP

#include "herd/data_storage/data_table.hpp"


namespace herd::storage
{
	class IDataRow
	{
	public:
		using column_key_type = typename DataTable::column_key_type;

		virtual ~IDataRow() = default;

	private:
		explicit IDataRow(const std::shared_ptr<DataTable>& table);

		std::weak_ptr<DataTable> table_;

	};
}

#endif //LIBHERD_I_DATA_ROW_HPP
