#include <gtest/gtest.h>
#include <herd/utils/csv_value_mappers.hpp>

using namespace herd::utils;


TEST(CSVValueMappers, unsigned_values_uint8_t)
{
	static_assert(std::is_same_v<uint8_t, decltype(csv_value_mappers::parse<uint8_t>({}))>);

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint8_t>(view);

		EXPECT_EQ(0u, result);
	}

	{
		const std::string text = "10";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint8_t>(view);

		EXPECT_EQ(10u, result);
	}

	{
		const std::string text = "128";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint8_t>(view);

		EXPECT_EQ(128u, result);
	}

	{
		const std::string text = "255";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint8_t>(view);

		EXPECT_EQ(255u, result);
	}
}

TEST(CSVValueMappers, unsigned_values_uint16_t)
{
	static_assert(std::is_same_v<uint16_t, decltype(csv_value_mappers::parse<uint16_t>({}))>);

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint16_t>(view);

		EXPECT_EQ(0u, result);
	}

	{
		const std::string text = "22";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint16_t>(view);

		EXPECT_EQ(22u, result);
	}

	{
		const std::string text = "32767";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint16_t>(view);

		EXPECT_EQ(32767u, result);
	}

	{
		const std::string text = "65535";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint16_t>(view);

		EXPECT_EQ(65535u, result);
	}
}

TEST(CSVValueMappers, unsigned_values_uint32_t)
{
	static_assert(std::is_same_v<uint32_t, decltype(csv_value_mappers::parse<uint32_t>({}))>);

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint32_t>(view);

		EXPECT_EQ(0u, result);
	}

	{
		const std::string text = "675";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint32_t>(view);

		EXPECT_EQ(675u, result);
	}

	{
		const std::string text = "2147483647";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint32_t>(view);

		EXPECT_EQ(2147483647u, result);
	}

	{
		const std::string text = "4294967295";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint32_t>(view);

		EXPECT_EQ(4294967295u, result);
	}
}

TEST(CSVValueMappers, unsigned_values_uint64_t)
{
	static_assert(std::is_same_v<uint64_t, decltype(csv_value_mappers::parse<uint64_t>({}))>);

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint64_t>(view);

		EXPECT_EQ(0u, result);
	}

	{
		const std::string text = "6765";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint64_t>(view);

		EXPECT_EQ(6765u, result);
	}

	{
		const std::string text = "2147483647";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint64_t>(view);

		EXPECT_EQ(2147483647, result);
	}

	{
		const std::string text = "18446744073709551615";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<uint64_t>(view);

		EXPECT_EQ(18446744073709551615u, result);
	}
}


TEST(CSVValueMappers, signed_values_int8_t)
{
	static_assert(std::is_same_v<int8_t, decltype(csv_value_mappers::parse<int8_t>({}))>);

	{
		const std::string text = "-128";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int8_t>(view);

		EXPECT_EQ(-128, result);
	}

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int8_t>(view);

		EXPECT_EQ(0, result);
	}

	{
		const std::string text = "22";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int8_t>(view);

		EXPECT_EQ(22, result);
	}

	{
		const std::string text = "127";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int8_t>(view);

		EXPECT_EQ(127, result);
	}
}

TEST(CSVValueMappers, signed_values_int16_t)
{
	static_assert(std::is_same_v<int16_t, decltype(csv_value_mappers::parse<int16_t>({}))>);

	{
		const std::string text = "-32768";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int16_t>(view);

		EXPECT_EQ(-32768, result);
	}

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int16_t>(view);

		EXPECT_EQ(0, result);
	}

	{
		const std::string text = "32767";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int16_t>(view);

		EXPECT_EQ(32767, result);
	}

	{
		const std::string text = "32767";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int16_t>(view);

		EXPECT_EQ(32767, result);
	}
}

TEST(CSVValueMappers, signed_values_int32_t)
{
	static_assert(std::is_same_v<int32_t, decltype(csv_value_mappers::parse<int32_t>({}))>);

	{
		const std::string text = "-2147483648";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int32_t>(view);

		EXPECT_EQ(-2147483648, result);
	}

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int32_t>(view);

		EXPECT_EQ(0, result);
	}

	{
		const std::string text = "23452";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int32_t>(view);

		EXPECT_EQ(23452, result);
	}

	{
		const std::string text = "2147483647";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int32_t>(view);

		EXPECT_EQ(2147483647, result);
	}
}

TEST(CSVValueMappers, signed_values_int64_t)
{
	static_assert(std::is_same_v<int64_t, decltype(csv_value_mappers::parse<int64_t>({}))>);

	{
		const std::string text = "-9223372036854775808";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int64_t>(view);

		EXPECT_EQ(-9223372036854775807LL - 1LL, result);
	}

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int64_t>(view);

		EXPECT_EQ(0, result);
	}

	{
		const std::string text = "2147483647";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int64_t>(view);

		EXPECT_EQ(2147483647, result);
	}

	{
		const std::string text = "9223372036854775807";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<int64_t>(view);

		EXPECT_EQ(9223372036854775807, result);
	}
}

TEST(CSVValueMappers, unsigned_values_invalid_value)
{
	{
		const std::string text = "-10";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint64_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10v";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10s";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10b";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint64_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "ba10v";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "gh10s";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "fd10b";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint64_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "bav";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "ghs";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "fdb";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint64_t>(view)), InvalidCsvValueError);
	}
}

TEST(CSVValueMappers, unsigned_values_overflow)
{
	{
		const std::string text = "256";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "65536";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "4294967296";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "18446744073709551616";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<uint64_t>(view)), InvalidCsvValueError);
	}
}

TEST(CSVValueMappers, signed_values_invalid_value)
{
	{
		const std::string text = "-10v";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-10s";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10b";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int64_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "ba10v";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "gh10s";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "10a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "fd10b";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int64_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "bav";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "ghs";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "fdb";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int64_t>(view)), InvalidCsvValueError);
	}
}

TEST(CSVValueMappers, signed_values_overflow)
{
	{
		const std::string text = "128";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "32768";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "2147483648";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "9223372036854775808";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int64_t>(view)), InvalidCsvValueError);
	}
}

TEST(CSVValueMappers, signed_values_overflow_minus)
{
	{
		const std::string text = "-129";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int8_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-32769";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int16_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-2147483649";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int32_t>(view)), InvalidCsvValueError);
	}

	{
		const std::string text = "-9223372036854775809";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<int64_t>(view)), InvalidCsvValueError);
	}
}

TEST(CSVValueMappers, boolean_value)
{
	static_assert(std::is_same_v<bool, decltype(csv_value_mappers::parse<bool>({}))>);

	{
		const std::string text = "0";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<bool>(view);

		EXPECT_EQ(false, result);
	}

	{
		const std::string text = "1";
		const std::string_view view = text;

		const auto result = csv_value_mappers::parse<bool>(view);

		EXPECT_EQ(true, result);
	}
}

TEST(CSVValueMappers, boolean_invalid_value)
{
	{
		const std::string text = "a";
		const std::string_view view = text;

		EXPECT_THROW(static_cast<void>(csv_value_mappers::parse<bool>(view)), InvalidCsvValueError);
	}
}
