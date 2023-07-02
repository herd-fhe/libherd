#ifndef LIBHERD_CRYPTO_FACTORY_HPP
#define LIBHERD_CRYPTO_FACTORY_HPP

#include <functional>
#include <map>
#include <set>

#include "i_crypto.hpp"
#include "keyset.hpp"


namespace herd::crypto
{
	class CryptoFactory
	{
	public:
		using FactoryMethod = std::function<std::unique_ptr<ICrypto>(Keyset&)>;

		CryptoFactory() = delete;

		static bool register_crypto(common::SchemaType type, FactoryMethod method);
		static bool is_registered(common::SchemaType type) noexcept;
		static std::set<common::SchemaType> registered_schemas() noexcept;

		static std::unique_ptr<ICrypto> create_crypto(Keyset& keyset);

	private:
		static std::unordered_map<common::SchemaType, FactoryMethod>& get_internal_map();
	};
}

#endif //LIBHERD_CRYPTO_FACTORY_HPP
