#include "herd/utils/progress_bar.hpp"

#include <termcolor/termcolor.hpp>


namespace herd::utils
{
	void ProgressBar::set_progress(float value)
	{
		std::unique_lock update_lock{mutex_};

		progress_ = value;
	}

	void ProgressBar::set_completed(bool completed)
	{
		std::unique_lock update_lock{mutex_};

		completed_ = completed;
	}

	void ProgressBar::set_status(std::string_view status)
	{
		std::unique_lock update_lock{mutex_};

		status_ = status;
	}

	void ProgressBar::set_bar_width(std::size_t width)
	{
		std::unique_lock update_lock{mutex_};

		width_ = width;
	}
	void ProgressBar::write(std::ostream& stream)
	{
		std::unique_lock write_lock{mutex_};
		stream << "\r" << std::flush;

		if(completed_)
		{
			stream << termcolor::green;
		}
		else
		{
			stream << termcolor::red;
		}

		const auto halves = completed_ ? width_ * 2
									   : std::min(static_cast<std::size_t>(static_cast<float>(width_) * progress_ / 100.0f * 2.0f), width_ * 2);

		const std::size_t full_symbols = halves / 2;

		//full segments
		auto remaining = width_;

		for(std::size_t i = 0; i < full_symbols; ++i)
		{
			stream << fill_;
		}
		remaining -= full_symbols;

		bool ends_with_half = halves % 2 == 1;
		if(ends_with_half)
		{
			stream << half_fill_left_;
			remaining -= 1;
		}

		stream << termcolor::bright_grey;

		if(!ends_with_half && remaining > 0)
		{
			stream << half_fill_right_;
			remaining -= 1;
		}

		for(std::size_t i = 0; i < remaining; ++i)
		{
			stream << fill_;
		}

		if(completed_)
		{
			stream << termcolor::green;
		}
		else
		{
			stream << termcolor::red;
		}

		stream << " " << std::min(static_cast<std::size_t>(progress_), std::size_t(100)) << "%";
		stream << termcolor::reset;
		stream << " " << status_;
	}
}