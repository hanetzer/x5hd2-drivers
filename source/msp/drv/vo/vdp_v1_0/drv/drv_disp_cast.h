
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_cast.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_CAST_H__
#define __DRV_DISP_CAST_H__

#include "drv_disp_com.h"
#include "drv_disp_hal.h"
#include "drv_disp_buffer.h"
#include "drv_disp_isr.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define DISP_CAST_MIN_W 320
#define DISP_CAST_MAX_W 1920
#define DISP_CAST_MIN_H 240
#define DISP_CAST_MAX_H 1080

#define DISP_CAST_BUFFER_MIN_NUMBER 3
#define DISP_CAST_BUFFER_MAX_NUMBER 16

/* in ms */
#define DISP_CAST_MAX_FRAME_RATE 3000


typedef struct tagDISP_CAST_ATTR_S
{
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */

	HI_RECT_S stIn;
	HI_BOOL bInterlace;
	HI_U32 u32InRate;
    HI_DRV_COLOR_SPACE_E eInColorSpace;
	
	HI_RECT_S stOut;
	HI_U32 u32OutRate;
    HI_DRV_COLOR_SPACE_E eOutColorSpace;

	// store output informaiton
	HI_DRV_VIDEO_FRAME_S stFrameDemo;
}DISP_CAST_ATTR_S;

typedef struct tagDISP_CAST_PRIV_FRAME_S
{
    HI_DRV_VIDEO_PRIVATE_S stPrivInfo;

	HI_HANDLE hCast;
	HI_U32    u32Pts0;  /* create PTS */
	HI_U32    u32Pts1;  /* acquire PTS */

}DISP_CAST_PRIV_FRAME_S;



typedef struct tagDISP_CAST_S
{
    //state
    HI_BOOL bOpen;
    HI_BOOL bEnable;
    HI_BOOL bMasked;

    // cfg
    HI_DRV_DISP_CAST_CFG_S stConfig;

    // disp info
    HI_DRV_DISPLAY_E eDisp;
    //HI_BOOL bDispSet;
    //HI_BOOL bDispUpdate;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_BOOL bToGetDispInfo;

    // private attr
    DISP_WBC_E eWBC;
    //HI_BOOL bAttrUpdate;
    DISP_CAST_ATTR_S stAttr;

    HI_U32 u32Periods;
    HI_U32 u32TaskCount;

    //mirrorcast
    //DISP_MIRACAST_S stMrCt;

    //algrithm operation

    // buffer
    BUF_POOL_S stBP;
    HI_U32 u32FrameCnt;
    HI_U32 u32LastCfgBufId;
    HI_U32 u32LastFrameBufId;

    //component operation
    DISP_INTF_OPERATION_S stIntfOpt;

}DISP_CAST_S;



HI_S32 DISP_CastCreate(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast);
HI_S32 DISP_CastDestroy(HI_HANDLE hCast);
HI_S32 DISP_CastSetEnable(HI_HANDLE hCast, HI_BOOL bEnable);
HI_S32 DISP_CastGetEnable(HI_HANDLE hCast, HI_BOOL *pbEnable);
HI_S32 DISP_CastAcquireFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 DISP_CastReleaseFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);


HI_VOID DISP_CastCBSetDispMode(HI_HANDLE hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID DISP_CastCBWork(HI_HANDLE hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_X_H__  */










