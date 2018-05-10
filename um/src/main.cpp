#define TCP_INSPECTION_LOGGING_MAIN
#include "Logging.h"

#include "TcpInspection.h"

#include <new>
#include <mutex>

std::mutex g_lock;
CTcpInspection *g_interface = nullptr;
uint32_t g_interfaceRef = 0;

using namespace TcpInspection;

extern "C"
{

Error GetInterface(int version, void **interface)
{
	LOG_INFO("called get interface version [%d] interface [%p]", version, interface);

	if (interface == nullptr) {
		LOG_ERROR("interface is null");
		return Error::nullPointer;
	}

	if (version != TCP_INSPECTION_VERSION) {
		LOG_ERROR("invalid version my version [%d]", TCP_INSPECTION_VERSION);
		return Error::invalidVersion;
	}

	uint32_t ref;

	{
		std::unique_lock<std::mutex> _lock(g_lock);

		if (g_interface == nullptr) {
			LOG_INFO("g_interface is null => create CTcpInspection object")
			g_interface = new (std::nothrow) CTcpInspection();
			if (g_interface == nullptr) {
				LOG_ERROR("could not allocate CTcpInspection");
				return Error::outOfMemory;
			}
		}

		*interface = static_cast<void*>(g_interface);
		ref = ++g_interfaceRef;
	}

	LOG_INFO("returned interface [%p] ref [%u]", *interface, ref);
	return Error::success;
}

void ReleaseInterface(void *interface)
{
	LOG_INFO("called release interface [%p]", interface);

	if (interface == nullptr) {
		LOG_ERROR("interface is null");
		return;
	}

	uint32_t ref;
	{
		std::unique_lock<std::mutex> _lock(g_lock);

		if (g_interface == nullptr) {
			LOG_ERROR("g_interface is null");
			return;
		}

		if (g_interface != interface) {
			LOG_ERROR("g_interface [%p] != interface [%p]", g_interface, interface);
			return;
		}

		ref = --g_interfaceRef;
		if (g_interfaceRef == 0) {
			LOG_INFO("interface ref is 0 => destroy CTcpInspection object");
			delete g_interface;
			g_interface = nullptr;
		}
	}

	LOG_INFO("current ref [%u]", ref);
}

}
