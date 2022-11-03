#include <herd/herd.hpp>

#include <iostream>


using namespace herd;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	ContextBuilder builder;

	// Setup
	{
		RemoteBackendConfig config
		{
			{
				"127.0.0.1",
				5000
			},
			std::nullopt
		};

		auto backend = std::make_unique<RemoteBackend>(
				config,
				"admin==true"
		);

		builder.with(std::move(backend));
	}

	auto context = builder.build_and_connect();

	{
		const auto temporary_session = context->create_session("temporary_session");
		const auto persistant_session = context->create_session("persistant_session", false);

		std::cout << "Inside scope\n";
		std::cout << "Session list:\n";
		for(const auto sessions = context->list_sessions(); const auto& session: sessions)
		{
			std::cout << "UUID: " << session.uuid.as_string() << " Name: " << session.name << "\n";
		}
		std::cout<<std::endl;
	}

	std::cout << "Outside scope\n";
	std::cout << "Session list:\n";
	for(const auto sessions = context->list_sessions(); const auto& session: sessions)
	{
		std::cout << "UUID: " << session.uuid.as_string() << " Name: " << session.name << "\n";
	}
	std::cout<<std::endl;

	return 0;
}