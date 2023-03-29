#ifndef LIBHERD_CRYPTO_FACTORY_HPP
#define LIBHERD_CRYPTO_FACTORY_HPP

#include <map>
#include <set>
#include <functional>

#include "i_crypto.hpp"
#include "i_keyset.hpp"


namespace herd::crypto
{
	class CryptoFactory
	{
	public:
		using FactoryMethod = std::function<std::unique_ptr<ICrypto>(IKeyset&)>;

		CryptoFactory() = delete;

		static bool register_crypto(common::SchemaType type, FactoryMethod method);
		static bool is_registered(common::SchemaType type) noexcept;
		static std::set<common::SchemaType> registered_schemas() noexcept;

		static std::unique_ptr<ICrypto> create_crypto(IKeyset& keyset);

	private:
		static std::unordered_map<common::SchemaType, FactoryMethod>& get_internal_map();
	};
}

#endif //LIBHERD_CRYPTO_FACTORY_HPP
