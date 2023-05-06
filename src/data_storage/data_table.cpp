#include <sstream>
#include <utility>

#include "herd/data_storage/data_table.hpp"
#include "herd/utils/csv_reader.hpp"

namespace herd::storage
{
	DataTable::DataTable(std::string name, const std::vector<ColumnParameters> &columns)
	:name_(std::move(name))
	{
		for(std::size_t i = 0; const auto& column: columns)
		{
			column_descriptors_.try_emplace(
				column.name,
				i,
			    column.type
			);

			++i;
		}
	}

	const std::string &DataTable::name() const
	{
		return name_;
	}

	const std::map<DataTable::column_key_type, DataTable::ColumnDescriptor, std::less<>>& DataTable::columns() const
	{
		return column_descriptors_;
	}

	namespace
	{
		template<common::DataType key_type,
				 typename NativeType=typename NativeTypeMapping<key_type>::native_type,
				 std::size_t NativeTypeSize=NativeTypeMapping<key_type>::bit_size>
		std::vector<std::byte> do_encrypt_column_value(NativeType value, const crypto::ICrypto& crypto)
		{
			std::vector<bool> bit_representation;
			bit_representation.resize(NativeTypeSize);

			if constexpr(NativeTypeSize == 1)
			{
				bit_representation[0] = value;
			}
			else
			{
				for(std::size_t i = 0; i < NativeTypeSize; ++i)
				{
					bit_representation[i] = (value >> i) & 0b1;
				}
			}

			const auto encrypted_bits = crypto.encrypt(bit_representation);

			std::stringstream bits_stream;
			encrypted_bits->serialize(bits_stream);

			bits_stream.seekg(0, std::ios::beg);
			const auto start = bits_stream.tellg();
			bits_stream.seekg(0, std::ios::end);
			const auto size = static_cast<uint32_t>(bits_stream.tellg() - start);
			bits_stream.seekg(0, std::ios::beg);

			std::vector<std::byte> bytes;
			bytes.resize(size);
			bits_stream.read(reinterpret_cast<char*>(bytes.data()), std::streamsize(size));

			return bytes;
		}
	}

	std::vector<std::byte> DataTable::encrypt_row(const utils::CSVRow& row, const std::vector<DataTable::column_type_key_type>& columns, const crypto::ICrypto& crypto)
	{
		std::vector<std::byte> row_bytes;
		row_bytes.resize(sizeof(uint32_t));

		std::size_t column_index = 0;

		for(const auto& column: columns)
		{
			const column_type_key_type type_key = column;
			std::vector<std::byte> column_data;

			switch(type_key)
			{
				using enum common::DataType;

				case BIT:
					column_data = do_encrypt_column_value<BIT>(row.get<bool>(column_index), crypto);
					break;
				case UINT8:
					column_data = do_encrypt_column_value<UINT8>(row.get<uint8_t>(column_index), crypto);
					break;
				case UINT16:
					column_data = do_encrypt_column_value<UINT16>(row.get<uint16_t>(column_index), crypto);
					break;
				case UINT32:
					column_data = do_encrypt_column_value<UINT32>(row.get<uint32_t>(column_index), crypto);
					break;
				case UINT64:
					column_data = do_encrypt_column_value<UINT64>(row.get<uint64_t>(column_index), crypto);
					break;
				case INT8:
					column_data = do_encrypt_column_value<INT8>(row.get<int8_t>(column_index), crypto);
					break;
				case INT16:
					column_data = do_encrypt_column_value<INT16>(row.get<int16_t>(column_index), crypto);
					break;
				case INT32:
					column_data = do_encrypt_column_value<INT32>(row.get<int32_t>(column_index), crypto);
					break;
				case INT64:
					column_data = do_encrypt_column_value<INT64>(row.get<int64_t>(column_index), crypto);
			}

			row_bytes.insert(std::end(row_bytes), std::begin(column_data), std::end(column_data));

			++column_index;
		}

		const auto size = static_cast<uint32_t>(row_bytes.size() - 4u);

		std::copy_n(reinterpret_cast<const std::byte*>(&size), sizeof(size), std::begin(row_bytes));
		return row_bytes;
	}
}