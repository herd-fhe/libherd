#include <gtest/gtest.h>
#include <herd/utils/csv_reader.hpp>

#include <sstream>
#include <ios>

using namespace herd::utils;


TEST(CSVReader, default_delimeter)
{
	const CSVReader reader;

	{
		std::stringstream stream;

		auto row = reader.read_row(stream);

		EXPECT_TRUE(row.empty());
		EXPECT_EQ(0, row.size());
	}

	{
		std::stringstream stream;
		stream << "10,20,30,40,50,60,70";

		auto row = reader.read_row(stream);

		EXPECT_FALSE(row.empty());
		EXPECT_EQ(7, row.size());
	}

	{
		std::stringstream stream;
		stream << "10,20;30,40 50,60.70";

		auto row = reader.read_row(stream);

		EXPECT_FALSE(row.empty());
		EXPECT_EQ(4, row.size());
	}
}

TEST(CSVReader, default_delimeter_multiline)
{
	const CSVReader reader;

	{
		std::stringstream stream;
		stream << "10,20,30,50,60,70";
		stream << "\n";
		stream << "10,30,40,50,70";

		auto row_1 = reader.read_row(stream);
		auto row_2 = reader.read_row(stream);

		EXPECT_FALSE(row_1.empty());
		EXPECT_EQ(6, row_1.size());

		EXPECT_FALSE(row_2.empty());
		EXPECT_EQ(5, row_2.size());
	}
}

TEST(CSVReader, custom_delimeter)
{
	const CSVReader reader({';'});

	{
		std::stringstream stream;

		auto row = reader.read_row(stream);

		EXPECT_TRUE(row.empty());
		EXPECT_EQ(0, row.size());
	}

	{
		std::stringstream stream;
		stream << "10;20;30;50;60;70";

		auto row = reader.read_row(stream);

		EXPECT_FALSE(row.empty());
		EXPECT_EQ(6, row.size());
	}

	{
		std::stringstream stream;
		stream << "10,20,30,40,50,60,70";

		auto row = reader.read_row(stream);

		EXPECT_FALSE(row.empty());
		EXPECT_EQ(1, row.size());
	}

	{
		std::stringstream stream;
		stream << "10;20;30,40;50 60;70";

		auto row = reader.read_row(stream);

		EXPECT_FALSE(row.empty());
		EXPECT_EQ(5, row.size());
	}
}

TEST(CSVReader, empty_on_bad_stream)
{
	const CSVReader reader;
	{
		std::stringstream stream;
		stream.setstate(std::ios_base::badbit);

		auto row = reader.read_row(stream);

		EXPECT_TRUE(row.empty());
		EXPECT_EQ(0, row.size());
	}
}