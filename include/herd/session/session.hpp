#ifndef LIBHERD_SESSION_HPP
#define LIBHERD_SESSION_HPP

#include <memory>
#include <vector>

#include "herd/crypto/keyring.hpp"
#include "herd/uuid.hpp"
#include "herd/utils/progress_future.hpp"


namespace herd
{
	class Context;

	struct SessionInfo
	{
		std::string name;
		UUID uuid;
	};

	class Session: public std::enable_shared_from_this<Session>
	{
	public:
		Session(Session&&) = default;
		Session& operator=(Session&&) = default;

		Session(const Session&) = delete;
		Session& operator=(const Session&) = delete;

		~Session();

		[[nodiscard]] UUID uuid() const noexcept
		{
			return uuid_;
		}

		utils::ProgressFuture<void> add_key(std::unique_ptr<crypto::IKeyset> keyset);

		void destroy();

	private:
		friend class Context;
		struct make_shared_enabler;

		explicit Session(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy);
		static std::shared_ptr<Session> make_shared(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy);

		bool auto_destroy_;
		bool destroyed_{false};

		std::string name_;
		UUID uuid_;

		std::shared_ptr<Context> context_;

		crypto::Keyring keyring_;
	};

	struct Session::make_shared_enabler: public Session
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
		:
			Session(std::forward<Args>(args)...)
		{}
	};

}

#endif //LIBHERD_SESSION_HPP
