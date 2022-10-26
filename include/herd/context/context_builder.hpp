#ifndef LIBHERD_CONTEXT_BUILDER_HPP
#define LIBHERD_CONTEXT_BUILDER_HPP

#include <memory>

#include "herd/computing_provider/i_computing_provider.hpp"
#include "herd/context/context.hpp"


namespace herd
{
	class Context;

	namespace crypto
	{
		class Keyring;
	}

	class ContextBuilder
	{
	public:
		ContextBuilder();

		explicit operator std::shared_ptr<Context>(); // NOLINT(google-explicit-constructor)

		ContextBuilder& with(std::unique_ptr<crypto::Keyring> keyring) noexcept;
		ContextBuilder& with(std::unique_ptr<IComputingProvider> computing_provider) noexcept;

	private:
		std::shared_ptr<Context> context_{Context::make_shared()};
	};
}

#endif //LIBHERD_CONTEXT_BUILDER_HPP
