#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>    /* printk() */
#include <linux/slab.h>      /* kmalloc() */
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

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif

#include "hi_type.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "tde_proc.h"

#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

extern int tde_init_module_k(void);
extern void tde_cleanup_module_k(void);
extern int tde_open(struct inode *finode, struct file  *ffile);
extern int tde_release(struct inode *finode, struct file  *ffile);

extern long tde_ioctl(struct file  *ffile, unsigned int  cmd, unsigned long arg); 

extern int tde_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);
extern int tde_pm_resume(PM_BASEDEV_S *pdev);


struct file_operations tde_fops =
{
    .owner   = THIS_MODULE,
    .unlocked_ioctl = tde_ioctl,
    .open    = tde_open,
    .release = tde_release,
};

static PM_BASEOPS_S tde_drvops = {
    .probe          = NULL,
    .remove       = NULL,
    .shutdown    = NULL,
    .prepare       = NULL,
    .complete     = NULL,
    .suspend      = tde_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = tde_pm_resume,
};

static UMAP_DEVICE_S tde_dev = {
    .devfs_name  = "hi_tde",
    .minor = UMAP_MIN_MINOR_TDE,
    .owner = THIS_MODULE,
    .fops = &tde_fops,
    .drvops = &tde_drvops
};

#ifndef CONFIG_SUPPORT_CA_RELEASE
static void tde_version(void)
{
    HI_CHAR TDEVersion[160] ="SDK_VERSION:["MKMARCOTOSTR(SDK_VERSION)"] Build Time:["\
        __DATE__", "__TIME__"]";
    printk("Load tde.ko success.\t\t(%s)\n", TDEVersion);
}
#endif
HI_S32  TDE_DRV_ModInit(HI_VOID)
{

#ifndef HI_MCE_SUPPORT
    int ret = 0;
    ret = tde_init_module_k();
    if (0 != ret)
    {
        return -1;
    }    
#endif
    /* register tde device */
    if(HI_DRV_DEV_Register(&tde_dev) < 0)
    {
        printk("register tde failed.\n");
#ifndef HI_MCE_SUPPORT
        tde_cleanup_module_k();
#endif
        return -1;
    }

#ifndef CONFIG_SUPPORT_CA_RELEASE
{
    DRV_PROC_ITEM_S *item;
    item = HI_DRV_PROC_AddModule("tde", NULL, NULL);
    if (!item)
    {
        //TDE_TRACE(TDE_KERN_ERR, "add proc module failed\n");
        tde_cleanup_module_k();
        return -1;
    }

    item->read  = tde_read_proc;
    item->write = tde_write_proc;

   
    tde_version();
}    
#endif
    return 0;
}

HI_VOID  TDE_DRV_ModExit(HI_VOID)
{
#ifndef CONFIG_SUPPORT_CA_RELEASE
    HI_DRV_PROC_RemoveModule("tde");
#endif

#ifndef HI_MCE_SUPPORT
    tde_cleanup_module_k();
#endif

    /* cleanup_module is never called if registering failed */
    HI_DRV_DEV_UnRegister(&tde_dev);
}

#ifdef MODULE
module_init(TDE_DRV_ModInit);
module_exit(TDE_DRV_ModExit);
#endif


MODULE_AUTHOR("Digital Media Team, Hisilicon crop.");
MODULE_DESCRIPTION("Hisilicon TDE Device driver for X5HD");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0.0.0");


