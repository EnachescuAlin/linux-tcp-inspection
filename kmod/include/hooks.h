#ifndef TCP_INSPECTION_HOOKS
#define TCP_INSPECTION_HOOKS

// linux headers
#include <linux/compiler.h>
#include <linux/socket.h>

// connect hook
typedef long (*connect_hook_fn_t)(
	int,
	struct sockaddr __user *,
	int
);
long TcpInspection_connect_hook(
	int,
	struct sockaddr __user *,
	int
);

// recvfrom hook
typedef long (*recvfrom_hook_fn_t)(
	int,
	void __user *,
	size_t,
	unsigned,
	struct sockaddr __user *,
	int __user *
);
long TcpInspection_recvfrom_hook(
	int,
	void __user *,
	size_t,
	unsigned,
	struct sockaddr __user *,
	int __user *
);

// sendto hook
typedef long (*sendto_hook_fn_t)(
	int,
	void __user *,
	size_t,
	unsigned,
	const struct sockaddr __user *,
	int
);
long TcpInspection_sendto_hook(
	int,
	void __user *,
	size_t,
	unsigned,
	const struct sockaddr __user *,
	int
);

#endif
