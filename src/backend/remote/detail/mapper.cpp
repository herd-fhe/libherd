#include "herd/backend/remote/detail/mapper.hpp"


namespace herd::mapper
{
	common::SchemaType to_model(proto::SchemaType type)
	{
		switch(type)
		{
			case proto::BINFHE:
				return common::SchemaType::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	common::DataType to_model(proto::DataType data_type)
	{
		using herd::common::DataType;
		switch(data_type)
		{
			case herd::proto::BIT:
				return DataType::BIT;
			case herd::proto::UINT8:
				return DataType::UINT8;
			case herd::proto::UINT16:
				return DataType::UINT16;
			case herd::proto::UINT32:
				return DataType::UINT32;
			case herd::proto::UINT64:
				return DataType::UINT64;
			case herd::proto::INT8:
				return DataType::INT8;
			case herd::proto::INT16:
				return DataType::INT16;
			case herd::proto::INT32:
				return DataType::INT32;
			case herd::proto::INT64:
				return DataType::INT64;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	std::vector<storage::DataFrame::ColumnParameters> to_model(const google::protobuf::RepeatedPtrField<proto::ColumnDescriptor>& columns)
	{
		assert(std::numeric_limits<uint8_t>::max() >= columns.size() && "Columns limit");

		std::vector<storage::DataFrame::ColumnParameters> column_parameters;
		column_parameters.reserve(static_cast<std::size_t>(columns.size()));

		std::ranges::transform(columns, std::back_inserter(column_parameters),
			[](const auto& column)
			{
				return storage::DataFrame::ColumnParameters{ column.name(), to_model(column.type()) };
			}
        );

		return column_parameters;
	}

	proto::SchemaType to_proto(common::SchemaType type)
	{
		switch(type)
		{
			case common::SchemaType::BINFHE:
				return proto::BINFHE;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	proto::DataType to_proto(common::DataType data_type)
	{
		using herd::common::DataType;
		switch(data_type)
		{
			case DataType::BIT:
				return herd::proto::BIT;
			case DataType::UINT8:
				return herd::proto::UINT8;
			case DataType::UINT16:
				return herd::proto::UINT16;
			case DataType::UINT32:
				return herd::proto::UINT32;
			case DataType::UINT64:
				return herd::proto::UINT64;
			case DataType::INT8:
				return herd::proto::INT8;
			case DataType::INT16:
				return herd::proto::INT16;
			case DataType::INT32:
				return herd::proto::INT32;
			case DataType::INT64:
				return herd::proto::INT64;
			default:
				throw MappingError("Proto schema, model mismatch");
		}
	}

	google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<storage::DataFrame::ColumnParameters>& columns)
	{
		assert(std::numeric_limits<uint8_t>::max() >= columns.size() && "Columns limit");

		google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> proto_columns;
		proto_columns.Reserve(static_cast<int>(columns.size()));

		for(const auto& column: columns)
		{
			const auto proto_column = proto_columns.Add();
			proto_column->set_name(column.name);
			proto_column->set_type(to_proto(column.type));
		}

		return proto_columns;
	}
}