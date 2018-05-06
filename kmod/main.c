// linux headers
#include <linux/module.h>
#include <linux/init.h>

// my headers
#include "include/log.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("tcp traffic inspection");

static int __init TcpInspection_init(void)
{
	LOG("initializing");
	return 0;
}

static void __exit TcpInspection_uninit(void)
{
	LOG("uninitializing");
}

module_init(TcpInspection_init);
module_exit(TcpInspection_uninit);
