#ifndef LIBHERD_KEYRING_HPP
#define LIBHERD_KEYRING_HPP

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "herd/crypto/i_keyset.hpp"


namespace herd::crypto
{
	class Keyring
	{
	public:
		void add_keyset(std::unique_ptr<IKeyset>&& keyset);
		[[nodiscard]] const std::unique_ptr<IKeyset>& get_keyset(const std::type_index& key_type) const;
		[[nodiscard]] bool contains_key(const std::type_index& key_type) const noexcept;

	private:
		std::unordered_map<std::type_index, std::unique_ptr<IKeyset>> keyring_;
	};
}

#endif //LIBHERD_KEYRING_HPP