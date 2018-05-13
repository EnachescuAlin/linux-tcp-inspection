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

	void AcquireProxiesList() const;

	void GetProxies(std::map<uint32_t, std::shared_ptr<CProxy>>& proxies) const;

	void ReleaseProxiesList() const;

	uint64_t GetProxiesCount() const;

private:
	std::map<uint32_t, std::shared_ptr<CProxy>> m_proxies;
	mutable std::shared_timed_mutex m_mutex;
};

class proxies_list_lock_guard
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	proxies_list_lock_guard(const CProxies& proxies)
		: m_proxies(&proxies)
	{
		m_proxies->AcquireProxiesList();
	}

	proxies_list_lock_guard(const CProxies *proxies)
		: m_proxies(proxies)
	{
		if (m_proxies != nullptr) {
			m_proxies->AcquireProxiesList();
		}
	}

	~proxies_list_lock_guard()
	{
		unlock();
	}

	void unlock()
	{
		if (m_proxies != nullptr) {
			m_proxies->ReleaseProxiesList();
			m_proxies = nullptr;
		}
	}

private:
	const CProxies *m_proxies;
};

#endif
