#include <grpcpp/create_channel.h>

#include <utility>

#include "herd/backend/remote/detail/remote_backend_connection_impl.hpp"


namespace herd
{


	RemoteBackend::RemoteBackendConnectionImpl::RemoteBackendConnectionImpl(const RemoteBackendConfig& config, std::string  token) noexcept
		:authentication_token_(std::move(token))
	{
		if(config.security.has_value())
		{
			grpc::SslCredentialsOptions ssl_credentials_options;
			ssl_credentials_options.pem_root_certs = config.security->pem_root_certs_path;
			ssl_credentials_options.pem_cert_chain = config.security->pem_cert_chain;
			ssl_credentials_options.pem_private_key = config.security->pem_private_key;

			credentials_ = grpc::SslCredentials(ssl_credentials_options);
		}
		else
		{
			credentials_ = grpc::experimental::LocalCredentials(LOCAL_TCP);
		}

		address_ = config.server.address + ":" + std::to_string(config.server.port);
		channel_ = grpc::CreateChannel(address_, credentials_);
	}

	void RemoteBackend::RemoteBackendConnectionImpl::connect()
	{
		create_stubs();
		authenticate();
	}

	void RemoteBackend::RemoteBackendConnectionImpl::create_stubs()
	{
		assert(channel_);

		auth_service_stub_ = herd::proto::Auth::NewStub(channel_);
	}

	void RemoteBackend::RemoteBackendConnectionImpl::authenticate()
	{
		assert(auth_service_stub_);
		using namespace herd::proto;

		AuthenticationToken auth_token;
		auth_token.set_authentication_token(authentication_token_);

		ConnectionToken connection_token;
		grpc::ClientContext client_context{};

		if(const auto status = auth_service_stub_->authorize_connection(&client_context, auth_token, &connection_token); !status.ok()) //todo: better error handling
		{
			throw RemoteConnectionError("Failed to authenticate");
		}

		connection_token_ = connection_token.token();
	}
}