#ifndef TCP_INSPECTION_LOG
#define TCP_INSPECTION_LOG

// linux headers
#include <linux/printk.h>

#define LOG(format, ...)														\
	printk(KERN_INFO "[TcpInspection] [%s] " format "\n",						\
		__FUNCTION__, ##__VA_ARGS__)

#endif
