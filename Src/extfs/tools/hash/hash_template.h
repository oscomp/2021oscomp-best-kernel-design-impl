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
** 文   件   名: hash_template.h
**
** 创   建   人: Pan Yanqi
**
** 文件创建日期: 2021 年 06 月 02 日
**
** 描        述: hash函数模板
*********************************************************************************************************/

#ifndef SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_
#define SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_

#include "SylixOS.h"
UINT32 hash(PUCHAR pucString, UINT32 uiMaxSize) {
    UINT32 uiHash = 5381;
    UCHAR uChar;
    UINT i = 0;
    while ((uChar = pucString[i++]) && i < uiMaxSize) {
        uiHash = (uiHash * 33) ^ uChar;
    }
    return uiHash;
}
#endif /* SYLIXOS_EXTFS_TOOLS_HASH_HASH_TEMPLATE_H_ */