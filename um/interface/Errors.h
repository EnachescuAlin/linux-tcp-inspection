#ifndef TCP_INSPECTION_ERRORS
#define TCP_INSPECTION_ERRORS

#include <cstdint>

namespace TcpInspection
{

enum Error : uint32_t
{
	success,
	nullPointer,
	proxyAlreadyRegistered,
	proxyNotFound,
	outOfMemory,
	invalidState,
	priorityAlreadyExists,
	invalidVersion,
	invalidParameter,
	connNotFound,
	connAlreadyExists
};

} // end TcpInspection namespace

#endif
