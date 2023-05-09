#ifndef LIBHERD_PROGRESS_BAR_HPP
#define LIBHERD_PROGRESS_BAR_HPP

#include <iostream>
#include <mutex>


namespace herd::utils
{
	class ProgressBar
	{
	public:
		void set_progress(float value);
		void set_completed(bool completed);
		void set_status(std::string_view status);
		void set_bar_width(std::size_t width);

		void write(std::ostream& stream = std::cout);

	private:
		std::mutex mutex_;
		float progress_{0.0};
		bool completed_{false};

		std::size_t width_{0};

		std::string fill_{"━"};
		std::string half_fill_left_{"╸"};
		std::string half_fill_right_{"╺"};

		std::string status_;
	};
}

#endif //LIBHERD_PROGRESS_BAR_HPP
