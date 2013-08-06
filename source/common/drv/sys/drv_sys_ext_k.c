/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_sys_ext_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/02/09
  Description   :
  History       :
  1.Date        : 2006/02/09
    Author      : Luo Chuanzao¡¡¡¡
    Modification: Created file

  2.Date         : 2006/2/9
    Author       : QuYaxin 46153
    Modification : Modified some macro for coherence
                   with mpi_struct.h

  3.Date         : 2010/1/25
    Author       : jianglei
    Modification : Modified for X5HD common module

******************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <mach/hardware.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/math64.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_sys_ext.h"

#include "drv_sys_ioctl.h"

#define DIV_NS_TO_MS  1000000

HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion)
{
    HI_U32  ChipId;

#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200)
    ChipId = HI_DRV_SYS_READ_REG(SYS_REG_SYSID, 0);
    if (0x19400200 == ChipId)
    {
        if (penChipType)
        {
            *penChipType = HI_CHIP_TYPE_HI3716CES;
        }

        if (penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_V200;
        }
    }
	else if(0x37160200 == ChipId)
    {
        if (penChipType)
        {
            *penChipType = HI_CHIP_TYPE_HI3716C;
        }

        if (penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_V200;
        }
    }
#else
    ChipId = (0xff & HI_DRV_SYS_READ_REG(SYS_REG_SYSID0, 0));
    ChipId |= (0xff & HI_DRV_SYS_READ_REG(SYS_REG_SYSID1, 0)) << 8;
    ChipId |= (0xff & HI_DRV_SYS_READ_REG(SYS_REG_SYSID2, 0)) << 16;
    ChipId |= (0xff & HI_DRV_SYS_READ_REG(SYS_REG_SYSID3, 0)) << 24;

    if ((ChipId & 0xffff) == 0xb010)
    {
        HI_U32 regv = 0;
        HI_U32 regChipType = 0;

        regv = HI_DRV_SYS_READ_REG(SYS_REG_BASE_ADDR_PHY1,0);
        regChipType = regv >> 14;
        regChipType &= 0x1f;

        if(penChipType)
        {
            /*
            chip version
            0000:Hi3720V100£»
            0001:Hi3716C£»
            0010:Hi3716H£»
            0011:Hi3716M£»
            other: reserve
            */
            switch(regChipType)
            {
                case 0x03:
                    *penChipType = HI_CHIP_TYPE_HI3716M;
                    break;
                case 0x02:
                    *penChipType = HI_CHIP_TYPE_HI3716H;
                    break;
                case 0x01:
                    *penChipType = HI_CHIP_TYPE_HI3716C;
                    break;
                default:
                    *penChipType = HI_CHIP_TYPE_HI3720;
                    break;
            }
        }
        if(penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_V100;
        }
    }
    else if((ChipId & 0xffff) == 0x200)
    {
        HI_U32 regv = 0;
        HI_U32 regChipType = 0;

        regv = HI_DRV_SYS_READ_REG(SYS_REG_BASE_ADDR_PHY2,0);
        regChipType = regv >> 14;
        regChipType &= 0x1f;

        if(penChipType)
        {
            /*
            chip version
            00000:Hi3716L£»
            01000:Hi3716M£»
            01101:Hi3716H£»
            11110:Hi3716C£»
            other: reserve
            */
            switch(regChipType)
            {
                case 0x08:
                    *penChipType = HI_CHIP_TYPE_HI3716M;
                    break;
                case 0x0d:
                    *penChipType = HI_CHIP_TYPE_HI3716H;
                    break;
                case 0x1e:
                    *penChipType = HI_CHIP_TYPE_HI3716C;
                    break;
                default:
                    *penChipType = HI_CHIP_TYPE_HI3720;
                    break;
            }
        }
        if(penChipVersion)
        {
            if(ChipId == 0x37200200)
            {
                *penChipVersion = HI_CHIP_VERSION_V101;
            }
            else
            {
                *penChipVersion = HI_CHIP_VERSION_V200;
            }
         }
    }
    else if (ChipId == 0x37160300)
    {
        if(penChipType)
        {
            *penChipType = HI_CHIP_TYPE_HI3716M;
        }
        if (penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_V300;
        }
    }
    else if (ChipId == 0x37120100)
    {
        if(penChipType)
        {
            *penChipType = HI_CHIP_TYPE_HI3712;
        }
        if (penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_V100;
        }
    }
    else
    {
        if(penChipType)
        {
            *penChipType = HI_CHIP_TYPE_BUTT;
        }
        if(penChipVersion)
        {
            *penChipVersion = HI_CHIP_VERSION_BUTT;
        }

        HI_WARN_SYS("Get CHIP ID error :%x!\n", ChipId);
    }
#endif
}

HI_S32 HI_DRV_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs)
{
    HI_U64 u64TimeNow;

    if (HI_NULL == pu32TimeMs)
    {
        HI_ERR_SYS("null pointer error\n");
        return HI_FAILURE;
    }

    u64TimeNow = sched_clock();

    do_div(u64TimeNow, DIV_NS_TO_MS);

    *pu32TimeMs = (HI_U32)u64TimeNow;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_SYS_KInit(HI_VOID)
{
    return 0;
}

HI_VOID HI_DRV_SYS_KExit(HI_VOID)
{
    return ;
}

EXPORT_SYMBOL(HI_DRV_SYS_GetChipVersion);
EXPORT_SYMBOL(HI_DRV_SYS_GetTimeStampMs);


