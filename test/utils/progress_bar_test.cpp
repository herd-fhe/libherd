#include <gtest/gtest.h>
#include <herd/utils/progress_bar.hpp>

using namespace herd::utils;

TEST(ProgressBar, zero_state)
{
	{
		//given
		std::stringstream stream;

		ProgressBar progressBar;

		//when
		progressBar.set_bar_width(10);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 0% ", stream.str());
	}
	{
		//given
		std::stringstream stream;

		ProgressBar progressBar;

		//when
		progressBar.set_bar_width(5);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 0% ", stream.str());
	}
}

TEST(ProgressBar, progress)
{
	ProgressBar progressBar;
	progressBar.set_bar_width(10);

	{
		std::stringstream stream;

		progressBar.set_progress(0.1f);
		progressBar.write(stream);

		EXPECT_EQ("\r\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 0% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(41.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 41% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(79.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x95\xB8\xE2\x94\x81\xE2\x94\x81 79% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(100.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 100% ", stream.str());
	}
}

TEST(ProgressBar, progress_completed)
{
	ProgressBar progressBar;
	progressBar.set_bar_width(10);
	progressBar.set_completed(true);

	{
		std::stringstream stream;

		progressBar.set_progress(0.1f);
		progressBar.write(stream);

		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 0% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(41.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 41% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(79.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 79% ", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(100.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 100% ", stream.str());
	}
}

TEST(ProgressBar, progress_with_status)
{
	ProgressBar progressBar;
	progressBar.set_bar_width(10);
	progressBar.set_status("Test status");

	{
		std::stringstream stream;

		progressBar.set_progress(0.1f);
		progressBar.write(stream);

		EXPECT_EQ("\r\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 0% Test status", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(41.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x95\xBA\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 41% Test status", stream.str());
	}

	{
		//given
		std::stringstream stream;

		//when
		progressBar.set_progress(100.0f);
		progressBar.write(stream);

		//then
		EXPECT_EQ("\r\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81\xE2\x94\x81 100% Test status", stream.str());
	}
}