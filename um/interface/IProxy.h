#ifndef TCP_INSPECTION_IPROXY
#define TCP_INSPECTION_IPROXY

#include <cstdint>

#include "IConnectionMgr.h"
#include "IConnection.h"
#include "Structs.h"

namespace TcpInspection
{

class IProxy
{
public:
	IProxy() = default;

	virtual ~IProxy()
	{
	}

	virtual uint32_t GetPriority() const = 0;

	virtual ConnectionVerdict OnNewConnection(
		IConnectionMgr *connMgr,
		IConnection **conn
	) = 0;

	virtual void OnDestroyConnection(
		IConnection *conn
	) = 0;
};

} // end TcpInspection namespace

#endif
