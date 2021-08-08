#include <type.h>

typedef struct utsname {
    uchar sysname[65];
    uchar nodename[65];
    uchar release[65];
    uchar version[65];
    uchar machine[65];
    uchar domainname[65];
}utsname_t;

extern uint8 do_uname(struct utsname *uts);