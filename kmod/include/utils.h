#ifndef TCP_INSPECTION_UTILS
#define TCP_INSPECTION_UTILS

// linux header
#include <linux/net.h>

int TcpInspection_fd_is_socket(unsigned int fd);

int TcpInspection_get_socket(unsigned int fd, struct socket **sock);

void TcpInspection_release_socket(struct socket *sock);

#endif
