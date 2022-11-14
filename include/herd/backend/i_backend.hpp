#ifndef LIBHERD_I_BACKEND_HPP
#define LIBHERD_I_BACKEND_HPP

#include <vector>

#include "herd/session/session.hpp"
#include "herd/uuid.hpp"


namespace herd
{
	class IBackend
	{
	public:
		virtual void connect() = 0;

		virtual SessionInfo create_session(const std::string& name) = 0;
		virtual void destroy_session(const UUID& uuid) = 0;
		virtual std::vector<SessionInfo> list_sessions() = 0;

		virtual utils::ProgressFuture<void> add_key(const UUID& session_uuid, crypto::SchemaType type, std::vector<std::byte>&& key_data) = 0;

		virtual ~IBackend() = default;
	};
}

#endif //LIBHERD_I_BACKEND_HPP
