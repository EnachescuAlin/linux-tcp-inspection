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

extern "C"
{

TcpInspection::Error GetInterface(int version, void **interface);
typedef TcpInspection::Error (*GetInterfacePfn)(int, void**);

void ReleaseInterface(void *interface);
typedef void (*ReleaseInterfacePfn)(void*);

}

#endif
