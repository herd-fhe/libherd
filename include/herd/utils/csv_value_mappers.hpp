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
		template<typename T, typename... Ts> inline constexpr bool is_in_v = std::disjunction_v<std::is_same<T, Ts>...>;

		template<typename T>
		concept CSVMapperSignedType = is_in_v<T, int8_t, int16_t, int32_t, int64_t>;
		template<typename T>
		concept CSVMapperUnsignedType = is_in_v<T, uint8_t, uint16_t, uint32_t, uint64_t>;

		template<typename T>
		concept CSVMapperSupportedTypes = CSVMapperSignedType<T> || CSVMapperUnsignedType<T> || std::is_same_v<T, bool>;
	}

	struct InvalidCsvValueError: public std::runtime_error
	{
		using runtime_error::runtime_error;
	};

	struct UnsupportedMapper: public std::runtime_error
	{
		using runtime_error::runtime_error;
	};

	namespace csv_value_mappers
	{
		//mappers

		template<typename Type>
		[[nodiscard]] static Type parse(const std::string_view&)
		{
			throw UnsupportedMapper("Mapper not supported");
		}

		template<detail::CSVMapperSignedType Type>
		[[nodiscard]] static Type parse(const std::string_view &raw_view)
		{
			long long value;
			const auto begin = raw_view.data();
			const auto end = begin + raw_view.size();

			if (
					const auto result = std::from_chars(begin, end, value);
					result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range
					|| result.ptr != end || !std::in_range<Type>(value)
	        )
			{
				throw InvalidCsvValueError("Invalid value for provided field type.");
			}

			return static_cast<Type>(value);
		}

		template<detail::CSVMapperUnsignedType Type>
		[[nodiscard]] static Type parse(const std::string_view &raw_view)
		{
			unsigned long value;
			const auto begin = raw_view.data();
			const auto end = begin + raw_view.size();

			if (
					const auto result = std::from_chars(begin, end, value);
					result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range
					|| result.ptr != end || !std::in_range<Type>(value)
	        )
			{
				throw InvalidCsvValueError("Invalid value for provided field type.");
			}

			return static_cast<Type>(value);
		}

		template<typename Type>
		[[nodiscard]] bool parse(const std::string_view &raw_view)
		requires std::is_same_v<bool, Type>
		{
			if (raw_view.size() != 1 || (raw_view[0] != '0' && raw_view[0] != '1'))
			{
				throw InvalidCsvValueError("Invalid value for BIT field");
			}

			return raw_view[0] == '1';
		}
	};
}
#endif //LIBHERD_CSV_VALUE_MAPPERS_HPP
