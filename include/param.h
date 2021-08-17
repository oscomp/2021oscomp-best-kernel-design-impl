#ifndef __PARAM_H
#define __PARAM_H

#define NPROC        50  // maximum number of processes
#define NCPU          2  // maximum number of CPUs
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXENV        8  // max execve envs
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       1000  // size of file system in blocks
#define MAXPATH      260   // maximum file path name

#ifndef QEMU 
//#define INTERVAL     (390000000 / 200) // timer interrupt interval
// assume that K210 system freq is 26MHz
// #define CLK_FREQ 		8900000
#define CLK_FREQ 		26000000
// #define CLK_FREQ 		10000000 
#else 
// QEMU system freq is 12.5MHz 
// #define CLK_FREQ 		12500000
#define CLK_FREQ 		10000000
#endif 

// set the interval as 50ms 
// #define INTERVAL 		(CLK_FREQ / 20)
#define INTERVAL			CLK_FREQ

#endif
