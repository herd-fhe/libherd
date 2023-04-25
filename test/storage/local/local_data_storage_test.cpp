#include <gtest/gtest.h>
#include <herd/data_storage/local/local_data_storage.hpp>

#include <sstream>

#include <herd/type.hpp>

using namespace herd::storage;


TEST(LocalDataStorage, load_from_csv_stream)
{
	using enum herd::DataType;

	LocalDataStorage storage;

	std::stringstream stream;
	stream << "0,10,-21,256";

	std::vector<DataFrame::ColumnParameters> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	auto frame = storage.load_from_csv(columns, stream, herd::common::SchemaType::NONE).get();

	EXPECT_NE(nullptr, frame);
	EXPECT_EQ(4, frame->columns().size());

	EXPECT_EQ(1, frame->size());
}

TEST(LocalDataStorage, load_from_csv_stream_multiline)
{
	using enum herd::DataType;

	LocalDataStorage storage;

	std::stringstream stream;
	stream << "0,10,-21,256";
	stream << "\n";
	stream << "1,129,0,11111111";

	std::vector<DataFrame::ColumnParameters> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	auto frame = storage.load_from_csv(columns, stream, herd::common::SchemaType::NONE).get();

	EXPECT_NE(nullptr, frame);
	EXPECT_EQ(4, frame->columns().size());

	EXPECT_EQ(2, frame->size());
}

TEST(LocalDataStorage, not_none_schema)
{
	using enum herd::DataType;

	LocalDataStorage storage;

	std::stringstream stream;
	stream << "0,10,-21,256";
	stream << "\n";
	stream << "1,129,0,11111111";

	std::vector<DataFrame::ColumnParameters> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	EXPECT_THROW(storage.load_from_csv(columns, stream, herd::common::SchemaType::BINFHE), std::runtime_error);
}
