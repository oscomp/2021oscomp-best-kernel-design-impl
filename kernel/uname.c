// System Related Information 

#ifndef __DEBUG_uname 
#undef DEBUG 
#endif 

#define __module_name__ 	"uname"

#include "include/types.h"
#include "include/vm.h"
#include "include/string.h"
#include "include/debug.h"

#include "include/uname.h"

char const UNAME_SYSNAME[] = "xv6-k210";
char const UNAME_NODENAME[] = "none";
char const UNAME_RELEASE[] = "5.0";
char const UNAME_VERSION[] = __DATE__" "__TIME__;
#ifndef QEMU 
char const UNAME_MACHINE[] = "Kendryte K210";
#else 
char const UNAME_MACHINE[] = "QEMU";
#endif 
char const UNAME_DOMAINNAME[] = "none";

int uname_copyout(uint64 addr) {
	struct utsname *user_uts = (struct utsname*)addr;

	if (copyout2((uint64)user_uts->sysname, 
			(void*)UNAME_SYSNAME, sizeof(UNAME_SYSNAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy sysname\n");
		return -1;
	}
	if (copyout2((uint64)user_uts->nodename, 
			(void*)UNAME_NODENAME, sizeof(UNAME_NODENAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy nodename\n");
		return -1;
	}
	if (copyout2((uint64)user_uts->release, 
			(void*)UNAME_RELEASE, sizeof(UNAME_RELEASE)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy release\n");
		return -1;
	}
	if (copyout2((uint64)user_uts->version, 
			(void*)UNAME_VERSION, sizeof(UNAME_VERSION)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy version\n");
		return -1;
	}
	if (copyout2((uint64)user_uts->machine, 
			(void*)UNAME_MACHINE, sizeof(UNAME_MACHINE)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy machine\n");
		return -1;
	}
	if (copyout2((uint64)user_uts->domainname, 
			(void*)UNAME_DOMAINNAME, sizeof(UNAME_DOMAINNAME)) < 0) 
	{
		__debug_warn("uname_copyout", "fail to copy domainname\n");
		return -1;
	}

	return 0;
}
