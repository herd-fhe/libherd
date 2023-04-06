#include "herd/crypto/crypto_factory.hpp"
#include <cassert>


namespace herd::crypto
{
	bool CryptoFactory::register_crypto(common::SchemaType type, CryptoFactory::FactoryMethod method)
	{
		auto& implementations = get_internal_map();
		assert(implementations.contains(type) && "Schema already registered");

		implementations[type] = std::move(method);
		return true;
	}

	bool CryptoFactory::is_registered(common::SchemaType type) noexcept
	{
		const auto& implementations = get_internal_map();
		return implementations.contains(type);
	}

	std::set<common::SchemaType> CryptoFactory::registered_schemas() noexcept
	{
		std::set<common::SchemaType> schemas;
		for (const auto& implementations = get_internal_map(); const auto&[key, val]: implementations)
		{
			schemas.insert(key);
		}

		return schemas;
	}

	std::unique_ptr<ICrypto> CryptoFactory::create_crypto(herd::crypto::IKeyset& keyset)
	{
		const auto schema_type = keyset.get_schema_type();

		assert(is_registered(schema_type) && "Schema type not registered");
		const auto& implementations = get_internal_map();

		const auto concrete_factory = implementations.find(schema_type);
		assert(concrete_factory != std::end(implementations));

		return concrete_factory->second(keyset);
	}

	std::unordered_map<common::SchemaType, CryptoFactory::FactoryMethod>& CryptoFactory::get_internal_map()
	{
		static std::unordered_map<common::SchemaType, FactoryMethod> implementations_;
		return implementations_;
	}
}