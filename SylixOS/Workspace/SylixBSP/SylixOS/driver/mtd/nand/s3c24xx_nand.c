/**
 * @file
 * s3c24xx samsung ARM processor NAND controler driver.
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
#include "config.h"
#include "nand.h"

#define NF_BASE CONFIG_SYS_NAND_BASE
#define NFCONF  (NF_BASE + 0x0)
#define NFCONT  (NF_BASE + 0x4)
#define NFCMD   (NF_BASE + 0x8)
#define NFADDR  (NF_BASE + 0xc)

#define NFMECCD0                (NF_BASE + 0x14)
#define NFMECCD1                (NF_BASE + 0x18)
#define NFSECCD                 (NF_BASE + 0x1C)
#define NFSTAT                  (NF_BASE + 0x20)
#define NFSTAT0                 (NF_BASE + 0x24)
#define NFSTAT1                 (NF_BASE + 0x28)
#define NFMECC0                 (NF_BASE + 0x2C)
#define NFMECC1                 (NF_BASE + 0x30)
#define NFSECC                  (NF_BASE + 0x34)
#define NFSBLK                  (NF_BASE + 0x38)
#define NFEBLK                  (NF_BASE + 0x3c)
#define S3C2440_NFCONT_nCE      (1 << 1)    /* chip select bit */
#define S3C2440_ADDR_NALE       0x0c        /* reg offset */
#define S3C2440_ADDR_NCLE       0x08        /* reg offset */

static void s3c24xx_hwcontrol (struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
    static ulong IO_ADDR_W = NF_BASE;   /* MUST be 'static' */

    if (ctrl & NAND_CTRL_CHANGE) {
        IO_ADDR_W = NF_BASE;

        if (!(ctrl & NAND_CLE)) {
            IO_ADDR_W |= S3C2440_ADDR_NALE;
        }
        if (!(ctrl & NAND_ALE)) {
            IO_ADDR_W |= S3C2440_ADDR_NCLE;
        }

        if (ctrl & NAND_NCE) {
            rNFCONT &= ~S3C2440_NFCONT_nCE;
        } else {
            rNFCONT |= S3C2440_NFCONT_nCE;
        }
    }

    if (cmd != NAND_CMD_NONE){
        writeb(cmd,(void *)IO_ADDR_W);
    }
}

static int s3c24xx_dev_ready (struct mtd_info *mtd)
{
    return (rNFSTAT & 0x01);
}

int board_nand_init (struct nand_chip *nand)
{
    rGPACON = (rGPACON & ~(0x3f << 17)) | (0x3f << 17); /* GPIO config */

#define NAND_TACLS                  2        /*  CLE & ALE (Min:10ns) */
                                             /*  CLE Hold Time tCLH */
#define NAND_TWRPH0                 4        /*  nWE (Min:25ns) */
                                             /*  nWE Pulse Width tWP */
#define NAND_TWRPH1                 2        /*  nWE (Min:15ns) */
                                             /*  nWE High Hold Time tWH */
    rNFCONF = (NAND_TACLS << 12)             /*  HCLK x (TACLS) */
            | (NAND_TWRPH0 << 8)             /*  HCLK x (TWRPH0 + 1) */
            | (NAND_TWRPH1 << 4);            /*  HCLK x (TWRPH1 + 1) */

    rNFCONT = (0 << 13)                      /*  Disable lock-tight */
            | (0 << 12)                      /*  Disable Soft lock */
            | (0 << 10)                      /*  Disable interrupt */
            | (0 <<  9)                      /*  Disable RnB interrupt */
            | (0 <<  8)                      /*  RnB Detect rising edge */
            | (1 <<  6)                      /*  Lock spare ECC */
            | (1 <<  5)                      /*  Lock main data area ECC */
            | (1 <<  4)                      /*  Initialize ECC de/encoder */
            | (1 <<  1)                      /*  Force nFCE to high */
                                             /*  (Disable chip select)*/
            | (1 <<  0);                     /*  NAND flash controller enable*/

    rNFSTAT = 0;

    /*
     * initialize nand_chip data structure
     */
    nand->IO_ADDR_R = nand->IO_ADDR_W = (void *)NFDATA;

    nand->ecc.mode  = NAND_ECC_SOFT;

    /*
     *  read_buf and write_buf are default
     *  read_byte and write_byte are default
     *  but cmd_ctrl and dev_ready always must be implemented
     */
    nand->cmd_ctrl  = s3c24xx_hwcontrol;
    nand->dev_ready = s3c24xx_dev_ready;

    return 0;
}
