#ifndef INCLUDE_SYSTEM_H
#define INCLUDE_SYSTEM_H

#include <log.h>
#include <os/time.h>
#include <type.h>
#include <os/errno.h>
#include <system/resource.h>

#define SYSCALL_SUCCESSED 0
#define SYSCALL_FAILED -1
#ifndef IGNORE
    #define IGNORE 0
#endif


struct sysinfo {
    long uptime;             /* Seconds since boot */
    unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
    unsigned long totalram;  /* Total usable main memory size */
    unsigned long freeram;   /* Available memory size */
    unsigned long sharedram; /* Amount of shared memory */
    unsigned long bufferram; /* Memory used by buffers */
    unsigned long totalswap; /* Total swap space size */
    unsigned long freeswap;  /* Swap space still available */
    unsigned short procs;    /* Number of current processes */
    unsigned long totalhigh; /* Total high memory size */
    unsigned long freehigh;  /* Available high memory size */
    unsigned int mem_unit;   /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding to 64 bytes */
};

#define RUSAGE_SELF 0
#define RUSAGE_CHILDREN -1

int do_prlimit(pid_t pid,   int resource,  const struct rlimit *new_limit, 
    struct rlimit *old_limit);
void do_syslog();
int32_t do_sysinfo(struct sysinfo *info);
int do_vm86(unsigned long fn, void *v86);

#endif