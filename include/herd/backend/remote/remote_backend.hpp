#ifndef LIBHERD_REMOTE_BACKEND_HPP
#define LIBHERD_REMOTE_BACKEND_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "herd/backend/i_backend.hpp"
#include "herd/session/session.hpp"
#include "herd/utils/pimpl.hpp"


namespace herd
{
	struct RemoteConnectionError: public std::runtime_error
	{
		using runtime_error::runtime_error;
	};

	struct RemoteBackendConfig
	{
		struct ServerConfig
		{
			std::string address;
			uint16_t port;
		};

		struct Credentials
		{
			std::string pem_root_certs_path;
			std::string pem_private_key;
			std::string pem_cert_chain;
		};

		ServerConfig server;
		std::optional<Credentials> security;
	};

	class RemoteBackend final: public IBackend
	{
	public:
		explicit RemoteBackend(const RemoteBackendConfig& config, std::string token);
		~RemoteBackend() override;

		void connect() override;

		SessionInfo create_session(const std::string &name) override;
		void destroy_session(const UUID& uuid) override;
		std::vector<SessionInfo> list_sessions() override;

	private:
		class RemoteBackendConnectionImpl;
		utils::PImpl<RemoteBackendConnectionImpl> pimpl_;
	};
}

#endif //LIBHERD_REMOTE_BACKEND_HPP
