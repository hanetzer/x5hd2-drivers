
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_display.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISPLAY_H__
#define __DRV_DISPLAY_H__

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


HI_S32 DISP_Init(HI_VOID);
HI_S32 DISP_DeInit(HI_VOID);

HI_S32 DISP_Suspend(HI_VOID);
HI_S32 DISP_Resume(HI_VOID);

HI_S32 DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
HI_S32 DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);

HI_S32 DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enEncFmt);
HI_S32 DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penEncFmt);

HI_S32 DISP_SetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming);
HI_S32 DISP_GetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming);

HI_S32 DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
HI_S32 DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);


HI_S32 DISP_Open(HI_DRV_DISPLAY_E enDisp);
HI_S32 DISP_Close(HI_DRV_DISPLAY_E enDisp);

HI_S32 DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
HI_S32 DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);

HI_S32 DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);

HI_S32 DISP_SetBGColor(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_COLOR_S *pstBGColor);
HI_S32 DISP_GetBGColor(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_COLOR_S *pstBGColor);

//set color
HI_S32 DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
HI_S32 DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);

/* set Display output window  */
HI_S32 DISP_GetScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect);
HI_S32 DISP_SetScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect);

//set aspect ratio: 0 and 0 means auto
HI_S32 DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v);
HI_S32 DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v);

HI_S32 DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag);
HI_S32 DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

//miracast
HI_S32 DISP_CreateCast(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast);
HI_S32 DISP_DestroyCast(HI_HANDLE hCast);
HI_S32 DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable);
HI_S32 DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable);

HI_S32 DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);

//snapshot
HI_S32 DISP_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstSnapShotFrame);

//Macrovision
HI_S32 DISP_TestMacrovisionSupport(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbSupport);
HI_S32 DISP_SetMacrovisionCustomer(HI_DRV_DISPLAY_E enDisp, HI_VOID *pData);
HI_S32 DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode);
HI_S32 DISP_GetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E *penMode);

//cgms-a
HI_S32 DISP_SetCGMS_A(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S *pstCfg);

//vbi
HI_S32 DISP_CreateVBIChannel(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_VBI_CFG_S *pstCfg, HI_HANDLE *phVbi);
HI_S32 DISP_DestroyVBIChannel(HI_HANDLE hVbi);
HI_S32 DISP_SendVbiData(HI_HANDLE hVbi, HI_DRV_DISP_VBI_DATA_S *pstVbiData);
HI_S32 DISP_SetWss(HI_HANDLE hVbi, HI_DRV_DISP_WSS_DATA_S *pstWssData);

//may be deleted
//setting
//HI_S32 DISP_SetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting);
//HI_S32 DISP_GetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting);
//HI_S32 DISP_ApplySetting(HI_DRV_DISPLAY_E enDisp);


/*****************************************************/
//internal state
HI_S32  DISP_GetInitFlag(HI_BOOL *pbInited);
HI_S32  DISP_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion);
HI_BOOL DISP_IsOpened(HI_DRV_DISPLAY_E enDisp);
HI_BOOL DISP_IsFollowed(HI_DRV_DISPLAY_E enDisp);

HI_S32 DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave);
HI_S32 DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster);
HI_S32 DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo);

//isr
HI_S32 DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                             HI_DRV_DISP_CALLBACK_S *pstCB);
HI_S32 DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType, 
                             HI_DRV_DISP_CALLBACK_S *pstCB);


typedef struct tagDISP_PROC_INFO_S
{
    HI_BOOL bEnable;
    HI_BOOL bMaster;
    HI_BOOL bSlave;

    //about encoding format
    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;
    HI_DRV_DISP_FMT_E eFmt;
    HI_DRV_DISP_TIMING_S stTiming;
    HI_U32 u32Underflow;
    HI_U32 u32StartTime;

    HI_BOOL bCustAspectRatio;
    HI_U32 u32CustomAR_w;
    HI_U32 u32CustomAR_h;
    HI_U32 u32AR_w;
    HI_U32 u32AR_h;

    HI_RECT_S stAdjRect;

    HI_DRV_COLOR_SPACE_E eMixColorSpace;
    HI_DRV_COLOR_SPACE_E eDispColorSpace;

    // about color setting
    HI_DRV_DISP_COLOR_SETTING_S stColorSetting;
    HI_DRV_DISP_COLOR_S stBgColor;

    //interface
    HI_U32 u32IntfNumber;
    HI_DRV_DISP_INTF_S stIntf[HI_DRV_DISP_INTF_ID_MAX];

    //mirrorcast
    HI_HANDLE hCast;
}DISP_PROC_INFO_S;

HI_S32 DISP_GetProcInto(HI_DRV_DISPLAY_E enDisp, DISP_PROC_INFO_S *pstInfo);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISPLAY_H__  */


