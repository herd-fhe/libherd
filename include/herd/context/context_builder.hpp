#ifndef LIBHERD_CONTEXT_BUILDER_HPP
#define LIBHERD_CONTEXT_BUILDER_HPP

#include <memory>

#include "herd/backend/i_backend.hpp"
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

		ContextBuilder& with(std::unique_ptr<IBackend> backend) noexcept;

		std::shared_ptr<Context> build_and_connect();

	private:
		std::shared_ptr<Context> context_{Context::make_shared()};
	};
}

#endif //LIBHERD_CONTEXT_BUILDER_HPP
