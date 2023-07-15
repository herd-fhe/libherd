#include "herd/utils/string_utils.hpp"

#include <locale>


namespace herd::utils
{
	std::string_view next_line(std::string_view::const_iterator start, std::string_view::const_iterator end) noexcept
	{
		auto line_end = start;
		while(line_end != end)
		{
			if(*line_end == '\n')
			{
				++line_end;
				break;
			}

			++line_end;
		}

		return {start, line_end};
	}

	std::string_view trim(std::string_view text) noexcept
	{
		auto start = std::cbegin(text);
		const auto end = std::cend(text);
		while(start != end)
		{
			if(!std::isspace(*start))
			{
				break;
			}
			++start;
		}

		auto rstart = std::crbegin(text);
		const auto rend = std::crend(text);
		while(rstart != rend)
		{
			if(!std::isspace(*rstart))
			{
				break;
			}
			++rstart;
		}

		return {start, rstart.base()};
	}
}