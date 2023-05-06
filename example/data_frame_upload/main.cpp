#include <herd/herd.hpp>

#include <chrono>
#include <iostream>

#include "key.hpp"

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

	setup_binfhe_keyset(session);

	std::stringstream data_frame_csv;
	for (size_t i = 0; i < 1024; ++i)
	{
		data_frame_csv << std::to_string(i)
					   << "\n";
	}

	auto data_frame_future = session->data_storage().load_from_csv({
		storage::DataTable::ColumnParameters{"id", herd::common::DataType::INT16}
	}, data_frame_csv, herd::common::SchemaType::BINFHE);

	std::cout << std::endl;

	herd::utils::ProgressBar bar;
	bar.set_bar_width(60);
	bar.set_status("Uploading DataFrame");
	while(data_frame_future.wait_for(5ms) == std::future_status::timeout)
	{
		bar.set_progress(data_frame_future.progress());
		bar.write();
	}
	bar.set_progress(data_frame_future.progress());
	bar.set_completed(true);
	bar.write();

	return 0;
}