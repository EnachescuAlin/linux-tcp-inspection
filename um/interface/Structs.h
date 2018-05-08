#ifndef TCP_INSPECTION_STRUCTS
#define TCP_INSPECTION_STRUCTS

#include <cstdint>

namespace TcpInspection
{

enum DataVerdict : uint32_t
{
	block,
	allow,
	dropData,
	closeConnection
};

enum DataFlags : uint32_t
{
	normal,
	reinjected
};

enum Direction : uint32_t
{
	in,
	out
};

enum ConnectionVerdict : uint32_t
{
	filter,
	unfilter,
	dropConnection
};

struct Ipv4Address
{
	uint16_t port;
	uint32_t ip;
};

typedef uint32_t Pid;

} // end TcpInspection namespace

#endif
