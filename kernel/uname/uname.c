#include <os/uname.h>
#include <os/string.h>
#include <stdio.h>

utsname_t utsname = {
    .sysname = "OSKernel-2021-404",
    .nodename = "@_@",
    .release = "Top Classified",
    .version = "0.0.0.0",
    .machine = "Dage",
    .domainname = "World Wide Web"
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