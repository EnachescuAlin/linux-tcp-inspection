#ifndef TCP_INSPECTION_ICONNECTION_MGR
#define TCP_INSPECTION_ICONNECTION_MGR

namespace TcpConnection
{

class IConnectionMgr
{
public:
	IConnectionMgr() = default;

	virtual ~IConnectionMgr()
	{
	}

	virtual uint64_t GetConnectionId() const = 0;
};

} // end TcpConnection namespace

#endif
