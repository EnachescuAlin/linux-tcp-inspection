#ifndef TCP_INSPECTION_ICONNECTION
#define TCP_INSPECTION_ICONNECTION

#include <cstdint>

#include "IConnectionMgr.h"

namespace TcpConnection
{

enum DataVerdict : uint32_t
{
	block,
	allow,
	drop
};

enum DataFlags : uint32_t
{
	normal,
	reinjected
};

enum Direction : uint32_t
{
	in,
	out
};

class IConnection
{
public:
	IConnection(IConnectionMgr *connMgr)
		: m_connMgr(connMgr)
	{
	}

	virtual ~IConnection()
	{
		m_connMgr = nullptr;
	}

	IConnectionMgr* GetConnectionMgr()
	{
		return m_connMgr;
	}

	const IConnectionMgr* GetConnectionMgr() const
	{
		return m_connMgr;
	}

	virtual DataVerdict OnData(
		const uint8_t *buffer,
		uint32_t bufferSize,
		Direction direction,
		DataFlags flags
	) = 0;

	virtual void OnDisconnect(
		Direction direction
	) = 0;

private:
	IConnectionMgr *m_connMgr = nullptr;
};

} // end TcpConnection namespace

#endif
