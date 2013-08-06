/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name             :     adec_intf.c
  Version               :     Initial Draft
  Author                :     Hisilicon multimedia software group
  Created               :     2006/01/23
  Last Modified         :
  Description           :
  Function List         :    So Much ....
  History               :
  1.Date                :     2006/01/23
    Author              :     f47391
    Modification        :    Created file

******************************************************************************/
#include <linux/proc_fs.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
//#include <linux/himedia.h>

#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>

#include <linux/miscdevice.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_common.h"
#include "hi_kernel_adapt.h"
#include "drv_dev_ext.h"
#include "drv_mem_ext.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher_ext.h"
#include "drv_advca_ext.h"
#include "drv_mmz_ext.h"
#include "drv_module_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen; /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;    /* mark the data done or not */
    wait_queue_head_t cipher_wait_queue; /* mutex method */
    struct file *pWichFile; /* which file need to operate */

    HI_UNF_CIPHER_DATA_S *pstDataPkg;
    HI_U32                u32DataPkgNum;
}CIPHER_OSR_CHN_S;

ADVCA_EXPORT_FUNC_S  *s_pAdvcaFunc = HI_NULL;

/* initialize mutex variable g_CipherMutexKernel to 1 */
HI_DECLARE_MUTEX(g_CipherMutexKernel);

static UMAP_DEVICE_S    g_CipherDevice;
static CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];

#define CIPHER_CheckHandle(softChnId, file) do{\
        if (HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)\
        {\
            up(&g_CipherMutexKernel);\
            HI_ERR_CIPHER("invalid chn %d, open=%d.\n", softChnId, g_stCipherOsrChn[softChnId].g_bSoftChnOpen);\
            return HI_ERR_CIPHER_INVALID_HANDLE;\
        }\
    }while(0)

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipID);

/*****************************************************************************
 Prototype    : DRV_CIPHER_UserCommCallBack
 Description  : 
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         :
    Author       :
    Modification :
*****************************************************************************/
HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg)
{
    HI_INFO_CIPHER("arg=%#x.\n", arg);

    g_stCipherOsrChn[arg].g_bDataDone = HI_TRUE;
    wake_up_interruptible(&(g_stCipherOsrChn[arg].cipher_wait_queue));

    return ;
}

/*****************************************************************************
 Prototype    : DRV_CIPHER_Release
 Description  : 
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
static HI_S32 DRV_CIPHER_Release(struct inode * inode, struct file * file)
{
    HI_U32 i;

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (g_stCipherOsrChn[i].pWichFile == file)
        {
            DRV_Cipher_CloseChn(i);
            g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
            g_stCipherOsrChn[i].pWichFile = NULL;
			if (g_stCipherOsrChn[i].pstDataPkg)
		    {
			    HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[i].pstDataPkg);
			    g_stCipherOsrChn[i].pstDataPkg = NULL;
		    }
        }
    }

    return 0;
}

/*****************************************************************************
 Prototype    : DRV_CIPHER_Open
 Description  : 
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
static HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file)
{
    // This ptr must be initialized into NULL before getting its value
    s_pAdvcaFunc = HI_NULL;  
    // No need to check the return value of GetFunction
    HI_DRV_MODULE_GetFunction(HI_ID_CA, (HI_VOID**)&s_pAdvcaFunc);

    return 0;
}

/*****************************************************************************
 Prototype    : DRV_CIPHER_Ioctl
 Description  : 
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

HI_S32 CIPHER_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *argp)
{
    HI_S32 Ret = 0;

    if(down_interruptible(&g_CipherMutexKernel))
    {
        return -1;
    }
	
    switch(cmd)
    {
	    case CMD_CIPHER_CREATEHANDLE:
		{
	        HI_U32 i = 0;
	        HI_HANDLE  Cipherchn = 0;   /* cipher handle */
	        HI_U32 softChnId = 0;       /* soft channel ID */
	        CIPHER_HANDLE_S *pstCIHandle = (CIPHER_HANDLE_S *)argp;
	
	        HI_CHIP_TYPE_E enChip;
	        HI_CHIP_VERSION_E enChipVersion;
	
	        HI_DRV_SYS_GetChipVersion(&enChip, &enChipVersion);
	        
	        /* check opened or not */
	        if ( HI_UNF_CIPHER_TYPE_NORMAL != pstCIHandle->stCipherAtts.enCipherType )
	        {
	        	if( HI_UNF_CIPHER_TYPE_BUTT == pstCIHandle->stCipherAtts.enCipherType )
	        	{
	                    Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
	                    HI_ERR_CIPHER("Invalid Cipher Type!\n");
	                    break;
	            }
				else if( HI_UNF_CIPHER_TYPE_COPY_AVOID == pstCIHandle->stCipherAtts.enCipherType )
	            {
	                if( (HI_CHIP_TYPE_HI3712 != enChip) && (HI_CHIP_TYPE_HI3716CES != enChip) )
	                {
	                    Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
	                    HI_ERR_CIPHER("Not supported!\n");
	                    break;
	                }
	                else
	                {
	                	/* for reserved */
	                }
	            }
	            else
	            {
	            	/* for reserved */
	            }
	            
	            if (0 == g_stCipherOsrChn[0].g_bSoftChnOpen)
	            {
	                i = 0;
	            }
	            else
	            {
	                i = CIPHER_INVALID_CHN;
	            }
	        }
	        else
	        {
	            for(i = CIPHER_PKGxN_CHAN_MIN; i < CIPHER_SOFT_CHAN_NUM; i++)
	            {
	                if (0 == g_stCipherOsrChn[i].g_bSoftChnOpen)
	                {
	                    break;
	                }
	            }
	        }
	
	        if (i >= CIPHER_SOFT_CHAN_NUM)
	        {
	            Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
	            HI_ERR_CIPHER("No more cipher chan left.\n");
	            break;
	        }
	        else /* get a free chn */
	        {
	            g_stCipherOsrChn[i].pstDataPkg = HI_VMALLOC(HI_ID_CIPHER, sizeof(HI_UNF_CIPHER_DATA_S) * CI_MAX_LIST_NUM);
	            if (NULL == g_stCipherOsrChn[i].pstDataPkg)
	            {
	                Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
	                HI_ERR_CIPHER("can NOT malloc memory for cipher.\n");
	                break;
	            }
	                
	            softChnId = i;
	            g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;
	        }
	
	        Cipherchn = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, softChnId);
	        HI_INFO_CIPHER("the softChnId and the handle are %d %#x\n", softChnId, Cipherchn);
	
	        Ret = DRV_Cipher_OpenChn(softChnId);
	        if (HI_SUCCESS != Ret)
	        {
	            HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[i].pstDataPkg);
	            g_stCipherOsrChn[i].pstDataPkg = NULL;
	            break;
	        }
	
	        /* return CIHANDLE */
	        pstCIHandle->hCIHandle = Cipherchn;
	        g_stCipherOsrChn[softChnId].pWichFile = file;
	        Ret = HI_SUCCESS;

	        break;
	    }
	
	    case CMD_CIPHER_DESTROYHANDLE:
	    {
	        HI_HANDLE  Cipherchn = 0;   /* cipher handle */
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        Cipherchn = *(HI_HANDLE *)argp;
	
	        softChnId = HI_HANDLE_GET_CHNID(Cipherchn);        
	        if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
	            ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
	        {
	            Ret = HI_SUCCESS; /* success on re-Destroy */
	            break;
	        }
	
		if (g_stCipherOsrChn[softChnId].pstDataPkg)
		{
			HI_VFREE(HI_ID_CIPHER, g_stCipherOsrChn[softChnId].pstDataPkg);
			g_stCipherOsrChn[softChnId].pstDataPkg = NULL;
	        }
	
	        g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_FALSE;
	        g_stCipherOsrChn[softChnId].pWichFile = NULL;
	        Ret = DRV_Cipher_CloseChn(softChnId);
	        break;
	    }
	
	    case CMD_CIPHER_CONFIGHANDLE:
	    {
	        CIPHER_Config_CTRL  CIConfig = *(CIPHER_Config_CTRL *)argp;
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        softChnId = HI_HANDLE_GET_CHNID(CIConfig.CIHandle);  
	        CIPHER_CheckHandle(softChnId, file);
	        Ret = DRV_Cipher_ConfigChn(softChnId, &CIConfig.CIpstCtrl, DRV_CIPHER_UserCommCallBack);
	        
	        break;
	    }
	
	    case CMD_CIPHER_ENCRYPT:
	    {
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        CIPHER_DATA_S       CIData = *(CIPHER_DATA_S *)argp;
	        HI_DRV_CIPHER_TASK_S       pCITask;
	
	        softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);
	
	        CIPHER_CheckHandle(softChnId, file);
	        
	        if ( 0 != softChnId )
	        {
	            /* channel 1~7 */            
	            pCITask.stData2Process.u32src = CIData.ScrPhyAddr;
	            pCITask.stData2Process.u32dest = CIData.DestPhyAddr;
	            pCITask.stData2Process.u32length = CIData.ByteLength;
	            pCITask.stData2Process.bDecrypt = HI_FALSE;
	            pCITask.u32CallBackArg = softChnId;
	
	            HI_INFO_CIPHER("Start to Encrypt, chnNum = %#x!\n", softChnId);
	
	            g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
	
	            up(&g_CipherMutexKernel); /*  */
	            Ret = DRV_Cipher_CreatTask(softChnId,&pCITask, NULL, NULL);
	            if (HI_SUCCESS != Ret)
	            {
	                return Ret;
	            }
	
	            if (0 == wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
	                        g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
	            {
	                HI_ERR_CIPHER("Encrypt time out! \n");
	                return HI_FAILURE;
	            }
	
	            if(down_interruptible(&g_CipherMutexKernel))
	            {
	                return -ERESTARTSYS;
	            }
	            HI_INFO_CIPHER("Encrypt OK, chnNum = %#x!\n", softChnId);
	            Ret = HI_SUCCESS;
	            break;
	        }
	        else
	        {
	            /* channel 0 */
	            MMZ_BUFFER_S stSrcMmzBuf;
	            MMZ_BUFFER_S stDestMmzBuf;            
				
	            if ( CIData.ByteLength % 16 != 0)
	            {
	                HI_ERR_CIPHER("Invalid encrypt length, must be multiple of 16 bytes!\n");
	                Ret = HI_FAILURE;
	                break;
	            }
	            
	            stSrcMmzBuf.u32Size = CIData.ByteLength;
	            stSrcMmzBuf.u32StartPhyAddr = CIData.ScrPhyAddr;
	            Ret = HI_DRV_MMZ_Map(&stSrcMmzBuf);
	            if ( HI_FAILURE == Ret)
	            {
	                HI_ERR_CIPHER("DRV SRC MMZ MAP ERROR!\n");
	                break;
	            }
	
	            stDestMmzBuf.u32Size = CIData.ByteLength;
	            stDestMmzBuf.u32StartPhyAddr = CIData.DestPhyAddr;
	            Ret = HI_DRV_MMZ_Map(&stDestMmzBuf);
	            if ( HI_FAILURE == Ret)
	            {
	                HI_ERR_CIPHER("DRV DEST MMZ MAP ERROR!\n");
	                break;
	            }
	            
	            CIPHER_CheckHandle(softChnId, file);
	
	            memcpy((HI_U8 *)(pCITask.stData2Process.u32DataPkg), (HI_U8 *)stSrcMmzBuf.u32StartVirAddr, CIData.ByteLength);
	            pCITask.stData2Process.u32length = CIData.ByteLength;
	            pCITask.stData2Process.bDecrypt = HI_FALSE;
	            pCITask.u32CallBackArg = softChnId;
	
	            up(&g_CipherMutexKernel);
	            Ret = DRV_Cipher_CreatTask(softChnId, &pCITask, NULL, NULL);
	            if (HI_SUCCESS != Ret)
	            { 
	                Ret = HI_FAILURE;
	                break;
	            }
	            
	            memcpy((HI_U8 *)stDestMmzBuf.u32StartVirAddr, (HI_U8 *)(pCITask.stData2Process.u32DataPkg), CIData.ByteLength);
	            HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
	            HI_DRV_MMZ_Unmap(&stDestMmzBuf);
	            
	            Ret = HI_SUCCESS;
	            break;            
	        }
	        
	    }
	
	    case CMD_CIPHER_DECRYPT:
	    {
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        CIPHER_DATA_S       CIData = *(CIPHER_DATA_S *)argp;
	        HI_DRV_CIPHER_TASK_S       pCITask;
	        
	        softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);
	        CIPHER_CheckHandle(softChnId, file);
	        
	        if ( 0 != softChnId)
	        {
	            /* chanel 1~7 */
	            pCITask.stData2Process.u32src=CIData.ScrPhyAddr;
	            pCITask.stData2Process.u32dest=CIData.DestPhyAddr;
	            pCITask.stData2Process.u32length=CIData.ByteLength;
	            pCITask.stData2Process.bDecrypt=HI_TRUE;
	
	            pCITask.u32CallBackArg=softChnId;
	
	            HI_INFO_CIPHER("Start to Decrypt, chnNum = %#x!\n", softChnId);
	
	            g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
	
	            up(&g_CipherMutexKernel);
	            Ret = DRV_Cipher_CreatTask(softChnId,&pCITask,NULL,NULL);
	            if (HI_SUCCESS != Ret)
	            {
	                return Ret;
	            }
	
	            if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
	                        g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
	            {
	                HI_ERR_CIPHER("Decrypt time out \n");
	                return HI_FAILURE;
	            }
	
	            if(down_interruptible(&g_CipherMutexKernel))
	            {
	                return -ERESTARTSYS;
	            }
	
	            HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);
	            Ret = HI_SUCCESS;
	            break;
	        }
	        else
	        {
	            /* channel 0 */
	            MMZ_BUFFER_S stSrcMmzBuf;
	            MMZ_BUFFER_S stDestMmzBuf;
	
	            if ( CIData.ByteLength % 16 != 0)
	            {
	                HI_ERR_CIPHER("Invalid decrypt length, must be multiple of 16 bytes!\n");
	                Ret = HI_FAILURE;
	                break;
	            }
	            
	            stSrcMmzBuf.u32Size = CIData.ByteLength;
	            stSrcMmzBuf.u32StartPhyAddr = CIData.ScrPhyAddr;
	            Ret = HI_DRV_MMZ_Map(&stSrcMmzBuf);
	            if ( HI_FAILURE == Ret)
	            {
	                HI_ERR_CIPHER("DRV SRC MMZ MAP ERROR!\n");
	                break;
	            }
	
	            stDestMmzBuf.u32Size = CIData.ByteLength;
	            stDestMmzBuf.u32StartPhyAddr = CIData.DestPhyAddr;
	            Ret = HI_DRV_MMZ_Map(&stDestMmzBuf);
	            if ( HI_FAILURE == Ret)
	            {
	                HI_ERR_CIPHER("DRV DEST MMZ MAP ERROR!\n");
	                break;
	            }
	            
	            memcpy((HI_U8 *)(pCITask.stData2Process.u32DataPkg), (HI_U8 *)stSrcMmzBuf.u32StartVirAddr, CIData.ByteLength);
	            pCITask.stData2Process.u32length = CIData.ByteLength;
	            pCITask.stData2Process.bDecrypt = HI_TRUE;
	            pCITask.u32CallBackArg = softChnId;
	
	            up(&g_CipherMutexKernel);
	            Ret = DRV_Cipher_CreatTask(softChnId, &pCITask, NULL, NULL);
	            if (HI_SUCCESS != Ret)
	            { 
	                Ret = HI_FAILURE;
	                break;
	            }
	            
	            memcpy((HI_U8 *)stDestMmzBuf.u32StartVirAddr, (HI_U8 *)(pCITask.stData2Process.u32DataPkg), CIData.ByteLength);
	
	            HI_DRV_MMZ_Unmap(&stSrcMmzBuf);
	            HI_DRV_MMZ_Unmap(&stDestMmzBuf);
	
	            Ret = HI_SUCCESS;
	            break;
	        }
	        
	    }
	    case CMD_CIPHER_ENCRYPTMULTI:
	    {
	        HI_U32 i;
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        CIPHER_DATA_S CIData = *(CIPHER_DATA_S *)argp;
	        static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CI_MAX_LIST_NUM];
	        HI_UNF_CIPHER_DATA_S *pTmp;
	        HI_U32 pkgNum;
	
	        softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);
	        CIPHER_CheckHandle(softChnId, file);
	
	        pkgNum = CIData.ByteLength;
	        if (pkgNum > CI_MAX_LIST_NUM)
	        {
	            HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",pkgNum, CI_MAX_LIST_NUM);
	            Ret = HI_ERR_CIPHER_INVALID_PARA;
	            break;
	        }
	        
	        if (copy_from_user(g_stCipherOsrChn[softChnId].pstDataPkg, (void*)CIData.ScrPhyAddr,
	                           pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)))
	        {
	            HI_ERR_CIPHER("copy data from user fail!\n");
	            Ret = HI_FAILURE;
	            break;
	        }
	       
	        
	        for (i = 0; i < CIData.ByteLength; i++)
	        {
	            pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
	            tmpData[i].bDecrypt = HI_FALSE;
	            tmpData[i].u32src = pTmp->u32SrcPhyAddr;
	            tmpData[i].u32dest = pTmp->u32DestPhyAddr;
	            tmpData[i].u32length = pTmp->u32ByteLength;
	        }
	
	        HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %#x, pkgNum=%d!\n", softChnId, pkgNum);
	
	        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
	
	        up(&g_CipherMutexKernel); /* ??? */
	        Ret = DRV_Cipher_CreatMultiPkgTask(softChnId, tmpData, pkgNum, softChnId);
	        if (HI_SUCCESS != Ret)
	        {
	            return Ret;
	        }
	
	        if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
	                    g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
	        {
	            HI_ERR_CIPHER("Decrypt time out \n");
	            return HI_FAILURE;
	        }
	
	        if(down_interruptible(&g_CipherMutexKernel))
	        {
	            return -ERESTARTSYS;
	        }
	
	        HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);
	        Ret = HI_SUCCESS;
	        break;
	    }
	    case CMD_CIPHER_DECRYPTMULTI:
	    {
	        HI_U32 i;
	        HI_U32 softChnId = 0;       /* soft channel ID */                
	        CIPHER_DATA_S CIData = *(CIPHER_DATA_S *)argp;
	        static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CI_MAX_LIST_NUM];
	        HI_UNF_CIPHER_DATA_S *pTmp;
	        HI_U32 pkgNum;
	
	        softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);
	        CIPHER_CheckHandle(softChnId, file);
	
	        pkgNum = CIData.ByteLength;
	        if (pkgNum > CI_MAX_LIST_NUM)
	        {
	            HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\n",pkgNum, CI_MAX_LIST_NUM);
	            Ret = HI_ERR_CIPHER_INVALID_PARA;
	            break;
	        }
	        if (copy_from_user(g_stCipherOsrChn[softChnId].pstDataPkg, (void*)CIData.ScrPhyAddr,
	                           pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)))
	        {
	            HI_ERR_CIPHER("copy data from user fail!\n");
	            Ret = HI_FAILURE;
	            break;
	        }
	       
	        
	        for (i = 0; i < CIData.ByteLength; i++)
	        {
	            pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
	            tmpData[i].bDecrypt = HI_TRUE;
	            tmpData[i].u32src = pTmp->u32SrcPhyAddr;
	            tmpData[i].u32dest = pTmp->u32DestPhyAddr;
	            tmpData[i].u32length = pTmp->u32ByteLength;
	        }
	
	        HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %#x, pkgNum=%d!\n", softChnId, pkgNum);
	
	        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
	
	        up(&g_CipherMutexKernel); /* ??? */
	        Ret = DRV_Cipher_CreatMultiPkgTask(softChnId, tmpData, pkgNum, softChnId);
	        if (HI_SUCCESS != Ret)
	        {
	            return Ret;
	        }
	
	        if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
	                    g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
	        {
	            HI_ERR_CIPHER("Decrypt time out \n");
	            return HI_FAILURE;
	        }
	
	        if(down_interruptible(&g_CipherMutexKernel))
	        {
	            return -ERESTARTSYS;
	        }
	
	        HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);
	        Ret = HI_SUCCESS;
	        break;
	    }
	    case CMD_CIPHER_GETRANDOMNUMBER:
	    {
	        HI_CHIP_TYPE_E enChip;
	        HI_CHIP_VERSION_E enChipVersion;
	        HI_U32 rngBaseAddr = REG_RNG_BASE_ADDR;
	        HI_U32 rngNumberAddr = REG_RNG_NUMBER_ADDR;
	        HI_U32 rngStatAddr = REG_RNG_STAT_ADDR;
	        HI_U32 u32RngStat = 0;
	        HI_U32 u32RngCtrl = 0;
	        HI_U32 u32RngRandomNumber = 0;
	        
	        HI_DRV_SYS_GetChipVersion(&enChip, &enChipVersion);
	        
	        if( ((HI_CHIP_VERSION_V300 == enChipVersion) && (HI_CHIP_TYPE_HI3716M == enChip))
	        	|| ((HI_CHIP_VERSION_V200 == enChipVersion) && (HI_CHIP_TYPE_HI3716CES == enChip)) )
	        {
	            //select the source of the random number
	            u32RngCtrl = (*(volatile HI_U32 *)(IO_ADDRESS(rngBaseAddr)));
	            if ((0x01 == (u32RngCtrl & 0x03)) || (0x0 == (u32RngCtrl & 0x03)))
	            {
	                u32RngCtrl &= 0xfffffffc;
	                u32RngCtrl |= 0x03;
	                (*(volatile HI_U32 *)(IO_ADDRESS(rngBaseAddr))) = u32RngCtrl;
	                msleep(2);
	            }
	            
	            while(1)
	            {
	                u32RngStat = (*(volatile HI_U32 *)(IO_ADDRESS(rngStatAddr)));
	                if((u32RngStat & 0xFF) > 0)
	                {
	                    break;
	                }
	            }
	
	            u32RngRandomNumber = (*(volatile HI_U32 *)(IO_ADDRESS(rngNumberAddr)));
	
	            if (copy_to_user(argp, &u32RngRandomNumber, sizeof(u32RngRandomNumber)))
	            {
	                HI_ERR_CIPHER("copy data to user fail!\n");
	                Ret = HI_FAILURE;
	                break;
	            }
	            
	            Ret = HI_SUCCESS;
	        }
	        else
	        {
	            HI_ERR_CIPHER("Error: Do not support!\n");
	            Ret = HI_FAILURE;
	        }
	        break;
	    }
	    case CMD_CIPHER_GETHANDLECONFIG:
	    {
	        HI_U32 softChnId = 0;       /* soft channel ID */
	        CIPHER_Config_CTRL *pstCIData = (CIPHER_Config_CTRL *)argp;
	
	        softChnId = HI_HANDLE_GET_CHNID(pstCIData->CIHandle);
	        CIPHER_CheckHandle(softChnId, file);
	
	        Ret = DRV_Cipher_GetHandleConfig(softChnId, &pstCIData->CIpstCtrl);
	        if(Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("Get handle configuration failed!\n");
	        }
	        
	        break;
	    }
	
	    case CMD_CIPHER_CALCHASHINIT:
	    {
	        CIPHER_HASH_DATA_S *pCipherHashData = (CIPHER_HASH_DATA_S*)argp;

            pCipherHashData->enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
	        Ret = DRV_Cipher_CalcHashInit(pCipherHashData);
	        if(Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("Init the hash failed!\n");
	        }
	        break;
	    }    
	
	    case CMD_CIPHER_CALCHASHUPDATE:
	    {
	        CIPHER_HASH_DATA_S *pCipherHashData = (CIPHER_HASH_DATA_S*)argp;
	
	        Ret = DRV_Cipher_CalcHashUpdate(pCipherHashData);
	        if(Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("Update the hash failed!\n");
	        }
	        break;
	    }
	
	    case CMD_CIPHER_CALCHASHFINAL:
	    {
	        CIPHER_HASH_DATA_S *pCipherHashData = (CIPHER_HASH_DATA_S*)argp;
	
	        Ret = DRV_Cipher_CalcHashFinal(pCipherHashData);
	        if(Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("Final the hash failed!\n");
	        }
	        break;
	    }
	    case CMD_CIPHER_LOADHDCPKEY:
	    {
	        HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S stFlashEncrytedHdcpKey = *(HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S *)argp;

	        Ret = DRV_Cipher_LoadHdcpKey(stFlashEncrytedHdcpKey);
	        if(Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("cipher load hdcp key failed!\n");
	        }
	        break;
	    }
	
	    default:
	    {
	        Ret = HI_FAILURE;
	        HI_ERR_CIPHER("Error: Inappropriate ioctl for device. cmd=%d\n", cmd);
	        break;
	    }
    }

    up(&g_CipherMutexKernel);
    return Ret;
}

static long DRV_CIPHER_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, CIPHER_Ioctl);

    return Ret;
}

/** <* ref from linux/fs.h @by g00182102 */
static struct file_operations DRV_CIPHER_Fops=
{
    .owner            = THIS_MODULE,
    .open             = DRV_CIPHER_Open,
    .unlocked_ioctl   = DRV_CIPHER_Ioctl,
    .release          = DRV_CIPHER_Release,
};


/*****************************************************************************
 Prototype    : 
 Description  : 
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
static HI_S32  cipher_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    DRV_Cipher_Suspend();
    HI_FATAL_CIPHER("ok !\n");
    return 0;
}

static HI_S32 cipher_pm_resume(PM_BASEDEV_S *pdev)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret= DRV_Cipher_Resume();
    if(s32Ret != HI_SUCCESS)
    {
        HI_FATAL_CIPHER("Cipher resume failed!\n");
        return HI_FAILURE;
    }
    HI_FATAL_CIPHER("ok !\n");

    return HI_SUCCESS;
}

static PM_BASEOPS_S cipher_drvops = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = cipher_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = cipher_pm_resume,
};


/*****************************************************************************
 Prototype    : DRV_CIPHER_ModExit
 Description  : 
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
HI_S32 CIPHER_DRV_ModInit(HI_VOID)
{
    HI_U32 i;
    HI_S32 ret;

    sprintf(g_CipherDevice.devfs_name, UMAP_DEVNAME_CIPHER);
    g_CipherDevice.fops = &DRV_CIPHER_Fops;
    g_CipherDevice.minor = UMAP_MIN_MINOR_CIPHER;
    g_CipherDevice.owner  = THIS_MODULE;
    g_CipherDevice.drvops = &cipher_drvops;

    /* */
    if (HI_DRV_DEV_Register(&g_CipherDevice) < 0)
    {
        HI_FATAL_CIPHER("register CIPHER failed.\n");
        return HI_FAILURE;
    }

    ret = DRV_Cipher_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[i].g_bDataDone = HI_FALSE;
        g_stCipherOsrChn[i].pWichFile = NULL;
        init_waitqueue_head(&(g_stCipherOsrChn[i].cipher_wait_queue));
        g_stCipherOsrChn[i].pstDataPkg = NULL;
    }

#ifdef MODULE
#ifndef CONFIG_SUPPORT_CA_RELEASE
    HI_INFO_CIPHER("Load hi_cipher.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

    return HI_SUCCESS;
}

HI_VOID CIPHER_DRV_ModExit(HI_VOID)
{
    HI_DRV_DEV_UnRegister(&g_CipherDevice);
    DRV_Cipher_DeInit();
    return ;
}

#ifdef MODULE
module_init(CIPHER_DRV_ModInit);
module_exit(CIPHER_DRV_ModExit);
#endif

MODULE_AUTHOR("Hi3720 MPP GRP");
MODULE_LICENSE("GPL");
//MODULE_VERSION(MPP_VERSION);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

