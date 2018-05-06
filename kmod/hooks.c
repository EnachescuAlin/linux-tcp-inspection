// linux headers
#include <linux/sched.h>

// my headers
#include "include/hooks.h"
#include "include/log.h"

extern connect_hook_fn_t 	original_connect;
extern recvfrom_hook_fn_t 	original_recvfrom;
extern sendto_hook_fn_t 	original_sendto;

long TcpInspection_connect_hook(
	int sockfd,
	struct sockaddr __user *addr,
	int addrlen)
{
	if (strcmp(current->comm, "curl") == 0) {
		LOG("called connect hook");
	}

	return original_connect(sockfd, addr, addrlen);
}

long TcpInspection_recvfrom_hook(
	int sockfd,
	void __user *buf,
	size_t len,
	unsigned flags,
	struct sockaddr __user *src_addr,
	int __user *addrlen)
{
	if (strcmp(current->comm, "curl") == 0) {
		LOG("called recvfrom hook");
	}

	return original_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

long TcpInspection_sendto_hook(
	int sockfd,
	void __user *buf,
	size_t len,
	unsigned flags,
	const struct sockaddr __user *dest_addr,
	int addrlen)
{
	if (strcmp(current->comm, "curl") == 0) {
		LOG("called sendto hook");
	}

	return original_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}
