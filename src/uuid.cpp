#include "herd/uuid.hpp"

#ifdef __linux__
#include <uuid/uuid.h>
#else
#error "System not supported"
#endif


namespace herd
{
	UUID::UUID()
	{
		uuid_t uuid_temp;
		uuid_generate(uuid_temp);

		uuid_unparse(uuid_temp, uuid_.data());
	}

	std::strong_ordering operator<=>(const UUID &lhs, const UUID &rhs)
	{
		return lhs.uuid_ <=> rhs.uuid_;
	}
}