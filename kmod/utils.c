// linux headers
#include <linux/file.h>
#include <linux/fs.h>

// my headers
#include "include/utils.h"
#include "include/errors.h"
#include "include/log.h"

int TcpInspection_fd_is_socket(unsigned int fd)
{
	struct file *f;
	int err = TCP_INSPECTION_FD_IS_NOT_SOCKET;

	f = fget(fd);
	if (f == NULL) {
		LOG("error, fget failed fd[%u]", fd);
		return TCP_INSPECTION_NOT_FOUND;
	}

	if (S_ISSOCK(f->f_path.dentry->d_inode->i_mode)) {
		err = TCP_INSPECTION_NO_ERROR;
	}

	fput(f);

	return err;
}

int TcpInspection_get_socket(unsigned int fd, struct socket **sock)
{
	struct file *f;

	if (sock == NULL) {
		return TCP_INSPECTION_NULL_POINTER;
	}

	f = fget(fd);
	if (f == NULL) {
		LOG("error, fget failed fd[%u]", fd);
		return TCP_INSPECTION_NOT_FOUND;
	}

	if ( ! (S_ISSOCK(f->f_path.dentry->d_inode->i_mode))) {
		fput(f);
		return TCP_INSPECTION_FD_IS_NOT_SOCKET;
	}

	*sock = (struct socket*) f->private_data;
	if (*sock == NULL) {
		fput(f);
		return TCP_INSPECTION_INVALID_SOCKET;
	}

	return TCP_INSPECTION_NO_ERROR;
}

void TcpInspection_release_socket(struct socket *sock)
{
	if (sock != NULL) {
		fput(sock->file);
	}
}
