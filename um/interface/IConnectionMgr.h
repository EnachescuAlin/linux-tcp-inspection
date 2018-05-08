#ifndef TCP_INSPECTION_ICONNECTION_MGR
#define TCP_INSPECTION_ICONNECTION_MGR

#include "Errors.h"
#include "Structs.h"

namespace TcpInspection
{

class IConnectionMgr
{
public:
	IConnectionMgr() = default;

	virtual ~IConnectionMgr()
	{
	}

	virtual uint64_t GetConnectionId() const = 0;

	virtual Pid GetProcessPid() const = 0;

	virtual const char* GetProcessPath() const = 0;

	virtual const Ipv4Address& GetLocalAddress() const = 0;

	virtual const Ipv4Address& GetRemoteAddress() const = 0;

	virtual Error ReinjectData(
		const uint8_t *buffer,
		uint32_t bufferSize,
		Direction direction
	) = 0;
};

} // end TcpInspection namespace

#endif
