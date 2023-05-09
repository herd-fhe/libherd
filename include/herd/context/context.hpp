#ifndef LIBHERD_CONTEXT_HPP
#define LIBHERD_CONTEXT_HPP

#include <memory>
#include <vector>

#include "herd/backend/i_backend.hpp"


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
		struct make_shared_enabler;

		Context() noexcept;
		static std::shared_ptr<Context> make_shared();

		std::unique_ptr<IBackend> backend_{};

	public:
		Context(Context&&) = default;
		Context& operator=(Context&&) = default;

		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		[[nodiscard]] std::shared_ptr<Session> create_session(const std::string& name, bool auto_destroy = true);
		[[nodiscard]] std::vector<SessionInfo> list_sessions() const;

		static ContextBuilder create();

	private:
		friend class Session;

		void destroy_session(const common::UUID& session_uuid);
		utils::ProgressFuture<void> add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data);

		[[nodiscard]] std::unique_ptr<storage::DataStorage> create_session_storage(Session& session);
	};

	struct Context::make_shared_enabler: public Context
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
		:	Context(std::forward<Args>(args)...)
		{}
	};
}

#endif //LIBHERD_CONTEXT_HPP
