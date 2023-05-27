#ifndef LIBHERD_SESSION_HPP
#define LIBHERD_SESSION_HPP

#include <memory>
#include <vector>

#include "herd/common/uuid.hpp"
#include "herd/crypto/keyring.hpp"
#include "herd/storage/data_storage.hpp"
#include "herd/executor/i_executor.hpp"
#include "herd/utils/progress_future.hpp"


namespace herd
{
	class Context;

	struct SessionInfo
	{
		std::string name;
		common::UUID uuid;
	};

	class Session: public std::enable_shared_from_this<Session>
	{
	public:
		Session(Session&&) = default;
		Session& operator=(Session&&) = default;

		Session(const Session&) = delete;
		Session& operator=(const Session&) = delete;

		~Session();

		[[nodiscard]] common::UUID uuid() const noexcept
		{
			return uuid_;
		}

		[[nodiscard]] storage::DataStorage& data_storage();
		[[nodiscard]] executor::IExecutor& executor();
		[[nodiscard]] crypto::ICrypto& crypto(common::SchemaType schema_type);

		utils::ProgressFuture<void> add_key(std::unique_ptr<crypto::IKeyset> keyset);

		void destroy();

	private:
		friend class Context;
		struct make_shared_enabler;

		Session(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy);
		static std::shared_ptr<Session> make_shared(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy);

		void set_storage(std::unique_ptr<storage::DataStorage> storage);
		void set_executor(std::unique_ptr<executor::IExecutor> executor);

		std::unique_ptr<storage::DataStorage> storage_;
		std::unique_ptr<executor::IExecutor> executor_;

		bool auto_destroy_;
		bool destroyed_{false};

		std::string name_;
		common::UUID uuid_;

		std::shared_ptr<Context> context_;

		crypto::Keyring keyring_;
		std::unordered_map<common::SchemaType, std::unique_ptr<crypto::ICrypto>> cryptos_;
	};

	struct Session::make_shared_enabler: public Session
	{
		template<typename... Args>
		explicit make_shared_enabler(Args&&... args)
		:	Session(std::forward<Args>(args)...)
		{}
	};

}

#endif //LIBHERD_SESSION_HPP
