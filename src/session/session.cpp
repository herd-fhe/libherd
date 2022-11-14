#include "herd/session/session.hpp"

#include "herd/context/context.hpp"


namespace herd
{
	Session::Session(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy)
		: auto_destroy_(auto_destroy), name_(info.name), uuid_(info.uuid), context_(std::move(context))
	{}

	Session::~Session()
	{
		if(auto_destroy_)
		{
			destroy();
		}
	}

	std::shared_ptr<Session> Session::make_shared(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy)
	{
		return std::make_shared<make_shared_enabler>(info, std::move(context), auto_destroy);
	}

	void Session::destroy()
	{
		destroyed_ = true;
		context_->destroy_session(uuid_);
	}

	utils::ProgressFuture<void> Session::add_key(std::unique_ptr<crypto::IKeyset> keyset)
	{
		const auto type = keyset->get_schema_type();
		keyring_.add_keyset(std::move(keyset));

		auto cloud_key = keyring_.get_keyset(type)->serialize_cloud_key();
		return context_->add_key(uuid_, type, std::move(cloud_key));
	}
}