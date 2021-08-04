#ifndef __ERRNO_H
#define __ERRNO_H

#define	EPERM			 1	/* Operation not permitted */
#define	ENOENT			 2	/* No such file or directory */
#define	ESRCH			 3	/* No such process */
#define	EINTR			 4	/* Interrupted system call */
#define	EIO				 5	/* I/O error */
#define	ENXIO			 6	/* No such device or address */
#define	E2BIG			 7	/* Argument list too long */
#define	ENOEXEC			 8	/* Exec format error */
#define	EBADF			 9	/* Bad file number */
#define	ECHILD			10	/* No child processes */
#define	EAGAIN			11	/* Try again */
#define	ENOMEM			12	/* Out of memory */
#define	EACCES			13	/* Permission denied */
#define	EFAULT			14	/* Bad address */
#define	ENOTBLK			15	/* Block device required */
#define	EBUSY			16	/* Device or resource busy */
#define	EEXIST			17	/* File exists */
#define	EXDEV			18	/* Cross-device link */
#define	ENODEV			19	/* No such device */
#define	ENOTDIR			20	/* Not a directory */
#define	EISDIR			21	/* Is a directory */
#define	EINVAL			22	/* Invalid argument */
#define	ENFILE			23	/* File table overflow */
#define	EMFILE			24	/* Too many open files */
#define	ENOTTY			25	/* Not a typewriter */
#define	ETXTBSY			26	/* Text file busy */
#define	EFBIG			27	/* File too large */
#define	ENOSPC			28	/* No space left on device */
#define	ESPIPE			29	/* Illegal seek */
#define	EROFS			30	/* Read-only file system */
#define	EMLINK			31	/* Too many links */
#define	EPIPE			32	/* Broken pipe */
#define	EDOM			33	/* Math argument out of domain of func */
#define	ERANGE			34	/* Math result not representable */

#define	EDEADLK			35	/* Resource deadlock would occur */
#define	ENAMETOOLONG	36	/* File name too long */
#define	ENOLCK			37	/* No record locks available */

#define	ENOSYS			38	/* Invalid system call number */
#define	ENOTEMPTY		39	/* Directory not empty */
#define	ELOOP			40	/* Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	ENOMSG			42	/* No message of desired type */
#define	EIDRM			43	/* Identifier removed */
#define	ECHRNG			44	/* Channel number out of range */
#define	EL2NSYNC		45	/* Level 2 not synchronized */
#define	EL3HLT			46	/* Level 3 halted */
#define	EL3RST			47	/* Level 3 reset */
#define	ELNRNG			48	/* Link number out of range */
#define	EUNATCH			49	/* Protocol driver not attached */
#define	ENOCSI			50	/* No CSI structure available */
#define	EL2HLT			51	/* Level 2 halted */
#define	EBADE			52	/* Invalid exchange */
#define	EBADR			53	/* Invalid request descriptor */
#define	EXFULL			54	/* Exchange full */
#define	ENOANO			55	/* No anode */
#define	EBADRQC			56	/* Invalid request code */
#define	EBADSLT			57	/* Invalid slot */

#define	EDEADLOCK  EDEADLK

#define	EBFONT			59	/* Bad font file format */
#define	ENOSTR			60	/* Device not a stream */
#define	ENODATA			61	/* No data available */
#define	ETIME			62	/* Timer expired */
#define	ENOSR			63	/* Out of streams resources */
#define	ENONET			64	/* Machine is not on the network */
#define	ENOPKG			65	/* Package not installed */
#define	EREMOTE			66	/* Object is remote */
#define	ENOLINK			67	/* Link has been severed */
#define	EADV			68	/* Advertise error */
#define	ESRMNT			69	/* Srmount error */
#define	ECOMM			70	/* Communication error on send */
#define	EPROTO			71	/* Protocol error */
#define	EMULTIHOP		72	/* Multihop attempted */
#define	EDOTDOT			73	/* RFS specific error */
#define	EBADMSG			74	/* Not a data message */
#define	EOVERFLOW		75	/* Value too large for defined data type */
#define	ENOTUNIQ		76	/* Name not unique on network */
#define	EBADFD			77	/* File descriptor in bad state */
#define	EREMCHG			78	/* Remote address changed */
#define	ELIBACC			79	/* Can not access a needed shared library */
#define	ELIBBAD			80	/* Accessing a corrupted shared library */
#define	ELIBSCN			81	/* .lib section in a.out corrupted */
#define	ELIBMAX			82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC		83	/* Cannot exec a shared library directly */
#define	EILSEQ			84	/* Illegal byte sequence */
#define	ERESTART		85	/* Interrupted system call should be restarted */
#define	ESTRPIPE		86	/* Streams pipe error */
#define	EUSERS			87	/* Too many users */
#define	ENOTSOCK		88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE		90	/* Message too long */
#define	EPROTOTYPE		91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT		92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP		95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE		98	/* Address already in use */

#endif
