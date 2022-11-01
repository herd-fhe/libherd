#include "herd/context/context_builder.hpp"

#include "herd/context/context.hpp"
#include "herd/crypto/keyring.hpp"


namespace herd
{
	ContextBuilder::ContextBuilder() = default;

	ContextBuilder &ContextBuilder::with(std::unique_ptr<IBackend> backend) noexcept
	{
		context_->backend_ = std::move(backend);
		return *this;
	}
	std::shared_ptr<Context> ContextBuilder::build_and_connect()
	{
		context_->backend_->connect();
		return std::move(context_);
	}
}