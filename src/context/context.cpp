#include "herd/context/context.hpp"

#include "herd/context/context_builder.hpp"
#include "herd/session/session.hpp"


namespace herd
{
	ContextBuilder Context::create()
	{
		return {};
	}

	std::shared_ptr<Session> Context::create_session()
	{
		auto session = Session::make_shared(shared_from_this());
		sessions_.emplace_back(session);

		return session;
	}

	std::shared_ptr<Context> Context::make_shared()
	{
		return std::make_shared<make_shared_enabler>();
	}
}