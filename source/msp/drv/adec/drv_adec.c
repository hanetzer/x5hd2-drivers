/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		    : 	adec_intf.c
  Version        		    : 	Initial Draft
  Author         		    : 	Hisilicon multimedia software group
  Created       		    : 	2006/01/23
  Last Modified		        :
  Description  		        :
  Function List 		:	So Much ....
  History       		:
  1.Date        		: 	2006/01/23
    Author      		: 	f47391
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
#include "hi_error_mpi.h"
#include "drv_adec_ext.h"
#include "hi_drv_adec.h"
#include "hi_kernel_adapt.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiADEC_KADDR_S
{
    HI_BOOL            bUsed;
    ADEC_PROC_ITEM_S  *psAdecKernelAddr;
	MMZ_BUFFER_S 	   AdecProcMmz;
    HI_CHAR 		   szProcMmzName[32];
} ADEC_KADDR_S;

static ADEC_KADDR_S g_sAdecKAddrArray[ADEC_INSTANCE_MAXNUM];
static UMAP_DEVICE_S g_sAdecDevice;
HI_DECLARE_MUTEX(g_AdecMutex);

static HI_S32 ADECConvertSampleRate(HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    return (HI_S32)enSampleRate;
}

#if 0
static HI_VOID ADECConvertCodecType(HI_U32 u32CodecID, HI_CHAR *pszType)
{
    switch (u32CodecID)
    {
    case HA_AUDIO_ID_AAC:
    {
        strcpy((HI_CHAR *)pszType, "AAC");
        break;
    }
    case HA_AUDIO_ID_MP3:
    {
        strcpy((HI_CHAR *)pszType, "MP3");
        break;
    }
    case HA_AUDIO_ID_MP2:
    {
        strcpy((HI_CHAR *)pszType, "MP2");
        break;
    }
    case HA_AUDIO_ID_WMA9STD:
    {
        strcpy((HI_CHAR *)pszType, "WMA9STD");
        break;
    }
    case HA_AUDIO_ID_DRA:
    {
        strcpy((HI_CHAR *)pszType, "DRA");
        break;
    }
    case HA_AUDIO_ID_PCM:
    {
        strcpy((HI_CHAR *)pszType, "PCM");
        break;
    }
    case HA_AUDIO_ID_AMRNB:
    {
        strcpy((HI_CHAR *)pszType, "AMRNB");
        break;
    }
    default:
    {
        strcpy((HI_CHAR *)pszType, "UNKNOWN");
        break;
    }
    }

    return;
}

#endif

/*****************************************************************************
 Prototype    : ADECResetVKaddrArray
 Description  : Reset
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
static HI_VOID ADECResetVKaddrArray(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < ADEC_INSTANCE_MAXNUM; i++)
    {
        g_sAdecKAddrArray[i].bUsed = HI_FALSE;
        g_sAdecKAddrArray[i].psAdecKernelAddr = NULL;
    }

    return;
}

/* Register HIAO Dev                                                         */
static HI_S32 ADECRegisterDevice(struct file_operations *drvFops, PM_BASEOPS_S *drvops)
{
    ADECResetVKaddrArray();

    /*register adec Dev*/
    sprintf(g_sAdecDevice.devfs_name, "%s", UMAP_DEVNAME_ADEC);
    g_sAdecDevice.fops  = drvFops;
    g_sAdecDevice.minor = UMAP_MIN_MINOR_ADEC;
	g_sAdecDevice.owner  = THIS_MODULE;
	g_sAdecDevice.drvops = drvops;
    if (HI_DRV_DEV_Register(&g_sAdecDevice) < 0)
    {
        //HI_ERR_ADEC("adec device register failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Unregister adec Dev                                                       */
static HI_VOID ADECUnregisterDevice(HI_VOID)
{
    HI_DRV_DEV_UnRegister(&g_sAdecDevice);

    return;
}

/*****************************************************************************
 Prototype    : ADEC_DRV_Open
 Description  : Open
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
static HI_S32 ADEC_DRV_Open(struct inode * inode, struct file * filp)
{
    HI_U32 i;
    HI_S32 Ret;

    Ret = down_interruptible(&g_AdecMutex);

    for (i = 0;  i < ADEC_INSTANCE_MAXNUM;  i++)
    {
        if (g_sAdecKAddrArray[i].bUsed == HI_FALSE)
        {
            break;
        }
    }

    if (i >= ADEC_INSTANCE_MAXNUM)
    {
        up(&g_AdecMutex);
        return -1;
    }

    g_sAdecKAddrArray[i].bUsed = HI_TRUE;
    g_sAdecKAddrArray[i].psAdecKernelAddr = NULL;
	sprintf(g_sAdecKAddrArray[i].szProcMmzName, "%s%02d", "adec", i);
	filp->private_data = ((void *)(&g_sAdecKAddrArray[i]));

    HI_DRV_PROC_AddModule(g_sAdecKAddrArray[i].szProcMmzName, ADEC_DRV_Proc, NULL);

    
    up(&g_AdecMutex);
    return 0;
}

/*****************************************************************************
 Prototype    : ADEC_DRV_Release
 Description  : Release
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
static HI_S32 ADEC_DRV_Release(struct inode * inode, struct file * filp)
{
    ADEC_KADDR_S  *psKAddrElem;
    HI_S32 Ret;
    Ret = down_interruptible(&g_AdecMutex);
    psKAddrElem = (ADEC_KADDR_S  *) filp->private_data;

    if (!psKAddrElem)
    {
        up(&g_AdecMutex);
        return -1;
    }

    if (psKAddrElem->bUsed != HI_TRUE)
    {
        up(&g_AdecMutex);
        return -1;
    }
    
    HI_DRV_PROC_RemoveModule(psKAddrElem->szProcMmzName);
    
	if (psKAddrElem->psAdecKernelAddr)
	{
		HI_DRV_MMZ_UnmapAndRelease(&psKAddrElem->AdecProcMmz);
	}
	psKAddrElem->psAdecKernelAddr = NULL;
    psKAddrElem->bUsed = HI_FALSE;
    up(&g_AdecMutex);
    return 0;
}

/*****************************************************************************
 Prototype    : ADEC_DRV_Ioctl
 Description  : Ioctl
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/

static long ADEC_DRV_Ioctl(struct file *filp,
                             HI_U32 cmd, unsigned long arg)
{
    HI_S32 Ret;
    ADEC_KADDR_S  *psKAddrElem;

    Ret = down_interruptible(&g_AdecMutex);

    psKAddrElem = (ADEC_KADDR_S  *) filp->private_data;

    if (!psKAddrElem)
    {
        //HI_ERR_ADEC(KERN_ERR "psKAddrElem == NULL");
        up(&g_AdecMutex);
        return -1;
    }

    if (psKAddrElem->bUsed != HI_TRUE)
    {
        //HI_ERR_ADEC(KERN_ERR "psKAddrElem->bUsed != HI_TRUE");
        up(&g_AdecMutex);
        return -1;
    }

    switch (cmd)
    {
	case DRV_ADEC_PROC_INIT:
	{
		Ret = HI_DRV_MMZ_AllocAndMap(psKAddrElem->szProcMmzName, MMZ_OTHERS, sizeof(ADEC_PROC_ITEM_S), 0, &psKAddrElem->AdecProcMmz);
		if (HI_SUCCESS != Ret)
		{
			up(&g_AdecMutex);
			return -1;
		}
		if (!psKAddrElem->psAdecKernelAddr)
		{
			psKAddrElem->psAdecKernelAddr = (ADEC_PROC_ITEM_S *)psKAddrElem->AdecProcMmz.u32StartVirAddr;
		}
		if(copy_to_user((void*)arg, &psKAddrElem->AdecProcMmz.u32StartPhyAddr, sizeof(HI_U32)))
		{
			up(&g_AdecMutex);
			return -1;
		}
		break;
	}
	case DRV_ADEC_PROC_EXIT:
	{
		HI_DRV_MMZ_UnmapAndRelease(&psKAddrElem->AdecProcMmz);
		psKAddrElem->psAdecKernelAddr = NULL;
		break;
	}
    default:
    {
        up(&g_AdecMutex);
        return -1;
    }
    }

    up(&g_AdecMutex);
    return 0;
}

static struct file_operations ADEC_DRV_Fops =
{
    .owner   		= THIS_MODULE,
    .open    		= ADEC_DRV_Open,
    .unlocked_ioctl = ADEC_DRV_Ioctl,
    .release 		= ADEC_DRV_Release,
};

/*****************************************************************************
 Prototype    : ADEC_DRV_Proc
 Description  : Proc
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
HI_S32 ADEC_DRV_Proc(struct seq_file *p, HI_VOID *v)
{
    HI_U32 i, u32BufPercent, u32FramePercent, u32FrameFullNum, u32DataSize;
    ADEC_PROC_ITEM_S *tmp = NULL;
	DRV_PROC_ITEM_S  *pProcItem;
    
    pProcItem = p->private;

    i = (pProcItem->entry_name[4] - '0')*10 + (pProcItem->entry_name[5] - '0');


    p += seq_printf(p, "\n############################# Hisi ADEC Dev %d Stat ############################\n",i);
    
    {
        p += seq_printf(p, "\n---------ADEC[%d] Stat---------\n", i);
        if (g_sAdecKAddrArray[i].bUsed == HI_FALSE)
        {
            p += seq_printf(p, "  ADEC not open\n");
            return HI_SUCCESS;
        }

        if (g_sAdecKAddrArray[i].psAdecKernelAddr == NULL)
        {
            p += seq_printf(p, "  ADEC PROC not INIT\n");
            return HI_SUCCESS;
        }

        tmp = (ADEC_PROC_ITEM_S *)g_sAdecKAddrArray[i].psAdecKernelAddr;
        if (tmp->u32BufSize)
        {
            if (tmp->u32BufWrite >= tmp->s32BufRead)
            {
                u32DataSize    = (tmp->u32BufWrite - tmp->s32BufRead);
            }
            else
            {
                u32DataSize    = (tmp->u32BufSize - tmp->s32BufRead) + tmp->u32FrameWrite;
            }
             u32BufPercent = u32DataSize * 100 / tmp->u32BufSize;
        }
        else
        {
            u32BufPercent = 0;
            u32DataSize = 0;
        }

        if (tmp->u32FrameSize)
        {
            if (tmp->u32FrameWrite >= tmp->u32FrameRead)
            {
                u32FramePercent = (tmp->u32FrameWrite - tmp->u32FrameRead) * 100 / tmp->u32FrameSize;
                u32FrameFullNum = tmp->u32FrameWrite - tmp->u32FrameRead;
            }
            else
            {
                u32FramePercent = ((tmp->u32FrameSize
                                    - tmp->u32FrameRead) + tmp->u32FrameWrite) * 100 / tmp->u32FrameSize;
                u32FrameFullNum = tmp->u32FrameSize - tmp->u32FrameRead + tmp->u32FrameWrite;
            }
        }
        else
        {
            u32FramePercent = 0;
            u32FrameFullNum = 0;
        }

        p += seq_printf(p,
                        "Work State                     :%s\n"
                        "Codec ID                       :%s(0x%x)\n"
			   "Description			:%s\n"
                        "Volume                         :%d\n"
                        "Sample Rate                    :%d\n"
                        "Bit Width                      :%d\n"
                        "Pts Lost Num                   :%d\n"
                        "FrameNum(Total/Err)            :%d/%d\n"
                        "Stream Buf(Size/Data/Percent)  :%u/%d/%u%%\n"
                        "Stream Buf(readPos/writePos)   :0x%x/0x%x\n"
                        "OutFrameBuf(Total/Use/Percent) :%u/%u/%u%%\n\n"
                        "GetBuffer(Try/OK)              :%u/%u\n"
                        "PutBuffer(Try/OK)              :%u/%u\n"
                        "ReceiveFrame(Try/OK)           :%u/%u\n"
                        "SendStream(Try/OK)             :%u/%u\n"
                        "Try  Decode times              :%u\n"
						"Stream Fmt						:%d\n",
                        (tmp->bAdecWorkEnable == HI_TRUE) ? "ON" : "OFF",
                        tmp->szCodecType, tmp->u32CodecID,
			   tmp->szCodecDescription,
                        tmp->u32Volume,
                        (ADECConvertSampleRate(tmp->enSampleRate)),
                        tmp->enBitWidth,
                        tmp->u32PtsLost,
                        tmp->u32FramnNm,
                        (HI_S32)tmp->u32ErrFrameNum,
                        tmp->u32BufSize,
                        u32DataSize,
                        u32BufPercent,
                        tmp->s32BufRead,
                        tmp->u32BufWrite,
                        tmp->u32FrameSize,
                        u32FrameFullNum,
                        u32FramePercent,
                        tmp->u32DbgGetBufCount_Try,
                        tmp->u32DbgGetBufCount,
                        tmp->u32DbgPutBufCount_Try,
                        tmp->u32DbgPutBufCount,
                        tmp->u32DbgReceiveFrameCount_Try,
                        tmp->u32DbgReceiveFrameCount,
                        tmp->u32DbgSendStraemCount_Try,
                        tmp->u32DbgSendStraemCount,
                        tmp->u32DbgTryDecodeCount,
                        tmp->enFmt);
    }

    p += seq_printf(p, "\n#########################################################################\n" );
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ADEC_DRV_Suspend
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
static int adec_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
//	int i;
	int ret;
	// 0
	ret = down_trylock(&g_AdecMutex);
	if(ret){
		HI_FATAL_ADEC("lock err!\n");
		return -1;
	}
	// 1
#if 0
	for (i = 0;  i < ADEC_INSTANCE_MAXNUM;  i++) {
		if (g_sAdecKAddrArray[i].bUsed) {
			up(&g_AdecMutex);
			HI_FATAL_ADEC("chan=%d used !\n", i);
			return -1;
		}
	}
#endif
	up(&g_AdecMutex);
	HI_FATAL_ADEC("ok \n");
    return 0;
}


/*****************************************************************************
 Prototype    : ADEC_DRV_Resume
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
static int adec_pm_resume(PM_BASEDEV_S *pdev)
{
#if 0
	down_trylock(&g_AdecMutex);
	up(&g_AdecMutex);
#endif
    HI_FATAL_ADEC("ok \n");
	return 0;
}

static PM_BASEOPS_S adec_drvops = {
	.probe        = NULL,
	.remove       = NULL,
	.shutdown     = NULL,
	.prepare      = NULL,
	.complete     = NULL,
	.suspend      = adec_pm_suspend,
	.suspend_late = NULL,
	.resume_early = NULL,
	.resume       = adec_pm_resume,
};

/*****************************************************************************
 Prototype    : ADEC_DRV_ModInit
 Description  : ModelInit
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
HI_S32 ADEC_DRV_ModInit(HI_VOID)
{
    /*register ADEC device*/
    HI_S32 ret;
#ifndef HI_MCE_SUPPORT    
    ret = HI_DRV_ADEC_Init();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_ADEC("ADEC_DRV_Init Fali \n");
        return ret;
    }
#endif    
    ADECRegisterDevice(&ADEC_DRV_Fops, &adec_drvops);
#ifndef CONFIG_SUPPORT_CA_RELEASE
	printk("Load hi_adec.ko success.  \t(%s)\n", VERSION_STRING);
#endif
    return HI_SUCCESS;
}

HI_VOID  ADEC_DRV_ModExit(HI_VOID)
{
    /*unregister ADEC device*/

    ADECUnregisterDevice();
#ifndef HI_MCE_SUPPORT 
    HI_DRV_ADEC_DeInit();
#endif
    return;
}

#ifdef MODULE
module_init(ADEC_DRV_ModInit);
module_exit(ADEC_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
