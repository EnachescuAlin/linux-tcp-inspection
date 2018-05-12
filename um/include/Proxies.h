#ifndef TCP_INSPECTION_PROXIES
#define TCP_INSPECTION_PROXIES

#include "Proxy.h"
#include "NonCopyable.h"
#include "NonMovable.h"
#include "Errors.h"

#include <shared_mutex>
#include <map>
#include <memory>

class CProxies
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	CProxies() = default;
	~CProxies() = default;

	TcpInspection::Error RegisterProxy(TcpInspection::IProxy *proxy);

	TcpInspection::Error UnregisterProxy(const TcpInspection::IProxy *proxy);

	void GetProxies(std::map<uint32_t, std::shared_ptr<CProxy>>& proxies) const;

	uint64_t GetProxiesCount() const;

private:
	std::map<uint32_t, std::shared_ptr<CProxy>> m_proxies;
	mutable std::shared_timed_mutex m_mutex;
};

#endif
