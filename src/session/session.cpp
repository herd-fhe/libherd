#include "herd/session/session.hpp"


namespace herd
{
	Session::Session(std::shared_ptr<Context> context)
			:   context_(std::move(context))
	{}

	std::shared_ptr<Session> Session::make_shared(std::shared_ptr<Context> context)
	{
		return std::make_shared<make_shared_enabler>(std::move(context));
	}
}