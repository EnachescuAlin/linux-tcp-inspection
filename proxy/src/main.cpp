#include <stdio.h>
#include <dlfcn.h>

#include <new>
#include <iostream>

#include "ITcpInspection.h"

using namespace std;

class Connection
	: public TcpInspection::IConnection
{
public:
	Connection(
		TcpInspection::IConnectionMgr *connMgr
	) : TcpInspection::IConnection(connMgr)
	{
	}

	~Connection() = default;

	TcpInspection::DataVerdict OnData(
		const uint8_t *buffer,
		uint32_t bufferSize,
		TcpInspection::Direction direction,
		TcpInspection::DataFlags flags
	) override
	{
		printf("on data => conn id [%lu] buffSize [%u], direction [%u]",
			GetConnectionMgr()->GetConnectionId(), bufferSize, direction
		);
		return TcpInspection::DataVerdict::allow;
	}

	void OnDisconnect(
		TcpInspection::Direction direction,
		TcpInspection::DisconnectFlags flags
	) override
	{
		printf("on disconnect => conn id [%lu], direction [%u], flags[%u]",
			GetConnectionMgr()->GetConnectionId(), direction, flags
		);
	}
};

class Proxy
	: public TcpInspection::IProxy
{
public:
	Proxy() = default;
	~Proxy() = default;

	uint32_t GetPriority() const override
	{
		return 1;
	}

	TcpInspection::ConnectionVerdict OnNewConnection(
		TcpInspection::IConnectionMgr *connMgr,
		TcpInspection::IConnection **conn
	) override
	{
		printf("new connection => conn id [%lu], pid [%d], path[%s]",
			connMgr->GetConnectionId(),
			connMgr->GetProcessPid(),
			connMgr->GetProcessPath()
		);

		*conn = new (std::nothrow) Connection(connMgr);
		if (*conn == nullptr) {
			return TcpInspection::ConnectionVerdict::unfilter;
		}
		return TcpInspection::ConnectionVerdict::filter;
	}

	void OnDestroyConnection(
		TcpInspection::IConnection *conn
	) override
	{
		printf("destroy connection => conn id [%lu]",
			conn->GetConnectionMgr()->GetConnectionId()
		);
	}
};

int main()
{
	void *handle;
	char *error;

	GetInterfacePfn get = NULL;
	ReleaseInterfacePfn release = NULL;
	TcpInspection::ITcpInspection *tcpInspection = nullptr;
	Proxy proxy;

	handle = dlopen ("../um/bin/TcpInspection.so", RTLD_NOW | RTLD_GLOBAL);
	if (!handle) {
		printf("dlopen failed %s\n", dlerror());
		return 1;
	}

	get = (GetInterfacePfn) dlsym(handle, "GetInterface");
	if ((error = dlerror()) != NULL)  {
		printf("dlsym GetInterfacePfn failed\n");
		return 1;
	}

	release = (ReleaseInterfacePfn) dlsym(handle, "ReleaseInterface");
	if ((error = dlerror()) != NULL) {
		printf("dlsym ReleaseInterfacePfn failed\n");
		return 1;
	}

	TcpInspection::Error err;
	err = get(TCP_INSPECTION_VERSION, (void**) &tcpInspection);
	if (err != TcpInspection::Error::success) {
		printf("get interface failed [%u]", err);
		return 1;
	}

	err = tcpInspection->RegisterProxy(&proxy);
	if (err != TcpInspection::Error::success) {
		printf("register proxy failed [%u]", err);
		return 1;
	}

	cout << "type 'q' to exit" << endl;

	char c;
	while (true) {
		cin >> c;
		if (c == 'q') {
			break;
		}
	}

	tcpInspection->UnregisterProxy(&proxy);
	release((void*)tcpInspection);

	dlclose(handle);

	return 0;
}