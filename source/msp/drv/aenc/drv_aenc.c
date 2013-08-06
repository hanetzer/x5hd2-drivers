/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		    : 	drv_aenc.c
  Version        		    : 	Initial Draft
  Author         		    : 	Hisilicon multimedia software group
  Created       		    : 	2006/01/23
  Last Modified		        :
  Description  		        :
  Function List 		:	So Much ....
  History       		:
  1.Date        		: 	2010/03/11
    Author      		: 	z40717
    Modification   	    :	Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/seq_file.h>

#include "drv_mmz_ext.h"
#include "drv_stat_ext.h"
#include "drv_sys_ext.h"
#include "drv_proc_ext.h"
//#include "drv_log_ext.h"
//#include "drv_event_ext.h"

//#include "mpi_sys.h"

#include "hi_drv_aenc.h"

#include "drv_mmz_ext.h"
#include "hi_kernel_adapt.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiAENC_KADDR_S
{
    HI_BOOL            bUsed;
    AENC_PROC_ITEM_S  *psAencKernelAddr;
    MMZ_BUFFER_S 	   AencProcMmz;
    HI_CHAR 		   szProcMmzName[32];
} AENC_KADDR_S;

static AENC_KADDR_S g_sAencKAddrArray[AENC_INSTANCE_MAXNUM];
static UMAP_DEVICE_S g_sAencDevice;
HI_DECLARE_MUTEX(g_AencMutex);

static HI_VOID AENCResetVKaddrArray(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < AENC_INSTANCE_MAXNUM; i++)
    {
        g_sAencKAddrArray[i].bUsed = HI_FALSE;
        g_sAencKAddrArray[i].psAencKernelAddr = NULL;
    }

    return;
}

/* Register AENC Dev                                                         */
static HI_S32 AENCRegisterDevice(struct file_operations *drvFops, PM_BASEOPS_S *drvops)
{
    AENCResetVKaddrArray();

    /*register AENC Dev*/
    sprintf(g_sAencDevice.devfs_name, "%s", UMAP_DEVNAME_AENC);
    g_sAencDevice.fops  = drvFops;
    g_sAencDevice.minor = UMAP_MIN_MINOR_AENC;
	g_sAencDevice.owner  = THIS_MODULE;
	g_sAencDevice.drvops = drvops;
    if (HI_DRV_DEV_Register(&g_sAencDevice) < 0)
    {
        //HI_ERR_AENC("AENC device register failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Unregister AENC Dev                                                       */
static HI_VOID AENCUnregisterDevice(HI_VOID)
{
    HI_DRV_DEV_UnRegister(&g_sAencDevice);

    return;
}

static HI_S32 AENC_DRV_Open(struct inode * inode, struct file * filp)
{
    HI_U32 i;
    HI_S32 Ret;

    Ret = down_interruptible(&g_AencMutex);

    for (i = 0;  i < AENC_INSTANCE_MAXNUM;  i++)
    {
        if (g_sAencKAddrArray[i].bUsed == HI_FALSE)
        {
            break;
        }
    }

    if (i >= AENC_INSTANCE_MAXNUM)
    {
        up(&g_AencMutex);
        return -1;
    }

    g_sAencKAddrArray[i].bUsed = HI_TRUE;
    g_sAencKAddrArray[i].psAencKernelAddr = NULL;
    sprintf(g_sAencKAddrArray[i].szProcMmzName, "%s%02d", "AENCProc", i);
    filp->private_data = ((void *)(&g_sAencKAddrArray[i]));
    up(&g_AencMutex);
    return 0;
}

static HI_S32 AENC_DRV_Release(struct inode * inode, struct file * filp)
{
    AENC_KADDR_S  *psKAddrElem;
    HI_S32 Ret;

    Ret = down_interruptible(&g_AencMutex);
    psKAddrElem = (AENC_KADDR_S  *) filp->private_data;

    if (!psKAddrElem)
    {
        up(&g_AencMutex);
        return -1;
    }

    if (psKAddrElem->bUsed != HI_TRUE)
    {
        up(&g_AencMutex);
        return -1;
    }
    if (psKAddrElem->psAencKernelAddr)
	{
		HI_DRV_MMZ_UnmapAndRelease(&psKAddrElem->AencProcMmz);
	}

    psKAddrElem->psAencKernelAddr = NULL;
    psKAddrElem->bUsed = HI_FALSE;
    up(&g_AencMutex);
    return 0;
}

static long AENC_DRV_Ioctl(struct file *filp,
                             HI_U32 cmd, unsigned long arg)
{
    AENC_KADDR_S  *psKAddrElem;
    HI_S32 Ret;

    Ret = down_interruptible(&g_AencMutex);
    psKAddrElem = (AENC_KADDR_S  *) filp->private_data;

    if (!psKAddrElem)
    {
        //HI_ERR_ADEC(KERN_ERR "psKAddrElem == NULL");
        up(&g_AencMutex);
        return -1;
    }

    if (psKAddrElem->bUsed != HI_TRUE)
    {
        //HI_ERR_ADEC(KERN_ERR "psKAddrElem->bUsed != HI_TRUE");
        up(&g_AencMutex);
        return -1;
    }

    switch (cmd)
    {
    case DRV_AENC_PROC_INIT:
    {
        Ret = HI_DRV_MMZ_AllocAndMap(psKAddrElem->szProcMmzName, MMZ_OTHERS, sizeof(AENC_PROC_ITEM_S), 0, &psKAddrElem->AencProcMmz);
		if (HI_SUCCESS != Ret)
		{
			up(&g_AencMutex);
			return -1;
		}
		if (!psKAddrElem->psAencKernelAddr)
		{
			psKAddrElem->psAencKernelAddr = (AENC_PROC_ITEM_S *)psKAddrElem->AencProcMmz.u32StartVirAddr;
		}
		if(copy_to_user((void*)arg, &psKAddrElem->AencProcMmz.u32StartPhyAddr, sizeof(HI_U32)))
		{
			up(&g_AencMutex);
			return -1;
		}
		break;
    }
    case DRV_AENC_PROC_EXIT:
    {
        HI_DRV_MMZ_UnmapAndRelease(&psKAddrElem->AencProcMmz);
        psKAddrElem->psAencKernelAddr = NULL;
        break;
    }
    default:
        up(&g_AencMutex);
        return -1;
    }

    up(&g_AencMutex);
    return 0;
}

static struct file_operations AENC_DRV_Fops =
{
    .owner   		= THIS_MODULE,
    .open    		= AENC_DRV_Open,
    .unlocked_ioctl = AENC_DRV_Ioctl,
    .release 		= AENC_DRV_Release,
};

HI_S32 AENC_DRV_Proc(struct seq_file *p, HI_VOID *v)
{
    HI_U32 i, u32BufPercent, u32FramePercent, u32FrameFullNum;
    AENC_PROC_ITEM_S *tmp = NULL;

    p += seq_printf(p, "\n############################# Hisi AENC  Dev Stat ############################\n" );
    for (i = 0; i < AENC_INSTANCE_MAXNUM; i++)
    {
        p += seq_printf(p, "\n---------AENC[%d] Stat---------\n", i);
        if (g_sAencKAddrArray[i].bUsed == HI_FALSE)
        {
            p += seq_printf(p, "  AENC not open\n");
            continue;
        }

        if (g_sAencKAddrArray[i].psAencKernelAddr == NULL)
        {
            p += seq_printf(p, "  AENC PROC not INIT\n");
            continue;
        }

        tmp = (AENC_PROC_ITEM_S *)g_sAencKAddrArray[i].psAencKernelAddr;
        if (tmp->u32InBufSize)
        {
            if (tmp->u32InBufWrite >= tmp->u32InBufRead)
            {
                u32BufPercent = (tmp->u32InBufWrite - tmp->u32InBufRead) * 100 / tmp->u32InBufSize;
            }
            else
            {
                u32BufPercent = ((tmp->u32InBufSize
                                  - tmp->u32InBufRead) + tmp->u32InBufWrite) * 100 / tmp->u32InBufSize;
            }
        }
        else
        {
            u32BufPercent = 0;
        }

        if (tmp->u32OutFrameNum)
        {
            if (tmp->u32OutFrameWIdx >= tmp->u32OutFrameRIdx)
            {
                u32FramePercent = (tmp->u32OutFrameWIdx - tmp->u32OutFrameRIdx) * 100 / tmp->u32OutFrameNum;
                u32FrameFullNum = tmp->u32OutFrameWIdx - tmp->u32OutFrameRIdx;
            }
            else
            {
                u32FramePercent = ((tmp->u32OutFrameNum
                                    - tmp->u32OutFrameRIdx) + tmp->u32OutFrameWIdx) * 100 / tmp->u32OutFrameNum;
                u32FrameFullNum = tmp->u32OutFrameNum - tmp->u32OutFrameRIdx + tmp->u32OutFrameWIdx;
            }
        }
        else
        {
            u32FramePercent = 0;
            u32FrameFullNum = 0;
        }

        p += seq_printf(p,
                        "Work State             :%s\n"
                        "Codec ID               :%s(0x%x)\n"
                        "Sample Rate            :%d\n"
                        "Channels               :%d\n"
                        "Auto SRC               :%s\n"
                        "Enc FrameNum           :%d\n"
                        "Err  FrameNum          :%d\n"
                        "IN Buf Size            :%d\n"
                        "IN Buf Percent         :%d\n"
                        "Out FrameBuf Num       :%d\n"
                        "Out FrameBuf Percent   :%d\n"

                        //"Pts Lost Num         :%d\n"
                        "SendBuffer:    Try=%d, OK=%d\n"
                        "ReceiveStream: Try=%d, OK=%d\n"
                        "Try  Encode times      :%d\n",
                        (tmp->bAdecWorkEnable == HI_TRUE) ? "ON" : "OFF",
                        tmp->szCodecType, tmp->u32CodecID,
                        tmp->u32SampleRate,
                        tmp->u32Channels,
                        (tmp->bAutoSRC == HI_TRUE) ? "ON" : "OFF",
                        tmp->u32EncFrame,
                        tmp->u32ErrFrame,
                        tmp->u32InBufSize,
                        u32BufPercent,
                        tmp->u32OutFrameNum,
                        u32FramePercent,
                        tmp->u32DbgSendBufCount_Try,
                        tmp->u32DbgSendBufCount,
                        tmp->u32DbgReceiveStreamCount_Try,
                        tmp->u32DbgReceiveStreamCount,
                        tmp->u32DbgTryEncodeCount);
    }

    p += seq_printf(p, "\n#########################################################################\n" );
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : AENC_DRV_Suspend
 Description  : Suspend
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2010/5/15
    Author       : wei deng
    Modification : Created function
*****************************************************************************/
static int aenc_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
	int ret;
	// 0 all user process suspend and exit, so every process resorted by this function is fatal error
	ret = down_trylock(&g_AencMutex);
	if(ret){
		HI_FATAL_AENC("lock err!\n");
		return -1;
	}
	// 1
#if 0
    int i;
	for (i = 0;  i < AENC_INSTANCE_MAXNUM;  i++) {
		if (g_sAencKAddrArray[i].bUsed) {
			up(&g_AencMutex);
			HI_FATAL_AENC("chan=%d used !\n", i);
			return -1;
		}
	}
#endif	
	up(&g_AencMutex);
    HI_FATAL_AENC("ok \n");
    return 0;
}

/*****************************************************************************
 Prototype    : AENC_DRV_Resume
 Description  : Resume
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2010/5/15
    Author       : wei deng
    Modification : Created function
*****************************************************************************/
static int aenc_pm_resume(PM_BASEDEV_S *pdev)
{
#if 0
	down_trylock(&g_AencMutex);
	up(&g_AencMutex);
#endif
    HI_FATAL_AENC("ok \n");
	return 0;
}

static PM_BASEOPS_S aenc_drvops = {
	.probe        = NULL,
	.remove       = NULL,
	.shutdown     = NULL,
	.prepare      = NULL,
	.complete     = NULL,
	.suspend      = aenc_pm_suspend,
	.suspend_late = NULL,
	.resume_early = NULL,
	.resume       = aenc_pm_resume,
};


HI_S32 AENC_DRV_ModInit(HI_VOID)
{
    /*register AENC device*/
    AENCRegisterDevice(&AENC_DRV_Fops, &aenc_drvops);

    HI_DRV_PROC_AddModule("aenc", AENC_DRV_Proc, NULL);

    return HI_SUCCESS;
}

HI_VOID  AENC_DRV_ModExit(HI_VOID)
{
    /*unregister AENC device*/
    HI_DRV_PROC_RemoveModule("aenc");

    AENCUnregisterDevice();
    return;
}

#ifdef MODULE
module_init(AENC_DRV_ModInit);
module_exit(AENC_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
