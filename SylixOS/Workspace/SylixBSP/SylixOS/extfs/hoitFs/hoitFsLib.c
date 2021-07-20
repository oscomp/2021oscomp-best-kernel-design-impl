/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: HoitFsLib.c
**
** 创   建   人: Hu Zhisheng
**
** 文件创建日期: 2021 年 03 月 20 日
**
** 描        述: Hoit文件系统内部函数.
*********************************************************************************************************/

#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
//#include "../SylixOS/kernel/include/k_kernel.h"
//#include "../SylixOS/system/include/s_system.h"
//#include "../SylixOS/fs/include/fs_fs.h"
//#include "hoitFsTree.h"
#include "hoitFsLib.h"
#include "hoitFsTree.h"
#include "hoitFsFDLib.h"
#include "hoitFsCache.h"
#include "hoitFsCmd.h"
#include "hoitFsLog.h"
#include "hoitWriteBuffer.h"
#include "../../driver/mtd/nor/nor.h"
#include "../tools/crc/crc32.h"
/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0

#ifndef HOITFSLIB_DISABLE
#define NS hoitFsLib
/*********************************************************************************************************
** 函数名称: __hoit_just_open
** 功能描述: 打开某个已打开的目录文件下面的一个文件
**           注意pcName是该目录文件下的一个文件名(相对路径)，要打开的文件必须是目录文件pdir的直接子文件，否则返回NULL
** 输　入  :
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_INFO  __hoit_just_open(PHOIT_INODE_INFO  pdir,
    PCHAR       pName)
{
    if (pdir == LW_NULL || !S_ISDIR(pdir->HOITN_mode)) {
        printk("Error in hoit_just_open\n");
        return (LW_NULL);
    }
    UINT newHash = __hoit_name_hash(pName);
    PHOIT_FULL_DIRENT pfile = pdir->HOITN_dents;
    while (pfile != LW_NULL) {
        if (pfile->HOITFD_nhash == newHash && lib_strcmp(pfile->HOITFD_file_name, pName) == 0) {
            return __hoit_get_full_file(pdir->HOITN_volume, pfile->HOITFD_ino);
        }
        else {
            pfile = pfile->HOITFD_next;
        }
    }

    return  (LW_NULL);                                                  /*  无法找到节点                */
}

/*********************************************************************************************************
** 函数名称: __hoit_name_hash
** 功能描述: 根据文件名计算出其hash值
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT __hoit_name_hash(CPCHAR pcName) {
    UINT ret = 0;
    while (*pcName != PX_EOS) {
        ret += *pcName;
        pcName++;
    }
    return ret;
}
/*********************************************************************************************************
** 函数名称: __hoit_free_full_dirent
** 功能描述: 释放FullDirent及其文件名
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT __hoit_free_full_dirent(PHOIT_FULL_DIRENT pDirent) {
    __SHEAP_FREE(pDirent->HOITFD_file_name);
    __SHEAP_FREE(pDirent);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_get_full_file
** 功能描述: 根据inode number，创建相应full_xxx结构体（目录文件创建出链表，普通文件创建出红黑树）
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_INFO __hoit_get_full_file(PHOIT_VOLUME pfs, UINT ino) {
    if (pfs == LW_NULL) {
        printk("Error in hoit_get_full_file\n");
        return LW_NULL;
    }

    if (ino == HOIT_ROOT_DIR_INO && pfs->HOITFS_pRootDir) {
        return pfs->HOITFS_pRootDir;
    }

    PHOIT_INODE_CACHE pcache = pfs->HOITFS_cache_list;

    while (pcache && pcache->HOITC_ino != ino) {
        pcache = pcache->HOITC_next;
    }
    if (pcache == LW_NULL) {
        return LW_NULL;
    }
    PHOIT_FULL_DIRENT   pDirentList = LW_NULL;
    PHOIT_FULL_DNODE    pDnodeList  = LW_NULL;
    PHOIT_FULL_DIRENT*  ppDirentList = &pDirentList;
    PHOIT_FULL_DNODE*   ppDnodeList  = &pDnodeList;

    __hoit_get_inode_nodes(pfs, pcache, ppDirentList, ppDnodeList);
    if (*ppDnodeList == LW_NULL) {
        return LW_NULL;
    }
    if (S_ISDIR((*ppDnodeList)->HOITFD_file_type)) {
        PHOIT_INODE_INFO pNewInode = (PHOIT_INODE_INFO)__SHEAP_ALLOC(sizeof(HOIT_INODE_INFO));
        pNewInode->HOITN_dents = *ppDirentList;
        pNewInode->HOITN_metadata = *ppDnodeList;
        pNewInode->HOITN_ino = ino;
        pNewInode->HOITN_inode_cache = pcache;
        pNewInode->HOITN_mode = (*ppDnodeList)->HOITFD_file_type;
        pNewInode->HOITN_rbtree = (PVOID)LW_NULL;
        pNewInode->HOITN_volume = pfs;
        pNewInode->HOITN_pcLink = LW_NULL;
        return pNewInode;
    }
    else if (S_ISLNK((*ppDnodeList)->HOITFD_file_type)) {   /* 链接文件 */
        PHOIT_INODE_INFO pNewInode = (PHOIT_INODE_INFO)__SHEAP_ALLOC(sizeof(HOIT_INODE_INFO));
        pNewInode->HOITN_dents = LW_NULL;
        pNewInode->HOITN_metadata = *ppDnodeList;
        pNewInode->HOITN_ino = ino;
        pNewInode->HOITN_inode_cache = pcache;
        pNewInode->HOITN_mode = (*ppDnodeList)->HOITFD_file_type;
        pNewInode->HOITN_rbtree = (PVOID)LW_NULL;
        pNewInode->HOITN_volume = pfs;
        pNewInode->HOITN_pcLink = __hoit_get_data_after_raw_inode(pfs, pNewInode->HOITN_metadata->HOITFD_raw_info);
        return pNewInode;
    }
    else {
        PHOIT_INODE_INFO pNewInode = (PHOIT_INODE_INFO)__SHEAP_ALLOC(sizeof(HOIT_INODE_INFO));
        pNewInode->HOITN_rbtree = hoitInitFragTree(pfs);
        PHOIT_FULL_DNODE pTempDnode = *ppDnodeList;
        PHOIT_FULL_DNODE pTempNext = LW_NULL;
        while (pTempDnode) {
            /*红黑树*/
            PHOIT_FRAG_TREE_NODE pTreeNode = newHoitFragTreeNode(pTempDnode, pTempDnode->HOITFD_length, pTempDnode->HOITFD_offset, pTempDnode->HOITFD_offset);
            hoitFragTreeInsertNode(pNewInode->HOITN_rbtree, pTreeNode);
            pTempNext = pTempDnode->HOITFD_next;
            pTempDnode->HOITFD_next = LW_NULL;
            pTempDnode = pTempNext;
        }
        hoitFragTreeOverlayFixUp(pNewInode->HOITN_rbtree);
        pNewInode->HOITN_dents = LW_NULL;
        pNewInode->HOITN_ino = ino;
        pNewInode->HOITN_inode_cache = pcache;
        pNewInode->HOITN_metadata = LW_NULL;
        pNewInode->HOITN_mode = (*ppDnodeList)->HOITFD_file_type;
        pNewInode->HOITN_volume = pfs;
        pNewInode->HOITN_pcLink = LW_NULL;
        __hoit_new_write_buffer(pNewInode); /* 07-18 增加初始化WriteBuffer By HZS */

        PHOIT_FRAG_TREE_LIST_HEADER pFTlistHeader = hoitFragTreeCollectRange(pNewInode->HOITN_rbtree, 0, INT_MAX);
        PHOIT_FRAG_TREE_LIST_NODE pFTlistNode = pFTlistHeader->pFTlistHeader->pFTlistNext;
        size_t HOITN_stSize = 0;
        while (pFTlistNode)
        {
           HOITN_stSize = __MAX(HOITN_stSize, pFTlistNode->pFTn->pFDnode->HOITFD_offset + pFTlistNode->pFTn->pFDnode->HOITFD_length);
           pFTlistNode = pFTlistNode->pFTlistNext;
        }

        pNewInode->HOITN_stSize = HOITN_stSize;
        return pNewInode;
    }

    //************************************ END  ************************************
    return LW_NULL;
}

/*********************************************************************************************************
** 函数名称: __hoit_get_inode_cache
** 功能描述: 根据inode number，返回inode_cache，没有就返回NULL
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_CACHE __hoit_get_inode_cache(PHOIT_VOLUME pfs, UINT ino) {
    if (pfs == LW_NULL) {
        printk("Error in hoit_get_inode_cache\n");
        return LW_NULL;
    }

    PHOIT_INODE_CACHE pcache = pfs->HOITFS_cache_list;

    while (pcache && pcache->HOITC_ino != ino) {
        pcache = pcache->HOITC_next;
    }
    
    return pcache;
}

/*********************************************************************************************************
** 函数名称: __hoit_add_dirent
** 功能描述: 给目录文件中添加一个dirent（涉及nhash）
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_add_dirent(PHOIT_INODE_INFO  pFatherInode,
    PHOIT_FULL_DIRENT pSonDirent, UINT needLog)
{
    
    UINT totlen = sizeof(HOIT_RAW_DIRENT) + lib_strlen(pSonDirent->HOITFD_file_name);
    PCHAR pWriteBuf = (PCHAR)__SHEAP_ALLOC(totlen);
    PHOIT_RAW_DIRENT    pRawDirent = (PHOIT_RAW_DIRENT)pWriteBuf;
    PHOIT_RAW_INFO      pRawInfo = (PHOIT_RAW_INFO)__SHEAP_ALLOC(sizeof(HOIT_RAW_INFO));

    if (pWriteBuf == LW_NULL || pRawInfo == LW_NULL || pFatherInode == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return;
    }
    PHOIT_VOLUME pfs = pFatherInode->HOITN_volume;

    lib_bzero(pWriteBuf, totlen);
    lib_bzero(pRawInfo, sizeof(HOIT_RAW_INFO));

    pRawDirent->file_type = pSonDirent->HOITFD_file_type;
    pRawDirent->ino = pSonDirent->HOITFD_ino;
    pRawDirent->magic_num = HOIT_MAGIC_NUM;
    pRawDirent->pino = pSonDirent->HOITFD_pino;
    pRawDirent->totlen = sizeof(HOIT_RAW_DIRENT) + lib_strlen(pSonDirent->HOITFD_file_name);
    pRawDirent->flag = HOIT_FLAG_NOT_OBSOLETE | HOIT_FLAG_TYPE_DIRENT;
    pRawDirent->version = pfs->HOITFS_highest_version++;

    PCHAR pFileName = pWriteBuf + sizeof(HOIT_RAW_DIRENT);
    lib_memcpy(pFileName, pSonDirent->HOITFD_file_name, lib_strlen(pSonDirent->HOITFD_file_name));

    UINT phys_addr = 0;

    pRawDirent->crc = 0;
    pRawDirent->crc = crc32_le(pWriteBuf, totlen);
    __hoit_write_flash(pfs, (PVOID)pWriteBuf, totlen, &phys_addr, needLog);

    pRawInfo->phys_addr = phys_addr;
    pRawInfo->totlen = pRawDirent->totlen;
    pRawInfo->next_logic = LW_NULL;
    pRawInfo->next_phys = LW_NULL;
    pRawInfo->is_obsolete = HOIT_FLAG_NOT_OBSOLETE;

    PHOIT_INODE_CACHE pInodeCache = __hoit_get_inode_cache(pfs, pFatherInode->HOITN_ino);

    __hoit_add_to_inode_cache(pInodeCache, pRawInfo);
    __hoit_add_raw_info_to_sector(pfs->HOITFS_now_sector, pRawInfo);
    pSonDirent->HOITFD_raw_info = pRawInfo;
    if (__hoit_add_to_dents(&(pFatherInode->HOITN_dents), pSonDirent)) {
        __SHEAP_FREE(pSonDirent->HOITFD_file_name);
        __SHEAP_FREE(pSonDirent);
        __SHEAP_FREE(pWriteBuf);
        return;
    }

    PHOIT_INODE_CACHE pSonInodeCache = __hoit_get_inode_cache(pfs, pSonDirent->HOITFD_ino);
    pSonInodeCache->HOITC_nlink++;

    __SHEAP_FREE(pWriteBuf);
}
/*********************************************************************************************************
** 函数名称: __hoit_alloc_ino
** 功能描述: 向文件系统申请一个新的inode number
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT __hoit_alloc_ino(PHOIT_VOLUME pfs) {
    if (pfs == LW_NULL) {
        printk("Error in hoit_alloc_ino\n");
        return HOIT_ERROR;
    }
    return pfs->HOITFS_highest_ino++;
}
/*********************************************************************************************************
** 函数名称: __hoit_read_flash
** 功能描述: 读取物理设备
** 输　入  :
** 输　出  : !=0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_read_flash(PHOIT_VOLUME pfs, UINT phys_addr, PVOID pdata, UINT length) {
    hoitReadFromCache(pfs->HOITFS_cacheHdr, phys_addr, pdata, length);
}

/*********************************************************************************************************
** 函数名称: __hoit_write_flash
** 功能描述: 写入物理设备，不能自己选物理地址
** 输　入  :
** 输　出  : !=0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_write_flash(PHOIT_VOLUME pfs, PVOID pdata, UINT length, UINT* phys_addr, UINT needLog) {
    /*
    write_nor(pfs->HOITFS_now_sector->HOITS_offset + pfs->HOITFS_now_sector->HOITS_addr, (PCHAR)(pdata), length, WRITE_KEEP);
    if (phys_addr != LW_NULL) {
        *phys_addr = pfs->HOITFS_now_sector->HOITS_offset + pfs->HOITFS_now_sector->HOITS_addr;
    }
    pfs->HOITFS_now_sector->HOITS_offset += length;
    */
    if(needLog)
        hoitLogAppend(pfs, pdata, length);
    UINT temp_addr = hoitWriteToCache(pfs->HOITFS_cacheHdr, pdata, length);
    if(phys_addr){
        *phys_addr = temp_addr;
    }
    if (temp_addr == PX_ERROR) {
        printf("Error in write flash\n");
        return PX_ERROR;
    }

    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_write_flash_thru
** 功能描述: 写入物理设备，可以自己选物理地址
** 输　入  :
** 输　出  : <0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_write_flash_thru(PHOIT_VOLUME pfs, PVOID pdata, UINT length, UINT phys_addr) {
    //write_nor(phys_addr, (PCHAR)(pdata), length, WRITE_KEEP);
    hoitWriteThroughCache(pfs->HOITFS_cacheHdr, phys_addr, pdata, length);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_add_to_inode_cache
** 功能描述: 将一个raw_info加入到inode_cache中
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_add_to_inode_cache(PHOIT_INODE_CACHE pInodeCache, PHOIT_RAW_INFO pRawInfo) {
    if (pInodeCache == LW_NULL || pRawInfo == LW_NULL) {
        printk("Error in %s\n", __func__);
        return HOIT_ERROR;
    }
    pRawInfo->next_logic = pInodeCache->HOITC_nodes;
    pInodeCache->HOITC_nodes = pRawInfo;
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_add_to_cache_list
** 功能描述: 将一个inode cache加入到cache_list中
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_add_to_cache_list(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeCache) {
    if (pfs == LW_NULL || pInodeCache == LW_NULL) {
        printk("Error in hoit_add_to_inode_cache\n");
        return HOIT_ERROR;
    }
    //pInodeCache->HOITC_nodes = LW_NULL;
    pInodeCache->HOITC_next = pfs->HOITFS_cache_list;
    pfs->HOITFS_cache_list = pInodeCache;
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_add_to_dents
** 功能描述: 将一个full_dirent加入到父目录文件夹的dents中
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_add_to_dents(PHOIT_FULL_DIRENT* ppFatherDents, PHOIT_FULL_DIRENT pFullDirent) {
    if (ppFatherDents == LW_NULL || pFullDirent == LW_NULL) {
        printk("Error in hoit_add_to_dents\n");
        return HOIT_ERROR;
    }

    PHOIT_FULL_DIRENT pDirent = *ppFatherDents;
    while (pDirent) {
        if ((pDirent->HOITFD_ino == pFullDirent->HOITFD_ino) && (lib_strcmp(pDirent->HOITFD_file_name, pFullDirent->HOITFD_file_name) == 0)) {
            if (pDirent->HOITFD_version > pFullDirent->HOITFD_version) {
                return HOIT_ERROR;
            }
            else {
                __hoit_del_full_dirent(ppFatherDents, pDirent);
                break;
            }
        }
        pDirent = pDirent->HOITFD_next;
    }

    pFullDirent->HOITFD_next = *ppFatherDents;
    *ppFatherDents = pFullDirent;
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_search_in_dents
** 功能描述: 在一个父dents链表中用二分法搜索一个指定ino的文件，返回FullDirent
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_FULL_DIRENT __hoit_search_in_dents(PHOIT_INODE_INFO pInodeFather, UINT ino, PCHAR pName) {
    if (pInodeFather == LW_NULL) {
        printk("Error in hoit_search_in_dents\n");
        return LW_NULL;
    }

    PCHAR pcFileName = lib_rindex(pName, PX_DIVIDER);
    if (pcFileName) {
        pcFileName++;
    }
    else {
        pcFileName = pName;
    }

    UINT nHash = __hoit_name_hash(pcFileName);
    PHOIT_FULL_DIRENT pDirent = pInodeFather->HOITN_dents;
    while (pDirent) {
        if (pDirent->HOITFD_ino == ino) {
            UINT nTempHash = __hoit_name_hash(pDirent->HOITFD_file_name);
            if (nTempHash == nHash) {
                break;
            }
        }
        pDirent = pDirent->HOITFD_next;
    }
    if (pDirent) 
        return pDirent;
    return LW_NULL;
}
/*********************************************************************************************************
** 函数名称: __hoit_del_raw_info
** 功能描述: 将一个RawInfo从对应的InodeCache链表中删除，但不free对应内存空间
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_del_raw_info(PHOIT_INODE_CACHE pInodeCache, PHOIT_RAW_INFO pRawInfo) {
    if (pInodeCache == LW_NULL || pRawInfo == LW_NULL) {
        printk("Error in hoit_del_raw_info\n");
        return HOIT_ERROR;
    }
    if (pInodeCache->HOITC_nodes == pRawInfo) {
        pInodeCache->HOITC_nodes = pRawInfo->next_logic;
        return 0;
    }
    else {
        PHOIT_RAW_INFO pRawTemp = pInodeCache->HOITC_nodes;
        while (pRawTemp && pRawTemp->next_logic != pRawInfo) {
            pRawTemp = pRawTemp->next_logic;
        }
        if (pRawTemp) {
            pRawTemp->next_logic = pRawInfo->next_logic;
            return 0;
        }
        else {
            return HOIT_ERROR;
        }
    }
    
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_del_raw_data
** 功能描述: 将一个RawDirent或RawInode在对应的磁盘中标记为过期,并不释放RawInfo内存
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_del_raw_data(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pRawInfo) {
    if (pRawInfo == LW_NULL) {
        printk("Error in hoit_del_raw_data\n");
        return HOIT_ERROR;
    }

    PCHAR buf = (PCHAR)__SHEAP_ALLOC(pRawInfo->totlen);
    lib_bzero(buf, pRawInfo->totlen);

    __hoit_read_flash(pfs, pRawInfo->phys_addr, buf, pRawInfo->totlen);

    PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)buf;
    crc32_check(pRawHeader);

    if (pRawHeader->magic_num != HOIT_MAGIC_NUM || (pRawHeader->flag & HOIT_FLAG_NOT_OBSOLETE) == 0) {
        printk("Error in hoit_del_raw_data\n");
        return HOIT_ERROR;
    }
    //!pRawHeader->flag &= (~HOIT_FLAG_NOT_OBSOLETE);      //将obsolete标志变为0，代表过期
    __hoit_mark_obsolete(pfs, pRawHeader, pRawInfo);
    // /* 将obsolete标志位清0后写回原地址 */
    // pRawHeader->crc = 0;
    // pRawHeader->crc = crc32_le(pRawHeader, pRawInfo->totlen);
    // __hoit_write_flash_thru(pfs, (PVOID)pRawHeader, pRawInfo->totlen, pRawInfo->phys_addr);

    __SHEAP_FREE(buf);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_del_full_dirent
** 功能描述: 将一个FullDirent从对应的InodeInfo的dents链表中删除，但不free对应内存空间
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_del_full_dirent(PHOIT_FULL_DIRENT* ppFatherDents, PHOIT_FULL_DIRENT pFullDirent) {
    if (ppFatherDents == LW_NULL || pFullDirent == LW_NULL) {
        printk("Error in hoit_del_full_dirent\n");
        return HOIT_ERROR;
    }
    if (*ppFatherDents == pFullDirent) {
        *ppFatherDents = pFullDirent->HOITFD_next;
        return 0;
    }
    else {
        PHOIT_FULL_DIRENT pFullTemp = *ppFatherDents;
        while (pFullTemp && pFullTemp->HOITFD_next != pFullDirent) {
            pFullTemp = pFullTemp->HOITFD_next;
        }
        if (pFullTemp) {
            pFullTemp->HOITFD_next = pFullDirent->HOITFD_next;
            return 0;
        }
        else {
            return HOIT_ERROR;
        }
    }
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_del_inode_cache
** 功能描述: 将一个InodeCache从挂载的文件系统中删除，但不free对应内存空间
** 输　入  :
** 输　出  : !=0 代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_del_inode_cache(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeCache) {
    PHOIT_INODE_CACHE pTemp = pfs->HOITFS_cache_list;
    if (pTemp == pInodeCache) {
        pfs->HOITFS_cache_list = pInodeCache->HOITC_next;
        return 0;
    }
    while (pTemp && pTemp->HOITC_next != pInodeCache) {
        pTemp = pTemp->HOITC_next;
    }
    if (pTemp == LW_NULL) return HOIT_ERROR;
    pTemp->HOITC_next = pInodeCache->HOITC_next;
    return 0;
}
/*********************************************************************************************************
** 函数名称: __hoit_get_inode_nodes
** 功能描述: 如果是普通文件，则返回一个FullDnode的链表，还未组成红黑树的
**           如果是目录文件，则返回一个FullDirent的链表，也是还未组成正式结构的
** 输　入  :
** 输　出  : !=0 代表出错, pDirentList是返回的FullDirent链表, pDnodeList是返回的FullDnode链表
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UINT8 __hoit_get_inode_nodes(PHOIT_VOLUME pfs, PHOIT_INODE_CACHE pInodeInfo, PHOIT_FULL_DIRENT* ppDirentList, PHOIT_FULL_DNODE* ppDnodeList) {
    PHOIT_RAW_INFO pRawInfo = pInodeInfo->HOITC_nodes;
    while (pRawInfo) {
        PCHAR pBuf = (PCHAR)__SHEAP_ALLOC(pRawInfo->totlen);
        
        __hoit_read_flash(pfs, pRawInfo->phys_addr, pBuf, pRawInfo->totlen);
        PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)pBuf;
        crc32_check(pRawHeader);

        if (!__HOIT_IS_OBSOLETE(pRawHeader)) {
            if (__HOIT_IS_TYPE_DIRENT(pRawHeader)) {
                PHOIT_RAW_DIRENT pRawDirent = (PHOIT_RAW_DIRENT)pRawHeader;
                PHOIT_FULL_DIRENT pFullDirent = (PHOIT_FULL_DIRENT)__SHEAP_ALLOC(sizeof(HOIT_FULL_DIRENT));

                pFullDirent->HOITFD_file_type = pRawDirent->file_type;
                pFullDirent->HOITFD_ino = pRawDirent->ino;
                pFullDirent->HOITFD_pino = pRawDirent->pino;
                pFullDirent->HOITFD_raw_info = pRawInfo;
                pFullDirent->HOITFD_version = pRawDirent->version;
                pFullDirent->HOITFD_file_name = (PCHAR)__SHEAP_ALLOC(pRawInfo->totlen - sizeof(HOIT_RAW_DIRENT) + 1);   // 这里要加1添加'\0', 因为在flash中存储的文件名'
                lib_bzero(pFullDirent->HOITFD_file_name, pRawInfo->totlen - sizeof(HOIT_RAW_DIRENT) + 1);

                PCHAR pRawName = ((PCHAR)pRawDirent) + sizeof(HOIT_RAW_DIRENT);
                lib_memcpy(pFullDirent->HOITFD_file_name, pRawName, pRawInfo->totlen - sizeof(HOIT_RAW_DIRENT));
                if (__hoit_add_to_dents(ppDirentList, pFullDirent)) {
                    __SHEAP_FREE(pFullDirent->HOITFD_file_name);
                    __SHEAP_FREE(pFullDirent);
                }
            }
            else {
                PHOIT_RAW_INODE pRawInode = (PHOIT_RAW_INODE)pRawHeader;
                PHOIT_FULL_DNODE pFullDnode = (PHOIT_FULL_DNODE)__SHEAP_ALLOC(sizeof(HOIT_FULL_DNODE));
                pFullDnode->HOITFD_length = pRawInfo->totlen - sizeof(HOIT_RAW_INODE);
                pFullDnode->HOITFD_offset = pRawInode->offset;
                pFullDnode->HOITFD_raw_info = pRawInfo;
                pFullDnode->HOITFD_next = *ppDnodeList;
                pFullDnode->HOITFD_file_type = pRawInode->file_type;
                pFullDnode->HOITFD_version = pRawInode->version;
                *ppDnodeList = pFullDnode;
            }
        }
        __SHEAP_FREE(pBuf);
        pRawInfo = pRawInfo->next_logic;
    }
    return ERROR_NONE;
}
/*********************************************************************************************************
** 函数名称: __hoit_add_to_sector_list
** 功能描述: 添加一个sector到volume
** 输　入  :
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_add_to_sector_list(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector) {
    pErasableSector->HOITS_next = pfs->HOITFS_erasableSectorList;
    pfs->HOITFS_erasableSectorList = pErasableSector;
    //!TODO: mount初始化链表，封装链表转移函数
    __hoit_fix_up_sector_list(pfs, pErasableSector);
    return LW_TRUE;
}
/*********************************************************************************************************
** 函数名称: __hoit_scan_single_sector
** 功能描述: 扫描一个擦除块
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_scan_single_sector(ScanThreadAttr* pThreadAttr) {
    PHOIT_VOLUME pfs    = pThreadAttr->pfs;
    UINT8 sector_no     = pThreadAttr->sector_no;

    UINT                    uiSectorSize;         
    UINT                    uiSectorOffset;
    UINT                    uiFreeSize;
    UINT                    uiUsedSize;
    PHOIT_ERASABLE_SECTOR   pErasableSector;
    UINT                    uiSectorNum;


    uiSectorSize            = hoitGetSectorSize(sector_no);
    uiSectorOffset          = hoitGetSectorOffset(sector_no);
    uiFreeSize              = uiSectorSize;
    uiUsedSize              = 0;

    /* 先创建sector结构体 */
    pErasableSector = (PHOIT_ERASABLE_SECTOR)__SHEAP_ALLOC(sizeof(HOIT_ERASABLE_SECTOR));
    pErasableSector->HOITS_bno              = sector_no;
    pErasableSector->HOITS_length           = uiSectorSize;
    pErasableSector->HOITS_addr             = uiSectorOffset;
    pErasableSector->HOITS_offset           = 0;
    pErasableSector->HOITS_pRawInfoFirst    = LW_NULL;
    pErasableSector->HOITS_pRawInfoLast     = LW_NULL;
    pErasableSector->HOITS_pRawInfoCurGC    = LW_NULL;
    pErasableSector->HOITS_pRawInfoPrevGC   = LW_NULL;
    pErasableSector->HOITS_pRawInfoLastGC   = LW_NULL;
    pErasableSector->HOITS_tBornAge         = API_TimeGet(); 
    
    LW_SPIN_INIT(&pErasableSector->HOITS_lock);         /* 初始化SPIN LOCK */

    // ugly now
    if(pfs->HOITFS_now_sector == LW_NULL){
        pfs->HOITFS_now_sector = pErasableSector;
    }

    /* 再整个块进行扫描 */
    PCHAR pReadBuf = (PCHAR)__SHEAP_ALLOC(uiSectorSize);
    lib_bzero(pReadBuf, uiSectorSize);

    __hoit_read_flash(pfs, uiSectorOffset, pReadBuf, uiSectorSize);
    crc32_check(pReadBuf);

    /* 2021-07-10 Modified by HZS */
    size_t pageAmount       = uiSectorSize / (HOIT_FILTER_EBS_ENTRY_SIZE + HOIT_FILTER_PAGE_SIZE);
    size_t EBSStartAddr     = HOIT_FILTER_PAGE_SIZE * pageAmount;

    PHOIT_EBS_ENTRY pEntry  = (PHOIT_EBS_ENTRY)(pReadBuf + EBSStartAddr);
    UINT32 uPageIndex       = 0;

    while ((PCHAR)pEntry < pReadBuf + uiSectorSize) {

        if (pEntry->HOIT_EBS_ENTRY_obsolete == 0) {/* 0是过期 */
            pEntry      += 1;
            uPageIndex  += 1;
            continue;
        }

        PCHAR pNow = pReadBuf + uPageIndex * HOIT_FILTER_PAGE_SIZE;    /* 拿到Entry对应的Page首地址 */

        PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)pNow;
        if(sector_no == 0 
        && pRawHeader->ino != -1
        && pRawHeader->magic_num == HOIT_MAGIC_NUM){
            printf("offs: %d ino: %d\n", uiSectorOffset + (pNow - pReadBuf), pRawHeader->ino);
        }
        if(pRawHeader->magic_num == HOIT_MAGIC_NUM){
            uiUsedSize += pRawHeader->totlen;
            uiFreeSize -= pRawHeader->totlen;
        }
        if (pRawHeader->magic_num == HOIT_MAGIC_NUM && !__HOIT_IS_OBSOLETE(pRawHeader)) {
            /* //TODO:后面这里还需添加CRC校验 */
            PHOIT_RAW_INFO pRawInfo = LW_NULL;
            if (__HOIT_IS_TYPE_INODE(pRawHeader)) {
                PHOIT_RAW_INODE     pRawInode   = (PHOIT_RAW_INODE)pNow;
                PHOIT_INODE_CACHE   pInodeCache = __hoit_get_inode_cache(pfs, pRawInode->ino);
                if (pInodeCache == LW_NULL) {                  /* 创建一个Inode Cache */
                    pInodeCache = (PHOIT_INODE_CACHE)__SHEAP_ALLOC(sizeof(HOIT_INODE_CACHE));
                    if (pInodeCache == LW_NULL) {
                        _ErrorHandle(ENOMEM);
                        return  (PX_ERROR);
                    }
                    pInodeCache->HOITC_ino = pRawInode->ino;
                    pInodeCache->HOITC_nlink = 0;
                    pInodeCache->HOITC_nodes = LW_NULL;
                    __hoit_add_to_cache_list(pfs, pInodeCache);
                }
                pRawInfo                    = (PHOIT_RAW_INFO)__SHEAP_ALLOC(sizeof(HOIT_RAW_INFO));
                pRawInfo->phys_addr         = uiSectorOffset + (pNow - pReadBuf);
                pRawInfo->totlen            = pRawInode->totlen;
                pRawInfo->is_obsolete       = HOIT_FLAG_NOT_OBSOLETE;
                pRawInfo->next_logic = LW_NULL;
                pRawInfo->next_phys = LW_NULL;

                __hoit_add_to_inode_cache(pInodeCache, pRawInfo);
                __hoit_add_raw_info_to_sector(pErasableSector, pRawInfo);

                if (pRawInode->ino == HOIT_ROOT_DIR_INO) {     /* 如果扫描到的是根目录的唯一的RawInode */
                    PHOIT_FULL_DNODE pFullDnode = __hoit_bulid_full_dnode(pfs, pRawInfo);
                    PHOIT_INODE_INFO pInodeInfo = (PHOIT_INODE_INFO)__SHEAP_ALLOC(sizeof(HOIT_INODE_INFO));
                    pInodeInfo->HOITN_dents = LW_NULL;
                    pInodeInfo->HOITN_gid = pfs->HOITFS_gid;
                    pInodeInfo->HOITN_ino = HOIT_ROOT_DIR_INO;
                    pInodeInfo->HOITN_inode_cache = pInodeCache;
                    pInodeInfo->HOITN_metadata = pFullDnode;
                    pInodeInfo->HOITN_mode = S_IFDIR;
                    pInodeInfo->HOITN_rbtree = LW_NULL;
                    pInodeInfo->HOITN_uid = pfs->HOITFS_uid;
                    pInodeInfo->HOITN_volume = pfs;

                    pfs->HOITFS_pRootDir = pInodeInfo;

                    if (pfs->HOITFS_pTempRootDirent != LW_NULL) {
                        pInodeInfo->HOITN_dents = pfs->HOITFS_pTempRootDirent;
                        pfs->HOITFS_pTempRootDirent = LW_NULL;
                    }
                }
            }
            else if (__HOIT_IS_TYPE_DIRENT(pRawHeader)) {
                PHOIT_RAW_DIRENT pRawDirent = (PHOIT_RAW_DIRENT)pNow;
                PHOIT_INODE_CACHE pInodeCache = __hoit_get_inode_cache(pfs, pRawDirent->pino);  /* 这里的pino才是目录文件自己的ino */
                if (pInodeCache == LW_NULL) {
                    pInodeCache = (PHOIT_INODE_CACHE)__SHEAP_ALLOC(sizeof(HOIT_INODE_CACHE));
                    if (pInodeCache == LW_NULL) {
                        _ErrorHandle(ENOMEM);
                        return  (PX_ERROR);
                    }
                    pInodeCache->HOITC_ino = pRawDirent->pino;  /* 这里的pino才是目录文件自己的ino */
                    pInodeCache->HOITC_nlink = 0;
                    pInodeCache->HOITC_nodes = LW_NULL;
                    __hoit_add_to_cache_list(pfs, pInodeCache);
                }
                pRawInfo                = (PHOIT_RAW_INFO)__SHEAP_ALLOC(sizeof(HOIT_RAW_INFO));
                pRawInfo->phys_addr     = uiSectorOffset + (pNow - pReadBuf);
                pRawInfo->totlen        = pRawDirent->totlen;
                pRawInfo->is_obsolete   = HOIT_FLAG_NOT_OBSOLETE;
                pRawInfo->next_logic = LW_NULL;
                pRawInfo->next_phys = LW_NULL;

                __hoit_add_to_inode_cache(pInodeCache, pRawInfo);
                __hoit_add_raw_info_to_sector(pErasableSector, pRawInfo);

                if (pRawDirent->pino == HOIT_ROOT_DIR_INO) {    /* 如果扫描到的是根目录的目录项 */
                    PHOIT_FULL_DIRENT pFullDirent = __hoit_bulid_full_dirent(pfs, pRawInfo);
                    INT addFail = 0;
                    if (pfs->HOITFS_pRootDir == LW_NULL) {      /* 如果根目录的唯一RawInode还未扫描到 */
                        addFail = __hoit_add_to_dents(&(pfs->HOITFS_pTempRootDirent), pFullDirent);
                    }
                    else {
                        addFail = __hoit_add_to_dents(&(pfs->HOITFS_pRootDir->HOITN_dents), pFullDirent);
                    }

                    if (addFail) {
                        __SHEAP_FREE(pFullDirent->HOITFD_file_name);
                        __SHEAP_FREE(pFullDirent);
                    }
                }
            }
            
            //else if (__HOIT_IS_TYPE_LOG(pRawHeader)) {
            //    *hasLog = 1;
            //    PHOIT_RAW_LOG pRawLog = (PHOIT_RAW_LOG)pRawHeader;
            //    if (pRawLog->uiLogFirstAddr != -1) {    /* LOG HDR */
            //        /* hoitLogOpen(pfs, pRawLog); */
            //        *ppRawLogHdr = (PHOIT_RAW_LOG)lib_malloc(sizeof(HOIT_RAW_LOG));
            //        lib_memcpy(*ppRawLogHdr, pRawLog, sizeof(HOIT_RAW_LOG));
            //    }
            //}
            
            if (pRawHeader->ino > pfs->HOITFS_highest_ino)
                pfs->HOITFS_highest_ino = pRawHeader->ino;
            if (pRawHeader->version > pfs->HOITFS_highest_version)
                pfs->HOITFS_highest_version = pRawHeader->version;
            
            //!初始化pErasableSector的更多信息, Added by PYQ 2021-04-26
            if (pRawInfo != LW_NULL){

            } 
            //pNow += __HOIT_MIN_4_TIMES(pRawHeader->totlen);
            //pNow += pRawHeader->totlen;
            pEntry += 1;
            uPageIndex += 1;
        }
        else {
            //pNow += 4;   /* 每次移动4字节 */
            //pNow += 1;   /* 每次移动1字节 */
            pEntry += 1;
            uPageIndex += 1;
        }
    }
    pErasableSector->HOITS_uiUsedSize = uiUsedSize;
    pErasableSector->HOITS_uiFreeSize = uiFreeSize;
    pErasableSector->HOITS_offset = uiUsedSize;     /*! Modified By PYQ 更新写入偏移 */

    __hoit_add_to_sector_list(pfs, pErasableSector);

    __SHEAP_FREE(pReadBuf);
    __SHEAP_FREE(pThreadAttr);
    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_new_inode_info
** 功能描述: hoitfs 创建一个新的文件，在Flash里写入一个简单的RawInode
** 输　入  : 
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_INFO __hoit_new_inode_info(PHOIT_VOLUME pfs, mode_t mode, CPCHAR pcLink) {
    PHOIT_RAW_INODE     pRawInode = LW_NULL;
    UINT32 totlen = sizeof(HOIT_RAW_INODE);
    CHAR cFullPathName[MAX_FILENAME_LENGTH];

    if (S_ISLNK(mode)) {    /* 链接文件 */
        if (_PathCat(_PathGetDef(), pcLink, cFullPathName) != ERROR_NONE) { /*  获得从根目录开始的路径      */
            _ErrorHandle(ENAMETOOLONG);
            return  LW_NULL;
        }

        pRawInode = (PHOIT_RAW_INODE)__SHEAP_ALLOC(sizeof(HOIT_RAW_INODE) + lib_strlen(cFullPathName));
        totlen = sizeof(HOIT_RAW_INODE) + lib_strlen(cFullPathName);
    }
    else {
        pRawInode = (PHOIT_RAW_INODE)__SHEAP_ALLOC(sizeof(HOIT_RAW_INODE));
    }
    PHOIT_RAW_INFO     pRawInfo = (PHOIT_RAW_INFO)__SHEAP_ALLOC(sizeof(HOIT_RAW_INFO));
    PHOIT_INODE_CACHE   pInodeCache = (PHOIT_INODE_CACHE)__SHEAP_ALLOC(sizeof(HOIT_INODE_CACHE));


    if (pRawInfo == LW_NULL || pRawInode == LW_NULL || pInodeCache == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (LW_NULL);
    }
    
    lib_bzero(pRawInode, totlen);
    lib_bzero(pRawInfo, sizeof(HOIT_RAW_INFO));
    lib_bzero(pInodeCache, sizeof(HOIT_INODE_CACHE));


    pRawInode->file_type = mode;
    pRawInode->ino = __hoit_alloc_ino(pfs);
    pRawInode->magic_num = HOIT_MAGIC_NUM;
    pRawInode->totlen = totlen;
    pRawInode->flag = HOIT_FLAG_TYPE_INODE | HOIT_FLAG_NOT_OBSOLETE;
    pRawInode->offset = 0;
    pRawInode->version = pfs->HOITFS_highest_version++;
    pRawInode->crc = 0;
    

    UINT phys_addr;
    if (S_ISLNK(mode)) {
        PCHAR pLink = ((PCHAR)pRawInode) + sizeof(HOIT_RAW_INODE);
        lib_memcpy(pLink, cFullPathName, lib_strlen(cFullPathName));  /* 链接文件 */
        pRawInode->crc = crc32_le(pRawInode, totlen);
        __hoit_write_flash(pfs, (PVOID)pRawInode, totlen, &phys_addr, 1);
    }
    else {
        pRawInode->crc = crc32_le(pRawInode, sizeof(HOIT_RAW_INODE));
        __hoit_write_flash(pfs, (PVOID)pRawInode, sizeof(HOIT_RAW_INODE), &phys_addr, 1);
    }
    
    


    pRawInfo->phys_addr = phys_addr;
    pRawInfo->totlen = totlen;
    pRawInfo->is_obsolete = HOIT_FLAG_NOT_OBSOLETE;

    pInodeCache->HOITC_ino = pRawInode->ino;
    pInodeCache->HOITC_nlink = 0;
    pInodeCache->HOITC_nodes = LW_NULL;
    __hoit_add_to_inode_cache(pInodeCache, pRawInfo);
    __hoit_add_raw_info_to_sector(pfs->HOITFS_now_sector, pRawInfo);
    __hoit_add_to_cache_list(pfs, pInodeCache);

    /*
    *   已经将新文件配置成了一个已经存在的文件，现在只需调用get_full_file即可
    */
    __SHEAP_FREE(pRawInode);

    return  __hoit_get_full_file(pfs, pInodeCache->HOITC_ino);
    
}
/*********************************************************************************************************
** 函数名称: __hoit_get_nlink
** 功能描述: hoitfs 按照树形结构递归地去统计每个文件的链接数
** 输　入  :
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoit_get_nlink(PHOIT_INODE_INFO pInodeInfo){
    if (!S_ISDIR(pInodeInfo->HOITN_mode)) return;
    PHOIT_VOLUME pfs = pInodeInfo->HOITN_volume;

    PHOIT_FULL_DIRENT pTempDirent = pInodeInfo->HOITN_dents;
    for (; pTempDirent != LW_NULL; pTempDirent = pTempDirent->HOITFD_next) {
        PHOIT_INODE_CACHE pInodeCache = __hoit_get_inode_cache(pfs, pTempDirent->HOITFD_ino);
        if (pInodeCache == LW_NULL) {
            continue;
        }
        pInodeCache->HOITC_nlink++;
        if (S_ISDIR(pTempDirent->HOITFD_file_type)) {   /* 子文件是目录文件, 则递归下去 */
            PHOIT_INODE_INFO pTempInode = __hoit_get_full_file(pfs, pInodeCache->HOITC_ino);
            __hoit_get_nlink(pTempInode);
            __hoit_close(pTempInode, 0);
        }
    }
}

/*********************************************************************************************************
** 函数名称: __hoit_get_data_after_raw_inode
** 功能描述: hoitfs 得到Flash上紧跟着raw_inode的后面的数据, 注意数据末尾会添加'\0'
** 输　入  :
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PCHAR __hoit_get_data_after_raw_inode(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pInodeInfo) {
    if (pInodeInfo == LW_NULL) {
        return LW_NULL;
    }

    INT iDataLen = pInodeInfo->totlen - sizeof(HOIT_RAW_INODE);
    PCHAR pTempBuf = (PCHAR)__SHEAP_ALLOC(pInodeInfo->totlen);

    __hoit_read_flash(pfs, pInodeInfo->phys_addr, pTempBuf, pInodeInfo->totlen);
    crc32_check(pTempBuf);

    PCHAR pData = (PCHAR)__SHEAP_ALLOC(iDataLen + 1);
    lib_bzero(pData, iDataLen + 1);
    PCHAR pTempData = pTempBuf + sizeof(HOIT_RAW_INODE);
    lib_memcpy(pData, pTempData, iDataLen);
    __SHEAP_FREE(pTempBuf);
    return pData;
}

/*********************************************************************************************************
** 函数名称: __hoit_add_raw_info_to_sector
** 功能描述: hoitfs 将rawInfo加入到块的raw_info链表上
** 输　入  :
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoit_add_raw_info_to_sector(PHOIT_ERASABLE_SECTOR pSector, PHOIT_RAW_INFO pRawInfo) {
    INTREG iregInterLevel;
    //API_SpinLockQuick(&pSector->HOITS_lock, &iregInterLevel);
    //TODO: Sector的HOITS_uiObsoleteEntityCount和HOITS_uiAvailableEntityCount初始化位置？
    /*                      
                          |--------------------------------|
                          |     Sector                     |      Sector
                          |    /     \                     |    /       \
        InodeCache----RawInfo(ino)-----RawInfo(ino2)-----RawInfo(ino)-----RawInfo(ino)
                        |                   |               |               |
                ---------------------------------------------------------------------
                   | [H][Entity]       [H][Entity]  |    [H][Entity]   [H][Entity]  |
                ------------------------------------------------------------------
    */
    pRawInfo->next_phys = LW_NULL;
    if(pRawInfo->is_obsolete){
        pSector->HOITS_uiObsoleteEntityCount++;
    }
    else {
        pSector->HOITS_uiAvailableEntityCount++;
    }

    //调用转移函数
#ifdef LIB_DEBUG
    printf("[%s]:add raw info at %p for sector %d\n", __func__, pRawInfo, pSector->HOITS_bno);
#endif // LIB_DEBUG
    if (pSector->HOITS_pRawInfoFirst == LW_NULL) {
        pSector->HOITS_pRawInfoFirst = pRawInfo;
        pSector->HOITS_pRawInfoLast = pRawInfo;
        //API_SpinUnlockQuick(&pSector->HOITS_lock, iregInterLevel);
        return;
    }
    else {
        if (pSector->HOITS_pRawInfoLast == LW_NULL) {
            //API_SpinUnlockQuick(&pSector->HOITS_lock, iregInterLevel);
            return;
        }
        pSector->HOITS_pRawInfoLast->next_phys = pRawInfo;
        pSector->HOITS_pRawInfoLast = pRawInfo;
    }

    //API_SpinUnlockQuick(&pSector->HOITS_lock, iregInterLevel);
}

/*********************************************************************************************************
** 函数名称: __hoit_move_home
** 功能描述: hoitfs 垃圾回收函数，将含有有效数据的rawInfo从旧sector搬到新sector上，
**          注意，这里不会修改某个数据实体的Prev指针，并且注意保存now_sector指针，
**          它会修改该指针；
** 输　入  : pSector是新sector
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
BOOL __hoit_move_home(PHOIT_VOLUME pfs, PHOIT_RAW_INFO pRawInfo) {
    //! 2021-05-16 修改 move home By PYQ，我们应该找一个新块来写
    PCHAR                   pReadBuf;
    PHOIT_RAW_LOG           pRawLog;
    pReadBuf = (PCHAR)__SHEAP_ALLOC(pRawInfo->totlen);
    /* 先读出旧数据 */
    lib_bzero(pReadBuf, pRawInfo->totlen);

    __hoit_read_flash(pfs, pRawInfo->phys_addr, pReadBuf, pRawInfo->totlen);
    crc32_check(pReadBuf);

    PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)pReadBuf;
    if (pRawHeader->magic_num != HOIT_MAGIC_NUM 
    || (pRawHeader->flag & HOIT_FLAG_NOT_OBSOLETE) == 0) {
        //printk("Error in hoit_move_home\n");
        return LW_FALSE;
    }
    //!pRawHeader->flag &= (~HOIT_FLAG_NOT_OBSOLETE);      //将obsolete标志变为0，代表过期
    __hoit_mark_obsolete(pfs, pRawHeader, pRawInfo);
    // /* 将obsolete标志位清0后写回原地址 */
    // pRawHeader->crc = 0;
    // pRawHeader->crc = crc32_le(pRawHeader, pRawInfo->totlen);
    // __hoit_write_flash_thru(pfs, (PVOID)pRawHeader, pRawInfo->totlen, pRawInfo->phys_addr);
    
    /* 将obsolete标志位恢复后写到新地址 */
    pRawHeader->flag |= HOIT_FLAG_NOT_OBSOLETE;         //将obsolete标志变为1，代表未过期
    UINT phys_addr = 0;

    //!2021-05-16 修改now_sector指针 modified by PYQ 
    pfs->HOITFS_now_sector = pfs->HOITFS_curGCSuvivorSector;
    
    pRawHeader->crc = 0;
    pRawHeader->crc = crc32_le(pRawHeader, pRawInfo->totlen);
    __hoit_write_flash(pfs, pReadBuf, pRawInfo->totlen, &phys_addr, 1);
    pRawInfo->phys_addr = phys_addr;

    if(__HOIT_IS_TYPE_LOG(pRawHeader)){                         /* 如果是LOG HDR，那么要调整logInfo里的信息 */
        pRawLog = (PHOIT_RAW_LOG)pRawHeader;
        if (pRawLog->uiLogFirstAddr != -1) {                    /* LOG HDR */
            pfs->HOITFS_logInfo->uiRawLogHdrAddr = pRawInfo->phys_addr;
        }
    }
    /* 将RawInfo从旧块搬到新块 */
    __hoit_add_raw_info_to_sector(pfs->HOITFS_now_sector, pRawInfo);
    return LW_TRUE;
}

/*********************************************************************************************************
** 函数名称: __hoit_open
** 功能描述: hoitfs 打开一个文件
** 输　入  : pfs              文件系统
**           pcName           文件名
**           ppinodeFather     当无法找到节点时保存最接近的一个,
                              但寻找到节点时保存父系节点.
                              LW_NULL 表示根
             pbRoot           是否为根节点
**           pbLast           当匹配失败时, 是否是最后一级文件匹配失败
**           ppcTail          如果存在连接文件, 指向连接文件后的路径
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_INFO  __hoit_open(PHOIT_VOLUME  pfs,
    CPCHAR       pcName,
    PHOIT_INODE_INFO* ppInodeFather,
    PHOIT_FULL_DIRENT* ppFullDirent,
    BOOL* pbRoot,
    BOOL* pbLast,
    PCHAR* ppcTail)
{
    CHAR                pcTempName[MAX_FILENAME_LENGTH];
    PCHAR               pcNext;
    PCHAR               pcNode;

    PHOIT_INODE_INFO    pinodeTemp;

    if (ppInodeFather == LW_NULL) {
        ppInodeFather = &pinodeTemp;                                      /*  临时变量                    */
    }
    *ppInodeFather = LW_NULL;
    UINT inodeFatherIno = 0;
    if (*pcName == PX_ROOT) {                                           /*  忽略根符号                  */
        lib_strlcpy(pcTempName, (pcName + 1), PATH_MAX);
    }
    else {
        lib_strlcpy(pcTempName, pcName, PATH_MAX);
    }

    if (pcTempName[0] == PX_EOS) {
        if (pbRoot) {
            *pbRoot = LW_TRUE;                                          /*  pcName 为根                 */
        }
        if (pbLast) {
            *pbLast = LW_FALSE;
        }
        return  (LW_NULL);
    }
    else {
        if (pbRoot) {
            *pbRoot = LW_FALSE;                                         /*  pcName 不为根               */
        }
    }
    PHOIT_INODE_INFO    pInode;
    PHOIT_FULL_DIRENT   pDirentTemp;

    pcNext = pcTempName;
    pInode = pfs->HOITFS_pRootDir;                               /*  从根目录开始搜索            */

    do {
        pcNode = pcNext;
        pcNext = lib_index(pcNode, PX_DIVIDER);                         /*  移动到下级目录              */
        if (pcNext) {                                                   /*  是否可以进入下一层          */
            *pcNext = PX_EOS;
            pcNext++;                                                   /*  下一层的指针                */
        }


        for (pDirentTemp = pInode->HOITN_dents;
            pDirentTemp != LW_NULL;
            pDirentTemp = pDirentTemp->HOITFD_next) {

            if (pDirentTemp == LW_NULL) {                                     /*  无法继续搜索                */
                goto    __find_error;
            }
            if (S_ISLNK(pDirentTemp->HOITFD_file_type)) {                            /*  链接文件                    */
                if (lib_strcmp(pDirentTemp->HOITFD_file_name, pcNode) == 0) {
                    goto    __find_ok;                                  /*  找到链接                    */
                }

            }
            else if (S_ISDIR(pDirentTemp->HOITFD_file_type)) {
                if (lib_strcmp(pDirentTemp->HOITFD_file_name, pcNode) == 0) {      /*  已经找到一级目录            */
                    break;
                }
            }
            else {
                if (lib_strcmp(pDirentTemp->HOITFD_file_name, pcNode) == 0) {
                    if (pcNext) {                                       /*  还存在下级, 这里必须为目录  */
                        goto    __find_error;                           /*  不是目录直接错误            */
                    }
                    goto    __find_ok;
                }
            }
        }

        if(pDirentTemp == LW_NULL)
            goto __find_error;
        if(S_ISDIR(pDirentTemp->HOITFD_file_type) && pcNext == NULL){
            goto    __find_ok;
        }
        inodeFatherIno = pDirentTemp->HOITFD_ino;                       /*  从当前节点开始搜索          */
        if (pInode != pfs->HOITFS_pRootDir) {
            __hoit_close(pInode, 0);
        }
        pInode = __hoit_get_full_file(pfs, inodeFatherIno);             /*  从第一个儿子开始            */
    } while (pcNext);                                                   /*  不存在下级目录              */

__find_ok:
    if (ppFullDirent) *ppFullDirent = pDirentTemp;
    if (ppInodeFather) *ppInodeFather = pInode;                            /*  父系节点                    */
    /*
     *  计算 tail 的位置.
     */
    if (ppcTail) {
        if (pcNext) {
            INT   iTail = pcNext - pcTempName;
            *ppcTail = (PCHAR)pcName + iTail;                           /*  指向没有被处理的 / 字符     */
        }
        else {
            *ppcTail = (PCHAR)pcName + lib_strlen(pcName);              /*  指向最末尾                  */
        }
    }
    return  __hoit_get_full_file(pfs, pDirentTemp->HOITFD_ino);

__find_error:
    if (ppFullDirent) *ppFullDirent = pDirentTemp;
    if (ppInodeFather) *ppInodeFather = pInode;                            /*  父系节点                    */
    if (pbLast) {
        if (pcNext == LW_NULL) {                                        /*  最后一级查找失败            */
            *pbLast = LW_TRUE;
        }
        else {
            *pbLast = LW_FALSE; 
        }
    }
    return  (LW_NULL);                                                  /*  无法找到节点                */
}


/*********************************************************************************************************
** 函数名称: __hoit_maken
** 功能描述: HoitFs 创建一个文件
** 输　入  : pfs              文件系统
**           pcName           文件名，不能含有上级目录的名称
**           pInodeFather     父亲, NULL 表示根目录
**           mode             mode_t
**           pcLink           如果为连接文件, 这里指明连接目标.
** 输　出  : 创建结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
PHOIT_INODE_INFO  __hoit_maken(PHOIT_VOLUME  pfs,
    CPCHAR       pcName,
    PHOIT_INODE_INFO    pInodeFather,
    mode_t       mode,
    CPCHAR       pcLink)
{   
    PHOIT_INODE_INFO pInodeInfo = LW_NULL;

    // 暂用socket代表硬链接文件
    if (S_ISSOCK(mode)) {
        pInodeInfo = __hoit_open(pfs, pcLink, LW_NULL, LW_NULL, LW_NULL, LW_NULL, LW_NULL);
    }
    else {
        pInodeInfo = __hoit_new_inode_info(pfs, mode, pcLink);
    }
    

    PHOIT_FULL_DIRENT   pFullDirent = (PHOIT_FULL_DIRENT)__SHEAP_ALLOC(sizeof(HOIT_FULL_DIRENT));
    CPCHAR      pcFileName;

    if (pFullDirent == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (LW_NULL);
    }

    lib_bzero(pFullDirent, sizeof(HOIT_FULL_DIRENT));

    pcFileName = lib_rindex(pcName, PX_DIVIDER);
    if (pcFileName) {
        pcFileName++;
    }
    else {
        pcFileName = pcName;
    }

    pFullDirent->HOITFD_file_name = (PCHAR)__SHEAP_ALLOC(lib_strlen(pcFileName) + 1);
    lib_bzero(pFullDirent->HOITFD_file_name, lib_strlen(pcFileName) + 1);
    if (pFullDirent->HOITFD_file_name == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (LW_NULL);
    }
    lib_memcpy(pFullDirent->HOITFD_file_name, pcFileName, lib_strlen(pcFileName));

    // 暂用socket代表硬链接文件
    if (S_ISSOCK(mode)) {
        pFullDirent->HOITFD_file_type = pInodeInfo->HOITN_mode;
    }
    else {
        pFullDirent->HOITFD_file_type = mode;
    }
    pFullDirent->HOITFD_ino = pInodeInfo->HOITN_ino;
    pFullDirent->HOITFD_nhash = __hoit_name_hash(pcFileName);
    pFullDirent->HOITFD_pino = pInodeFather->HOITN_ino;

    __hoit_add_dirent(pInodeFather, pFullDirent, 1);

    return pInodeInfo;
}
/*********************************************************************************************************
** 函数名称: __hoit_unlink_regular
** 功能描述: HoitFs 将普通文件链接数减1，将相应的FullDirent标记为过期，如果链接数减至为0则文件的RawInode也将被标记过期
**           相当于本函数只删除非目录文件
**           注意参数传进来的pDirent不会在该函数内被释放，应该由调用该函数的上级函数负责释放
** 输　入  : pramn            文件节点
** 输　出  : 删除结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_unlink_regular(PHOIT_INODE_INFO pInodeFather, PHOIT_FULL_DIRENT  pDirent)
{
    if (pDirent == LW_NULL || S_ISDIR(pDirent->HOITFD_file_type)) {
        _ErrorHandle(ENOTEMPTY);
        return  (PX_ERROR);
    }
    PHOIT_VOLUME pfs = pInodeFather->HOITN_volume;
    PHOIT_INODE_CACHE pInodeCache = __hoit_get_inode_cache(pfs, pDirent->HOITFD_ino);
    pInodeCache->HOITC_nlink -= 1;

    PHOIT_INODE_CACHE pFatherInodeCache = __hoit_get_inode_cache(pfs, pInodeFather->HOITN_ino);
    /*
    *将被删除的FullDirent对应的RawInfo和Flash上的RawDirent删除
    */
    PHOIT_RAW_INFO pRawInfo = pDirent->HOITFD_raw_info;
    __hoit_del_raw_info(pFatherInodeCache, pRawInfo);     //将RawInfo从InodeCache的链表中删除
    __hoit_del_raw_data(pfs, pRawInfo);
    pRawInfo->is_obsolete = 1;

    /*
    *将该FullDirent从父目录文件中的dents链表删除
    */
    __hoit_del_full_dirent(&(pInodeFather->HOITN_dents), pDirent);

    /*
    *如果nlink减为0，则将该InodeCache对应的文件所有在Flash上的数据标记为过期并释放掉内存中的InodeCache
    */
    if (pInodeCache->HOITC_nlink == 0) {
        PHOIT_RAW_INFO pRawTemp = pInodeCache->HOITC_nodes;
        PHOIT_RAW_INFO pRawNext = LW_NULL;
        while (pRawTemp) {
            __hoit_del_raw_data(pfs, pRawTemp);
            pRawNext = pRawTemp->next_logic;
            pRawTemp->is_obsolete = HOIT_FLAG_OBSOLETE;
            pRawTemp = pRawNext;
        }
        __hoit_del_inode_cache(pfs, pInodeCache);
        __SHEAP_FREE(pInodeCache);
    }

    return  (ERROR_NONE);
}

/*********************************************************************************************************
** 函数名称: __hoit_truncate
** 功能描述: hoitfs 截断一个文件(直接删除数据), 注意这个函数只用来截断普通类型文件
** 输　入  : pInodeInfo       文件节点
**           offset            截断点
** 输　出  : 截断结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_truncate(PHOIT_INODE_INFO  pInodeInfo, size_t  offset)
{
    hoitFragTreeDeleteRange(pInodeInfo->HOITN_rbtree, offset, UINT_MAX, LW_TRUE);
}

/*********************************************************************************************************
** 函数名称: __hoit_unlink_dir
** 功能描述: 将一个目录文件删除，包括对其所有子文件进行删除（普通文件调用__hoit_unlink_regular，如果有子文件是目录文件则递归调用__hoit_unlink_dir）
**           相当于本函数只删除目录文件（与ramfs不同，本函数既删除目录文件下的子文件，又删除目录文件本身）
**           注意参数传进来的pDirent不会在该函数内被释放，应该由调用该函数的上级函数负责释放
** 输　入  : pramn            文件节点
** 输　出  : !=0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_unlink_dir(PHOIT_INODE_INFO pInodeFather, PHOIT_FULL_DIRENT  pDirent) {
    if (pDirent == LW_NULL || !S_ISDIR(pDirent->HOITFD_file_type)) {
        _ErrorHandle(ENOTEMPTY);
        return  (PX_ERROR);
    }
    PHOIT_VOLUME pfs = pInodeFather->HOITN_volume;
    PHOIT_INODE_CACHE pInodeCache = __hoit_get_inode_cache(pfs, pDirent->HOITFD_ino);
    

    PHOIT_INODE_CACHE pFatherInodeCache = __hoit_get_inode_cache(pfs, pInodeFather->HOITN_ino);
    /*
    *将被删除的FullDirent对应的RawInfo和Flash上的RawDirent删除
    */
    PHOIT_RAW_INFO pRawInfo = pDirent->HOITFD_raw_info;
    __hoit_del_raw_info(pFatherInodeCache, pRawInfo);     //将RawInfo从InodeCache的链表中删除
    __hoit_del_raw_data(pfs, pRawInfo);
    pRawInfo->is_obsolete = HOIT_FLAG_OBSOLETE;
    /*
    *将该FullDirent从父目录文件中的dents链表删除，接着将FullDirent内存释放掉
    */
    __hoit_del_full_dirent(&(pInodeFather->HOITN_dents), pDirent);
;
    /*
    *目录文件nlink为1，再减1就变为0了，必须先尝试unlink子文件，再删除目录文件本身的数据
    */
    if (pInodeCache->HOITC_nlink == 1) {
        //先打开目录文件
        PHOIT_INODE_INFO pDirFileInode = __hoit_get_full_file(pfs, pInodeCache->HOITC_ino);
        if (!S_ISDIR(pDirFileInode->HOITN_mode)) return HOIT_ERROR;

        //再一次unlink目录文件下的每个子文件
        PHOIT_FULL_DIRENT pFullDirent = pDirFileInode->HOITN_dents;
        PHOIT_FULL_DIRENT pFullDirentNext = LW_NULL;
        while (pFullDirent) {
            pFullDirentNext = pFullDirent->HOITFD_next;
            if (!S_ISDIR(pFullDirent->HOITFD_file_type)) {
                __hoit_unlink_regular(pDirFileInode, pFullDirent);
            }
            else {
                __hoit_unlink_dir(pDirFileInode, pFullDirent);
            }
            __hoit_free_full_dirent(pFullDirent);
            pFullDirent = pFullDirentNext;
        }

        //每个目录文件有一个自己的RawInode需要我们自己删除
        PHOIT_RAW_INFO pRawTemp = pInodeCache->HOITC_nodes;
        PHOIT_RAW_INFO pRawNext = LW_NULL;
        while (pRawTemp) {
            __hoit_del_raw_data(pfs, pRawTemp);
            pRawNext = pRawTemp->next_logic;
            pRawTemp->is_obsolete = HOIT_FLAG_OBSOLETE;
            pRawTemp = pRawNext;
        }

        __hoit_del_inode_cache(pfs, pInodeCache);
        __SHEAP_FREE(pDirFileInode);
        __SHEAP_FREE(pInodeCache);
    }
    pInodeCache->HOITC_nlink -= 1;
    return ERROR_NONE;
}

/*********************************************************************************************************
** 函数名称: __hoit_close
** 功能描述: hoitfs 关闭一个文件, 清空其所有full_xxx结构体, 最后会释放传入的InodeInfo
** 输　入  : pInodeInfo           文件节点
**           iFlag            打开文件时的方法
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_close(PHOIT_INODE_INFO  pInodeInfo, INT  iFlag)
{
    if(pInodeInfo->HOITN_ino == HOIT_ROOT_DIR_INO || pInodeInfo == LW_NULL){ /* 不close根目录 */
        return;
    }
    if (S_ISDIR(pInodeInfo->HOITN_mode)) {
        PHOIT_FULL_DIRENT pFullDirent = pInodeInfo->HOITN_dents;
        PHOIT_FULL_DIRENT pFullNext = LW_NULL;
        while (pFullDirent) {
            pFullNext = pFullDirent->HOITFD_next;
            __SHEAP_FREE(pFullDirent->HOITFD_file_name);
            __SHEAP_FREE(pFullDirent);
            pFullDirent = pFullNext;
        }
        if (pInodeInfo->HOITN_metadata != LW_NULL) __SHEAP_FREE(pInodeInfo->HOITN_metadata);
        __SHEAP_FREE(pInodeInfo);
    }
    else if(S_ISLNK(pInodeInfo->HOITN_mode)){
        if (pInodeInfo->HOITN_metadata != LW_NULL) __SHEAP_FREE(pInodeInfo->HOITN_metadata);
        __SHEAP_FREE(pInodeInfo);
    }
    else {
        hoitFragTreeDeleteTree(pInodeInfo->HOITN_rbtree, LW_FALSE);
        if (pInodeInfo->HOITN_metadata != LW_NULL) __SHEAP_FREE(pInodeInfo->HOITN_metadata);
        __SHEAP_FREE(pInodeInfo);
    }
}
/*********************************************************************************************************
** 函数名称: __hoit_move_check
** 功能描述: HoitFs 检查第二个节点是否为第一个节点的子孙
** 输　入  : pInode1       第一个节点
**           pInode2       第二个节点
** 输　出  : ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_move_check(PHOIT_INODE_INFO  pInode1, PHOIT_INODE_INFO  pInode2)
{
    PHOIT_VOLUME pfs = pInode1->HOITN_volume;

    if (!S_ISDIR(pInode1->HOITN_mode)) {
        return (PX_ERROR);
    }
    PHOIT_FULL_DIRENT pFullDirent = pInode1->HOITN_dents;
    while (pFullDirent) {
        if (S_ISDIR(pFullDirent->HOITFD_file_type)) {
            if (pFullDirent->HOITFD_ino == pInode2->HOITN_ino)
                return PX_ERROR;
            PHOIT_INODE_INFO pTempInfo = __hoit_get_full_file(pfs, pFullDirent->HOITFD_ino);
            int checkResult = __hoit_move_check(pTempInfo, pInode2);

            __hoit_close(pTempInfo, 0);
            if (checkResult != ERROR_NONE)
                return PX_ERROR;
        }
        else {
            if (pFullDirent->HOITFD_ino == pInode2->HOITN_ino)
                return PX_ERROR;
        }
    }
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __hoit_move
** 功能描述: HoitFs 移动或者重命名一个文件
** 输　入  : pInodeFather     文件的父目录节点（move之前的）
**           pInodeInfo       文件节点
**           pcNewName        新的名字
** 输　出  : ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_move(PHOIT_INODE_INFO pInodeFather, PHOIT_INODE_INFO  pInodeInfo, PCHAR  pcNewName)
{
    INT         iRet;
    PHOIT_VOLUME pfs;
    PHOIT_INODE_INFO   pInodeTemp;
    PHOIT_INODE_INFO   pInodeNewFather;
    BOOL        bRoot;
    BOOL        bLast;
    PCHAR       pcTail;
    PCHAR       pcTemp;
    PCHAR       pcFileName;

    pfs = pInodeInfo->HOITN_volume;

    pInodeTemp = __hoit_open(pfs, pcNewName, &pInodeNewFather, LW_NULL, &bRoot, &bLast, &pcTail);

    if (!pInodeTemp && (bRoot || (bLast == LW_FALSE))) {                 /*  新名字指向根或者没有目录    */
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }

    if (pInodeInfo == pInodeTemp) {                                           /*  相同                        */
        return  (ERROR_NONE);
    }

    if (S_ISDIR(pInodeInfo->HOITN_mode) && pInodeNewFather) {
        if (__hoit_move_check(pInodeInfo, pInodeNewFather)) {                  /*  检查目录合法性              */
            _ErrorHandle(EINVAL);
            return  (PX_ERROR);
        }
    }
    pcFileName = lib_rindex(pcNewName, PX_DIVIDER);
    if (pcFileName) {
        pcFileName++;
    }
    else {
        pcFileName = pcNewName;
    }

    pcTemp = (PCHAR)__SHEAP_ALLOC(lib_strlen(pcFileName) + 1);          /*  预分配名字缓存              */
    lib_bzero(pcTemp, lib_strlen(pcFileName) + 1);
    if (pcTemp == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (PX_ERROR);
    }
    lib_memcpy(pcTemp, pcFileName, lib_strlen(pcFileName));
    if (pInodeTemp) {
        if (!S_ISDIR(pInodeInfo->HOITN_mode) && S_ISDIR(pInodeTemp->HOITN_mode)) {
            __SHEAP_FREE(pcTemp);
            _ErrorHandle(EISDIR);
            return  (PX_ERROR);
        }
        if (S_ISDIR(pInodeInfo->HOITN_mode) && !S_ISDIR(pInodeTemp->HOITN_mode)) {
            __SHEAP_FREE(pcTemp);
            _ErrorHandle(ENOTDIR);
            return  (PX_ERROR);
        }

        PHOIT_FULL_DIRENT pFullDirent = __hoit_search_in_dents(pInodeNewFather, pInodeTemp->HOITN_ino, pcTemp);
        if (pFullDirent == LW_NULL) {
            __SHEAP_FREE(pcTemp);
            _ErrorHandle(ENOTDIR);
            return  (PX_ERROR);
        }
        if (S_ISDIR(pInodeTemp->HOITN_mode)) {
            iRet = __hoit_unlink_dir(pInodeNewFather, pFullDirent);     /*  删除目标                    */
        }
        else {
            iRet = __hoit_unlink_regular(pInodeNewFather, pFullDirent); /*  删除目标                    */
        }
        __hoit_free_full_dirent(pFullDirent);
        

        if (iRet) {
            __SHEAP_FREE(pcTemp);
            return  (PX_ERROR);
        }
    }

    if (pInodeFather != pInodeNewFather) {                              /*  目录发生改变                */
        PHOIT_FULL_DIRENT pFullDirent = (PHOIT_FULL_DIRENT)__SHEAP_ALLOC(sizeof(HOIT_FULL_DIRENT));
        pFullDirent->HOITFD_file_name = pcTemp;
        pFullDirent->HOITFD_file_type = pInodeInfo->HOITN_mode;
        pFullDirent->HOITFD_ino = pInodeInfo->HOITN_ino;
        pFullDirent->HOITFD_nhash = __hoit_name_hash(pcTemp);
        pFullDirent->HOITFD_pino = pInodeNewFather->HOITN_ino;
        __hoit_add_dirent(pInodeNewFather, pFullDirent, 1);

        PHOIT_FULL_DIRENT pOldDirent = __hoit_search_in_dents(pInodeFather, pInodeInfo->HOITN_ino, pcTemp);
        __hoit_del_full_dirent(&(pInodeFather->HOITN_dents), pOldDirent);
    }
    else {
        PHOIT_FULL_DIRENT pOldDirent = __hoit_search_in_dents(pInodeFather, pInodeInfo->HOITN_ino, pcTemp);
        __hoit_del_full_dirent(&(pInodeFather->HOITN_dents), pOldDirent);

        __SHEAP_FREE(pOldDirent->HOITFD_file_name);
        pOldDirent->HOITFD_file_name = pcTemp;

        if (__hoit_add_to_dents(&(pInodeFather->HOITN_dents), pOldDirent)) {
            __SHEAP_FREE(pOldDirent->HOITFD_file_name);
            __SHEAP_FREE(pOldDirent);
        }

    }
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: __hoit_stat
** 功能描述: 从一个打开的文件中读取相应数据到stat结构体
** 输　入  : pInodeInfo       文件节点
** 输　出  : !=0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_stat(PHOIT_INODE_INFO pInodeInfo, PHOIT_VOLUME  pfs, struct stat* pstat) {
    if (pInodeInfo) {
        pstat->st_dev = LW_DEV_MAKE_STDEV(&pfs->HOITFS_devhdrHdr);
        pstat->st_ino = (ino_t)pInodeInfo;
        pstat->st_mode = pInodeInfo->HOITN_mode;
        pstat->st_nlink = pInodeInfo->HOITN_inode_cache->HOITC_nlink;
        pstat->st_uid = pInodeInfo->HOITN_uid;
        pstat->st_gid = pInodeInfo->HOITN_gid;
        pstat->st_rdev = 1;
        pstat->st_size = (off_t)pInodeInfo->HOITN_stSize;
        // pstat->st_atime = pInodeInfo->HOITN_timeAccess;
        // pstat->st_mtime = pInodeInfo->HOITN_timeChange;
        // pstat->st_ctime = pInodeInfo->HOITN_timeCreate;
        pstat->st_blksize = 0;
        pstat->st_blocks = 0;
    }
    else {
        pstat->st_dev = LW_DEV_MAKE_STDEV(&pfs->HOITFS_devhdrHdr);
        pstat->st_ino = (ino_t)0;
        pstat->st_mode = pfs->HOITFS_mode;
        pstat->st_nlink = 1;
        pstat->st_uid = pfs->HOITFS_uid;
        pstat->st_gid = pfs->HOITFS_gid;
        pstat->st_rdev = 1;
        pstat->st_size = 0;
        pstat->st_atime = pfs->HOITFS_time;
        pstat->st_mtime = pfs->HOITFS_time;
        pstat->st_ctime = pfs->HOITFS_time;
        pstat->st_blksize = 0;
        pstat->st_blocks = 0;
    }

    pstat->st_resv1 = LW_NULL;
    pstat->st_resv2 = LW_NULL;
    pstat->st_resv3 = LW_NULL;
    return ERROR_NONE;
}
/*********************************************************************************************************
** 函数名称: __hoit_statfs
** 功能描述: 读取文件系统相关信息到pstatfs
** 输　入  : pInodeInfo       文件节点
** 输　出  : !=0代表出错
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  __hoit_statfs(PHOIT_VOLUME  pfs, struct statfs* pstatfs) {
    pstatfs->f_type = TMPFS_MAGIC;  //需要修改
    pstatfs->f_bsize = 0;
    pstatfs->f_blocks = 0;
    pstatfs->f_bfree = 0;
    pstatfs->f_bavail = 1;

    pstatfs->f_files = 0;
    pstatfs->f_ffree = 0;

#if LW_CFG_CPU_WORD_LENGHT == 64
    pstatfs->f_fsid.val[0] = (int32_t)((addr_t)pfs >> 32);
    pstatfs->f_fsid.val[1] = (int32_t)((addr_t)pfs & 0xffffffff);
#else
    pstatfs->f_fsid.val[0] = (int32_t)pfs;
    pstatfs->f_fsid.val[1] = 0;
#endif

    pstatfs->f_flag = 0;
    pstatfs->f_namelen = PATH_MAX;
    return ERROR_NONE;
}

/*********************************************************************************************************
** 函数名称: __hoit_read
** 功能描述: hoitfs 读取文件内容
** 输　入  : 
** 输　出  : 读取的字节数
** 全局变量:
** 调用模块:
*********************************************************************************************************/
ssize_t  __hoit_read(PHOIT_INODE_INFO  pInodeInfo, PVOID  pvBuffer, size_t  stSize, size_t  stOft)
{
    ssize_t readSize = 0;
    if (pInodeInfo->HOITN_stSize <= stOft) {                                 /*  已经到文件末尾              */
        return  (0);
    }

    readSize = pInodeInfo->HOITN_stSize - stOft;                           /*  计算剩余数据量              */
    readSize = __MIN(readSize, stSize);
    hoitFragTreeRead(pInodeInfo->HOITN_rbtree, stOft, stSize, pvBuffer);
    return readSize;
}
/*********************************************************************************************************
** 函数名称: __hoit_write
** 功能描述: hoitfs 写入文件内容(该函数只创建raw_inode类型的数据实体)
** 输　入  : pInodeInfo            文件节点
**           pvBuffer         缓冲区
**           stNBytes         需要读取的大小
**           stOft            偏移量
** 输　出  : 读取的字节数
** 全局变量:
** 调用模块:
*********************************************************************************************************/
ssize_t  __hoit_write(PHOIT_INODE_INFO  pInodeInfo, CPVOID  pvBuffer, size_t  stNBytes, size_t  stOft, UINT needLog) {
    if (pInodeInfo->HOITN_rbtree != LW_NULL) {
        if(stNBytes == 0){
            return stNBytes;
        }
        PHOIT_FULL_DNODE pFullDnode = __hoit_write_full_dnode(pInodeInfo, stOft, stNBytes, pvBuffer, needLog);
        PHOIT_FRAG_TREE_NODE pTreeNode = newHoitFragTreeNode(pFullDnode, stNBytes, stOft, stOft);
        hoitFragTreeInsertNode(pInodeInfo->HOITN_rbtree, pTreeNode);
        hoitFragTreeOverlayFixUp(pInodeInfo->HOITN_rbtree);

        if (stNBytes < HOIT_WRITE_BUFFER_FRAGSIZE) {
            __hoit_new_write_entry(pInodeInfo, pInodeInfo->HOITN_pWriteBuffer, pTreeNode);
        }

        return stNBytes;
    }
    else {
        PHOIT_FULL_DNODE pFullDnode = __hoit_write_full_dnode(pInodeInfo, stOft, stNBytes, pvBuffer, needLog);
        pInodeInfo->HOITN_metadata = pFullDnode;
        return stNBytes;
    }
}

/*********************************************************************************************************
** 函数名称: __hoit_ummount
** 功能描述: hoitfs 卸载
** 输　入  : pfs               文件系统
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_unmount(PHOIT_VOLUME pfs)
{
    /* TODO 释放RAW INFO需要把GC先关了*/
    //API_SpinDestory()
    if (pfs == LW_NULL) {
        printf("Error in unmount.\n");
        return;
    }
    hoitFlushCache(pfs->HOITFS_cacheHdr);
    hoitGCClose(pfs);
    __hoit_close(pfs->HOITFS_pRootDir, 0);  /* 先删除根目录 */
    hoitFreeCache(pfs->HOITFS_cacheHdr);    /* 释放缓存层 */

    if (pfs->HOITFS_pTempRootDirent != LW_NULL) {   /* 删除TempDirent链表 */
        PHOIT_FULL_DIRENT pTempDirent = pfs->HOITFS_pTempRootDirent;
        PHOIT_FULL_DIRENT pNextDirent = LW_NULL;
        while (pTempDirent) {
            pNextDirent = pTempDirent->HOITFD_next;
            __SHEAP_FREE(pTempDirent);
            pTempDirent = pNextDirent;
        }
    }

    /* 只先删除所有的inode cache, raw info等到删除sector的时候再删除 */
    PHOIT_INODE_CACHE pTempCache = pfs->HOITFS_cache_list;
    PHOIT_INODE_CACHE pNextCache = LW_NULL;
    while (pTempCache) {
        pNextCache = pTempCache->HOITC_next;
        __SHEAP_FREE(pTempCache);
        pTempCache = pNextCache;
    }

    /* 释放所有sector */
    PHOIT_ERASABLE_SECTOR pTempSector = pfs->HOITFS_erasableSectorList;
    PHOIT_ERASABLE_SECTOR pNextSector = LW_NULL;
    while (pTempSector) {
        pNextSector = pTempSector->HOITS_next;
        /* 删除RAW INFO, 小心GC */
        PHOIT_RAW_INFO pTempRaw = pTempSector->HOITS_pRawInfoFirst;
        PHOIT_RAW_INFO pNextRaw = LW_NULL;
        while (pTempRaw) {
            pNextRaw = pTempRaw->next_phys;
            __SHEAP_FREE(pTempRaw);
            pTempRaw = pNextRaw;
        }
        pTempSector = pNextSector;
    }
}
/*********************************************************************************************************
** 函数名称: __hoit_mount
** 功能描述: hoitfs 挂载
** 输　入  : pfs           文件系统
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_mount(PHOIT_VOLUME  pfs)
{
    pfs->HOITFS_highest_ino = 0;
    pfs->HOITFS_highest_version = 0;

    INT                 i;
    INT                 hasLog      = 0;
    UINT                phys_addr   = 0;
    UINT8               sector_no   = hoitGetSectorNo(phys_addr);
    PHOIT_RAW_LOG       pRawLogHdr  = LW_NULL;
    
    LW_CLASS_THREADATTR scThreadAttr;
    LW_OBJECT_HANDLE ulObjectHandle[NOR_FLASH_NSECTOR];
    INT handleSize = 0;

    while (hoitGetSectorSize(sector_no) != -1) {

        ScanThreadAttr* pThreadAttr = (ScanThreadAttr*)lib_malloc(sizeof(ScanThreadAttr));
        pThreadAttr->pfs = pfs;
        pThreadAttr->sector_no = sector_no;

        API_ThreadAttrBuild(&scThreadAttr,
            4 * LW_CFG_KB_SIZE,
            LW_PRIO_NORMAL,
            LW_OPTION_THREAD_STK_CHK,
            (VOID*)pThreadAttr);

        ulObjectHandle[handleSize++] = API_ThreadCreate("t_scan_thread",
            (PTHREAD_START_ROUTINE)__hoit_scan_single_sector,
            &scThreadAttr,
            LW_NULL);
        sector_no++;
    }

    int i;
    for (i = 0; i < handleSize; i++) {
        API_ThreadJoin(ulObjectHandle[i], LW_NULL);
    }
    
    pfs->HOITFS_highest_ino++;
    pfs->HOITFS_highest_version++;
    printf("now sector offs: %d \n", pfs->HOITFS_now_sector->HOITS_offset);
    if (!hasLog) {
        hoitLogInit(pfs, hoitGetSectorSize(8), 1);
    }
    if (pRawLogHdr != LW_NULL){
        hoitLogOpen(pfs, pRawLogHdr);
    }

#ifdef LOG_ENABLE
    __hoit_redo_log(pfs);
#endif // LOG_ENABLE

    if (pfs->HOITFS_highest_ino == HOIT_ROOT_DIR_INO) {    /* 系统第一次运行, 创建根目录文件 */
        mode_t mode = S_IFDIR;
        PHOIT_INODE_INFO pRootDir = __hoit_new_inode_info(pfs, mode, LW_NULL);
        pfs->HOITFS_pRootDir = pRootDir;
    }
    /* 系统不是第一次运行的话会在扫描时就找到pRootDir */


    /* 基本的inode_cache和raw_info构建完毕  */
    /* 接下来要递归统计所有文件的nlink          */
    __hoit_get_nlink(pfs->HOITFS_pRootDir);
    register_hoitfs_cmd(pfs);
}

/*********************************************************************************************************
** 函数名称: __hoit_redo_log
** 功能描述: hoitfs 恢复log
** 输　入  :
** 输　出  : 打开结果
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  __hoit_redo_log(PHOIT_VOLUME  pfs) {
    PHOIT_LOG_INFO pLogInfo = pfs->HOITFS_logInfo;
    UINT i;
    for (i = 0; i < pLogInfo->uiLogEntityCnt; i++) {
        PCHAR p = hoitLogEntityGet(pfs, i);
        PHOIT_RAW_HEADER pRawHeader = (PHOIT_RAW_HEADER)p;
        if(pRawHeader->ino == HOIT_ROOT_DIR_INO){
            continue;
        }
        if (__HOIT_IS_TYPE_INODE(pRawHeader)) {
            PHOIT_RAW_INODE pRawInode = (PHOIT_RAW_INODE)pRawHeader;
            PCHAR pData = p + sizeof(HOIT_RAW_INODE);
            PHOIT_INODE_INFO pInodeInfo = __hoit_get_full_file(pfs, pRawInode->ino);

            pRawHeader->version = pfs->HOITFS_highest_version++;

            __hoit_write(pInodeInfo, pData, pRawInode->totlen - sizeof(HOIT_RAW_INODE), pRawInode->offset, 0);

            __hoit_close(pInodeInfo, 0);
        }
        else if (__HOIT_IS_TYPE_DIRENT(pRawHeader)) {
            PHOIT_RAW_DIRENT pRawDirent = (PHOIT_RAW_DIRENT)pRawHeader;
            PCHAR pFileName = p + sizeof(HOIT_RAW_DIRENT);
            UINT uiNameLength = pRawDirent->totlen - sizeof(HOIT_RAW_DIRENT);
            PHOIT_FULL_DIRENT pFullDirent = (PHOIT_FULL_DIRENT)__SHEAP_ALLOC(sizeof(HOIT_FULL_DIRENT));
            pFullDirent->HOITFD_file_name = (PCHAR)__SHEAP_ALLOC(uiNameLength + 1);
            pRawHeader->version = pfs->HOITFS_highest_version++;

            lib_bzero(pFullDirent->HOITFD_file_name, uiNameLength + 1);
            if (pFullDirent->HOITFD_file_name == LW_NULL) {
                _ErrorHandle(ENOMEM);
                return  (LW_NULL);
            }
            lib_memcpy(pFullDirent->HOITFD_file_name, pFileName, uiNameLength);

            pFullDirent->HOITFD_file_type = pRawDirent->file_type;
            pFullDirent->HOITFD_ino = pRawDirent->ino;
            pFullDirent->HOITFD_nhash = __hoit_name_hash(pFullDirent->HOITFD_file_name);
            pFullDirent->HOITFD_pino = pRawDirent->pino;
      
            PHOIT_INODE_INFO pInodeFather = __hoit_get_full_file(pfs, pRawDirent->pino);

            __hoit_add_dirent(pInodeFather, pFullDirent, 0);

            __hoit_close(pInodeFather, 0);
        }

        __SHEAP_FREE(p);
    }
}

/*********************************************************************************************************
** 函数名称: __hoit_erasable_sector_list_check_exist
** 功能描述: 检查链表上是否有该sector。
** 输　入  :
**          HOITFS_sectorList   将要检查的列表
**          pErasableSector     需要添加到列表的sector
** 输　出  : 存在，返回LW_TRUE指针；不存在，返回LW_FALSE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
USE_LIST_TEMPLATE(NS, HOIT_ERASABLE_SECTOR);

BOOL __hoit_erasable_sector_list_check_exist(List(HOIT_ERASABLE_SECTOR) HOITFS_sectorList, PHOIT_ERASABLE_SECTOR pErasableSector) {
    Iterator(HOIT_ERASABLE_SECTOR)      iter;
    InitIterator(iter, NS, HOIT_ERASABLE_SECTOR);

    PHOIT_ERASABLE_SECTOR               psector;
    for(iter->begin(iter, HOITFS_sectorList); iter->isValid(iter); iter->next(iter)) {
        psector = iter->get(iter);
        if (psector == pErasableSector) {
            return LW_TRUE;
        }
    }

    FreeIterator(iter);
    return LW_FALSE;
}


//TODO: 只在初始化阶段调用，其他写入或GC操作部分也应该调用
/*********************************************************************************************************
** 函数名称: __hoit_fix_up_sector_list
** 功能描述: 检查一个sector的类型（dirty，clean或free），并添加到volume对应链表上。
**          如果sector已经在对应链表中，则不做处理
** 输　入  :
**          pfs                 文件卷
**          pErasableSector     需要添加到列表的sector
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID __hoit_fix_up_sector_list(PHOIT_VOLUME pfs, PHOIT_ERASABLE_SECTOR pErasableSector) {
    if (pErasableSector->HOITS_uiFreeSize == GET_SECTOR_SIZE(pErasableSector->HOITS_bno)) { /* 空sector */
        if (!__hoit_erasable_sector_list_check_exist(GET_FREE_LIST(pfs), pErasableSector)) {
            GET_FREE_LIST(pfs)->insert(GET_FREE_LIST(pfs), pErasableSector, 0);
        }
    }
    if (pErasableSector->HOITS_uiObsoleteEntityCount != 0) {
        /* 目前是只要有脏数据实体，就把sector放到dirty list中 */
        if (!__hoit_erasable_sector_list_check_exist(GET_DIRTY_LIST(pfs), pErasableSector)) {
            GET_DIRTY_LIST(pfs)->insert(GET_DIRTY_LIST(pfs), pErasableSector, 0);
        }
    } else if (pErasableSector->HOITS_uiAvailableEntityCount != 0) {
        if (!__hoit_erasable_sector_list_check_exist(GET_CLEAN_LIST(pfs), pErasableSector)) {
            GET_CLEAN_LIST(pfs)->insert(GET_CLEAN_LIST(pfs), pErasableSector, 0);
        }
    }
}


/*********************************************************************************************************
** 函数名称: __hoit_mark_obsolete
** 功能描述: 
** 输　入  :
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
//! 2021-07-07 ZN整合标注过期
VOID __hoit_mark_obsolete(PHOIT_VOLUME pfs, PHOIT_RAW_HEADER pRawHeader, PHOIT_RAW_INFO pRawInfo){
    PHOIT_CACHE_HDR pcacheHdr = pfs->HOITFS_cacheHdr;
    UINT32  EBS_entry_flag  = 0;
    UINT32  i;
    UINT32  EBS_entry_addr  = (pRawInfo->phys_addr/HOIT_FILTER_PAGE_SIZE) *                    /* EBS entry首地址 */
                                HOIT_FILTER_EBS_ENTRY_SIZE+pcacheHdr->HOITCACHE_EBSStartAddr + 
                                sizeof(UINT32);
    UINT32  EBS_entry_num   = pRawInfo->totlen % HOIT_FILTER_PAGE_SIZE ?                       /* 需要标过期的entry数量 */
                                pRawInfo->totlen / HOIT_FILTER_PAGE_SIZE + 1 : 
                                pRawInfo->totlen / HOIT_FILTER_PAGE_SIZE;

    pRawHeader->flag &= (~HOIT_FLAG_NOT_OBSOLETE);      //将obsolete标志变为0，代表过期
    //TODO 修改flash上EBS采用写不分配
    pRawHeader->crc = 0;
    pRawHeader->crc = crc32_le(pRawHeader, pRawInfo->totlen);
    __hoit_write_flash_thru(pfs, (PVOID)pRawHeader, pRawInfo->totlen, pRawInfo->phys_addr);
    //! 2021-07-07 修改EBS区域
    for(i=0 ; i<EBS_entry_num ; i++) {
        __hoit_write_flash_thru(pfs, (PVOID)&EBS_entry_flag, sizeof(UINT32), EBS_entry_addr + i *sizeof(HOIT_EBS_ENTRY));
    }
}


#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
#endif //HOITFSLIB_DISABLE
