#include "herd/backend/remote/detail/mapper.hpp"


namespace herd::mapper
{
	common::SchemaType to_model(proto::SchemaType type) noexcept
	{
		switch(type)
		{
			case proto::BINFHE:
				return common::SchemaType::BINFHE;
			default:
				assert(false && "Proto schema, model mismatch");
				return static_cast<common::SchemaType>(0);
		}
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

	google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> to_proto(const std::vector<storage::DataTable::ColumnParameters>& columns)
	{
		assert(std::numeric_limits<uint8_t>::max() >= columns.size() && "Columns limit");

		google::protobuf::RepeatedPtrField<proto::ColumnDescriptor> proto_columns;

		proto_columns.Reserve(static_cast<int>(columns.size()));

		for (const auto& column: columns)
		{
			const auto proto_column = proto_columns.Add();
			proto_column->set_name(column.name);
			proto_column->set_type(to_proto(column.type));
		}

		return proto_columns;
	}
}