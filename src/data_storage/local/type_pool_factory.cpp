#include "herd/data_storage/local/type_pool_factory.hpp"


namespace herd::storage
{
	std::unordered_map<TypeKey, std::function<std::unique_ptr<ITypePool>()>> TypePoolFactory::factory_register;

	std::unique_ptr<ITypePool> TypePoolFactory::create_pool(TypeKey key)
	{
		if(auto iter = factory_register.find(key); iter != std::end(factory_register))
		{
			return iter->second();
		}
		return nullptr;
	}

	bool TypePoolFactory::register_type(TypeKey key, std::function<std::unique_ptr<ITypePool>()> factory_fun)
	{
		if(auto iter = factory_register.find(key); iter == std::end(factory_register))
		{
			factory_register[key] = std::move(factory_fun);
			return true;
		}

		return false;
	}

	namespace {
		[[maybe_unused]] const bool bit_registered = TypePoolFactory::register_type<TypeKey::BIT>();

		[[maybe_unused]] const bool uint8_registered = TypePoolFactory::register_type<TypeKey::UINT8>();
		[[maybe_unused]] const bool uint16_registered = TypePoolFactory::register_type<TypeKey::UINT16>();
		[[maybe_unused]] const bool uint32_registered = TypePoolFactory::register_type<TypeKey::UINT32>();
		[[maybe_unused]] const bool uint64_registered = TypePoolFactory::register_type<TypeKey::UINT64>();

		[[maybe_unused]] const bool int8_registered = TypePoolFactory::register_type<TypeKey::INT8>();
		[[maybe_unused]] const bool int16_registered = TypePoolFactory::register_type<TypeKey::INT16>();
		[[maybe_unused]] const bool int32_registered = TypePoolFactory::register_type<TypeKey::INT32>();
		[[maybe_unused]] const bool int64_registered = TypePoolFactory::register_type<TypeKey::INT64>();
	}
}