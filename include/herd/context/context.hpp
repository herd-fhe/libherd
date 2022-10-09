#ifndef LIBHERD_CONTEXT_HPP
#define LIBHERD_CONTEXT_HPP

#include <memory>
#include <vector>

#include "herd/computing_provider/i_computing_provider.hpp"


namespace herd
{
	class ContextBuilder;
	class Session;

	namespace crypto
	{
		class Keyring;
	}

	class Context: public std::enable_shared_from_this<Context>
	{
	private:
		friend class ContextBuilder;

		Context() noexcept = default;
		static std::shared_ptr<Context> make_shared() { return {}; };

		std::unique_ptr<crypto::Keyring> keyring_{};
		std::unique_ptr<IComputingProvider> computing_provider_{};
		std::vector<std::weak_ptr<Session>> sessions_;

	public:
		Context(Context&&) = default;
		Context& operator=(Context&&) = default;

		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		[[nodiscard]] std::shared_ptr<Session> create_session();

		static ContextBuilder create();
	};
}

#endif //LIBHERD_CONTEXT_HPP
