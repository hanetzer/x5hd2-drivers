/***********************************************************
*                      Copyright    , 2009-2050, Hisilicon Tech. Co., Ltd.
*                                   ALL RIGHTS RESERVED
* FileName:  emmc_raw.c
* Description: emmc flash read and write module
*
* History:
* Version   Date           Author            DefectNum      Description
* main\1    2011-09-20     lidongxiang           NULL       Create this file.
************************************************************/
#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hi_flash.h"
#include "nand.h"
#include "emmc_raw.h"

EMMC_FLASH_S g_stEmmcFlash;

#if 1
#define EMMC_RAW_ASSERT(fmt...)      \
                     PRINTF_CA("ASSERT:[%s %d]", __FUNCTION__, __LINE__);  \
                     PRINTF_CA(fmt);\
                     PRINTF_CA("\n");

#else
#define EMMC_RAW_ASSERT(fmt...)
#endif


HI_S32 emmc_raw_init(void)
{
    HI_U8  aucBuf[512];
    HI_S32  dev_fd;
    HI_U8 *pcStr;
    HI_U32 u32Sectors;
    HI_U8  ucLoop;

#if defined (ANDROID)
    if ((dev_fd = open("/dev/block/mmcblk0", O_RDWR)) == -1)
#else
    if ((dev_fd = open("/dev/mmcblk0", O_RDWR)) == -1)
#endif
    {
        return HI_FAILURE;
    }

    g_stEmmcFlash.u32EraseSize = EMMC_SECTOR_SIZE;

    if((ssize_t)sizeof(aucBuf) != read(dev_fd, aucBuf, sizeof(aucBuf)))
    {
        EMMC_RAW_ASSERT("Failed to read dev.");
        close(dev_fd);
        return HI_FAILURE;
    }

    close(dev_fd);

    if( EMMC_SECTOR_TAIL != *(HI_U16*)&aucBuf[510])
    {
        EMMC_RAW_ASSERT("Record Tag[%x] is wrong.", *(HI_U16*)&aucBuf[510]);
    }

    pcStr = &aucBuf[446];

    /* Raw area start from 512, after MBR.. */
    g_stEmmcFlash.u64RawAreaStart = 512;
    for( ucLoop = 0; ucLoop < 4; ucLoop++)
    {
        pcStr += ucLoop*16;
        if(0 == pcStr[4])
        {
            continue;
        }
        else if( EMMC_EXT_PART_ID != pcStr[4])
        {
            EMMC_RAW_ASSERT("Unknown main partition type 0x%x.", pcStr[4]);
            return HI_FAILURE;
        }

        /* Get raw area size from MBR */
        u32Sectors = pcStr[8]| pcStr[9]<<8 | pcStr[10] <<16 | pcStr[11];
        if( 16 > u32Sectors)
        {
            EMMC_RAW_ASSERT("The emmc hasn't been fdisked.");
            return HI_FAILURE;
        }

        g_stEmmcFlash.u64ExtAreaStart = g_stEmmcFlash.u64RawAreaSize = (HI_U64)u32Sectors * EMMC_SECTOR_SIZE;

        /* Get extend area size from MBR */
        u32Sectors = pcStr[0xc]| pcStr[0xd]<<8 | pcStr[0xe] <<16 | pcStr[0xf];
        if( 16 > u32Sectors)
        {
            EMMC_RAW_ASSERT("The emmc hasn't been fdisked.");
            return HI_FAILURE;
        }

        g_stEmmcFlash.u64ExtAreaSize = (HI_U64)u32Sectors * EMMC_SECTOR_SIZE;

        break;
    }

    if( 4 <= ucLoop )
    {
        EMMC_RAW_ASSERT("No found extention partition.");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 emmc_flash_probe(void)
{
    int dev;
#if defined (ANDROID)
    if ((dev = open("/dev/block/mmcblk0", O_RDWR)) == -1)
#else
    if ((dev = open("/dev/mmcblk0", O_RDWR)) == -1)
#endif
    {
        EMMC_RAW_ASSERT("Failed to open device '/dev/mmcblk0'.");
        return HI_FAILURE;
    }

    return (dev);
}

EMMC_CB_S *emmc_raw_open(HI_U64 u64Addr,
                         HI_U64 u64Length)
{
    EMMC_CB_S *pstEmmcCB;
    int fd;

    fd = emmc_flash_probe();
    if( -1 == fd )
    {
        EMMC_RAW_ASSERT("no devices available.");
        return NULL;
    }

    /* Reject open, which are not block aligned */
    if ((u64Addr & (g_stEmmcFlash.u32EraseSize - 1))
            || (u64Length & (g_stEmmcFlash.u32EraseSize - 1)))
    {
        EMMC_RAW_ASSERT("Attempt to open non block aligned, "
                "eMMC blocksize: 0x%x, address: 0x%08llx, length: 0x%08llx.",
                g_stEmmcFlash.u32EraseSize,
                u64Addr,
                u64Length);
        close(fd);
        return NULL;
    }

    if ((u64Addr > g_stEmmcFlash.u64ExtAreaStart + g_stEmmcFlash.u64ExtAreaSize)
            || (u64Length > g_stEmmcFlash.u64ExtAreaStart + g_stEmmcFlash.u64ExtAreaSize)
            || ((u64Addr + u64Length) > g_stEmmcFlash.u64ExtAreaStart + g_stEmmcFlash.u64ExtAreaSize))
    {
        EMMC_RAW_ASSERT("Attempt to open outside the flash area, "
                "eMMC chipsize: 0x%08llx, address: 0x%08llx, length: 0x%08llx\n",
                g_stEmmcFlash.u64ExtAreaStart + g_stEmmcFlash.u64ExtAreaSize,
                u64Addr,
                u64Length);
        close(fd);
        return NULL;
    }

    if ((pstEmmcCB = (EMMC_CB_S *)malloc(sizeof(EMMC_CB_S))) == NULL)
    {
        EMMC_RAW_ASSERT("no many memory.");
        close(fd);
        return NULL;
    }

    pstEmmcCB->u64Address  = u64Addr;
    pstEmmcCB->u64PartSize = u64Length;
    pstEmmcCB->u32EraseSize = g_stEmmcFlash.u32EraseSize;
    pstEmmcCB->fd          = fd;
    pstEmmcCB->enPartType  = EMMC_PART_TYPE_RAW;

    return pstEmmcCB;
}

EMMC_CB_S *emmc_node_open(const HI_U8 *pu8Node)
{
    EMMC_CB_S *pstEmmcCB;
    HI_S32    fd;

    if( NULL == pu8Node )
    {
        return NULL;
    }

    if (-1 == (fd = open((const char*)pu8Node, O_RDWR)))
    {
        EMMC_RAW_ASSERT("no devices available.");
        return NULL;
    }

    if( NULL == (pstEmmcCB = (EMMC_CB_S *)malloc(sizeof(EMMC_CB_S))))
    {
        EMMC_RAW_ASSERT("No enough space.");
        close(fd);
        return NULL;
    }

    pstEmmcCB->u64Address   = 0;
    pstEmmcCB->u64PartSize  = 0;
    pstEmmcCB->u32EraseSize = g_stEmmcFlash.u32EraseSize;
    pstEmmcCB->fd         = fd;
    pstEmmcCB->enPartType = EMMC_PART_TYPE_LOGIC;

    return pstEmmcCB;
}

HI_S32 emmc_block_read(HI_S32 fd,
                              HI_U64 u64Start,
                              HI_U32 u32Len,
                              void *buff)
{
    HI_S32 s32Ret;
    if( -1 == lseek64(fd, (off64_t)u64Start, SEEK_SET))
    {
        EMMC_RAW_ASSERT("Failed to lseek64.");
        return HI_FAILURE;
    }

    s32Ret = read(fd, buff, u32Len);
    return s32Ret;
}

HI_S32 emmc_block_write(HI_S32 fd,
                               HI_U64 u64Start,
                               HI_U32 u32Len,
                               const void *buff)
{
    HI_S32 s32Ret;
    if( -1 == lseek64(fd, (off64_t)u64Start, SEEK_SET))
    {
        EMMC_RAW_ASSERT("Failed to lseek64.");
        return HI_FAILURE;
    }

    s32Ret = write(fd, buff, u32Len);
    return s32Ret;
}

HI_S32 emmc_raw_read(const EMMC_CB_S *pstEmmcCB,
                     HI_U64    u64Offset,    /* should be alignment with emmc block size */
                     HI_U32    u32Length,    /* should be alignment with emmc block size */
                     HI_U8     *buf)
{
    HI_S32 S32Ret;
    HI_U64 u64Start;

    if( NULL == pstEmmcCB || NULL == buf)
    {
        EMMC_RAW_ASSERT("Pointer is null.");
        return HI_FAILURE;
    }

    /* Reject read, which are not block aligned */
    if( EMMC_PART_TYPE_RAW == pstEmmcCB->enPartType )
    {
        if ((u64Offset > pstEmmcCB->u64PartSize)
                || (u32Length > pstEmmcCB->u64PartSize)
                || ((u64Offset + u32Length) > pstEmmcCB->u64PartSize))
        {
            EMMC_RAW_ASSERT("Attempt to write outside the flash handle area, "
                    "eMMC part size: 0x%08llx, offset: 0x%08llx, "
                    "length: 0x%08x.",
                    pstEmmcCB->u64PartSize,
                    u64Offset,
                    u32Length);

            return HI_FAILURE;
        }
    }

    u64Start = pstEmmcCB->u64Address + u64Offset;
    S32Ret = emmc_block_read(pstEmmcCB->fd, u64Start, u32Length, buf);
    return S32Ret;
}

HI_S32 emmc_raw_write(const EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,    /* should be alignment with emmc block size */
                      HI_U32    u32Length,    /* should be alignment with emmc block size */
                      const HI_U8     *buf)
{
    HI_S32 S32Ret;
    HI_U64 u64Start;

    if( NULL == pstEmmcCB || NULL == buf)
    {
        EMMC_RAW_ASSERT("Pointer is null.");
        return HI_FAILURE;
    }

    if( EMMC_PART_TYPE_RAW == pstEmmcCB->enPartType )
    {
        if ((u64Offset > pstEmmcCB->u64PartSize)
                || (u32Length > pstEmmcCB->u64PartSize)
                || ((u64Offset + u32Length) > pstEmmcCB->u64PartSize))
        {
            EMMC_RAW_ASSERT("Attempt to write outside the flash handle area, "
                    "eMMC part size: 0x%08llx, offset: 0x%08llx, "
                    "length: 0x%08x\n",
                    pstEmmcCB->u64PartSize,
                    u64Offset,
                    u32Length);

            return HI_FAILURE;
        }
    }

    u64Start = pstEmmcCB->u64Address + u64Offset;
    S32Ret = emmc_block_write(pstEmmcCB->fd, u64Start, u32Length, buf);
    return S32Ret;
}

HI_S32 emmc_raw_close(EMMC_CB_S *pstEmmcCB)
{
    if( NULL == pstEmmcCB )
    {
        EMMC_RAW_ASSERT("Pointer is null.");
        return HI_FAILURE;
    }

    close(pstEmmcCB->fd);
    free(pstEmmcCB);
    pstEmmcCB = NULL;

    return HI_SUCCESS;
}

