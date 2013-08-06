/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_ext.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __DRV_DISP_EXT_H__
#define __DRV_DISP_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"


typedef struct hiDRV_DISP_STATE_S
{
    HI_BOOL bDispOpen[HI_DRV_DISPLAY_BUTT];
    HI_HANDLE hCastHandle[HI_DRV_DISPLAY_BUTT];
}DRV_DISP_STATE_S;

typedef struct hiDRV_DISP_GLOBAL_STATE_S
{      
    HI_U32 DispOpenNum[HI_DRV_DISPLAY_BUTT];
}DRV_DISP_GLOBAL_STATE_S;

typedef struct
{
    HI_S32  (* DRV_DISP_Init)(HI_VOID);
    HI_S32  (* DRV_DISP_DeInit)(HI_VOID);
    HI_S32  (* DRV_DISP_Attach)(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
    HI_S32  (* DRV_DISP_Detach)(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
    HI_S32  (* DRV_DISP_SetFormat)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEnFormat);
    HI_S32  (* DRV_DISP_GetFormat)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penEnFormat);
    HI_S32  (* DRV_DISP_SetTiming)(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming);
    HI_S32  (* DRV_DISP_AddIntf)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
    HI_S32  (* DRV_DISP_Open)(HI_DRV_DISPLAY_E enDisp);
    HI_S32  (* DRV_DISP_Close)(HI_DRV_DISPLAY_E enDisp);
    HI_S32  (* DRV_DISP_SetEnable)(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
    //HI_S32  (* DRV_DISP_GetEnable)(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);
    HI_S32  (* DRV_DISP_SetBgColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
    //HI_S32  (* DRV_DISP_GetBgColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
    HI_S32  (* DRV_DISP_SetColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
    HI_S32  (* DRV_DISP_GetColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
    HI_S32  (* DRV_DISP_SetScreen)(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect);
    //HI_S32  (* DRV_DISP_GetScreen)(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect);

    HI_S32  (* DRV_DISP_SetAspectRatio)(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v);
    //HI_S32  (* DRV_DISP_GetAspectRatio)(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v);
    HI_S32  (* DRV_DISP_SetLayerZorder)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag);
    //HI_S32  (* DRV_DISP_GetLayerZorder)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

    HI_S32  (* DRV_DISP_GetInitFlag)(HI_BOOL *pbInited);
    HI_S32  (* DRV_DISP_GetVersion)(HI_DRV_DISP_VERSION_S *pstVersion);
    HI_BOOL (* DRV_DISP_IsOpened)(HI_DRV_DISPLAY_E enDisp);
    HI_S32  (* DRV_DISP_GetSlave)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave);
    HI_S32  (* DRV_DISP_GetMaster)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster);
    HI_S32  (* DRV_DISP_GetDisplayInfo)(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo);

    HI_S32 (* FN_DISP_Ioctl)(HI_U32 cmd, HI_VOID *arg);
    HI_S32 (* FN_DISP_RegCallback)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                                   HI_DRV_DISP_CALLBACK_S *pstCallback);

    HI_S32 (* FN_DISP_UnRegCallback)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                                     HI_DRV_DISP_CALLBACK_S *pstCallback);

}DISP_EXPORT_FUNC_S;

HI_S32  DRV_DISP_Register(HI_VOID);
HI_VOID DRV_DISP_UnRegister(HI_VOID);

HI_S32  VDP_DRV_ModInit(HI_VOID);
HI_VOID VDP_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_DISP_EXT_H__ */

