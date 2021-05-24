# Specification

> 这里记录了一部分开发规范

## Sylix变量命名规范

```c
PTPS_SUPER_BLOCK    psb          = LW_NULL;
PUCHAR              pucSectorBuf = LW_NULL;
UINT                uiSectorSize;
UINT64              uiTotalBlkCnt;
UINT                uiSctPerBlk;
UINT64              uiLogBlkCnt;
UINT64              uiLogSize;
INT                 iMode = 0;
errno_t             iErr  = ERROR_NONE;
```

变量命名原则：**类型 + 驼峰**

例如，uiSectorSize，表明它是一个**无符号整型**，意义为Sector的大小

## Sylix函数命名规范

**1. 内部函数**

```c
static void __camelName (args)
{
    ...
}
```

内部函数命名原则：**__ + 驼峰**

**2. 外部接口**

```c
void camelName(args);
```

## SylixOS文件命名规范

![image-20210325101630941](G:\MyProject\Project.HoitOS\HoitOS\images\spec-file-name.png)

文件命名原则：**驼峰**

## SylixOS注释规范

**1. 开头**

```c
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
** 文   件   名: xxx.c
**
** 创   建   人: xxx
**
** 文件创建日期: yyyy 年 mm 月 dd 日
**
** 描        述: xxxx

** BUG:
*********************************************************************************************************/
```

**2. 函数功能**

```c
/*********************************************************************************************************
** 函数名称: func_name
** 功能描述: 用于干这个
** 输　入  : 变量1           解释1
**          变量2  		  解释2
**          变量3  		  解释3
** 输　出  : 输出			解释输出
** 全局变量: 无
** 调用模块: 无
*********************************************************************************************************/
```

**3. 行注释**

```c
if ((uiSectorShift <= 9) || (uiSectorShift >= 16)) {                /* sector 大小错误              */
```

**4. 块注释**

```c
/*
 *  初始化空间管理inode和root inode
 */
```

## 