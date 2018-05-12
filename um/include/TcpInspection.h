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

	void GetProxies(std::map<uint32_t, std::shared_ptr<CProxy>>& proxies) const
	{
		m_proxies.GetProxies(proxies);
	}

	uint64_t GetProxiesCount() const
	{
		return m_proxies.GetProxiesCount();
	}

private:
	CProxies m_proxies;
};

#endif
