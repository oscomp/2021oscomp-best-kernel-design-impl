/**
 * @file
 * samsung K9F1G08 128Mx8bit NAND FLASH driver.
 */

/*
 * Copyright (c) 2006-2012 SylixOS Group.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Han.hui <sylixos@gmail.com>
 *
 */

#define  __SYLIXOS_KERNEL
#define  __SYLIXOS_YAFFS_DRV
#define  __SYLIXOS_YAFFS_MTD
#include "config.h"
#include "nand.h"

extern nand_info_t  nand_info[];

static struct yaffs_dev boot_dev;
static struct yaffs_dev comm_dev;

static char boot_dev_name[64];
static char comm_dev_name[64];

int  mtdDevCreateEx (char  *pcDevName)
{
    struct mtd_info            *pmtd = &nand_info[0];

    lib_bzero(&boot_dev, sizeof(boot_dev));
    lib_bzero(&comm_dev, sizeof(comm_dev));

    snprintf(boot_dev_name, 64, "%s%d", pcDevName, 0);
    snprintf(comm_dev_name, 64, "%s%d", pcDevName, 1);

    /*
     *  Set up devices Boot
     */
    boot_dev.param.name                  = boot_dev_name;
    boot_dev.param.total_bytes_per_chunk = pmtd->writesize;
    boot_dev.param.chunks_per_block      = pmtd->erasesize / pmtd->writesize;
    boot_dev.param.n_reserved_blocks     = 10;
#ifdef MINI2440_PACKET
    boot_dev.param.start_block           = 1;
    boot_dev.param.end_block             = 128;
#elif defined(MICRO2440_PACKET)
    boot_dev.param.start_block           = 512;
    boot_dev.param.end_block             = boot_dev.param.start_block + 128;
#else
#error "please select MINI2440_PACKET or MICRO2440_PACKET in config.h"
#endif
    boot_dev.param.spare_bytes_per_chunk = pmtd->oobsize;
    boot_dev.param.is_yaffs2             = 1;
    boot_dev.param.use_nand_ecc          = 1;
    boot_dev.param.no_tags_ecc           = 0;
    boot_dev.param.n_caches              = 10;
    boot_dev.driver_context              = (void *)pmtd;  /* mtd device for later use     */
    boot_dev.os_context                  = (void *)&boot_dev;

    /*
     *  Set up devices Comm
     */
    comm_dev.param.name                  = comm_dev_name;
    comm_dev.param.total_bytes_per_chunk = pmtd->writesize;
    comm_dev.param.chunks_per_block      = pmtd->erasesize / pmtd->writesize;
    comm_dev.param.n_reserved_blocks     = 16;
#ifdef MINI2440_PACKET
    comm_dev.param.start_block           = 129;
    comm_dev.param.end_block             = 1023;
#elif defined(MICRO2440_PACKET)
    comm_dev.param.start_block           = boot_dev.param.end_block + 1;
    comm_dev.param.end_block             = 2047;
#endif
    comm_dev.param.spare_bytes_per_chunk = pmtd->oobsize;
    comm_dev.param.is_yaffs2             = 1;
    comm_dev.param.use_nand_ecc          = 1;
    comm_dev.param.no_tags_ecc           = 0;
    comm_dev.param.n_caches              = 50;
    comm_dev.driver_context              = (void *)pmtd;  /* mtd device for later use */
    comm_dev.os_context                  = (void *)&comm_dev;

    yaffs_mtd_drv_install(&boot_dev);
    yaffs_mtd_drv_install(&comm_dev);

    yaffs_add_device(&boot_dev);    /* add to yaffs device table */
    yaffs_add_device(&comm_dev);    /* add to yaffs device table */

    yaffs_mount(boot_dev_name);
    yaffs_mount(comm_dev_name);

    return  0;
}
