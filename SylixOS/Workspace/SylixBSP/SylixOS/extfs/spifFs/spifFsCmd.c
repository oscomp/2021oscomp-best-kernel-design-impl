/*
 * spifFsCmd.c
 *
 *  Created on: Jul 19, 2021
 *      Author: Deadpool
 */
#include "spifFsCmd.h"
PSPIF_VOLUME    _G_Volumn;

/*********************************************************************************************************
** 函数名称: spiffsEraseBlk
** 功能描述: 擦除一个块，因为Cache是与硬件相接，因此，将该函数放在这里
** 输　入  : pfs          文件头
**          blkIX         逻辑块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void spiffsCreateFile(PCHAR pFileName) {
    mode_t  mode            = DEFAULT_FILE_PERM;
    PCHAR   pcWriteBuffer   = "Hello spiffs   \0";

    INT     iFd;

    iFd = open(pFileName, O_WRONLY | O_CREAT | O_TRUNC, mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("%s create error!\n",pFileName);
        return ;
    }
    /* 文件统一写入"Hello spiffs" */
    printf("\t%s starts writing.\n", pFileName);
    write(iFd, pcWriteBuffer, lib_strlen(pcWriteBuffer));
    close(iFd);
}
/*********************************************************************************************************
** 函数名称: spiffsEraseBlk
** 功能描述: 擦除一个块，因为Cache是与硬件相接，因此，将该函数放在这里
** 输　入  : pfs          文件头
**          blkIX         逻辑块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL spiffsCheckFile(PCHAR pFileName){
    mode_t  mode                = DEFAULT_FILE_PERM;
    CHAR    ucReadBuffer[16];
    PCHAR   pcWriteBuffer       = "Hello spiffs   \0";
    INT     iFd;

    iFd = open(pFileName, O_RDONLY, mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("%s open error!\n",pFileName);
        return ;
    }
    /* 文件统一写入"Hello hoitfs" */
    printf("\t%s starts reading.\n", pFileName);
    lib_memset(ucReadBuffer, 0, 16);
    read(iFd, ucReadBuffer, 16);
    close(iFd);
    if(lib_strcmp(ucReadBuffer, pcWriteBuffer) == 0){
        return LW_TRUE;
    }
    else {
        return LW_FALSE;
    }
}
/*********************************************************************************************************
** 函数名称: spiffsEraseBlk
** 功能描述: 擦除一个块，因为Cache是与硬件相接，因此，将该函数放在这里
** 输　入  : pfs          文件头
**          blkIX         逻辑块号
** 输　出  : None
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT spiffs_cmd_wrapper(INT  iArgC, PCHAR  ppcArgV[]) {
    INT     i;
    CHAR    cFileName[100];
    INT     iRes;
    for (i = 0 ; i < 5 ; i++) {
        lib_memset(cFileName, 0, 100);
        sprintf(cFileName, "%d_NewFile", i);
        spiffsCreateFile(cFileName);
    }    

    for (i = 0; i < 5 ; i++)
    {
        lib_memset(cFileName, 0, 100);
        sprintf(cFileName, "%d_NewFile", i);
        iRes = spiffsCheckFile(cFileName);
        if(iRes == LW_FALSE){
            break;
        }
    }
    if(iRes){
        printf("pass!\n");
    }
    else {
        printf("some error occurs when read file %s\n", cFileName);
    }
}

VOID register_spiffs_cmd(PSPIF_VOLUME pfs) {
    _G_Volumn = pfs;
    API_TShellKeywordAdd("spif", spiffs_cmd_wrapper);
    // API_TShellKeywordAdd("ftt", hoitTestFileTree);
    // API_TShellKeywordAdd("fot", hoitTestFileOverWrite);
}
