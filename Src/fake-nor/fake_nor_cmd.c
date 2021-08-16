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
** 文   件   名: fake_nor_cmd.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 02 月 25 日
**
** 描        述: RAM模拟NorFlash驱动 命令 便于调试BUG
*********************************************************************************************************/
#include "fake_nor.h"
#include "fake_nor_cmd.h"

/*********************************************************************************************************
** 函数名称: pretty_print
** 功能描述: 美化打印格式
** 输　入  : header         首部描述
**           content          内容
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID pretty_print(PCHAR header, PCHAR content){
    int space_i;
    const int width = 80;
    printf("%s", header);
    int header_len = lib_strlen(header);
    int content_len = lib_strlen(content);
    int space_len = width - header_len - content_len;
    for (space_i = 0; space_i < space_len; space_i++)
    {
        printf(" ");
    }
    printf("%s\n", content);
}
/*********************************************************************************************************
** 函数名称: scan_nor
** 功能描述: 扫描nor_flash，给出nor_flash基本信息
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID scan_nor(){
    CHAR temp_buffer[44];
    SHOW_DIVIDER();
    pretty_print("[name]:", NOR_FLASH_NM);
    pretty_print("[manufacture]:", "PYQ");
    pretty_print("[date]:", "2021-02-24");
    sprintf(temp_buffer, "%dB", NOR_FLASH_SZ);
    pretty_print("[size]:", temp_buffer);
    SHOW_DIVIDER();
}
/*********************************************************************************************************
** 函数名称: nor_summary
** 功能描述: 获取nor_flash基本扇区信息
** 输　入  : NONE
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT nor_summary(){
    CHAR temp_buffer[44];
    SHOW_DIVIDER();
    INT clean_sector = 0, i = 0, bad_sector = 0;
    pretty_print("total sector:", "#512");

    sprintf(temp_buffer, "%dB", NOR_FLASH_SECTORSZ);
    pretty_print("sector size:", temp_buffer);
    lib_memset(temp_buffer, 0, 44);
    
    sprintf(temp_buffer, "%dB", NOR_FLASH_BLKSZ);
    pretty_print("block size:", temp_buffer);
    lib_memset(temp_buffer, 0, 44);

    for (i = 0; i < NOR_FLASH_NSECTOR; i++)
    {
        if(sector_infos[i].erase_cnt == 0){
            clean_sector++;
        }
    }
    sprintf(temp_buffer, "#%d", clean_sector);
    pretty_print("clean sector:", temp_buffer);
    lib_memset(temp_buffer, 0, 44);

    pretty_print("erase count:", "");
    for (i = 0; i < NOR_FLASH_NSECTOR; i++)
    {
        CHAR header_buffer[44];
        CHAR content_buffer[44];
        lib_memset(header_buffer, 0, sizeof(CHAR) * 44);
        lib_memset(content_buffer, 0, sizeof(CHAR) * 44);
        sprintf(header_buffer, "sector %d:", i);
        if(sector_infos[i].is_bad){
            bad_sector++;
            sprintf(content_buffer, "-bad  #%d erase(s)", sector_infos[i].erase_cnt);
        }
        else
            sprintf(content_buffer, "-nice #%d erase(s)", sector_infos[i].erase_cnt);
        pretty_print(header_buffer, content_buffer);
    }

    sprintf(temp_buffer, "#%d", bad_sector);
    pretty_print("bad sector(s):", temp_buffer);
    lib_memset(temp_buffer, 0, 44);

    SHOW_DIVIDER();
    return 0;
}

INT case_ignore_strcmp(PCHAR str1, PCHAR str2){
    PCHAR p1 = str1;
    PCHAR p2 = str2;
    LOWER_STR(p1);
    LOWER_STR(p2);
    return lib_strcmp(str1, str2);
}
/*********************************************************************************************************
** 以下均为命令解析部分函数
*********************************************************************************************************/
CMD_TYPE parse_cmd(PCHAR ops) {
    CMD_TYPE type = SUMMARY;
    if(lib_strcmp(ops, "-w") == 0 || lib_strcmp(ops, "-W") == 0)
    {
        type = WRITE;
    }
    else if (lib_strcmp(ops, "-r") == 0 || lib_strcmp(ops, "-R") == 0)
    {
        type = READ;
    }
    else if (lib_strcmp(ops, "-e") == 0 || lib_strcmp(ops, "-E") == 0)
    {
        type = ERASE;
    }
    else if (lib_strcmp(ops, "-a") == 0 || lib_strcmp(ops, "-A") == 0)
    {
        type = SUMMARY;
    }
    else if (lib_strcmp(ops, "-h") == 0 || lib_strcmp(ops, "-H") == 0)
    {
        type = HELP;
    }
    else if (lib_strcmp(ops, "-t") == 0 || lib_strcmp(ops, "-T") == 0)
    {
        type = TEST;
    }
    else if (case_ignore_strcmp(ops, "-reset") == 0)
    {
        type = RESET;
    }
    return type;
}



READ_OPTIONS parse_read_options(PCHAR ops){
    READ_OPTIONS read_ops = READ_SECTOR;
    if(case_ignore_strcmp(ops, "byte") == 0 || case_ignore_strcmp(ops, "b") == 0)
    {
        read_ops = READ_BYTE;
    }
    else if (case_ignore_strcmp(ops, "sect") == 0 || case_ignore_strcmp(ops, "sector") == 0 
                                                  || case_ignore_strcmp(ops, "s") == 0)
    {
        read_ops = READ_SECTOR;
    }
    return read_ops;
}

ERASE_OPTIONS parse_erase_options(PCHAR ops){
    ERASE_OPTIONS erase_ops = ERASE_SECTOR;
    if(case_ignore_strcmp(ops, "chip") == 0 || case_ignore_strcmp(ops, "c") == 0)
    {
        erase_ops = ERASE_CHIP;
    }
    else if (case_ignore_strcmp(ops, "sect") == 0 || case_ignore_strcmp(ops, "sector") == 0 
                                                  || case_ignore_strcmp(ops, "s") == 0)
    {
        erase_ops = ERASE_SECTOR;
    }
    else if (case_ignore_strcmp(ops, "block") == 0 || case_ignore_strcmp(ops, "b") == 0) 
    {
        erase_ops = ERASE_BLOCK;
    }
    return erase_ops;
}
/*********************************************************************************************************
** 函数名称: nor_flash_test_ablility
** 功能描述: 测试模拟的nor_flash功能
** 输　入  : NONE
** 输　出  : 如果通过所有测试点，返回TRUE，否则返回FALSE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
BOOL nor_flash_test_ablility(){
    BOOL is_success = TRUE;
    INT i, j;
    SHOW_DIVIDER();
    
    pretty_print("[test case 1 (write 1st sector)]:", "");
    write_nor(0, "hello power nor, and you know deadpool loves his star!", 55);

    pretty_print("[test case 2 (read 1st setcor)]:", "");
    read_content_t content = read_nor(0, READ_SECTOR);
    if(lib_strcmp(content.content, "hello power nor, and you know deadpool loves his star!") != 0){
        pretty_print("[test case 2 failed]", "some byte not match");
        printf("origin is: hello power nor, and you know deadpool loves his star! \n");
        printf("yours: %s \n", content.content);
        is_success = FALSE;
    }

    pretty_print("[test case 3 (read 1st byte)]:", "");
    content = read_nor(0, READ_BYTE);
    CHAR target = *(content.content);
    if(target != 'h'){
        pretty_print("[test case 3 failed]", "char not matches");
        printf("origin is: h\n");
        printf("yours: %c \n", target);
        is_success = FALSE;
    }

    pretty_print("[test case 4 (pressure write)]:", "");
    PCHAR astr = (PCHAR)malloc(NOR_FLASH_SECTORSZ + 1);
    for (i = 0; i < NOR_FLASH_SECTORSZ; i++)
    {
        *(astr + i) = 'a';
    }
    *(astr + NOR_FLASH_SECTORSZ) = '\0';
    
    for (i = 0; i < NOR_FLASH_NSECTOR; i++)                                 /* 写n个sector */
    {
        INT offset = i * NOR_FLASH_SECTORSZ;
        for (j = 0; j < NOR_FLASH_MAX_ERASE_CNT; j++)                       /* 每个setcor写MAX_CNT */
        {
            write_nor(offset, astr, NOR_FLASH_SECTORSZ);
        }
        sleep(1);
    }
    nor_summary();

    pretty_print("[test case 5 (check bad)]:", "");
    BOOL has_bad = FALSE;
    for (i = 0; i < NOR_FLASH_NSECTOR; i++)                                 /* 写n个sector */
    {
        INT offset = i * NOR_FLASH_SECTORSZ;
        content = read_nor(offset, READ_SECTOR);
        if(content.is_success){
            PCHAR temp = (PCHAR)malloc(NOR_FLASH_SECTORSZ + 1);
            lib_strncpy(temp, content.content, NOR_FLASH_SECTORSZ);
            *(temp + NOR_FLASH_SECTORSZ) = '\0';
            if(lib_strcmp(astr, temp) != 0){
                has_bad = TRUE;
                break;
            }
            free(temp);
            printf("-> sectcor #%d checked\n", i);
        }
        else
        {
            pretty_print("[test case 5 failed]", "read error");
            is_success = FALSE;
            break;
        }
    }
    if(!has_bad){
        pretty_print("[test case 5 failed]", "no differece (bad)");
        pretty_print("sector origin is: a.....a", "");
        pretty_print("yours: a.....a", "");
        is_success = FALSE;
    }
    SHOW_DIVIDER();
    
    return is_success;
}
/*********************************************************************************************************
** 函数名称: nor_flash_cmd_wrppaer
** 功能描述: 注册SylixOS ttinyShell命令函数
** 输　入  : iArgC         变量数
**           ppcArgV       变量内容
** 输　出  : 0
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT nor_flash_cmd_wrppaer(INT  iArgC, PCHAR  ppcArgV[]){
    INT i;
    CHAR temp[40] = {'\0'};
    SHOW_DIVIDER();
    pretty_print("func:", GET_PREFIX());
    sprintf(temp, "%s", GET_ARG(1));                                    /* 只显示一个参数，避免溢出 */
    pretty_print("args:", temp);
    SHOW_DIVIDER();
    if(iArgC == 1){
        nor_summary();
    }
    else
    {
        PCHAR ops = GET_ARG(1);
        CMD_TYPE type = parse_cmd(ops);
        switch (type)
        {
        case SUMMARY:
            nor_summary();
            break;
        case WRITE:{
            UINT offset = atoi(GET_ARG(2));
            PCHAR content = GET_ARG(3);
            UINT size = lib_strlen(content);
            write_nor(offset, content, size);
            break;
        }
        case READ:{
            UINT offset = atoi(GET_ARG(2));
            BOOL toggle_high_level_ops = iArgC >= 5 ? TRUE : FALSE;
            READ_OPTIONS read_ops = READ_SECTOR;
            if(toggle_high_level_ops){
                read_ops = parse_read_options(GET_ARG(4));
            }
            read_content_t content = read_nor(offset, read_ops);
            printf("[read content belows]: \n");
            if(read_ops == READ_BYTE){
                printf("-> %c \n", *content.content);
            }
            else if (read_ops == READ_SECTOR)
            {
                printf("-> ");
                for (i = 0; i < NOR_FLASH_SECTORSZ; i++)
                {
                    if(*(content.content + i) == '\0')
                        printf(" ");
                    else
                        printf("%c", *(content.content + i));
                }
                printf("\n");
            }
            break;
        }   
        case ERASE:{
            UINT offset = atoi(GET_ARG(2));
            ERASE_OPTIONS erase_ops = ERASE_SECTOR;
            BOOL toggle_high_level_ops = iArgC >= 5 ? TRUE : FALSE;
            if(toggle_high_level_ops){
                erase_ops = parse_erase_options(GET_ARG(4));
            }
            erase_nor(offset, erase_ops);
            break;
        }
        case HELP:{
            SHOW_DIVIDER();
            pretty_print("[summary]:", "fls [-s|-S] | fls");
            printf("\n[description]: Check nor flash status, you can use '-s' or '-S' or NULL to trigger summary command. Summary includes some configs, whether sector is bad and earsing count of sectors and so on. \n");
            
            SHOW_DIVIDER();

            pretty_print("[write]:", "fls [-w|-W] offset content");
            printf("\n[description]: Write some content to offset, you can use '-w' or '-W' to trigger write command. Here, offset means bias of [flash base address], so you don't need to manage actual address yourself :)\n");
            
            SHOW_DIVIDER();

            pretty_print("[read]:", "");
            pretty_print("  -[normal]:", "fls [-r|-R] offset");
            pretty_print("  -[option]:", "fls [-r|-R] offset -o [b|s]");
            pretty_print("", "default:  -o s");
            printf("\n[description]: Read some content from offset, you can use '-r' or '-R' to trigger read command. Moreover, you can use '-o' to gain more ablilities, '-o b' or '-o byte' means we can read one byte from offset, while '-o s' or '-o sector' or '-o sect' means we can read one sector from offset. [Note that] if option is reading by sector, then you will read the sector where contain that offset, but not the size of sector from offset. [Default], the options is reading by setcor. \n");
            
            SHOW_DIVIDER();
            
            pretty_print("[erase]:", "");
            pretty_print("  -[normal]:", "fls [-e|-E] offset");
            pretty_print("  -[option]:","fls [-e|-E] offset -o [b|s|c]");
            pretty_print("", "default:  -o s");
            printf("\n[description]: Erase some content in offset, you can use '-e' or '-E' to trigger erase command. Here, '-o b' means erase one block, [Note that] one block is consist of #%d sectors, and '-o s' means erase one sector, while '-o c' meas erase the whole chip.\n", NOR_FLASH_SECTPBLK);
            
            SHOW_DIVIDER();

            pretty_print("[test]:", "fls [-t|-T]");
            printf("\n[description]: Do some tests to verify the correction of this fake nor flash. Test includes: \n 1. write small content. \n 2. read the content which step 1 write in by '-o s' and see if they are same. \n 3. read the content which step 1 write in by '-o b' and see if their first character is same. \n 4. pressure write test. \n 5. bad sector test, see if our bad sector simulator works. \nBy the way, you can use '-t' or '-T' to trigger test command. \n");
            
            SHOW_DIVIDER();
            
            pretty_print("[reset]:", "fls -reset");
            printf("\n[description]: Reset the whole nor flash as it looks like a brand new one. \n");
            SHOW_DIVIDER();
            break;
        }
        case TEST:{
            BOOL is_success = nor_flash_test_ablility();
            if(is_success){
                pretty_print("[test result]:", "success");
            }
            else
            {
                pretty_print("[test result]:", "some test cases failed");
            }
            break;
        }
        case RESET:{
            reset_nor();
            break;
        }
        default:
            break;
        }
    }
    return 0;
}





VOID register_nor_cmd(){
    API_TShellKeywordAdd("fls", nor_flash_cmd_wrppaer);
}
