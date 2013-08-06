/* extdrv/peripheral/vad/tw2815a.c
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
 *      10-April-2006 create this file
 *      2006-04-29  add record path half d1 mod
 *      2006-05-13  set the playpath default output mod to full
 *      2006-05-24  add record mod 2cif
 *      2006-06-15  support mod changing between every record mod
 *      2006-08-12  change the filters when record mod change
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
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>

#include "tw2864.h"
#include "drv_gpioi2c_ext.h"
#include "drv_module_ext.h"
#include "hi_kernel_adapt.h"

#define setd1(x)
#define tw2815_device_video_init(x, y) 0
#define DEBUG_2815 1

#define TW2864_GPIO_ID_CLOCK 11
#define TW2864_GPIO_CLOCK_BIT 3
#define TW2864_GPIO_ID_DATA 12
#define TW2864_GPIO_DATA_BIT 5

static GPIO_I2C_EXT_FUNC_S *s_pGpioI2cFunc = HI_NULL;
static HI_U32 g_u32I2cNum = 0;

#if 0
static unsigned char gpio_i2c1_read(unsigned char devaddress, unsigned char address)
{
    return s_pGpioI2cFunc->pfnGpioI2cRead(devaddress, address);
}

#endif

static unsigned char gpio_i2c0_read(unsigned char devaddress, unsigned char address)
{
    HI_U8 value = 0xff;

    s_pGpioI2cFunc->pfnGpioI2cRead(g_u32I2cNum, devaddress, address, &value);
    return value;
}

static void gpio_i2c1_write(unsigned char devaddress, unsigned char address, unsigned char data)
{
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, devaddress, address, data);
}

static void gpio_i2c0_write(unsigned char devaddress, unsigned char address, unsigned char data)
{
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, devaddress, address, data);
}

static void tw2815_write_table(unsigned char chip_addr, unsigned char addr, unsigned char *tbl_ptr, unsigned tbl_cnt)
{
    unsigned char i;

    if ((chip_addr == 0x50) || (chip_addr == 0x54))
    {
        for (i = 0; i < tbl_cnt; i++)
        {
            gpio_i2c0_write(chip_addr, (addr + i), *(tbl_ptr + i));
        }
    }
    else
    {
        for (i = 0; i < tbl_cnt; i++)
        {
            gpio_i2c1_write(chip_addr, (addr + i), *(tbl_ptr + i));
        }
    }
}

int tw2864_video_mode_init(unsigned chip_addr, unsigned char video_mode, unsigned char ch)
{
    // unsigned char video_mode_ctrl,temp,
    unsigned char mode_temp;

    mode_temp = video_mode;

    if (video_mode == AUTOMATICALLY)
    {
        /*
        video_mode_ctrl = tw2815_byte_read(chip_addr,ch*0x10+0x1);
        CLEAR_BIT(video_mode_ctrl,0x80);
        tw2815_byte_write(chip_addr,ch*0x10+0x01,video_mode_ctrl);
        //usleep(50);//delay for automatically
        mode_temp = (tw2815_byte_read(chip_addr,ch*0x10+0x0))>>5;
        //HI_INFO_TW2864("temp1 = %x\n",mode_temp);
        if(mode_temp <= 3)
        {
            mode_temp = PAL;
        }
        else
        {
            mode_temp = NTSC;
        }
         */
    }

    if (mode_temp == NTSC)
    {
        /*
        tw2815_write_table(chip_addr,0x00+0x10*ch,tbl_ntsc_tw2815_common,15);
        tw2815_write_table(chip_addr,0x40,tbl_ntsc_tw2815_sfr1,16);
        tw2815_write_table(chip_addr,0x50,tbl_ntsc_tw2815_sfr2,10);
         */
        tw2815_write_table(chip_addr, (0x00 + ch * 0x10), tw2864_ntsc_channel, 16);
    }
    else
    {
        /*
        tw2815_write_table(chip_addr,0x00+0x10*ch,tbl_pal_tw2815_common,15);
        tw2815_write_table(chip_addr,0x40,tbl_pal_tw2815_sfr1,16);
        tw2815_write_table(chip_addr,0x50,tbl_pal_tw2815_sfr2,10);
         */
        tw2815_write_table(chip_addr, (0x00 + ch * 0x10), tw2864_pal_channel, 16);
    }

    /*
    temp = tw2815_byte_read(chip_addr,0x43);
    SET_BIT(chip_addr,0x80);
    tw2815_byte_write(chip_addr,0x43,temp);
     */
    return 0;
}

void set_2_d1(unsigned char chip_addr, unsigned char ch1, unsigned char ch2)
{
    /*
    unsigned char temp;
    if(ch1 >3 || ch2 > 3)
    {
        HI_ERR_TW2864("tw2815 video chunnel error\n");
        return;
    }
    temp = tw2815_byte_read(chip_addr,0x43);
    SET_BIT(temp,0x3);
    tw2815_byte_write(chip_addr,0x43,temp);

    temp = tw2815_byte_read(chip_addr,0x0d+ch1*0x10);
    SET_BIT(temp,0x04);
    tw2815_byte_write(chip_addr,(0x0d+ch1*0x10),temp);

    temp = tw2815_byte_read(chip_addr,0x0d+ch1*0x10);
    CLEAR_BIT(temp,0x03);
    SET_BIT(temp,ch2);
    tw2815_byte_write(chip_addr,(0x0d+ch1*0x10),temp);
    return;
     */
    tw2815_write_table(chip_addr, 0x80, tbl_tw2864_common_0x80, 16);
    tw2815_write_table(chip_addr, 0x90, tbl_tw2864_common_0x90, 16);
    tw2815_write_table(chip_addr, 0xa4, tbl_tw2864_common_0xa4, 12);
    tw2815_write_table(chip_addr, 0xb0, tbl_tw2864_common_0xb0, 1);
    tw2815_write_table(chip_addr, 0xc4, tbl_tw2864_common_0xc4, 12);
    tw2815_write_table(chip_addr, 0xd0, tbl_tw2864_common_0xd0, 16);
    tw2815_write_table(chip_addr, 0xe0, tbl_tw2864_common_0xe0, 16);
    tw2815_write_table(chip_addr, 0xf0, tbl_tw2864_common_0xf0, 16);
}

// not change for TW2864

static unsigned char tw2815_byte_write(unsigned char chip_addr, unsigned char addr, unsigned char data)
{
    gpio_i2c0_write(chip_addr, addr, data);
    return 0;
}

static unsigned char tw2815_byte_read(unsigned char chip_addr, unsigned char addr)
{
    return gpio_i2c0_read(chip_addr, addr);
}

void set_4cif(unsigned char chip_addr, unsigned char ch)
{
    unsigned char temp;

    temp = tw2815_byte_read(chip_addr, 0x0d + ch * 0x10);
    CLEAR_BIT(temp, 0x7);
    tw2815_byte_write(chip_addr, 0x0d + ch * 0x10, temp);

    temp = tw2815_byte_read(chip_addr, 0x75);
    CLEAR_BIT(temp, 0xf);
    SET_BIT(temp, 0x1 << ch);
    tw2815_byte_write(chip_addr, 0x75, temp);

    temp = tw2815_byte_read(chip_addr, 0x71);
    CLEAR_BIT(temp, 0x40);
    tw2815_byte_write(chip_addr, 0x71, temp);

    temp = tw2815_byte_read(chip_addr, 0x43);
    SET_BIT(temp, 0x03);
    tw2815_byte_write(chip_addr, 0x43, temp);
    return;
}

static void __exit TW2864_DRV_ModExit(HI_VOID)
{
    if (s_pGpioI2cFunc)
    {
        s_pGpioI2cFunc->pfnGpioI2cDestroyChannel(g_u32I2cNum);
    }

    HI_DRV_MODULE_UnRegister(HI_ID_TW2864);

    return;
}

static int __init TW2864_DRV_ModInit(void)
{
    HI_U32 ui2864Sel;
    HI_U8 tmpReg  = 0x00;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_DRV_MODULE_Register(HI_ID_TW2864, TW2864_NAME, HI_NULL))
    {
        HI_ERR_TW2864("Register TW2864 module failed.\n");
        return HI_FAILURE;
    }

    s_pGpioI2cFunc = HI_NULL;
    HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&s_pGpioI2cFunc);
    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite || !s_pGpioI2cFunc->pfnGpioI2cRead)
    {
        HI_ERR_TW2864("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    s32Ret = s_pGpioI2cFunc->pfnGpioI2cCreateChannel(&g_u32I2cNum, TW2864_GPIO_ID_CLOCK * 8 + TW2864_GPIO_CLOCK_BIT,
                                                     TW2864_GPIO_ID_DATA * 8 + TW2864_GPIO_DATA_BIT);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TW2864("GpioI2cCreateChannel failed!\n");
        return HI_FAILURE;
    }

    ui2864Sel = tw2864_chipID(2);
    tw2864_video_mode_init(ui2864Sel, 2, 0);
    tw2864_setd1(ui2864Sel);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, ui2864Sel, 0x9f, 0xff);
#if 1
    s32Ret = s_pGpioI2cFunc->pfnGpioI2cRead(g_u32I2cNum, ui2864Sel, 0xfa, &tmpReg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TW2864("GpioI2cRead failed\n");
        return HI_FAILURE;
    }

    HI_INFO_TW2864("devAddr=0x%x, reg0xfa=0x%x\r\n", ui2864Sel, tmpReg);
#endif

    ui2864Sel = tw2864_chipID(3);  //0x56
    tw2864_video_mode_init(ui2864Sel, 2, 0);
    tw2864_setd1(ui2864Sel);
    s_pGpioI2cFunc->pfnGpioI2cWrite(g_u32I2cNum, ui2864Sel, 0x9f, 0xff);
#if 1
    s32Ret = s_pGpioI2cFunc->pfnGpioI2cRead(g_u32I2cNum, ui2864Sel, 0xfa, &tmpReg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TW2864("GpioI2cRead failed\n");
        return HI_FAILURE;
    }

    HI_INFO_TW2864("devAddr=0x%x, reg0xfa=0x%x\r\n", ui2864Sel, tmpReg);
#endif

#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_tw2864.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return 0;
}

#if 0
static int tw2815_device_audio_init(unsigned char chip_addr)
{
    unsigned char temp;

    tw2815_write_table(chip_addr, 0x5a, tbl_tw2815_audio, 28);
    tw2815_byte_write(chip_addr, 0x4d, 0x88);

    temp = tw2815_byte_read(chip_addr, 0x62); //for multi-chip
    CLEAR_BIT(temp, 0xc0);
    SET_BIT(temp, 0x80);
    tw2815_byte_write(chip_addr, 0x62, temp);
    return 0;
}

static void tw2815_vin_cropping(unsigned chip_addr, unsigned int path, unsigned int hdelay, unsigned int hactive,
                                unsigned int vdelay,
                                unsigned int vactive)
{
    tw2815_byte_write(chip_addr, (0x02 + path * 0x10), (hdelay & 0xff));
    tw2815_byte_write(chip_addr, (0x06 + path * 0x10), ((hdelay & 0x300) >> 8));
    tw2815_byte_write(chip_addr, (0x03 + path * 0x10), (hdelay & 0xff));
    tw2815_byte_write(chip_addr, (0x06 + path * 0x10), ((hdelay & 0xc00) >> 8));
    tw2815_byte_write(chip_addr, (0x04 + path * 0x10), (hdelay & 0xff));
    tw2815_byte_write(chip_addr, (0x06 + path * 0x10), ((hdelay & 0x3000) >> 8));
    tw2815_byte_write(chip_addr, (0x05 + path * 0x10), (hdelay & 0xff));
    tw2815_byte_write(chip_addr, (0x06 + path * 0x10), ((hdelay & 0xc000) >> 8));
}

#endif

void tw2864_setd1(unsigned char chip_addr)
{
    tw2815_write_table(chip_addr, 0x80, tbl_tw2864_common_0x80, 16);
    tw2815_write_table(chip_addr, 0x90, tbl_tw2864_common_0x90, 16);
    tw2815_write_table(chip_addr, 0xa4, tbl_tw2864_common_0xa4, 12);
    tw2815_write_table(chip_addr, 0xb0, tbl_tw2864_common_0xb0, 1);
    tw2815_write_table(chip_addr, 0xc4, tbl_tw2864_common_0xc4, 12);
    tw2815_write_table(chip_addr, 0xd0, tbl_tw2864_common_0xd0, 16);
    tw2815_write_table(chip_addr, 0xe0, tbl_tw2864_common_0xe0, 16);
    tw2815_write_table(chip_addr, 0xf0, tbl_tw2864_common_0xf0, 16);

    if ((chip_addr == 0x50) || (chip_addr == 0x54))
    {
        gpio_i2c0_write(chip_addr, 0xfa, 0x45); //0x45
        gpio_i2c0_write(chip_addr, 0xca, 0x0);
    }
    else
    {
        gpio_i2c1_write(chip_addr, 0xfa, 0x45); //0x45
        gpio_i2c1_write(chip_addr, 0xca, 0x0);
    }

    /*
    unsigned char t1,temp;
    for(t1 = 0;t1 < 4;t1++)
    {
        temp = tw2815_byte_read(chip_addr,0x0d+t1*0x10);
        CLEAR_BIT(temp,0x04);
        tw2815_byte_write(chip_addr,(0x0d+t1*0x10),temp);
    }
    temp = tw2815_byte_read(chip_addr,0x43);
    CLEAR_BIT(temp,0x3);
    tw2815_byte_write(chip_addr,0x43,temp);

    temp = tw2815_byte_read(chip_addr,0x75);
    CLEAR_BIT(temp,0xff);
    //HI_INFO_TW2864("temp = %d\n",temp);
    tw2815_byte_write(chip_addr,0x75,temp);
     */
    return;
}

void tw2864_set4d1(unsigned char chip_addr)
{
    tw2815_write_table(chip_addr, 0x80, tbl_tw2864_common_0x80, 16);
    tw2815_write_table(chip_addr, 0x90, tbl_tw2864_common_0x90, 16);
    tw2815_write_table(chip_addr, 0xa4, tbl_tw2864_common_0xa4, 12);
    tw2815_write_table(chip_addr, 0xb0, tbl_tw2864_common_0xb0, 1);
    tw2815_write_table(chip_addr, 0xc4, tbl_tw2864_common_0xc4, 12);
    tw2815_write_table(chip_addr, 0xd0, tbl_tw2864_common_0xd0, 16);
    tw2815_write_table(chip_addr, 0xe0, tbl_tw2864_common_0xe0, 16);
    tw2815_write_table(chip_addr, 0xf0, tbl_tw2864_common_0xf0, 16);

    if ((chip_addr == 0x50) || (chip_addr == 0x54))
    {
        gpio_i2c0_write(chip_addr, 0xfa, 0x4a);
        gpio_i2c0_write(chip_addr, 0xca, 0xaa);
        gpio_i2c0_write(chip_addr, 0xca, 0xaa);
        gpio_i2c0_write(chip_addr, 0xfa, 0x4a);
        gpio_i2c0_write(chip_addr, 0x9e, 0x52);
        gpio_i2c0_write(chip_addr, 0x89, 0x02);
        gpio_i2c0_write(chip_addr, 0x9f, 0x0);
    }
    else
    {
        gpio_i2c1_write(chip_addr, 0xfa, 0x4a);
        gpio_i2c1_write(chip_addr, 0xca, 0xaa);
        gpio_i2c1_write(chip_addr, 0xca, 0xaa);
        gpio_i2c1_write(chip_addr, 0xfa, 0x4a);
        gpio_i2c1_write(chip_addr, 0x9e, 0x52);
        gpio_i2c1_write(chip_addr, 0x89, 0x02);
        gpio_i2c1_write(chip_addr, 0x9f, 0x0);
    }
}

unsigned int tw2864_chipID(unsigned int uiPortId)
{
    unsigned int uiSel_tmp;

    switch (uiPortId)
    {
    case 0:
    {
        uiSel_tmp = 0x50;
        break;
    }
    case 1:
    {
        uiSel_tmp = 0x54;
        break;
    }
    case 2:
    {
        uiSel_tmp = 0x52;
        break;
    }
    case 3:
    {
        uiSel_tmp = 0x56;
        break;
    }
    default:
    {
        uiSel_tmp = 0x50;
        break;
    }
    }

    return uiSel_tmp;
}

static void set_4half_d1(unsigned char chip_addr, unsigned char ch)
{
    unsigned char temp;

    temp = tw2815_byte_read(chip_addr, 0x0d + ch * 0x10);
    CLEAR_BIT(temp, 0x7);
    tw2815_byte_write(chip_addr, 0x0d + ch * 0x10, temp);

    temp = tw2815_byte_read(chip_addr, 0x75);
    CLEAR_BIT(temp, 0xf);
    SET_BIT(temp, 0x1 << ch);
    tw2815_byte_write(chip_addr, 0x75, temp);

    temp = tw2815_byte_read(chip_addr, 0x71);
    CLEAR_BIT(temp, 0x40);
    tw2815_byte_write(chip_addr, 0x71, temp);

    temp = tw2815_byte_read(chip_addr, 0x43);
    SET_BIT(temp, 0x03);
    tw2815_byte_write(chip_addr, 0x43, temp);
    return;
}

static void set_audio_output(unsigned char chip_addr, char num_path)
{
    unsigned char temp;
    unsigned char help;

    temp = tw2815_byte_read(chip_addr, 0x63);
    CLEAR_BIT(temp, 0x3);
    if ((0 < num_path) && (num_path <= 2))
    {
        help = 0;
    }

    if ((2 < num_path) && (num_path <= 4))
    {
        help = 1;
    }

    if ((4 < num_path) && (num_path <= 8))
    {
        help = 2;
    }

    if ((8 < num_path) && (num_path <= 16))
    {
        help = 3;
    }
    else
    {
        HI_ERR_TW2864("tw2815a audio path choice error\n");
        return;
    }

    SET_BIT(temp, help);
    tw2815_byte_write(chip_addr, 0x63, temp);
    return;
}

static void set_audio_mix_out(unsigned char chip_addr, unsigned char ch)
{
    unsigned char temp;

    temp = tw2815_byte_read(chip_addr, 0x63);
    CLEAR_BIT(temp, 0x04);
    tw2815_byte_write(chip_addr, 0x63, temp);
    temp = tw2815_byte_read(chip_addr, 0x71);
    CLEAR_BIT(temp, 0x1f);
    SET_BIT(temp, ch);
    tw2815_byte_write(chip_addr, 0x71, temp);
    if (ch == 16)
    {
        HI_INFO_TW2864("tw2815 select playback audio out\n");
    }

    if (ch == 17)
    {
        HI_INFO_TW2864("tw2815 select mix digital and analog audio data\n");
    }

    return;
}

static void set_audio_record_m(unsigned char chip_addr, unsigned char num_path)
{
    unsigned char temp, help;

    temp = tw2815_byte_read(chip_addr, 0x63);
    SET_BIT(temp, 0x04);

    CLEAR_BIT(temp, 0x3);
    if ((0 < num_path) && (num_path <= 2))
    {
        help = 0;
    }

    if ((2 < num_path) && (num_path <= 4))
    {
        help = 1;
    }

    if ((4 < num_path) && (num_path <= 8))
    {
        help = 2;
    }

    if ((8 < num_path) && (num_path <= 16))
    {
        help = 3;
    }
    else
    {
        HI_ERR_TW2864("tw2815 audio path choice error\n");
        return;
    }

    SET_BIT(temp, help);
    tw2815_byte_write(chip_addr, 0x63, temp);
    return;
}

static void set_audio_mix_mute(unsigned char chip_addr, unsigned char ch)
{
    unsigned char temp;

    temp = tw2815_byte_read(chip_addr, 0x6d);
    SET_BIT(temp, 1 << ch);
    tw2815_byte_write(chip_addr, 0x6d, temp);
    return;
}

static void clear_audio_mix_mute(unsigned char chip_addr, unsigned char ch)
{
    unsigned char temp;

    temp = tw2815_byte_read(chip_addr, 0x6d);
    CLEAR_BIT(temp, 1 << ch);
    tw2815_byte_write(chip_addr, 0x6d, temp);
    return;
}

#ifndef VXWORKS

/*
 * tw2815a read routine.
 * do nothing.
 */
ssize_t tw2815a_read(struct file * file, char __user * buf, size_t count, loff_t * offset)
{
    return 0;
}

/*
 * tw2815a write routine.
 * do nothing.
 */
ssize_t tw2815a_write(struct file * file, const char __user * buf, size_t count, loff_t * offset)
{
    return 0;
}

/*
 * tw2834 open routine.
 * do nothing.
 */
int tw2815a_open(struct inode * inode, struct file * file)
{
    return 0;
}

/*
 * tw2815a close routine.
 * do nothing.
 */
int tw2815a_close(struct inode * inode, struct file * file)
{
    return 0;
}

long tw2815a_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    unsigned int tmp = 0, temp_help, samplerate, ada_samplerate, bitwidth, ada_bitwidth, bitrate, ada_bitrate;
    static struct tw2815_w_reg tw2815reg;
    static struct tw2815_set_2d1 tw2815_2d1;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite || !s_pGpioI2cFunc->pfnGpioI2cRead)
    {
        HI_ERR_TW2864("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
    case TW2815_READ_REG:
        if (copy_from_user(&temp_help, argp, sizeof(temp_help)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy.\n");
            return -EFAULT;
        }

        tmp = tw2815_byte_read(TW2815A_I2C_ADDR, temp_help);
        if (copy_to_user(argp, &tmp, sizeof(tmp)))
        {
            return -EFAULT;
        }

        break;
    case TW2815_WRITE_REG:
        if (copy_from_user(&tw2815reg, argp, sizeof(tw2815reg)))
        {
            HI_ERR_TW2864("ttw2815a_ERROR");
            return -EFAULT;
        }

        tw2815_byte_write(TW2815A_I2C_ADDR, tw2815reg.addr, tw2815reg.value);
        break;

    case TW2815_SET_ADA_PLAYBACK_SAMPLERATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        samplerate = tmp;
        switch (samplerate)
        {
        case SET_8K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_samplerate);
            break;

        case SET_16K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_samplerate);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_SAMPLERATE:
        ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_samplerate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_PLAYBACK_BITWIDTH:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitwidth = tmp;
        switch (bitwidth)
        {
        case SET_8_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_bitwidth);
            break;

        case SET_16_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_bitwidth);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_BITWIDTH:
        ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitwidth)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_PLAYBACK_BITRATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitrate = tmp;
        switch (bitrate)
        {
        case SET_256_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_bitrate);
            break;

        case SET_384_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_playback_control, ada_bitrate);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_BITRATE:
        ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitrate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_SAMPLERATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        samplerate = tmp;
        switch (samplerate)
        {
        case SET_8K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_samplerate);
            break;

        case SET_16K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            SET_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_samplerate);
            break;
        }

        break;

    case TW2815_GET_ADA_SAMPLERATE:
        ada_samplerate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_samplerate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_BITWIDTH:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitwidth = tmp;
        switch (bitwidth)
        {
        case SET_8_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_bitwidth);
            break;

        case SET_16_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            SET_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_bitwidth);
            break;
        }

        break;

    case TW2815_GET_ADA_BITWIDTH:
        ada_bitwidth = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitwidth)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_BITRATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitrate = tmp;
        switch (bitrate)
        {
        case SET_256_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_bitrate);
            break;

        case SET_384_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
            SET_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815A_I2C_ADDR, serial_control, ada_bitrate);
            break;
        }

        break;
    case TW2815_GET_ADA_BITRATE:
        ada_bitrate = tw2815_byte_read(TW2815A_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitrate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_D1:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        setd1(TW2815A_I2C_ADDR);
        break;

    case TW2815_SET_2_D1:

        if (copy_from_user(&tw2815_2d1, argp, sizeof(tw2815_2d1)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_2_d1(TW2815A_I2C_ADDR, tw2815_2d1.ch1, tw2815_2d1.ch2);
        break;

    case TW2815_SET_4HALF_D1:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_4half_d1(TW2815A_I2C_ADDR, tmp);
        break;

    case TW2815_SET_4_CIF:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_4cif(TW2815A_I2C_ADDR, tmp);
        break;

    case  TW2815_SET_AUDIO_OUTPUT:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_output(TW2815A_I2C_ADDR, tmp);
        break;

    case  TW2815_SET_AUDIO_MIX_OUT:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_mix_out(TW2815A_I2C_ADDR, tmp);
        break;

    case TW2815_SET_AUDIO_RECORD_M:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_record_m(TW2815A_I2C_ADDR, tmp);
        break;

    case TW2815_SET_MIX_MUTE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_mix_mute(TW2815A_I2C_ADDR, tmp);
        break;

    case TW2815_CLEAR_MIX_MUTE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        clear_audio_mix_mute(TW2815A_I2C_ADDR, tmp);
        break;

    case TW2815_SET_VIDEO_MODE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        if ((tmp != NTSC) || (tmp != PAL))
        {
            HI_ERR_TW2864("set video mode %d error\n ", tmp);
            break;
        }

        tw2815_device_video_init(TW2815A_I2C_ADDR, tmp);
    default:
        break;
    }

    return 0;
}

/*
 * tw2815b read routine.
 * do nothing.
 */
ssize_t tw2815b_read(struct file * file, char __user * buf, size_t count, loff_t * offset)
{
    return 0;
}

/*
 * tw2815b write routine.
 * do nothing.
 */
ssize_t tw2815b_write(struct file * file, const char __user * buf, size_t count, loff_t * offset)
{
    return 0;
}

/*
 * tw2834 open routine.
 * do nothing.
 */
int tw2815b_open(struct inode * inode, struct file * file)
{
    return 0;
}

/*
 * tw2815b close routine.
 * do nothing.
 */
int tw2815b_close(struct inode * inode, struct file * file)
{
    return 0;
}

long tw2815b_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    unsigned int tmp, samplerate, ada_samplerate, bitwidth, ada_bitwidth, bitrate, ada_bitrate;
    static struct tw2815_w_reg tw2815reg;
    static struct tw2815_set_2d1 tw2815_2d1;

    if (!s_pGpioI2cFunc || !s_pGpioI2cFunc->pfnGpioI2cWrite || !s_pGpioI2cFunc->pfnGpioI2cRead)
    {
        HI_ERR_TW2864("GPIO_I2C not found\n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
    case TW2815_READ_REG:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        tmp = tw2815_byte_read(TW2815B_I2C_ADDR, tmp);
        if (copy_to_user(argp, &tmp, sizeof(tmp)))
        {
            return -EFAULT;
        }

        break;
    case TW2815_WRITE_REG:
        if (copy_from_user(&tw2815reg, argp, sizeof(tw2815reg)))
        {
            HI_ERR_TW2864("ttw2815b_ERROR");
            return -EFAULT;
        }

        tw2815_byte_write(TW2815B_I2C_ADDR, tw2815reg.addr, tw2815reg.value);
        break;

    case TW2815_SET_ADA_PLAYBACK_SAMPLERATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        samplerate = tmp;
        switch (samplerate)
        {
        case SET_8K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_samplerate);
            break;

        case SET_16K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_samplerate);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_SAMPLERATE:
        ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_samplerate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_PLAYBACK_BITWIDTH:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitwidth = tmp;
        switch (bitwidth)
        {
        case SET_8_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_bitwidth);
            break;

        case SET_16_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_bitwidth);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_BITWIDTH:
        ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitwidth)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_PLAYBACK_BITRATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitrate = tmp;
        switch (bitrate)
        {
        case SET_256_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            CLEAR_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_bitrate);
            break;

        case SET_384_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
            SET_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_playback_control, ada_bitrate);
            break;
        }

        break;

    case TW2815_GET_ADA_PLAYBACK_BITRATE:
        ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_playback_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitrate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_SAMPLERATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        samplerate = tmp;
        switch (samplerate)
        {
        case SET_8K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_samplerate);
            break;

        case SET_16K_SAMPLERATE:
            ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            SET_BIT(ada_samplerate, 0x04);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_samplerate);
            break;
        }

        break;

    case TW2815_GET_ADA_SAMPLERATE:
        ada_samplerate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_samplerate)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_BITWIDTH:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitwidth = tmp;
        switch (bitwidth)
        {
        case SET_8_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_bitwidth);
            break;

        case SET_16_BITWIDTH:
            ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            SET_BIT(ada_bitwidth, 0x02);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_bitwidth);
            break;
        }

        break;

    case TW2815_GET_ADA_BITWIDTH:
        ada_bitwidth = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitwidth)))
        {
            return -EFAULT;
        }

        break;

    case TW2815_SET_ADA_BITRATE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815a_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        bitrate = tmp;
        switch (bitrate)
        {
        case SET_256_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            CLEAR_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_bitrate);
            break;

        case SET_384_BITRATE:
            ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
            SET_BIT(ada_bitrate, 0x10);
            tw2815_byte_write(TW2815B_I2C_ADDR, serial_control, ada_bitrate);
            break;
        }

        break;
    case TW2815_GET_ADA_BITRATE:
        ada_bitrate = tw2815_byte_read(TW2815B_I2C_ADDR, serial_control);
        if (copy_to_user(argp, &ada_samplerate, sizeof(ada_bitrate)))
        {
            return -EFAULT;
        }

        break;
    case TW2815_SET_D1:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        setd1(TW2815B_I2C_ADDR);
        break;

    case TW2815_SET_2_D1:
        if (copy_from_user(&tw2815_2d1, argp, sizeof(tw2815_2d1)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy.\n");
            return -EFAULT;
        }

        set_2_d1(TW2815B_I2C_ADDR, tw2815_2d1.ch1, tw2815_2d1.ch2);
        break;

    case TW2815_SET_4HALF_D1:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_4half_d1(TW2815B_I2C_ADDR, tmp);
        break;

    case TW2815_SET_4_CIF:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_4cif(TW2815B_I2C_ADDR, tmp);
        break;

    case  TW2815_SET_AUDIO_OUTPUT:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_output(TW2815B_I2C_ADDR, tmp);
        break;

    case  TW2815_SET_AUDIO_MIX_OUT:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_mix_out(TW2815B_I2C_ADDR, tmp);
        break;

    case TW2815_SET_AUDIO_RECORD_M:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_record_m(TW2815B_I2C_ADDR, tmp);
        break;

    case TW2815_SET_MIX_MUTE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        set_audio_mix_mute(TW2815B_I2C_ADDR, tmp);
        break;

    case TW2815_CLEAR_MIX_MUTE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        clear_audio_mix_mute(TW2815B_I2C_ADDR, tmp);
        break;

    case TW2815_SET_VIDEO_MODE:
        if (copy_from_user(&tmp, argp, sizeof(tmp)))
        {
            HI_ERR_TW2864("\ttw2815b_ERROR: WRONG cpy tmp is %x\n", tmp);
            return -EFAULT;
        }

        if ((tmp != NTSC) || (tmp != PAL))
        {
            HI_ERR_TW2864("set video mode %d error\n ", tmp);
            break;
        }

        tw2815_device_video_init(TW2815B_I2C_ADDR, tmp);
    default:
        break;
    }

    return 0;
}

 #if 0

/*
 *      The various file operations we support.
 */
static struct file_operations tw2815a_fops =
{
    .owner			= THIS_MODULE,
    .read			= tw2815a_read,
    .write			= tw2815a_write,
    .unlocked_ioctl = tw2815a_ioctl,
    .open			= tw2815a_open,
    .release		= tw2815a_close
};

static struct miscdevice tw2815a_dev =
{
    MISC_DYNAMIC_MINOR,
    "tw2815adev",
    &tw2815a_fops,
};

/*
 *      The various file operations we support.
 */
static struct file_operations tw2815b_fops =
{
    .owner			= THIS_MODULE,
    .read			= tw2815b_read,
    .write			= tw2815b_write,
    .unlocked_ioctl = tw2815b_ioctl,
    .open			= tw2815b_open,
    .release		= tw2815b_close
};

static struct miscdevice tw2815b_dev =
{
    MISC_DYNAMIC_MINOR,
    "tw2815bdev",
    &tw2815b_fops,
};

static int __init tw2815_init(void)
{
    int ret = 0;

    /*register tw2815b_dev*/
    ret = misc_register(&tw2815a_dev);
    HI_INFO_TW2864("TW2815 driver init start ... \n");
    if (ret)
    {
        HI_ERR_TW2864("\ttw2815a_ERROR: could not register tw2815a devices. \n");
        return ret;
    }

    if ((tw2815_device_video_init(TW2815A_I2C_ADDR,
                                  AUTOMATICALLY) < 0) || (tw2815_device_audio_init(TW2815A_I2C_ADDR) < 0))
    {
        misc_deregister(&tw2815a_dev);
        HI_ERR_TW2864("\ttw2815a_ERROR: tw2815a driver init fail for device init error!\n");
        return -1;
    }

    HI_INFO_TW2864("tw2815a driver init successful!\n");

  #if 0
    /*register tw2815b_dev*/
    ret = misc_register(&tw2815b_dev);
    if (ret)
    {
        HI_ERR_TW2864("\ttw2815b_ERROR: could not register tw2815b devices. \n");
        return ret;
    }

    if ((tw2815_device_video_init(TW2815B_I2C_ADDR,
                                  AUTOMATICALLY) < 0) || (tw2815_device_audio_init(TW2815B_I2C_ADDR) < 0))
    {
        misc_deregister(&tw2815b_dev);
        HI_ERR_TW2864("\ttw2815b_ERROR: tw2815b driver init fail for device init error!\n");
        return -1;
    }

    HI_INFO_TW2864("tw2815b driver init successful!\n");
  #endif

    return ret;
}

static void __exit tw2815_exit(void)
{
    misc_deregister(&tw2815a_dev);

    // misc_deregister(&tw2815b_dev);
}

 #endif

module_init(TW2864_DRV_ModInit);
module_exit(TW2864_DRV_ModExit);

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
#endif
