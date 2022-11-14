#include "herd/backend/remote/remote_backend.hpp"

#include <grpcpp/grpcpp.h>

#include <utility>

#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"


namespace herd
{
	RemoteBackend::RemoteBackend(const RemoteBackendConfig& config, std::string token)
		:pool_(std::make_unique<utils::ThreadPool>(DEFAULT_THREADS)), pimpl_(*pool_, config, std::move(token))
	{
	}

	void RemoteBackend::connect()
	{
		pimpl_->connect();
	}

	SessionInfo RemoteBackend::create_session(const std::string& name)
	{
		return pimpl_->create_session(name);
	}

	void RemoteBackend::destroy_session(const UUID& session_uuid)
	{
		return pimpl_->destroy_session(session_uuid);
	}

	std::vector<SessionInfo> RemoteBackend::list_sessions()
	{
		return pimpl_->list_sessions();
	}

	utils::ProgressFuture<void> RemoteBackend::add_key(const UUID& session_uuid, crypto::SchemaType type, std::vector<std::byte>&& key_data)
	{
		return pimpl_->add_key(session_uuid, type, std::move(key_data));
	}

	RemoteBackend::~RemoteBackend() = default;
}