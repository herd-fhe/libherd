#ifndef LIBHERD_SESSION_HPP
#define LIBHERD_SESSION_HPP

#include <memory>

#include "herd/uuid.hpp"


namespace herd
{
	class Context;

	class Session: public std::enable_shared_from_this<Session>
	{
	public:
		Session(Session&&) = default;
		Session& operator=(Session&&) = default;

		Session(const Session&) = delete;
		Session& operator=(const Session&) = delete;

		[[nodiscard]] UUID uuid() const noexcept
		{
			return uuid_;
		}

	private:
		friend class Context;
		struct make_shared_enabler;

		explicit Session(std::shared_ptr<Context> context);
		static std::shared_ptr<Session> make_shared(std::shared_ptr<Context> context);

		UUID uuid_;
		std::shared_ptr<Context> context_;
	};

	struct Session::make_shared_enabler: public Session
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
		:   Session(std::forward<Args>(args)...)
		{}
	};

}

#endif //LIBHERD_SESSION_HPP
