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
** 文   件   名: fake_nor.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 02 月 24 日
**
** 描        述: RAM模拟NorFlash驱动
*********************************************************************************************************/

#include "fake_nor.h"
#include "fake_nor_cmd.h"

BOOL get_sector_is_bad(INT sectro_no){
    BOOL is_bad;
    API_ThreadLock();
    is_bad = sector_infos[sectro_no].is_bad;
    API_ThreadUnlock();
    return is_bad;
}

VOID assign_sector_bad(INT sectro_no){
    API_ThreadLock();
    sector_infos[sectro_no].is_bad = TRUE;
    API_ThreadUnlock();
}
/*********************************************************************************************************
** 函数名称: generate_bad_sector
** 功能描述: 自动检测坏块线程，当某个sector的擦写次数到达最大擦写次数的70%，那么就有70%的概率变为坏块，
            达到100%，就有100%的几率变为坏块。
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID generate_bad_sector(){
    UINT i;

    API_ThreadLock();
    printf("[generating bad sector...]\n");
    API_ThreadUnlock();
    
    while (TRUE)
    {
        for (i = 0; i < NOR_FLASH_NSECTOR; i++)
        { 
            if(((float)NOR_FLASH_MAX_ERASE_CNT * 0.7 - sector_infos[i].erase_cnt) <= 0      /* 擦了最大次数的70% */
                && sector_infos[i].is_bad == FALSE) {                                       /* 且该Sector不是坏块 */
                srand(time(LW_NULL));
                INT percent = rand() % 170 + 70;                                            /* 产生70 - 170的随机数 */
                INT threshold = 70 * NOR_FLASH_MAX_ERASE_CNT / sector_infos[i].erase_cnt;   /* 70 * 10 / 7 = 100 ~ 70 */
                if(percent >= threshold) {                                                  /* 70% ~ 100%的概率变为坏块 */
                    assign_sector_bad(i);
                }
            }
        }    
        sleep(1);                                                                           /* 每1s检测1次 */
    }
}
/*********************************************************************************************************
** 函数名称: nor_init
** 功能描述: 初始化fake nor flash
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_init(void){
    if((nor_flash_base_addr = (PCHAR)malloc(NOR_FLASH_SZ)) < 0){
        printf("[Low Memory] can't create nor flash\n");
        return;
    }
    lib_memset(nor_flash_base_addr, 0, NOR_FLASH_SZ);
    if((sector_infos = (sector_info_t *)malloc(NOR_FLASH_NSECTOR * sizeof(sector_info_t))) < 0){
        printf("[Low Memory] can't create sector summary\n");
        return;
    }
    lib_memset(sector_infos, 0, NOR_FLASH_NSECTOR * sizeof(sector_info_t));
    printf("base addr: %p; sector_infos: %p\n", nor_flash_base_addr, sector_infos);
    
    INT i;
    for (i = 0; i < NOR_FLASH_NSECTOR; i++)
    {
        sector_infos[i].erase_cnt = 0;
        sector_infos[i].is_bad = FALSE;
    }
    pretty_print("[nor init success]", "use 'fls -h' to get help");

    API_ThreadCreate("t_generate_bad_block",
                     (PTHREAD_START_ROUTINE)generate_bad_sector,
                     LW_NULL,
                     LW_NULL);                                          /*  Create generate bad block thread */     
    register_nor_cmd();
}
/*********************************************************************************************************
** 函数名称: reset_nor
** 功能描述: 重置nor flash
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID reset_nor(){
    CHAR temp[40] = {'\0'};
    SHOW_DIVIDER();
    lib_memset(nor_flash_base_addr, 0, NOR_FLASH_SZ);
    sprintf(temp, "%dB", NOR_FLASH_SZ);
    pretty_print("[norflash reset]:", temp);
    
    lib_memset(temp, 0, 40);
    lib_memset(sector_infos, 0, NOR_FLASH_NSECTOR * sizeof(sector_info_t));
    sprintf(temp, "#%d", NOR_FLASH_NSECTOR);
    pretty_print("[reset sectors]:", temp);

    pretty_print("[reset success]", "");
    SHOW_DIVIDER();
}

/*********************************************************************************************************
** 函数名称: erase_nor
** 功能描述: 根据选项擦除nor flash的一部分
** 输　入  : offset        偏移地址
**           ops          选项，可以是ERASE_SECTOR|ERASE_BLOCK|ERASE_CHIP
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID erase_nor(UINT offset, ERASE_OPTIONS ops){
    PCHAR addr = GET_NOR_FLASH_ADDR(offset);
    INT i;
    if(addr < nor_flash_base_addr){
        printf("[ERASE: target address can't lower than base]\n");
        return;
    }
    if(addr > nor_flash_base_addr + NOR_FLASH_SZ){
        printf("[ERASE: target address can't out bound of flash]\n");
        return;
    }
    switch (ops)
    {
    case ERASE_CHIP:
        printf("[erasing the whole chip...]\n");
        lib_memset(nor_flash_base_addr, 0, sizeof(CHAR) * NOR_FLASH_SZ);        /* 清空整段内存 */
        for (i = 0; i < NOR_FLASH_NSECTOR; i++)                                 /* 擦除次数增加 */
        {
            sector_infos[i].erase_cnt++;
        }
        printf("[erasing success]\n");
        break;

    case ERASE_BLOCK:{
        UINT block_no = GET_NOR_FLASH_BLK(offset);                              /* 获取offset对应的block号 */
        UINT sector_no = GET_NOR_FLASH_SECTOR(offset);                          /* 计算block对应的sector基本位置 */
        printf("[erasing block %d...]\n", block_no);    
        PCHAR block_addr = GET_NOR_FLASH_BLK_ADDR(block_no);                    /* 获取block对应的地址 */
        lib_memset(block_addr, 0, NOR_FLASH_BLKSZ);                             /* 擦除block首地址对应的block */
        for (i = 0; i < NOR_FLASH_SECTPBLK; i++)                                /* 擦除次数增加 */
        {
            sector_infos[i + sector_no].erase_cnt++;
        }
        printf("[erasing success]\n");
        break;
    }

    case ERASE_SECTOR:{
        UINT sector_no = GET_NOR_FLASH_SECTOR(offset);
        printf("[erasing sector %d]\n", sector_no);
        PCHAR sector_addr = GET_NOR_FLASH_SECTOR_ADDR(sector_no);
        lib_memset(sector_addr, 0, NOR_FLASH_SECTORSZ);
        sector_infos[sector_no].erase_cnt++;
        printf("[erasing success]\n");
        break;
    }
    default:
        break;
    }
}
/*********************************************************************************************************
** 函数名称: read_nor
** 功能描述: 根据选项从offset处读取内容
** 输　入  : offset         首部偏移
**           ops            读取选项，可以是READ_BYTE|READ_SECTOR
** 输　出  : 读取内容的结构体read_content_t
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
read_content_t read_nor(UINT offset, READ_OPTIONS ops){
    PCHAR addr = GET_NOR_FLASH_ADDR(offset);
    read_content_t result;
    if(addr < nor_flash_base_addr){
        printf("[READ: target address can't lower than base]\n");
        result.is_success = FALSE;
        return result;
    }
    if(addr > nor_flash_base_addr + NOR_FLASH_SZ){
        printf("[READ: target address can't out bound of flash]\n");
        result.is_success = FALSE;
        return result;
    }
    switch (ops)
    {
    case READ_BYTE:                                                     /* 读取1个字节 */
        printf("READ BYTE\n");
        result.content = addr;
        result.size = 1;
        break;
    case READ_SECTOR:           
        printf("READ SECT\n");                                          /* 读取1个Sector */
        UINT sector_no = GET_NOR_FLASH_SECTOR(offset);
        result.content = GET_NOR_FLASH_SECTOR_ADDR(sector_no);
        result.size = NOR_FLASH_SECTORSZ;
        break;
    default:
        break;
    }
    return result;
}
/*********************************************************************************************************
** 函数名称: write_nor
** 功能描述: 根据选项在offset处写入长度为size的内容
** 输　入  : offset         首部偏移
**           content        内容首部指针
             size           内容大小
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID write_nor(UINT offset, PCHAR content, UINT size){
    PCHAR addr = GET_NOR_FLASH_ADDR(offset);
    INT i;
    if(addr < nor_flash_base_addr){
        printf("[WRITE: target address can't lower than base]\n");
        return;
    }
    if(addr + size > nor_flash_base_addr + NOR_FLASH_SZ){                             /* 不能写超了 */
        printf("[WRITE: low memory]\n");
        return;
    }
    INT start_sector_no = GET_NOR_FLASH_SECTOR(offset);
    INT end_sector_no = ((offset + size) % NOR_FLASH_SECTORSZ) == 0 ?                     /* 判断是不是整数 */
                                    GET_NOR_FLASH_SECTOR(offset + size) - 1 : 
                                    GET_NOR_FLASH_SECTOR(offset + size)     ;
    INT nsectors = end_sector_no - start_sector_no + 1;                                   /* 需要擦除的sector数 */
    PCHAR start_sector_addr = GET_NOR_FLASH_SECTOR_ADDR(start_sector_no);                 /* 获得起始sector地址 */
    PCHAR buffer;
    if((buffer = (PCHAR)malloc(nsectors * NOR_FLASH_SECTORSZ)) < 0){              /* 回收器 */
        printf("[Buffer Init Failed]\n");
        return;
    }                      
    INT start_offset = addr - start_sector_addr;
    for (i = 0; i < nsectors; i++)                                                    /* 先读取 */
    {
        PCHAR sector_addr = GET_NOR_FLASH_SECTOR_ADDR(start_sector_no + i);
        UINT sector_offset = sector_addr - nor_flash_base_addr;
        read_content_t content = read_nor(sector_offset, READ_SECTOR);
        lib_memcpy(buffer + i * NOR_FLASH_SECTORSZ, content.content, content.size);
    }
    lib_memcpy(buffer + start_offset, content, size);
    for (i = 0; i < nsectors; i++)                                                    /* 再擦除 */
    {
        PCHAR sector_addr = GET_NOR_FLASH_SECTOR_ADDR(start_sector_no + i);
        erase_nor(sector_addr - nor_flash_base_addr, ERASE_SECTOR);
    }
    for (i = 0; i < nsectors; i++)                                                    /* 最后写入 */
    {
        UINT sector_offset = i * NOR_FLASH_SECTORSZ;
        PCHAR p = buffer + sector_offset;
        if(get_sector_is_bad(i)){                                                     /* 随机修改 */
            printf("[sector #%d is bad, there may be some error(s), remember to check]\n", start_sector_no + i);
            PCHAR pe = p + NOR_FLASH_SECTORSZ;
            for (; p < pe; p++)
            {
                INT possibily = rand() % 100 + 1;
                INT random_change = rand() % 127;                                      /* 0 ~ 127 ascii */
                if(possibily >= 50){                                                   /* 50%的几率写错 */
                    *p += random_change;
                }
            } 
        }
        lib_memcpy(start_sector_addr + sector_offset, p, NOR_FLASH_SECTORSZ);
    }
    free(buffer);
    printf("[write %d bytes success]\n", size);
}








