#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>        /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/cdev.h>
#include <asm/uaccess.h> /* copy_*_user */

#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/workqueue.h>
#include <asm/io.h>


#include "hi_jpge_type.h"
#include "hi_jpge_ioctl.h"
#include "jpge_ext.h"
#include "jpge_hal.h"

#include "drv_dev_ext.h"

#include "hi_jpge_config.h"
#include "drv_mem_ext.h"
#include "drv_module_ext.h"

static int jpge_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);
static int jpge_pm_resume(PM_BASEDEV_S *pdev);

static atomic_t g_JPGECount = ATOMIC_INIT(0);

static int jpge_open(struct inode *finode, struct file  *ffile)
{
    if (1 == atomic_inc_return(&g_JPGECount))
    {
        //do nothing..??
    }

    return 0;
}

int JpgeOsiOpen(HI_VOID)
{
    return jpge_open(NULL, NULL);
}

static int jpge_release(struct inode *finode, struct file  *ffile)
{
    if (atomic_dec_and_test(&g_JPGECount))
    {
        //do nothing..??
    }

    if ( atomic_read(&g_JPGECount) < 0 )
    {
        atomic_set(&g_JPGECount, 0);
    }
    return 0;
}

int JpgeOsiClose(HI_VOID)
{
    return jpge_release(NULL, NULL);
}


static long jpge_ioctl(struct file  *ffile, unsigned int  cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;
    long ret;

    switch (cmd)
    {
    case JPGE_CREATE_CMD:
    {
        Jpge_EncCfgInfo_S EncCfgInfo;
        HI_U32 ret;
        if ((ret = copy_from_user(&EncCfgInfo, argp, sizeof(Jpge_EncCfgInfo_S))) < 0)
        {
            return -EFAULT;
        }
        ret =  Jpge_Create ( EncCfgInfo.pEncHandle, &EncCfgInfo.EncCfg);
        if(0 != ret)
        {
	        return ret;
	    }
        if((ret = copy_to_user(argp, &EncCfgInfo, sizeof(Jpge_EncCfgInfo_S))) < 0)
        {
            return -EFAULT;
        }
        return 0;
    }
    case JPGE_ENCODE_CMD:
    {
         Jpge_EncInfo_S EncInfo;   
         if ((ret = copy_from_user(&EncInfo, argp, sizeof(Jpge_EncInfo_S))) < 0)
        {
            return -EFAULT;
        }
        ret =  Jpge_Encode ( EncInfo.EncHandle, &EncInfo.EncIn, &EncInfo.EncOut);
	     if(0 != ret)
         {
	        return ret;
	    }
        if((ret = copy_to_user(argp, &EncInfo, sizeof(Jpge_EncInfo_S))) < 0)
        {
            return -EFAULT;
        }
        return 0;
    }
    case JPGE_DESTROY_CMD:
    {
         HI_U32  u32Handle;
        if ((ret = copy_from_user(&u32Handle, argp, sizeof(HI_U32))) < 0)
        {
            return -EFAULT;
        }
        return Jpge_Destroy(u32Handle);
    }
    default:
        return -ENOIOCTLCMD;
    }
}

struct file_operations jpge_fops =
{
    .owner   = THIS_MODULE,
	.unlocked_ioctl = jpge_ioctl,
    .open    = jpge_open,
    .release = jpge_release,
};

static PM_BASEOPS_S jpge_drvops = {
    .probe          = NULL,
    .remove       = NULL,
    .shutdown    = NULL,
    .prepare       = NULL,
    .complete     = NULL,
    .suspend      = jpge_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = jpge_pm_resume,
};

static UMAP_DEVICE_S jpge_dev = {
    .devfs_name  = "hi_jpge",
    .minor = UMAP_MIN_MINOR_JPGE,
    .owner = THIS_MODULE,
    .fops = &jpge_fops,
    .drvops = &jpge_drvops
};

typedef unsigned long       HI_UL;


void JPGE_DRV_ModExit(void);

#define JPGENAME "HI_JPGE"

HI_VOID jpge_version(HI_VOID)
{
    HI_CHAR JPGEVersion[80] ="SDK_VERSION:["\
        MKMARCOTOSTR(SDK_VERSION)"] Build Time:["\
        __DATE__", "__TIME__"]";

    printk("Load hi_jpge.ko success.     \t(%s)\n", JPGEVersion);
}


int JPGE_DRV_ModInit(void)
{
    int ret; 
    ret = Jpge_Open();
    if(0 != ret)
    {
        HIJPGE_TRACE("request_irq for JPGE failure!\n");
        return -1;
    }
    ret = HI_DRV_DEV_Register(&jpge_dev);
    if (ret)
    {
        Jpge_Close();
        return -1;
    }
	ret = HI_DRV_MODULE_Register(HI_ID_JPGENC, JPGENAME, NULL); 
    if (HI_SUCCESS != ret)
    {
	   HIJPGE_TRACE("HI_DRV_MODULE_Register JPGE failed\n");
       JPGE_DRV_ModExit();
	   return -1;
    }

    jpge_version();
    
    return 0;
}

void JPGE_DRV_ModExit(void)
{
	HI_DRV_MODULE_UnRegister(HI_ID_JPGENC);
    Jpge_Close();
    /* cleanup_module is never called if registering failed */
    HI_DRV_DEV_UnRegister(&jpge_dev);
}

static int jpge_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HIJPGE_TRACE("jpge suspend!\n");

    return 0;
}

/* wait for resume */
static int jpge_pm_resume(PM_BASEDEV_S *pdev)
{
    Jpge_SetClock();

    HIJPGE_TRACE("jpge resume!\n");
   
    return 0;
}

#ifdef MODULE
module_init(JPGE_DRV_ModInit);
module_exit(JPGE_DRV_ModExit);
#else
//late_initcall_sync(JPGE_DRV_ModInit);
#endif

MODULE_DESCRIPTION("driver for the all jpge");
MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0.0.0");


EXPORT_SYMBOL(JpgeOsiOpen);
EXPORT_SYMBOL(JpgeOsiClose);
