#ifndef LIBHERD_UUID_HPP
#define LIBHERD_UUID_HPP

#ifdef __linux__
	#include <uuid/uuid.h>
#else
	#error "System not supported"
#endif

#include <string>


namespace herd
{
	class UUID
	{
	public:
		UUID();
		explicit UUID(const std::string& uuid_string);

		[[nodiscard]] std::string as_string() const;

		friend bool operator==(const UUID& lhs, const UUID& rhs);
		friend std::strong_ordering operator<=>(const UUID& lhs, const UUID& rhs);

	private:
		uuid_t uuid_{};
	};

}

#endif //LIBHERD_UUID_HPP
