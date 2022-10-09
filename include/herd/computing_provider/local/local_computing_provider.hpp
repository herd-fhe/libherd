#ifndef LIBHERD_LOCAL_COMPUTING_PROVIDER_HPP
#define LIBHERD_LOCAL_COMPUTING_PROVIDER_HPP

#include "herd/computing_provider/i_computing_provider.hpp"


namespace herd
{
	class LocalComputingProvider : public IComputingProvider
	{
	public:
		void connect() override;
	};
}

#endif //LIBHERD_LOCAL_COMPUTING_PROVIDER_HPP
