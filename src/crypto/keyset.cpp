#include "herd/crypto/keyset.hpp"

#include <fstream>


namespace herd::crypto
{
	void Keyset::store_private_key_to_file(const std::filesystem::path& path) const
	{
		std::ofstream key_stream(path, std::ios::out | std::ios::binary);
		const auto data = private_key_to_bytes();
		key_stream.write(reinterpret_cast<const char*>(data.data()), static_cast<long>(data.size()));
	}
}