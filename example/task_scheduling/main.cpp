#include <herd/herd.hpp>

#include <chrono>
#include <iostream>

#include "herd/translator/xlscc/detail/compiler.hpp"
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
	const auto& keyset = session->crypto(herd::common::SchemaType::BINFHE).keyset();
	keyset.store_private_key_to_file("./1.key");

	std::stringstream data_frame_csv;
	for(size_t i = 0; i < 4; ++i)
	{
		data_frame_csv << std::to_string(i)
					   << "\n";
	}

	herd::storage::ImportOptions options;
	options.set_columns({common::ColumnMeta{"id", herd::common::DataType::UINT16}});
	options.set_schema(herd::common::SchemaType::BINFHE);
	options.set_partitions(2);

	auto data_frame_future = session->data_storage().import_from_csv(data_frame_csv, options);

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

	const auto data_frame = data_frame_future.get();

	std::cout << std::endl
			  << "DataFrame uploaded" << std::endl;

	std::cout << std::flush;

	const std::string mapper_code =
			"struct in_input\n"
			"{\n"
			"    short in;\n"
			"};\n"
			"struct out_output\n"
			"{\n"
			"    short out;\n"
			"};\n"
			"#pragma hls_top\n"
			"out_output top_add3(in_input input)\n"
			"{\n"
			"    return {input.in * 2};\n"
			"}";

	herd::utils::TempStorage storage;
	herd::translator::xlscc::detail::Compiler translator(storage);

	const auto mapper_circuit = translator.translate(mapper_code);

	common::ExecutionPlan plan;
	plan.schema_type = herd::common::SchemaType::BINFHE;

	auto prev = plan.execution_graph.emplace(common::InputStage{data_frame->uuid()});
	prev = plan.execution_graph.emplace(prev, common::MapperStage{mapper_circuit});
	plan.execution_graph.emplace(prev, common::OutputStage{"output"});

	{
		const auto job = session->executor().schedule(plan);
		std::cout << "Job scheduled: " << job->uuid().as_string() << std::endl;
	}
	std::cout << "Listing jobs:\n";

	auto jobs = session->executor().list_jobs(false);
	for(std::size_t i = 0; const auto& job: jobs)
	{
		std::cout << "\t" << std::to_string(i) << "." << job->uuid().as_string() << "\n";
	}
	std::cout << std::endl;

	return 0;
}