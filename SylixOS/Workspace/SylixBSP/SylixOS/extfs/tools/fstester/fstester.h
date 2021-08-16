/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: fstester.h
**
** 创   建   人: Pan Yanqi
**
** 文件创建日期: 2021 年 07 月 27 日
**
** 描        述: 生成测试脚本与捕捉输出
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL

#ifndef SYLIXOS_EXTFS_TOOLS_FSTESTER_FSTESTER_H_
#define SYLIXOS_EXTFS_TOOLS_FSTESTER_FSTESTER_H_
#include "SylixOS.h"
#include "config.h"
#include "../list/list_interface.h"

// #define FSTESTER_WRRD_TEST
#define MAX_FUNC_NAME_LEN  128

typedef enum fs_type {
    FS_TYPE_SPIFFS,
    FS_TYPE_HOITFS,
                                                    /* 暂时不予实现 */
    FS_TYPE_YAFFS,                                                  
    FS_TYPE_FATFS
} FS_TYPE;

typedef enum test_type{
    TEST_TYPE_RDM_WR,
    TEST_TYPE_RDM_RD,           
    
    TEST_TYPE_SEQ_WR,
    TEST_TYPE_SEQ_RD,           

    TEST_TYPE_SMALL_WR,
    
    TEST_TYPE_MOUNT,
    TEST_TYPE_GC,
    TEST_TYPE_MERGEABLE_TREE
} TEST_TYPE;


typedef INT (*fstester_functionality)(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
typedef fstester_functionality FSTESTER_FUNCTIONALITY;

typedef struct fstester_func_node
{
    PCHAR                  *ppcOpts;
    INT                    iOptCnt;
    PCHAR                  pUsage;
    TEST_TYPE              testType;
    FSTESTER_FUNCTIONALITY functionality;
    PVOID                  pUserValue;
} FSTESTER_FUNC_NODE;
typedef FSTESTER_FUNC_NODE * PFSTESTER_FUNC_NODE;

static inline PFSTESTER_FUNC_NODE newFstesterCmdNode(PCHAR ppcOpts[], INT iOptCnt, PCHAR pUsage, TEST_TYPE testType,  
                                                     FSTESTER_FUNCTIONALITY functionality){
    PFSTESTER_FUNC_NODE pFuncNode = (PFSTESTER_FUNC_NODE)lib_malloc(sizeof(FSTESTER_FUNC_NODE));
    PCHAR               *ppcOptsCpys = (PCHAR *)lib_malloc(sizeof(PCHAR));
    INT                 i;
    PCHAR               pOpt;
    PCHAR               pOptCpy;
    for (i = 0; i < iOptCnt; i++)
    {
        pOpt = ppcOpts[i];
        pOptCpy = (PCHAR)lib_malloc(lib_strlen(pOpt));
        lib_strcpy(pOptCpy, pOpt);
        ppcOptsCpys[i] = pOptCpy;
    }
    pFuncNode->ppcOpts       = ppcOptsCpys;
    pFuncNode->iOptCnt       = iOptCnt;
    pFuncNode->pUsage        = pUsage;
    pFuncNode->testType      = testType;
    pFuncNode->functionality = functionality;
    return pFuncNode;
}

DECLARE_LIST_TEMPLATE(FSTESTER_FUNC_NODE);
/*********************************************************************************************************
  测试函数API
*********************************************************************************************************/
INT __fstesterRandomRead(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterSequentialRead(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterRandomWrite(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterSequentialWrite(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterSmallWrite(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterMount(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterGC(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
INT __fstesterMergeableTree(INT iFdTest, UINT uiTestRange, UINT uiLoopTimes, PCHAR pMountPoint, PVOID pUserValue);
/*********************************************************************************************************
  工具函数
*********************************************************************************************************/
/*********************************************************************************************************
** 函数名称: translateFSType
** 功能描述: 翻译FSType
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline const PCHAR translateFSType(FS_TYPE fsType){
    switch (fsType)
    {
    case FS_TYPE_SPIFFS:
        return "spiffs";
    case FS_TYPE_HOITFS:
        return "hoitfs";
    default:
        return "unsupported";
    }
}
static inline const PCHAR translateTestType(TEST_TYPE testType){
    switch (testType)
    {
    case TEST_TYPE_RDM_WR:
        return "random-write-test";
    case TEST_TYPE_RDM_RD:
        return "random-read-test";
    case TEST_TYPE_SEQ_WR:
        return "sequence-write-test";
    case TEST_TYPE_SEQ_RD:
        return "sequence-read-test";
    case TEST_TYPE_SMALL_WR:
        return "small-write-test";
    case TEST_TYPE_MOUNT:
        return "mount-test";
    case TEST_TYPE_GC:
        return "gc-test";
    case TEST_TYPE_MERGEABLE_TREE:
        return "mergeable-tree-test";
    default:
        return "unsupported";
    }
}
/*********************************************************************************************************
** 函数名称: translateFSType
** 功能描述: 翻译FSType
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline FS_TYPE getFSTypeByStr(PCHAR fsTypeStr){
    if(lib_strcmp(fsTypeStr, "spiffs") == 0){
        return FS_TYPE_SPIFFS;
    }
    else if(lib_strcmp(fsTypeStr, "hoitfs") == 0){
        return FS_TYPE_HOITFS;
    }
    else {                                                  /* 其余先不做选项 */
        return FS_TYPE_HOITFS;
    }
}
/*********************************************************************************************************
** 函数名称: getFSMountPoint
** 功能描述: 生成挂载点
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline const PCHAR getFSMountPoint(FS_TYPE fsType){
    switch (fsType)
    {
    case FS_TYPE_SPIFFS:
        return "/mnt/spiffs";
    case FS_TYPE_HOITFS:
        return "/mnt/hoitfs";
    default:
        return "unsupported";
    }
}  
/*********************************************************************************************************
** 函数名称: getFSTestOutputDir
** 功能描述: 生成测试输出文件夹路径
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline const PCHAR getFSTestOutputDir(FS_TYPE fsType, TEST_TYPE testType){
    switch (fsType)
    {
    case FS_TYPE_SPIFFS:
        return "/root/spiffs-test";
    case FS_TYPE_HOITFS:
        return "/root/hoitfs-test";
    default:
        return "unsupported";
    }
}
/*********************************************************************************************************
** 函数名称: getFSTestOutputPath
** 功能描述: 生成测试输出文件路径
** 输　入  : pfs          文件头
**           pObjId        返回的Object ID
**           pucConflictingName 文件路径名
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline PCHAR getFSTestOutputPath(FS_TYPE fsType, TEST_TYPE testType){
    PCHAR pOutputPath;
    PCHAR pOutputDir;
    PCHAR pOutputFileName;
    PCHAR pOutputBasedir = "/root";
    switch (fsType)
    {
    case FS_TYPE_SPIFFS:
        pOutputDir = "spiffs-test";
        break;
    case FS_TYPE_HOITFS:
        pOutputDir = "hoitfs-test";
        break;
    default:
        pOutputDir = "unsupported";
        break;
    }

    switch (testType)
    {
    case TEST_TYPE_RDM_WR:
        pOutputFileName = "out-random-write-test";
        break;
    case TEST_TYPE_RDM_RD:
        pOutputFileName = "out-random-read-test";
        break;
    case TEST_TYPE_SEQ_WR:
        pOutputFileName = "out-sequence-write-test";
        break;
    case TEST_TYPE_SEQ_RD:
        pOutputFileName = "out-sequence-read-test";
        break;
    case TEST_TYPE_SMALL_WR:
        pOutputFileName = "out-small-write-test";
        break;
    case TEST_TYPE_MOUNT:
        pOutputFileName = "out-mount-test";
        break;
    case TEST_TYPE_GC:
        pOutputFileName = "out-gc-test";
        break;
    case TEST_TYPE_MERGEABLE_TREE:
        pOutputFileName = "out-mergeable-tree-test";
        break;
    default:
        break;
    }
    asprintf(&pOutputPath, "%s/%s/%s", pOutputBasedir, pOutputDir, pOutputFileName);
    return pOutputPath;
}
/*********************************************************************************************************
** 函数名称: uiPower
** 功能描述: 幂运算
** 输　入  : index      指数
**           base       底数
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static inline UINT uiPower(UINT index, UINT base) {
    UINT result = 1;
    UINT i;
    for(i = 0; i < index; i++) {
        result *= base;
    }
    return result;
}

#define FSTESTER_SEED   2302
VOID register_fstester_cmd();

#endif /* SYLIXOS_EXTFS_TOOLS_FSTESTER_FSTESTER_H_ */
