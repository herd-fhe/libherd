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

	RemoteBackend::~RemoteBackend() = default;
}