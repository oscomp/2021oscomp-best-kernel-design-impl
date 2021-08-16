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
** 文   件   名: nor_cmd.c
**
** 创   建   人: Pan yanqi (潘延麒)
**
** 文件创建日期: 2021 年 03 月 17 日
**
** 描        述: NorFlash驱动 命令 便于调试BUG，支持Fake和True
*********************************************************************************************************/
#include "nor.h"
#include "nor_cmd.h"
#include "nor_util.h"

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

ENUM_ERASE_OPTIONS parse_erase_options(PCHAR ops){
    ENUM_ERASE_OPTIONS erase_ops = ERASE_SECTOR;
    if(case_ignore_strcmp(ops, "chip") == 0 || case_ignore_strcmp(ops, "c") == 0)
    {
        erase_ops = ERASE_CHIP;
    }
    else if (case_ignore_strcmp(ops, "sect") == 0 || case_ignore_strcmp(ops, "sector") == 0 
                                                  || case_ignore_strcmp(ops, "s") == 0)
    {
        erase_ops = ERASE_SECTOR;
    }
    else if (case_ignore_strcmp(ops, "region") == 0 || case_ignore_strcmp(ops, "r") == 0) 
    {
        erase_ops = ERASE_REGION;
    }
    return erase_ops;
}

ENUM_WRITE_OPTIONS parse_write_options(PCHAR ops){
    ENUM_WRITE_OPTIONS write_ops = WRITE_KEEP;
    if(case_ignore_strcmp(ops, "o") == 0 || case_ignore_strcmp(ops, "overwrite") == 0)
    {
        write_ops = WRITE_OVERWRITE;
    }
    else if (case_ignore_strcmp(ops, "k") == 0 || case_ignore_strcmp(ops, "keep") == 0)
    {
        write_ops = WRITE_KEEP;
    }
    return write_ops;
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
    return test_nor();
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
    CHAR temp[TEMP_BUF_SZ] = {'\0'};

    show_divider(LW_NULL);
    sprintf(temp, "%s", GET_PREFIX());                                   
    pretty_print("func:", temp, DONT_CENTRAL);
    lib_memset(temp, 0, TEMP_BUF_SZ);
    if(iArgC > 2){
        sprintf(temp, "%s", GET_ARG(1));                                    /* 只显示一个参数，避免溢出 */
        pretty_print("args:", temp, DONT_CENTRAL);
    }
    show_divider(LW_NULL);

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
            BOOL toggle_high_level_ops = iArgC >= 6 ? TRUE : FALSE;
            ENUM_WRITE_OPTIONS write_ops = WRITE_KEEP;
            if(toggle_high_level_ops){
                write_ops = parse_write_options(GET_ARG(5));
            }
            write_nor(offset, content, size, write_ops);
            break;
        }
        case READ:{
            UINT offset = atoi(GET_ARG(2));
            UINT size = atoi(GET_ARG(3));
            PCHAR content = (PCHAR)lib_malloc(size);
            read_nor(offset, content, size);
            pretty_print("[read content belows]", "", DONT_CENTRAL);
            INT i;
            for (i = 0; i < size; i++)
            {
                printf("%c", *(content + i));
            }
            printf("\n");
            break;
        }   
        case ERASE:{
            UINT offset = atoi(GET_ARG(2));
            ENUM_ERASE_OPTIONS erase_ops = ERASE_SECTOR;
            BOOL toggle_high_level_ops = iArgC >= 5 ? TRUE : FALSE;
            if(toggle_high_level_ops){
                erase_ops = parse_erase_options(GET_ARG(4));
            }
            erase_nor(offset, erase_ops);
            break;
        }
        case HELP:{
            show_divider(LW_NULL);
            pretty_print("[summary]", "fls [-s|-S] | fls", DONT_CENTRAL);
            printf("\n[description]: Check nor flash status, you can use '-s' or '-S' or NULL to trigger summary command. \ 
                    \nSummary includes some configs, whether sector is bad and earsing count of sectors and so on. \n");
            
            show_divider(LW_NULL);

            pretty_print("[write]", "", DONT_CENTRAL);
            pretty_print("  -[normal]:", "fls [-w|-W] offset content", DONT_CENTRAL);
            pretty_print("  -[option]:","fls [-e|-E] offset -o [k|o]", DONT_CENTRAL);
            printf("\n[description]: Write some `content` to `offset`, you can use '-w' or '-W' to trigger write command.    \ 
                    \nHere, offset means bias of [flash base address], so you don't need to manage actual address yourself :)\
                    \nBy the way, '-o o' means overwrite `content` in flash, while '-o k' means keep `content` in flash\n");
            
            show_divider(LW_NULL);

            pretty_print("[read]:", "fls [-r|-R] offset size", DONT_CENTRAL);
            printf("\n[description]: Read `size` content from `offset`, you can use '-r' or '-R' to trigger read command.\n");
            show_divider(LW_NULL);
            
            pretty_print("[erase]:", "", DONT_CENTRAL);
            pretty_print("  -[normal]:", "fls [-e|-E] offset", DONT_CENTRAL);
            pretty_print("  -[option]:","fls [-e|-E] offset -o [r|s|c]", DONT_CENTRAL);
            pretty_print("", "default:  -o s", DONT_CENTRAL);
            printf("\n[description]: Erase some content in offset, you can use '-e' or '-E' to trigger erase command.       \
                    \nHere, '-o r' means erase one region, [Note that] one region is consist of numbers of sectors, and     \
                    \n'-o s' means erase one sector, while '-o c' means erase the whole chip. However, some erase operation \
                    \nwill give some warning becase of UBOOT region\n");
                    
            
            show_divider(LW_NULL);

            pretty_print("[test]:", "fls [-t|-T]", DONT_CENTRAL);
            printf("\n[description]: Do some tests to verify the correction of nor flash. Test includes:            \ 
                    \n 1. write small content.                                                                      \
                    \n 2. read the content which step 1 write in by '-o s' and see if they are same.                \ 
                    \n 3. read the content which step 1 write in by '-o b' and see if their first character is same.\
                    \n 4. pressure write test.(Fake Only)                                                           \
                    \n 5. bad sector test, see if our bad sector simulator works.(Fake Only)                        \
                    \nBy the way, you can use '-t' or '-T' to trigger test command. \n");
            
            show_divider(LW_NULL);
            
            pretty_print("[reset]:", "fls -reset", DONT_CENTRAL);
            printf("\n[description]: Reset the whole nor flash as it looks like a brand new one. (Fake Only) While \
                    \nThe True Nor Flash is only reset its cmd.\n");
            show_divider(LW_NULL);
            break;
        }
        case TEST:{
            BOOL is_success = nor_flash_test_ablility();
            if(is_success){
                pretty_print("[test result]:", "success", DONT_CENTRAL);
            }
            else
            {
                pretty_print("[test result]:", "some test cases failed", DONT_CENTRAL);
            }
            break;
        }
        case RESET:{
            nor_reset(NOR_FLASH_BASE);
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