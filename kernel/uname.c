// System Related Information 

#ifndef __DEBUG_uname 
#undef DEBUG 
#endif 

#define __module_name__ 	"uname"

#include "include/types.h"
#include "include/proc.h"
#include "include/string.h"
#include "include/debug.h"

#include "include/uname.h"

char const *const UNAME_SYSNAME = "xv6-k210";
char const *const UNAME_NODENAME = "none";
char const *const UNAME_RELEASE = "v1.0";
char const *const UNAME_VERSION = __DATE__" "__TIME__;
#ifndef QEMU 
char const *const UNAME_MACHINE = "Kendryte K210";
#else 
char const *const UNAME_MACHINE = "QEMU";
#endif 
char const *const UNAME_DOMAINNAME = "none";

int uname_copyout(uint64 addr) {
	struct utsname *user_uts = (struct utsname*)addr;

	if (either_copyout(1, (uint64)user_uts->sysname, 
			(void*)UNAME_SYSNAME, strlen(UNAME_SYSNAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy sysname\n");
		return -1;
	}
	if (either_copyout(1, (uint64)user_uts->nodename, 
			(void*)UNAME_NODENAME, strlen(UNAME_NODENAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy nodename\n");
		return -1;
	}
	if (either_copyout(1, (uint64)user_uts->release, 
			(void*)UNAME_RELEASE, strlen(UNAME_RELEASE)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy release\n");
		return -1;
	}
	if (either_copyout(1, (uint64)user_uts->version, 
			(void*)UNAME_VERSION, strlen(UNAME_VERSION)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy version\n");
		return -1;
	}
	if (either_copyout(1, (uint64)user_uts->machine, 
			(void*)UNAME_MACHINE, strlen(UNAME_MACHINE)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy machine\n");
		return -1;
	}
	if (either_copyout(1, (uint64)user_uts->domainname, 
			(void*)UNAME_DOMAINNAME, strlen(UNAME_DOMAINNAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy domainname\n");
		return -1;
	}

	return 0;
}
