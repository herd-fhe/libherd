#ifndef LIBHERD_CSV_READER_HPP
#define LIBHERD_CSV_READER_HPP

#include <istream>
#include <vector>

#include "herd/common/native_type_mapping.hpp"
#include "herd/utils/csv_value_mappers.hpp"


namespace herd::utils
{
	struct CSVOptions
	{
		char delimiter = ',';
	};

	class CSVRow
	{
	public:
		template<typename Type>
		[[nodiscard]] Type get(size_t column) const;

		[[nodiscard]] bool empty() const noexcept;
		[[nodiscard]] std::size_t size() const noexcept;

	private:
		std::string raw_row_;
		std::vector<std::string_view> raw_values_;

		friend class CSVReader;
	};

	template<typename Type>
	Type CSVRow::get(size_t column) const
	{
		return csv_value_mappers::parse<Type>(raw_values_[column]);
	}

	class CSVReader
	{
	public:
		explicit CSVReader(CSVOptions options = {});

		CSVRow read_row(std::istream& stream) const;
		[[nodiscard]] static std::size_t row_count(std::istream& stream);

	private:
		CSVOptions options_;
	};
}

#endif //LIBHERD_CSV_READER_HPP
