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
** 文   件   名: hoitFsTest.h
**
** 创   建   人: 张楠
**
** 文件创建日期: 2021 年 05 月 15 日
**
** 描        述: 测试函数，用于测试hoitfs文件系统
*********************************************************************************************************/

#include "./hoitFsTest.h"

/*********************************************************************************************************
 * File Tree Test 文件树基础测试
*********************************************************************************************************/
UINT max_depth      = 6;
UINT max_dirNo      = 5;
UINT max_fileNo     = 5;
UINT max_data_write = 16;
UINT ERROR_FLAG     = 0;
UINT depth  = 1;
extern PHOIT_VOLUME _G_Volumn;

void createDir(PUCHAR pFileName, UINT dirNo) {
    mode_t  mode        = DEFAULT_DIR_PERM;
    PUCHAR  pEnd        = pFileName;
    UCHAR   dirname[9]  = "Newdir0/\0";
    INT     iFd;
    /* 赋予新的名字 */
    dirname[6]  = '0'+dirNo;
    dirNo ++;
    while (*pEnd != 0)
    {
        pEnd ++;
    }

    lib_memcpy(pEnd, dirname, sizeof(dirname));

    mode &= ~S_IFMT;
    
    iFd = open(pFileName, O_RDWR | O_CREAT | O_EXCL, S_IFDIR | mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("error!\n");
        ERROR_FLAG = 1;
        return ;
    }
    close(iFd);
    /* 进入文件夹，开始下一轮递归测试 */
    if (depth <= max_depth) {
        depth ++;
        FileTreeTestStart(pFileName);
        if (ERROR_FLAG == 1) 
            return;
        depth --;
    }
    /* 清空目录名 */
    lib_memset(pEnd, 0, sizeof(dirname));
}

void createFile(PUCHAR pFileName, UINT fileNo) {
    mode_t  mode            = DEFAULT_FILE_PERM;
    PUCHAR  pEnd            = pFileName;
    UCHAR   filename[9]     = "Newfile0\0";
    UCHAR   filewrite[16]   = "Hello hoitfs   \0";
    UCHAR   fileread[16];

    UINT    writetimes      = max_data_write*sizeof(UCHAR)/sizeof(filewrite);
    INT     writebytes      = 0;
    INT     iFd;
    INT     i;
    lib_memset(fileread, 0, sizeof(fileread));
    /* 赋予新的名字 */
    filename[7]  = '0'+fileNo;
    fileNo ++;
    while (*pEnd != 0)
    {
        pEnd ++;
    }
    lib_memcpy(pEnd, filename, sizeof(filename));

    iFd = open(pFileName, O_WRONLY | O_CREAT | O_TRUNC, mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("%s create error!\n",pFileName);
        ERROR_FLAG = 1;
        return ;
    }
    /* 文件统一写入"Hello hoitfs" */
    printf("\t%s starts writing.\n", pFileName);
    for(i=0; i<writetimes ; i++) {        
        writebytes = write(iFd, filewrite, sizeof(filewrite));
    }
    close(iFd);

    // iFd = open(pFileName, O_RDONLY | O_CREAT | O_TRUNC, mode);
    // if (iFd < 0) {
    //     printf("error!\n");
    //     ERROR_FLAG = 1;
    //     return ;
    // }
    // read(iFd, fileread, sizeof(fileread));
    // close(iFd);

    lib_memset(pEnd, 0, sizeof(filename));
}
/*
    创建文件和目录
*/
void FileTreeTestStart(PUCHAR pFileName) {
    UINT    file_num    = max_fileNo;
    UINT    dir_num     = max_dirNo;
    UINT    i;
    /* 创建随机数 */
    // srand((UINT)&file_num);
    // file_num = rand() % max_fileNo + 1;
    // srand((UINT)&dir_num);
    // dir_num = rand() % max_dirNo + 1;

    /* 创建文件 */
    for (i=0 ; i<file_num ; i++) {
        createFile(pFileName, i);
        if (ERROR_FLAG == 1) {
            return;
        }
    }

    /* 创建目录 */
    for (i=0 ; i<dir_num ; i++) {
        createDir(pFileName, i);
        if (ERROR_FLAG == 1) {
            return;
        }        
    }
}
/******************************  check part  ******************************/
void checkDir(PUCHAR pFileName, UINT dirNo) {
    mode_t  mode        = DEFAULT_DIR_PERM;
    PUCHAR  pEnd        = pFileName;
    UCHAR   dirname[9]  = "Newdir0/\0";
    INT     iFd;
    /* 赋予新的名字 */
    dirname[6]  = '0'+dirNo;
    dirNo ++;
    while (*pEnd != 0)
    {
        pEnd ++;
    }

    lib_memcpy(pEnd, dirname, sizeof(dirname));

    mode &= ~S_IFMT;
    
    iFd = open(pFileName, O_RDWR, S_IFDIR | mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("\"%s\" does not exist!\n",pFileName);
        ERROR_FLAG = 1;
        return ;
    }
    close(iFd);
    /* 进入文件夹，开始下一轮递归测试 */
    if (depth <= max_depth) {
        depth ++;
        FileTreeTestCheck(pFileName);
        if (ERROR_FLAG == 1) 
            return;
        depth --;
    }
    /* 清空目录名 */
    lib_memset(pEnd, 0, sizeof(dirname));
}

void checkFile(PUCHAR pFileName, UINT fileNo) {
    mode_t  mode            = DEFAULT_FILE_PERM;
    PUCHAR  pEnd            = pFileName;
    UCHAR   filename[9]     = "Newfile0\0";
    UCHAR   filewrite[16]   = "Hello hoitfs   \0";
    UCHAR   fileread[16];

    UINT    writetimes      = max_data_write*sizeof(UCHAR)/sizeof(filewrite);
    INT     writebytes      = 0;
    INT     iFd;
    INT     i;

    
    lib_memset(fileread, 0, sizeof(fileread));
    /* 赋予新的名字 */
    filename[7]  = '0'+fileNo;
    fileNo ++;
    while (*pEnd != 0)
    {
        pEnd ++;
    }
    lib_memcpy(pEnd, filename, sizeof(filename));
    
    printf("\tcheck %s content\n", pFileName);
    iFd = open(pFileName, O_RDONLY, mode);   /*  排他性创建 */ 
    if (iFd < 0) {
        printf("\"%s\" does not exist!\n",pFileName);
        ERROR_FLAG = 1;
        return ;
    }    
    /* 查看文件内容 "Hello hoitfs" */

    for(i=0; i < writetimes ; i++) {        
        writebytes = read(iFd, fileread, sizeof(filewrite));
        if (writebytes == 0) {
            printf("\"%s\" is empty!\n");
            ERROR_FLAG = 1;
            return ;            
        }
        if (lib_strcmp(fileread, filewrite) != 0) {
            printf("\"%s\" read content is not correct!\n");
        }
        else {
            API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
            printf("\tcheck %s ok\n", pFileName);
            API_TShellColorEnd(STD_OUT);
        }
    }

    close(iFd);
    lib_memset(pEnd, 0, sizeof(filename));
//    lib_memcpy(pEnd, zero, sizeof(zero));
}
/*
    检查创建的文件是否存在，内容是否一致
    检查目录是否存在
*/
void FileTreeTestCheck(PUCHAR pFileName) {
    UINT    i;
    /* 检查文件 */
    for (i=0 ; i<max_fileNo ; i++) {
        checkFile(pFileName, i);
        if (ERROR_FLAG == 1) {
            return;
        }
    }
 
    /* 检查目录 */
    for (i=0 ; i<max_dirNo ; i++) {
        checkDir(pFileName, i);
        if (ERROR_FLAG == 1) {
            return;
        }        
    }    
}

INT hoitTestFileTree (INT  iArgC, PCHAR  ppcArgV[])
{
    UCHAR       filename[512];
    UINT        temp;
    lib_memset(filename, 0 ,sizeof(filename));
    lib_strcpy(filename, "/mnt/hoitfs/\0");

    if (iArgC != 2 && iArgC != 5) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }

    if (iArgC == 2) {
        if (lib_strcmp("-h",ppcArgV[1]) == 0) {
            printf("file tree test argc:\n");
            printf("first argc  :   max file tree depth\n");
            printf("second argc :   max file number in a directory\n");
            printf("thrid argc  :   max dir number in a directory\n");
            return ERROR_NONE;
        } else {
            fprintf(stderr, "arguments error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
    } else if (iArgC == 5) {
        if (lib_strcmp("-t",ppcArgV[1]) == 0) {
            temp = lib_atoi(ppcArgV[2]);
            if (temp < max_depth)
                max_depth = temp;

            temp = lib_atoi(ppcArgV[3]);
            if (temp < max_fileNo)
                max_fileNo = temp;

            temp = lib_atoi(ppcArgV[4]);
            if (temp < max_dirNo)
                max_dirNo = temp;
        } else {
            fprintf(stderr, "arguments error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }        
    }

    printf("===========  File Tree Test!         ===========\n");
    printf("===========  Create File And Dir!    ===========\n");
    FileTreeTestStart(filename);
    printf("===========  Check File And Dir!     ===========\n");
    FileTreeTestCheck(filename);
    printf("===========  File Tree Test End!     ===========\n");
    return  ERROR_NONE;
}

/*********************************************************************************************************
 * File Over Write Test
*********************************************************************************************************/
INT hoitTestFileOverWrite (INT  iArgC, PCHAR  ppcArgV[]) {
    UCHAR   filename[30]     = "/mnt/hoitfs/OverWriteTest\0";
    UCHAR   writeData[1024+256];
    UCHAR   readData[1024+256];
    INT     iFd;
    UCHAR   data    = '1';
    INT     i;
    
    lib_memset(writeData, 0, sizeof(writeData));

    printf("===========  File Overwrite Test!       ===========\n");
    iFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_FILE_PERM);   /*  排他性创建 */ 
    lib_memset(readData, 0, sizeof(readData));
    /* 初始写入64个'1' */
    for (i=0 ; i<64 ; i++) {
        lseek(iFd, (i)*sizeof(CHAR), SEEK_SET);
        write(iFd, &data, sizeof(CHAR));
        writeData[i] = data;
    }
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));
    printf("%s\n",readData);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", data);
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", data);
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }

    /* 关闭文件再打开，追加写256个'2' */
    data ++;
    close(iFd);
    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
    for (i=64 ; i<96 ; i++) {
        lseek(iFd, (i)*sizeof(CHAR), SEEK_SET);
        write(iFd, &data, sizeof(CHAR));
        writeData[i] = data;
    }
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));
    printf("%s\n",readData);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", data);
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", data);
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }

    /* 覆盖修改前256个字为"3" */
    data ++;
    close(iFd);
    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
    for(i=0 ; i<32 ; i++) {
        lseek(iFd, (i)*sizeof(CHAR), SEEK_SET);
        write(iFd, &data, sizeof(CHAR));
        writeData[i] = data;
    }
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));    
    printf("%s\n",readData);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", data);
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", data);
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }

    /* 从512位开始写512个'4' */
    data ++;
    close(iFd);
    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
    for(i=64 ; i<128 ; i++) {
        lseek(iFd, (i)*sizeof(CHAR), SEEK_SET);
        write(iFd, &data, sizeof(CHAR));
        writeData[i] = data;
    }
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));
    printf("%s\n",readData);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", data);
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", data);
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }
    /* 从1024位开始中间空128字节，然后写128个'5' */
//    data ++;
//    close(iFd);
//    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
//    for(i=128 ; i<160 ; i++) {
//        lseek(iFd, (i)*sizeof(CHAR), SEEK_SET);
//        write(iFd, &data, sizeof(CHAR));
//    }
//    printf("\nNo.%c result:\n", data);
//    close(iFd);
//    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
//    read(iFd, readData, sizeof(readData));
//    for ( i = 0; i < sizeof(readData)/sizeof(UCHAR); i++)
//    {
//        if(readData[i] != 0)
//            printf("%c",readData[i]);
//        else
//            printf(" ");
//    }

    data ++;
    close(iFd);
    /* 在开头写下16个x */
    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
    lseek(iFd, 0, SEEK_SET);
    write(iFd, "xxxxxxxxxxxxxxxx", 16);
    for (i=0 ; i<16; i++) {
        writeData[i] = 'x';
    }
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));
    printf("%s\n", readData);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", 'x');
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", 'x');
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }

    data ++;
    close(iFd);
    /* 将前8个x换成y */
    iFd = open(filename, O_WRONLY, DEFAULT_FILE_PERM);
    lseek(iFd, 0, SEEK_SET);
    write(iFd, "yyyyyyyy", 8);
    for (i=0 ; i<8; i++) {
        writeData[i] = 'y';
    }    
    printf("\nNo.%c result:\n", data);
    close(iFd);
    iFd = open(filename, O_RDONLY, DEFAULT_FILE_PERM);
    read(iFd, readData, sizeof(readData));
    printf("%s\n", readData);
    close(iFd);
    if (lib_strcmp(writeData,readData) == 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
        printf("\tcheck No.%c ok\n", 'y');
        API_TShellColorEnd(STD_OUT);        
    } else {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("\tcheck No.%c failed\n", 'y');
        API_TShellColorEnd(STD_OUT);  
        goto __fot_end;       
    }

__fot_end:
    printf("\n");
    printf("===========  File Overwrite Test End!    ===========\n");
    return  ERROR_NONE;
}

/*********************************************************************************************************
 * 软硬连接测试
*********************************************************************************************************/
/*
    读取iFd内容，与write_data比对
*/
INT checkData(INT iFd, UCHAR write_data[128]){
    UCHAR read_data[128];
    /* 先读取文件查看内容是否一致 */
    lib_bzero(read_data, sizeof(read_data));
    read(iFd, read_data, sizeof(read_data));
    if (lib_memcmp(read_data, write_data, sizeof(write_data))!= 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("data is not consist\n");
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;        
    }
    return ERROR_NONE;
}

void checkOK(char const* const _Format) {
    API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
    printf("%s check ok\n",_Format);
    API_TShellColorEnd(STD_OUT);
}

/*
    软硬连接测试流程如下：
    step 1: 创建源文件
        在A目录下，创建源文件，写入原始数据
    step 2: 创建软硬链接
        在B目录下，创建源文件的软链接
        在B目录下。创建源文件的硬链接
    step 3: 检查基本读写
        通过软链接读取源文件数据，检查正确性，再写入新数据，通过源文件读取新数据
        通过硬链接读取源文件数据，检查正确性，再写入新数据，通过源文件读取新数据
    step 4: 检查传递链接
        对软链接创建软链接，通过次级软链接读取数据。
    step 5:检查删除链接对源文件的影响
        删除软链接，再从源文件读取数据
        删除硬链接，再从源文件读取数据
    step 6:检查不同文件系统之间是否可以建立软链接文件。
        在ramfs下创建指向hoitfs下源文件的软链接，读取文件数据。
        创建ramfs，在ramfs下创建新文件，在hoitfs创建指向其的软链接，读取文件数据。
*/
INT hoitTestLink (INT  iArgC, PCHAR  ppcArgV[]) {
    UCHAR   file_name[64]           = "/mnt/hoitfs/A/Test_LinkSourceFile\0";
    UCHAR   file_temp[64]           = "A/Test_LinkSourceFile\0";
    UCHAR   outer_file_name[64]     = "/mnt/ramfs/Test_OuterFile\0";    /* hoitfs软链接指向ramfs的目标文件 */
    UCHAR   file_dir[64]            = "/mnt/hoitfs/A\0";

    UCHAR   hard_link[64]           = "B/Test_HardLinkFile\0";
    UCHAR   soft_link[64]           = "/mnt/hoitfs/B/Test_SoftLinkFile\0";
    UCHAR   second_soft_link[64]    = "/mnt/hoitfs/B/Test_SoftLinkFile2\0";
    UCHAR   inner_link[64]          = "/mnt/hoitfs/B/Test_InnerLinkFile\0"; /* 从hoitfs指向ramfs的软链接 */
    UCHAR   outer_link[64]          = "/mnt/ramfs/Test_OuterLinkFile\0";    /* 从ramfs指向hoitfs的软链接 */

    UCHAR   link_dir[64]            = "/mnt/hoitfs/B\0";

    mode_t  dir_mode                = DEFAULT_DIR_PERM & (~S_IFMT);
    mode_t  file_mode               = DEFAULT_FILE_PERM;

    UCHAR   write_data[128];
    UCHAR   read_data[128];  
    UCHAR   data = 'x';  
    INT     iFd;
    INT     i;
    
    /* 创建目录 */
    iFd = open(file_dir, O_RDWR | O_CREAT | O_EXCL, S_IFDIR | dir_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("create \" %s \" error!\n", file_dir);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    close(iFd);
    iFd = open(link_dir, O_RDWR | O_CREAT | O_EXCL, S_IFDIR | dir_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("create \" %s \" error!\n", link_dir);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    close(iFd);

    /******************************* step1 *******************************/
    iFd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, file_mode); 
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("create \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    
    /* 写128个 "x" */
    for (i=0 ; i<128 ; i++) {
        write(iFd, &data, sizeof(data));
        write_data[i] = data;
    }
    close(iFd);

    /******************************* step2 *******************************/
    /* 软链接 */
    if (symlink(file_name, soft_link) != ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("soft link creating failed!\n");
        API_TShellColorEnd(STD_OUT); 
    }

    /* 硬链接 */
    if (_G_Volumn == LW_NULL) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("can't get the hoitfs header!\n");
        API_TShellColorEnd(STD_OUT); 
    }
    if (__hoitFsHardlink(_G_Volumn, hard_link, file_temp) != ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("hard link creating failed!\n");
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    /******************************* step3 *******************************/
    /******************************* 软链接 *******************************/
    printf("check soft link read...\n");
    iFd = open(soft_link, O_RDWR, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", soft_link);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    /* 先读取文件查看内容是否一致 */
    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    checkOK("soft link read");

    /* 文件前半部'x'改为'y' */
    printf("check soft link write...\n");
    lseek(iFd, 0, SEEK_SET);
    data = 'y';
    for (i=0; i< 64 ; i++) {
        write(iFd, &data, sizeof(UCHAR));
        write_data[i] = data;
    }
    close(iFd);

    /* 通过源文件检查 */
    iFd = open(file_name, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);   
    checkOK("soft link write");

    /******************************* 硬链接 *******************************/
    printf("check hard link read...\n");
    iFd = open(hard_link, O_RDWR, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", hard_link);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }  

    /* 先读取文件查看内容是否一致 */
    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    checkOK("hard link read");

    /* 文件后半部'x'改为'z' */
    printf("check hard link write...\n");
    lseek(iFd, 0, SEEK_SET);
    data = 'z';
    for (i=0; i< 64 ; i++) {
        write(iFd, &data, sizeof(UCHAR));
        write_data[i] = data;
    }
    close(iFd);     
    
    /* 通过源文件检查 */
    iFd = open(file_name, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);
    checkOK("hard link write");

    /******************************* step4 *******************************/
    /***************************** 传递软链接 ****************************/
    printf("check passing check link read...\n");
    if (symlink(soft_link, second_soft_link) != ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("second soft link creating failed!\n");
        API_TShellColorEnd(STD_OUT); 
    }
    iFd = open(second_soft_link, O_RDWR, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", second_soft_link);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    /* 先读取文件查看内容是否一致 */
    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }

    checkOK("passing link read");

    /* 文件数据全改成'0' */
    printf("check passing check link write...\n");
    lseek(iFd, 0, SEEK_SET);
    data = '0';
    for (i=0; i< 128 ; i++) {
        write(iFd, &data, sizeof(UCHAR));
        write_data[i] = data;
    }
    close(iFd);

    /* 通过源文件检查 */
    iFd = open(file_name, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);
    checkOK("passing link write");

    /******************************* step5 *******************************/
    /*********************** 删除软链接和传递软链接 ***********************/
    printf("delete soft link...\n");
    if(unlink(second_soft_link)==PX_ERROR) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("delete soft link failed!\n");
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;         
    }
    if(unlink(soft_link)==PX_ERROR) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("delete soft link failed!\n");
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;         
    }

    /* 通过源文件检查 */
    printf("check soft link deleting influence...\n");
    iFd = open(file_name, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);
    checkOK("");
    /***************************** 删除硬链接 ****************************/
    printf("delete hard link...\n");
    if (unlink(hard_link)==PX_ERROR) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("delete hard link failed!\n");
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;         
    }

    /* 通过源文件检查 */
    printf("check hard link deleting influence...\n");
    iFd = open(file_name, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }

    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);
    checkOK("");
    /******************************* step6 *******************************/
    /* 创建ramfs */
    printf("mount ramfs...\n");
    if (API_MountEx("10000","/mnt/ramfs\0","ramfs\0",LW_NULL)!=ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("mount ramfs failed!\n");
        API_TShellColorEnd(STD_OUT);
        return (0);
    }   
    API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
    printf("mount ramfs OK\n");
    API_TShellColorEnd(STD_OUT);

    /* 在ramfs下指向hoitfs的软链接 */
    if (symlink(file_name, outer_link) != ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("outer link creating failed!\n");
        API_TShellColorEnd(STD_OUT); 
    }

    printf("check outer link...\n");
    iFd = open(outer_link, O_RDWR, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", outer_link);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    
    if (checkData(iFd, write_data)!=ERROR_NONE) {
        close(iFd);
        return PX_ERROR;
    }
    close(iFd);
    checkOK("outer link");
    
    /* 检查从hoitfs指向ramfs的软链接 */
    /* 创建文件 */
    iFd = open(outer_file_name, O_WRONLY | O_CREAT | O_TRUNC, file_mode); 
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("create \" %s \" error!\n", outer_file_name);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }       
    for (i=0 ; i<128 ; i++) {
        write(iFd, &write_data[i], sizeof(data));
    }
    close(iFd);

    if (symlink(outer_file_name, inner_link) != ERROR_NONE) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("inner link creating failed!\n");
        API_TShellColorEnd(STD_OUT); 
    }   
    printf("check inner link...\n");
    iFd = open(inner_link, O_RDONLY, file_mode);
    if (iFd < 0) {
        API_TShellColorStart2(LW_TSHELL_COLOR_RED, STD_OUT);
        printf("open \" %s \" error!\n", inner_link);
        API_TShellColorEnd(STD_OUT); 
        return PX_ERROR;
    }
    
   if (checkData(iFd, write_data)!=ERROR_NONE) {
       close(iFd);
       return PX_ERROR;
   }
    close(iFd);
    checkOK("");
    /* 删除文件和链接 */
    unlink(outer_link);
    unlink(outer_file_name);
    unlink(inner_link);
    unlink(file_name);
}


/*********************************************************************************************************
 * GC测试
*********************************************************************************************************/
#define FILE_MODE                       (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define BIG_FILE                        "RealBigFiles"
#define FILE_SIZE                       60 * 1024
#define PER_SIZE                        1024

INT hoitTestGC(PHOIT_VOLUME pfs){
    INT   iFd;
    UINT  i, j;
    UINT  uiSizeWritten;
    PCHAR pcWriteBuffer;
    
    iFd = open(BIG_FILE, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if(iFd < 0){
        printf("[Create " BIG_FILE "Fail]");
        return;
    }

    uiSizeWritten = 0;
    /* 写入 64 * 1024B */
    for (i = 0; i < FILE_SIZE / PER_SIZE; i++)
    {
        pcWriteBuffer = (PCHAR)lib_malloc(PER_SIZE);
        printf("start cycle %d \n", i);
        for (j = 0; j < PER_SIZE; j++)
        {
            *(pcWriteBuffer + j) = 'a';
        }
        write(iFd, pcWriteBuffer, PER_SIZE);
        uiSizeWritten += (PER_SIZE / 1024);
        printf("write cycle %d ok, %dKB has written, now sector is %d\n" , i, uiSizeWritten, 
                pfs->HOITFS_now_sector->HOITS_bno);
        lib_free(pcWriteBuffer);
    }
    
    API_TShellColorStart2(LW_TSHELL_COLOR_GREEN, STD_OUT);
    printf("Write BigFile OK \n");
    API_TShellColorEnd(STD_OUT);

    close(iFd);
}
