#ifndef LIBHERD_UUID_HPP
#define LIBHERD_UUID_HPP

#include <array>


namespace herd
{
	class UUID
	{
	public:
		UUID();

		friend std::strong_ordering operator<=>(const UUID& lhs, const UUID& rhs);

	private:
		std::array<char, 36> uuid_{};
	};
}

#endif //LIBHERD_UUID_HPP
