#include "Proxies.h"
#include "Logging.h"

using namespace TcpInspection;

Error CProxies::RegisterProxy(IProxy *proxy)
{
	LOG_INFO("registering proxy [%p]", proxy);

	if (proxy == nullptr) {
		LOG_ERROR("proxy is null");
		return Error::nullPointer;
	}

	uint32_t priority = proxy->GetPriority();
	{
		std::unique_lock<std::shared_timed_mutex> _lock(m_mutex);

		for (auto& it : m_proxies) {
			if (*it.second.get() == proxy) {
				LOG_ERROR("proxy already registered");
				return Error::proxyAlreadyRegistered;
			}
			if (it.first == priority) {
				LOG_ERROR("priority already exists");
				return Error::priorityAlreadyExists;
			}
		}

		m_proxies.insert(
			std::make_pair(
				priority,
				std::make_shared<CProxy>(proxy)
			)
		);
	}

	LOG_INFO("proxy registered successfully");
	return Error::success;
}

Error CProxies::UnregisterProxy(const IProxy *proxy)
{
	LOG_INFO("unregistering proxy [%p]", proxy);

	if (proxy == nullptr) {
		LOG_ERROR("proxy is null");
		return Error::nullPointer;
	}

	std::shared_ptr<CProxy> cproxy;

	m_mutex.lock();

	auto it = m_proxies.find(proxy->GetPriority());
	if (it == m_proxies.end()) {
		m_mutex.unlock();
		LOG_ERROR("proxy not found");
		return Error::proxyNotFound;
	}

	cproxy = it->second;
	m_proxies.erase(it);

	// this function unlock the mutex
	cproxy->Unregister(m_mutex);

	LOG_INFO("proxy unregistered successfully");
	return Error::success;
}

void CProxies::GetProxies(std::map<uint32_t, std::shared_ptr<CProxy>>& proxies) const
{
	std::shared_lock<std::shared_timed_mutex> _lock(m_mutex);
	proxies = m_proxies;
}

uint64_t CProxies::GetProxiesCount() const
{
	std::shared_lock<std::shared_timed_mutex> _lock(m_mutex);
	return m_proxies.size();
}
