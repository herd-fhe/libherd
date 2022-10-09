#include "herd/context/context_builder.hpp"

#include "herd/context/context.hpp"
#include "herd/crypto/keyring.hpp"


namespace herd
{
	ContextBuilder::ContextBuilder()
			: context_(Context::make_shared())
	{}

	ContextBuilder::operator std::shared_ptr<Context>()
	{
		return std::move(context_);
	}

	ContextBuilder &ContextBuilder::with(std::unique_ptr<crypto::Keyring> keyring) noexcept
	{
		context_->keyring_ = std::move(keyring);
		return *this;
	}

	ContextBuilder &ContextBuilder::with(std::unique_ptr<IComputingProvider> computing_provider) noexcept
	{
		context_->computing_provider_ = std::move(computing_provider);
		return *this;
	}
}