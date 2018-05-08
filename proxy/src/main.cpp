#include <stdio.h>
#include <dlfcn.h>

#include "ITcpInspection.h"

int main()
{
	void *handle;
	char *error;

	GetInterfacePfn get = NULL;
	ReleaseInterfacePfn release = NULL;

	handle = dlopen ("../um/bin/TcpInspection.so", RTLD_LAZY);
	if (!handle) {
		printf("dlopen failed\n");
		return 1;
	}

	get = (GetInterfacePfn) dlsym(handle, "GetInterface");
	if ((error = dlerror()) != NULL)  {
		printf("dlsym failed 1\n");
		return 1;
	}

	release = (ReleaseInterfacePfn) dlsym(handle, "ReleaseInterface");
	if ((error = dlerror()) != NULL) {
		printf("dlsym failed 2\n");
		return 1;
	}

	get(0, NULL);
	release(NULL);

	dlclose(handle);

	return 0;
}