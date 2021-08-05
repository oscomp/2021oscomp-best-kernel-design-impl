#include <os/uname.h>
#include <os/string.h>
#include <stdio.h>

utsname_t utsname = {
    .sysname = "Linux",
    .nodename = "debian",
    .release = "5.8.0-59-generic",
    .version = "#66~20.04.1-Ubuntu",
    .machine = "Sipeed M1 DOCK",
    .domainname = "NIS domain name"
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