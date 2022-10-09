#include "herd/utils/csv_reader.hpp"

#include <algorithm>


namespace herd::utils
{
	bool CSVRow::empty() const noexcept
	{
		return raw_values_.empty();
	}

	std::size_t CSVRow::size() const noexcept
	{
		return raw_values_.size();
	}

	CSVReader::CSVReader(CSVOptions options)
	:options_(options)
	{}

	CSVRow CSVReader::read_row(std::istream &stream) const
	{
		CSVRow row;

		if(!std::getline(stream, row.raw_row_))
		{
			return row;
		}

		auto iter = std::cbegin(row.raw_row_);

		while(iter != std::cend(row.raw_row_))
		{
			const auto delim_pos = std::find(iter, std::cend(row.raw_row_), options_.delimiter);

			if(iter != delim_pos)
			{
				row.raw_values_.emplace_back(iter, delim_pos);
			}

			if(delim_pos == std::cend(row.raw_row_))
			{
				break;
			}

			iter = std::next(delim_pos);
		}

		return row;
	}
}