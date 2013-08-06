
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_priv.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_PRIV_H__
#define __DRV_DISP_PRIV_H__

#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "drv_disp_hal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define DISP_DEVICE_STATE_CLOSE   0
#define DISP_DEVICE_STATE_OPEN    1
#define DISP_DEVICE_STATE_SUSPEND 2

#define DISP_SET_TIMEOUT_THRESHOLD  10

/* default back ground color */
#define DISP_DEFAULT_COLOR_RED   0
#define DISP_DEFAULT_COLOR_GREEN 0
#define DISP_DEFAULT_COLOR_BLUE  0

/* default csc parameters value */
#define DISP_DEFAULT_BRIGHT       50
#define DISP_DEFAULT_HUE          50
#define DISP_DEFAULT_SATURATION   50
#define DISP_DEFAULT_CONTRAST     50
#define DISP_DEFAULT_KR           50
#define DISP_DEFAULT_KG           50
#define DISP_DEFAULT_KB           50

#define DISP_DEFAULT_SATURATION_OFFSET 0
#define DISP_DEFAULT_BRIGHT_OFFSET     0
#define DISP_DEFAULT_CONTRAST_OFFSET   0
#define DISP_DEFAULT_HUE_OFFSET        0
#define DISP_DEFAULT_KR_OFFSET         0
#define DISP_DEFAULT_KG_OFFSET         0
#define DISP_DEFAULT_KB_OFFSET         0


typedef enum tagDISP_PRIV_STATE_E
{
    DISP_PRIV_STATE_DISABLE = 0,
    DISP_PRIV_STATE_WILL_ENABLE,
    DISP_PRIV_STATE_ENABLE,
    DISP_PRIV_STATE_WILL_DISABLE,
    DISP_PRIV_STATE_BUTT
}DISP_PRIV_STATE_E;


typedef struct tagDISP_SETTING_S
{
    HI_U32  u32Version;
    HI_U32  u32BootVersion;
    HI_BOOL bSelfStart;
    HI_BOOL bGetPDMParam;
    
    /* output format */
    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;
    HI_DRV_DISP_FMT_E enFormat;
    HI_BOOL bFmtChanged;

    HI_DRV_DISP_TIMING_S stCustomTimg;
    HI_BOOL bCustomTimingIsSet;
    HI_BOOL bCustomTimingChange;

    /* about color */
    HI_DRV_DISP_COLOR_SETTING_S stColor;

    /* background color */
    HI_DRV_DISP_COLOR_S stBgColor;

    //HI_BOOL bCGMSAEnable;
    //HI_DRV_DISP_CGMSA_TYPE_E  eCGMSAType;
    //HI_DRV_DISP_CGMSA_MODE_E  eCGMSAMode;

    //HI_DRV_DISP_MACROVISION_E eMcvnType;

    /* interface setting */
    HI_U32 u32IntfNumber;
    //HI_DRV_DISP_INTF_S stIntf[HI_DRV_DISP_INTF_ID_MAX];
    DISP_INTF_S stIntf[HI_DRV_DISP_INTF_ID_MAX];

    HI_U32 u32LayerNumber;
    HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */

    /* about sink display screen */
    HI_BOOL bAdjRect;
    HI_RECT_S stRefAdjRect;
    HI_BOOL bRefScreenIsSet;
    HI_RECT_S stRefScreen;
    HI_RECT_S stUsingAdjRect;

    HI_BOOL bCustomRatio;
    HI_U32 u32CustomRatioWidth;
    HI_U32 u32CustomRatioHeight;

    HI_U32  u32Reseve;
    HI_VOID *pRevData;
}DISP_SETTING_S;

typedef struct tagDISP_S
{
    HI_DRV_DISPLAY_E enDisp;

    //state
    HI_BOOL bSupport;
    HI_BOOL bOpen;
    HI_BOOL bEnable;
    HI_BOOL bStateBackup;
        
    /* for attach display */
    HI_BOOL bIsMaster;
    HI_BOOL bIsSlave;
    HI_DRV_DISPLAY_E enAttachedDisp;

    DISP_SETTING_S stSetting;
    HI_BOOL bDispSettingChange;
    HI_BOOL bDispAreaChange;

    volatile DISP_PRIV_STATE_E eState;
    HI_U32 u32Underflow;
    HI_U32 u32StartTime;

    // for other module get
    //HI_BOOL bDispInfoValid;
    HI_DISP_DISPLAY_INFO_S stDispInfo;


    //vbi

    //isr

    //mirrorcast
    HI_HANDLE hCast;

    //algrithm operation
    //HI_HANDLE hAlgOpt;

    //component operation
    DISP_INTF_OPERATION_S *pstIntfOpt;

}DISP_S;

typedef struct tagDISP_ATTACH_ID_S
{
    HI_DRV_DISPLAY_E eMaster;
    HI_DRV_DISPLAY_E eSlave;
}DISP_ATTACH_ID_S;

typedef struct tagDISP_DEV_S
{
    HI_BOOL bHwReseted;
    DISP_S  stDisp[HI_DRV_DISPLAY_BUTT];

    HI_BOOL bAttachEnable;
    DISP_ATTACH_ID_S stAttchDisp;
    DISP_INTF_OPERATION_S stIntfOpt;
}DISP_DEV_S;



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_PRIV_H__  */

