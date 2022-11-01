#ifndef LIBHERD_I_BACKEND_HPP
#define LIBHERD_I_BACKEND_HPP


namespace herd
{
	class IBackend
	{
	public:
		virtual void connect() = 0;

		virtual ~IBackend() = default;
	};
}

#endif //LIBHERD_I_BACKEND_HPP
