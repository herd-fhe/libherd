#ifndef LIBHERD_EXAMPLE_UTILS_KEY_HPP
#define LIBHERD_EXAMPLE_UTILS_KEY_HPP

#include <herd/herd.hpp>

void setup_binfhe_keyset(const std::shared_ptr<herd::Session>& session)
{
	using namespace std::chrono_literals;

	auto keyset = herd::crypto::binfhe::create_binfhe_keyset();
	auto future = session->add_key(std::move(keyset));

	herd::utils::ProgressBar bar;
	bar.set_bar_width(60);
	bar.set_status("Uploading BinFHE keys");
	while(future.wait_for(5ms) == std::future_status::timeout)
	{
		bar.set_progress(future.progress());
		bar.write();
	}
	bar.set_progress(future.progress());
	bar.set_completed(true);
	bar.write();
}

#endif //LIBHERD_EXAMPLE_UTILS_KEY_HPP
