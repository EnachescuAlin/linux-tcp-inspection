#ifndef TCP_INSPECTION_PROXY
#define TCP_INSPECTION_PROXY

#include "IProxy.h"
#include "Errors.h"

#include "NonCopyable.h"
#include "NonMovable.h"

#include <memory>
#include <map>

class CProxy
	: private utils::NonCopyable
	, private utils::NonMovable
{
public:
	CProxy(TcpInspection::IProxy *proxy)
		: m_proxy(proxy)
	{
	}

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

private:
	TcpInspection::IProxy *m_proxy = nullptr;
	//std::map<uint64_t, std::shared_ptr<CConnectionMgr>> m_filteredConns;
};

#endif
