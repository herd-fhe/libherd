#include "herd/context/context.hpp"

#include "herd/context/context_builder.hpp"
#include "herd/session/session.hpp"


namespace herd
{
	ContextBuilder Context::create()
	{
		return {};
	}

	std::shared_ptr<Session> Context::create_session(const std::string& name, bool auto_destroy)
	{
		const auto info = backend_->create_session(name);
		auto session = Session::make_shared(info, shared_from_this(), auto_destroy);

		return session;
	}

	std::shared_ptr<Context> Context::make_shared()
	{
		return std::make_shared<make_shared_enabler>();
	}

	void Context::destroy_session(const common::UUID& session_uuid)
	{
		backend_->destroy_session(session_uuid);
	}

	std::vector<SessionInfo> Context::list_sessions() const
	{
		return backend_->list_sessions();
	}
	utils::ProgressFuture<void> Context::add_key(const common::UUID& session_uuid, common::SchemaType type, std::vector<std::byte>&& key_data)
	{
		return backend_->add_key(session_uuid, type, std::move(key_data));
	}

	std::unique_ptr<storage::DataStorage> Context::create_session_storage(Session& session)
	{
		return backend_->create_session_storage(session);
	}

	std::unique_ptr<executor::IExecutor> Context::create_session_executor(herd::Session& session)
	{
		return backend_->create_session_executor(session);
	}
}

