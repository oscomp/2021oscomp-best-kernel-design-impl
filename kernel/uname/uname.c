#include <os/uname.h>
#include <os/string.h>
#include <stdio.h>

utsname_t utsname = {
    .sysname = "Linux",
    .nodename = "debian",
    .release = "5.10.0-7-riscv64",
    .version = "#1 SMP Debian 5.10.40-1 (2021-05-28)",
    .machine = "riscv64",
    .domainname = "(none)"
};

/* return name of my os
    0 : success
    -1: fail
    */
uint8 do_uname(struct utsname *uts)
{
    // if (sizeof(uts) != sizeof(utsname_t))
    //     return -1;
    strcpy(uts->sysname, utsname.sysname); 
    strcpy(uts->nodename, utsname.nodename); 
    strcpy(uts->release, utsname.release);
    strcpy(uts->version, utsname.version);
    strcpy(uts->machine, utsname.machine);
    strcpy(uts->domainname, utsname.domainname);
    return 0;
}