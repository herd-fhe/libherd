#ifndef LIBHERD_TYPE_HPP
#define LIBHERD_TYPE_HPP


namespace herd
{
	enum class TypeKey
	{
		BIT,

		UINT8,
		UINT16,
		UINT32,
		UINT64,

		INT8,
		INT16,
		INT32,
		INT64
	};

	namespace detail
	{
		using enum TypeKey;

		template<TypeKey type_key>
		struct NativeTypeMapping
		{
			constexpr static TypeKey key = type_key;
			using native_type = std::false_type;
		};

		template<>
		struct NativeTypeMapping<BIT>
		{
			constexpr static TypeKey key =BIT;
			using native_type = bool;
		};

		template<>
		struct NativeTypeMapping<UINT8>
		{
			constexpr static TypeKey key = UINT8;
			using native_type = uint8_t;
		};

		template<>
		struct NativeTypeMapping<UINT16>
		{
			constexpr static TypeKey key = UINT16;
			using native_type = uint16_t;
		};

		template<>
		struct NativeTypeMapping<UINT32>
		{
			constexpr static TypeKey key = UINT32;
			using native_type = uint32_t;
		};

		template<>
		struct NativeTypeMapping<UINT64>
		{
			constexpr static TypeKey key = UINT64;
			using native_type = uint64_t;
		};

		template<>
		struct NativeTypeMapping<INT8>
		{
			constexpr static TypeKey key = INT8;
			using native_type = int8_t;
		};

		template<>
		struct NativeTypeMapping<INT16>
		{
			constexpr static TypeKey key = INT16;
			using native_type = int16_t;
		};

		template<>
		struct NativeTypeMapping<INT32>
		{
			constexpr static TypeKey key = INT32;
			using native_type = int32_t;
		};

		template<>
		struct NativeTypeMapping<INT64>
		{
			constexpr static TypeKey key = INT64;
			using native_type = int64_t;
		};
	}

	template<TypeKey type_key>
	struct NativeTypeMapping
	{
		constexpr static TypeKey key = detail::NativeTypeMapping<type_key>::key;
		using native_type = typename detail::NativeTypeMapping<type_key>::native_type;

		static_assert(!std::is_same_v<native_type, std::false_type>);
	};
}

#endif //LIBHERD_TYPE_HPP
