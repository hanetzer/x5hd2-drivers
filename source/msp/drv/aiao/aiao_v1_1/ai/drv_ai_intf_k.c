/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: ai_intf_k.c
 * Description: ai interface of module.
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

#include "drv_aiao_comm.h"


DECLARE_MUTEX(g_AIMutex);

static atomic_t g_AIOpenCnt = ATOMIC_INIT(0);


//AI Resource
static AI_GLOBAL_RESOURCE_S  *g_pstGlobalAIRS;

/************************************************************************/
static HI_S32 AI_ProcessCmd(HI_U32 cmd, HI_VOID *arg, AI_STATE_S *pState, HI_BOOL bUser)
{
    AI_STATE_S    *pAIState;
    HI_S32 Ret = HI_SUCCESS;

    if(HI_NULL == pState)
    {
        //TO DO
    }
    pAIState = pState;

    switch(cmd)
    {
        //Record CMD TYPE (call hal_aiao)
        case CMD_AI_Init:
            break;
        case CMD_AI_DeInit:
            break;
        case CMD_AI_GetDefaultAttr:
            break;
        case CMD_AI_Create:
            break;
        case CMD_AI_Destory:
            break;
        case CMD_AI_Start:
            break;
        case CMD_AI_Stop:
            break;
        case CMD_AI_SetAttr:
            break;
        case CMD_AI_GetAttr:
            break;
        case CMD_AI_GetFrame:
            break;
            
        default:
            break;

    }

    return HI_SUCCESS;

}

long AI_DRV_Ioctl(struct file *file, unsigned int cmd, HI_VOID *arg)
{
    AI_STATE_S    *pAIState;
    long Ret;

    Ret = down_interruptible(&g_AIMutex);

    pAIState = file->private_data;

    //cmd process
    Ret = (long)AI_ProcessCmd(cmd, arg, pAIState, HI_TRUE);
    if(HI_SUCCESS != Ret)
    {
        //to do
    }

    up(&g_AIMutex);
    return Ret;
}


HI_S32 AI_DRV_Open(struct inode *finode, struct file  *ffile)
{
    //TO DO
    //check open times

    AI_STATE_S    *pAIState = HI_NULL;

    pAIState = HI_KMALLOC(HI_ID_AI, sizeof(AI_STATE_S), GFP_KERNEL);
    if (!pAIState)
    {
        HI_ERR_DRV_AI("malloc pAIState failed.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 AI_DRV_Release(struct inode *finode, struct file  *ffile)
{
    //TO DO
    return HI_SUCCESS;
}

HI_VOID AI_InitRM(HI_VOID)
{
    //TO DO
    //init AOE RS
    return HI_SUCCESS;
}

HI_VOID AI_DeinitRM(HI_VOID)
{
    //TO DO
    //deinit AOE RS
    return HI_SUCCESS;
}

