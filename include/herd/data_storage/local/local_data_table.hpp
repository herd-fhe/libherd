#ifndef LIBHERD_LOCAL_DATA_TABLE_HPP
#define LIBHERD_LOCAL_DATA_TABLE_HPP

#include "herd/data_storage/data_table.hpp"
#include "herd/data_storage/local/type_pool.hpp"


namespace herd::storage
{
	class LocalDataTable: public DataTable
	{
	public:
		size_t size() const override;
		bool empty() const override;

	private:
		void add_row(const utils::CSVRow &row);
		void flush_rows();

		friend class LocalDataStorage;
		struct make_shared_enabler;

		LocalDataTable(const UUID& uuid, std::string name, const std::vector<ColumnParameters>& columns);
		static std::shared_ptr<LocalDataTable> make_shared(const UUID& uuid, std::string name, const std::vector<ColumnParameters>& columns);

		std::vector<std::unique_ptr<ITypePool>> pools_;

	};

	struct LocalDataTable::make_shared_enabler: public LocalDataTable
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
				:   LocalDataTable(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_LOCAL_DATA_TABLE_HPP
