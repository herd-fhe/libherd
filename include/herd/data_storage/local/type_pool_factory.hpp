#ifndef LIBHERD_TYPE_POOL_FACTORY_HPP
#define LIBHERD_TYPE_POOL_FACTORY_HPP

#include <unordered_map>
#include <memory>
#include <functional>

#include "herd/type.hpp"
#include "herd/data_storage/local/type_pool.hpp"


namespace herd::storage
{
	class TypePoolFactory
	{
	public:
		template<DataType key>
		[[nodiscard]] static bool register_type()
		{
			using mapping = NativeTypeMapping<key>;
			return register_type(
					mapping::key,
					[]() -> std::unique_ptr<ITypePool>
					{
						return std::make_unique<TypePool<typename mapping::native_type>>();
					}
			);
		}

		[[nodiscard]] static std::unique_ptr<ITypePool> create_pool(DataType key);

	private:
		static std::unordered_map<DataType, std::function<std::unique_ptr<ITypePool>()>> factory_register;

		static bool register_type(DataType key, std::function<std::unique_ptr<ITypePool>()> factory_fun);
	};
}

#endif //LIBHERD_TYPE_POOL_FACTORY_HPP
