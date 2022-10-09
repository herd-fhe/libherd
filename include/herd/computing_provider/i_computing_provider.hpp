#ifndef LIBHERD_I_COMPUTING_PROVIDER_HPP
#define LIBHERD_I_COMPUTING_PROVIDER_HPP


namespace herd
{
	class IComputingProvider
	{
	public:
		virtual void connect() = 0;

		virtual ~IComputingProvider() = 0;
	};
}

#endif //LIBHERD_I_COMPUTING_PROVIDER_HPP
