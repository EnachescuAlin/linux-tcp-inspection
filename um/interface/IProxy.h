#ifndef TCP_INSPECTION_IPROXY
#define TCP_INSPECTION_IPROXY

#include <cstdint>

#include "IConnectionMgr.h"
#include "IConnection.h"

namespace TcpInspection
{

enum ConnectionVerdict : uint32_t
{
	filter,
	unfilter,
	drop
};

class IProxy
{
public:
	IProxy() = default;

	virtual ~IProxy()
	{
	}

	virtual uint32_t GetPriority() const = 0;

	virtual ConnectionVerdict OnNewConnection(
		const IConnectionMgr *connMgr,
		IConnection **conn
	) = 0;

	virtual void OnDestroyConnection() = 0;
};

} // end TcpInspection namespace

#endif
