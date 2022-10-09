#ifndef LIBHERD_I_KEYSET_HPP
#define LIBHERD_I_KEYSET_HPP


namespace herd::crypto
{
	class IKeyset
	{
	public:
		virtual ~IKeyset() = 0;
	};

	IKeyset::~IKeyset() = default;

	template<typename Context, typename PrivateKey, typename CloudKey>
	class BaseKeyset: public IKeyset
	{
	public:
		using crypto_context = Context;
		using private_key = PrivateKey;
		using cloud_key = CloudKey;

		~BaseKeyset() override = default;
	protected:
		crypto_context context_;
		cloud_key cloud_key_;
		private_key private_key_;
	};
}

#endif //LIBHERD_I_KEYSET_HPP
