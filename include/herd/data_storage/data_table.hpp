#ifndef LIBHERD_DATA_TABLE_HPP
#define LIBHERD_DATA_TABLE_HPP

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "herd/type.hpp"


namespace herd::utils
{
	class CSVRow;
}

namespace herd::storage
{
	class DataTable: public std::enable_shared_from_this<DataTable>
	{
	public:
		using row_index_type = std::size_t;
		using column_key_type = std::string;
		using column_index_type = std::size_t;
		using column_type_key_type = DataType;

		struct ColumnDescriptor {
			column_index_type index;
			column_type_key_type type;

			ColumnDescriptor(column_index_type idx, column_type_key_type type_key):
				index(idx), type(type_key)
			{}

			std::strong_ordering operator<=>(const ColumnDescriptor& rhs) const
			{
				return index <=> rhs.index;
			}
		};

		struct ColumnParameters {
			std::string name;
			column_type_key_type type;
		};

		explicit DataTable(std::string name, const std::vector<ColumnParameters>& columns);

		virtual ~DataTable() = default;

		const std::string& name() const;

		[[nodiscard]] virtual size_t size() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;

		[[nodiscard]] const std::map<column_key_type, ColumnDescriptor, std::less<>>& columns() const;

		virtual void flush_rows() = 0;

	protected:
		friend class DataStorage;

		std::string name_;
		std::map<column_key_type, ColumnDescriptor, std::less<>> column_descriptors_;

		virtual void add_row(const utils::CSVRow& row) = 0;
	};
} // namespace herd::storage

namespace std
{
	template<>
	struct hash<herd::storage::DataTable::ColumnDescriptor> {
		size_t operator()(const herd::storage::DataTable::ColumnDescriptor& descriptor) const
		{
			using namespace herd::storage;

			return std::hash<DataTable::column_index_type>{}(descriptor.index);
		}
	};
} // namespace std

#endif //LIBHERD_DATA_TABLE_HPP
