#ifndef __LFS_H
#define __LFS_H
LW_API INT  API_LFSDrvInstall (VOID);
LW_API INT  API_LFsDevCreate (PCHAR   pcName, PLW_BLK_DEV  pblkd);
LW_API INT  API_LFsDevDelete (PCHAR   pcName);

#define lFsDrv             API_LFsDrvInstall
#define lFsDevCreate       API_LFsDevCreate
#define lFsDevDelete       API_LFsDevDelete

#endif
