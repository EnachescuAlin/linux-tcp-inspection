// linux headers
#include <linux/kallsyms.h>
#include <linux/syscalls.h>
#include <linux/socket.h>
#include <linux/unistd.h>

// my headers
#include "include/syscall_hooks.h"
#include "include/log.h"
#include "include/errors.h"
#include "include/hooks.h"

connect_hook_fn_t 	original_connect 	= NULL;
recvfrom_hook_fn_t 	original_recvfrom	= NULL;
sendto_hook_fn_t 	original_sendto		= NULL;

#define SET_HOOK(fn_t, addr, hookId, hookFn, hookBackup) 						\
	hookBackup = (fn_t)((unsigned long*) addr)[hookId]; 						\
	((unsigned long*) addr)[hookId] = (unsigned long) &hookFn

#define UNSET_HOOK(addr, hookId, originalSyscall) 								\
	((unsigned long*) addr)[hookId] = (unsigned long) originalSyscall

static int TcpInspection_get_syscall_table(unsigned long *syscallTableAddr)
{
	unsigned long tmpSyscallTableAddr;

	if (syscallTableAddr == NULL) {
		LOG("error, syscallTableAddr is null");
		return TCP_INSPECTION_NULL_POINTER;
	}

	tmpSyscallTableAddr = kallsyms_lookup_name("sys_call_table");
	
	if (tmpSyscallTableAddr == 0) {
		LOG("error, kallsyms_lookup_name returned 0");
		return TCP_INSPECTION_CANNOT_GET_SYSCALL_TABLE_ADDR;
	}

	LOG("info, found syscall table address [%p]", (void*) tmpSyscallTableAddr);
	*syscallTableAddr = tmpSyscallTableAddr;

	return TCP_INSPECTION_NO_ERROR;
}

static int TcpInspection_make_page_rw(unsigned long addr)
{
	pte_t * pte;
	unsigned int level;

	pte = lookup_address(addr, &level);
	if (pte == NULL) {
		LOG("error, lookup address returned null");
		return TCP_INSPECTION_NULL_POINTER;
	}

	if (pte->pte & ~_PAGE_RW) {
		pte->pte |= _PAGE_RW;
	}

	return TCP_INSPECTION_NO_ERROR;
}

static int TcpInspection_make_page_ro(unsigned long addr)
{
	pte_t * pte;
	unsigned int level;

	pte = lookup_address(addr, &level);
	if (pte == NULL) {
		LOG("error, lookup address returned null");
		return TCP_INSPECTION_NULL_POINTER;
	}

	pte->pte = pte->pte & ~_PAGE_RW;

	return TCP_INSPECTION_NO_ERROR;
}

int TcpInspection_set_hooks(void)
{
	unsigned long addr;
	int err;

	err = TcpInspection_get_syscall_table(&addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, get syscall table failed [%d]", err);
		return err;
	}

	err = TcpInspection_make_page_rw(addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, make page rw failed [%d]", err);
		return TCP_INSPECTION_CANNOT_CHANGE_PAGE_PROTECTION;
	}

	SET_HOOK(
		sendto_hook_fn_t,
		addr,
		__NR_sendto,
		TcpInspection_sendto_hook,
		original_sendto
	);

	SET_HOOK(
		recvfrom_hook_fn_t,
		addr,
		__NR_recvfrom,
		TcpInspection_recvfrom_hook,
		original_recvfrom
	);

	SET_HOOK(
		connect_hook_fn_t,
		addr,
		__NR_connect,
		TcpInspection_connect_hook,
		original_connect
	);

	err = TcpInspection_make_page_ro(addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, make page ro failed [%d]", err);
		// do not return error
	}

	return TCP_INSPECTION_NO_ERROR;
}

void TcpInspection_remove_hooks(void)
{
	unsigned long addr;
	int err;

	err = TcpInspection_get_syscall_table(&addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, get syscall table failed [%d]", err);
		return;
	}

	err = TcpInspection_make_page_rw(addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, make page rw failed [%d]", err);
		return;
	}

	UNSET_HOOK(addr, __NR_sendto, original_sendto);
	UNSET_HOOK(addr, __NR_recvfrom, original_recvfrom);
	UNSET_HOOK(addr, __NR_connect, original_connect);

	err = TcpInspection_make_page_ro(addr);
	if (err != TCP_INSPECTION_NO_ERROR) {
		LOG("error, make page ro failed [%d]", err);
	}
}
