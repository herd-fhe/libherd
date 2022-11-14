#include "herd/crypto/binfhe_keyset.hpp"

#include <binfhecontext-ser.h>


namespace herd::crypto::binfhe
{
	class Keyset: public IKeyset
	{
	public:
		Keyset();

		[[nodiscard]] SchemaType get_schema_type() const noexcept override;
		[[nodiscard]] std::vector<std::byte> serialize_cloud_key() const override;

		void recreate_keys();

	private:
		lbcrypto::BinFHEContext context_;
		lbcrypto::LWEPrivateKey private_key_;

		lbcrypto::LWESwitchingKey switching_key_;
		lbcrypto::RingGSWACCKey refresh_key_;
	};

	std::unique_ptr<IKeyset> create_binfhe_keyset()
	{
		return std::make_unique<Keyset>();
	}

	Keyset::Keyset()
	{
		context_.GenerateBinFHEContext(lbcrypto::STD128);
		recreate_keys();
	}

	void Keyset::recreate_keys()
	{
		private_key_ = context_.KeyGen();
		context_.BTKeyGen(private_key_);

		refresh_key_ = context_.GetRefreshKey();
		switching_key_ = context_.GetSwitchKey();
	}

	SchemaType Keyset::get_schema_type() const noexcept
	{
		return SchemaType::BINFHE;
	}

	std::vector<std::byte> Keyset::serialize_cloud_key() const
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
}