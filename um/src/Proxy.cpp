#include "Proxy.h"
#include "Logging.h"

#include <atomic>

using namespace TcpInspection;
using namespace utils::sync;

std::atomic<uint32_t> g_currentAvailableProxyId(1);

CProxy::CProxy(IProxy *proxy)
	: m_proxy(proxy)
	, m_semaphorePtr(nullptr)
{
	m_proxyId = g_currentAvailableProxyId.fetch_add(1, std::memory_order_relaxed);
}

void CProxy::Unregister()
{
	{
		std::lock_guard<std::mutex> _lock(m_lock);

		if (m_filteredConns.size() == 0) {
			return;
		}

		m_semaphorePtr = &m_semaphore;

		for (auto& conn : m_filteredConns) {
			// TODO
			// send unregister event
		}
	}

	m_semaphore.wait();
}

Error CProxy::AddConnection(
	uint64_t connId,
	std::shared_ptr<CConnectionMgr>& connMgr)
{
	LOG_INFO("[proxy_%u] add connId[%lu]", m_proxyId, connId);

	std::lock_guard<std::mutex> _lock(m_lock);
	auto it = m_filteredConns.emplace(connId, connMgr);

	if (it.second == false) {
		LOG_ERROR("[proxy_%u] connection id already exists connId[%lu]", m_proxyId, connId);
		return Error::connAlreadyExists;
	}

	return Error::success;
}

Error CProxy::RemoveConnection(
	uint64_t connId)
{
	LOG_INFO("[proxy_%u] remove connId[%lu]", m_proxyId, connId);

	std::lock_guard<std::mutex> _lock(m_lock);
	auto eraseRet = m_filteredConns.erase(connId);

	if (eraseRet == 0) {
		LOG_ERROR("[proxy_%u] connection id not found connId[%lu]", m_proxyId, connId);
		return Error::connNotFound;
	}

	if (eraseRet != 1) {
		LOG_ERROR("[proxy_%u] removed too many connection connId[%lu] num[%lu]",
			m_proxyId, connId, eraseRet);
		return Error::invalidState;
	}

	if (m_filteredConns.size() == 0 && m_semaphorePtr != nullptr) {
		LOG_INFO("[proxy_%u] notify semaphore connId[%lu]", m_proxyId, connId);
		m_semaphorePtr->notify();
	}

	return Error::success;
}

ConnectionVerdict CProxy::OnNewConnectionEvent(
	IConnectionMgr *connMgr,
	IConnection **conn)
{
	return m_proxy->OnNewConnection(connMgr, conn);
}