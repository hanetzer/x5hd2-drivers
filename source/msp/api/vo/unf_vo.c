/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/16
  Description   :
  History       :
  1.Date        : 2009/12/16
    Author      : w58735
    Modification: Created file

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_mpi_win.h"
//#include "hi_unf_vi.h"
#include "hi_error_mpi.h"
#include "mpi_disp_tran.h"
#include "hi_mpi_avplay.h"


HI_S32 HI_UNF_VO_Init(HI_UNF_VO_DEV_MODE_E enDevMode)
{
    if (HI_UNF_VO_DEV_MODE_BUTT <= enDevMode)
    {
        HI_FATAL_WIN("Invalid mode!\n");
        return HI_FAILURE;
    }

    return HI_MPI_WIN_Init();
}

HI_S32 HI_UNF_VO_DeInit(HI_VOID)
{
    return HI_MPI_WIN_DeInit();
}

HI_S32 VO_ConvertWinAttrToMPI(HI_UNF_WINDOW_ATTR_S *pUnfAttr, HI_DRV_WIN_ATTR_S *pstMpiAttr)
{
    if (!pstMpiAttr)
    {
        HI_ERR_WIN("para pstMpiAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    if (!pUnfAttr)
    {
        HI_ERR_WIN("para pUnfAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    /* conver unf parameter to driver parameters */
    memset(pstMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    Transfer_DispID(&pUnfAttr->enDisp, &pstMpiAttr->enDisp, HI_TRUE);

    pstMpiAttr->bVirtual = pUnfAttr->bVirtual;

    if (pUnfAttr->stWinAspectAttr.bUserDefAspectRatio)
    {
        pstMpiAttr->stCustmAR.u8ARw = pUnfAttr->stWinAspectAttr.u32UserAspectWidth;
        pstMpiAttr->stCustmAR.u8ARh = pUnfAttr->stWinAspectAttr.u32UserAspectHeight;
    }
    else
    {
        pstMpiAttr->stCustmAR.u8ARw = 0;
        pstMpiAttr->stCustmAR.u8ARh = 0;    
    }
    Transfe_ARConvert(&pUnfAttr->stWinAspectAttr.enAspectCvrs, &pstMpiAttr->enARCvrs, HI_TRUE);
    pstMpiAttr->enARCvrs = (HI_DRV_ASP_RAT_MODE_E)(pUnfAttr->stWinAspectAttr.enAspectCvrs);

    if (pUnfAttr->bUseCropRect)
    {
        pstMpiAttr->stCropRect.u32LeftOffset = pUnfAttr->stCropRect.u32LeftOffset;
        pstMpiAttr->stCropRect.u32TopOffset  = pUnfAttr->stCropRect.u32TopOffset;
        pstMpiAttr->stCropRect.u32RightOffset  = pUnfAttr->stCropRect.u32RightOffset;
        pstMpiAttr->stCropRect.u32BottomOffset = pUnfAttr->stCropRect.u32BottomOffset;
    }

    pstMpiAttr->stInRect = pUnfAttr->stInputRect;
    pstMpiAttr->stOutRect = pUnfAttr->stOutputRect;

    if (pUnfAttr->bVirtual)
    {
        Transfer_VideoFormat(&pUnfAttr->enVideoFormat, &pstMpiAttr->enDataFormat, HI_TRUE);
    }

    return HI_SUCCESS;
}

HI_S32 VO_ConvertWinAttrToUNF(HI_DRV_WIN_ATTR_S *pstMpiAttr, HI_UNF_WINDOW_ATTR_S *pUnfAttr)
{
    if (!pstMpiAttr)
    {
        HI_ERR_WIN("para pstMpiAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    if (!pUnfAttr)
    {
        HI_ERR_WIN("para pUnfAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    /* conver unf parameter to driver parameters */
    memset(pUnfAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    Transfer_DispID(&pUnfAttr->enDisp, &pstMpiAttr->enDisp, HI_FALSE);

    pUnfAttr->bVirtual = pstMpiAttr->bVirtual;

    if (pstMpiAttr->stCustmAR.u8ARw && pstMpiAttr->stCustmAR.u8ARh)
    {
        pUnfAttr->stWinAspectAttr.bUserDefAspectRatio = HI_TRUE;
    }

    pUnfAttr->stWinAspectAttr.u32UserAspectWidth  = pstMpiAttr->stCustmAR.u8ARw;
    pUnfAttr->stWinAspectAttr.u32UserAspectHeight = pstMpiAttr->stCustmAR.u8ARh;

    Transfe_ARConvert(&pUnfAttr->stWinAspectAttr.enAspectCvrs, &pstMpiAttr->enARCvrs, HI_FALSE);
    pUnfAttr->stWinAspectAttr.enAspectCvrs = (HI_UNF_VO_ASPECT_CVRS_E)pstMpiAttr->enARCvrs;

    if (   pstMpiAttr->stCropRect.u32TopOffset || pstMpiAttr->stCropRect.u32LeftOffset
        || pstMpiAttr->stCropRect.u32BottomOffset || pstMpiAttr->stCropRect.u32RightOffset
        )
    {
        pUnfAttr->bUseCropRect = HI_TRUE;
    }

    pUnfAttr->stCropRect.u32LeftOffset   = pstMpiAttr->stCropRect.u32LeftOffset;
    pUnfAttr->stCropRect.u32TopOffset    = pstMpiAttr->stCropRect.u32TopOffset;
    pUnfAttr->stCropRect.u32RightOffset  = pstMpiAttr->stCropRect.u32RightOffset;
    pUnfAttr->stCropRect.u32BottomOffset = pstMpiAttr->stCropRect.u32BottomOffset;

    pUnfAttr->stInputRect = pstMpiAttr->stInRect;

    pUnfAttr->stOutputRect = pstMpiAttr->stOutRect;

    if (pstMpiAttr->bVirtual)
    {
        Transfer_VideoFormat(&pUnfAttr->enVideoFormat, &pstMpiAttr->enDataFormat, HI_FALSE);
    }

    return HI_SUCCESS;
}


HI_S32 HI_UNF_VO_CreateWindow(const HI_UNF_WINDOW_ATTR_S *pWinAttr, HI_HANDLE *phWindow)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret;
    
    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!phWindow)
    {
        HI_ERR_WIN("para phWindow is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    VO_ConvertWinAttrToMPI((HI_UNF_WINDOW_ATTR_S *)pWinAttr, &stMpiAttr);

    s32Ret = HI_MPI_WIN_Create(&stMpiAttr, phWindow);

    return s32Ret;
}

HI_S32 HI_UNF_VO_DestroyWindow(HI_HANDLE hWindow)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_Destroy(hWindow);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetWindowEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_SetEnable(hWindow, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_GetEnable(hWindow, pbEnable);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetWindowAttr(HI_HANDLE hWindow, const HI_UNF_WINDOW_ATTR_S *pWinAttr)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    VO_ConvertWinAttrToMPI((HI_UNF_WINDOW_ATTR_S *)pWinAttr, &stMpiAttr);

    s32Ret = HI_MPI_WIN_SetAttr(hWindow, &stMpiAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowAttr(HI_HANDLE hWindow, HI_UNF_WINDOW_ATTR_S *pWinAttr)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret;
    
    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = HI_MPI_WIN_GetAttr(hWindow, &stMpiAttr);
    if (!s32Ret)
    {
        VO_ConvertWinAttrToUNF(&stMpiAttr, pWinAttr);
    }

    return s32Ret;
}


HI_S32 HI_UNF_VO_AcquireFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo, HI_U32 u32TimeoutMs)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret;
    
    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para pstFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = HI_MPI_WIN_AcquireFrame(hWindow, &stMpi);
    if (!s32Ret)
    {
        Transfer_Frame(pstFrameinfo, &stMpi, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_ReleaseFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret;
    
    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para pstFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    Transfer_Frame(pstFrameinfo, &stMpi, HI_TRUE);
    s32Ret = HI_MPI_WIN_ReleaseFrame(hWindow, &stMpi);

    return s32Ret;
}


HI_S32 HI_UNF_VO_SetWindowZorder(HI_HANDLE hWindow, HI_LAYER_ZORDER_E enZFlag)
{
    HI_DRV_DISP_ZORDER_E enZorder;
    HI_S32 s32Ret;

    if (enZFlag >= HI_LAYER_ZORDER_BUTT)
    {
        HI_ERR_WIN("Invalid zorder parameter!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    Transfe_ZOrder(&enZFlag, &enZorder, HI_TRUE);

    s32Ret = HI_MPI_WIN_SetZorder(hWindow, enZorder);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowZorder(HI_HANDLE hWindow, HI_U32 *pu32Zorder)
{
    HI_S32 s32Ret;
    if (!pu32Zorder)
    {
        HI_ERR_WIN("para pu32Zorder is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetZorder(hWindow, pu32Zorder);

    return s32Ret;
}

HI_S32 HI_UNF_VO_AttachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_AVPLAY_AttachWindow(hSrc, hWindow);
    if (s32Ret)
    {
        HI_ERR_WIN("HI_MPI_AVPLAY_AttachWindow failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VO_DetachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MPI_AVPLAY_DetachWindow(hSrc, hWindow);
	if (s32Ret)
	{
		HI_ERR_WIN("HI_MPI_AVPLAY_DettachWindow failed!\n");
		return s32Ret;
	}

    return HI_SUCCESS;
}


HI_S32 HI_UNF_VO_FreezeWindow(HI_HANDLE hWindow, HI_BOOL bEnable, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode)
{
    HI_DRV_WIN_SWITCH_E eFrzMode;
    HI_S32 s32Ret;

    Transfe_SwitchMode(&enWinFreezeMode, &eFrzMode, HI_TRUE);
    s32Ret = HI_MPI_WIN_Freeze(hWindow, bEnable, eFrzMode);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetWindowFieldMode(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32 s32Ret = HI_SUCCESS;

    //s32Ret = HI_MPI_WIN_SetFieldMode(hWindow, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_VO_ResetWindow(HI_HANDLE hWindow, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode)
{
    HI_DRV_WIN_SWITCH_E eRstMode;
    HI_S32 s32Ret;

    Transfe_SwitchMode(&enWinFreezeMode, &eRstMode, HI_TRUE);

    s32Ret = HI_MPI_WIN_Reset(hWindow, eRstMode);

    return s32Ret;
}

HI_S32 HI_UNF_VO_AttachExternBuffer(HI_HANDLE hWindow,HI_UNF_BUFFER_ATTR_S* pstBufAttr)
{
    HI_DRV_VIDEO_BUFFER_POOL_S stBufPool;
    HI_S32 s32Ret;
    if (!pstBufAttr)
    {
        HI_ERR_WIN("para pstBufAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    Transfer_BufferPool(pstBufAttr, &stBufPool, HI_TRUE);

    s32Ret = HI_MPI_WIN_SetExtBuffer(hWindow, &stBufPool);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetQuickOutputEnable(HI_HANDLE hWindow, HI_BOOL bQuickOutputEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_SetQuickOutput(hWindow, bQuickOutputEnable);
    return s32Ret;
}


HI_S32 HI_UNF_VO_CapturePicture(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture)
{
//    HI_DRV_VIDEO_FRAME_S stMpi;
    //HI_S32 s32Ret;
    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    //s32Ret = HI_MPI_WIN_CapturePicture(hWindow, &stMpi);
#if 0
    if (s32Ret)
    {
        Transfer_Frame(pstCapPicture, &stMpi, HI_FALSE);
    }
#endif
    return HI_ERR_VO_MV300_UNSUPPORT;
}

HI_S32 HI_UNF_VO_CapturePictureRelease(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstCapPicture)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret;
    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    Transfer_Frame(pstCapPicture, &stMpi, HI_TRUE);
    s32Ret = HI_MPI_WIN_CapturePictureRelease(hWindow, &stMpi);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetRotation(HI_HANDLE hWindow, HI_UNF_VO_ROTATION_E enRotation)
{
    HI_DRV_ROT_ANGLE_E eRot;
    HI_S32 s32Ret;

    Transfe_Rotate(&enRotation, &eRot, HI_TRUE);
    s32Ret = HI_MPI_WIN_SetRotation(hWindow, eRot);
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetRotation(HI_HANDLE hWindow, HI_UNF_VO_ROTATION_E *penRotation)
{
    HI_DRV_ROT_ANGLE_E eRot;
    HI_S32 s32Ret;
    if (!penRotation)
    {
        HI_ERR_WIN("para penRotation is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetRotation(hWindow, &eRot);
    if(!s32Ret)
    {
        Transfe_Rotate(penRotation, &eRot, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetFlip(HI_HANDLE hWindow, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_WIN_SetFlip(hWindow, bHoriFlip, bVertFlip);
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetFlip(HI_HANDLE hWindow, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip)
{
    HI_S32 s32Ret;
    if (!pbVertFlip)
    {
        HI_ERR_WIN("para pbVertFlip is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetFlip(hWindow, pbHoriFlip, pbVertFlip);
    return s32Ret;
}

HI_S32 HI_UNF_VO_SetStereoDetpth(HI_HANDLE hWindow, HI_S32 s32Depth)
{
    return HI_FAILURE;
}



