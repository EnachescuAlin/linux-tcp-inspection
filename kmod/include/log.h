#ifndef TCP_INSPECTION_LOG
#define TCP_INSPECTION_LOG

#define LOG(format, ...)														\
	printk(KERN_INFO "[TcpInspection] [%s] " format "\n",						\
		__FUNCTION__, ##__VA_ARGS__)

#endif
