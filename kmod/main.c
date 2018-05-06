// linux headers
#include <linux/module.h>
#include <linux/init.h>

// my headers
#include "include/errors.h"
#include "include/log.h"
#include "include/syscall_hooks.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("tcp traffic inspection");

static int __init TcpInspection_init(void)
{
	int err = TCP_INSPECTION_NO_ERROR;

	do {
		LOG("initializing module");

		err = TcpInspection_set_hooks();
		if (err != TCP_INSPECTION_NO_ERROR) {
			LOG("error, set hooks failed [%d]", err);
			break;
		}

		LOG("module initialized successfully");
		goto no_error;
	} while (0);

no_error:
	return err;
}

static void __exit TcpInspection_uninit(void)
{
	LOG("uninitializing module");

	TcpInspection_remove_hooks();

	LOG("module uninitialized successfully");
}

module_init(TcpInspection_init);
module_exit(TcpInspection_uninit);
