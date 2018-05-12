#ifndef TCP_INSPECTION_PROXY
#define TCP_INSPECTION_PROXY

#include "IProxy.h"
#include "Errors.h"

#include "NonCopyable.h"
#include "NonMovable.h"
#include "Semaphore.h"

#include <memory>
#include <map>
#include <mutex>
#include <atomic>

class CConnectionMgr;

class CProxy
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	CProxy(TcpInspection::IProxy *proxy);

	~CProxy() = default;

	void Unregister();

	bool operator==(const TcpInspection::IProxy *proxy) const
	{
		return m_proxy == proxy;
	}

	uint32_t GetPriority() const
	{
		return m_proxy->GetPriority();
	}

	TcpInspection::Error AddConnection(
		uint64_t connId,
		std::shared_ptr<CConnectionMgr>& connMgr
	);

	TcpInspection::Error RemoveConnection(
		uint64_t connId
	);

private:
	TcpInspection::IProxy *m_proxy = nullptr;
	uint32_t m_proxyId;

	std::map<uint64_t, std::shared_ptr<CConnectionMgr>> m_filteredConns;
	std::mutex m_lock;

	utils::sync::Semaphore m_semaphore;
	std::atomic<utils::sync::Semaphore*> m_semaphorePtr;
};

#endif
