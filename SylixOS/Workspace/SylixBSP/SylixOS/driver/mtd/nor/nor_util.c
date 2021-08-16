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
** 文   件   名: nor_util.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 11 日
**
** 描        述: NorFlash裸板驱动工具库
*********************************************************************************************************/
#include "nor.h"
#include "nor_util.h"
#include "fake_nor.h"

/*********************************************************************************************************
** 函数名称: write_word_to_mem
** 功能描述: 向虚拟内存写offset地址写Data
** 输　入  : base         norflash起始地址
**			 offset		  片内偏移
**			 data		  写入的数据
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID write_word_to_mem(UINT32 base, UINT32 offset, UINT32 data)
{
	volatile UINT16* p = (volatile UINT16*)(base + (offset << 1));
	*p = data;
}
/*********************************************************************************************************
** 函数名称: read_word_from_mem
** 功能描述: 从虚拟内存offset处读一个字
** 输　入  : base         norflash起始地址
**			 offset		  片内偏移
** 输　出  : 16位数据
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT16 read_word_from_mem(UINT32 base, UINT32 offset)
{
	volatile UINT16* p = (volatile UINT16*)(base + (offset << 1));
	return *p;
}
/*********************************************************************************************************
** 函数名称: read_word_from_mem
** 功能描述: 从虚拟内存offset处读一个字节
** 输　入  : base         norflash起始地址
**			 offset		  片内偏移
** 输　出  : 16位数据
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 read_byte_from_mem(UINT32 base, UINT32 offset){
	volatile UINT8* p = (volatile UINT8*)(base + offset);
	return *p;
}

/*********************************************************************************************************
** 函数名称: wait_ready
** 功能描述: 等待擦写操作完成
** 输　入  : base         norflash起始地址
**			 offset		  片内偏移
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID wait_ready(UINT32 base, UINT32 offset)
{
	if(IS_FAKE_MODE()){
		return;
	}
	else{
		UINT32 val;
		UINT32 pre;

		pre = read_word_from_mem(base, offset >> 1);
		val = read_word_from_mem(base, offset >> 1);
		while ((val & (1 << 6)) != (pre & (1 << 6)))			/* 比较 Q6 位 */
		{
#ifdef NOR_DEBUG
		// printf("pre: %d, val: %d\n", (val & (1 << 6)), (pre & (1 << 6)));
		// printf("pre: %d, val: %d\n", val, pre);
		// printf("pre: %u, val: %u\n", val, pre);
		// printf("pre: %x, val: %x\n", val, pre);
#endif  // NOR_DEBUG
			pre = val;
			val = read_word_from_mem(base, offset >> 1);		
		}
	}
	
}

/*********************************************************************************************************
** 函数名称: nor_command_unlock
** 功能描述: 用于解锁norflash擦写、读设备ID、厂商ID
** 输　入  : base         norflash起始地址
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_command_unlock(UINT32 base){
	if(IS_FAKE_MODE()){
		return;
	}
	else{
		write_word_to_mem(base, 0x555, 0xAA); 			/* 解锁 */
		write_word_to_mem(base, 0x2AA, 0x55); 
	}
}
/*********************************************************************************************************
** 函数名称: nor_reset
** 功能描述: 用于重置NorFlash命令
** 输　入  : base         norflash起始地址
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_reset(UINT32 base){
	if(IS_FAKE_MODE()){
		lib_memset((PVOID)NOR_FLASH_BASE, (INT)-1, NOR_FLASH_SZ);
		lib_memset((PVOID)sector_infos, 0, NOR_FLASH_NSECTOR * sizeof(sector_info_t));
		INT i;
		for (i = 0; i < NOR_FLASH_NSECTOR; i++)
		{
			sector_infos[i].erase_cnt = 0;
			sector_infos[i].is_bad = FALSE;
		}
	}
	else{
		write_word_to_mem(base, 0x0, 0xF0);	
	}
}

/*********************************************************************************************************
** 函数名称: nor_check_offset_range
** 功能描述: 查看offset是否位于芯片内
** 输　入  : base         norflash起始地址
** 			 offset		  片内偏移
**			 size_bytes   欲写入或读出的大小 
** 输　出  : TRUE or False
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
BOOL nor_check_offset_range(UINT32 base, UINT32 offset, UINT size_bytes){
	if(offset + size_bytes > NOR_FLASH_SZ || offset < base){
		return FALSE;
	}
	return TRUE;
}
/*********************************************************************************************************
** 函数名称: nor_check_modifiable_perm
** 功能描述: 查看offset是否合法，因为前226KB是Uboot区域，不能写入或擦除；
** 输　入  : offset		  片内偏移
** 输　出  : 可以进行修改TRUE， 不可进行修改False
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
BOOL nor_check_modifiable_perm(UINT32 offset){
	if(offset < NOR_FLASH_START_OFFSET){
		return FALSE;
	}
	return TRUE;
}
/*********************************************************************************************************
** 函数名称: nor_check_should_erase
** 功能描述: 查看本次写入操作是否需要擦除
** 原　理  : 基本原理 NorFlash可将1直接编程为0
**			a1...a8代表0或1
**			比如要写入字节byte_to_write： a1  a2  a3  ... a8
**			闪存上已有字节byte_in_flash： a1' a2' a3' ... a8'
**			则它们之间异或结果byte_diff： (a1 ^ a1') (a2 ^ a2') ... (a8 ^ a8')
**			
**			如此，我们便能够找到 将要写的 和 Flash上的字节的不同位所在地全部找出来，且不同位均为1
**			如果将要写的均是从1写为0，那么byte_diff和byte_to_write与的结果一定是0
** 输　入  : offset		  片内偏移
**			 content	  写入内容
**			 size_bytes	  写入字节大小
** 输　出  : 需要擦除TRUE ，不需要擦除False
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
BOOL nor_check_should_erase(UINT32 base, UINT offset, PCHAR content, UINT size_bytes){
	INT i;
	for (i = 0; i < size_bytes; i++)
	{
		UINT8 byte_in_flash = read_byte_from_mem(base, offset + i);
		UINT8 byte_to_write = (UINT8)(*(content + i));
		UINT8 byte_diff = byte_in_flash ^ byte_to_write;
#ifdef NOR_DEBUG
		if((byte_diff & byte_to_write) != 0){
			printf("byte_in_flash: %x\n", byte_in_flash);
			printf("byte_to_write: %x\n", byte_to_write);
			printf("byte_diff: %x\n", byte_diff);
			printf("byte_diff & byte_to_write: %d \n", byte_diff & byte_to_write);
		}
#endif // NOR_DEBUG
		if((byte_diff & byte_to_write) != 0){
			return TRUE;
		}
	}
	return FALSE;
}

/*********************************************************************************************************
** 函数名称: nor_write_buffer
** 功能描述: 将buffer中的内容写入norflash中，只允许写入不超过从相对sector的offset算起，sector剩余空间的内容，
** 输　入  : base         norflash起始地址
** 			 offset		  片内偏移
**			 content      写入的内容
**			 size_bytes   欲写入的大小 
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_write_buffer(UINT32 base, UINT offset, PCHAR content, UINT size_bytes){
	UINT8 sector_no = GET_SECTOR_NO(offset);
	UINT32 sector_start_offset = GET_SECTOR_OFFSET(sector_no);
	UINT sector_remain_size = GET_SECTOR_SIZE(sector_no) - (offset - sector_start_offset); 
	if(size_bytes > sector_remain_size){
#ifdef NOR_DEBUG
		pretty_print("[nor write buffer]", FAIL "size not permit", DONT_CENTRAL);
		printf("size_bytes: %d\n", size_bytes);
		printf("sector_remain_size: %d\n", sector_remain_size);
		printf("offset: %d\n", offset);
#endif // NOR_DEBUG
		return;
	}
	if(IS_FAKE_MODE()){
		PCHAR p = content;
		INT	  i, size_words;
		volatile INT fake;
		if(get_sector_is_bad(sector_no)){                                                          /* 随机修改 */
#ifdef NOR_DEBUG
			printf("| -> [sector #%d is bad, there may be some error(s), remember to check]\n", sector_no);
#endif // DEBUG
			PCHAR pe = p + size_bytes;
			for (; p < pe; p++)
			{
				INT possibily = rand() % 100 + 1;
				INT random_change = rand() % 127;                                          			/* 0 ~ 127 ascii */
				if(possibily >= 50){                                                       			/* 50%的几率写错 */
					*p += random_change;
				}
			} 
		}
		size_words = size_bytes / 2;
		for (i = 0; i < size_words; i++)
		{
			/* 模拟nor_cmd_unlock， 2个访存周期 */
			fake = 1;		/* Cycle 1 */
			fake = 2;		/* Cycle 2 */
			/* 模拟写入，2个访存周期 */
			fake = 3;		/* Cycle 3 */
			fake = 4;		/* Cycle 4 */
			/* 模拟wait_ready， 至少2个访存周期 */
			fake = 5;		/* Cycle 5 */
			fake = 6;		/* Cycle 6 */
		}
		lib_memcpy((PVOID)(base + offset), content, size_bytes);
	}
	else {
		UINT size_words = size_bytes / 2;
		INT remain_byte = size_bytes - size_words * 2 ;
		INT i;
#ifdef NOR_DEBUG
		printf("size_words: %d, remain_byte: %dB \n", size_words, remain_byte);
#endif // NOR_DEBUG
		for (i = 0; i < size_words; i++)
		{
			INT index = 2 * i;
			UINT32 data = content[index] + (content[index + 1] << 8);
			nor_command_unlock(base);
			write_word_to_mem(base, 0x555, 0xA0);
			write_word_to_mem(base, offset >> 1, data);
			wait_ready(base, offset);	   
			offset += 2;
		}
		if(remain_byte){
			UINT32 data = content[size_bytes - 1];
			nor_command_unlock(base);
			write_word_to_mem(base, 0x555, 0xA0);
			write_word_to_mem(base, offset >> 1, data);
			wait_ready(base, offset);	   
		}
	}
}

/*********************************************************************************************************
** 函数名称: nor_erase_sector
** 功能描述: 根据offset擦除一个扇区
** 输　入  : base		  norflash起始地址
** 			 offset		  偏移地址		  
** 输　出  : 0 成功 , -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_erase_sector(UINT32 base, UINT offset){
	if(IS_FAKE_MODE()){
		UINT8 sector_no = GET_SECTOR_NO(offset);
		UINT32 sector_start_offset = GET_SECTOR_OFFSET(sector_no);
		UINT sector_size = GET_SECTOR_SIZE(sector_no);
		lib_memset((PVOID)(base + sector_start_offset), (INT)-1, sector_size);
		sector_infos[sector_no].erase_cnt++;
	}
	else{
		nor_command_unlock(NOR_FLASH_BASE);
		write_word_to_mem(NOR_FLASH_BASE,0x555, 0x80);	 
		nor_command_unlock(NOR_FLASH_BASE);
		write_word_to_mem(NOR_FLASH_BASE,offset >> 1, 0x30);	 	           /* 发出扇区地址 */ 
		wait_ready(NOR_FLASH_BASE, offset);									   /* 等待擦除完成 */
	}
}
/*********************************************************************************************************
** 函数名称: nor_erase_range
** 功能描述: 将[low_sector_no, high_sector_no)一并擦除
** 输　入  : low_sector_no		  低sector号
** 			 high_sector_no		  高sector号
**			 erase_nor      	  基本擦除函数
** 输　出  : 0 成功 , -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 nor_erase_range(UINT8 low_sector_no, UINT8 high_sector_no, UINT8 (*erase_nor)(UINT, ENUM_ERASE_OPTIONS)){
	INT i;
	for (i = low_sector_no; i < high_sector_no; i++)
	{
		if(erase_nor(GET_SECTOR_OFFSET(i), ERASE_SECTOR) < 0){
#ifdef NOR_DEBUG
			CHAR temp[TEMP_BUF_SZ];
			lib_memset(temp, 0, TEMP_BUF_SZ);
			sprintf(temp, WARN "try to erase protected sector %d", i);
			pretty_print("[nor erase range statue]", temp, DONT_CENTRAL);
#endif // DEBUG
		}
	}
	return 0;
}
/*********************************************************************************************************
** 函数名称: nor_erase_region
** 功能描述: 封装擦除Region部分，用于erase_nor的调用
** 输　入  : region_no         首部偏移
**           erase_nor         擦除函数
** 输　出  : 0成功， -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 nor_erase_region(INT8 region_no, UINT8 (*erase_nor)(UINT, ENUM_ERASE_OPTIONS)){
	INT count;
	INT region_base;
	switch (region_no)
	{
	case 0:
		count = 1;
		region_base = 0;
		break;
	case 1:
		count = 2;
		region_base = 1;
		break;
	case 2:
		count = 1;
		region_base = 3;
		break;
	case 3:
		count = 31;
		region_base = 4;
		break;
	default:
		break;
	}
	if(nor_erase_range(region_base, region_base + count, erase_nor) < 0){
		// return -1
#ifdef NOR_DEBUG
		CHAR temp[TEMP_BUF_SZ];
		lib_memset(temp, 0, TEMP_BUF_SZ);
		sprintf(temp, WARN "region %d has protected sectors", region_no);
		pretty_print("[nor erase region statue]", temp, DONT_CENTRAL);
#endif // DEBUG
	}
	return 0;
}

/*********************************************************************************************************
** 函数名称: pretty_print
** 功能描述: 美化打印格式
** 输　入  : header         首部描述
**           content          内容
**			 centralized	  是否居中打印，如果居中打印，将只打印header的内容
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID pretty_print(PCHAR header, PCHAR content, BOOL centralized){
    INT space_i;
    INT width = NMAX_DISPLAY;
	if(!centralized){
		printf("| %s", header);
		INT header_len = lib_strlen(header) + 2;
		INT content_len = lib_strlen(content) + 2;
		INT space_len = width - header_len - content_len;
		for (space_i = 0; space_i < space_len; space_i++)
		{
			printf(" ");
		}
		printf("%s |\n", content);
	}
    else
	{
		INT header_len = lib_strlen(header) + 4;
		INT l_minus_len = (width - header_len) / 2;
		INT r_minus_len = width - header_len - l_minus_len;
		printf("| ");
		for (space_i = 1; space_i < l_minus_len; space_i++)
		{
			printf(" ");
		}
		printf("%s", header);
		for (space_i = 0; space_i < r_minus_len - 1; space_i++)
		{
			printf(" ");
		}
		printf(" |\n");
	}
	
}
/*********************************************************************************************************
** 函数名称: show_divider
** 功能描述: 显示分割线
** 输　入  : header         描述：-----header-----
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID show_divider(PCHAR header){
	INT i;
	INT width = NMAX_DISPLAY;
	printf("+");
	if(header == LW_NULL){
		for (i = 1; i < width - 1; i++)
		{
			printf("-");
		}	
	}
	else
	{
		INT header_len = lib_strlen(header);
		INT l_minus_len = (width - header_len / 2);
		INT r_minus_len = width - header_len - l_minus_len;
		for (i = 1; i < l_minus_len; i++)
		{
			printf("-");
		}
		printf("%s", header);
		for (i = 0; i < r_minus_len - 1; i++)
		{
			printf("-");
		}
	}
	printf("+\n");
}


