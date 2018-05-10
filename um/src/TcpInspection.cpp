#include "TcpInspection.h"

using namespace TcpInspection;

Error CTcpInspection::RegisterProxy(IProxy *proxy)
{
	return m_proxies.RegisterProxy(proxy);
}

Error CTcpInspection::UnregisterProxy(const IProxy *proxy)
{
	return m_proxies.UnregisterProxy(proxy);
}
