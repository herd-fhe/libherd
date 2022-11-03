#include "herd/backend/remote/remote_backend.hpp"

#include <grpcpp/grpcpp.h>

#include <utility>

#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"


namespace herd
{
	RemoteBackend::RemoteBackend(const RemoteBackendConfig& config, std::string token)
		:pimpl_{config, std::move(token)}
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

	void RemoteBackend::destroy_session(const UUID& uuid)
	{
		return pimpl_->destroy_session(uuid);
	}

	std::vector<SessionInfo> RemoteBackend::list_sessions()
	{
		return pimpl_->list_sessions();
	}

	RemoteBackend::~RemoteBackend() = default;
}