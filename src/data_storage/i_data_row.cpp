#include "herd/data_storage/i_data_row.hpp"


namespace herd::storage
{

	IDataRow::IDataRow(const std::shared_ptr<DataFrame> & frame)
	:
		frame_(frame)
	{
	}
}