#ifndef LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP
#define LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include <auth.grpc.pb.h>
#include <session.grpc.pb.h>

#include "herd/backend/remote/remote_backend.hpp"


namespace herd
{
	class RemoteBackend::RemoteBackendConnectionImpl
	{
	public:
		explicit RemoteBackendConnectionImpl(const RemoteBackendConfig& config, std::string  token) noexcept;

		void connect();

	private:
		std::string address_;

		std::string authentication_token_;
		std::string connection_token_;

		std::shared_ptr<grpc::ChannelCredentials> credentials_;

		std::shared_ptr<grpc_impl::Channel> channel_;

		std::unique_ptr<herd::proto::Auth::Stub> auth_service_stub_;
		std::unique_ptr<herd::proto::Session::Stub> session_service_stub_;

		void create_stubs();
		void authenticate();
	};
}

#endif //LIBHERD_REMOTE_BACKEND_CONNECTION_IMPL_HPP
