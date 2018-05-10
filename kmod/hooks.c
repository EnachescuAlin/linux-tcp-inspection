// linux headers
#include <linux/sched.h>
#include <linux/net.h>
#include <net/sock.h>
#include <net/inet_sock.h>
#include <linux/kprobes.h>

// my headers
#include "include/hooks.h"
#include "include/log.h"
#include "include/utils.h"
#include "include/errors.h"

extern connect_hook_fn_t 	original_connect;
extern recvfrom_hook_fn_t 	original_recvfrom;
extern sendto_hook_fn_t 	original_sendto;
extern shutdown_hook_fn_t	original_shutdown;
extern close_hook_fn_t		original_close;

long TcpInspection_connect_hook(
	int sockfd,
	struct sockaddr __user *addr,
	int addrlen)
{
	struct socket *sock;
	struct inet_sock *inetSock;
	int err;
	int saddr, sport, daddr, dport;
	struct sockaddr_in *dest;
	long res;

	err = TcpInspection_get_socket(sockfd, &sock);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, get socket failed [%d]", err);
		return EBADFD;
	}

	if (strcmp(current->comm, "curl") == 0) {
		if (sock->type == SOCK_STREAM && sock->sk->sk_family == AF_INET) {
			LOG("called connect hook [%d] pid %d", sockfd, current->pid);

			res = original_connect(sockfd, addr, addrlen);

			inetSock = inet_sk(sock->sk);
			dest = (struct sockaddr_in*) addr;

			saddr = inetSock->inet_saddr;
			sport = inetSock->inet_sport;

			//daddr = dest->sin_addr.s_addr;
			//dport = dest->sin_port;
			daddr = 0; dport = 0;

			LOG("sport[%d] dport[%d] saddr[%d] daddr[%d]",
				sport, dport, saddr, daddr);

			return res;
		}
	}

	TcpInspection_release_socket(sock);
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
	return original_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

long TcpInspection_shutdown_hook(
	int sockfd,
	int how)
{
	return original_shutdown(sockfd, how);
}

long TcpInspection_close_hook(
	unsigned int fd)
{
	int err;

	err = TcpInspection_fd_is_socket(fd);
	if (err != TCP_INSPECTION_NO_ERROR) {
		if (err != TCP_INSPECTION_FD_IS_NOT_SOCKET) {
			LOG("error, fd is socket failed [%d]", err);
		}
		return original_close(fd);
	}

	return original_close(fd);
}
