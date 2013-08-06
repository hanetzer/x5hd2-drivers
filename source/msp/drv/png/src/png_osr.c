/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.
******************************************************************************
File Name	: hi_png_api.c
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/11
Description	: implemention of PNG ioctl
Function List 	: 
			  	  
History       	:
Date				Author        		Modification
2010/10/11		z00141204		Created file      	

******************************************************************************/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include "drv_dev_ext.h"
#include "drv_sys_ext.h"
#include "drv_struct_ext.h"
#include "hi_drv_png.h"
#include "png_hal.h"
#include "png_osires.h"
#include "png_osi.h"
#include "png_proc.h"
#include "png_define.h"

#include "hi_png_config.h"

#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

#define PNGNAME "HI_PNG"
extern HI_S32 PngOsiInit(HI_VOID);
extern HI_S32 PngOsiDeinit(HI_VOID);
extern HI_VOID PngOsiIntHandle(HI_U32 u32Int);
extern HI_VOID PngOsiDelTimer(HI_VOID);
extern HI_S32 PngOsiSuspend(HI_VOID);
extern HI_S32 pngOsiResume(HI_VOID);

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

static long png_ioctl(struct file  *ffile,unsigned int  cmd,unsigned long arg);

static int png_open(struct inode *finode, struct file  *ffile);
static int png_close(struct inode *finode, struct file  *ffile);
static int png_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);
static int png_pm_resume(PM_BASEDEV_S *pdev);

/* png device operation*/
static struct file_operations png_fops =
{
    .owner   = THIS_MODULE,
    .unlocked_ioctl = png_ioctl,
    .open    = png_open,
    .release = png_close,
};

/* png device extention operation */
static PM_BASEOPS_S  png_drvops = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = png_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = png_pm_resume,
};

/* png device */
static UMAP_DEVICE_S  png_dev = {
    .minor = UMAP_MIN_MINOR_PNG,
    .devfs_name  = "hi_png",
    .owner = THIS_MODULE,
    .fops = &png_fops,
    .drvops = &png_drvops
};

#ifndef HIPNG_GAO_AN_VERSION
/* PNG version info*/
static inline void png_version(void)
{
    #ifdef YANJIANQING_DEBUG
	HI_CHAR PNGVersion[160] ="SDK_VERSION:["MKMARCOTOSTR(SDK_VERSION)"] Build Time:["\
	__DATE__", "__TIME__"]";
	printk("Load hi_png.ko success.\t\t(%s)\n", PNGVersion);
	#endif
}	
#endif

/* interrutp function */
static int png_isr(int irq, void *dev_id)
{
    HI_U32 u32IntValue;

    /* read register and clear the int*/
    u32IntValue = PngHalGetIntStatus();

    PngOsiIntHandle(u32IntValue);

    return IRQ_HANDLED;
}

void PNG_DRV_ModExit(void);


/* module init */

int PNG_DRV_ModInit(void)
{

	HI_S32 Ret;
	HI_CHIP_TYPE_E enChipType;
    HI_CHIP_VERSION_E enChipVersion;
    HI_DRV_SYS_GetChipVersion(&enChipType,&enChipVersion);
    if(HI_CHIP_VERSION_V100 == enChipVersion)
    {
        if((HI_CHIP_TYPE_HI3716H == enChipType)
			|| (HI_CHIP_TYPE_HI3716C == enChipType)
			|| (HI_CHIP_TYPE_HI3716M == enChipType)
            ||(HI_CHIP_TYPE_HI3720 == enChipType))
        {
           return -1;
        }
    }


    /* Hal init*/
    if (0 != PngHalInit())
    {
    	return -1;
    }

    /* Osires init*/
    if (0 != PngOsiResInitHandle())
    {
    	goto ERR5;
    }

#if 0
    if (0 != PngOsiResInitMem())
    {
    	goto ERR4;
    }
    if (0 != PngOsiResInitBuf())
    {
        goto ERR3;
    }
#endif
    if (0 != PngOsiInit())
    {
        goto ERR2;
    }

    /* register interrupt function*/
    if (0 != request_irq(PngHalGetIrqNum(), (irq_handler_t)png_isr, IRQF_PROBE_SHARED, "png_isr", NULL))
    {
    	goto ERR1;
    }

    /* register device*/
    if(HI_DRV_DEV_Register(&png_dev) < 0)        
    {
    	PNG_ERROR("register png failed.\n");
    	goto ERR1;
    }

	#ifndef HIPNG_GAO_AN_VERSION
    /* proc init*/
    PNG_ProcInit();

    /*Version info*/
    /*CNcomment:版本信息*/
    png_version();

	#endif
	Ret = HI_DRV_MODULE_Register(HI_ID_PNG, PNGNAME, NULL); 
    if (HI_SUCCESS != Ret)
    {
	   PNG_ERROR("HI_DRV_MODULE_Register PNG failed\n");
	   PNG_DRV_ModExit();
	   goto ERR1;
    }
	
    return 0;

ERR1:
    PngOsiDeinit();
ERR2:
    PngOsiResDeinitBuf();
#if 0    
ERR3:    
    PngOsiResDeinitMem();
ERR4:
    PngOsiResDeinitHandle();
#endif    
ERR5:
    PngHalDeinit();

    return -1;	
}

/* module deinit */

void PNG_DRV_ModExit(void)
{
    #ifndef HIPNG_GAO_AN_VERSION
    /* proc deinit*/
    PNG_ProcCleanup();
    #endif
	HI_DRV_MODULE_UnRegister(HI_ID_PNG);
    /* logoout device */
    HI_DRV_DEV_UnRegister(&png_dev);

    /* release interrupt num*/
    free_irq(PngHalGetIrqNum(), NULL);

    PngOsiDeinit();
    PngOsiResDeinitBuf();
    PngOsiResDeinitMem();
    PngOsiResDeinitHandle();
    PngHalDeinit();
	
    return;
}

/* open  device*/
static int png_open(struct inode *finode, struct file  *ffile)
{
    return PngOsiOpen();
}

/* close device */
static int png_close(struct inode *finode, struct file  *ffile)
{
    PngOsiClose();
    
    return 0;
}

/* ioctl function */
static long png_ioctl(struct file  *ffile,unsigned int  cmd,unsigned long arg)
{
    void __user *argp = (void __user *)arg;

    if (NULL == argp)
    {
        PNG_ERROR("NULL param!\n");
    	return -EFAULT;
    }

    switch(cmd)
    {
    	case PNG_CREATE_DECODER:
    	{
            HI_S32 s32Ret = 0;
            HI_PNG_HANDLE s32Handle;

            s32Ret = PngOsiCreateDecoder(&s32Handle);
            if (s32Ret < 0)
            {
                return s32Ret;
            }

            if (copy_to_user(argp, &s32Handle, sizeof(HI_S32)))
            {
            	return -EFAULT;
            }

            return 0;
    	}
        case PNG_DESTROY_DECODER:
        {
            HI_PNG_HANDLE s32Handle = -1;

            if (copy_from_user(&s32Handle, argp, sizeof(HI_PNG_HANDLE)))
            {
    	        return -EFAULT;
            }

            return PngOsiDestroyDecoder(s32Handle);
        }
        case PNG_ALLOC_BUF:
        {
            HI_S32 s32Ret = HI_SUCCESS;
            PNG_GETBUF_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_GETBUF_CMD_S):%d\n", sizeof(PNG_GETBUF_CMD_S));
            if (copy_from_user(&stCmd, argp, sizeof(PNG_GETBUF_CMD_S)))
            {
                return -EFAULT;
            }

            s32Ret = PngOsiAllocBuf(stCmd.s32Handle, &stCmd.stBuf);
            if (s32Ret < 0)
            {
                return s32Ret;
            }

            if (copy_to_user(argp, &stCmd, sizeof(PNG_GETBUF_CMD_S)))
            {
                return -EFAULT;
            }

            return 0;
        }
        case PNG_RELEASE_BUF:
        {
            HI_S32 s32Ret = HI_SUCCESS;
            HI_PNG_HANDLE s32Handle = 0;

            if (copy_from_user(&s32Handle, argp, sizeof(HI_PNG_HANDLE)))
            {
                return -EFAULT;
            }

            s32Ret = PngOsiReleaseBuf(s32Handle);
            if (s32Ret < 0)
            {
                return s32Ret;
            }

            return 0;
        }
        case PNG_SET_STREAMLEN:
        {
            PNG_SETSTREAM_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_SETSTREAM_CMD_S):%d\n", sizeof(PNG_SETSTREAM_CMD_S));

            if (copy_from_user(&stCmd, argp, sizeof(PNG_SETSTREAM_CMD_S)))
            {
                return -EFAULT;
            }

            return PngOsiSetStreamLen(stCmd.s32Handle, stCmd.u32Phyaddr, stCmd.u32Len);
        }
        case PNG_GET_STREAMLEN:
        {
            HI_S32 s32Ret = HI_SUCCESS;
            PNG_SETSTREAM_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_SETSTREAM_CMD_S):%d\n", sizeof(PNG_SETSTREAM_CMD_S));

            if (copy_from_user(&stCmd, argp, sizeof(PNG_SETSTREAM_CMD_S)))
            {
                return -EFAULT;
            }

            s32Ret = PngOsiGetStreamLen(stCmd.s32Handle, stCmd.u32Phyaddr, &stCmd.u32Len);
            if (s32Ret < 0)
            {
                return s32Ret;
            }

            if (copy_to_user(argp, &stCmd, sizeof(PNG_SETSTREAM_CMD_S)))
            {
                return -EFAULT;
            }

            return 0;
        }
        case PNG_DECODE:
        {
            PNG_DECODE_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_DECODE_CMD_S):%d\n", sizeof(PNG_DECODE_CMD_S));
            if (copy_from_user(&stCmd, argp, sizeof(PNG_DECODE_CMD_S)))
            {
                return -EFAULT;
            }

            return PngOsiDecode(stCmd.s32Handle, &stCmd.stDecInfo);
        }
        case PNG_GET_DECRESULT:
        {
            HI_S32 s32Ret = HI_SUCCESS;
            PNG_DECRESULT_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_DECODE_CMD_S):%d\n", sizeof(PNG_DECODE_CMD_S));

            if (copy_from_user(&stCmd, argp, sizeof(PNG_DECODE_CMD_S)))
            {
    	        return -EFAULT;
            }

            s32Ret = PngOsiGetResult(stCmd.s32Handle, stCmd.bBlock, &stCmd.eDecResult);
            if (s32Ret < 0)
            {
    	        return s32Ret;
            }

            if (copy_to_user(argp, &stCmd, sizeof(PNG_DECRESULT_CMD_S)))
            {
    	        return -EFAULT;
            }

            return 0;
        }
        case PNG_GET_GETBUFPARAM:
        {
            HI_S32 s32Ret = 0;
            PNG_GETBUFPARAM_CMD_S stCmd = {0};

            //PNG_ERROR("kernel sizeof(PNG_GETBUFPARAM_CMD_S):%d\n", sizeof(PNG_GETBUFPARAM_CMD_S));

            if (copy_from_user(&stCmd, argp, sizeof(PNG_GETBUFPARAM_CMD_S)))
            {
                return -EFAULT;
            }

            s32Ret = PngOsiGetBufParam(stCmd.s32Handle, &stCmd.u32PhyAddr, &stCmd.u32Size);
            if (s32Ret < 0)
            {
                return s32Ret;
            }

            if (copy_to_user(argp, &stCmd, sizeof(PNG_GETBUFPARAM_CMD_S)))
            {
                return -EFAULT;
            }

            return 0;
        }
        default:
        {
    	    return -EFAULT;
        }
    }
}

/* suspend */
static int png_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return PngOsiSuspend();
}

/* resume */
static int png_pm_resume(PM_BASEDEV_S *pdev)
{
    return pngOsiResume();
}

/** 这两个函数要按此命名 **/
#ifdef MODULE
module_init(PNG_DRV_ModInit);
module_exit(PNG_DRV_ModExit);
#endif


MODULE_DESCRIPTION("driver for the all png");
MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0.0.0");
