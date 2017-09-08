/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_tuner.c
* Description:
*
* History:
* Version   Date             Author       DefectNum    Description
* main\1    2007-08-03   w54542      NULL            Create this file.
* main\1    2007-11-10   w54542      NULL            modify this file.
***********************************************************************************/
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
//#include <linux/himedia.h>

#include "hi_type.h"
//#include "hi_i2c.h"
#include "drv_i2c_ext.h"
#include "drv_gpioi2c_ext.h"
#include "hi_debug.h"

#include "hi_drv_tuner.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "drv_tuner_ioctl.h"

#include "hi_unf_i2c.h"

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
//#include "common_dev.h"
//#include "common_proc.h"
//#include "common_mem.h"
//#include "common_stat.h"
//#include "hi_common_id.h"
//#include "common_module_drv.h"

#define TUNER_NAME "hi_tuner"


extern HI_S32 hi_tuner_open(struct inode *inode, struct file *filp);
extern HI_S32 hi_tuner_release(struct inode *inode, struct file *filp);
extern long hi_tuner_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
extern HI_S32 tuner_proc_read(struct seq_file *p, HI_VOID *v);
extern HI_S32 tuner_proc_read_reg(struct seq_file *p, HI_VOID *v);
extern HI_S32 tuner_resume(PM_BASEDEV_S *pdev);
extern HI_S32 tuner_suspend (PM_BASEDEV_S *pdev, pm_message_t state);
extern HI_S32 qam_ram_collect_data(struct file * file, const char __user * buf, size_t count, loff_t *ppos);

static struct file_operations hi_tuner_fops =
{
	.owner = THIS_MODULE,
	.unlocked_ioctl = hi_tuner_ioctl,
	.open  = hi_tuner_open,
	.release = hi_tuner_release,
};

static PM_BASEOPS_S tuner_baseOps =
{
    .probe  = NULL,
    .remove = NULL,
    .shutdown = NULL,
    .prepare  = NULL,
    .complete = NULL,
    .suspend   = tuner_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume  = tuner_resume
};

static UMAP_DEVICE_S  hi_tuner_dev =
{
    .devfs_name = UMAP_DEVNAME_TUNER,
    .minor  = UMAP_MIN_MINOR_TUNER,
    .owner  = THIS_MODULE,
    .fops   = &hi_tuner_fops,
    .drvops = &tuner_baseOps
};

static TUNER_EXPORT_FUNC_S s_TunerExportFuncs =
{
    .pfnTunerOpen = hi_tuner_open,
    .pfnTunerIoctl = hi_tuner_ioctl,
    .pfnTunerRelease = hi_tuner_release,
    .pfnTunerSuspend = tuner_suspend,
    .pfnTunerResume = tuner_resume,
    .pfnTunerProcRead = tuner_proc_read,
    .pfnTunerProcReadReg = tuner_proc_read_reg,
    .pfnTunerDrvInit = HI_DRV_TUNER_Init,
    .pfnTunerDrvExit = HI_DRV_TUNER_DeInit
};

HI_S32 TUNER_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    DRV_PROC_ITEM_S *tuner_proc = NULL;
    DRV_PROC_ITEM_S *tuner_reg_proc = NULL;
    DRV_PROC_ITEM_S *ram_collect_proc = NULL;
    
	/* pre */
#ifndef HI_MCE_SUPPORT
    ret = HI_DRV_TUNER_Init();
    if (HI_SUCCESS != ret)
    {
    	HI_ERR_TUNER("HI_DRV_TUNER_Init err! \n");
        return ret;
    }
#endif

    /* misc register tuner */
    ret = HI_DRV_DEV_Register(&hi_tuner_dev);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_TUNER("hi tuner(tuner and QAM) register failed!\n");
        ret = -EFAULT;
        goto err0;
    }

    ret = HI_DRV_MODULE_Register(HI_ID_TUNER,
                TUNER_NAME, (HI_VOID*)&s_TunerExportFuncs); 
    if (HI_SUCCESS != ret)
    {
        HI_INFO_TUNER("hi tuner(tuner and QAM) register failed!\n");
        ret = -EFAULT;
        goto err0;
    }

	/*proc*/
    tuner_proc = HI_DRV_PROC_AddModule("tuner", NULL, NULL);
    if (! tuner_proc)
    {
        HI_INFO_TUNER("add proc module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_proc->read = tuner_proc_read;

    tuner_reg_proc = HI_DRV_PROC_AddModule("tuner_reg", NULL, NULL);
    if (! tuner_reg_proc)
    {
        HI_INFO_TUNER("add proc module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_reg_proc->read = tuner_proc_read_reg;

#if defined (CHIP_TYPE_hi3716cv200es)|| defined (CHIP_TYPE_hi3716cv200)
    ram_collect_proc = HI_DRV_PROC_AddModule("qam_ram_collect", NULL, NULL);
    if (! ram_collect_proc)
    {
        HI_FATAL_TUNER("add proc module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    ram_collect_proc->write = qam_ram_collect_data;
#endif
	
#ifdef MODULE
	HI_PRINT("Load hi_tuner.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;    


err1:
    HI_DRV_MODULE_UnRegister(HI_ID_TUNER);
    HI_DRV_DEV_UnRegister(&hi_tuner_dev);
err0:
#ifndef HI_MCE_SUPPORT
    HI_DRV_TUNER_DeInit();
#endif
    return ret;
}

HI_VOID TUNER_DRV_ModExit(HI_VOID)
{
#if defined (CHIP_TYPE_hi3716cv200es)|| defined (CHIP_TYPE_hi3716cv200)
    HI_DRV_PROC_RemoveModule("qam_ram_collect");
#endif

    HI_DRV_PROC_RemoveModule("tuner");
    HI_DRV_PROC_RemoveModule("tuner_reg");
    HI_DRV_MODULE_UnRegister(HI_ID_TUNER);
    HI_DRV_DEV_UnRegister(&hi_tuner_dev);
#ifndef HI_MCE_SUPPORT
    HI_DRV_TUNER_DeInit();
#endif
    //HI_I2C_Close();
        
    return;
}

#ifdef MODULE
module_init(TUNER_DRV_ModInit);
module_exit(TUNER_DRV_ModExit);
#endif

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("tuner interface for Hi3110 Solution");



