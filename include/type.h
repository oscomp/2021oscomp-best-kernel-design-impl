
#ifndef INCLUDE_TYPE_H_
#define INCLUDE_TYPE_H_

#ifndef NULL
#define NULL 	(void*)0
#endif

#ifndef TRUE
	#define TRUE 	1
	#define FALSE 	0
#endif

#ifndef IGNORE
    #define IGNORE 0
#endif

typedef unsigned __attribute__((__mode__(QI))) int8_t;
typedef unsigned __attribute__((__mode__(QI))) uint8_t;
typedef int      __attribute__((__mode__(HI))) int16_t;
typedef unsigned __attribute__((__mode__(HI))) uint16_t;
typedef int      __attribute__((__mode__(SI))) int32_t;
typedef unsigned __attribute__((__mode__(SI))) uint32_t;
typedef int      __attribute__((__mode__(DI))) int64_t;
typedef unsigned __attribute__((__mode__(DI))) uint64_t;

typedef char int8;
typedef unsigned __attribute__((__mode__(QI))) uint8;
typedef int      __attribute__((__mode__(HI))) int16;
typedef unsigned __attribute__((__mode__(HI))) uint16;
typedef int      __attribute__((__mode__(SI))) int32;
typedef unsigned __attribute__((__mode__(SI))) uint32;
typedef int      __attribute__((__mode__(DI))) int64;
typedef unsigned __attribute__((__mode__(DI))) uint64;

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned short wchar;

typedef int32_t pid_t;
typedef uint64_t reg_t;
typedef uint64_t ptr_t;
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint32_t fd_num_t;
typedef uint16_t unicode_t;
typedef uint32_t spawn_num_t;

typedef uint32_t pipe_num_t;

typedef uint64_t dev_t;
typedef uint64_t ino_t;
typedef unsigned int mode_t;
typedef uint32_t nlink_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef long int off_t;
typedef uint32_t blksize_t;
typedef uint64_t blkcnt_t;
typedef uint64_t sigval_t;

/* Minimum of signed integral types.  */
# define INT8_MIN               (-128)
# define INT16_MIN              (-32767-1)
# define INT32_MIN              (-2147483647-1)
# define INT64_MIN              (-9223372036854775807lu-1)
/* Maximum of signed integral types.  */
# define INT8_MAX               (127)
# define INT16_MAX              (32767)
# define INT32_MAX              (2147483647)
# define INT64_MAX              (9223372036854775807lu)

/* Maximum of unsigned integral types.  */
# define UINT8_MAX              (255)
# define UINT16_MAX             (65535)
# define UINT32_MAX             (4294967295U)
# define UINT64_MAX             (18446744073709551615lu)

#endif
