#include "herd/data_storage/i_data_row.hpp"


namespace herd::storage
{

	IDataRow::IDataRow(const std::shared_ptr<DataTable> &table)
	:table_(table)
	{
	}
}