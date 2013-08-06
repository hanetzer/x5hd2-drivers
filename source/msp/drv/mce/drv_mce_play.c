#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/relay.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/splice.h>
#include <asm/io.h>
#include <mach/platform.h>
#include <mach/hardware.h>
#include <asm/memory.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/sched.h>

#include "hi_kernel_adapt.h"

#include "drv_mce_avplay.h"
#include "drv_base_ext_k.h"
#include "drv_mmz_ext.h"
#include "drv_i2c_ext.h"
#include "drv_gpio_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_tuner_ext.h"
#include "drv_demux_ext.h"
#include "drv_base_ext_k.h"
#include "drv_vdec_ext.h"
#include "drv_vo_ext.h"
#include "drv_disp_ext.h"
#include "drv_sync_ext.h"
#include "drv_vfmw_ext.h"
#include "drv_tde_ext.h"
#include "drv_file_ext.h"
#include "drv_adec_ext.h"
#include "drv_hiao_ext.h"
#include "drv_hdmi_ext.h"
#include "hi_drv_mce.h"
#include "drv_mce_boardcfg.h"
#include "hi_drv_pdm.h"

#define MCE_DMX_ID              0
#define MCE_DMX_TS_PORT         0
#define MCE_DMX_DVB_PORT        0
#define MCE_GET_TS_LEN          (188*50)


#define MCE_LOCK(pMutex)    \
    do{ \
        if(down_interruptible(pMutex))  \
        {       \
            HI_ERR_MCE("ERR: mce lock err!\n"); \
        }       \
    }while(0)

#define MCE_UNLOCK(pMutex)    \
    do{ \
        up(pMutex); \
    }while(0)

extern MCE_S   g_Mce;

HI_S32 MCE_Init(HI_VOID);
HI_S32 MCE_DeInit(HI_VOID);

HI_U32 MCE_GetCurTime(HI_VOID)
{
    HI_U64   SysTime;

    SysTime = sched_clock();

    do_div(SysTime, 1000000);

    return (HI_U32)SysTime;
    
}

HI_VOID MCE_TransFomat(HI_UNF_ENC_FMT_E enSrcFmt, HI_UNF_ENC_FMT_E *penHdFmt, HI_UNF_ENC_FMT_E *penSdFmt)
{
    switch(enSrcFmt)
    {
        /* bellow are tv display formats */
        case HI_UNF_ENC_FMT_1080P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_1080P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        }
        case HI_UNF_ENC_FMT_1080i_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_1080i_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        }
        case HI_UNF_ENC_FMT_720P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_720P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_720P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_720P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        } 
        case HI_UNF_ENC_FMT_576P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_576P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;        
        }
        case HI_UNF_ENC_FMT_480P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_480P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;         
        }
        case HI_UNF_ENC_FMT_PAL:
        {
            *penHdFmt = HI_UNF_ENC_FMT_PAL;
            *penSdFmt = HI_UNF_ENC_FMT_PAL;
            break;
        }
        case HI_UNF_ENC_FMT_NTSC:
        {
            *penHdFmt = HI_UNF_ENC_FMT_NTSC;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        } 
        
        /* bellow are vga display formats */
        case HI_UNF_ENC_FMT_861D_640X480_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_800X600_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1024X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X720_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X800_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X1024_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1360X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1366X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1400X1050_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1440X900_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;    
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1440X900_60_RB:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1600X900_60_RB:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1600X1200_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1680X1050_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        } 
        
        case HI_UNF_ENC_FMT_VESA_1920X1080_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1920X1200_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;            
            break;
        }
        case HI_UNF_ENC_FMT_VESA_2048X1152_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;            
            break;
        }
        default:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL;
            break;
        }
    }

    return;
}

HI_S32 MCE_TransToNewWin(HI_HANDLE hOldWin, HI_HANDLE hNewWin)
{
    HI_S32                          Ret = HI_SUCCESS;
    HI_UNF_VIDEO_FRAME_INFO_S       stCapPic;

    memset(&stCapPic, 0, sizeof(stCapPic));

    Ret = HI_DRV_VO_CapturePicture(hOldWin, &stCapPic);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_CapturePicture \n");
        return Ret;
    }
    
    Ret = HI_DRV_VO_SendFrame(hNewWin, &stCapPic);
    if (HI_SUCCESS != Ret)
    {
        (HI_VOID)HI_DRV_VO_CapturePictureRelease(hOldWin, &stCapPic);
        HI_ERR_MCE("ERR: HI_DRV_VO_SendFrame \n");
        return Ret;
    }
    
    Ret = HI_DRV_VO_SetWindowZorder(hNewWin, HI_LAYER_ZORDER_MOVETOP);
    if (HI_SUCCESS != Ret)
    {
        (HI_VOID)HI_DRV_VO_CapturePictureRelease(hOldWin, &stCapPic);
        HI_ERR_MCE("ERR: HI_DRV_VO_SetWindowZorder \n");
        return Ret;
    }   
    
    Ret = HI_DRV_VO_CapturePictureRelease(hOldWin, &stCapPic);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_CapturePictureRelease \n");
        return Ret;
    } 

    return Ret;
}

HI_S32 MCE_ADP_DispOpen(HI_DISP_PARAM_S stDispParam)
{
    HI_S32                      Ret;
    HI_UNF_ENC_FMT_E            HdFmt, SdFmt;
    HI_UNF_DISP_BG_COLOR_S      stBgColor;
#if 0
    /*set the bgcolor to black*/
    stBgColor.u8Blue = 0x00;
    stBgColor.u8Green = 0x00;
    stBgColor.u8Red = 0x00;
    Ret |= HI_DRV_DISP_SetBgColor(HI_UNF_DISPLAY1, &stBgColor);
    Ret |= HI_DRV_DISP_SetBgColor(HI_UNF_DISPLAY0, &stBgColor);

    /* set sd and hd format */
    MCE_TransFomat(stDispParam.enFormat, &HdFmt, &SdFmt);
    Ret |= HI_DRV_DISP_SetFormat(HI_UNF_DISPLAY1, HdFmt);
    Ret |= HI_DRV_DISP_SetFormat(HI_UNF_DISPLAY0, SdFmt);

    Ret |= HI_DRV_DISP_SetBrightness(HI_UNF_DISPLAY1, stDispParam.u32Brightness);
    Ret |= HI_DRV_DISP_SetContrast(HI_UNF_DISPLAY1, stDispParam.u32Contrast);
    Ret |= HI_DRV_DISP_SetSaturation(HI_UNF_DISPLAY1, stDispParam.u32Saturation);
    Ret |= HI_DRV_DISP_SetHuePlus(HI_UNF_DISPLAY1, stDispParam.u32HuePlus);

    Ret |= HI_DRV_DISP_SetBrightness(HI_UNF_DISPLAY0, stDispParam.u32Brightness);
    Ret |= HI_DRV_DISP_SetContrast(HI_UNF_DISPLAY0, stDispParam.u32Contrast);
    Ret |= HI_DRV_DISP_SetSaturation(HI_UNF_DISPLAY0, stDispParam.u32Saturation);
    Ret |= HI_DRV_DISP_SetHuePlus(HI_UNF_DISPLAY0, stDispParam.u32HuePlus); 

    /* set sd and hd dac interface */
    if (HI_UNF_DISP_INTF_TYPE_BUTT != stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].enIntfType)
    {
        Ret |= HI_DRV_DISP_AttachIntf(HI_UNF_DISPLAY0, &(stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_CVBS]), 1);
    }
    
    if (HI_UNF_DISP_INTF_TYPE_BUTT != stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_SVIDEO].enIntfType)
    {
        Ret |= HI_DRV_DISP_AttachIntf(HI_UNF_DISPLAY0, &(stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_SVIDEO]), 1);
    }

    if (HI_UNF_DISP_INTF_TYPE_BUTT != stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].enIntfType)
    {
        Ret |= HI_DRV_DISP_AttachIntf(HI_UNF_DISPLAY1, &(stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR]), 1);
    }
    else if (HI_UNF_DISP_INTF_TYPE_BUTT != stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_RGB].enIntfType)
    {
        Ret |= HI_DRV_DISP_AttachIntf(HI_UNF_DISPLAY1, &(stDispParam.stIntf[HI_UNF_DISP_INTF_TYPE_RGB]), 1);
    }
#endif
    Ret |= HI_DRV_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);

    Ret |= HI_DRV_DISP_Open(HI_UNF_DISPLAY1);
    Ret |= HI_DRV_DISP_Open(HI_UNF_DISPLAY0);

    Ret |= HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_0);

    return Ret;
}

HI_S32 MCE_ADP_DispClose(HI_VOID)
{
    HI_S32      Ret;

    Ret = HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_0);

    Ret = HI_DRV_DISP_Close(HI_UNF_DISPLAY0);

    Ret |= HI_DRV_DISP_Close(HI_UNF_DISPLAY1);
    
    Ret |= HI_DRV_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);

    return Ret;
}

HI_S32 MCE_ADP_VoOpen(HI_VOID)
{
    HI_S32      Ret;

    Ret = HI_DRV_VO_SetDevMode(HI_UNF_VO_DEV_MODE_NORMAL);

    return Ret;
}

HI_S32 MCE_ADP_VoClose(HI_VOID)
{
    //HI_S32      Ret;

    //Ret = HI_DRV_VO_Close(HI_UNF_DISPLAY1);

    return HI_SUCCESS;
}

HI_S32 MCE_ADP_VoCreateWin(HI_HANDLE *phWin)
{
    HI_S32                  Ret;
    HI_UNF_WINDOW_ATTR_S    WinAttr;
    
    WinAttr.enDisp = HI_UNF_DISPLAY1;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    WinAttr.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_IGNORE;    
    WinAttr.stInputRect.s32X = 0;
    WinAttr.stInputRect.s32Y = 0;
    WinAttr.stInputRect.s32Width = 1920;
    WinAttr.stInputRect.s32Height = 1080;

    memset(&WinAttr.stOutputRect, 0x0, sizeof(HI_RECT_S));
    //mcpy(&WinAttr.stOutputRect,&WinAttr.stInputRect,sizeof(HI_RECT_S));
    
    Ret = HI_DRV_VO_CreateWindow(&WinAttr, phWin);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_CreateWindow\n");
    }

    return Ret;
}


HI_S32 MCE_ADP_TunerConnect(HI_UNF_MCE_DVB_PARAM_S stParam)
{
    HI_S32                              Ret;
    HI_UNF_TUNER_ATTR_S                 stAttr;
    HI_UNF_TUNER_SAT_ATTR_S             stSatTunerAttr;
    
    Ret = HI_DRV_TUNER_GetDeftAttr(TUNER_USE, &stAttr);
    GET_TUNER_CONFIG(stAttr);
    Ret |= HI_DRV_TUNER_SetAttr(TUNER_USE, &stAttr);

    if (HI_UNF_TUNER_SIG_TYPE_SAT == stAttr.enSigType)
    {
        GET_SAT_TUNER_CONFIG(stSatTunerAttr);
        Ret |= HI_DRV_TUNER_SetSatAttr(TUNER_USE, &stSatTunerAttr);

        Ret |= HI_DRV_TUNER_SetLNBConfig(TUNER_USE, &stParam.stLnbCfg);

        Ret |= HI_DRV_TUNER_SetLNBPower(TUNER_USE, stParam.enLNBPower);

        Ret |= HI_DRV_TUNER_DISEQC_Switch16Port(TUNER_USE, &stParam.st16Port);

        Ret |= HI_DRV_TUNER_DISEQC_Switch4Port(TUNER_USE, &stParam.st4Port);

        Ret |= HI_DRV_TUNER_Switch22K(TUNER_USE, stParam.enSwitch22K);
    }
    
    Ret |= HI_DRV_TUNER_Connect(TUNER_USE, &stParam.stConnectPara, 500);

    return Ret;
}

HI_S32 MCE_TsThread(HI_VOID *args)
{
    HI_S32                          Ret;
    DMX_DATA_BUF_S                  StreamBuf; 
    HI_U32                          TotalLen = 0;
    HI_U32                          ReadLen = 0;
    HI_U32                          CycleCount = 0;
    HI_BOOL                         bSendEnd = HI_FALSE;
    MCE_S                           *pMce;

    pMce = (MCE_S *)args;

    while(!pMce->bPlayStop)
    {    
        if (bSendEnd)
        {            
            if (HI_TRUE == HI_DRV_AVPLAY_IsBufEmpty(pMce->hAvplay))
            {
                pMce->TsplayEnd = HI_TRUE;
            }

            msleep(50);
            continue;
        }
        
        Ret = HI_DRV_DMX_GetTSBuffer(MCE_DMX_TS_PORT, MCE_GET_TS_LEN, &StreamBuf, 1000);
        if(HI_SUCCESS != Ret)
        {
            msleep(10);
            continue;
        }

        if(TotalLen + MCE_GET_TS_LEN < pMce->stMceParam.u32PlayDataLen)
        {
            ReadLen = MCE_GET_TS_LEN;
            memcpy((HI_U8 *)StreamBuf.BufKerAddr, (HI_U8 *)(pMce->u32PlayDataAddr + TotalLen), ReadLen);
            TotalLen += ReadLen;
        }
        else
        {
            ReadLen = pMce->stMceParam.u32PlayDataLen - TotalLen;
            memcpy((HI_U8 *)StreamBuf.BufKerAddr, (HI_U8 *)(pMce->u32PlayDataAddr + TotalLen), ReadLen);
            TotalLen = 0;
            CycleCount++;
        }

        Ret = HI_DRV_DMX_PutTSBuffer(MCE_DMX_TS_PORT, ReadLen, 0);
        if(HI_SUCCESS != Ret)
        {
            HI_ERR_MCE("ERR: HI_DRV_DMX_PutTSBuffer\n");
        }

        if (HI_UNF_MCE_PLAYCTRL_BY_COUNT == pMce->stStopParam.enCtrlMode)
        {
            if (CycleCount >= pMce->stStopParam.u32PlayCount)
            {
                bSendEnd = HI_TRUE;
            }
        }
    }
    
    Ret = HI_DRV_DMX_ResetTSBuffer(MCE_DMX_TS_PORT);

    return Ret;
}

HI_S32 MCE_TsPlayStart(MCE_S *pMce)
{
    HI_S32                          Ret;
    HI_UNF_AVPLAY_ATTR_S            AvplayAttr;
    HI_UNF_VCODEC_ATTR_S            VdecAttr;
    HI_UNF_AVPLAY_OPEN_OPT_S        OpenOpt;
    HI_UNF_SYNC_ATTR_S              SyncAttr;
    HI_HANDLE                       hVdec;
    HI_HANDLE                       hSync;
    HI_HANDLE                       hSnd;
    HI_UNF_MCE_TSFILE_PARAM_S       stParam;

    stParam = pMce->stMceParam.stPlayParam.unParam.stTsParam;
    
    Ret = HI_DRV_HIAO_Open(HI_NULL);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_HIAO_Open!\n");
        goto ERR0;
    }

    Ret = MCE_ADP_DispOpen(pMce->stDispParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ADP_DispOpen!\n");
        goto ERR1;
    }

    Ret = MCE_ADP_VoOpen();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_Open!\n");
        goto ERR2;
    }

    Ret = MCE_ADP_VoCreateWin(&pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_SetWindowEnable!\n");
        goto ERR3;
    }

    Ret = HI_DRV_DMX_AttachRamPort(MCE_DMX_ID, MCE_DMX_TS_PORT);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DMX_AttachRamPort!\n");
        goto ERR4;
    }

    Ret = HI_DRV_DMX_CreateTSBuffer(MCE_DMX_TS_PORT, 0x200000, &pMce->TsBuf, HI_NULL);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DMX_CreateTSBuffer!\n");
        goto ERR5;
    }

    Ret = HI_DRV_DMX_ResetTSBuffer(MCE_DMX_TS_PORT);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DMX_ResetTSBuffer!\n");
        goto ERR6;
    }
    
    Ret = HI_DRV_AVPLAY_Init();
    AvplayAttr.u32DemuxId = MCE_DMX_ID;
    AvplayAttr.stStreamAttr.enStreamType = HI_UNF_AVPLAY_STREAM_TYPE_TS;
    AvplayAttr.stStreamAttr.u32VidBufSize = (5*1024*1024);
    AvplayAttr.stStreamAttr.u32AudBufSize = (384*1024);
    Ret |= HI_DRV_AVPLAY_Create(&AvplayAttr, &pMce->hAvplay);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Create!\n");
        goto ERR6;
    }

    Ret = HI_DRV_AVPLAY_ChnOpen(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);

    OpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    OpenOpt.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    OpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;   
    Ret |= HI_DRV_AVPLAY_ChnOpen(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &OpenOpt);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_ChnOpen!\n");
        goto ERR7;
    }

    Ret = HI_DRV_HIAO_GetHandle(&hSnd, HIAO_MAIN_PORD_ID);
    Ret |= HI_DRV_AVPLAY_AttachSnd(pMce->hAvplay, hSnd);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_MPI_AVPLAY_AttachSnd!\n");
        goto ERR8;
    }
    
    Ret = HI_DRV_AVPLAY_GetSyncVdecHandle( pMce->hAvplay, &hVdec, &hSync);    
    Ret |= HI_DRV_VO_AttachWindow(pMce->hWindow, hVdec, hSync, HI_ID_AVPLAY);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_AttachWindow!\n");
        goto ERR9;
    }

    Ret = HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_TRUE);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_SetWindowEnable!\n");
        goto ERR10;
    }

    Ret = HI_DRV_AVPLAY_AttachWindow(pMce->hAvplay, pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_AttachWindow!\n");
        goto ERR10;
    } 
    
    Ret = HI_DRV_AVPLAY_SetAttr(pMce->hAvplay,HI_UNF_AVPLAY_ATTR_ID_VID_PID, &stParam.u32VideoPid);
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay,HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &stParam.u32AudioPid);

    Ret |= HI_DRV_AVPLAY_GetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);

    Ret |= HI_DRV_AVPLAY_GetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    VdecAttr.enType = stParam.enVideoType;
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_SetAttr!\n");
        goto ERR11;
    }

    pMce->pTsTask = kthread_create(MCE_TsThread, pMce, "MceTsPlay");
    if(IS_ERR(pMce->pTsTask) < 0)
    {
        HI_ERR_MCE("ERR: crate thread err!\n");
        goto ERR11;
    }
   
    wake_up_process(pMce->pTsTask);
    
    Ret = HI_DRV_AVPLAY_Start(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Start!\n");
        goto ERR12;
    } 

    pMce->BeginTime = MCE_GetCurTime();;

    return Ret;

ERR12:
    pMce->bPlayStop = HI_TRUE;
    kthread_stop(pMce->pTsTask);
    pMce->pTsTask = HI_NULL;    
ERR11:
    HI_DRV_AVPLAY_DetachWindow(pMce->hAvplay, pMce->hWindow);
ERR10:
    HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_FALSE);
    HI_DRV_VO_DetachWindow(pMce->hWindow, hVdec);
ERR9:
    HI_DRV_AVPLAY_DetachSnd(pMce->hAvplay, hSnd);
ERR8:
    HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
ERR7:
    HI_DRV_AVPLAY_Destroy(pMce->hAvplay);
    HI_DRV_AVPLAY_DeInit();
ERR6:
    (HI_VOID)HI_DRV_DMX_DestroyTSBuffer(MCE_DMX_TS_PORT);
ERR5:
    (HI_VOID)HI_DRV_DMX_DetachPort(MCE_DMX_ID);
ERR4:
    HI_DRV_VO_DestroyWindow(pMce->hWindow);
ERR3:
    MCE_ADP_VoClose();
ERR2:
    MCE_ADP_DispClose();
ERR1:
    HI_DRV_HIAO_Close();
ERR0:
    return Ret;
}


HI_S32 MCE_TsPlayStop(MCE_S *pMce)
{
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;

    stStop.enMode = pMce->stStopParam.enStopMode;
    stStop.u32TimeoutMs = 0;
    (HI_VOID)HI_DRV_AVPLAY_Stop(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStop);

    if(HI_NULL != pMce->pTsTask)
    {
        pMce->bPlayStop = HI_TRUE;
        kthread_stop(pMce->pTsTask);
        pMce->pTsTask = HI_NULL;
    }
   
    return HI_SUCCESS;
}

/*release tsplay resource*/
HI_S32  MCE_TsPlayExit(MCE_S *pMce, HI_UNF_MCE_EXITPARAM_S *pstExitParam)
{
    HI_HANDLE                   hSync, hVdec, hSnd;

    (HI_VOID)HI_DRV_HIAO_GetHandle(&hSnd, HIAO_MAIN_PORD_ID);
    (HI_VOID)HI_DRV_AVPLAY_GetSyncVdecHandle( pMce->hAvplay, &hVdec, &hSync); 

    if (HI_INVALID_HANDLE != pstExitParam->hNewWin)
    {
        (HI_VOID)MCE_TransToNewWin(pMce->hWindow, pstExitParam->hNewWin);
    }
    
    (HI_VOID)HI_DRV_AVPLAY_DetachWindow(pMce->hAvplay, pMce->hWindow);

    (HI_VOID)HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_FALSE);
    
    (HI_VOID)HI_DRV_VO_DetachWindow(pMce->hWindow, hVdec);

    (HI_VOID)HI_DRV_AVPLAY_DetachSnd(pMce->hAvplay, hSnd);

    (HI_VOID)HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    (HI_VOID)HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

    (HI_VOID)HI_DRV_AVPLAY_Destroy(pMce->hAvplay);
    
    (HI_VOID)HI_DRV_AVPLAY_DeInit();

    (HI_VOID)HI_DRV_VO_DestroyWindow(pMce->hWindow);

    (HI_VOID)HI_DRV_DMX_DestroyTSBuffer(MCE_DMX_TS_PORT);

	/*still mode, the following resource need not to be released*/
    if (HI_INVALID_HANDLE != pstExitParam->hNewWin)
    {
        return HI_SUCCESS;
    }

    (HI_VOID)HI_DRV_DMX_DetachPort(MCE_DMX_ID);
        
    (HI_VOID)MCE_ADP_VoClose();

    /*we can not close display here for smooth trans*/
#if 0
    (HI_VOID)MCE_ADP_DispClose();
#endif

    (HI_VOID)HI_DRV_HIAO_Close();
   
    return HI_SUCCESS;
}

HI_S32 MCE_DvbPlayStart(MCE_S *pMce)
{
    HI_S32                          Ret;
    HI_UNF_AVPLAY_ATTR_S            AvplayAttr;
    HI_UNF_VCODEC_ATTR_S            VdecAttr;
    HI_UNF_AVPLAY_OPEN_OPT_S        OpenOpt;
    HI_UNF_SYNC_ATTR_S              SyncAttr;
    HI_HANDLE                       hVdec;
    HI_HANDLE                       hSync;
    HI_HANDLE                       hSnd;    
    HI_UNF_MCE_DVB_PARAM_S          stParam;

    stParam = pMce->stMceParam.stPlayParam.unParam.stDvbParam;

    Ret = HI_DRV_HIAO_Open(HI_NULL);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_HIAO_Open!\n");
        goto ERR0;
    }
    
    Ret = MCE_ADP_DispOpen(pMce->stDispParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ADP_DispOpen!\n");
        goto ERR1;
    }
    
    Ret = MCE_ADP_VoOpen();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_Open!\n");
        goto ERR2;
    }
    
    Ret = MCE_ADP_VoCreateWin(&pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_SetWindowEnable!\n");
        goto ERR3;
    }
    
    Ret = HI_DRV_DMX_AttachTunerPort(MCE_DMX_ID, MCE_DMX_DVB_PORT);   
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DMX_AttachTunerPort!\n");
        goto ERR4;
    }

    Ret = HI_DRV_AVPLAY_Init();
    AvplayAttr.u32DemuxId = MCE_DMX_ID;
    AvplayAttr.stStreamAttr.enStreamType = HI_UNF_AVPLAY_STREAM_TYPE_TS;
    AvplayAttr.stStreamAttr.u32VidBufSize = (5*1024*1024);
    AvplayAttr.stStreamAttr.u32AudBufSize = (384*1024);
    Ret |= HI_DRV_AVPLAY_Create(&AvplayAttr, &pMce->hAvplay);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Create!\n");
        goto ERR5;
    }

    Ret = HI_DRV_AVPLAY_ChnOpen(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    OpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    OpenOpt.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    OpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;   
    Ret |= HI_DRV_AVPLAY_ChnOpen(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &OpenOpt);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_ChnOpen!\n");
        goto ERR6;
    }

    Ret = HI_DRV_HIAO_GetHandle(&hSnd, HIAO_MAIN_PORD_ID);
    Ret |= HI_DRV_AVPLAY_AttachSnd(pMce->hAvplay, hSnd);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_MPI_AVPLAY_AttachSnd!\n");
        goto ERR7;
    }
    
    Ret = HI_DRV_AVPLAY_GetSyncVdecHandle( pMce->hAvplay, &hVdec, &hSync);    
    Ret |= HI_DRV_VO_AttachWindow(pMce->hWindow, hVdec, hSync, HI_ID_AVPLAY);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_AttachWindow!\n");
        goto ERR8;
    }

    Ret = HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_TRUE);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_SetWindowEnable!\n");
        goto ERR9;
    }

    Ret = HI_DRV_AVPLAY_AttachWindow(pMce->hAvplay, pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_AttachWindow!\n");
        goto ERR9;
    } 
    
    Ret = HI_DRV_AVPLAY_SetAttr(pMce->hAvplay,HI_UNF_AVPLAY_ATTR_ID_VID_PID, &stParam.u32VideoPid);
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay,HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &stParam.u32AudioPid);

    Ret |= HI_DRV_AVPLAY_GetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);

    Ret |= HI_DRV_AVPLAY_GetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    VdecAttr.enType = stParam.enVideoType;
    Ret |= HI_DRV_AVPLAY_SetAttr(pMce->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_SetAttr!\n");
        goto ERR10;
    }

    Ret = HI_DRV_TUNER_Open(TUNER_USE);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_TUNER_Open!\n");
        goto ERR10;
    }
    
    Ret = MCE_ADP_TunerConnect(stParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ADP_TunerConnect!\n");
        goto ERR11;
    }    

    Ret = HI_DRV_AVPLAY_Start(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Start!\n");
        goto ERR11;
    } 

    pMce->BeginTime = MCE_GetCurTime();
    
    return Ret;

ERR11:
    HI_DRV_TUNER_Close(TUNER_USE);
ERR10:
    HI_DRV_AVPLAY_DetachWindow(pMce->hAvplay, pMce->hWindow);
ERR9:
    HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_FALSE);
    HI_DRV_VO_DetachWindow(pMce->hWindow, hVdec);
ERR8:
    HI_DRV_AVPLAY_DetachSnd(pMce->hAvplay, hSnd);
ERR7:
    HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
ERR6:
    HI_DRV_AVPLAY_Destroy(pMce->hAvplay);
    (HI_VOID)HI_DRV_AVPLAY_DeInit();
ERR5:
    HI_DRV_DMX_DetachPort(MCE_DMX_ID);
ERR4:
    HI_DRV_VO_DestroyWindow(pMce->hWindow);
ERR3:
    MCE_ADP_VoClose();
ERR2:
    MCE_ADP_DispClose();
ERR1:
    HI_DRV_HIAO_Close();
ERR0:
    return Ret;
}

HI_S32 MCE_DvbPlayStop(MCE_S *pMce)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;

    stStop.enMode = pMce->stStopParam.enStopMode;
    stStop.u32TimeoutMs = 0;
    Ret = HI_DRV_AVPLAY_Stop(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStop);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Stop!\n");
        return Ret;
    }

    return HI_SUCCESS;
}

/*release dvbplay resource*/
HI_S32  MCE_DvbPlayExit(MCE_S *pMce, HI_UNF_MCE_EXITPARAM_S *pstExitParam)
{
    HI_S32                      Ret;
    HI_HANDLE                   hSync, hVdec, hSnd;

    (HI_VOID)HI_DRV_HIAO_GetHandle(&hSnd, HIAO_MAIN_PORD_ID);
    (HI_VOID)HI_DRV_AVPLAY_GetSyncVdecHandle( pMce->hAvplay, &hVdec, &hSync);      

    if (HI_INVALID_HANDLE != pstExitParam->hNewWin)
    {
        (HI_VOID)MCE_TransToNewWin(pMce->hWindow, pstExitParam->hNewWin);
    }
    
    Ret = HI_DRV_AVPLAY_DetachWindow(pMce->hAvplay, pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_DetachWindow!\n");
        return Ret;
    }    
    
    Ret = HI_DRV_VO_SetWindowEnable(pMce->hWindow, HI_FALSE);
    Ret |= HI_DRV_VO_DetachWindow(pMce->hWindow, hVdec);  
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_DetachWindow!\n");
        return Ret;
    }  

    Ret |= HI_DRV_AVPLAY_DetachSnd(pMce->hAvplay, hSnd);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_DetachSnd!\n");
        return Ret;
    }
    
    Ret = HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_ChnClose!\n");
        return Ret;
    }
    
    Ret = HI_DRV_AVPLAY_ChnClose(pMce->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_ChnClose!\n");
        return Ret;
    }
    
    Ret = HI_DRV_AVPLAY_Destroy(pMce->hAvplay);
    Ret|= HI_DRV_AVPLAY_DeInit();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Destroy!\n");
        return Ret;
    }

    Ret = HI_DRV_VO_DestroyWindow(pMce->hWindow);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_DestroyWindow!\n");
        return Ret;
    }
    
	/*still mode, the following resource need not to be released*/
    if (HI_INVALID_HANDLE != pstExitParam->hNewWin)
    {
        return HI_SUCCESS;
    }

    Ret = HI_DRV_TUNER_Close(TUNER_USE);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_TUNER_Close!\n");
        return Ret;
    }

    Ret = HI_DRV_DMX_DetachPort(MCE_DMX_ID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_AVPLAY_Destroy!\n");
        return Ret;
    }
    /**/
    Ret = MCE_ADP_VoClose();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ADP_VoClose!\n");
        return Ret;
    }

    /*we can not close display here for smooth trans*/
#if 0   
    Ret = MCE_ADP_DispClose();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ADP_DispClose!\n");
        return Ret;
    }
#endif   
    Ret = HI_DRV_HIAO_Close();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_HIAO_Close!\n");
        return Ret;
    }   
    
    return HI_SUCCESS;    
}


HI_S32 MCE_PlayStart(MCE_S *pMce)
{
    HI_S32      Ret = HI_SUCCESS;

    if(HI_UNF_MCE_TYPE_PLAY_DVB == pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_DvbPlayStart(pMce);
    }
    else if(HI_UNF_MCE_TYPE_PLAY_TSFILE == pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_TsPlayStart(pMce);
    }

    /* disable gfx after vid play start */
    Ret |= HI_DRV_DISP_SetGfxEnable(HI_UNF_DISPLAY0, HI_FALSE);
    Ret |= HI_DRV_DISP_SetGfxEnable(HI_UNF_DISPLAY1, HI_FALSE);

    /*release the reserve mem for logo*/
    HI_DRV_PDM_ReleaseReserveMem("LAYER_SURFACE");
    HI_DRV_PDM_ReleaseReserveMem("VO_GfxWbc2");
    
    return Ret;
}

HI_S32 MCE_PlayStop(MCE_S *pMce)
{
    HI_S32      Ret;

    if(HI_UNF_MCE_TYPE_PLAY_DVB ==  pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_DvbPlayStop(pMce);
    }
    else if(HI_UNF_MCE_TYPE_PLAY_TSFILE == pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_TsPlayStop(pMce);

        /*release reserve mem for ts data*/
        HI_DRV_PDM_ReleaseReserveMem("playdata");
    }
    
    return HI_SUCCESS;
}

HI_S32 MCE_ModuleInit(HI_VOID)
{
#if 1
    HI_S32      Ret;
    
    Ret = HI_DRV_CommonInit();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_CommonInit!\n");
        return Ret;
    }

    Ret = HI_DRV_I2C_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_I2C_Init!\n");
        return Ret;
    }

    Ret = HI_DRV_GPIO_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_GPIO_Init!\n");
        return Ret;
    }
    
#ifdef HI_GPIOI2C_SUPPORT
    Ret = HI_DRV_GPIOI2C_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_GPIOI2C_Init!\n");
        return Ret;
    }
#endif

    Ret = HI_DRV_TUNER_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_TUNER_Init!\n");
        return Ret;
    }
    
    Ret = HI_DRV_DMX_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DMX_Init!\n");
        return Ret;
    }

    Ret = VFMW_DRV_ModInit();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: VFMW_DRV_Init!\n");
        return Ret;
    }

    Ret = HI_DRV_VDEC_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_Init!\n");
        return Ret;
    }

    Ret = HI_DRV_SYNC_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_SYNC_Init!\n");
        return Ret;
    } 

    Ret = HI_DRV_PDM_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_PDM_Init!\n");
        return Ret;
    } 

    Ret = HI_DRV_DISP_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_DISP_Init!\n");
        return Ret;
    } 

    Ret = HI_DRV_HDMI_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_HDMI_Init!\n");
        return Ret;    
    }

    Ret = tde_init_module_k();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_TDE_Init!\n");
        return Ret;
    } 

    Ret = HI_DRV_VO_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VO_ModInit!\n");
        return Ret;
    }
 
    Ret = HI_DRV_ADEC_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_ADEC_Init!\n");
        return Ret;
    } 

    Ret = HI_DRV_HIAO_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_HIAO_Init!\n");
        return Ret;
    }     

    Ret = hifb_init_module_k();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: hifb_init_module_k!\n");
        return Ret;
    }     

    return Ret;
#endif  
    return HI_SUCCESS;
}

HI_S32 HI_DRV_MCE_Exit(HI_UNF_MCE_EXITPARAM_S *pstExitParam)
{
    HI_S32              Ret = HI_SUCCESS;
    MCE_S               *pMce = &g_Mce;

    if (pMce->bMceExit)
    {
        return HI_SUCCESS;
    }
    
    if(HI_UNF_MCE_TYPE_PLAY_DVB == pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_DvbPlayExit(pMce, pstExitParam);
    }
    else if(HI_UNF_MCE_TYPE_PLAY_TSFILE == pMce->stMceParam.stPlayParam.enPlayType)
    {
        Ret = MCE_TsPlayExit(pMce, pstExitParam);
    }  
    
    pMce->bMceExit = HI_TRUE;
    
    return Ret;
}

HI_S32 HI_DRV_MCE_Stop(HI_UNF_MCE_STOPPARM_S *pstStopParam)
{
    HI_S32              Ret;
    HI_U32              CurTime;
    MCE_S               *pMce = &g_Mce;

    if ((HI_UNF_MCE_TYPE_PLAY_DVB == pMce->stMceParam.stPlayParam.enPlayType) &&
        (HI_UNF_MCE_PLAYCTRL_BY_COUNT == pstStopParam->enCtrlMode))
    {
        HI_ERR_MCE("ERR: DVB play type does not support count control mode\n");
    }

    if (pMce->bPlayStop)
    {
        return HI_SUCCESS;
    }
	
    memcpy(&pMce->stStopParam, pstStopParam, sizeof(HI_UNF_MCE_STOPPARM_S));

    while(1)
    {
        CurTime = MCE_GetCurTime();
        
        if (HI_UNF_MCE_PLAYCTRL_BY_TIME == pstStopParam->enCtrlMode)
        {
            if (CurTime - pMce->BeginTime > pstStopParam->u32PlayTimeMs)
            {
                break;
            }
        }
        else if (HI_UNF_MCE_PLAYCTRL_BY_COUNT == pstStopParam->enCtrlMode)
        {
            if (pMce->TsplayEnd)
            {
                break;
            }
        }
        else
        {
            break;
        }
        
        msleep(100);
    }

    pMce->EndTime = MCE_GetCurTime();
    
    Ret = MCE_PlayStop(pMce);

    pMce->bPlayStop = HI_TRUE;

    return Ret;
}


HI_S32 MCE_Init(HI_VOID)
{
    HI_S32              Ret;
    MCE_S               *pMce = &g_Mce;

    Ret = MCE_ModuleInit();
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_ModuleInit\n");
        Ret = HI_FAILURE;  
        goto ERR0;
    } 

    Ret = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &pMce->stDispParam);
    Ret |= HI_DRV_PDM_GetGrcParam(&pMce->stGrcParam);
    Ret |= HI_DRV_PDM_GetMceParam(&pMce->stMceParam);
    if (0 != pMce->stMceParam.u32PlayDataLen)
    {
        Ret |= HI_DRV_PDM_GetMceData(pMce->stMceParam.u32PlayDataLen, &pMce->u32PlayDataAddr);
    }

    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: get PDM Param\n");
        Ret = HI_FAILURE;  
        goto ERR0;
    }    
         
    if ((1 != pMce->stMceParam.u32CheckFlag) || (HI_TRUE != pMce->stMceParam.stPlayParam.bPlayEnable))
    {
        HI_ERR_MCE("mce checkflag is not open\n");
        Ret = HI_FAILURE;
        goto ERR0; 
    }
        
    pMce->bPlayStop = HI_FALSE;
    pMce->bMceExit = HI_FALSE;

    Ret = MCE_PlayStart(pMce);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: MCE_PlayStart\n");
        Ret = HI_FAILURE;
        goto ERR0;  
    } 

    return Ret;
    
ERR0:
    pMce->bMceExit = HI_TRUE;
    return Ret;
}

HI_S32 MCE_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

#ifndef MODULE
early_initcall(MCE_Init);
#else
#endif

