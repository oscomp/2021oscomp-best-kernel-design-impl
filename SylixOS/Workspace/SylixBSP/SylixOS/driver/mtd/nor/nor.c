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
** 文   件   名: nor.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 11 日
**
** 描        述: NorFlash裸板驱动
*********************************************************************************************************/
#include "nor.h"
#include "fake_nor.h"
#include "nor_cmd.h"
/*********************************************************************************************************
** 函数名称: nor_init
** 功能描述: 初始化NorFlash，主要做SylixOS base的地址映射
** 输　入  : nor_init_flag			初始化类型
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID nor_init(ENUM_NOR_INIT_FLAG nor_init_flag){
	switch (nor_init_flag)
	{
	case INIT_FAKE_NOR:{
		FAKE_MODE();
		if((NOR_FLASH_BASE = (UINT32)lib_malloc(NOR_FLASH_SZ)) < 0){
			pretty_print("[nor init statue]", "fail cannot create nor", DONT_CENTRAL);
			return;
		}
		lib_memset((PVOID)NOR_FLASH_BASE, (INT)-1, NOR_FLASH_SZ);
		if((sector_infos = (sector_info_t *)malloc(NOR_FLASH_NSECTOR * sizeof(sector_info_t))) < 0){
			pretty_print("[nor init statue]", "fail cannot create sector info", DONT_CENTRAL);
			return;
		}
		INT i;
		for (i = 0; i < NOR_FLASH_NSECTOR; i++)
		{
			sector_infos[i].erase_cnt = 0;
			sector_infos[i].is_bad = FALSE;
		}
		pretty_print("[nor init statue]", "init successfully", DONT_CENTRAL);

		API_ThreadCreate("t_generate_bad_block",
						(PTHREAD_START_ROUTINE)generate_bad_sector,
						LW_NULL,
						LW_NULL);
		break;
	}
	case INIT_TRUE_NOR:
		TRUE_MODE();
		NOR_FLASH_BASE = (UINT32)API_VmmIoRemap2(0, 2 * 1024 * 1024);								
		break;
	default:
		break;
	}
	register_nor_cmd();
	/* 尝试保存文件，好像不行 */
	// INT fd;
	// fd = open(NOR_FLASH_SECTOR_INFO_FILE_PATH, O_RDWR);
	// if(fd < 0){
	// 	fd = open(NOR_FLASH_SECTOR_INFO_FILE_PATH, O_CREAT | O_RDWR,
	// 			  S_IXUSR | S_IRUSR | S_IWUSR);
	// 	if(fd < 0){
	// 		pretty_print("[nor init statue]:", "fail can't get vital infos", DONT_CENTRAL);
	// 		return;
	// 	}
	// }
	pretty_print("[nor init statue]:", "OK", DONT_CENTRAL);
	
}

/*********************************************************************************************************
** 函数名称: scan_nor
** 功能描述: 扫描NorFlash，给出NorFlash基本信息
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID scan_nor(){
	if(NOR_FLASH_BASE == 0){
		pretty_print("[nor scanning statue]", "fail call `nor_init()` first", DONT_CENTRAL);
		goto scan_fail;
	}
	if(IS_FAKE_MODE()){
		show_divider(LW_NULL);
		CHAR temp[TEMP_BUF_SZ];

		lib_memset(temp, 0, TEMP_BUF_SZ);

		pretty_print("[Driver By]", "PYQ", DONT_CENTRAL);
		pretty_print("[Date]", "2021-03-15", DONT_CENTRAL);
		pretty_print("[License]", "FAKE", DONT_CENTRAL);
		pretty_print("[Manufacturer]", "0x2302", DONT_CENTRAL);
		pretty_print("[Device Ident]", "0xC", DONT_CENTRAL);

		sprintf(temp, "%dMB", NOR_FLASH_SZ / (1024 * 1024));
		pretty_print("[Nor Flash SZ]", temp, DONT_CENTRAL);
		lib_memset(temp, 0, TEMP_BUF_SZ);

		sprintf(temp, "#%d", NOR_FLASH_NSECTOR);
		pretty_print("[Eraseable Regions]", temp, DONT_CENTRAL);
		lib_memset(temp, 0, TEMP_BUF_SZ);
		
		show_divider(LW_NULL);
		pretty_print("[scan over]", "", DO_CENTRAL);
	}
	else
	{
		INT device_id, manufacturer_id;
		INT nor_flash_size = 0;
		INT nor_eraseable_nregions;
		PCHAR cfi_magic = (PCHAR)lib_malloc(4);
		pretty_print("[nor scanning statue]", "scanning start", DONT_CENTRAL);
		
		nor_command_unlock(NOR_FLASH_BASE);										/* 解锁擦写、读ID命令 */
		write_word_to_mem(NOR_FLASH_BASE, 0x555, 0x90);
		manufacturer_id = read_word_from_mem(NOR_FLASH_BASE, 0x00);
		device_id = read_word_from_mem(NOR_FLASH_BASE, 0x01);
		nor_reset(NOR_FLASH_BASE);
		
		write_word_to_mem(NOR_FLASH_BASE, 0x55, 0x98);							/* 进入CFI模式 */
		nor_flash_size = 1 << (read_word_from_mem(NOR_FLASH_BASE, 0x27));		/* 查询norflash大小 */
		*cfi_magic = read_word_from_mem(NOR_FLASH_BASE, 0x10);					/* 查找 Q.R.Y */
		*(cfi_magic + 1) = read_word_from_mem(NOR_FLASH_BASE, 0x11);
		*(cfi_magic + 2) = read_word_from_mem(NOR_FLASH_BASE, 0x12);
		*(cfi_magic + 3) = '\0';
		nor_eraseable_nregions = read_word_from_mem(NOR_FLASH_BASE, 0x2C);	    /* 查询区域数 */
		nor_reset(NOR_FLASH_BASE);
		
		if(manufacturer_id != NOR_FLASH_MANUID || device_id != NOR_FLASH_DEVID
		|| nor_flash_size != NOR_FLASH_SZ 
		|| nor_eraseable_nregions != NOR_FALSH_NREGION
		|| lib_strcmp(cfi_magic, CFI_FLAG) != 0){
			// printf("manufacturer_id: 0x%x 0x%x \n", manufacturer_id, NOR_FLASH_MANUID);
			// printf("device_id: 0x%x 0x%x \n", device_id, NOR_FLASH_DEVID);
			// printf("nor_flash_size: %d %d \n", nor_flash_size, NOR_FLASH_SZ);
			// printf("nor_eraseable_nregions: %d %d \n", nor_eraseable_nregions, NOR_FALSH_NREGION);
			// printf("cfi_magic: %s %s \n", cfi_magic, CFI_FLAG);
			pretty_print("[nor scanning statue]", FAIL "Lost Device Info", DONT_CENTRAL);
			goto scan_fail;
		}
		else
		{
			show_divider(LW_NULL);
			CHAR temp[TEMP_BUF_SZ];

			lib_memset(temp, 0, TEMP_BUF_SZ);

			pretty_print("[Driver By]", "PYQ", DONT_CENTRAL);
			pretty_print("[Date]", "2021-03-15", DONT_CENTRAL);
			pretty_print("[License]", "... ", DONT_CENTRAL);

			sprintf(temp, "0x%x", manufacturer_id);
			pretty_print("[Manufacturer]", temp, DONT_CENTRAL);
			lib_memset(temp, 0, TEMP_BUF_SZ);

			sprintf(temp, "0x%x", device_id);
			pretty_print("[Device Ident]", temp, DONT_CENTRAL);
			lib_memset(temp, 0, TEMP_BUF_SZ);

			sprintf(temp, "%dMB", nor_flash_size / (1024 * 1024));
			pretty_print("[Nor Flash SZ]", temp, DONT_CENTRAL);
			lib_memset(temp, 0, TEMP_BUF_SZ);

			sprintf(temp, "#%d", nor_eraseable_nregions);
			pretty_print("[Eraseable Regions]", temp, DONT_CENTRAL);
			lib_memset(temp, 0, TEMP_BUF_SZ);
			
			show_divider(LW_NULL);
			pretty_print("[scan over]", "", DO_CENTRAL);
		}
	}
	nor_summary();
	return;	
scan_fail:
	pretty_print("[nor scanning statue]", FAIL "scan fail", DONT_CENTRAL);
	return;	
}

/*********************************************************************************************************
** 函数名称: read_nor
** 功能描述: 根据选项从offset处读取内容
** 输　入  : offset         首部偏移
**			 content		读出的内容
**           ops            读取选项，可以是READ_BYTE|READ_SECTOR
** 输　出  : 0成功， -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 read_nor(UINT offset, PCHAR content, UINT size_bytes){
	if(nor_check_offset_range(NOR_FLASH_BASE, offset, size_bytes)){
#ifdef NOR_DEBUG
		pretty_print("[nor read statue]", FAIL "nor read out of range", DONT_CENTRAL);
#endif
		return -1;
	}
	INT i;
#ifdef NOR_DEBUG
	pretty_print("[nor read statue]", INFO "nor read start", DONT_CENTRAL);
#endif
	for (i = 0; i < size_bytes; i++)
	{
		*(content + i) = read_byte_from_mem(NOR_FLASH_BASE, offset + i);
	}
#ifdef NOR_DEBUG
	pretty_print("[nor read statue]", INFO "nor read successfully", DONT_CENTRAL);
#endif
	return 0;
}

/*********************************************************************************************************
** 函数名称: erase_nor
** 功能描述: 根据选项从offset处擦除内容
** 输　入  : offset         首部偏移
**           ops            擦除选项，可以是ERASE_SECTOR|ERASE_CHIP|ERASE_REGION
**			 				ERASE_CHIP会调用多次ERASE_SECTOR，不会擦除前7个SECTOR（用于保存UBOOT）
** 输　出  : 0成功， -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 erase_nor(UINT offset, ENUM_ERASE_OPTIONS ops)
{
#ifdef NOR_DEBUG
	CHAR temp[TEMP_BUF_SZ];
	lib_memset(temp, 0, TEMP_BUF_SZ);
#endif // DEBUG	

	if(!nor_check_modifiable_perm(offset)){
#ifdef NOR_DEBUG
		pretty_print("[nor erase statue]", FAIL "no permission", DONT_CENTRAL);
#endif
		return -1;
	}
#ifdef NOR_DEBUG
	pretty_print("[nor erase statue]", INFO "nor erase start", DONT_CENTRAL);
#endif
	switch (ops)
	{
	case ERASE_SECTOR:{
		UINT8 sector_no; 
		if((sector_no = GET_SECTOR_NO(offset)) < 0){
			return -1;
		}

		if(!IS_SECTOR_DIRTY(NOR_FLASH_BASE, sector_no)){
#ifdef NOR_DEBUG
			sprintf(temp, FAIL "sector %d is clean", sector_no);
			pretty_print("[nor erase statue]", temp, DONT_CENTRAL);
			lib_memset(temp, 0, TEMP_BUF_SZ);
			return 0;
#endif // NOR_DEBUG
		}
#ifdef NOR_DEBUG
		sprintf(temp, INFO "erasing sector %d", sector_no);
		pretty_print("[nor erase statue]", temp, DONT_CENTRAL);
		lib_memset(temp, 0, TEMP_BUF_SZ);
#endif // DEBUG

		nor_erase_sector(NOR_FLASH_BASE, offset);						/* 擦除该sector */

#ifdef NOR_DEBUG
		sprintf(temp, INFO "sector %d erased", sector_no);
		pretty_print("[nor erase statue]", temp, DONT_CENTRAL);
		lib_memset(temp, 0, TEMP_BUF_SZ);
#endif // DEBUG

		return 0;
	}
	case ERASE_CHIP:{
		INT region_no;
		for (region_no = 0; region_no < NOR_FALSH_NREGION; region_no++)
		{	
			if(nor_erase_region(region_no, erase_nor) < 0) {	
#ifdef NOR_DEBUG
				sprintf(temp, WARN "try to erase protected region %d ", region_no);
				pretty_print("[nor erase region statue]", temp, DONT_CENTRAL);
				lib_memset(temp, 0, TEMP_BUF_SZ);
#endif // DEBUG
			}
		}
		return 0;
	}
	case ERASE_REGION:{														   /* 调用Erase Sector来擦除Region */
		INT region_no, sector_no;
		if((sector_no = GET_SECTOR_REGION(offset)) < 0){
			return -1;
		}
		region_no = GET_SECTOR_REGION(sector_no);
		/* 擦除 Regions : 对于Region0应该怎么做？暂时不做处理 */
		if(nor_erase_region(region_no, erase_nor) < 0){				
			return -1;
		}
		return 0;
	}
	default:
		break;
	}
	pretty_print("[nor erase statue]", INFO "erase successfully", DONT_CENTRAL);
	return 0;
}

/*********************************************************************************************************
** 函数名称: erase_nor
** 功能描述: 根据选项从offset处写入一定大小的内容
** 输　入  : offset         首部偏移
**			 content 		写入的内容
**           ops            写入选项，可以是WRITE_KEEP|WRITE_OVERWRITE
** 输　出  : 0成功， -1失败
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
UINT8 write_nor(UINT offset, PCHAR content, UINT size_bytes, ENUM_WRITE_OPTIONS ops){
#ifdef NOR_DEBUG
	printf("offset: %x \n", offset);
#endif // NOR_DEBUG
	if(nor_check_offset_range(NOR_FLASH_BASE, offset, size_bytes)){
#ifdef NOR_DEBUG
		pretty_print("[nor write statue]", FAIL "out of range", DONT_CENTRAL);
#endif
		return -1;
	}
	if(!nor_check_modifiable_perm(offset)){
#ifdef NOR_DEBUG
		pretty_print("[nor write statue]", FAIL "no permission", DONT_CENTRAL);
#endif
		return -1;
	}
	UINT8 	start_sector_no = GET_SECTOR_NO(offset);
	UINT8 	end_sector_no = GET_SECTOR_NO(offset + size_bytes - 1);
	UINT8 	sector_no;
	UINT 	size_bytes_have_written = 0;
	for (sector_no = start_sector_no; sector_no <= end_sector_no; sector_no++)			    /* 按照每个sector来写 */
	{
		UINT size_bytes_to_write;
		PCHAR content_to_write;
		UINT offset_to_write;
		if(sector_no == start_sector_no){													/* 第一个sector */
			if(start_sector_no == end_sector_no){											/* 如果只写一个sector，那么就是写size_bytes */
				size_bytes_to_write = size_bytes;										
			}
			else {
				size_bytes_to_write = GET_SECTOR_OFFSET(sector_no + 1) - offset;			/* 否则，写offset ~ 第一个sector结束的所有位置 */
			}
		}
		else if (sector_no == end_sector_no)												/* 最后一个sector */
		{
			size_bytes_to_write = size_bytes - size_bytes_have_written;						/* 总共需要写的字节 减去 已经写入的字节 */
		}
		else {
			size_bytes_to_write = GET_SECTOR_SIZE(sector_no);								/* 写入一个sector大小 */
		}
		// TODO: 完成 is_sector_should_erased 判断
		content_to_write = content + size_bytes_have_written;								/* 写入内容偏移量 */
		offset_to_write = offset + size_bytes_have_written;									/* 写入位置偏移量 */
		BOOL is_sector_should_erased = nor_check_should_erase(NOR_FLASH_BASE, 				/* 查看是否需要擦除这一段内容 */
															  offset_to_write,
															  content_to_write,
															  size_bytes_to_write);
#ifdef NOR_DEBUG
		printf("content_to_write: %s \n", content_to_write);
		printf("offset_to_write: %d \n", offset_to_write);
		printf("size_bytes_to_write: %d\n", size_bytes_to_write);
		printf("size_bytes_have_written: %d\n", size_bytes_have_written);
		printf("is_sector_should_erased: %d\n", is_sector_should_erased);
#endif // NOR_DEBUG
		switch (ops)
		{
		case WRITE_OVERWRITE:{																/* 覆盖写，如果需要擦除则直接擦除 */
			UINT32 start_setcor_offset = GET_SECTOR_OFFSET(sector_no);
			if(is_sector_should_erased){
				erase_nor(start_setcor_offset, ERASE_SECTOR);
			}
			nor_write_buffer(NOR_FLASH_BASE, offset_to_write, content_to_write, size_bytes_to_write);
			break;	
		}
		case WRITE_KEEP:{
			UINT32 start_setcor_offset = GET_SECTOR_OFFSET(sector_no);						/* 保持写，如果需要擦除，则先保存整个sector，然后再将整个sector写入 */
			UINT sector_size = GET_SECTOR_SIZE(sector_no);
			if(is_sector_should_erased){
				PCHAR buffer = (PCHAR)lib_malloc(sector_size);								/* 读出sector的内容到buffer中 */
				read_nor(start_setcor_offset, buffer, sector_size);
				UINT bias_to_write = offset_to_write - start_setcor_offset;					/* 计算写入偏移相对该sector的偏移量 */
				lib_memcpy(buffer + bias_to_write, content_to_write, size_bytes_to_write);  /* 向buffer中写入要写的内容 */
				erase_nor(start_setcor_offset, ERASE_SECTOR);								/* 擦除sector */
#ifdef NOR_DEBUG
				// INT i;
				// PCHAR hh = (PCHAR)lib_malloc(sector_size);
				// read_nor(start_setcor_offset, hh, sector_size);
				// BOOL checked = TRUE;
				// for (i = 0; i < sector_size; i++)
				// {
				// 	if((UINT8)*(hh + i) != 0xFF){
				// 		printf("%d: %x", i, (UINT8)*(hh + i));
				// 		checked = FALSE;
				// 		break;
				// 	}
				// }
				// printf("\nchecked: %d \n", checked);
				// printf("buffer \n");
				// for (i = 0; i < sector_size; i++)
				// {
				// 	printf("%x", *(buffer + i));	
				// }
#endif
				nor_write_buffer(NOR_FLASH_BASE, start_setcor_offset, buffer, sector_size);		/* 将buffer写入sector */
				lib_free(buffer);
			}
			else
			{
				nor_write_buffer(NOR_FLASH_BASE, offset_to_write, content_to_write, size_bytes_to_write);		/* 直接写入sector */
			}
			break;
		}
		default:
			break;
		}
		size_bytes_have_written += size_bytes_to_write;
	}
	return 0;
}

#ifdef NOR_TEST
BOOL test_nor(){
	if(IS_FAKE_MODE()){
		BOOL is_success = TRUE;
		show_divider(LW_NULL);

		pretty_print("[test case 1 (write 1st sector)]:", "", DONT_CENTRAL);
		write_nor(NOR_FLASH_START_OFFSET, "hello power nor, and you know deadpool loves his star!", 55, WRITE_OVERWRITE);

		pretty_print("[test case 2 (read 1st setcor)]:", "", DONT_CENTRAL);
		PCHAR content = lib_malloc(56); 
		read_nor(NOR_FLASH_START_OFFSET, content, 55);
		*(content + 55) = '\0';
		if(lib_strcmp(content, "hello power nor, and you know deadpool loves his star!") != 0){
			pretty_print("[test case 2 failed]", "some byte not match", DONT_CENTRAL);
			printf("origin is: hello power nor, and you know deadpool loves his star! \n");
			printf("yours: %s \n", content);
			is_success = FALSE;
		}

		pretty_print("[test case 3 (pressure write)]:", "", DONT_CENTRAL);
		INT sector_no, write_cnt;
		INT start_sector_no = GET_SECTOR_NO(NOR_FLASH_START_OFFSET);
		UINT offset = NOR_FLASH_START_OFFSET;
		for (sector_no = start_sector_no; sector_no < NOR_FLASH_NSECTOR; sector_no++)	/* 写n个sector */
		{
			UINT sector_size = GET_SECTOR_SIZE(sector_no);
			PCHAR content_one = (PCHAR)lib_malloc(sector_size);
			lib_memset(content_one, (INT)-1, sector_size);
			for (write_cnt = 0; write_cnt <= NOR_FLASH_MAX_ERASE_CNT; write_cnt++)                       		/* 每个setcor写MAX_CNT */
			{
				write_nor(offset, content_one, sector_size, WRITE_OVERWRITE);
			}
			offset += GET_SECTOR_SIZE(sector_no);
			lib_free(content_one);
		}
		
		sleep(1);
		nor_summary();

		pretty_print("[test case 4 (check bad)]:", "", DONT_CENTRAL);
		write_nor(NOR_FLASH_START_OFFSET, "hello power nor, and you know deadpool loves his star!", 55, WRITE_OVERWRITE);
		BOOL has_bad = FALSE;
		offset = NOR_FLASH_START_OFFSET;
		for (sector_no = start_sector_no; sector_no < NOR_FLASH_NSECTOR; sector_no++)                                 /* 写n个sector */
		{
			UINT sector_size = GET_SECTOR_SIZE(sector_no);
			
			PCHAR content = (PCHAR)lib_malloc(sector_size); 
			read_nor(offset, content, sector_size);

			PCHAR content_one = (PCHAR)lib_malloc(sector_size);
			lib_memset(content_one, (INT)-1, sector_size);
			if(lib_memcmp(content_one, content, sector_size) != 0){
				has_bad = TRUE;
				break;
			}
			printf("-> sectcor #%d checked\n", sector_no);
			offset += GET_SECTOR_SIZE(sector_no);
		}
		if(!has_bad){
			pretty_print("[test case 4 failed]", "no differece (bad)", DONT_CENTRAL);
			pretty_print("sector origin is: 11111...", "", DONT_CENTRAL);
			pretty_print("yours: 11111...", "", DONT_CENTRAL);
			is_success = FALSE;
		}
		show_divider(LW_NULL);
		return is_success;
	}
	else{
		UINT sector_no = GET_SECTOR_NO(NOR_FLASH_START_OFFSET);
		pretty_print("#[nor test1]", "basic erase test", DONT_CENTRAL);
		erase_nor(NOR_FLASH_START_OFFSET, ERASE_SECTOR);
		if(IS_SECTOR_DIRTY(NOR_FLASH_BASE, sector_no)){
			pretty_print("#[nor test1]", "fail erase", DONT_CENTRAL);
			return FALSE;
		}

		pretty_print("#[nor test2]", "basic write&read test - OVERWRITE", DONT_CENTRAL);
		write_nor(NOR_FLASH_START_OFFSET, "hello", 5, WRITE_OVERWRITE);
		PCHAR content = (PCHAR)lib_malloc(6);
		read_nor(NOR_FLASH_START_OFFSET, content, 5);
		*(content + 5) = '\0';
		if(lib_strcmp(content, "hello") != 0){
			pretty_print("#[nor test2]", "fail write or read", DONT_CENTRAL);
			return FALSE;
		}
		lib_free(content);

		pretty_print("#[nor test3]", "basic write&read test - KEEP", DONT_CENTRAL);
		write_nor(NOR_FLASH_START_OFFSET + 1, "hello", 5, WRITE_KEEP);
		content = (PCHAR)lib_malloc(7);
		read_nor(NOR_FLASH_START_OFFSET, content, 6);
		*(content + 6) = '\0';
		if(lib_strcmp(content, "hhello") != 0){
			pretty_print("#[nor test3]", "fail write or read", DONT_CENTRAL);
			return FALSE;
		}
		lib_free(content);

		pretty_print("#[nor test4]", "write without erase test", DONT_CENTRAL);
		erase_nor(NOR_FLASH_START_OFFSET, ERASE_SECTOR);
		write_nor(NOR_FLASH_START_OFFSET, ">", 1, WRITE_OVERWRITE);
		write_nor(NOR_FLASH_START_OFFSET, "4", 1, WRITE_OVERWRITE);

		pretty_print("#[nor test5]", "write big content", DONT_CENTRAL);
		UINT size_bytes = GET_SECTOR_SIZE(sector_no) + GET_SECTOR_SIZE(sector_no + 1);
		PCHAR buffer = (PCHAR)lib_malloc(size_bytes);
		lib_memset(buffer, 0, size_bytes);
		write_nor(NOR_FLASH_START_OFFSET, buffer, size_bytes, WRITE_OVERWRITE); 

		content = (PCHAR)lib_malloc(size_bytes);
		read_nor(NOR_FLASH_START_OFFSET, content, size_bytes);

		if(lib_memcmp(content, buffer, size_bytes) != 0){
			pretty_print("#[nor test5]", "fail write big size 128KB", DONT_CENTRAL);
			return FALSE;
		}
		lib_free(content);
		lib_free(buffer);
	}

	return TRUE;
}
#endif // NOR_TEST


/*********************************************************************************************************
 Reference:
 Mini24TEMP_BUF_SZ NorFlash访问时序：
 
 Word访问
 1. 读: 1Cycles
	- RA 			RD								(RA: Address of the memory location to be read)	
													(RA: Data read from location RA during read operation)
 2.	复位: 1 Cycles
	- XXX 			0xF0
 
 3. 写（按字节写入）: 4 Cycles
	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 555			0xA0
	- PA			PD								(PA: Address of the memory location to be programmed)	
													(PD: Data to be programmed at location PA)		
 4. 擦除（Sector）: 6 Cycles
	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 555			0x80

	- 0x555			0xAA 
	- 0x2AA 		0x55
	- SA			0x30								(SA: Address of the sector to be verified (in autoselect mode) or erased)	

 5. 擦除（Chip）: 6 Cycles
	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 555			0x80

	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 0x555			0x10

 6. 获取设备ID（底部Boot Block）: 4 Cycles
	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 0x555			0x90
	- X01			22C4

 7. 获取厂商ID : 4 Cycles
 	- 0x555			0xAA 
	- 0x2AA 		0x55
	- 0x555			0x90
	- X00			01

 8. CFI查询模式 : 1 Cycles
	- 0x55			0x98

	8.1 查询Magic Number Q.R.Y
		- 0x10			Q
		- 0x12			R
		- 0x14			Y
 
 	8.2 查询大小
	 	- 0x27			0x15 (2 ^ 15 Byte = 2 MB)
	
	8.3 查询可擦除区域Regions
		- 0x2C			0x4 (4个)
	
	8.4 查询可擦除区域信息
	(Region 1)
		- 0x2D			0 (1个Sector)
		- 0x2E			0
		- 0x2F			0xTEMP_BUF_SZ (16KB)
		- 0x30			0	
	(Region 2)
		- 0x31			1 (2个Sector)
		- 0x32			0
		- 0x33			0x20 (8KB)
		- 0x34			0	
	(Region 3)
		- 0x35			0 (1个Sector)
		- 0x36			0
		- 0x37			0x80 (32KB)
		- 0x38			0	
	(Region 4)
		- 0x39			0x1E (31个Sector)
		- 0x3A			0
		- 0x3B			0 
		- 0x3C			0x1	(64KB)		

*********************************************************************************************************/
