#ifndef LIBHERD_STRING_UTILS_HPP
#define LIBHERD_STRING_UTILS_HPP

#include <string_view>
#include <charconv>


namespace herd::utils
{
	std::string_view next_line(std::string_view::const_iterator start, std::string_view::const_iterator end) noexcept;
	std::string_view trim(std::string_view text) noexcept;

	template<typename T>
	T parse_num(std::string_view num_str)
	{
		T value{};

		[[maybe_unused]] auto [ptr, ec] = std::from_chars(std::cbegin(num_str), std::cend(num_str), value);

		assert(ec == std::errc());

		return value;
	}
}

#endif //LIBHERD_STRING_UTILS_HPP
