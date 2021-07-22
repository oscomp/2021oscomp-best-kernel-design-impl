#ifndef __UNAME_H 
#define __UNAME_H 

#include "types.h"

/* Copy system information to an user-space `utsname`. */
int uname_copyout(uint64 addr);

#define UTSNAME_STR_LEN 	65
struct utsname {
	char sysname[UTSNAME_STR_LEN];
	char nodename[UTSNAME_STR_LEN];
	char release[UTSNAME_STR_LEN];
	char version[UTSNAME_STR_LEN];
	char machine[UTSNAME_STR_LEN];
	char domainname[UTSNAME_STR_LEN];
};

extern char const UNAME_SYSNAME[];
extern char const UNAME_NODENAME[];
extern char const UNAME_RELEASE[];
extern char const UNAME_VERSION[];
extern char const UNAME_MACHINE[];
extern char const UNAME_DOMAINNAME[];

#endif 
