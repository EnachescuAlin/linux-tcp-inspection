#include "TcpInspection.h"
#include "Logging.h"

#include "Errors.h"

#include <stdio.h>

extern "C"
{

TcpInspection::Error GetInterface(int version, void **interface)
{
	printf("called get\n");
	return TcpInspection::Error::success;
}

void ReleaseInterface(void *interface)
{
	printf("called release\n");
}

}
