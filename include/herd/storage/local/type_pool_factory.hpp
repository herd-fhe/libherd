#ifndef LIBHERD_TYPE_POOL_FACTORY_HPP
#define LIBHERD_TYPE_POOL_FACTORY_HPP

#include <functional>
#include <memory>
#include <unordered_map>

#include "herd/data_storage/local/type_pool.hpp"
#include "herd/type.hpp"


namespace herd::storage
{
	class TypePoolFactory
	{
	public:
		template<common::DataType key>
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

		[[nodiscard]] static std::unique_ptr<ITypePool> create_pool(common::DataType key);

	private:
		static std::unordered_map<common::DataType, std::function<std::unique_ptr<ITypePool>()>> factory_register;

		static bool register_type(common::DataType key, std::function<std::unique_ptr<ITypePool>()> factory_fun);
	};
}

#endif //LIBHERD_TYPE_POOL_FACTORY_HPP
