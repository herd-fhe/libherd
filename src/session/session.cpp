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
}