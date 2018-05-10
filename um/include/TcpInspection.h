#ifndef TCP_CONNECTION_TCP_CONNECTION
#define TCP_CONNECTION_TCP_CONNECTION

#include "ITcpInspection.h"
#include "Proxies.h"

class CTcpInspection
	: public TcpInspection::ITcpInspection
{
public:
	CTcpInspection() = default;
	~CTcpInspection() = default;

	TcpInspection::Error RegisterProxy(
		TcpInspection::IProxy *proxy
	) override;

	TcpInspection::Error UnregisterProxy(
		const TcpInspection::IProxy *proxy
	) override;

private:
	CProxies m_proxies;
};

#endif
