#ifndef TCP_INSPECTION_ITCP_INSPECTION
#define TCP_INSPECTION_ITCP_INSPECTION

#include "Errors.h"
#include "IProxy.h"

#define TCP_INSPECTION_VERSION 1

namespace TcpInspection
{

class ITcpInspection
{
public:
	ITcpInspection() = default;

	virtual ~ITcpInspection()
	{
	}

	virtual Error RegisterProxy(
		IProxy *proxy
	) = 0;

	virtual Error UnregisterProxy(
		const IProxy *proxy
	);
};

} // end TcpInspection namespace

TcpInspection::Error GetInterface(int version, void **interface);

void ReleaseInterface(void *interface);

#endif
