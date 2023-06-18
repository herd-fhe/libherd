#include <gtest/gtest.h>
#include <herd/storage/local/detail/data_storage.hpp>

#include <sstream>

#include <herd/type.hpp>

using namespace herd::storage::local::detail;


TEST(LocalDataStorage, load_from_csv_stream)
{
	using enum herd::common::DataType;

	DataStorageImpl storage;

	std::stringstream stream;
	stream << "0,10,-21,256";

	std::vector<herd::common::ColumnMeta> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	herd::storage::ImportOptions options;
	options.set_columns(columns);
	options.set_schema(herd::common::SchemaType::NONE);
	options.set_partitions(2);

	auto frame = storage.import_from_csv(stream, options).get();

	EXPECT_NE(nullptr, frame);
	EXPECT_EQ(4, frame->columns().size());

	EXPECT_EQ(1, frame->size());
}

TEST(LocalDataStorage, load_from_csv_stream_multiline)
{
	using enum herd::common::DataType;

	DataStorageImpl storage;

	std::stringstream stream;
	stream << "0,10,-21,256";
	stream << "\n";
	stream << "1,129,0,11111111";

	std::vector<herd::common::ColumnMeta> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	herd::storage::ImportOptions options;
	options.set_columns(columns);
	options.set_schema(herd::common::SchemaType::NONE);
	options.set_partitions(2);

	auto frame = storage.import_from_csv(stream, options).get();

	EXPECT_NE(nullptr, frame);
	EXPECT_EQ(4, frame->columns().size());

	EXPECT_EQ(2, frame->size());
}

TEST(LocalDataStorage, not_none_schema)
{
	using enum herd::common::DataType;

	DataStorageImpl storage;

	std::stringstream stream;
	stream << "0,10,-21,256";
	stream << "\n";
	stream << "1,129,0,11111111";

	std::vector<herd::common::ColumnMeta> columns = {
			{"first", BIT},
			{"second", UINT8},
			{"third", INT16},
			{"fourth", INT64},
	};

	herd::storage::ImportOptions options;
	options.set_columns(columns);
	options.set_schema(herd::common::SchemaType::BINFHE);
	options.set_partitions(2);

	EXPECT_THROW(static_cast<void>(storage.import_from_csv(stream, options)), std::runtime_error);
}
