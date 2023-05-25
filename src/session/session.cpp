#include "herd/session/session.hpp"

#include "herd/context/context.hpp"
#include "herd/crypto/crypto_factory.hpp"


namespace herd
{
	Session::Session(const SessionInfo& info, std::shared_ptr<Context> context, bool auto_destroy)
	:	auto_destroy_(auto_destroy),
		name_(info.name), uuid_(info.uuid),
		context_(std::move(context))
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
		auto session = std::make_shared<make_shared_enabler>(info, context, auto_destroy);
		auto storage = context->create_session_storage(*session);
		session->set_storage(std::move(storage));

		return session;
	}

	void Session::set_storage(std::unique_ptr<storage::DataStorage> storage)
	{
		storage_ = std::move(storage);
	}

	void Session::set_executor(std::unique_ptr<executor::IExecutor> executor)
	{
		executor_ = std::move(executor);
	}

	void Session::destroy()
	{
		destroyed_ = true;
		context_->destroy_session(uuid_);
	}

	storage::DataStorage& Session::data_storage()
	{
		return *storage_;
	}

	executor::IExecutor& Session::executor()
	{
		return *executor_;
	}

	utils::ProgressFuture<void> Session::add_key(std::unique_ptr<crypto::IKeyset> keyset)
	{
		const auto type = keyset->get_schema_type();
		keyring_.add_keyset(std::move(keyset));

		auto cloud_key = keyring_.get_keyset(type)->to_bytes();
		return context_->add_key(uuid_, type, std::move(cloud_key));
	}

	crypto::ICrypto& Session::crypto(common::SchemaType schema_type)
	{
		if(!cryptos_.contains(schema_type))
		{
			const auto& keyset = keyring_.get_keyset(schema_type);

			cryptos_.try_emplace(schema_type, crypto::CryptoFactory::create_crypto(*keyset));
		}

		auto cached_crypto_iter = cryptos_.find(schema_type);
		assert(cached_crypto_iter != std::end(cryptos_));

		return *cached_crypto_iter->second;
	}
}