#include "herd/crypto/binfhe_keyset.hpp"

#include <binfhecontext.h>


namespace herd::crypto::binfhe
{
	namespace
	{
		struct CloudKeyData
		{
			std::shared_ptr<lbcrypto::LWESwitchingKey> switching_key;
			std::shared_ptr<lbcrypto::RingGSWBTKey> refresh_key;
		};
	}

	class Keyset: public BaseKeyset<lbcrypto::BinFHEContext, lbcrypto::LWEPrivateKey, CloudKeyData>
	{
	public:
		Keyset();

		[[nodiscard]] SchemaType get_schema_type() const noexcept override;

		void recreate_keys();
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

		cloud_key_.refresh_key = context_.GetRefreshKey();
		cloud_key_.switching_key = context_.GetSwitchKey();
	}

	SchemaType Keyset::get_schema_type() const noexcept
	{
		return SchemaType::BINFHE;
	}
}