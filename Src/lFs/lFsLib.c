#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
//#include "../SylixOS/fs/fsCommon/fsCommon.h"
#include "../SylixOS/fs/include/fs_fs.h"
//#include "../SylixOS/kernel/inline/inlErrorHandle.h"
//#include "../SylixOS/system/logLib/logLib.h"


/*********************************************************************************************************
  本地头文件
*********************************************************************************************************/
#include "lFsLib.h"


/*********************************************************************************************************
  LFS全局变量
*********************************************************************************************************/
/* 每个block有一个summary secotr，第一个为inode号，第二个为有效位 */
static UINT                             BLOCK_SUMMARY[NOR_FLASH_SECTPBLK*NOR_FLASH_NBLK][2]; 

/* 以sector为单位的缓冲区 */  
static CHAR                              SEC_BUF[NOR_FLASH_SECTORSZ];

/* Block Buffer */
static CHAR                             BLK_BUF[NOR_FLASH_BLKSZ];

/* 
    CHECKPOINT_REGION，在内存中维护，umount时再写入
    第一个数字表示imap所在sector，第二个表示block当前可用sector
 */
static CHECKPOINT_REGION                CPR;

/* IMAP也在内存中维护 */
static UINT                             IMAP[MAX_FILE+1];

/* 当前Block，范围(2~31)，0，1被占用 */
static UINT                             BLOCK_NO = 2;   

/* 在当前BLOCK_NO中，下一个可用sector，范围(0~15) */
static UINT                             AVAILABLE_SECTOR = 0;

/* 查看哪块BLOCK是否是脏的 */
static UINT8                            CLEAN_BLOCK[NOR_FLASH_NBLK];

/*********************************************************************************************************
  LFS初始化
*********************************************************************************************************/
/*
    初始化每个block，以及summary sector
*/
VOID initBlocks(){
    UINT i;
    UINT neg1 = -1;
    UINT summary_sec_offset;
    lib_bzero(SEC_BUF,sizeof(SEC_BUF));
    /* CPR_FN_1_BLK 和 CPR_FN_2_BLK 不做处理，summary sector下面处理 */
    for (i = 3; i < NOR_FLASH_NBLK; i++)
    {
        /* 不经过NOR_FLASH_BUF，直接写入flash */
        write_nor(GET_NOR_OFFSET_FROM_BLKN(i),
                    (PCHAR)SEC_BUF,
                    NOR_FLASH_SECTORSZ);
    }
    /* summary sector 中 ，记录summary sector的那一部分初始化 */
    summary_sec_offset = GET_OFFSET_FROM_BLK_AND_SECN(2,0);
    SEC_BUF[summary_sec_offset] = SUMMARY_SUMMARY_TYPE;
    SEC_BUF[summary_sec_offset+1] = neg1;
    write_nor(GET_NOR_OFFSET_FROM_BLKN(2),
                (PCHAR)SEC_BUF,
                NOR_FLASH_SECTORSZ);   
}

/*
    初始化CPR_FNMap区域，即CheckpointRegion_FileNameMap。该区域用两个block保存。
    CPR.imap_sec：当前imap所在sector，占4B
    CPR.summary_sec：当前summary sector所在位置，占4B
    CPR.free_sec：当前block可用sector，占32B
        block1              block2
    +---------------+   +---------------+
    |     CPR       |   | File511 name  |
    +---------------+   +---------------+
    |  File0 name   |   |      ...      |
    +---------------+   +---------------+
    |     ...       |   |      ...      |
    +---------------+   +---------------+
    |  File510 name |   | File1022 name |
    +---------------+   +---------------+
*/
VOID initCPR_FNMap()
{
    UINT i;
    CHAR buf[NOR_FLASH_BLKSZ];
    lib_bzero(buf,NOR_FLASH_BLKSZ);
    UINT neg1 = -1;
    UINT temp_sec = 2*NOR_FLASH_SECTPBLK;
    PCHAR temp_p = (PCHAR)&temp_sec;
    /* CPR.imap_sec初始化为-1 */
    /* CPR.summary_sec初始化为 2*NOR_FLASH_SECTPBLK，即第三block的第一个secotr */
    /* CPR.free_sec 以及FileName全初始化为0 */ 
    for ( i = 0; i < 4; i++)
        buf[i] = neg1;

    for ( i = 0; i < 4; i++)
        buf[i+4] = *(temp_p+i);

    /* 写入flash */
    write_nor(GET_NOR_OFFSET_FROM_BLKN(CPR_FN_1_BLK),
                (PCHAR)buf,
                NOR_FLASH_BLKSZ);

    lib_bzero(buf,NOR_FLASH_BLKSZ);
    write_nor(GET_NOR_OFFSET_FROM_BLKN(CPR_FN_2_BLK),
                (PCHAR)buf,
                NOR_FLASH_BLKSZ);

}

/*
    初始化CleanBlock
*/
VOID initCleanBlock()
{
    lib_bzero(CLEAN_BLOCK,NOR_FLASH_NBLK);
}

/*
    TODO:初始化LFS的各种缓冲区
*/
INT initLFS(PLFS_VOLUME plfs)
{
    return ERROR_NONE;
}

/*
    读取CPR到内存
*/
INT readInCheckpointRegion(PLFS_VOLUME plfs)
{
    UINT                i;
    read_content_t      content;
    PCHECKPOINT_REGION  CPR;
    PCHAR               temp;
    CPR = plfs->LFS_cpr;

    content = read_nor(GET_NOR_OFFSET_FROM_BLKN(CPR_FN_1_BLK),READ_SECTOR);
    if (content.is_success)
    {
        /* 读取CPR.imap_sec */
        temp = (PCHAR)CPR;
        // for ( i = 0; i < 3; i++)
        // {
        //     *(temp+i)=content.content[i];
        // }

        // /* 读取CPR.summary_sector_no */

        // for ( i = 0; i < 3; i++)
        // {
        //     *(temp+i)=content.content[i+4];
        // }

        // /* 读取CPR.free_sec */
        // temp = (CHAR*)CPR.free_sec;
        // for ( i = 0; i < 32; i++)
        // {
        //     *(temp+i) = content.content[i+8];       /* 跳过CPR前面的数据 */
        // }
        for ( i = 0; i < sizeof(CHECKPOINT_REGION); i++)
        {
            *(temp+i) = content.content[i];
        }
        

        return ERROR_NONE;
    }
    else
    {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "nor flash read failed .\r\n");
        _ErrorHandle(EFAULT); 
        return PX_ERROR;
    }
}

/*
    找下一个空闲Block，把结果保存在全局变量BLOCK_NO和AVAILABLE_SECTOR中
*/
INT findNextAvailableSector()
{
    if(CPR.imap_sec!=(UINT)-1)
    {
        BLOCK_NO = GET_BLKN_OF_SECN(CPR.imap_sec);
        AVAILABLE_SECTOR = GET_SEC_IDX_IN_BLK(CPR.imap_sec);
        return ERROR_NONE;
    }
    else
    {
        return PX_ERROR;
    }
}

/* 
    读取当前summary sector到BLOCK_SUMMARY中
 */
INT readSummary()
{
    read_content_t content;
    content = read_nor(GET_NOR_OFFSET_FROM_SECN(CPR.summary_sec),READ_SECTOR);
    if(content.is_success)
    {
        lib_memcpy(BLOCK_SUMMARY,content.content,NOR_FLASH_SECTORSZ);
        return ERROR_NONE;
    }
    else
        return PX_ERROR;
}

/*
    读取当前sector到SEC_BUF中
    @sector_no 全局sector号
*/
INT readInSector(PLFS_VOLUME plfs, UINT sector_no)
{
    read_content_t content;
    UINT    i = GET_NOR_OFFSET_FROM_SECN(sector_no);
    if(sector_no<0||sector_no>NOR_FLASH_NSECTOR)
        return PX_ERROR;

    content = read_nor(GET_NOR_OFFSET_FROM_SECN(sector_no),READ_SECTOR);
    if(content.is_success)
    {
        lib_memcpy((PCHAR)plfs->LFS_secBuf,
                        content.content,
                        NOR_FLASH_SECTORSZ);
        return ERROR_NONE;
    }
    else
        return PX_ERROR; 
}

/*
    读当前block到BLK_BUF中
    @blk_no block号
    modified
*/
INT readInBlock(PLFS_VOLUME plfs, UINT blk_no)
{
    UINT i;
    read_content_t content;
    /* 一个sector一个sector地读取 */
    for ( i = 0; i < NOR_FLASH_SECTPBLK; i++)
    {
        content = read_nor(GET_OFFSET_FROM_BLK_AND_SECN(blk_no,i),READ_SECTOR);
        if(content.is_success)
        {
            lib_memcpy((PCHAR)&plfs->LFS_blkBuf[i*NOR_FLASH_SECTORSZ],
                            content.content,
                            NOR_FLASH_SECTORSZ);
        }
        else
            return PX_ERROR;        
    }
    return ERROR_NONE;
}

/*
    读imap sector到IMAP中
*/
UINT readInImapSector(UINT address, UINT fragment_no)
{
    read_content_t content;
    content = read_nor(GET_NOR_OFFSET_FROM_SECN(CPR.imap_sec),READ_SECTOR);
    if(content.is_success)
    {
        lib_memcpy(IMAP,content.content,NOR_FLASH_SECTORSZ);
        return ERROR_NONE;
    }
    else
        return PX_ERROR;    
}

/* 
    回写CheckoutPoint Region 
*/
INT updateCheckpointRegion(PLFS_VOLUME plfs)
{
    UINT i;
    /* 因为CPR只占block的一部分，所以先把该sector读出来 */
    read_content_t content;
    content = read_nor(GET_NOR_OFFSET_FROM_BLKN(CPR_FN_1_BLK),READ_SECTOR);
    if(content.is_success)
    {
        lib_memcpy(SEC_BUF,content.content,NOR_FLASH_SECTORSZ);
        return ERROR_NONE;
    }
    else
        return PX_ERROR;
    
    /* CPR写入buf */
    PCHAR temp = (PCHAR)plfs->LFS_cpr;
    for ( i = 0; i < sizeof(CHECKPOINT_REGION); i++)
    {
        plfs->LFS_secBuf[i] = *(temp+i);
    }

    /* 写回flash */   
    write_nor(GET_NOR_OFFSET_FROM_BLKN(CPR_FN_1_BLK),
                (PCHAR)plfs->LFS_secBuf,
                NOR_FLASH_SECTORSZ);
}

/* 
    寻找下一个空闲的Segment，在CPR中查找
    返回segment在block中的位置或-1
    modified
*/
UINT findNextCleanBlock(PLFS_VOLUME plfs)
{
    UINT i;
    for (i = 2; i < NOR_FLASH_NBLK; i++)
    {
        if ((plfs->LFS_cpr)->free_sec[i]==BLK_CLEAN)
        {
            //updateCheckpointRegion(plfs);
            return i;
        } 
    }
    //_DebugHandle(__ERRORMESSAGE_LEVEL, "No available memory remaining.\r\n");
    return (PX_ERROR);
}

/* 
    将当前SEC_BUF内容写回LFS_curBlockNo的sector中，并更新LFS_availableSector
    如果是block最后一块sector，则要换下一块了
    返回写入的sector的全局序号
*/
UINT writeOutSector(PLFS_VOLUME plfs)
{
    UINT next_block_no;
    UINT sector_to_write;
    write_nor(GET_OFFSET_FROM_BLK_AND_SECN(plfs->LFS_curBlockNo,
                                            plfs->LFS_availableSector),
                (PCHAR)plfs->LFS_secBuf,
                NOR_FLASH_SECTORSZ);
    sector_to_write = plfs->LFS_curBlockNo*NOR_FLASH_SECTPBLK+plfs->LFS_availableSector;
    /* 成功写入sector后要及时更新CPR和当前可用sector号 */
    plfs->LFS_availableSector++;
    (plfs->LFS_cpr)->free_sec[plfs->LFS_curBlockNo] = plfs->LFS_availableSector;

    if(plfs->LFS_availableSector==NOR_FLASH_SECTPBLK) {
        next_block_no = findNextCleanBlock(plfs);
        if (next_block_no == PX_ERROR)
        {
            printk("lfs: all blocks have been dirty.\r\n");
            plfs->LFS_availableSector = 0; 
            return PX_ERROR;

        } 

        plfs->LFS_curBlockNo = next_block_no;
        plfs->LFS_availableSector = (plfs->LFS_cpr)->free_sec[next_block_no];   
        
    }
    return sector_to_write;
}


/* 
    将当前BLK_BUF内容写回BLOCK_NO指定的block中
    modified
*/
VOID writeOutBlock(PLFS_VOLUME plfs, UINT block_no)
{
    UINT next_block_no;
    write_nor(GET_NOR_OFFSET_FROM_BLKN(block_no),
                (PCHAR)plfs->LFS_blkBuf,
                NOR_FLASH_BLKSZ);
    
    if(block_no == CPR_FN_1_BLK || block_no == CPR_FN_2_BLK) {
        /* 如果只是修改FileNameMap的两个块，就不需要将块标记为脏，也不需要找下一个干净块 */
        return;
    }

    plfs->LFS_cleanBlock[block_no] = BLK_DIRTY;
    (plfs->LFS_cpr)->free_sec[block_no] = 0;        /* 写入的block，其free_sector重置为0 */

    next_block_no = findNextCleanBlock(plfs);
    if (next_block_no == PX_ERROR)
    {
        printk("lfs: all blocks have been dirty.\r\n");
        plfs->LFS_availableSector = 0; 
        return ;

    } 
    
    /* 
        写入block一般是该block满了，所以要找下一个block。
        于是就要将AVAILABLE_SECTOR重置为0。
     */
    plfs->LFS_curBlockNo = next_block_no;
    plfs->LFS_availableSector = (plfs->LFS_cpr)->free_sec[next_block_no];   
}

/*
    寻找下一个Inode Number，通过FileMap来查找
    modified
*/
UINT nextInodeNumber(PLFS_VOLUME plfs)
{
    UINT i;
    /* 先查找CPR_FN_1_BLK */
    readInBlock(plfs, CPR_FN_1_BLK);
    for (i = 1; i < FN_ENTRY_PBLK; i++)
    {
        if(!(plfs->LFS_blkBuf[i*FN_ENTRY_SZ]))
            return (i-1);
    }
    /* 再查找CPR_FN_2_BLK */
    readInBlock(plfs, CPR_FN_2_BLK);
    for (i = 0; i < FN_ENTRY_PBLK; i++)
    {
        if((!plfs->LFS_blkBuf[i*FN_ENTRY_SZ]))
            return (i+511);
    }
    return UINT_MAX;
}

/* 
    向LFS添加新的inode，如果inode存在，则更新imap中相应的位置。
    modified
*/
INT writeInode(PLFS_VOLUME plfs, const PLFS_INODE plfsn, UINT inode_no)
{
    UINT inode_in_sector;                                           /* inode在sector中的位置 */
    UINT global_inode_sector_no;                                    /* inode的sector的全局位置 */
    UINT global_inode_sector_no_last = plfs->LFS_imap[inode_no];   /* 如果inode之前存在，则表示上一次存在的sector号 */
    //inode 写入sector
    inode_in_sector = inode_no%INODE_PSEC;

    /* 如果inode存在，summary中有效位置为-1，表示无效（和summary及imap不同） */
    if(global_inode_sector_no_last!=(UINT)-1) {
        plfs->LFS_blockSummary[global_inode_sector_no_last][0] = (UINT)-1;
    }


    lib_memcpy(&(plfs->LFS_secBuf[inode_in_sector*sizeof(LFS_INODE)]),
                (PCHAR)plfsn,
                sizeof(LFS_INODE));
    global_inode_sector_no = writeOutSector(plfs);

    if (global_inode_sector_no == (UINT)-1)
    {
        return (PX_ERROR);
    }
    

    /* 更新当前 block 对应的 summary */
    plfs->LFS_blockSummary[global_inode_sector_no][0] = inode_no;
    plfs->LFS_blockSummary[global_inode_sector_no][1] = SUMMARY_INODE_TYPE;

    /* 更新imap */
    if(updateImap(plfs,inode_no,global_inode_sector_no)<0) {
        return (PX_ERROR);
    }

    /* 修改FileNameMap */
    if(updateFilemap(plfs,inode_no,plfsn->LFSN_pcname)<0) {
        return (PX_ERROR);
    }
    
    /* 更新summary sector */
    if(updateSummary(plfs)<0) {
        return (PX_ERROR);
    }

    /* 更新CPR */
    if(updateCheckpointRegion(plfs)<0) {
        return (PX_ERROR);
    }
    return (ERROR_NONE);
}

/* 更新Filemap
  @inode_number：inode号
  @lfs_filename：更新文件名，为NULL时则是删除该文件名

  因为每个Filemap就保存了Valid Byte与Name Bytes
    modified
*/
INT updateFilemap(PLFS_VOLUME plfs, UINT inode_number, PCHAR lfs_filename)
{
    UINT    i;
    PCHAR   temp;
    BOOL    bRemove = (lfs_filename)?LW_FALSE:LW_TRUE;
    /* 根据inode_number决定读取哪一个block */
    if(inode_number<511&&inode_number>=0)
    {
        readInBlock(plfs, CPR_FN_1_BLK);
        temp = &(plfs->LFS_blkBuf[(inode_number+1)*FN_ENTRY_SZ]);
        i = CPR_FN_1_BLK;
    }
    else if(inode_number>=511&&inode_number<MAX_FILE)
    {
        readInBlock(plfs, CPR_FN_2_BLK);
        temp = &(plfs->LFS_blkBuf[(inode_number-511)*FN_ENTRY_SZ]);
        i = CPR_FN_2_BLK;
    }
    else
    {
        return PX_ERROR;
    }
    if(bRemove == LW_FALSE) {
        *temp = 1;
        temp++;
        lib_strcpy(temp, lfs_filename);        
    } else {
        *temp = 0;
    }

    writeOutBlock(plfs,i);
    return ERROR_NONE;
}

/*
    将新的imap写入新的sector，并修改内存中block summary
    更新CPR.imap_sec
    @inode_number：     inode序号
    @sector_position：  imap保存新的sector全局序号
    啊，因为一个sector中保存着16个inode所以修改一个，就有可能要修改其他15个的inode位置
    modified
*/
INT updateImap(PLFS_VOLUME plfs, UINT inode_number, UINT sector_position)
{
    UINT    old_imap_sector         = (plfs->LFS_cpr)->imap_sec;
    UINT    head_inode_in_sector    = (inode_number/INODE_PSEC)*INODE_PSEC;
    UINT    i;
    if (sector_position<2*NOR_FLASH_SECTPBLK||sector_position>=NOR_FLASH_NSECTOR)
        return PX_ERROR;
    for ( i = head_inode_in_sector; i < INODE_PSEC; i++)
    {   /* 如果同在一个sector中的其他inode存在，则也要修改他们的imap项 */
        if(plfs->LFS_imap[i]!=(UINT)-1) {
            plfs->LFS_imap[i] = sector_position;
        }
    }
    /* 注意，传进来的inode的imap项无论之前存不存在，都是必然修改的 */
    plfs->LFS_imap[inode_number] = sector_position;

    /* 更新CPR */
    (plfs->LFS_cpr)->imap_sec = plfs->LFS_availableSector + plfs->LFS_curBlockNo*NOR_FLASH_SECTPBLK;
    
    /* 更新block summary */
    if(old_imap_sector != (UINT)-1) {
        plfs->LFS_blockSummary[old_imap_sector][0] = 0;
        //plfs->LFS_blockSummary[old_imap_sector][1] = 0;
    }

    plfs->LFS_blockSummary[(plfs->LFS_cpr)->imap_sec][0] = (UINT) -1;
    plfs->LFS_blockSummary[(plfs->LFS_cpr)->imap_sec][1] = SUMMARY_IMAP_TYPE;

    /* IMAP回写到SECOTR_BUF的AVAILABLE_SECTOR中 */
    lib_memcpy(plfs->LFS_secBuf,
                (PCHAR)plfs->LFS_imap,
                NOR_FLASH_SECTORSZ);

    /* 回写 imap sector */
    writeOutSector(plfs);

    return ERROR_NONE;
}

/*
    将新的 block summary 写入新的 sector
    更新CPR.summary_sec
    modified
*/
INT updateSummary(PLFS_VOLUME plfs)
{

    UINT old_summary_sector = (plfs->LFS_cpr)->summary_sec;
    /* 更新CPR */
    (plfs->LFS_cpr)->summary_sec = plfs->LFS_availableSector + plfs->LFS_curBlockNo*NOR_FLASH_SECTPBLK;

    /* 更新block summary */
    if(old_summary_sector != (UINT) -1) {
        plfs->LFS_blockSummary[old_summary_sector][0] = 0;
        //plfs->LFS_blockSummary[old_summary_sector][1] = 0;
    }

    plfs->LFS_blockSummary[(plfs->LFS_cpr)->summary_sec][0] = (UINT) -1;
    plfs->LFS_blockSummary[(plfs->LFS_cpr)->summary_sec][1] = SUMMARY_SUMMARY_TYPE;

    lib_memcpy(plfs->LFS_secBuf,
                (PCHAR)plfs->LFS_blockSummary,
                NOR_FLASH_SECTORSZ);   

    /* 回写 imap sector */
    writeOutSector(plfs); 
    return ERROR_NONE;    
}


/*
    获取inode，返回inode指针。如果失败，返回Null。
    @inode_number：inode序号
    modified
*/
PLFS_INODE getInodeFromInumber(PLFS_VOLUME plfs,
                                UINT inode_number)
{
    UINT sector_position = plfs->LFS_imap[inode_number];
    /* sector相对block位置 */
    UINT local_sector_pos = sector_position%NOR_FLASH_SECTPBLK;
    /* 获取inode结构体在sector中相对偏移，以字节为单位 */
    UINT local_inode_pos = (inode_number%INODE_PSEC)*sizeof(LFS_INODE);
    PLFS_INODE meta;
    read_content_t content;
    PCHAR temp;

    if(inode_number > MAX_FILE || sector_position == (UINT)-1){
        return LW_NULL;
    }

    meta = (PLFS_INODE)__SHEAP_ALLOC(sizeof(LFS_INODE));
    if (meta==LW_NULL)
    {
        _ErrorHandle(ENOMEM);
        return  LW_NULL;
    }

    /* 检查是否在内存中，（blkBuf会改变，不成立了） */
    //if(BLOCK_NO==block_location)
    if(LW_FALSE)
    {
        lib_memcpy(meta,
                    ((PCHAR)&plfs->LFS_blkBuf[local_sector_pos*NOR_FLASH_SECTORSZ])+local_inode_pos,
                    sizeof(LFS_INODE));
    }
    else
    {
        /* 如果不在内存，则直接从flash中读取 */
        content = read_nor(GET_NOR_OFFSET_FROM_SECN(sector_position),READ_SECTOR);
        if (content.is_success)
        {
            temp = content.content;
            lib_memcpy(meta,
                        (PCHAR)(temp+local_inode_pos),
                        sizeof(LFS_INODE));
        }
        else
            return LW_NULL;
    }
    return meta;
}

/*
    根据文件名获取inode序号
    modified
*/
UINT getInodeNumberOfFile(PLFS_VOLUME plfs, PCHAR pcname)
{
    UINT i;
    PCHAR temp;
    /* 先读取CPR_FN_1_BLK */
    readInBlock(plfs ,CPR_FN_1_BLK);
    for (i = 0; i < 511; i++)
    {
        temp = &(plfs->LFS_blkBuf[(i+1)*FN_ENTRY_SZ]);
        if (*temp)/* 检查有效位 */
        {
            temp++;
            if (lib_strcmp(temp,pcname)==0)
                return i;    
        }
    }
    /* 再读取CPR_FN_2_BLK */
    readInBlock(plfs ,CPR_FN_2_BLK);
    for (i = 0; i < 512; i++)
    {
        temp = &(plfs->LFS_blkBuf[(i+511)*FN_ENTRY_SZ]);
        if (*temp)/* 检查有效位 */
        {
            temp++;
            if (lib_strcmp(temp,pcname)==0)
                return (i+511);    
        }
    }  
    return UINT_MAX;  
}

/*
    根据inode序号复制文件名
    modified
*/

INT getFileNameOfInodeNumber(PLFS_VOLUME plfs, PCHAR pcname,UINT inode_number)
{
    PCHAR temp;
    if (pcname == LW_NULL)
    {
        _ErrorHandle(EFAULT);
        return (PX_ERROR);
    }

    if (inode_number>MAX_FILE)
    {
        return (PX_ERROR);
    }
    
    if(inode_number<511) {
        if(readInBlock(plfs ,CPR_FN_1_BLK)<0){
            return (PX_ERROR);
        }
        temp = &(plfs->LFS_blkBuf[(inode_number+1)*FN_ENTRY_SZ]);
    } else if (inode_number <MAX_FILE) {
        if(readInBlock(plfs ,CPR_FN_2_BLK)<0){
            return (PX_ERROR);
        }
        temp = &(plfs->LFS_blkBuf[(inode_number-511)*FN_ENTRY_SZ]);        
    }

    if(*temp==0)
        return (PX_ERROR);
    
    temp++;
    lib_strcpy(pcname,(CPCHAR)temp);
    return (ERROR_NONE);
}

/* 
    输出Sector
    @sector_no：需要输出的sector全局的位置
*/
VOID printSector(UINT sector_no)
{
    UINT block_location = GET_BLKN_OF_SECN(sector_no);
    UINT local_sector_pos = sector_no%NOR_FLASH_SECTPBLK;
    PCHAR temp;
    read_content_t content;
    UINT i;
    /* 如果在内存，则直接读取 */
    if (block_location==BLOCK_NO)
    {
        temp = &BLK_BUF[local_sector_pos*NOR_FLASH_SECTORSZ];
    }
    /* 否则用write_nor */
    else
    {
        read_nor(sector_no*NOR_FLASH_SECTORSZ,READ_SECTOR);
        if (content.is_success)
        {
            temp = content.content;
        }
        else
        {
            //_DebugHandle(__ERRORMESSAGE_LEVEL, "nor flash read fail.\r\n");
            return;
        }
    }
    for (i = 0; i < NOR_FLASH_SECTORSZ; i++)
    {
        printk("%c",*temp);
        temp++;
    }
    printk("\r\n");
}

/*
    lfs 打开一个文件
*/
PLFS_INODE __lfs_open(  PLFS_VOLUME     plfs,
                        PCHAR           pcName,
                        BOOL            *pbRoot)
{

    UINT        inode_no;
    PLFS_INODE  pinode;
    CHAR                pcTempName[MAX_FN_SZ];

    if (*pcName == PX_ROOT) {                                           /*  忽略根符号                  */
        lib_strlcpy(pcTempName, (pcName + 1), PATH_MAX);
    } else {
        lib_strlcpy(pcTempName, pcName, PATH_MAX);
    }

    if (pcTempName[0] == PX_EOS) {
        if (pbRoot) {
            *pbRoot = LW_TRUE;                                          /*  pcName 为根                 */
        }
        return  (LW_NULL);
    
    } else {
        if (pbRoot) {
            *pbRoot = LW_FALSE;                                         /*  pcName 不为根               */
        }
    }

    inode_no = getInodeNumberOfFile(plfs, pcTempName);
    if (inode_no == UINT_MAX)
    {
        return LW_NULL;
    }

    pinode = getInodeFromInumber(plfs, inode_no);
    return pinode;
}
/*
    lfs 创建一个文件
*/
PLFS_INODE __lfs_maken( PLFS_VOLUME     plfs,
                        PCHAR           pcname,
                        mode_t          mode)
{
    PLFS_INODE  plfsn = (PLFS_INODE)__SHEAP_ALLOC(sizeof(LFS_INODE));
    CPCHAR      pcFileName;
    UINT        inode_no;
    INT         i;
    if(plfs->LFS_availableFile>0) {
        plfs->LFS_availableFile --;
    } else {
        printk("no more file can be created.\r\n");
        return LW_NULL;
    }

    inode_no = nextInodeNumber(plfs);
    if (inode_no == UINT_MAX)
    {
        return LW_NULL;
    }
    

    if(plfsn == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (LW_NULL);        
    }
    lib_bzero(plfsn,sizeof(LFS_INODE));


    pcFileName = lib_rindex(pcname, PX_DIVIDER);            /* 这里考虑到多级目录,把pcFileName指向"\"后一个字符*/
    if (pcFileName) {                                       /* 虽然本系统暂不支持多级目录，但还是保留 */
        pcFileName++;
    } else {
        pcFileName = pcname;
    }    

    if ((mode & S_IFMT) == 0) {
        mode |= S_IFREG;
    }

    plfsn->LFSN_pcname = (PCHAR)__SHEAP_ALLOC(MAX_FN_SZ);
    if(plfsn->LFSN_pcname == LW_NULL) {
        _ErrorHandle(ENOMEM);
        return  (LW_NULL);        
    }
    lib_strcpy(plfsn->LFSN_pcname,pcFileName);

    plfsn->LFSN_inodeNo     = inode_no;
    plfsn->LFSN_stSize      = 0;
    plfsn->LFSN_stVSize     = 0;
    plfsn->LFSN_mode        = mode;
    plfsn->LFSN_timeCreate  = lib_time(LW_NULL);
    plfsn->LFSN_timeAccess  = plfsn->LFSN_timeCreate;
    plfsn->LFSN_timeChange  = plfsn->LFSN_timeCreate;
    plfsn->LFSN_uid         = getuid();
    plfsn->LFSN_gid         = getgid();

    

    for(i = 0; i<MAX_DATA_SECTOR;i++)
        plfsn->LFSN_sectorLocation[i] = (UINT) -1;
    
    return  (plfsn);
}

/*
    lfs 获取文件状态
*/
VOID __lfs_stat(PLFS_INODE plfsn, PLFS_VOLUME plfs, struct stat  *pstat)
{
    INT     i;
    if(plfsn) {
        pstat->st_dev     = LW_DEV_MAKE_STDEV(&plfs->LFS_devhdrHdr);
        pstat->st_ino     = (ino_t)plfs;
        pstat->st_mode    = plfsn->LFSN_mode;
        pstat->st_nlink   = 1;
        pstat->st_uid     = plfsn->LFSN_uid;
        pstat->st_gid     = plfsn->LFSN_gid;
        pstat->st_rdev    = 1;
        pstat->st_size    = (off_t)plfsn->LFSN_stSize;
        pstat->st_atime   = plfsn->LFSN_timeAccess;
        pstat->st_mtime   = plfsn->LFSN_timeChange;
        pstat->st_ctime   = plfsn->LFSN_timeCreate;
        pstat->st_blksize = NOR_FLASH_SECTORSZ;

        for ( i = 0; i < MAX_DATA_SECTOR; i++)
        {
            if(plfsn->LFSN_sectorLocation[i]!=(UINT)-1)
                pstat->st_blocks++;
        }

    } else {
        pstat->st_dev     = LW_DEV_MAKE_STDEV(&plfs->LFS_devhdrHdr);
        pstat->st_ino     = (ino_t)0;
        pstat->st_mode    = plfs->LFS_mode;
        pstat->st_nlink   = 1;
        pstat->st_uid     = plfs->LFS_uid;
        pstat->st_gid     = plfs->LFS_gid;
        pstat->st_rdev    = 1;
        pstat->st_size    = 0;
        pstat->st_atime   = plfs->LFS_time;
        pstat->st_mtime   = plfs->LFS_time;
        pstat->st_ctime   = plfs->LFS_time;
        pstat->st_blksize = NOR_FLASH_SECTORSZ;
        pstat->st_blocks  = 0;        
    }

    pstat->st_resv1 = LW_NULL;
    pstat->st_resv2 = LW_NULL;
    pstat->st_resv3 = LW_NULL;
}

/*
    LFS 删除一个文件
*/
VOID __lfs_close(PLFS_INODE plfsn, INT iFlag) 
{
    plfsn->LFSN_timeAccess = lib_time(LW_NULL);

    // O_ACCMODE 指的是只读、只写、读写其中一个
    if (plfsn->LFSN_bChange && (iFlag & O_ACCMODE) != O_RDONLY) {
        plfsn->LFSN_timeChange = plfsn->LFSN_timeAccess;
    }
}

/*
    LFS 删除一个文件
*/
INT __lfs_unlink(PLFS_VOLUME plfs, PLFS_INODE plfsn)
{
    INT     i;
    UINT    data_sector_no;
    UINT    inode_no;

    if(plfsn == NULL)
        return (PX_ERROR);

    inode_no = plfsn->LFSN_inodeNo;
    
    /* 只有根目录为目录类型，其他的为文件类型*/
    if (S_ISDIR(plfsn->LFSN_mode) 
        && plfs->LFS_availableFile != MAX_FILE)
    {
        _ErrorHandle(ENOTEMPTY);
        return  (PX_ERROR);        
    }

    /* 删除文件本身 */
    /* 更新inode */

    for(i = 0 ; i<MAX_DATA_SECTOR; i++)
    {
        if (plfsn->LFSN_sectorLocation[i] != -1)
        {
            data_sector_no = plfsn->LFSN_sectorLocation[i];
            /* 修改inode数据列表 */
            plfsn->LFSN_sectorLocation[i] = -1;
            /* 针对数据sector修改summary */
            plfs->LFS_blockSummary[data_sector_no][0] = 0;
            plfs->LFS_blockSummary[data_sector_no][1] = 0;
        }
    } 
    /* 修改imap */
    updateImap(plfs, inode_no, (UINT)-1);

    /* 修改fileNameMap */
    updateFilemap(plfs, inode_no, LW_NULL);

    /* 更新相关数据 */
    plfs->LFS_availableFile ++;
    plfs->LFS_inodeMap[inode_no] = 0;
    
    /* 释放空间 */
    __SHEAP_FREE(plfsn->LFSN_pcname);
    __SHEAP_FREE(plfsn);

    return (ERROR_NONE);
}

/*
    LFS 卸载
    实际时递归删除文件系统中所有目录下的所有文件
    由于LFS不支持目录项，这里就是删除所有文件即可
*/
VOID __lfs_unmount(PLFS_VOLUME plfs)
{
    UINT         i;
    PLFS_INODE  plfsn;
    for (i = 0; i<MAX_FILE ; i++) {
        plfsn = getInodeFromInumber(plfs,i);
        __lfs_unlink(plfs, plfsn);
    }
}

/*
    LFS 写入文件内容
     输　入  :  pramn            文件节点
                pvBuffer         缓冲区
                stNBytes         需要写入的大小
                stOft            偏移量
    写入过程：
        data_sector -> inode -> imap ->summary -> CPR
*/
ssize_t  __lfs_write (PLFS_VOLUME plfs, PLFS_INODE  plfsn, CPVOID  pvBuffer, size_t  stNBytes, size_t  stOft)
{
    UINT8          *pucDest = (UINT8 *)pvBuffer;
    size_t          stEnd   = stOft + stNBytes;
    size_t          stWrite = 0;
    size_t          stStart;
    UINT            NSector;                    /* 写完后总共的sector */
    UINT            curSectorIndex;             /* 当前要写入文件的第几个Sector*/
    UINT            curSector;                  /* 当前已写入文件的Sector全局序号 */

    NSector = (ULONG)(stEnd / NOR_FLASH_SECTORSZ);
    
    if (stEnd % NOR_FLASH_SECTORSZ) {
        NSector++;
    }

    /* 
        完成此次写入需要的总块数大于单个文件最大块数
        或超过了整个文件系统的总块数，则会报错
     */
    if ((NSector > MAX_DATA_SECTOR)||
        (plfs->LFS_ulCurSector+NSector)>plfs->LFS_ulMaxSector)
    {
        return (0);        
    }
    
    stStart = stOft % NOR_FLASH_SECTORSZ;
    curSectorIndex = stOft / NOR_FLASH_SECTORSZ;

    while (stNBytes)
    {
        size_t  stBufSize = (NOR_FLASH_SECTORSZ - stStart);
        /* 先检查是否会覆盖原有数据 */
        if(curSectorIndex<plfsn->LFSN_Cnt) {
            if(readInSector(plfs,plfsn->LFSN_sectorLocation[curSectorIndex])<0) {
                goto __lfs_write_end;
            }
        }

        if (stBufSize >= stNBytes) { /* 写最后一块 */   
            lib_memcpy(&plfs->LFS_secBuf[stStart], pucDest, stNBytes);
            /* 回写sector并更新summary */
            curSector = writeOutSector(plfs);
            if(curSector<0) {
                goto __lfs_write_end;
            }
            plfsn->LFSN_sectorLocation[curSectorIndex] = curSector;
            if(stStart == 0)
                plfsn->LFSN_Cnt++;

            plfs->LFS_blockSummary[curSector][0] =  (UINT) -1;
            plfs->LFS_blockSummary[curSector][1] = plfsn->LFSN_inodeNo;

            stWrite += stNBytes;
            break;
        } else {/* 写一整块 */
            lib_memcpy(&plfs->LFS_secBuf[stStart], pucDest, stBufSize);
            /* 回写sector并更新summary */
            curSector = writeOutSector(plfs);
            if(curSector<0) {
                goto __lfs_write_end;
            }
            plfsn->LFSN_sectorLocation[curSectorIndex] = curSector;
            plfsn->LFSN_Cnt++;

            plfs->LFS_blockSummary[curSector][0] =  (UINT) -1;
            plfs->LFS_blockSummary[curSector][1] = plfsn->LFSN_inodeNo; 

            pucDest     += stBufSize;
            stWrite     += stBufSize;
            stNBytes    -= stBufSize;
            stStart      = 0;
            curSectorIndex++;
        }
    }
    
    if (plfsn->LFSN_stSize < (stOft + stWrite)) {
        plfsn->LFSN_stSize = (stOft + stWrite);
        if(plfsn->LFSN_stVSize < plfsn->LFSN_stSize) {
            plfsn->LFSN_stVSize = plfsn->LFSN_stSize;
        }
    }

__lfs_write_end:
    writeInode(plfs, plfsn, plfsn->LFSN_inodeNo);
    return  ((ssize_t)stWrite);
}

/*********************************************************************************************************
** 函数名称: __lfs_read
** 功能描述: ramfs 读取文件内容
** 输　入  : pramn            文件节点
**           pvBuffer         缓冲区
**           stSize           缓冲区大小
**           stOft            偏移量
** 输　出  : 读取的字节数
** 全局变量:
** 调用模块:
*********************************************************************************************************/

ssize_t __lfs_read(PLFS_VOLUME plfs, PLFS_INODE plfsn, PVOID pvBuffer, size_t stSize, size_t stOft)
{
    UINT8          *pucDest = (UINT8 *)pvBuffer;
    size_t          stDataLeft;
    size_t          stNBytes;
    size_t          stRead  = 0;
    size_t          stStart;
    UINT            curSectorIndex;

    /*
        stOft是文件当前指针，如果文件指针超过了文件大小，则不会读入任何东西。
    */
    if( plfsn->LFSN_stVSize <= stOft) {
        return (0);
    }

    stDataLeft = plfsn->LFSN_stVSize - stOft; 
    stNBytes   = __MIN(stDataLeft, stSize);
    
    stStart = stOft % NOR_FLASH_SECTORSZ;
    curSectorIndex = stOft / NOR_FLASH_SECTORSZ;
    do {
        size_t stBufSize = (NOR_FLASH_SECTORSZ - stStart);
        if(stBufSize >= stNBytes) {
            if(readInSector(plfs,plfsn->LFSN_sectorLocation[curSectorIndex])<0) {
                return ((ssize_t)stRead);
            }
            lib_memcpy(pucDest, &plfs->LFS_secBuf[stStart], stNBytes);
            stRead += stNBytes;
            break;
        } else {
            if(readInSector(plfs,plfsn->LFSN_sectorLocation[curSectorIndex])<0) {
                return ((ssize_t)stRead);
            }
            lib_memcpy(pucDest, &plfs->LFS_secBuf[stStart], stBufSize);
            pucDest     += stBufSize; 
            stRead      += stBufSize;
            stNBytes    -= stBufSize; 
            stStart      = 0;  
            curSectorIndex++;    
        }
    } while (stNBytes);

    return  ((ssize_t)stRead);
}

/*
**  LFS 移动或者重命名一个文件
**  由于LFS没有子目录系统，所以这个函数只能用于重命名
**  注意，无法重命名文件系统的根目录
** 输　入  : plfsn            文件节点
**           pcNewName        新的名字
** 输　出  : ERROR    
*/
INT __lfs_move (PLFS_VOLUME plfs, PLFS_INODE plfsn, PCHAR pcNewName)
{
    PLFS_INODE      plfsnTemp;
    BOOL            bRoot;
    PCHAR           pcFileName;

    plfsnTemp = __lfs_open(plfs, pcNewName, &bRoot);
    /* 只要文件为根节点，直接报错 */
    if (!plfsnTemp) {
        if(bRoot) {
            _ErrorHandle(EINVAL);
            return  (PX_ERROR);
        }
    }

    /* 文件原名和新名一样，直接返回 */
    if (plfsn == plfsnTemp) {                                           /*  相同                        */
        return  (ERROR_NONE);
    }    

    /* 从多级目录路径中获取文件名，本LFS不支持目录文件，这部分也可以忽略 */
    pcFileName = lib_rindex(pcNewName, PX_DIVIDER);
    if (pcFileName) {
        pcFileName++;
    } else {
        pcFileName = pcNewName;
    }

    return updateFilemap(plfs, plfsn->LFSN_inodeNo, pcFileName);
}
