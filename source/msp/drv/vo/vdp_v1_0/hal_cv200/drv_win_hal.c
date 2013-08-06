
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_hal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_win_hal.h"
#include "drv_disp_ua.h"
#include "drv_disp_da.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 s_bVideoSurfaceFlag = -1;
static VIDEO_LAYER_FUNCTIONG_S s_stVieoLayerFunc;
static VIDEO_LAYER_S s_stVideoLayer[DEF_VIDEO_LAYER_MAX_NUMBER];
static VIDEO_LAYER_CAPABILITY_S s_stVideoLayerCap[DEF_VIDEO_LAYER_MAX_NUMBER];


HI_S32 GetCapability(HI_U32 eLayer, VIDEO_LAYER_CAPABILITY_S *pstSurf)
{
    if ( (eLayer < DEF_VIDEO_LAYER_MAX_NUMBER) && pstSurf)
    {
        *pstSurf = s_stVideoLayerCap[eLayer];
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AcquireLayer(HI_U32 eLayer)
{
    if (   (!s_stVideoLayerCap[eLayer].bSupport) 
        || (s_stVideoLayer[eLayer].bWorking)
        )
    {
        return HI_FAILURE;
    }

    s_stVideoLayer[eLayer].bWorking = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 AcquireLayerByDisplay(HI_DRV_DISPLAY_E eDisp, HI_U32 *peLayer)
{
    HI_U32 eId;
    
    if (HI_DRV_DISPLAY_1 == eDisp)
    {
        for(eId = VDP_RM_LAYER_VID0; eId < VDP_RM_LAYER_VID2; eId++)
        {
            if( s_stVideoLayerCap[eId].bSupport && !s_stVideoLayer[eId].bWorking)
            {
                *peLayer = eId;
                s_stVideoLayer[eId].bWorking = HI_TRUE;
                //printk(">>>>>>>>>>>  VIDEO LAYER %d Start....\n", eId);
                return HI_SUCCESS;
            }
        }
    }
    
    if (HI_DRV_DISPLAY_0 == eDisp)
    {
        for(eId = VDP_RM_LAYER_VID3; eId >= VDP_RM_LAYER_VID2; eId--)
        {
            if( s_stVideoLayerCap[eId].bSupport && !s_stVideoLayer[eId].bWorking)
            {
                *peLayer = eId;
                s_stVideoLayer[eId].bWorking = HI_TRUE;
                //printk(">>>>>>>>>>>  VIDEO LAYER %d Start....\n", eId);
                return HI_SUCCESS;
            }
        }
    }

    return HI_FAILURE;
}


HI_S32 ReleaseLayer(HI_U32 eLayer)
{
    if (!s_stVideoLayerCap[eLayer].bSupport) 
    {
        return HI_FAILURE;
    }

    s_stVideoLayer[eLayer].bWorking = HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 SetEnable(HI_U32 eLayer, HI_BOOL bEnable)
{
    VDP_VID_SetLayerEnable(eLayer, bEnable);
    return HI_SUCCESS;
}


HI_S32 Update(HI_U32 eLayer)
{
    VDP_VID_SetRegUp(eLayer);
    return HI_SUCCESS;
}



HI_S32 SetDefault(HI_U32 eLayer)
{
    HI_U32 uHid;
    VDP_BKG_S vidBkg;

    uHid = eLayer;
    
    VDP_VID_SetLayerEnable(uHid, HI_FALSE);
    VDP_VID_SetInDataFmt(uHid, VDP_VID_IFMT_SP_420);
    VDP_VID_SetReadMode(uHid, VDP_RMODE_PROGRESSIVE, VDP_RMODE_PROGRESSIVE);
    VDP_VID_SetMuteEnable(uHid, HI_FALSE);
    VDP_VID_SetFlipEnable(uHid, HI_FALSE);

    VDP_VID_SetIfirMode(uHid, VDP_IFIRMODE_COPY);

    VDP_VID_SetLayerGalpha(uHid, 0xff);

    vidBkg.u32BkgU = 0x11;
    vidBkg.u32BkgV = 0x11;
    vidBkg.u32BkgY = 0x11;
    vidBkg.u32BkgA = 0xff;
    VDP_VID_SetLayerBkg(uHid, vidBkg);

    return HI_SUCCESS;
}

HI_S32 SetAllLayerDefault(HI_VOID)
{
    SetDefault(VDP_RM_LAYER_VID0);
    SetDefault(VDP_RM_LAYER_VID1);
    //SetDefault(VDP_RM_LAYER_VID2);
    SetDefault(VDP_RM_LAYER_VID3);

    return HI_SUCCESS;
}


HI_S32 SetDispMode(HI_U32 u32id, HI_DRV_DISP_STEREO_MODE_E eMode)
{
//printk("=================eLayer=%d, video SetDispMode = %d\n", eLayer,eMode);
    switch(eMode)
    {
        case HI_DRV_DISP_STEREO_FRAME_PACKING:
            VDP_VID_SetDispMode(u32id, VDP_DISP_MODE_FP);
            break;
        case HI_DRV_DISP_STEREO_SBS_HALF:
            VDP_VID_SetDispMode(u32id, VDP_DISP_MODE_SBS);
            break;
        case HI_DRV_DISP_STEREO_TAB:
            VDP_VID_SetDispMode(u32id, VDP_DISP_MODE_TAB); //todo
            break;
        case HI_DRV_DISP_STEREO_NONE:
        default:
            VDP_VID_SetDispMode(u32id, VDP_DISP_MODE_2D);
            break;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_HAL_SetZme(HI_U32 eLayer, HI_RECT_S *in, HI_RECT_S *disp, HI_RECT_S *video)
{
//    DISP_UA_FUNCTION_S *pfZme;

    return HI_SUCCESS;
}

HI_S32 SetIORect(HI_U32 u32LayerId, HI_RECT_S *in, HI_RECT_S *disp, HI_RECT_S *video)
{
    VDP_RECT_S stFrmVRect;
    VDP_RECT_S stVRect;
    HI_U32 u32Ratio;


    stFrmVRect.u32X = 0;
    stFrmVRect.u32Y = 0;
    stFrmVRect.u32Wth = in->s32Width;
    stFrmVRect.u32Hgt = in->s32Height;

    stVRect.u32X = video->s32X;
    stVRect.u32Y = video->s32Y;
    stVRect.u32Wth = video->s32Width;
    stVRect.u32Hgt = video->s32Height;
    
    //VDP_VID_ZME_DEFAULT();

    VDP_VID_SetInReso(u32LayerId, stFrmVRect);
    VDP_VID_SetOutReso(u32LayerId,  stVRect);
    VDP_VID_SetVideoPos(u32LayerId, stVRect);
    VDP_VID_SetDispPos(u32LayerId,  stVRect);

    u32Ratio = (stFrmVRect.u32Wth << 20) / stVRect.u32Wth;
    VDP_VID_SetZmeHorRatio(u32LayerId, u32Ratio);

    u32Ratio = (stFrmVRect.u32Hgt << 12) / stVRect.u32Hgt;
    VDP_VID_SetZmeVerRatio(u32LayerId, u32Ratio);

    VDP_VID_SetZmeEnable(u32LayerId, VDP_ZME_MODE_HOR, 1);
    VDP_VID_SetZmeEnable(u32LayerId, VDP_ZME_MODE_VER, 1);

    return HI_SUCCESS;
}


HI_S32 WinHalSetColor_MPW(HI_U32 u32LayerId, HI_DRV_DISP_COLOR_SETTING_S *pstColor)
{
    ALG_CSC_DRV_PARA_S stIn;
    ALG_CSC_RTL_PARA_S stOut;
    VDP_CSC_DC_COEF_S stCscCoef;
    VDP_CSC_COEF_S stCscCoef2;
    DISP_UA_FUNCTION_S *pstuUA;

    pstuUA = DISP_UA_GetFunction();
    if (!pstuUA)
    {
        return HI_FAILURE;
    }

    stIn.eInputCS  = pstColor->enInCS;
    stIn.eOutputCS = pstColor->enOutCS;
    //stIn.eInputCS  = HI_DRV_CS_BT709_YUV_LIMITED;
    //stIn.eOutputCS = HI_DRV_CS_BT709_YUV_LIMITED;

    stIn.bIsBGRIn = HI_FALSE;

    stIn.u32Bright  = pstColor->u32Bright;
    stIn.u32Contrst = pstColor->u32Contrst;
    stIn.u32Hue     = pstColor->u32Hue;
    stIn.u32Satur   = pstColor->u32Satur;
    stIn.u32Kr = pstColor->u32Kr;
    stIn.u32Kg = pstColor->u32Kg;
    stIn.u32Kb = pstColor->u32Kb;

/*
    DISP_PRINT(">>>>>>>>WinHalSetColor_MPW i=%d, o=%d, B=%d, C=%d, H=%d, S=%d,KR=%d,KG=%d, KB=%d\n",
               pstColor->enInCS, pstColor->enOutCS, 
               pstColor->u32Bright,
               pstColor->u32Contrst,
               pstColor->u32Hue,
               pstColor->u32Satur,
               pstColor->u32Kr,
               pstColor->u32Kg,
               pstColor->u32Kb);
*/

    pstuUA->pfCalcCscCoef(&stIn, &stOut);

/*
    DISP_PRINT(">>>>>>>>WinHalSetColor_MPW D1=%d, D2=%d, C00=%d, C11=%d, C22=%d\n",
               stOut.s32CscDcIn_1, stOut.s32CscDcIn_2, 
               stOut.s32CscCoef_00,
               stOut.s32CscCoef_11,
               stOut.s32CscCoef_22);
*/

    stCscCoef.csc_in_dc0 = stOut.s32CscDcIn_0;
    stCscCoef.csc_in_dc1 = stOut.s32CscDcIn_1;
    stCscCoef.csc_in_dc2 = stOut.s32CscDcIn_2;

    stCscCoef.csc_out_dc0 = stOut.s32CscDcOut_0;
    stCscCoef.csc_out_dc1 = stOut.s32CscDcOut_1;
    stCscCoef.csc_out_dc2 = stOut.s32CscDcOut_2;
    VDP_VID_SetCscDcCoef(u32LayerId, stCscCoef);

    stCscCoef2.csc_coef00 = stOut.s32CscCoef_00;
    stCscCoef2.csc_coef01 = stOut.s32CscCoef_01;
    stCscCoef2.csc_coef02 = stOut.s32CscCoef_02;

    stCscCoef2.csc_coef10 = stOut.s32CscCoef_10;
    stCscCoef2.csc_coef11 = stOut.s32CscCoef_11;
    stCscCoef2.csc_coef12 = stOut.s32CscCoef_12;

    stCscCoef2.csc_coef20 = stOut.s32CscCoef_20;
    stCscCoef2.csc_coef21 = stOut.s32CscCoef_21;
    stCscCoef2.csc_coef22 = stOut.s32CscCoef_22;

    VDP_VID_SetCscCoef(u32LayerId, stCscCoef2);
    
    VDP_VID_SetCscEnable(u32LayerId, 1);

    return HI_SUCCESS;
}


HI_S32 SetPixFmt(HI_U32 u32LayerId, HI_DRV_PIX_FORMAT_E eFmt)
{
    if (eFmt == HI_DRV_PIX_FMT_NV12)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV21)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV16)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV61)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YUYV)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YUYV);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YVYU)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YVYU);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_UYVY)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_UYVY);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }

    else
    {
        WIN_FATAL(">>>>>>>>>>>>> Error! not support vid format!\n");
    }

    return HI_SUCCESS;
}


HI_S32 SetAddr(HI_U32 u32LayerId, HI_DRV_VID_FRAME_ADDR_S *pstAddr, HI_U32 u32Num)
{
    VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr->u32PhyAddr_Y, pstAddr->u32PhyAddr_C, 
                         pstAddr->u32Stride_Y, pstAddr->u32Stride_C);

    VDP_VID_SetLayerAddr(u32LayerId, 1, pstAddr->u32PhyAddr_Y, pstAddr->u32PhyAddr_C, 
                         pstAddr->u32Stride_Y, pstAddr->u32Stride_C);

    return HI_SUCCESS;
}

HI_S32 WinHalSetAddr_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara, HI_S32 s32exl)
{
    HI_DRV_VID_FRAME_ADDR_S *pstAddr;
    HI_U32 OffsetL, OffsetC;

    if (!pstPara)
    {
        DISP_FATAL_RETURN();
    }

//printk("u32LayerId=%d.", u32LayerId);
    pstAddr = &(pstPara->pstFrame->stBufAddr[0]);

    if (  (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
        ||(pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
        )
    {
        OffsetL = pstPara->stIn.s32X + (pstPara->stIn.s32Y * pstAddr[0].u32Stride_Y);
        OffsetC = pstPara->stIn.s32X + (pstPara->stIn.s32Y * pstAddr[0].u32Stride_Y/2);
    }
    else
    {
       DISP_FATAL_RETURN();
    }

    if (pstPara->en3Dmode == DISP_STEREO_FPK)
    {
        if (HI_DRV_FT_NOT_STEREO == pstPara->pstFrame->eFrmType)
        {
            VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr[0].u32PhyAddr_Y+OffsetL, 
                                                pstAddr[0].u32PhyAddr_C+OffsetC, 
                                                pstAddr[0].u32Stride_Y * s32exl, 
                                                pstAddr[0].u32Stride_C * s32exl);

            VDP_VID_SetLayerAddr(u32LayerId, 1, pstAddr[0].u32PhyAddr_Y+OffsetL, 
                                                pstAddr[0].u32PhyAddr_C+OffsetC, 
                                                pstAddr[0].u32Stride_Y * s32exl, 
                                                pstAddr[0].u32Stride_C* s32exl);

        }
        else if (HI_DRV_FT_BUTT > pstPara->pstFrame->eFrmType)
        {
            if (pstPara->bRightEyeFirst)
            {
                VDP_VID_SetLayerAddr(u32LayerId, 1, pstAddr[0].u32PhyAddr_Y+OffsetL, 
                                                    pstAddr[0].u32PhyAddr_C+OffsetC, 
                                                    pstAddr[0].u32Stride_Y* s32exl, 
                                                    pstAddr[0].u32Stride_C* s32exl);
                
                VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr[1].u32PhyAddr_Y+OffsetL,
                                                    pstAddr[1].u32PhyAddr_C+OffsetC, 
                                                    pstAddr[1].u32Stride_Y* s32exl, 
                                                    pstAddr[1].u32Stride_C* s32exl);
            }
            else
            {
                VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr[0].u32PhyAddr_Y+OffsetL, 
                                                    pstAddr[0].u32PhyAddr_C+OffsetC, 
                                                    pstAddr[0].u32Stride_Y* s32exl, 
                                                    pstAddr[0].u32Stride_C* s32exl);
                
                VDP_VID_SetLayerAddr(u32LayerId, 1, pstAddr[1].u32PhyAddr_Y+OffsetL,
                                                    pstAddr[1].u32PhyAddr_C+OffsetC, 
                                                    pstAddr[1].u32Stride_Y* s32exl, 
                                                    pstAddr[1].u32Stride_C* s32exl);
            }
        }
        else
        {
            DISP_FATAL_RETURN();
        }

        VDP_VID_SetFlipEnable(u32LayerId,HI_FALSE);
    }
    else
    {
        if (pstPara->pstFrame->u32Circumrotate == 0)
        {
            VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr[0].u32PhyAddr_Y+OffsetL, 
                                            pstAddr[0].u32PhyAddr_C+OffsetC, 
                                            pstAddr[0].u32Stride_Y * s32exl, 
                                            pstAddr[0].u32Stride_C * s32exl);
            VDP_VID_SetFlipEnable(u32LayerId,HI_FALSE);
        }
        else /*VP6 stream need flip*/
        {
            VDP_VID_SetLayerAddr(u32LayerId, 0, pstAddr[0].u32PhyAddr_Y + (pstPara->stIn.s32Height-1)*pstAddr[0].u32Stride_Y,
                                            pstAddr[0].u32PhyAddr_C + (pstPara->stIn.s32Height/2-1)*pstAddr[0].u32Stride_C, 
                                            pstAddr[0].u32Stride_Y * s32exl, 
                                            pstAddr[0].u32Stride_C * s32exl);
            VDP_VID_SetFlipEnable(u32LayerId,HI_TRUE);
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinHalSetPixFmt_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_DRV_PIX_FORMAT_E eFmt = pstPara->pstFrame->ePixFormat;

    if (!pstPara)
    {
        DISP_FATAL_RETURN();
    }
    
    if (eFmt == HI_DRV_PIX_FMT_NV12)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV21)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_420);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_420);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV16)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_NV61)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_SP_422);
        VDP_VID_SetInDataUVOrder(u32LayerId, 0);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YUYV)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YUYV);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_YVYU)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_YVYU);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if (eFmt == HI_DRV_PIX_FMT_UYVY)
    {
        VDP_VID_SetInDataFmt(u32LayerId, VDP_VID_IFMT_PKG_UYVY);
        //VDP_VID_SetInDataUVOrder(u32LayerId, 1);
        VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else
    {
        WIN_FATAL(">>>>>>>>>>>>> Error! not support vid format!\n");
    }

    return HI_SUCCESS;
}

HI_S32 TranPixFmtToAlg(HI_DRV_PIX_FORMAT_E enFmt)
{
    switch (enFmt)
    {
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_YVU420:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            return 1;

        case HI_DRV_PIX_FMT_NV16:
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YYUV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
        case HI_DRV_PIX_FMT_VYUY:
        case HI_DRV_PIX_FMT_YUV422P:
        case HI_DRV_PIX_FMT_NV16_CMP:
        case HI_DRV_PIX_FMT_NV61_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
            return 0;

        case HI_DRV_PIX_FMT_NV24_CMP:
        case HI_DRV_PIX_FMT_NV42_CMP:
        case HI_DRV_PIX_FMT_NV24:
        case HI_DRV_PIX_FMT_NV42:
            return 2;

        default:
            return 1;
    }

}



HI_S32 WinHalSetRect_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara, HI_S32 s32exl)
{
    //HI_U32 u32Ratio;
    DISP_UA_FUNCTION_S *pfUA;
    ALG_VZME_DRV_PARA_S stZmeI;
    ALG_VZME_RTL_PARA_S stZmeO;
    HI_RECT_S stIntmp, stVideo, stDisp;
    
    pfUA = DISP_UA_GetFunction();
    if(!pfUA)
    {
        return HI_FAILURE;
    }

    stIntmp = pstPara->stIn;
    stIntmp.s32Height = stIntmp.s32Height/s32exl;

    stVideo = pstPara->stVideo;
    stDisp = pstPara->stDisp;

    if (  pstPara->pstDispInfo->stOrgRect.s32Width == 
          (pstPara->pstDispInfo->stRefRect.s32Width*2)
        )
    {
        stVideo.s32X     = stVideo.s32X * 2;
        stVideo.s32Width = stVideo.s32Width* 2;
        stDisp.s32X     = stDisp.s32X * 2;
        stDisp.s32Width = stDisp.s32Width* 2;
    }

    VDP_VID_SetInReso2(u32LayerId, &stIntmp);
    VDP_VID_SetOutReso2(u32LayerId,  &stVideo);
    VDP_VID_SetVideoPos2(u32LayerId, &stVideo);
    VDP_VID_SetDispPos2(u32LayerId,  &stDisp);

#if 0
    u32Ratio = (pstPara->stIn.s32Width << 20) / pstPara->stVideo.s32Width;
    VDP_VID_SetZmeHorRatio(u32LayerId, u32Ratio);

    u32Ratio = (pstPara->stIn.s32Height << 12) / pstPara->stVideo.s32Height;
    VDP_VID_SetZmeVerRatio(u32LayerId, u32Ratio);

    VDP_VID_SetZmePhaseH(u32LayerId, 0, 0);
    VDP_VID_SetZmePhaseV(u32LayerId, 0, 0);
    VDP_VID_SetZmePhaseVB(u32LayerId, 0, 0);
    VDP_VID_SetZmeFirEnable2(u32LayerId, 0);
    VDP_VID_SetZmeMidEnable2(u32LayerId, 0);

    VDP_VID_SetZmeEnable2(u32LayerId, 1);
#else
    stZmeI.u32ZmeFrmWIn = stIntmp.s32Width;
    stZmeI.u32ZmeFrmHIn = stIntmp.s32Height;
    stZmeI.u32ZmeFrmWOut = stVideo.s32Width;
    stZmeI.u32ZmeFrmHOut = stVideo.s32Height;

    stZmeI.u8ZmeYCFmtIn  = TranPixFmtToAlg(pstPara->pstFrame->ePixFormat);

    stZmeI.u8ZmeYCFmtOut = 0;  // X5HD2 MPW FIX '0'(422)
    stZmeI.bZmeFrmFmtIn  = 1;
    stZmeI.bZmeFrmFmtOut = (pstPara->pstDispInfo->bInterlace == HI_TRUE) ? 0 : 1;
    //stZmeI.bZmeFrmFmtOut = 0;
    
    stZmeI.bZmeBFIn  = 0;
    stZmeI.bZmeBFOut = 0;

    if ( (u32LayerId == (HI_U32)VDP_LAYER_VID2)
        || (u32LayerId == (HI_U32)VDP_LAYER_VID3))
    {
        //DISP_PRINT("S.");
        pfUA->pfVZmeVdpHQSet(&stZmeI, &stZmeO);
    }
    else
    {
        pfUA->pfVZmeVdpHQSet(&stZmeI, &stZmeO);
    }
#if 0
    printk("HRatio=0x%x, VRatio=0x%x, Hoffset=%d, Voffset=%d, \
            filter=%d, md=%d,order=%d, vctap=%d,\
            HADDR=0x%x,VADDR=0x%x,En =%d\n",
                stZmeO.u32ZmeRatioHL,
                stZmeO.u32ZmeRatioVL,
                stZmeO.s32ZmeOffsetHL,
                stZmeO.s32ZmeOffsetVL,
                stZmeO.bZmeMdVL,
                stZmeO.bZmeMedHL,
                stZmeO.bZmeOrder,
                stZmeO.bZmeTapVC,
                stZmeO.u32ZmeCoefAddrHL,
                stZmeO.u32ZmeCoefAddrVL,
                stZmeO.bZmeEnVL);
#endif

    //stZmeO.u32ZmeRatioHL = 0x90000;
    //stZmeO.u32ZmeRatioVL = 0xccdul,
    VDP_VID_SetZmeHorRatio(u32LayerId, stZmeO.u32ZmeRatioHL);
    VDP_VID_SetZmeVerRatio(u32LayerId, stZmeO.u32ZmeRatioVL);

    VDP_VID_SetZmePhaseH(u32LayerId, stZmeO.s32ZmeOffsetHL, stZmeO.s32ZmeOffsetHC);
    VDP_VID_SetZmePhaseV(u32LayerId, stZmeO.s32ZmeOffsetVL, stZmeO.s32ZmeOffsetVC);
    VDP_VID_SetZmePhaseVB(u32LayerId, stZmeO.s32ZmeOffsetVLBtm, stZmeO.s32ZmeOffsetVCBtm);

    VDP_VID_SetZmeFirEnable2(u32LayerId, stZmeO.bZmeMdVL);
    VDP_VID_SetZmeMidEnable2(u32LayerId, stZmeO.bZmeMedHL);

    VDP_VID_SetZmeHfirOrder(u32LayerId, stZmeO.bZmeOrder);
    VDP_VID_SetZmeVchTap(u32LayerId, stZmeO.bZmeTapVC);

    if (stZmeO.u32ZmeRatioHL)
    {
        VDP_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_HOR, stZmeO.u32ZmeCoefAddrHL);
        VDP_VID_SetParaUpd(u32LayerId,VDP_VID_PARA_ZME_HOR);
    }

    if (stZmeO.bZmeMdVL)
    {
        VDP_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_VER, stZmeO.u32ZmeCoefAddrVL);
        VDP_VID_SetParaUpd(u32LayerId,VDP_VID_PARA_ZME_VER);
    }

    VDP_VID_SetZmeInFmt(u32LayerId, VDP_PROC_FMT_SP_420);
    VDP_VID_SetZmeOutFmt(u32LayerId, VDP_PROC_FMT_SP_422);

    VDP_VID_SetZmeEnable2(u32LayerId, stZmeO.bZmeEnVL);
#endif

    return HI_SUCCESS;
}


#if 0
HI_S32 WinHalSetxxx_MPW(WIN_HAL_PARA_S *pstPara)
{

    return HI_SUCCESS;
}

HI_S32 WinHalSetxxx_MPW(WIN_HAL_PARA_S *pstPara)
{

    return HI_SUCCESS;
}

HI_S32 WinHalSetxxx_MPW(WIN_HAL_PARA_S *pstPara)
{

    return HI_SUCCESS;
}
#endif



HI_S32 WinHalGetExtrLineParam(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_S32 s32exl = 1;
    HI_S32 s32HeightIn, s32HeightOut;
    

    s32HeightIn = pstPara->stIn.s32Height;
    s32HeightOut = pstPara->stVideo.s32Height;
    while(s32HeightIn  > (s32HeightOut * VIDEO_ZOOM_IN_VERTICAL_MAX))
    {
        s32exl = s32exl * 2;
        s32HeightIn = s32HeightIn / s32exl;
    }

    return s32exl;
}


HI_S32 WinHalSetFrame_MPW(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    HI_S32 s32exl;
    
    if( SetDispMode(u32LayerId, pstPara->en3Dmode) )
    {
        return HI_FAILURE;
    }

    s32exl = WinHalGetExtrLineParam(u32LayerId, pstPara);
    if( WinHalSetAddr_MPW(u32LayerId, pstPara, s32exl) )
    {
        return HI_FAILURE;
    }

    if( WinHalSetPixFmt_MPW(u32LayerId, pstPara) )
    {
        return HI_FAILURE;
    }

    if (pstPara->bZmeUpdate)
    {
        if( WinHalSetRect_MPW(u32LayerId, pstPara, s32exl) )
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

VDP_CBM_MIX_E GetMixerID(VDP_LAYER_VID_E eLayer)
{
    if ( (VDP_LAYER_VID0 <= eLayer) && (VDP_LAYER_VID1 >= eLayer))
    {
        return VDP_CBM_MIXV0;
    }
    else if ( (VDP_LAYER_VID2 <= eLayer) && (VDP_LAYER_VID3 >= eLayer))
    {
        return VDP_CBM_MIXV1;
    }
    else
    {
        return VDP_CBM_MIX_BUTT;
    }
}

HI_U32 GetMixMaxNumvber(VDP_CBM_MIX_E eM)
{
    switch(eM)
    {
        case VDP_CBM_MIXV0:
            return 2;
        case VDP_CBM_MIXV1:
            return 1;
        default:
            return 0;
    }
}

HI_S32 MovUp(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;
    // get mixv id
    eMixId = GetMixerID(eLayerHalId);


//printk("in id=%d, halid=%d\n", eLayer, eLayerHalId);
    // get mixv setting
    nMaxLayer = GetMixMaxNumvber(eMixId);

//printk("eMixId=%d, nMaxLayer=%d\n", eMixId, nMaxLayer);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        VDP_CBM_GetMixerPrio(eMixId, i, &MixArray[i]);
        //printk("prio=%d, id=%d\n", i, MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
    	//printk("i=%d, id=%d\n", i, MixArray[i]);
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

  	//printk("index=%d\n", index);

    // not found or just single layer work
    if (index >= (nMaxLayer-1))
    {
        return HI_SUCCESS;
    }

    // change mixv order
    MixArray[index]= MixArray[index+1];
    MixArray[index+1] = eLayerHalId;

    // set mixv setting
    VDP_CBM_SetMixerPrioQuick(eMixId, MixArray, nMaxLayer);

    return HI_SUCCESS;
}


HI_S32 MovTop(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = GetMixMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        VDP_CBM_GetMixerPrio(eMixId, i, &MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found or just single layer work
    if (index >= (nMaxLayer-1))
    {
        return HI_SUCCESS;
    }

    // change mixv order
    for(i=index; i<(nMaxLayer-1); i++)
    {
        MixArray[i]= MixArray[i+1];
    }
    MixArray[i] = eLayerHalId;

    // set mixv setting
    VDP_CBM_SetMixerPrioQuick(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}


HI_S32 MovDown(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = GetMixMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        VDP_CBM_GetMixerPrio(eMixId, i, &MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found
    if (index >= nMaxLayer)
    {
        return HI_SUCCESS;
    }

    // layer at bottom
    if (!index)
    {
        return HI_SUCCESS;
    }

    // change mixv order
    MixArray[index]= MixArray[index-1];
    MixArray[index - 1] = eLayerHalId;

    // set mixv setting
    VDP_CBM_SetMixerPrioQuick(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}


HI_S32 MovBottom(HI_U32 eLayer)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = GetMixMaxNumvber(eMixId);
    if (nMaxLayer <= 1)
    {
        return HI_SUCCESS;
    }

    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        VDP_CBM_GetMixerPrio(eMixId, i, &MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found
    if (index >= nMaxLayer)
    {
        return HI_SUCCESS;
    }

    // layer at bottom
    if (!index)
    {
        return HI_SUCCESS;
    }

    // change mixv order
    for(i=index; i>0; i--)
    {
        MixArray[i]= MixArray[i-1];
    }
    MixArray[0] = eLayerHalId;

    // set mixv setting
    VDP_CBM_SetMixerPrioQuick(eMixId, MixArray, nMaxLayer);
    return HI_SUCCESS;
}

HI_S32 GetZorder(HI_U32 eLayer, HI_U32 *pZOrder)
{
    VDP_CBM_MIX_E eMixId;
    HI_U32 nMaxLayer;
    VDP_LAYER_VID_E eLayerHalId;
    HI_U32 MixArray[VDP_LAYER_VID_BUTT+1];
    HI_U32 i, index;

    // get eLayer Id
    eLayerHalId = (VDP_LAYER_VID_E)eLayer;

//printk("in id=%d, halid=%d\n", eLayer, eLayerHalId);

    // get mixv id
    eMixId = GetMixerID(eLayerHalId);

    // get mixv setting
    nMaxLayer = GetMixMaxNumvber(eMixId);

//printk("eMixId=%d, nMaxLayer=%d\n", eMixId, nMaxLayer);
    if (nMaxLayer <= 1)
    {
        *pZOrder = 0;
        return HI_SUCCESS;
    }


    // get eLayer prio
    for(i=0; i<nMaxLayer; i++)
    {
        VDP_CBM_GetMixerPrio(eMixId, i, &MixArray[i]);
        //printk("prio=%d, id=%d\n", i, MixArray[i]);
    }

    // get eLayer index
    index = nMaxLayer;
    for(i=0; i<nMaxLayer; i++)
    {
        if (MixArray[i] == (HI_U32)eLayerHalId)
        {
            index = i;
            break;
        }
    }

    // not found
    if (index >= nMaxLayer)
    {
        *pZOrder = 0xfffffffful;
    }
    else
    {
        *pZOrder = index;
    }

    return HI_SUCCESS;
}



HI_S32 SetACC(HI_U32 eLayer, VIDEO_LAYER_ACC_MODE_E eMode)
{

    return HI_SUCCESS;
}


HI_S32 SetACM(HI_U32 eLayer, VIDEO_LAYER_ACM_MODE_E eMode)
{

    return HI_SUCCESS;
}


HI_S32 SetDebug(HI_U32 eLayer, HI_BOOL bEnable)
{

    return HI_SUCCESS;
}


HI_S32 InitCapabilityV200(HI_VOID)
{
    HI_U32 eId;

    DISP_MEMSET(&s_stVideoLayerCap, 0, sizeof(VIDEO_LAYER_CAPABILITY_S) * DEF_VIDEO_LAYER_MAX_NUMBER);

    // s1 set va0
    eId = 0;
    s_stVideoLayerCap[eId].eId      = (HI_U32)VDP_RM_LAYER_VID0;
    s_stVideoLayerCap[eId].bSupport = HI_TRUE;
    s_stVideoLayerCap[eId].bZme     = HI_TRUE;
    s_stVideoLayerCap[eId].bACC     = HI_TRUE;
    s_stVideoLayerCap[eId].bACM     = HI_TRUE;
    s_stVideoLayerCap[eId].bHDIn    = HI_TRUE;
    s_stVideoLayerCap[eId].bHDOut   = HI_TRUE;

    // s2 set va1
    eId++;
    s_stVideoLayerCap[eId].eId      = (HI_U32)VDP_RM_LAYER_VID1;
    s_stVideoLayerCap[eId].bSupport = HI_TRUE;
    s_stVideoLayerCap[eId].bZme     = HI_TRUE;
    s_stVideoLayerCap[eId].bACC     = HI_TRUE;
    s_stVideoLayerCap[eId].bACM     = HI_TRUE;
    s_stVideoLayerCap[eId].bHDIn    = HI_TRUE;
    s_stVideoLayerCap[eId].bHDOut   = HI_TRUE;

    // s2 set vb0
    eId++;
    s_stVideoLayerCap[eId].eId      = (HI_U32)VDP_RM_LAYER_VID2;
    s_stVideoLayerCap[eId].bSupport = HI_TRUE;
    s_stVideoLayerCap[eId].bZme     = HI_TRUE;
    s_stVideoLayerCap[eId].bACC     = HI_TRUE;
    s_stVideoLayerCap[eId].bACM     = HI_TRUE;
    s_stVideoLayerCap[eId].bHDIn    = HI_TRUE;
    s_stVideoLayerCap[eId].bHDOut   = HI_TRUE;

    // s2 set vb1
    eId++;
    s_stVideoLayerCap[eId].eId      = (HI_U32)VDP_RM_LAYER_VID3;
    s_stVideoLayerCap[eId].bSupport = HI_TRUE;
    s_stVideoLayerCap[eId].bZme     = HI_TRUE;
    s_stVideoLayerCap[eId].bACC     = HI_TRUE;
    s_stVideoLayerCap[eId].bACM     = HI_TRUE;
    s_stVideoLayerCap[eId].bHDIn    = HI_TRUE;
    s_stVideoLayerCap[eId].bHDOut   = HI_TRUE;

    eId++;
    for(; eId <DEF_VIDEO_LAYER_MAX_NUMBER; eId++)
    {
        s_stVideoLayerCap[eId].eId = (HI_U32)DEF_VIDEO_LAYER_INVALID_ID;
        s_stVideoLayerCap[eId].bSupport = HI_FALSE;
        s_stVideoLayerCap[eId].bZme     = HI_FALSE;
        s_stVideoLayerCap[eId].bACC     = HI_FALSE;
        s_stVideoLayerCap[eId].bACM     = HI_FALSE;
        s_stVideoLayerCap[eId].bHDIn    = HI_FALSE;
        s_stVideoLayerCap[eId].bHDOut   = HI_FALSE;
    }


    return HI_SUCCESS;
}


HI_S32 VideoLayer_Init(HI_DRV_DISP_VERSION_S *pstVersion)
{
    if (s_bVideoSurfaceFlag >= 0)
    {
        return HI_SUCCESS;
    }

    // s1 init videolayer
    DISP_MEMSET(&s_stVieoLayerFunc, 0, sizeof(VIDEO_LAYER_FUNCTIONG_S));
    DISP_MEMSET(&s_stVideoLayer, 0, sizeof(VIDEO_LAYER_S) * DEF_VIDEO_LAYER_MAX_NUMBER);


    // s2 init function pointer
    if (   (pstVersion->u32VersionPartH == DISP_X5HD2_MPW_VERSION_H)
        && (pstVersion->u32VersionPartL == DISP_X5HD2_MPW_VERSION_L)
        )
    {
        // s2.1 set function pointer
        s_stVieoLayerFunc.PF_GetCapability  = GetCapability;
        s_stVieoLayerFunc.PF_AcquireLayer   = AcquireLayer;
        s_stVieoLayerFunc.PF_AcquireLayerByDisplay = AcquireLayerByDisplay;
        s_stVieoLayerFunc.PF_ReleaseLayer   = ReleaseLayer;    
        s_stVieoLayerFunc.PF_SetEnable      = SetEnable;       
        s_stVieoLayerFunc.PF_Update         = Update;          
        s_stVieoLayerFunc.PF_SetDefault     = SetDefault;
        s_stVieoLayerFunc.PF_SetAllLayerDefault = SetAllLayerDefault;
        //s_stVieoLayerFunc.PF_SetFramePara   = SetFramePara;    
        s_stVieoLayerFunc.PF_SetDispMode    = SetDispMode;     
        s_stVieoLayerFunc.PF_SetIORect      = SetIORect;       
        s_stVieoLayerFunc.PF_SetColor  = WinHalSetColor_MPW;   
        s_stVieoLayerFunc.PF_SetPixFmt      = SetPixFmt;       
        s_stVieoLayerFunc.PF_SetAddr        = SetAddr;         
        s_stVieoLayerFunc.PF_MovUp          = MovUp;           
        s_stVieoLayerFunc.PF_MovTop         = MovTop;          
        s_stVieoLayerFunc.PF_MovDown        = MovDown;         
        s_stVieoLayerFunc.PF_MovBottom      = MovBottom;  
        s_stVieoLayerFunc.PF_GetZorder      = GetZorder;  
        s_stVieoLayerFunc.PF_SetACC         = SetACC;          
        s_stVieoLayerFunc.PF_SetACM         = SetACM;          
        s_stVieoLayerFunc.PF_SetDebug       = SetDebug;        


        s_stVieoLayerFunc.PF_SetFramePara   = WinHalSetFrame_MPW;

        // s2.2 init videolayer capbility
        InitCapabilityV200();

        // s2.3 init hardware

    }
    else
    {
        WIN_ERROR("Not support version : %x %x\n", 
                   pstVersion->u32VersionPartH, pstVersion->u32VersionPartL);

        return HI_FAILURE;
    }

    s_bVideoSurfaceFlag++;
    
    return HI_SUCCESS;
}


HI_S32 VideoLayer_DeInit(HI_VOID)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        return HI_SUCCESS;
    }

    s_bVideoSurfaceFlag--;
    
    return HI_SUCCESS;
}

HI_S32 VideoLayer_GetFunction(VIDEO_LAYER_FUNCTIONG_S *pstFunc)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        WIN_ERROR("Video layer NOT INIT\n");
        return HI_FAILURE;
    }


    if (!pstFunc)
    {
        WIN_ERROR("NULL Pointer\n");
        return HI_FAILURE;
    }

    *pstFunc = s_stVieoLayerFunc;

    return HI_SUCCESS;
}

VIDEO_LAYER_FUNCTIONG_S *VideoLayer_GetFunctionPtr(HI_VOID)
{
    if (s_bVideoSurfaceFlag < 0)
    {
        WIN_ERROR("Video layer NOT INIT\n");
        return HI_NULL;
    }

    return &s_stVieoLayerFunc;
}




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
