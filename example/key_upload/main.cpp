#include <herd/herd.hpp>

#include <chrono>
#include <iostream>


using namespace herd;
using namespace std::chrono_literals;

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
	auto session = context->create_session("example_session");

	{
		auto keyset = crypto::binfhe::create_binfhe_keyset();
		auto future = session->add_key(std::move(keyset));

		std::cout << "0%";
		std::flush(std::cout);
		while(future.wait_for(5ms) == std::future_status::timeout)
		{
			std::cout << "\r" << std::to_string(future.progress()) << "%";
			std::flush(std::cout);
		}
	}

	return 0;
}