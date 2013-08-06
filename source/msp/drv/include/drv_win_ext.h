/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_ext.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_EXT_H__
#define __DRV_WIN_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_drv_win.h"

typedef struct
{
    HI_HANDLE hWin;
    HI_HANDLE hSlvWin;
}WIN_PRIV_STATE_S;


typedef struct
{
    HI_S32 (*FN_GetPlayInfo)(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pstInfo);
    HI_S32 (*FN_GetInfo)(HI_HANDLE hWindow, HI_DRV_WIN_INFO_S * pstInfo);

    HI_S32 (*FN_QueueFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *Frame);
    HI_S32 (*FN_QueueULSFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *Frame);
    HI_S32 (*FN_DequeueFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *Frame);

    HI_S32 (*FN_ReleaseFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *Frame);
    HI_S32 (*FN_AcquireFrame)(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *Frame);

}WIN_EXPORT_FUNC_S;

HI_S32  DRV_WIN_Register(HI_VOID);
HI_VOID DRV_WIN_UnRegister(HI_VOID);

HI_S32 DRV_WIN_Init(HI_VOID);
HI_S32 DRV_WIN_DeInit(HI_VOID);

HI_S32 DRV_WIN_Create(const HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow);
HI_S32 DRV_WIN_Destroy(HI_HANDLE hWindow);
HI_S32 DRV_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable);
HI_S32 DRV_WIN_SetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_INFO_S *pstSrc);

HI_S32 DRV_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enMode);
HI_S32 DRV_WIN_GetPlayInfo(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pstInfo);

HI_S32 DRV_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 DRV_WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E ZFlag);

HI_S32 DRV_WIN_GetInfo(HI_HANDLE hWindow, HI_DRV_WIN_INFO_S *pInfo);

HI_S32 DRV_WIN_QFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 DRV_WIN_DQFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);

HI_S32 DRV_WIN_AcquireFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);
HI_S32 DRV_WIN_ReleaseFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /* __VO_EXT_H__ */

