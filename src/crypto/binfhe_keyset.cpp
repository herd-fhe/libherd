#include "herd/crypto/binfhe_keyset.hpp"


namespace herd::crypto::binfhe
{
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
}