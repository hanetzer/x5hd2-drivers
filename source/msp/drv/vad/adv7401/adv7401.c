/*
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
 * along with this program;
 *
 * History:
 *      15-Jan-2011 create this file
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
//#include <linux/smp_lock.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>

#include "drv_i2c_ext.h"
#include "drv_gpio_ext.h"
#include "drv_module_ext.h"
#include "adv7401.h"
#include "hi_kernel_adapt.h"

#define ADV7401_NAME "HI_ADV7401"
#define BIT_NUM_IN_GPIO_GROUP 8

static HI_U32 AdvState = 0;
static I2C_EXT_FUNC_S  *s_pI2CFunc  = HI_NULL;
static GPIO_EXT_FUNC_S *s_pGpioFunc = HI_NULL;

static HI_S32 g_GpioPathNum = 6;
static HI_S32 g_GpioBit = 3;

HI_U32 g_7401I2cNum = 3;
HI_U32 g_7401RdDevAddr = 0x43;
HI_U32 g_7401WrDevAddr = 0x42;

typedef struct
{
    HI_CHAR u8SubAddr;
    HI_CHAR u8Data;
} ADV7401_FMT_S;

ADV7401_FMT_S g_Adv7401Fmt[MAX_ADV7401_FMT_NUM][MAX_ADV7401_REG_CFG_NUM] =
{
    /* must define in order! */
    /* ADV7401_FMT_480I */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x00}, {0x06, 0x0a}, {0x1d, 0x47},
        {0x3a, 0x11}, {0x3b, 0x80}, {0x3c, 0x52}, {0x6b, 0xc3}, {0x7b, 0x06},
        {0x85, 0x19}, {0x86, 0x0b}, {0x8f, 0x77}, {0x90, 0x1c}, {0xc5, 0x01},
        {0xc9, 0x0c}, {0xf3, 0x07}, {0x0e, 0x80}, {0x52, 0x46}, {0x54, 0x00},
        {0x0e, 0x00},
    },

    /* ADV7401_FMT_576I */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x00}, {0x06, 0x0b}, {0x1d, 0x47},
        {0x3a, 0x11}, {0x3b, 0x80}, {0x3c, 0x52}, {0x6b, 0xc3}, {0x7b, 0x06},
        {0x85, 0x19}, {0x86, 0x0b}, {0x8f, 0x77}, {0x90, 0x29}, {0xc5, 0x01},
        {0xc9, 0x0c}, {0xf3, 0x07}, {0x0e, 0x80}, {0x52, 0x46}, {0x54, 0x00},
        {0x0e, 0x00},
    },

    /* ADV7401_FMT_480P */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x06}, {0x1d, 0x47},
        {0x3a, 0x11}, {0x3b, 0x80}, {0x3c, 0x5c}, {0x6b, 0xc3}, {0x85, 0x19},
        {0x86, 0x1b},
    },

    /* ADV7401_FMT_576P */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x07}, {0x1d, 0x47},
        {0x3a, 0x11}, {0x3b, 0x80}, {0x3c, 0x5c}, {0x6b, 0xc3}, {0x85, 0x19},
        {0x86, 0x1b},
    },

    /* ADV7401_FMT_720P_50 */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x0a}, {0x1d, 0x47},
        {0x3a, 0x21}, {0x3b, 0x80}, {0x3c, 0x5c}, {0x6b, 0xc1}, {0x85, 0x19},
        {0x86, 0x1b}, {0x87, 0xe7}, {0x88, 0xbc}, {0x8f, 0x02}, {0x90, 0xfc},
    },

    /* ADV7401_FMT_720P_60 */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x0a}, {0x1d, 0x47},
        {0x3a, 0x21}, {0x3b, 0x80}, {0x3c, 0x5d}, {0x6b, 0xc3}, {0x85, 0x19},
        {0x86, 0x1b}, {0x87, 0xe7}, {0x0e, 0x80}, {0x52, 0x46}, {0x54, 0x00},
        {0x0e, 0x00},
    },

    /* ADV7401_FMT_1080I_50 */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x0c}, {0x1d, 0x47},
        {0x3a, 0x20}, {0x3b, 0x80}, {0x3c, 0x5c}, {0x6b, 0xc3}, {0x85, 0x19},
        {0x86, 0x1b}, {0x87, 0xea}, {0x88, 0x50}, {0x8f, 0x03}, {0x90, 0xfa},
    },

    /* ADV7401_FMT_1080I_60 */
    {
        {0x0f, 0x80}, {0x0f, 0x00}, {0x05, 0x01}, {0x06, 0x0c}, {0x1d, 0x47},
        {0x3a, 0x21}, {0x3b, 0x80}, {0x3c, 0x5d}, {0x6b, 0xc3}, {0x85, 0x19},
        {0x86, 0x1b},
    },
};

module_param(g_GpioPathNum, int, S_IRUGO);
module_param(g_GpioBit, int, S_IRUGO);

HI_S32 ADV7401_Read(HI_U32 u32SubAddr, HI_U8 *pData)
{
    HI_S32 Ret;

    Ret = (s_pI2CFunc->pfnI2cRead)(g_7401I2cNum, g_7401RdDevAddr, u32SubAddr, 1, pData, 1);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_ADV7401("call HI_I2C_Read failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ADV7401_Write(HI_U32 u32SubAddr, HI_U8 *pData)
{
    HI_S32 Ret;

    Ret = (s_pI2CFunc->pfnI2cWrite)(g_7401I2cNum, g_7401WrDevAddr, u32SubAddr, 1, pData, 1);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_ADV7401("call HI_I2C_Write failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AD7401_Reset(HI_U32 u32GpioPathNum, HI_U32 u32BitX)
{
    HI_S32 Ret;

    Ret = (s_pGpioFunc->pfnGpioDirSetBit)(u32GpioPathNum * BIT_NUM_IN_GPIO_GROUP + u32BitX, HI_FALSE);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_ADV7401("call hi_gpio_dirset_bit failed\n");
        return Ret;
    }

    Ret = (s_pGpioFunc->pfnGpioWriteBit)(u32GpioPathNum * BIT_NUM_IN_GPIO_GROUP + u32BitX, HI_FALSE);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_ADV7401("call hi_gpio_write_bit failed\n");
        return Ret;
    }

    msleep(100);

    Ret = (s_pGpioFunc->pfnGpioWriteBit)(u32GpioPathNum * BIT_NUM_IN_GPIO_GROUP + u32BitX, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_ADV7401("call hi_gpio_write_bit failed\n");
        return Ret;
    }

    msleep(100);

    return HI_SUCCESS;
}

HI_S32 ADV7401_SetFmt(ADV7401_FMT_E eAdv7401FMT)
{
    HI_U32 u32Count = 0;

    if ((eAdv7401FMT < ADV7401_FMT_480I) || (eAdv7401FMT > ADV7401_FMT_1080I_60))
    {
        HI_ERR_ADV7401("ADV7401_SetFmt Unrecongnised format\n");
        return HI_FAILURE;
    }

    for (u32Count = 0; u32Count < MAX_ADV7401_REG_CFG_NUM; u32Count++)
    {
        if (u32Count == 1)
        {
            msleep(20);
        }

        if (g_Adv7401Fmt[eAdv7401FMT][u32Count].u8SubAddr == 0x00)
        {
            break;
        }

        ADV7401_Write(g_Adv7401Fmt[eAdv7401FMT][u32Count].u8SubAddr, &(g_Adv7401Fmt[eAdv7401FMT][u32Count].u8Data));

        //        ADV7401_Read(g_Adv7401Fmt[eAdv7401FMT][u32Count].u8SubAddr, &(g_Adv7401Fmt[eAdv7401FMT][u32Count].u8Data));
        HI_INFO_ADV7401("Adv7401 read reg offset %x: %x !\n", g_Adv7401Fmt[eAdv7401FMT][u32Count].u8SubAddr,
                        g_Adv7401Fmt[eAdv7401FMT][u32Count].u8Data);
    }

    return HI_SUCCESS;
}

static HI_S32 ADV7401_DRV_Open(struct inode * inode, struct file * file)
{
    if (0 == AdvState)
    {
        AdvState = 1;

        s_pI2CFunc = HI_NULL;

        HI_DRV_MODULE_GetFunction(HI_ID_I2C, (HI_VOID**)&s_pI2CFunc);

        if (!s_pI2CFunc || !s_pI2CFunc->pfnI2cWrite || !s_pI2CFunc->pfnI2cRead)
        {
            HI_ERR_ADV7401("I2C not found\n");

            return HI_FAILURE;
        }

        s_pGpioFunc = HI_NULL;

        HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&s_pGpioFunc);

        if (!s_pGpioFunc || !s_pGpioFunc->pfnGpioDirSetBit || !s_pGpioFunc->pfnGpioWriteBit)
        {
            HI_ERR_ADV7401("GPIO not found\n");

            return HI_FAILURE;
        }

        AD7401_Reset(g_GpioPathNum, g_GpioBit);
    }

    return 0;
}

static HI_S32 ADV7401_DRV_Close(struct inode * inode, struct file * file)
{
    return 0;
}

static long ADV7401_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long Ret = -ENOIOCTLCMD;

    switch (cmd)
    {
    case ADV7401_SET_FMT:
        Ret = ADV7401_SetFmt(arg);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_ADV7401("ADV7401_SetFmt failed.\n");
        }

        break;

    default:
        HI_ERR_ADV7401("Unrecongnised command.\n");
        Ret = -EINVAL;
        break;
    }

    return Ret;
}

static struct file_operations adv7401_fops =
{
    .owner			= THIS_MODULE,
    .unlocked_ioctl = ADV7401_DRV_Ioctl,
    .open			= ADV7401_DRV_Open,
    .release		= ADV7401_DRV_Close
};

static struct miscdevice adv7401_dev =
{
    MISC_DYNAMIC_MINOR,
    "adv7401",
    &adv7401_fops,
};

static HI_S32 __init ADV7401_DRV_ModInit(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register(HI_ID_ADV7401, ADV7401_NAME, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = misc_register(&adv7401_dev);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_ADV7401("could not register adv7401 devices.\n");
        return ret;
    }

#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_adv7401.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return 0;
}

static HI_VOID __exit ADV7401_DRV_ModExit(HI_VOID)
{
    misc_deregister(&adv7401_dev);

    HI_DRV_MODULE_UnRegister(HI_ID_ADV7401);
}

module_init(ADV7401_DRV_ModInit);
module_exit(ADV7401_DRV_ModExit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");
