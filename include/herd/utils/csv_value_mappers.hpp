#ifndef LIBHERD_CSV_VALUE_MAPPERS_HPP
#define LIBHERD_CSV_VALUE_MAPPERS_HPP

#include <string_view>
#include <charconv>
#include <stdexcept>
#include <utility>
#include <limits>


namespace herd::utils
{
	namespace detail
	{
		template<typename T, typename... Ts> using is_in = std::disjunction<std::is_same<T, Ts>...>;
		template<typename T, typename... Ts> inline constexpr bool is_in_v = is_in<T, Ts...>::value;

		template<typename T>
		concept CSVMapperSignedType = is_in_v<T, int8_t, int16_t, int32_t, int64_t>;
		template<typename T>
		concept CSVMapperUnsignedType = is_in_v<T, uint8_t, uint16_t, uint32_t, uint64_t>;

		template<typename T>
		concept CSVMapperSupportedTypes = CSVMapperSignedType<T> || CSVMapperUnsignedType<T> || std::is_same_v<T, bool>;
	}

	struct invalid_csv_value_error : public std::runtime_error
	{
		explicit invalid_csv_value_error(const std::string &error)
		: std::runtime_error(error)
		{};
	};

	namespace csv_value_mappers
	{
		//mappers

		template<typename Type>
		[[nodiscard]] static Type parse(const std::string_view&)
		{
			throw std::runtime_error("Mapper not supported");
		}

		template<detail::CSVMapperSignedType Type>
		[[nodiscard]] static Type parse(const std::string_view &raw_view)
		{
			long long value;
			const auto begin = raw_view.data();
			const auto end = begin + raw_view.size();

			const auto result = std::from_chars(begin, end, value);
			if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range
			|| result.ptr != end || !std::in_range<Type>(value))
			{
				throw invalid_csv_value_error("Invalid value for provided field type.");
			}

			return static_cast<Type>(value);
		}

		template<detail::CSVMapperUnsignedType Type>
		[[nodiscard]] static Type parse(const std::string_view &raw_view)
		{
			unsigned long value;
			const auto begin = raw_view.data();
			const auto end = begin + raw_view.size();

			const auto result = std::from_chars(begin, end, value);
			if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range
			|| result.ptr != end || !std::in_range<Type>(value))
			{
				throw invalid_csv_value_error("Invalid value for provided field type.");
			}

			return static_cast<Type>(value);
		}

		template<typename Type>
		[[nodiscard]] bool parse(const std::string_view &raw_view)
		requires std::is_same_v<bool, Type>
		{
			if (raw_view.size() != 1 || (raw_view[0] != '0' && raw_view[0] != '1'))
			{
				throw invalid_csv_value_error("Invalid value for BIT field");
			}

			return raw_view[0] == '1';
		}
	};
}
#endif //LIBHERD_CSV_VALUE_MAPPERS_HPP
