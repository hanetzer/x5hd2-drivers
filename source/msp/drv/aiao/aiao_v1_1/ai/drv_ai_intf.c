/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: aiao_intf_k.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    
 ********************************************************************************/



#include <asm/setup.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_stat_ext.h"

#include "hi_module.h"
#include "drv_mmz_ext.h"
#include "drv_stat_ext.h"
#include "drv_sys_ext.h"
#include "drv_proc_ext.h"
#include "drv_module_ext.h"
#include "drv_mem_ext.h"
#include "hi_error_mpi.h"

#include "hi_drv_aiao.h"
#include "drv_ai_comm.h"


#define AI_PMOC

/**************************** global variables ****************************/
static UMAP_DEVICE_S g_stAIDev;
static atomic_t g_AIModInitFlag = ATOMIC_INIT(0);

extern HI_S32 AI_DRV_Open(struct inode *finode, struct file  *ffile);
extern long AI_DRV_Ioctl(struct file *file, unsigned int cmd, HI_VOID *arg);
extern HI_S32 AI_DRV_Release(struct inode *finode, struct file  *ffile);





static struct file_operations AI_DRV_Fops =
{
    .owner   = THIS_MODULE,
    .open    = AI_DRV_Open,
    .unlocked_ioctl   = AI_DRV_Ioctl,
    .release = AI_DRV_Release,
};

static PM_BASEOPS_S ai_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = NULL,               /*TODO  AI_Suspend*/
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = NULL,               /*TODO  AI_Resume*/
};


static HI_S32  AIRegisterDevice(HI_VOID)
{
    HI_S32 s32Ret;

    sprintf(g_stAIDev.devfs_name, "%s", UMAP_DEVNAME_AI);
    g_stAIDev.fops  = &AI_DRV_Fops;
    g_stAIDev.minor = UMAP_MIN_MINOR_AI;
#ifdef AI_PMOC
    g_stAIDev.owner  = THIS_MODULE;
    g_stAIDev.drvops = &ai_drvops;
#endif
    s32Ret = HI_DRV_DEV_Register(&g_stAIDev);
    if(HI_SUCCESS != s32Ret)
    {
        HI_FATAL_DRV_AI("Unable to register ai dev\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AIUnregisterDevice(HI_VOID)
{
    HI_S32 s32Ret;
    
    //TO DO
    s32Ret = HI_DRV_DEV_UnRegister(&g_stAIDev);
    if(HI_SUCCESS != s32Ret)
    {
        HI_FATAL_DRV_AI("Unable to unregister ai dev\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 AI_ModInit(HI_VOID *)
{
    HI_S32 s32Ret;
    
#ifdef HI_AI_PROC_SUPPORT
    DRV_PROC_ITEM_S *item;
#endif

    s32Ret = AIRegisterDevice();
    if(HI_SUCCESS != s32Ret)
    {
        HI_FATAL_DRV_AI("Unable to register ai dev\n");
        return HI_FAILURE;
    }

#ifdef HI_AI_PROC_SUPPORT
    item = HI_DRV_PROC_AddModule("ai", AI_DRV_ProcRead, NULL);
    if (!item)
    {
        HI_ERR_DRV_AI("add proc ai failed\n");
    }
#endif

    //TO DO
    //init global resource
    AI_InitRM();

    return HI_SUCCESS;
}


HI_VOID AI_ModExit(HI_VOID *)
{
    //TO DO
    //unregister device
    AIUnregisterDevice();
    //deinit global resource
    AI_DeinitRM();
}


