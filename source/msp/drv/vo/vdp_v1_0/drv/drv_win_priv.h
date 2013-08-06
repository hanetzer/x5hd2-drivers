
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_priv.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_PRIV_H__
#define __DRV_WIN_PRIV_H__

#include "drv_disp_com.h"
#include "drv_win_hal.h"
#include "drv_win_prc.h"
#include "drv_win_buffer.h"
#include "drv_disp_buffer.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/* window state */
#define WIN_DEVICE_STATE_CLOSE   0
#define WIN_DEVICE_STATE_OPEN    1
#define WIN_DEVICE_STATE_SUSPEND 2

#define WIN_DISPLAY_MAX_NUMBER 16
#define WIN_VIRTAUL_MAX_NUMBER 16

#define WIN_INDEX_PREFIX      0x5A500000ul
#define WIN_INDEX_PREFIX_MASK 0xFFF00000ul
#define WIN_INDEX_DISPID_SHIFT_NUMBER 8
#define WIN_INDEX_DISPID_MASK 0x0000000Ful
#define WIN_INDEX_MASK        0x000000FFul

#define WIN_INDEX_SLAVE_PREFIX  0x00000080ul

#define WIN_FRAME_MIN_WIDTH  128
#define WIN_FRAME_MIN_HEIGHT 64
#define WIN_FRAME_MAX_WIDTH  1920
#define WIN_FRAME_MAX_HEIGHT 1920

#define WIN_MAX_ASPECT_RATIO 16

#define WIN_INRECT_MIN_WIDTH   128
#define WIN_INRECT_MAX_WIDTH   1920
#define WIN_INRECT_MIN_HEIGHT  64
#define WIN_INRECT_MAX_HEIGHT  1920

#define WIN_OUTRECT_MIN_WIDTH  128
#define WIN_OUTRECT_MAX_WIDTH  1920
#define WIN_OUTRECT_MIN_HEIGHT 64
#define WIN_OUTRECT_MAX_HEIGHT 1920

#define WIN_CROPRECT_MAX_OFFSET_TOP     128
#define WIN_CROPRECT_MAX_OFFSET_LEFT    128
#define WIN_CROPRECT_MAX_OFFSET_BOTTOM  128
#define WIN_CROPRECT_MAX_OFFSET_RIGHT   128


/* in 1/100 Hz */
#define WIN_MAX_FRAME_RATE   12000
#define WIN_TRANSFER_CODE_MAX_FRAME_RATE 3000

#define WIN_MAX_FRAME_PLAY_TIME  1

#define WIN_IN_FB_DEFAULT_NUMBER 16
#define WIN_USING_FB_MAX_NUMBER 2

#define WIN_USELESS_FRAME_MAX_NUMBER  16

typedef struct tagWIN_CONFIG_S
{
    HI_DRV_WIN_ATTR_S stAttr;

    HI_DRV_DISP_STEREO_E eDispMode;
    HI_BOOL bRightEyeFirst;

    HI_DRV_WIN_ATTR_S stAttrBuf;
    atomic_t bNewAttrFlag;

    /* for calc outrect when display format changes */
    HI_RECT_S stRefScreen;
    HI_RECT_S stRefOutRect;

    HI_DRV_WIN_SRC_INFO_S stSource;

    /* may change when window lives */
    HI_BOOL bQuickOutput;

    /*  */

}WIN_CONFIG_S;

typedef struct tagWIN_STATISTIC_S
{
    HI_U32 u32Reserved;

}WIN_STATISTIC_S;

typedef struct tagWIN_DEBUG_S
{
    HI_U32 u32Reserved;

}WIN_DEBUG_S;

typedef enum tagWIN_FRAME_TYPE_E
{
    WIN_FRAME_NORMAL = 0,
    WIN_FRAME_BLACK,
    WIN_FRAME_FREEZE,
    WIN_FRAME_TYPE_BUTT
}WIN_FRAME_TYPE_E;

typedef struct tagWIN_BUF_NODE_S
{
    HI_DRV_VIDEO_FRAME_S stFrame;
    HI_U32 u32BufId;
    HI_BOOL bIdle;
    WIN_FRAME_TYPE_E enType;
}WIN_BUF_NODE_S;

#define WIN_INVALID_BUFFER_INDEX 0xfffffffful
#define WIN_BLACK_FRAME_INDEX   0xf0000001ul


typedef struct tagWIN_BUFFER_S
{
    //BUF_POOL_S    stBP;
    WB_POOL_S stWinBP;
    
    HI_U32        u32UsingFbNum;
    WIN_BUF_NODE_S stUsingBufNode[WIN_USING_FB_MAX_NUMBER];
    HI_U32 u32DispIndex;
    HI_U32 u32CfgIndex;

    /* useless frame buffer */
    HI_DRV_VIDEO_FRAME_S stUselessFrame[WIN_USELESS_FRAME_MAX_NUMBER];
    HI_U32 u32ULSRdPtr;
    HI_U32 u32ULSWtPtr;
    HI_U32 u32ULSIn;
    HI_U32 u32ULSOut;

    HI_BOOL bWaitRelease;
    HI_DRV_VIDEO_FRAME_S stWaitReleaseFrame;

    HI_U32 u32UnderLoad;
}WIN_BUFFER_S;

typedef struct tagWIN_FREEZE_PRIV_S
{
    HI_DRV_WIN_SWITCH_E enFreezeMode;
    HI_U32 u32Reserve;
}WIN_FREEZE_PRIV_S;

typedef struct tagWIN_RESET_PRIV_S
{
    HI_DRV_WIN_SWITCH_E enResetMode;
    HI_U32 u32Reserve;
}WIN_RESET_PRIV_S;

typedef enum tagWIN_STATE_E
{
    WIN_STATE_WORK = 0,
    WIN_STATE_PAUSE,
    WIN_STATE_RESUME,
    WIN_STATE_FREEZE,
    WIN_STATE_UNFREEZE,
    WIN_STATE_BUTT
}WIN_STATE_E;

typedef struct tagWIN_DELAY_INFO_S
{
    HI_U32 u32DispRate;  /* in 1/100 Hz */
    HI_U32 T;  /* in ms */

//    volatile HI_U32 u32FrameNumber;
    volatile HI_U32 u32DisplayTime;
    volatile HI_U32 u32CfgTime;
    HI_BOOL bInterlace;
    volatile HI_BOOL bTBMatch;  /* for interlace frame display on interlace timing */
}WIN_DELAY_INFO_S;

typedef struct tagWINDOW_S
{
    HI_U32 u32Index;

    /* state */
    HI_BOOL bEnable;
    HI_BOOL bMasked;

    HI_DRV_DISPLAY_E  enDisp;
    HI_DRV_WIN_TYPE_E enType;
    HI_U32  u32VideoLayer;

    /* window config */
    WIN_CONFIG_S stCfg;
    HI_BOOL bDispInfoChange;

    /* private attribute */
    HI_DRV_WIN_ATTR_S stUsingAttr;

    volatile HI_BOOL bUpState;
    volatile WIN_STATE_E enState;
    volatile WIN_STATE_E enStateNew;

    // reset flag
    volatile HI_BOOL bReset;
    WIN_RESET_PRIV_S stRst;

    // freeze flag
    WIN_FREEZE_PRIV_S stFrz;

    // quickout flag
    HI_BOOL bQuickMode;

    // stepmode flag
    HI_BOOL bStepMode;

    /* play info */
    volatile WIN_DELAY_INFO_S stDelayInfo;

    /* statistic info */
    WIN_STATISTIC_S stStatistic;

    /* debug info */
    HI_BOOL     bDebugEn;
    WIN_DEBUG_S stDebug;

    /*  buffer */
    WIN_BUFFER_S stBuffer;
    HI_BOOL bConfigedBlackFrame;

    /* slave window for HD&SD display the same content at the same time */
    HI_HANDLE hSlvWin;
    HI_HANDLE pstMstWin;

    HI_U32 u32TBTestCount;
    HI_U32 u32TBNotMatchCount;

    /* video surface function */
    VIDEO_LAYER_FUNCTIONG_S stVLayerFunc;
}WINDOW_S;


typedef struct tagDISPLAY_WINDOW_S
{
    //WINDOW_S *pstWinList;
    WINDOW_S *pstWinArray[HI_DRV_DISPLAY_BUTT][WINDOW_MAX_NUMBER];
    HI_U32    u32WinNumber;

    VIDEO_LAYER_FUNCTIONG_S stVSurfFunc;
    
}DISPLAY_WINDOW_S;


typedef struct tagVIRTUAL_WINDOW_S
{
    HI_BOOL bOpen;
    WINDOW_S *stWinArray[WINDOW_MAX_NUMBER];
    
}VIRTUAL_WINDOW_S;



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_VO_PRIV_H__  */










