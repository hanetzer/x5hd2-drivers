/*   extdrv/peripheral/dc/ov7725.c
 *
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 *
 * History:
 *     04-Apr-2006 create this file
 *
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
//#include <linux/smp_lock.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "drv_gpio_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_module_ext.h"
#include "ov7725.h"
#include "hi_kernel_adapt.h"

/* ov7725 i2c slaver address micro-definition. */
#define I2C_OV7725 0x42

#define DC_COM7 0x12

#define DC_PIDH 0x0A
#define DC_PIDL 0x0B

#define PIDH 0x77
#define PIDL 0x21
#define BIT_NUM_IN_GPIO_GROUP 8

#define OV7725_GPIO_ID_CLOCK 11
#define OV7725_GPIO_CLOCK_BIT 3
#define OV7725_GPIO_ID_DATA 12
#define OV7725_GPIO_DATA_BIT 5

#define REG_DIRECT_OPR 0x8000
#define is_directreg_opt(cmd) ((cmd & REG_DIRECT_OPR) == REG_DIRECT_OPR)

static int out_mode   = 1;
static int s_s32Scene = DC_VAL_AUTO;
static int powerfreq  = DC_VAL_50HZ;
static HI_U32 g_u32I2cNum = 0;

static GPIO_EXT_FUNC_S *s_pGpioFunc = HI_NULL;
static GPIO_I2C_EXT_FUNC_S *s_pGpioI2cFunc = HI_NULL;

/*
 *  ov7725's mirror image get routine.
 *
 *  @return value: ov7725's mirror image enable or disable
 *
 */

static unsigned int dc_mirror_get(void)
{
    HI_U8 regvalue, reg_mirror = DC_VAL_OFF;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    if ((regvalue & 0x40) == 0x00)
    {
        reg_mirror = DC_VAL_ON;
    }
    else if ((regvalue & 0x40) == 0x40)
    {
        reg_mirror = DC_VAL_OFF;
    }

    return reg_mirror;
}

/*
 *  ov7725's mirror image get routine.
 *
 *  @return value: ov7725's mirror image enable or disable
 *
 */

static unsigned int dc_flip_get(void)
{
    HI_U8 regvalue, reg_flip = DC_VAL_OFF;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    if ((regvalue & 0x80) == 0)
    {
        reg_flip = DC_VAL_ON;
    }
    else if ((regvalue & 0x80) == 0x80)
    {
        reg_flip = DC_VAL_OFF;
    }

    return reg_flip;
}

/*
 *  registers initialise for  ov7725's  YCbCr output routine.
 *  YCbCr output 640*480
 *  fps =30
 *
 */

void ov7725_vga_init(void)
{
    HI_U8 regvalue = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return;
    }

    if (powerfreq == DC_VAL_50HZ)
    {
        //int hue;
        //hue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xaa);
        //HI_INFO_OV7725("hue = %x\n",hue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x12, 0x80);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3d, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x17, 0x22);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x18, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x19, 0x07);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x1a, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x32, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0xa0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2a, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x11, 0x01); //00/01/03/07 for 60/30/15/7.5fps

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x42, 0x7f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4d, 0x09);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x63, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x64, 0xff);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x20);

        do
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x66, 0x00);

            s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x66, &regvalue);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
                return;
            }
        } while (0x00 != regvalue);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x67, 0x48);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0d, 0x41); //0x51/0x61/0x71 for different AEC/AGC window
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0f, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x14, 0x11); //0x81
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x7f); //ff/7f/3f/1f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03); //01/03/07/0f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x24, 0x58); //0x80
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x25, 0x30); //5a
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x26, 0xa1); //c1
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00); //ff

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x6b, 0xaa);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 3, 0xff);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x90, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x91, 0x01);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x92, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x93, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x94, 0x5f); //b0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x95, 0x4f); //9d
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x96, 0x11); //13
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x97, 0x1d); //16
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x98, 0x3d); //7b
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x99, 0x5a); //91
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9a, 0x1e); //1e

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9b, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9c, 0x25);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9e, 0x81);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa6, 0x06);

        //modified saturation initialization value by pw 2008-03-04
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, 0x5e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, 0x5e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7e, 0x0c);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7f, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x80, 0x2a);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x81, 0x4e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x82, 0x61);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x83, 0x6f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x84, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x85, 0x86);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x86, 0x8e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x87, 0x97);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x88, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x89, 0xaf);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8a, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8b, 0xd7);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8c, 0xe8);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8d, 0x20);

#if 0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x34, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x40); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x99);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4a, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x49, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4b, 0x14);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4c, 0x17);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x46, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
#endif

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x69, 0x5d);

        do
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, 0x00);
            s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
                return;
            }
        } while (0x00 != regvalue);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0xb2); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef); //Enable banding Filter
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0xab); //Banding Filter Minimum AEC Value
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03); //Banding Filter Step

        //sharpness strength modified by pw 2008-03-05
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xdf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8f, 0x04);

        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x66, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        //        regvalue  = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x66);
        //        regvalue += s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x0c);

        //        HI_INFO_OV7725("config over!0x66:0x0c:0x%x\n", regvalue);
    }

    if (powerfreq == DC_VAL_60HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x12, 0x80);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3d, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x17, 0x22);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x18, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x19, 0x07);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x1a, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x32, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0xa0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2a, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x11, 0x01); //00/01/03/07 for 60/30/15/7.5fps

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x42, 0x7f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4d, 0x09);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x63, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x64, 0xff);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x20);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x66, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x67, 0x48);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0d, 0x41); //0x51/0x61/0x71 for different AEC/AGC window
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0f, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x14, 0x11); //0x81
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x7f); //ff/7f/3f/1f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03); //01/03/07/0f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x24, 0x58); //0x80
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x25, 0x30); //5a
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x26, 0xa1); //c1
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00); //ff
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x6b, 0xaa);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x90, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x91, 0x01);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x92, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x93, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x94, 0xb0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x95, 0x9d);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x96, 0x13);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x97, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x98, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x99, 0x91);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9a, 0x1e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9b, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9c, 0x25);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9e, 0x81);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa6, 0x06);

        //modified saturation initialization value by pw 2008-03-04
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, 0x5e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, 0x5e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7e, 0x0c);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7f, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x80, 0x2a);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x81, 0x4e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x82, 0x61);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x83, 0x6f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x84, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x85, 0x86);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x86, 0x8e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x87, 0x97);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x88, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x89, 0xaf);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8a, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8b, 0xd7);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8c, 0xe8);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8d, 0x20);

#if 0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x34, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x40); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x99);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4a, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x49, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4b, 0x14);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4c, 0x17);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x46, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
#endif

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x69, 0x5d);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x3f); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);

        //sharpness strength modified by pw 2008-03-05
        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xdf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8f, 0x04);
    }
}

void ov7725_qvga_init(void)
{
    HI_U8 regvalue = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return;
    }

    if (powerfreq == DC_VAL_50HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x12, 0x80);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3d, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x17, 0x22);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x18, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x19, 0x07);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x1a, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x32, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0x50);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0x78);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2a, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x11, 0x01); //00/01/03/07 for 60/30/15/7.5fps

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x42, 0x7f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4d, 0x09);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x63, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x64, 0xff);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x2f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x66, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x67, 0x48);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0d, 0x41);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0f, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x14, 0x11);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x7f); //ff/7f/3f/1f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03); //01/03/07/0f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x24, 0x58);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x25, 0x30);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x26, 0xa1);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x6b, 0xaa);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x90, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x91, 0x01);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x92, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x93, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x94, 0x5f); //b0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x95, 0x4f); //9d
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x96, 0x11); //13
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x97, 0x1d); //16
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x98, 0x3d); //7b
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x99, 0x5a); //91
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9a, 0x1e); //1e

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9b, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9c, 0x25);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9e, 0x81);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa6, 0x06);

        //modified saturation initialization value by pw 2008-03-04
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, 0x5e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, 0x5e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7e, 0x0c);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7f, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x80, 0x2a);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x81, 0x4e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x82, 0x61);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x83, 0x6f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x84, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x85, 0x86);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x86, 0x8e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x87, 0x97);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x88, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x89, 0xaf);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8a, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8b, 0xd7);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8c, 0xe8);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8d, 0x20);
#if 0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x34, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x40);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x99);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4a, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x49, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4b, 0x14);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4c, 0x17);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x46, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
#endif
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x69, 0x5d);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x49); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x34, 0x01); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef); //Enable banding Filter

        //sharpness strength modified by pw 2008-03-05
        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xdf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8f, 0x04);
    }

    if (powerfreq == DC_VAL_60HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x12, 0x80);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3d, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x17, 0x22);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x18, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x19, 0x07);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x1a, 0xf0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x32, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0x50);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0x78);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2a, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x11, 0x01); //00/01/03/07 for 60/30/15/7.5fps

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x42, 0x7f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4d, 0x09);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x63, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x64, 0xff);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x2f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x66, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x67, 0x48);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xe0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0d, 0x41);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0f, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x14, 0x11);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x7f); //ff/7f/3f/1f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03); //01/03/07/0f for 60/30/15/7.5fps
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x24, 0x58);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x25, 0x30);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x26, 0xa1);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x6b, 0xaa);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x90, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x91, 0x01);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x92, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x93, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x94, 0xb0);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x95, 0x9d);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x96, 0x13);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x97, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x98, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x99, 0x91);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9a, 0x1e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9b, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9c, 0x25);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9e, 0x81);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa6, 0x06);

        //modified saturation initialization value by pw 2008-03-04
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, 0x5e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, 0x5e);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7e, 0x0c);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x7f, 0x16);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x80, 0x2a);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x81, 0x4e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x82, 0x61);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x83, 0x6f);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x84, 0x7b);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x85, 0x86);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x86, 0x8e);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x87, 0x97);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x88, 0xa4);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x89, 0xaf);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8a, 0xc5);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8b, 0xd7);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8c, 0xe8);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8d, 0x20);
#if 0
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x34, 0x00);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0x40);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x22, 0x99);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x23, 0x03);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4a, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x49, 0x10);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4b, 0x14);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x4c, 0x17);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x46, 0x05);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
#endif
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x69, 0x5d);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, 0x00);

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x33, 0xfb); //0x66/0x99
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);

        //sharpness strength modified by pw 2008-03-05
        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xdf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8f, 0x04);
    }
}

void ov7725_vga_ioctl(void)
{
    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return;
    }

    if (powerfreq == DC_VAL_50HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00);
    }
    else if (powerfreq == DC_VAL_60HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0xff);
    }

    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0xa0);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0xf0);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x20);
}

void  ov7725_qvga_ioctl(void)
{
    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return;
    }

    if (powerfreq == DC_VAL_50HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00);
    }
    else if (powerfreq == DC_VAL_60HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0xff);
    }

    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0x50);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0x78);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x2f);
}

void  ov7725_qqvga_ioctl(void)
{
    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return;
    }

    if (powerfreq == DC_VAL_50HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00);
    }
    else if (powerfreq == DC_VAL_60HZ)
    {
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0xff);
    }

    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x29, 0x28);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2c, 0x40);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x65, 0x2f);
}

/*
 * ov7725 open routine.
 * do nothing.
 *
 */

int OV7725_DRV_Open(struct inode * inode, struct file * file)
{
    return 0;
}

/*
 * ov7725 close routine.
 * do nothing.
 *
 */

int OV7725_DRV_Close(struct inode * inode, struct file * file)
{
    return 0;
}

int OV7725_DRV_Ioctl_reg(unsigned int oprcmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;

#define REG_OPR_READ_MASK 0x8000
#define REG_OPR_WRITE_MASK 0xC000
#define REG_ADDR_MASK 0x00FF

    unsigned int regaddr = 0;
    HI_U8 regvalue   = 0;
    unsigned int val = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    if ((oprcmd & REG_DIRECT_OPR) == 0)
    {
        return -1;
    }

    regaddr = oprcmd & REG_ADDR_MASK;

    if ((oprcmd & REG_OPR_WRITE_MASK) == REG_OPR_WRITE_MASK)/*WRITE*/
    {
        if (copy_from_user(&val, argp, sizeof(val)))
        {
            return -EFAULT;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, regaddr, val);
    }
    else if ((oprcmd & REG_OPR_READ_MASK) == REG_OPR_READ_MASK)  /*READ*/
    {
        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, regaddr);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, regaddr, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &regvalue, sizeof(regvalue)) ? -EFAULT : 0;
    }
    else
    {
        return -1;
    }

    return 0;
}

/*
 * ov7725 ioctl routine.
 * @param inode: pointer of the node;
 * @param file: pointer of the file;
 *
 * @param cmd: command from the app:
 * DC_MODE_SET(1):set ov7725's output format;
 * DC_MODE_GET(2):get ov7725's output format;
 * DC_ZOOM_SET(3):set ov7725's output drive capability;
 * DC_ZOOM_GET(4):get ov7725's output drive capability;
 * DC_MIRROR_SET(5):set ov7725's mirror enable or disable;
 * DC_MIRROR_GET(6):get ov7725's mirror enable or disable;;
 * DC_NIGHTMODE_SET(7):set ov7725's night mode;
 * DC_NIGHTMODE_GET(8):get ov7725's night mode;
 * DC_SYNCMODE_SET(9):set ov7725's sync mode;
 * DC_SYNCMODE_GET(a):get ov7725's sync mode;
 *
 * @param arg:arg from app layer.
 *
 * @return value:0-- set success; 1-- set error.
 *
 */

static long OV7725_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;
    unsigned int val = 0;
    HI_U8 regvalue = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    if (is_directreg_opt(cmd))
    {
        return OV7725_DRV_Ioctl_reg(cmd, arg);
    }

    if (copy_from_user(&val, argp, sizeof(val)))
    {
        return -EFAULT;
    }

    switch (cmd)
    {
    case DC_SET_IMAGESIZE:
    {
        unsigned int imagesize = val;
        if (imagesize == DC_VAL_VGA)
        {
            ov7725_vga_ioctl();
        }
        else if (imagesize == DC_VAL_QVGA)
        {
            ov7725_qvga_ioctl();
        }
        else if (imagesize == DC_VAL_QQVGA)
        {
            ov7725_qqvga_ioctl();
        }
        else
        {
            /*HI_ERR_OV7725("imagesize_set_error.\n");*/
            return -1;
        }

        break;
    }

    case DC_SET_BRIGHT:
    {
        unsigned int bright = val;
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9b, bright);
        break;
    }

    case DC_SET_CONTRACT:
    {
        unsigned int contrast = val;
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x9c, contrast);
        break;
    }

    case DC_SET_HUE:
    {
        unsigned int hue = val;

        hue   &= 0xFF;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa6, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa6);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa6, regvalue | 0x01);
        HI_INFO_OV7725("--------------------set hue : %x\n", hue);
        if (hue < 0x80)
        {
            HI_INFO_OV7725("--------------------set hue+0x80 : %x\n", hue + 0x80);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xaa, (hue + 0x80));
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa9, (hue + 0x80));
        }
        else if ((hue >= 0x80) && (hue <= 0xff))
        {
            unsigned int u8Hue = hue;
            HI_INFO_OV7725("--------------------set ~u8Hue : %hx\n", ~u8Hue);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xaa, ~u8Hue);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa9, u8Hue - 0x80);
        }
        else
        {
            HI_ERR_OV7725("hue_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_SATURATION:
    {
        unsigned int saturation = val;
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, saturation);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, saturation);
        break;
    }
    case DC_SET_SHARPNESS:
    {
        unsigned int sharpness = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xdf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8f, sharpness);
        break;
    }

    case DC_SET_ADNSWITCH:
    {
        unsigned int adnswitch = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        if (adnswitch == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, (0x40 | regvalue));
        }
        else if (adnswitch == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, (0xbf & regvalue));
        }
        else
        {
            HI_ERR_OV7725("adnswitch_set_error.\n");
            return -1;
        }

        break;
    }
    case DC_SET_DNT:
    {
        unsigned int dnt = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xac, 0xbf & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x8e, dnt);
        break;
    }

    case DC_SET_AWBSWITCH:
    {
        unsigned int awbswitch = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        if (awbswitch == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0x02 | regvalue));
        }
        else if (awbswitch == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0xfd & regvalue));
        }

        break;
    }
    case DC_SET_WBR:
    {
        unsigned int wbr = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0xfd & regvalue));
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x02, wbr);
        break;
    }

    case DC_SET_WBB:
    {
        unsigned int wbb = val;

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0xfd & regvalue));
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x01, wbb);
        break;
    }

    case DC_SET_AECSWITCH:
    {
        unsigned int aecswitch = val;

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (aecswitch == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0x01 | regvalue));
        }
        else if (aecswitch == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0xfe & regvalue));
        }
        else
        {
            HI_ERR_OV7725("aecswitch_set_error.\n");
            return -1;
        }

        break;
    }
    case DC_SET_EC:
    {
        unsigned int ec = val;
        if (ec >= 0x80)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x10, 0xff);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x08, (ec - 0x80) * 2);
        }
        else if (ec < 0x80)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x08, 0x00);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x10, ec * 2);
        }
        else
        {
            HI_ERR_OV7725("ec_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_AGCSWITCH:
    {
        unsigned int agcswitch = val;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        if (agcswitch == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0x04 | regvalue));
        }
        else if (agcswitch == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, (0xfb & regvalue));
        }
        else
        {
            HI_ERR_OV7725("agcswitch_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_GC:
    {
        unsigned int agc = val;

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xfb & regvalue);
        s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x00, agc);
        break;
    }

    case DC_SET_ABLCSWITCH:
    {
        unsigned int ablcswitch = val;

        //      regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x3e);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x3e, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (ablcswitch == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3e, (0x04 | regvalue));
        }
        else if (ablcswitch == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x3e, (0xfb & regvalue));
        }
        else
        {
            HI_ERR_OV7725("ablcswitch_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_COLOR:
    {
        HI_U8 color = val, precolor_u, precolor_v;

        //        precolor_u = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa7);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa7, &precolor_u);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //        precolor_v = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa8);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa8, &precolor_v);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        HI_INFO_OV7725("set_color:color=%x,precolor_u = %x, precolor_v = %x\n", color, precolor_u, precolor_v);
        if (color == DC_VAL_COLOR)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, precolor_u);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, precolor_v);
        }
        else if (color == DC_VAL_BAW)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa7, 0x00);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0xa8, 0x00);
        }
        else
        {
            HI_ERR_OV7725("color_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_SCENE:
    {
        unsigned int scene = val;
        if (scene == DC_VAL_OUTDOOR)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xed);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x01, 0x5a);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x02, 0x49);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2d, 0x00);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2e, 0x00);
            s_s32Scene = DC_VAL_OUTDOOR;
        }
        else if (scene == DC_VAL_INDOOR)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xed);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x01, 0x70);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x02, 0x46);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0e, 0x65);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2d, 0x00);
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2e, 0x00);
            s_s32Scene = DC_VAL_INDOOR;
        }
        else if (scene == DC_VAL_AUTO)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xef);
            s_s32Scene = DC_VAL_AUTO;
        }
        else if (scene == DC_VAL_MANUAL)
        {
            s_s32Scene = DC_VAL_MANUAL;
        }
        else
        {
            HI_ERR_OV7725("scene_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_POWERFREQ:
    {
        unsigned int powerfreq_dy = val;
        if (powerfreq_dy == DC_VAL_50HZ)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0x00);
            powerfreq = DC_VAL_50HZ;
        }
        else if (powerfreq_dy == DC_VAL_60HZ)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x2b, 0xff);
            powerfreq = DC_VAL_60HZ;
        }
        else
        {
            HI_ERR_OV7725("powerfreq_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_FLIP:
    {
        unsigned int flip = val;

        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x0c);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (flip == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, (regvalue & 0x7f));
        }
        else if (flip == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, (regvalue | 0x80));
        }
        else
        {
            HI_ERR_OV7725("flip_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_SET_MIRROR:
    {
        unsigned int mirror = val;

        //        regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x0c);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x0c, &regvalue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (mirror == DC_VAL_ON)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, (regvalue & 0xbf));
        }
        else if (mirror == DC_VAL_OFF)
        {
            s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x0c, (regvalue | 0x40));
        }
        else
        {
            HI_ERR_OV7725("mirror_set_error.\n");
            return -1;
        }

        break;
    }

    case DC_GET_IMAGESIZE:
    {
        HI_U8 imagesize, dc_size;

        //        imagesize = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x29);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x29, &imagesize);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (imagesize == 0xa0)
        {
            dc_size = DC_VAL_VGA;
        }

        if (imagesize == 0x50)
        {
            dc_size = DC_VAL_QVGA;
        }

        if (imagesize == 0x25)
        {
            dc_size = DC_VAL_QQVGA;
        }

        return copy_to_user(argp, &dc_size, 1) ? -EFAULT : 0;
    }

    case DC_GET_BRIGHT:
    {
        HI_U8 birght;

        //        birght = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x9b);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x9b, &birght);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &birght, 1) ? -EFAULT : 0;
    }

    case DC_GET_CONTRACT:
    {
        HI_U8 contrast;

        //        contrast = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x9c);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x9c, &contrast);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &contrast, 1) ? -EFAULT : 0;
    }

    case DC_GET_HUE:
    {
        unsigned int hue;
        HI_U8 u8Hue;

        //        u8Hue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa9);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa9, &u8Hue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        HI_INFO_OV7725("======hue a9 = %x\n", u8Hue);

        //        u8Hue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xaa);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xaa, &u8Hue);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (u8Hue < 0x80)
        {
            hue = ~u8Hue;
        }
        else
        {
            hue = u8Hue - 0x80;
        }

        return copy_to_user(argp, &hue, 1) ? -EFAULT : 0;
    }

    case DC_GET_SATURATION:
    {
        HI_U8 saturation;

        //        saturation = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa7);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa7, &saturation);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &saturation, 1) ? -EFAULT : 0;
    }

    case DC_GET_SHARPNESS:
    {
        HI_U8 sharpness;

        //        sharpness = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x8f);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x8f, &sharpness);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &sharpness, 1) ? -EFAULT : 0;
    }

    case DC_GET_ADNSWITCH:
    {
        HI_U8 adnswitch, adnswitch_value;

        //        adnswitch = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xac);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xac, &adnswitch);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        adnswitch = adnswitch & 0x40;
        if (adnswitch == 0x40)
        {
            adnswitch_value = DC_VAL_ON;
        }
        else
        {
            adnswitch_value = DC_VAL_OFF;
        }

        return copy_to_user(argp, &adnswitch_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_DNT:
    {
        HI_U8 dnt;

        //        dnt = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x8e);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x8e, &dnt);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &dnt, 1) ? -EFAULT : 0;
    }

    case DC_GET_AWBSWITCH:
    {
        HI_U8 awbswitch, awbswitch_value;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &awbswitch);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //        awbswitch = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        awbswitch = awbswitch & 0x02;
        if (awbswitch == 0x02)
        {
            awbswitch_value = DC_VAL_ON;
        }
        else
        {
            awbswitch_value = DC_VAL_OFF;
        }

        return copy_to_user(argp, &awbswitch_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_WBR:
    {
        HI_U8 wbr;

        //s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, 0x13, 0xed);
        //        wbr = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x02);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x02, &wbr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &wbr, 1) ? -EFAULT : 0;
    }

    case DC_GET_WBB:
    {
        HI_U8 wbb;

        //        wbb = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x01);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x01, &wbb);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &wbb, 1) ? -EFAULT : 0;
    }

    case DC_GET_AECSWITCH:
    {
        HI_U8 aecswitch, aecswitch_value;

        //        aecswitch = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &aecswitch);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        aecswitch = aecswitch & 0x01;
        if (aecswitch == 0x01)
        {
            aecswitch_value = DC_VAL_ON;
        }
        else
        {
            aecswitch_value = DC_VAL_OFF;
        }

        return copy_to_user(argp, &aecswitch_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_EC:
    {
        HI_U8 aecswitch_lower, aecswitch_upper, aecswitch_value;

        //        aecswitch_lower = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x10);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x10, &aecswitch_lower);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //        aecswitch_upper = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x08);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x08, &aecswitch_upper);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (aecswitch_lower == 0xff)
        {
            aecswitch_value = aecswitch_upper / 2 + 0x80;
        }
        else
        {
            aecswitch_value = aecswitch_lower / 2;
        }

        return copy_to_user(argp, &aecswitch_value, 1) ? -EFAULT : 0;
    }
    case DC_GET_AGCSWITCH:
    {
        HI_U8 agcswitch, agcswitch_value;
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x13, &agcswitch);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        //        agcswitch = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x13);
        agcswitch = agcswitch & 0x04;
        if (agcswitch == 0x04)
        {
            agcswitch_value = DC_VAL_ON;
        }
        else
        {
            agcswitch_value = DC_VAL_OFF;
        }

        return copy_to_user(argp, &agcswitch_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_GC:
    {
        HI_U8 gc;

        //        gc = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x00);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x00, &gc);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        return copy_to_user(argp, &gc, 1) ? -EFAULT : 0;
    }

    case DC_GET_ABLCSWITCH:
    {
        HI_U8 ablcswitch, ablcswitch_value;

        //        ablcswitch = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0x3e);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0x3e, &ablcswitch);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        ablcswitch = ablcswitch & 0x04;
        if (ablcswitch == 0x04)
        {
            ablcswitch_value = DC_VAL_ON;
        }
        else
        {
            ablcswitch_value = DC_VAL_OFF;
        }

        return copy_to_user(argp, &ablcswitch_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_COLOR:
    {
        HI_U8 color, color_value;

        //        color = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, 0xa7);
        s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, 0xa7, &color);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
            return HI_FAILURE;
        }

        if (color > 0x00)
        {
            color_value = DC_VAL_COLOR;
        }
        else
        {
            color_value = DC_VAL_BAW;
        }

        HI_INFO_OV7725("color_value = %x\n", color_value);
        return copy_to_user(argp, &color_value, 1) ? -EFAULT : 0;
    }

    case DC_GET_SCENE:
    {
        return copy_to_user(argp, &s_s32Scene, 1) ? -EFAULT : 0;
    }

    case DC_GET_POWERFREQ:
    {
        return copy_to_user(argp, &powerfreq, 1) ? -EFAULT : 0;
    }

    case DC_GET_FLIP:
    {
        unsigned int reg_flip;
        reg_flip = dc_flip_get();
        return copy_to_user(argp, &reg_flip, 1) ? -EFAULT : 0;
    }

    case DC_GET_MIRROR:
    {
        unsigned int reg_mirror1;
        reg_mirror1 = dc_mirror_get();
        return copy_to_user(argp, &reg_mirror1, 1) ? -EFAULT : 0;
    }
    default:
        return -1;
    }

    return 0;
}

/*
 *  The various file operations we support.
 */

static struct file_operations ov7725_fops =
{
    .owner			= THIS_MODULE,
    .unlocked_ioctl = OV7725_DRV_Ioctl,
    .open			= OV7725_DRV_Open,
    .release		= OV7725_DRV_Close
};

static struct miscdevice ov7725_dev =
{
    MISC_DYNAMIC_MINOR,
    "ov7725",
    &ov7725_fops,
};

static int ov7725_device_init(void)
{
    HI_U8 regvalue, regvalue2;
    int loop1;
    HI_S32 s32Ret = HI_FAILURE;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, DC_COM7, 0x80);

    for (loop1 = 0; loop1 < 5000; loop1++)
    {
        ;
    }

    //    regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, DC_COM7);
    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, DC_COM7, &regvalue);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    regvalue &= 0x7f;
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, DC_COM7, regvalue);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, DC_COM7, regvalue);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, I2C_OV7725, DC_COM7, regvalue);

    //    regvalue = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, DC_PIDH);
    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, DC_PIDH, &regvalue);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    //    loop1 = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, DC_PIDL);
    //    loop1 = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(I2C_OV7725, DC_PIDL);
    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, DC_PIDL, &regvalue2);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    s32Ret = s_pGpioI2cFunc->pfnGpioI2cSCCBRead(g_u32I2cNum, I2C_OV7725, DC_PIDL, &regvalue2);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OV7725("GpioI2cSCCBRead failed\n");
        return HI_FAILURE;
    }

    if ((regvalue != PIDH) || (regvalue2 != PIDL))
    {
        HI_ERR_OV7725("read Prodect ID Number MSB is %x\n", regvalue);
        HI_ERR_OV7725("read Prodect ID Number LSB is %x\n", regvalue2);
        HI_ERR_OV7725("check ov7725 ID error.\n");

        //return -EFAULT;
    }

    if (out_mode == 1)
    {
        ov7725_vga_init();
    }
    else
    {
        ov7725_qvga_init();
    }

    return 0;
}

static int __init OV7725_DRV_ModInit(void)
{
    int ret = 0;

    if (HI_DRV_MODULE_Register(HI_ID_OV7725, OV7725_NAME, HI_NULL))
    {
        HI_ERR_OV7725("Register OV7725 module failed.\n");
        return HI_FAILURE;
    }

    s_pGpioFunc = HI_NULL;
    HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&s_pGpioFunc);
    if (!s_pGpioFunc || !s_pGpioFunc->pfnGpioDirSetBit || !s_pGpioFunc->pfnGpioWriteBit)
    {
        HI_ERR_OV7725("GPIO not found\n");
        return HI_FAILURE;
    }

    s_pGpioI2cFunc = HI_NULL;
    HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&s_pGpioI2cFunc);
    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cSCCBRead || !s_pGpioI2cFunc->pfnGpioI2cWrite
        || !s_pGpioI2cFunc->pfnGpioI2cCreateChannel)
    {
        HI_ERR_OV7725("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    ret = s_pGpioI2cFunc->pfnGpioI2cCreateChannel(&g_u32I2cNum, OV7725_GPIO_ID_CLOCK * 8 + OV7725_GPIO_CLOCK_BIT,
                                                  OV7725_GPIO_ID_DATA * 8 + OV7725_GPIO_DATA_BIT);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OV7725("GpioI2cCreateChannel failed!\n");
        return HI_FAILURE;
    }

    /*liusanwei reset ov7725*/
    s_pGpioFunc->pfnGpioDirSetBit(0 * BIT_NUM_IN_GPIO_GROUP + 1, HI_FALSE);
    HI_INFO_OV7725("reset ov7725.............. .....\n");
    s_pGpioFunc->pfnGpioWriteBit(0 * BIT_NUM_IN_GPIO_GROUP + 1, 0);
    s_pGpioFunc->pfnGpioWriteBit(0 * BIT_NUM_IN_GPIO_GROUP + 1, 1);

    ret = misc_register(&ov7725_dev);
    if (ret)
    {
        HI_ERR_OV7725("could not register ov7725 devices. \n");
        return ret;
    }

    if (ov7725_device_init() < 0)
    {
        misc_deregister(&ov7725_dev);
        HI_ERR_OV7725("ov7725 driver init fail for device init error!\n");
        return -1;
    }

#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_ov7725.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return ret;
}

static void __exit OV7725_DRV_ModExit(void)
{
    if (s_pGpioI2cFunc)
    {
        s_pGpioI2cFunc->pfnGpioI2cDestroyChannel(g_u32I2cNum);
    }

    misc_deregister(&ov7725_dev);
    HI_DRV_MODULE_UnRegister(HI_ID_OV7725);
}

module_init(OV7725_DRV_ModInit);
module_exit(OV7725_DRV_ModExit);

module_param(out_mode, int, S_IRUGO);
module_param(s_s32Scene, int, S_IRUGO);

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
