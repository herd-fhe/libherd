#include "herd/crypto/binfhe/detail/keyset.hpp"
#include "herd/crypto/keyset.hpp"


#include <binfhecontext-ser.h>


namespace herd::crypto::binfhe::detail
{
	KeysetImpl::KeysetImpl()
	{
		if(std::getenv("DEMO"))
		{
			context_.GenerateBinFHEContext(lbcrypto::TOY);
		}
		else
		{
			context_.GenerateBinFHEContext(lbcrypto::STD128);
		}
		recreate_keys();
	}

	void KeysetImpl::recreate_keys()
	{
		private_key_ = context_.KeyGen();
		context_.BTKeyGen(private_key_);

		refresh_key_ = context_.GetRefreshKey();
		switching_key_ = context_.GetSwitchKey();
	}

	common::SchemaType KeysetImpl::get_schema_type() const noexcept
	{
		return common::SchemaType::BINFHE;
	}

	std::vector<std::byte> KeysetImpl::cloud_key_to_bytes() const
	{
		std::stringstream stream;
		lbcrypto::Serial::Serialize(context_, stream, lbcrypto::SerType::BINARY);
		lbcrypto::Serial::Serialize(refresh_key_, stream, lbcrypto::SerType::BINARY);
		lbcrypto::Serial::Serialize(switching_key_, stream, lbcrypto::SerType::BINARY);

		std::vector<std::byte> out;

		stream.seekg(0, std::ios::beg);
		const auto start = stream.tellg();
		stream.seekg(0, std::ios::end);
		const auto size = std::size_t(stream.tellg() - start);
		stream.seekg(0, std::ios::beg);

		out.resize(size);

		stream.read(reinterpret_cast<char*>(out.data()), std::streamsize(size));
		return out;
	}

	std::vector<std::byte> KeysetImpl::private_key_to_bytes() const
	{
		std::stringstream stream;
		lbcrypto::Serial::Serialize(private_key_, stream, lbcrypto::SerType::BINARY);

		std::vector<std::byte> out;

		stream.seekg(0, std::ios::beg);
		const auto start = stream.tellg();
		stream.seekg(0, std::ios::end);
		const auto size = std::size_t(stream.tellg() - start);
		stream.seekg(0, std::ios::beg);

		out.resize(size);

		stream.read(reinterpret_cast<char*>(out.data()), std::streamsize(size));
		return out;
	}

	const lbcrypto::BinFHEContext& KeysetImpl::context() const
	{
		return context_;
	}

	const lbcrypto::LWEPrivateKey& KeysetImpl::private_key()
	{
		return private_key_;
	}
}