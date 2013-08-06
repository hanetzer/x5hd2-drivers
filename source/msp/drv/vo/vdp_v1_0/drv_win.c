/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_win.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/20
  Description   :
  History       :
  1.Date        : 
  Author        : 
  Modification  : Created file

*******************************************************************************/

#include <linux/vmalloc.h>
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
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>

#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "hi_kernel_adapt.h"

#include "drv_module_ext.h"

#include "hi_drv_disp.h"
#include "drv_disp.h"
#include "drv_display.h"
#include "drv_disp_debug.h"
#include "drv_disp_ext.h"
#include "drv_disp_osal.h"
#include "drv_disp_debug.h"

#include "hi_drv_win.h"
#include "drv_win_ext.h"
#include "drv_win_ioctl.h"
#include "drv_window.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

static atomic_t    g_VoCount = ATOMIC_INIT(0);
//WIN_GLOBAL_STATE_S   g_VoGlobalState;
WIN_STATE_S          g_VoModState;
HI_BOOL     g_VoSuspend = HI_FALSE;

HI_DECLARE_MUTEX(g_VoMutex);

//WIN_PROC_INFO_S s_stProcInfo;
//static HI_U32 s_WinProcId[HI_DRV_DISPLAY_BUTT][WINDOW_MAX_NUMBER]={WINDOW_INVALID_ID};
HI_U8 *g_pWinStateString[6] = {
    "Working",
    "Pause",
    "Resume",
    "FreezeLast",
    "FreezeBlack",
    "UnFreeze",
};

HI_U8 *g_pWinDispModeString[DISP_STEREO_BUTT] = {
    "2D", 
    "FPK", 
    "SBS_HALF",
    "TAB",
    "FILED_ALTE",
    "LINE_ALTE",
    "SBS_FULL", 
    "L_DEPTH",
    "L_DEPTH_G_DEPTH",
};

HI_U8 *g_pWinQucikString[2] = {
    " ",
    "+QuickMode",
};

HI_U8 *g_pWinStepString[2] = {
    " ",
    "+StepMode",
};


HI_U8 *g_pWinFreezeString[2] = {"LAST", "BLACK"};

HI_U8 *g_pWinTpyeString[HI_DRV_WIN_BUTT] = {
    "Display",   
    "Virtual",      
    "MainAndSlave",
    "Slave",
};

HI_U8 *g_pWinAspectCvrsString[HI_DRV_ASP_RAT_MODE_BUTT] = {
    "Full",   
    "LetterBox",
    "PanAndScan",
    "Combined",
    "FullHori",
    "FullVert",
    "Customer"
};

HI_U8 *g_pWinFrameTypetring[HI_DRV_FT_BUTT] = {
    "NotStereo",   
    "SideBySid",      
    "TopAndBottom",     
    "MVC",
};

HI_U8 *g_pWinFieldModeString[HI_DRV_FIELD_BUTT] = {
    "Top",   
    "Bottom",      
    "All",     
};

HI_U8 *g_pWinMemTypeString[3] = {
    "SrcSupply",   
    "WinSupply",      
    "UserSupply",     
};

HI_U8 *g_pWinFieldTypeString[HI_DRV_FIELD_BUTT+1] = {
    "Top",
    "Bottom",
    "Frame",
    "BUTT",
};


HI_S32 WIN_ProcParsePara(HI_CHAR *pProcPara,HI_CHAR **ppItem,HI_CHAR **ppValue)
{
    HI_CHAR *pChar = HI_NULL;
    HI_CHAR *pItem,*pValue;

    pChar = strchr(pProcPara,'=');
    if (HI_NULL == pChar)
    {
        return HI_FAILURE; /* Not Found '=' */
    }

    pItem = pProcPara;
    pValue = pChar + 1;
    *pChar = '\0';

    /* remove blank bytes from item tail */
    pChar = pItem;
    while(*pChar != ' ' && *pChar != '\0')
    {
        pChar++;
    }
    *pChar = '\0';
    
    /* remove blank bytes from value head */
    while(*pValue == ' ')
    {
        pValue++;
    }

    *ppItem = pItem;
    *ppValue = pValue;

    return HI_SUCCESS;
}

HI_VOID WIN_ProcPrintHelp(HI_VOID)
{
    printk("Please input these commands:\n"
           "echo in|height = 1080 > /proc/msp/winxx\n"
           "echo in|width = 1920 > proc/msp/winxx\n"
           "echo in|startx = 100 > proc/msp/winxx\n"
           "echo in|starty = 100 > /proc/msp/winxxx\n"
           "echo RatioType = 0(Full)|1(Source)|2(Customer) > proc/msp/winxx\n"
           "echo CustomRatioW = 1|4|14|16|221|235 >/proc/msp/winxxx\n"
           "echo CustomRatioH = 1|3|9|9|100|100 >/proc/msp/winxxx\n"
           "echo enAspectCvrs= 0(ignore)|1(letterbox)|2(pan&scan)|3(COMBINED) > proc/msp/winxx\n"
           "echo freeze = 0|1,0|1(last,black) > /proc/msp/winxxx\n"
           "echo ratio = 256 > /proc/msp/winxxx\n"
           "echo quickout = 1|0 > /proc/msp/winxxx\n"
           "echo fi = 1|0 > /proc/msp/winxxx\n"
           "echo orgfi = 1|0 > /proc/msp/winxxx\n"

           );
}

HI_S32 DRV_WIN_ProcRead(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *pProcItem;
    WINBUF_STATE_S *pstBuffer;
    HI_DRV_VIDEO_FRAME_S *pstNewFrame;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    WIN_PROC_INFO_S *pstProcInfo;
    HI_HANDLE hWin;
    HI_S32 nRet;
    HI_U32 i;
    HI_U8 *pu8WinSate;

    nRet = down_interruptible(&g_VoMutex);
    pProcItem = p->private;

    hWin = (HI_HANDLE)pProcItem->data;

    pstProcInfo = (WIN_PROC_INFO_S *)DISP_MALLOC(sizeof(WIN_PROC_INFO_S));
    if (!pstProcInfo)
    {
        p += seq_printf(p,"-------- Malloc Proc Buffer Failed!--------\n");
        goto _ERR_EXIT_;
    }

    nRet = WinGetProcInfo(hWin, pstProcInfo);
    if (nRet)
    {
        HI_ERR_WIN("WinGetProcInfo FAILED!\n");
        goto _ERR_EXIT_;
    }

    p += seq_printf(p,"--------Win%04x [Z=%d] Info--------\n", 
                        (HI_U32)(pstProcInfo->u32Index & 0xffffUL), 
                        pstProcInfo->u32Zorder);

    if (pstProcInfo->bReset)
    {
        pu8WinSate = g_pWinFreezeString[(HI_S32)(pstProcInfo->enResetMode)];
    }
    else
    {
        switch(pstProcInfo->u32WinState)
        {
            case 1:
                pu8WinSate = g_pWinStateString[1];
                break;
            case 2:
                pu8WinSate = g_pWinStateString[2];
                break;
            case 3:
                if (pstProcInfo->enFreezeMode == HI_DRV_WIN_SWITCH_LAST)
                {
                    pu8WinSate = g_pWinStateString[3];
                }
                else
                {
                    pu8WinSate = g_pWinStateString[4];
                }
                break;
            case 4:
                pu8WinSate = g_pWinStateString[5];
                break;
            default:
                pu8WinSate = g_pWinStateString[0];
                break;
        }
    }

    
    p += seq_printf(p,
        "Win(En/Mask/State)  :%d/%d/%s%s%s\n"
        "SrcHandle           :%x\n"
        "SrcCB(A/R/W)        :%x/%x/%x\n"
        "WinType             :%s\n"
        "Disp/Layer          :%d/%d\n"
        "ARConvert(WvsH)     :%s(%dvs%d)\n"
        "CropEnable          :%d\n"
        "In  (X/Y/W/H)       :%d/%d/%d/%d\n"
        "Crop(L/T/R/B)       :%d/%d/%d/%d\n"
        "Out (X/Y/W/H)       :%d/%d/%d/%d\n"
        "Vir(Mem/Num/Fmt)    :%d/%d/%d\n"
        "SlaveHandle         :%x\n"
        "DispMode/RightFirst :%s/%d\n"
        "TBNotMatch          :%d\n"
        "DebugEn             :%d\n",        
        pstProcInfo->bEnable,
        pstProcInfo->bMasked,
        pu8WinSate,
        g_pWinQucikString[(HI_S32)pstProcInfo->bQuickMode],
        g_pWinStepString[(HI_S32)pstProcInfo->bStepMode],
        /* source info */
        pstProcInfo->hSrc,
        pstProcInfo->pfAcqFrame,
        pstProcInfo->pfRlsFrame,
        pstProcInfo->pfSendWinInfo,

        /* attribute */
        //HI_DRV_WIN_ATTR_S stAttr;
        g_pWinTpyeString[pstProcInfo->enType],
        pstProcInfo->stAttr.enDisp,
        pstProcInfo->u32LayerId,

        /* may change when window lives */
        g_pWinAspectCvrsString[pstProcInfo->stAttr.enARCvrs],
        (HI_U32)pstProcInfo->stAttr.stCustmAR.u8ARw,
        (HI_U32)pstProcInfo->stAttr.stCustmAR.u8ARh,

        pstProcInfo->stAttr.bUseCropRect,
        pstProcInfo->stAttr.stInRect.s32X,
        pstProcInfo->stAttr.stInRect.s32Y,
        pstProcInfo->stAttr.stInRect.s32Width,
        pstProcInfo->stAttr.stInRect.s32Height,

        pstProcInfo->stAttr.stCropRect.u32LeftOffset,
        pstProcInfo->stAttr.stCropRect.u32TopOffset,
        pstProcInfo->stAttr.stCropRect.u32RightOffset,
        pstProcInfo->stAttr.stCropRect.u32BottomOffset,

        pstProcInfo->stAttr.stOutRect.s32X,
        pstProcInfo->stAttr.stOutRect.s32Y,
        pstProcInfo->stAttr.stOutRect.s32Width,
        pstProcInfo->stAttr.stOutRect.s32Height,

        /* only for virtual window */
        pstProcInfo->stAttr.bUserAllocBuffer,
        pstProcInfo->stAttr.u32BufNumber,
        pstProcInfo->stAttr.enDataFormat,

        pstProcInfo->hSlvWin,

        g_pWinDispModeString[pstProcInfo->eDispMode],
        pstProcInfo->bRightEyeFirst,
        pstProcInfo->u32TBNotMatchCount,
        pstProcInfo->bDebugEn
        );

    pstBuffer   = &pstProcInfo->stBufState;
    pstNewFrame = &pstProcInfo->stBufState.stCurrentFrame;;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&pstNewFrame->u32Priv[0];

    p += seq_printf(p,
        "--------Frame Info--------\n"
        "Type/PixFmt         :%s/%d\n"
        "Circurotate         :%d\n"
        "W/H(WvsH)           :%d/%d(%dvs%d)\n"
        "Disp(X/Y/W/H)       :%d/%d/%d/%d\n"
        "FrameRate           :%d\n"
        "ColorSpace          :%d\n"
        "Field(Ori/New)      :%s/%s\n"
        "OriRect(X/Y/W/H)    :%d/%d/%d/%d\n"
        "FrameIndex          :0x%x\n"
        "SrcPTS/PTS          :0x%x/0x%x\n"
        "PlayTime            :%d\n"
        "FieldMode           :%s\n"
        "Y/CAddr             :0x%x/0x%x\n"
        "Y/CStride           :0x%x/0x%x\n",

        /* frame information */
        g_pWinFrameTypetring[pstNewFrame->eFrmType],
        pstNewFrame->ePixFormat,
        pstNewFrame->u32Circumrotate,

        pstNewFrame->u32Width,
        pstNewFrame->u32Height,
        (HI_U32)pstNewFrame->u32AspectWidth,
        (HI_U32)pstNewFrame->u32AspectHeight,

        pstNewFrame->stDispRect.s32X,
        pstNewFrame->stDispRect.s32Y,
        pstNewFrame->stDispRect.s32Width,
        pstNewFrame->stDispRect.s32Height,

        pstNewFrame->u32FrameRate,
        pstPriv->eColorSpace,
        g_pWinFieldTypeString[pstPriv->eOriginField],
        g_pWinFieldTypeString[pstNewFrame->enFieldMode],
        pstPriv->stOriginImageRect.s32X,
        pstPriv->stOriginImageRect.s32Y,
        pstPriv->stOriginImageRect.s32Width,
        pstPriv->stOriginImageRect.s32Height,        

        /* these member may be changed per frame */
        pstNewFrame->u32FrameIndex,
        pstNewFrame->u32SrcPts,
        pstNewFrame->u32Pts,

        pstPriv->u32PlayTime,
        g_pWinFieldModeString[pstNewFrame->enFieldMode],

        /* stBufAddr[1] is right eye for stereo video */
        pstNewFrame->stBufAddr[0].u32PhyAddr_Y,  
        pstNewFrame->stBufAddr[0].u32PhyAddr_C,  
        pstNewFrame->stBufAddr[0].u32Stride_Y,
        pstNewFrame->stBufAddr[0].u32Stride_C
        );


    p += seq_printf(p,
        "--------Buffer State(%d)--------\n"
        "QFrame(Try/OK)      :%d/%d\n"
        "Cfg/Release         :%d/%d\n"
        "Underload           :%d\n"
        "Discard             :%d\n"
        "DiscFrame(I/O)      :%d/%d\n",
        pstBuffer->u32Number,
        pstBuffer->stRecord.u32TryQueueFrame,
        pstBuffer->stRecord.u32QueueFrame,
        pstBuffer->stRecord.u32Config,
        pstBuffer->stRecord.u32Release,
        pstBuffer->stRecord.u32Underload,
        pstBuffer->stRecord.u32Disacard,    
        pstProcInfo->u32ULSIn,
        pstProcInfo->u32ULSOut
        );

    p += seq_printf(p,
        "Empty(Read/Write)   :%d/%d\n",
        pstBuffer->u32EmptyRPtr,
        pstBuffer->u32EmptyWPtr
        );
    for (i=0; i<pstBuffer->u32Number;)
    {
        p += seq_printf(p,"0x%08x", 
            pstBuffer->u32EmptyArray[i]);
        if (i == pstBuffer->u32EmptyRPtr)
        {
            p += seq_printf(p,"(R) ");
        }
        else if (i == pstBuffer->u32EmptyWPtr)
        {
            p += seq_printf(p,"(W) ");
        }
        else
        {
            p += seq_printf(p,"    ");
        }

        i++;
        if( (i%8) == 0)
        {
            p += seq_printf(p, "\n");
        }
    }

    p += seq_printf(p,
        "Full(Read/Write)   :%d/%d\n",
        pstBuffer->u32FullRPtr,
        pstBuffer->u32FullWPtr);
    
    for (i=0; i<pstBuffer->u32Number;)
    {
        p += seq_printf(p,"0x%08x", 
            pstBuffer->u32FullArray[i]);
        if (i == pstBuffer->u32FullRPtr)
        {
            p += seq_printf(p,"(R) "); 
        }
        else if (i == pstBuffer->u32FullWPtr)
        {
            p += seq_printf(p,"(W) ");
        }
        else
        {
            p += seq_printf(p,"    ");        
        }

        i++;
        if( (i%8) == 0)
        {
            p += seq_printf(p, "\n");
        }
    }

    p += seq_printf(p, "BQState[state, WriteIn/Readout, FrameID]:\n");
    p += seq_printf(p, "(State: 0,Idle; 1,Empty; 2,Write; 3,Full; 4,Read)\n");
    for (i=0; i<pstBuffer->u32Number;)
    {
        p += seq_printf(p,"[%d,%d/%d,0x%x] ", 
            pstBuffer->stNode[i].u32State,
            pstBuffer->stNode[i].u32Empty,
            pstBuffer->stNode[i].u32Full,
            pstBuffer->stNode[i].u32FrameIndex
            );

        i++;
        if( (i%4) == 0)
        {
            p += seq_printf(p, "\n");
        }
    }

    p += seq_printf(p, "\n");

    DISP_FREE(pstProcInfo);

_ERR_EXIT_:
    up(&g_VoMutex);
    return nRet;
}



HI_VOID WIN_ProcWriteHelp(HI_VOID)
{
    printk("=========== Win Proc Help ==============\n");
    printk("USAGE:echo cmd [para] > /proc/msp/winXXXX\n");
    printk("cmd = 0x%x, set window enable\n", DRV_WIN_PROC_CMD_EN);
    printk("cmd = 0x%x, set window disable\n", DRV_WIN_PROC_CMD_DIS);
    printk("cmd = 0x%x, set window freeze still frame\n", DRV_WIN_PROC_CMD_FRZ_S);
    printk("cmd = 0x%x, set window unfreeze\n", DRV_WIN_PROC_CMD_UNFRZ);
    printk("cmd = 0x%x, set window pause\n", DRV_WIN_PROC_CMD_PAUSE);
    printk("cmd = 0x%x, set window unpause\n", DRV_WIN_PROC_CMD_RESUME);

    printk("echo 0x%x path > /proc/msp/winXXXX, save current frame into file at path\n", DRV_WIN_PROC_CMD_SV_YUV);
}

char buffer[256], str[256];
HI_S32 DRV_WIN_ProcWrite(struct file * file,
                   const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file *p = file->private_data;
    DRV_PROC_ITEM_S *pProcItem = p->private;
    HI_HANDLE hWin;
    int i,j;
    unsigned int dat1;
    HI_S32 nRet;

    hWin = (HI_HANDLE)(pProcItem->data);

    if(count >= sizeof(buffer)) 
    {
        HI_ERR_WIN("your parameter string is too long!\n");
        return -EFAULT;
    }

    nRet = down_interruptible(&g_VoMutex);

    memset(buffer, 0, sizeof(buffer));
    if (copy_from_user(buffer, buf, count))
    {
        HI_ERR_WIN("MMZ: copy_from_user failed!\n");    
        return -EFAULT;
    }
    buffer[count] = 0;

    /* dat1 */
    i = 0;
    j = 0;
    for(; i < count; i++)
    {
        if(j==0 && buffer[i]==' ')continue;
        if(buffer[i] > ' ')str[j++] = buffer[i];
        if(j>0 && buffer[i]==' ')break;
    }
    str[j] = 0;

    if(vdp_str2val(str, &dat1) != 0)
    {
        HI_ERR_WIN("error echo cmd '%s'!\n", buffer);
        return count;
    }

    switch (dat1)
    {
        case DRV_WIN_PROC_CMD_EN:
            nRet = WIN_SetEnable(hWin, HI_TRUE);
            break;
        case DRV_WIN_PROC_CMD_DIS:
            nRet = WIN_SetEnable(hWin, HI_FALSE);
            break;
        case DRV_WIN_PROC_CMD_PAUSE:
            nRet = WIN_Pause(hWin, HI_TRUE);
            break;
        case DRV_WIN_PROC_CMD_RESUME:
            nRet = WIN_Pause(hWin, HI_FALSE);
            break;
        case DRV_WIN_PROC_CMD_FRZ_S:
            nRet = WIN_Freeze(hWin, HI_TRUE, HI_DRV_WIN_SWITCH_LAST);
            break;
        case DRV_WIN_PROC_CMD_UNFRZ:
            nRet = WIN_Freeze(hWin, HI_FALSE, HI_DRV_WIN_SWITCH_LAST);
            break;
        case DRV_WIN_PROC_CMD_SV_YUV:
            nRet = vdp_WinSaveDispImg(hWin, &buffer[i], (HI_U32)(count-i));
            break;
            
        case DRV_WIN_PROC_CMD_VALID:
        default:
            WIN_ProcWriteHelp();
            nRet = HI_SUCCESS;
            break;
    
    }

    up(&g_VoMutex);

    if (nRet != HI_SUCCESS)
    {
        HI_ERR_WIN("Win proc cmd = 0x%x failed, and ret=0x%x\n",
                    dat1, nRet);
    }

    return count;
}



/***************************************************************/
HI_S32 WIN_AddToProc(HI_HANDLE hWindow)
{
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR           ProcName[12];
    HI_U32 u32Index;
    HI_S32 Ret;

    Ret = WinGetProcIndex(hWindow, &u32Index);
    if (Ret)
    {
        HI_ERR_WIN("WinGetProcInfo failed!\n");
        return HI_ERR_VO_ADD_PROC_ERR;
    }

    //printk("WIN_AddToProc win index=0x%x\n", u32Index);
    sprintf(ProcName, "win%04x", (HI_U32)(u32Index & WINDOW_INDEX_MASK));

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_ERR_WIN("Window add proc failed!\n");
        HI_ERR_WIN("WinGetProcInfo failed!\n");
        return HI_ERR_VO_ADD_PROC_ERR;
    }

    //s_WinProcId[WinGetDispId(u32Index)][WinGetId(u32Index)] = u32Index;

    pProcItem->data  = (HI_VOID *)hWindow;
    pProcItem->read  = DRV_WIN_ProcRead;
    pProcItem->write = DRV_WIN_ProcWrite;

    return HI_SUCCESS;
}

HI_S32 WIN_RemFromProc(HI_HANDLE hWindow)
{
    HI_CHAR           ProcName[12];
    HI_U32 u32Index;
    HI_S32 Ret;

    Ret = WinGetProcIndex(hWindow, &u32Index);
    //printk("WIN_RemFromProc win index=0x%x\n", u32Index);
    if (!Ret)
    {
        sprintf(ProcName, "win%04x", (HI_U32)(u32Index & WINDOW_INDEX_MASK));
        HI_DRV_PROC_RemoveModule(ProcName);

        //s_WinProcId[WinGetDispId(u32Index)][WinGetId(u32Index)] = WINDOW_INVALID_ID;
    }
    else
    {
        HI_FATAL_WIN("WinGetProcInfo failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_AddToState(WIN_STATE_S *pst2WinState, HI_DRV_DISPLAY_E enDisp, HI_HANDLE hWin)
{
    HI_S32 i;

    for(i=0; i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
    {
        if (pst2WinState->hWin[enDisp][i] == HI_NULL)
        {
            pst2WinState->hWin[enDisp][i] = hWin;
            return HI_SUCCESS;
        }
    }

    return HI_SUCCESS;    
}

HI_S32 WIN_RemFromState(WIN_STATE_S *pst2WinState, HI_HANDLE hWin)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 i;

    for(enDisp=0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
    {
        for(i=0; i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
        {
            if (pst2WinState->hWin[enDisp][i] == hWin)
            {
                pst2WinState->hWin[enDisp][i] = HI_NULL;
                return HI_SUCCESS;
            }
        }
    }
    
    return HI_SUCCESS;    
}


HI_S32 WIN_CreateExt(WIN_CREATE_S *pVoWinCreate, WIN_STATE_S *pst2WinState)
{   
    HI_DRV_WIN_INFO_S stWinInfo;
    HI_S32 Ret;

//printk("============WIN_CreateExt======= 001\n");
    Ret = WIN_Create(&pVoWinCreate->WinAttr, &pVoWinCreate->hWindow);
    if (Ret)
    {
        goto __ERR_EXIT__;
    }

    Ret = WIN_AddToProc(pVoWinCreate->hWindow);
    if (Ret)
    {
        goto __ERR_EXIT_DESTROY__;
    }
//printk("============WIN_CreateExt======= 002\n");

    if (HI_SUCCESS == WIN_GetInfo(pVoWinCreate->hWindow, &stWinInfo))
    {
        if (stWinInfo.hSec)
        {
//printk("============WIN_CreateExt======= 003\n");

            Ret = WIN_AddToProc(stWinInfo.hSec);
            if (Ret)
            {
                goto __ERR_EXIT_REM_PORC__;
            }        
        }
    }

    WIN_AddToState(pst2WinState, pVoWinCreate->WinAttr.enDisp, pVoWinCreate->hWindow);

    return HI_SUCCESS;

__ERR_EXIT_REM_PORC__:
    WIN_RemFromProc(pVoWinCreate->hWindow);
__ERR_EXIT_DESTROY__:
    WIN_Destroy(pVoWinCreate->hWindow);
__ERR_EXIT__:
    return Ret;
}

HI_S32 WIN_DestroyExt(HI_HANDLE hWindow, WIN_STATE_S *pstWinState)
{
    HI_DRV_WIN_INFO_S stWinInfo;
    HI_S32 Ret;

    if (HI_SUCCESS == WIN_GetInfo(hWindow, &stWinInfo))
    {
        if (stWinInfo.hSec)
        {
//printk("============WIN_DestroyExt======= 001\n");

            Ret = WIN_RemFromProc(stWinInfo.hSec);
            if (Ret)
            {
                HI_ERR_WIN("WIN_RemFromProc slave failed!\n");
            }        
        }
    }

//printk("============WIN_DestroyExt======= 002\n");

    Ret = WIN_RemFromProc(hWindow);;
    if (Ret)
    {
        HI_ERR_WIN("WIN_RemFromProc failed!\n");;
    }

    Ret = WIN_Destroy(hWindow);
    if (Ret != HI_SUCCESS)
    {
        HI_FATAL_WIN("call WIN_Destroy failed.\n");
    }

    WIN_RemFromState(pstWinState, hWindow);

    return Ret;
}



HI_S32 WIN_CheckHanlde(HI_HANDLE hWindow, WIN_STATE_S *pstWinState)
{    

    return HI_SUCCESS;
}

HI_S32 WIN_ProcessCmd(unsigned int cmd, HI_VOID *arg, WIN_STATE_S *pstWinState)
{
    HI_S32 Ret = HI_SUCCESS;
    
    switch (cmd)
    {
        case CMD_WIN_CREATE:
        {
            WIN_CREATE_S  *pVoWinCreate;

            pVoWinCreate = (WIN_CREATE_S *)arg;

            Ret = WIN_CreateExt(pVoWinCreate, pstWinState);

            break;
        }

        case CMD_WIN_DESTROY:
        {
            Ret = WIN_CheckHanlde(*((HI_HANDLE *)arg), pstWinState);
            
            if (HI_SUCCESS == Ret)
            {               
                Ret = WIN_DestroyExt(*((HI_HANDLE *)arg), pstWinState);                
            }

            break;
        }

        case CMD_WIN_SET_ENABLE:
        {
            WIN_ENABLE_S   *pWinEnable;

            pWinEnable = (WIN_ENABLE_S *)arg;

            Ret = WIN_CheckHanlde(pWinEnable->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetEnable(pWinEnable->hWindow, pWinEnable->bEnable);
            }

            break;
        }

        case CMD_WIN_GET_ENABLE:
        {
            WIN_ENABLE_S   *pVoWinEnable;

            pVoWinEnable = (WIN_ENABLE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinEnable->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetEnable(pVoWinEnable->hWindow, &pVoWinEnable->bEnable);
            }

            break;
        }

        case CMD_WIN_VIR_ACQUIRE:
        {
            WIN_FRAME_S   *pVoWinFrame;
            pVoWinFrame = (WIN_FRAME_S*)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_AcquireFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_VIR_RELEASE:
        {
            WIN_FRAME_S   *pVoWinFrame;
            pVoWinFrame = (WIN_FRAME_S*)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_ReleaseFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_SET_ATTR:
        {
            WIN_CREATE_S *pVoWinAttr;

            pVoWinAttr = (WIN_CREATE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetAttr(pVoWinAttr->hWindow, &pVoWinAttr->WinAttr);
            }

            break;
        }

        case CMD_WIN_GET_ATTR:
        {
            WIN_CREATE_S   *pVoWinAttr;

            pVoWinAttr = (WIN_CREATE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetAttr(pVoWinAttr->hWindow, &pVoWinAttr->WinAttr);
            }

            break;
        }

        case CMD_WIN_SET_ZORDER:
        {
            WIN_ZORDER_S *pVoWinZorder;

            pVoWinZorder = (WIN_ZORDER_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinZorder->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetZorder(pVoWinZorder->hWindow, pVoWinZorder->eZFlag);
            }

            break;
        }

        case CMD_WIN_GET_ORDER:
        {
            WIN_ORDER_S *pVoWinOrder;

            pVoWinOrder = (WIN_ORDER_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinOrder->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetZorder(pVoWinOrder->hWindow, &pVoWinOrder->Order);
            }

            break;
        }

        case CMD_WIN_SET_SOURCE:
        {
            WIN_SOURCE_S *pVoWinAttach;

            pVoWinAttach = (WIN_SOURCE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinAttach->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetSource(pVoWinAttach->hWindow, &pVoWinAttach->stSrc);
            }

            break;
        }

        case CMD_WIN_FREEZE:
        {
            WIN_FREEZE_S  *pVoWinFreeze;

            pVoWinFreeze = (WIN_FREEZE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFreeze->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Freeze(pVoWinFreeze->hWindow, pVoWinFreeze->bEnable, pVoWinFreeze->eMode);
            }

            break;
        }


        case CMD_WIN_SEND_FRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SendFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_QU_FRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
    	        Ret = WIN_QueueFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }
            break;
        }

        case CMD_WIN_QU_ULSFRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
	            Ret = WIN_QueueUselessFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }
            break;
        }

        case CMD_WIN_DQ_FRAME:
        {
            WIN_FRAME_S	 *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_DequeueFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_RESET:
        {
            WIN_RESET_S  *pVoWinReset;

            pVoWinReset = (WIN_RESET_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinReset->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Reset(pVoWinReset->hWindow, pVoWinReset->eMode);
            }

            break;
        }

        case CMD_WIN_PAUSE:
        {
            WIN_PAUSE_S  *pVoWinPause;

            pVoWinPause = (WIN_PAUSE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinPause->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Pause(pVoWinPause->hWindow, pVoWinPause->bEnable);
            }

            break;
        }

        case CMD_WIN_GET_PLAY_INFO:
        {
            WIN_PLAY_INFO_S  *pVoWinDelay;

            pVoWinDelay = (WIN_PLAY_INFO_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinDelay->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetPlayInfo(pVoWinDelay->hWindow, &pVoWinDelay->stPlayInfo);
            }

            break;
        }

        case CMD_WIN_GET_INFO:
        {
            WIN_PRIV_INFO_S  *pVoWinDelay;

            pVoWinDelay = (WIN_PRIV_INFO_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinDelay->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetInfo(pVoWinDelay->hWindow, &pVoWinDelay->stPrivInfo);
            }

            break;
        }


        case CMD_WIN_STEP_MODE:
        {
            WIN_STEP_MODE_S  *pVoWinStepMode;

            pVoWinStepMode = (WIN_STEP_MODE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinStepMode->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetStepMode(pVoWinStepMode->hWindow, pVoWinStepMode->bStep);
            }

            break;
        }

        case CMD_WIN_STEP_PLAY:
        {
            Ret = WIN_CheckHanlde(*((HI_HANDLE *)arg), pstWinState);
            
            if (HI_SUCCESS == Ret)
            {               
                Ret = WIN_SetStepPlay(*((HI_HANDLE *)arg));
            }

        break;
        }

        case CMD_WIN_VIR_EXTERNBUF:
        {
            WIN_BUF_POOL_S*      winBufAttr = (WIN_BUF_POOL_S*)arg;
            
            Ret = WIN_CheckHanlde(winBufAttr->hwin, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetExtBuffer(winBufAttr->hwin, &winBufAttr->stBufPool);
            }
            break;
        }
        
        case CMD_WIN_SET_QUICK:
        {
            WIN_SET_QUICK_S * stQuickOutputAttr = (WIN_SET_QUICK_S*)arg;
            Ret = WIN_CheckHanlde(stQuickOutputAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetQuick(stQuickOutputAttr->hWindow, stQuickOutputAttr->bQuickEnable);
            }

            break;
        }
        
        case CMD_WIN_SUSPEND:
        {
            Ret = WIN_Suspend();
            break;
        }

        case CMD_WIN_RESUM:
        {
            Ret = WIN_Resume();
            break;
        }

        case CMD_WIN_GET_HANDLE:
        {
            WIN_HANDLE_ARRAY_S stWinArray;
            WIN_GET_HANDLE_S *pWin = (WIN_GET_HANDLE_S*)arg;
            
            Ret = Win_DebugGetHandle(pWin->enDisp, &stWinArray);
            if (Ret == HI_SUCCESS)
            {
                HI_S32 i;
                
                pWin->u32WinNumber = stWinArray.u32WinNumber;

                for (i=0; (i<pWin->u32WinNumber) && (i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP); i++)
                {
                    pWin->ahWinHandle[i] = stWinArray.ahWinHandle[i];
                }
            }
            break;
        }

        default:
        up(&g_VoMutex);
        return -ENOIOCTLCMD;
    }   
    
    return Ret;
}

extern HI_U32 DISP_Get_CountStatus(HI_VOID);

HI_S32 DRV_WIN_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32  Ret;

    Ret = down_trylock(&g_VoMutex);
    if (Ret)
    {
        HI_FATAL_WIN("down g_VoMutex failed.\n");
        return -1;
    }

    /* just return if no task opened the vo device.*/
    /*CNcomment: 没有进程打开过VO设备，直接返回 */
#if 0
    if( !atomic_read(&g_VoCount) )
    {
        up(&g_VoMutex);
        return 0;
    }
#endif

    g_VoSuspend = HI_TRUE;
    WIN_Suspend();
    
    msleep(50);

    HI_FATAL_WIN("VO suspend OK.\n");

    up(&g_VoMutex);
    return 0;
}

HI_S32 DRV_WIN_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32  Ret;

    Ret = down_trylock(&g_VoMutex);
    if (Ret)
    {
        HI_FATAL_WIN("down g_VoMutex failed.\n");
        return -1;
    }

#if 0
    if(!atomic_read(&g_VoCount)) 
    {
        up(&g_VoMutex);
        return 0;
    }
#endif

    WIN_Resume();
    g_VoSuspend = HI_FALSE;

    HI_FATAL_WIN("VO resume OK.\n");

    up(&g_VoMutex);
    return 0;
}

HI_S32 DRV_WIN_Process(HI_U32 cmd, HI_VOID *arg)
{
    WIN_STATE_S   *pstWinState;    
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    pstWinState = &g_VoModState;

    Ret = WIN_ProcessCmd(cmd, arg, pstWinState);

    up(&g_VoMutex);
    return Ret;
}




/*add for mce interface*/
HI_S32  DRV_WIN_Create(const HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow)
{
    HI_S32 Ret; 
    WIN_CREATE_S voWinCreate;
    
    memcpy(&voWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));
    Ret = DRV_WIN_Process(CMD_WIN_CREATE, &voWinCreate);
    *phWindow = voWinCreate.hWindow;
    return Ret;
}

HI_S32  DRV_WIN_Destroy(HI_HANDLE hWindow)
{
    HI_S32 Ret; 

    Ret = DRV_WIN_Process(CMD_WIN_DESTROY, &hWindow);
    return Ret;
}

HI_S32  DRV_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32 Ret; 
    WIN_ENABLE_S   enVoWinEnable;

    enVoWinEnable.bEnable = bEnable;
    enVoWinEnable.hWindow = hWindow;
    
    Ret = DRV_WIN_Process(CMD_WIN_SET_ENABLE, &enVoWinEnable);
    return Ret;
}


HI_S32  DRV_WIN_SetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_S32 Ret; 
    WIN_SOURCE_S VoWinAttach;

    VoWinAttach.hWindow = hWindow;
    VoWinAttach.stSrc   = *pstSrc;


    Ret = DRV_WIN_Process(CMD_WIN_SET_SOURCE, &VoWinAttach);
    return Ret;
}


HI_S32 DRV_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enSwitch)
{
    HI_S32 Ret; 
    WIN_RESET_S   VoWinReset;

    VoWinReset.hWindow = hWindow;
    VoWinReset.eMode   = enSwitch;
    Ret = DRV_WIN_Process(CMD_WIN_RESET, &VoWinReset);
    return Ret;
}


HI_S32 DRV_WIN_GetPlayInfo(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pInfo)
{    
    HI_S32 Ret; 
    WIN_PLAY_INFO_S WinPlayInfo;

    WinPlayInfo.hWindow = hWindow;
    
    Ret = DRV_WIN_Process(CMD_WIN_GET_PLAY_INFO, &WinPlayInfo);
    *pInfo = WinPlayInfo.stPlayInfo;
    return Ret;
}

HI_S32 DRV_WIN_GetInfo(HI_HANDLE hWindow, HI_DRV_WIN_INFO_S *pInfo)
{    
    HI_S32 Ret; 
    WIN_PRIV_INFO_S WinPrivInfo;

    WinPrivInfo.hWindow = hWindow;
    
    Ret = DRV_WIN_Process(CMD_WIN_GET_INFO, &WinPrivInfo);
    *pInfo = WinPrivInfo.stPrivInfo;
	
    return Ret;
}

HI_S32 DRV_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;
    stWinFrame.stFrame = *pFrame;
    
    Ret = DRV_WIN_Process(CMD_WIN_SEND_FRAME, &stWinFrame);

    return Ret;
}


HI_S32 DRV_WIN_QFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;
    stWinFrame.stFrame = *pFrame;
    
    Ret = DRV_WIN_Process(CMD_WIN_QU_FRAME, &stWinFrame);

    return Ret;
}

HI_S32 DRV_WIN_QULSFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;
    stWinFrame.stFrame = *pFrame;
    
    Ret = DRV_WIN_Process(CMD_WIN_QU_ULSFRAME, &stWinFrame);

    return Ret;
}

HI_S32 DRV_WIN_DQFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;

    
    Ret = DRV_WIN_Process(CMD_WIN_DQ_FRAME, &stWinFrame);
	if (Ret)
	{
    	stWinFrame.stFrame = *pFrame;
	}

    return Ret;
}

HI_S32 DRV_WIN_AcquireFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;
    stWinFrame.stFrame = *pFrame;
    
    Ret = DRV_WIN_Process(CMD_WIN_VIR_ACQUIRE, &stWinFrame);

    return Ret;
}

HI_S32 DRV_WIN_ReleaseFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{    
    HI_S32     Ret;
    WIN_FRAME_S stWinFrame;

    stWinFrame.hWindow = hWindow;

    
    Ret = DRV_WIN_Process(CMD_WIN_VIR_RELEASE, &stWinFrame);
    if (Ret)
    {
        stWinFrame.stFrame = *pFrame;
    }

    return Ret;
}




HI_S32 DRV_WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E ZFlag)
{
//    HI_S32     Ret;

    return HI_SUCCESS;
}

HI_S32 WIN_DRV_DestroyAll(WIN_STATE_S *pst2WinState)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 i;

    for(enDisp=0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
    {
        for(i=0; i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
        {
            if (pst2WinState->hWin[enDisp][i] != HI_NULL)
            {
                WIN_DestroyExt(pst2WinState->hWin[enDisp][i], pst2WinState);
                return HI_SUCCESS;
            }
        }
    }
    
    return HI_SUCCESS;    
}



HI_S32 DRV_WIN_Init(HI_VOID)
{
    HI_S32 i, j, Ret;

    Ret = down_interruptible(&g_VoMutex);

    for (i=HI_DRV_DISPLAY_0; i<HI_DRV_DISPLAY_BUTT; i++)
    {
        for (j=0; j<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; j++)
        {
            g_VoModState.hWin[i][j] = HI_NULL;
        }
    }    

    if (1 == atomic_inc_return(&g_VoCount))
    {
        Ret = WIN_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_WIN("call VO_Init failed.\n");
            atomic_dec(&g_VoCount);
            up(&g_VoMutex);
            return -1;
        }
    }

    up(&g_VoMutex);
    return 0;
}



HI_S32 DRV_WIN_DeInit(HI_VOID)
{
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    WIN_DRV_DestroyAll(&g_VoModState);

    if (atomic_dec_and_test(&g_VoCount))
    {       
        WIN_DeInit();
    }

    up(&g_VoMutex);
    return 0;
}

static WIN_EXPORT_FUNC_S s_stWinExportFuncs = {
    .FN_GetPlayInfo   = DRV_WIN_GetPlayInfo,
    .FN_GetInfo       = DRV_WIN_GetInfo,
    .FN_QueueFrame    = DRV_WIN_QFrame,
    .FN_QueueULSFrame = DRV_WIN_QULSFrame,
    .FN_DequeueFrame = DRV_WIN_DQFrame,
    .FN_ReleaseFrame = HI_NULL,
    .FN_AcquireFrame = HI_NULL,
};

HI_S32 DRV_WIN_Register(HI_VOID)
{
    HI_S32  Ret;

    Ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_VO, "HI_VO", (HI_VOID *)(&s_stWinExportFuncs)); 
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_WIN("HI_DRV_MODULE_Register VO failed\n");
        return Ret;
    }

    g_VoSuspend = HI_FALSE;

    return  0;
}

HI_VOID DRV_WIN_UnRegister(HI_VOID)
{

    DRV_WIN_DeInit();    

    HI_DRV_MODULE_UnRegister(HI_ID_VO);

    return;
}



HI_S32 WIN_DRV_Open(struct inode *finode, struct file  *ffile)
{
    WIN_STATE_S *pWinState = HI_NULL;
    HI_S32     Ret;

    Ret = down_interruptible(&g_VoMutex);

    pWinState = HI_KMALLOC(HI_ID_VO, sizeof(WIN_STATE_S), GFP_KERNEL);
    if (!pWinState)
    {
        WIN_FATAL("malloc pWinState failed.\n");
        up(&g_VoMutex);
        return -1;
    }

    memset(pWinState, 0, sizeof(WIN_STATE_S));

    if (1 == atomic_inc_return(&g_VoCount))
    {
        Ret = WIN_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_KFREE(HI_ID_VO, pWinState); 
            WIN_FATAL("call VO_Init failed.\n");
            atomic_dec(&g_VoCount);
            up(&g_VoMutex);
            return -1;
        }
    }

    ffile->private_data = pWinState;

    up(&g_VoMutex);
    return 0;
}



HI_S32 WIN_DRV_Close(struct inode *finode, struct file  *ffile)
{
    WIN_STATE_S    *pVoState;
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    pVoState = ffile->private_data;

    WIN_DRV_DestroyAll(pVoState);

    if (atomic_dec_and_test(&g_VoCount))
    {   
//printk("################################### close window now\n");
        WIN_DeInit();
    }

    HI_KFREE(HI_ID_VO, pVoState);

    up(&g_VoMutex);

    return 0;
}

HI_S32 DRV_WIN_Ioctl(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg)
{
    WIN_STATE_S   *pstWinState;    
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    pstWinState = file->private_data;

    //printk("zzzzzzzzzzzzzzz WIN cmd = 0x%x\n", cmd);
    Ret = WIN_ProcessCmd(cmd, arg, pstWinState);

    up(&g_VoMutex);
    return Ret;
}

#ifndef MODULE
EXPORT_SYMBOL(g_VoMutex);

EXPORT_SYMBOL(DRV_WIN_Ioctl);
EXPORT_SYMBOL(WIN_DRV_Open);
EXPORT_SYMBOL(WIN_DRV_Close);
EXPORT_SYMBOL(DRV_WIN_Suspend);
EXPORT_SYMBOL(DRV_WIN_Resume);
//EXPORT_SYMBOL(DRV_WIN_ProcRegister);
//EXPORT_SYMBOL(DRV_WIN_ProcfUnRegister);
EXPORT_SYMBOL(WIN_AcquireFrame);
EXPORT_SYMBOL(WIN_ReleaseFrame);

EXPORT_SYMBOL(DRV_WIN_SetEnable);
EXPORT_SYMBOL(DRV_WIN_Register);
EXPORT_SYMBOL(DRV_WIN_UnRegister);

EXPORT_SYMBOL(DRV_WIN_Process);
EXPORT_SYMBOL(DRV_WIN_Create);
EXPORT_SYMBOL(DRV_WIN_Destroy);
//EXPORT_SYMBOL(DRV_WIN_SetEnable);
EXPORT_SYMBOL(DRV_WIN_SetSource);
EXPORT_SYMBOL(DRV_WIN_Reset);
EXPORT_SYMBOL(DRV_WIN_GetPlayInfo);
EXPORT_SYMBOL(DRV_WIN_SendFrame);
EXPORT_SYMBOL(DRV_WIN_SetZorder);
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


