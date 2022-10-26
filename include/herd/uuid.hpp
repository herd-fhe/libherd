#ifndef LIBHERD_UUID_HPP
#define LIBHERD_UUID_HPP

#include <array>


namespace herd
{
	class UUID
	{
	public:
		UUID();

	private:
		std::array<char, 36> uuid_{};
	};
}

#endif //LIBHERD_UUID_HPP
