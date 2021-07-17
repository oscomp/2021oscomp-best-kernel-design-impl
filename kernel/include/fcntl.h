#ifndef __FCNTL_H
#define __FCNTL_H

#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x040
#define O_TRUNC   0x200
#define O_APPEND  0x400
#define O_DIRECTORY 0x010000
#define O_CLOEXEC 0x80000

#define AT_FDCWD  -100
#define FD_CLOEXEC 1

#define F_DUPFD         1
#define F_DUPFD_CLOEXEC 1030

#endif