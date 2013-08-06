
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

#include "drv_disp_com.h"
#include "drv_disp_cast.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 DispCastCheckCfg(HI_DRV_DISP_CAST_CFG_S * pstCfg)
{
    if (  (pstCfg->u32Width < DISP_CAST_MIN_W)
        ||(pstCfg->u32Width > DISP_CAST_MAX_W)
        ||(pstCfg->u32Height < DISP_CAST_MIN_H)
        ||(pstCfg->u32Height > DISP_CAST_MAX_H)
        ||( (pstCfg->u32Width & 0x1)  != 0)
        ||( (pstCfg->u32Height & 0x3) != 0)
        )
    {
        DISP_ERROR("Cast w= %d or h=%d invalid\n", pstCfg->u32Width, pstCfg->u32Height);
        return HI_FAILURE;
    }

    if (  (pstCfg->eFormat != HI_DRV_PIX_FMT_NV21)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_NV12)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_YUYV)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_YVYU)
        &&(pstCfg->eFormat != HI_DRV_PIX_FMT_UYVY)
        )
    {
        DISP_ERROR("Cast pixfmt = %d invalid\n", pstCfg->eFormat);
        return HI_FAILURE;
    }

    if (  (pstCfg->u32BufNumber < DISP_CAST_BUFFER_MIN_NUMBER) 
        ||(pstCfg->u32BufNumber > DISP_CAST_BUFFER_MAX_NUMBER) )
    {
        DISP_ERROR("Cast u32BufNumber =%d invalid\n", pstCfg->u32BufNumber);
        return HI_FAILURE;
    }

    if(pstCfg->bUserAlloc) 
    {
        DISP_ERROR("Cast not support User Alloc memory\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DispSetFrameDemoPartA(HI_HANDLE hCast, HI_DRV_DISP_CAST_CFG_S *pstCfg, DISP_CAST_ATTR_S *pstAttr)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    DISP_CAST_PRIV_FRAME_S *pstPriv;

    // set frame demo
    DISP_MEMSET(pstAttr, 0, sizeof(DISP_CAST_ATTR_S));

    pstAttr->stOut.s32Width  = (HI_S32)pstCfg->u32Width;
    pstAttr->stOut.s32Height = (HI_S32)pstCfg->u32Height;

    pstFrame = &pstAttr->stFrameDemo;
    pstFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
    pstFrame->ePixFormat = pstCfg->eFormat;
    //printk("DispSetFrameDemoPartA pstCfg->eFormat=%d\n", pstCfg->eFormat);
    pstFrame->bProgressive = HI_TRUE;
    pstFrame->u32Width  = pstCfg->u32Width;
    pstFrame->u32Height = pstCfg->u32Height;
    pstFrame->stDispRect = pstAttr->stOut;

    pstPriv = (DISP_CAST_PRIV_FRAME_S *)&(pstFrame->u32Priv[0]);

    pstPriv->hCast = hCast;
    pstPriv->stPrivInfo.u32PlayTime = 1;

    return HI_SUCCESS;
}

HI_S32 DISP_CastCreate(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S *pstCfg, HI_HANDLE *phCast)
{
    HI_DRV_DISP_CALLBACK_S stCB1;
    DISP_CAST_S *pstCast;
    BUF_ALLOC_S stAlloc;
    HI_S32 nRet;


    // check cfg
    if( DispCastCheckCfg(pstCfg) )
    {
        DISP_ERROR("Cast config invalid!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    // alloc mem
    pstCast = (DISP_CAST_S *)DISP_MALLOC( sizeof(DISP_CAST_S) );
    if (!pstCast)
    {
        DISP_ERROR("Cast malloc failed!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    DISP_MEMSET(pstCast, 0, sizeof(DISP_CAST_S));

    // set attr
    DispSetFrameDemoPartA((HI_HANDLE)pstCast, pstCfg, &pstCast->stAttr);

    // get hal operation
    nRet = DISP_HAL_GetOperation(&pstCast->stIntfOpt);
    if (nRet)
    {
        DISP_ERROR("Cast get hal operation failed!\n");
        goto __ERR_EXIT__;
    }

    // get wbclayer
    nRet = pstCast->stIntfOpt.PF_AcquireWbcByChn(enDisp, &pstCast->eWBC);
    if (nRet)
    {
        DISP_ERROR("Cast get wbc layer failed!\n");
        goto __ERR_EXIT__;
    }

    // create buffer
    stAlloc.bFbAllocMem  = !pstCfg->bUserAlloc;
    stAlloc.eDataFormat  = pstCfg->eFormat;
    stAlloc.u32BufWidth  = pstCfg->u32Width;
    stAlloc.u32BufHeight = pstCfg->u32Height;
    stAlloc.u32BufStride = pstCfg->u32BufStride;
    nRet = BP_Create(pstCfg->u32BufNumber, &stAlloc, &pstCast->stBP);
    if (nRet)
    {
        DISP_ERROR("Cast alloc buffer failed!\n");
        goto __ERR_EXIT__;
    }

    // register callback
    stCB1.hDst = (HI_HANDLE)pstCast;
    stCB1.pfDISP_Callback = DISP_CastCBWork;
    nRet = DISP_ISR_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);
    if (nRet)
    {
        DISP_ERROR("Cast register work callback failed!\n");
        goto __ERR_EXIT__;
    }

    pstCast->u32LastCfgBufId= 0;
    pstCast->u32LastFrameBufId = 0;

    
    pstCast->bToGetDispInfo = HI_TRUE;
    pstCast->bOpen = HI_TRUE;
    pstCast->eDisp = enDisp;

    *phCast = (HI_HANDLE)pstCast;

    DISP_PRINT("DISP_CastCreate ok\n");

    return HI_SUCCESS;

__ERR_EXIT__:
    DISP_ISR_UnRegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);

    BP_Destroy(&pstCast->stBP);

    DISP_FREE(pstCast);
    return nRet;
}

HI_S32 DISP_CastDestroy(HI_HANDLE hCast)
{
    HI_DRV_DISP_CALLBACK_S stCB1;
    DISP_CAST_S *pstCast;
    HI_S32 nRet;

    pstCast = (DISP_CAST_S *)hCast;

    // set disable
    pstCast->bEnable = HI_FALSE;

    // unregister callback
    stCB1.hDst = (HI_HANDLE)hCast;
    stCB1.pfDISP_Callback = DISP_CastCBWork;
    nRet = DISP_ISR_UnRegCallback(pstCast->eDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB1);

    // destroy buffer
    BP_Destroy(&pstCast->stBP);

    pstCast->stIntfOpt.PF_ReleaseWbc(pstCast->eWBC);

    // free mem
    DISP_FREE(pstCast);

    return HI_SUCCESS;
}

HI_S32 DISP_CastSetEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    DISP_CAST_S *pstCast;

    pstCast = (DISP_CAST_S *)hCast;

    pstCast->bEnable = bEnable;

    DISP_PRINT("DISP_CastSetEnable  bEnable = 0x%x\n", (HI_U32)bEnable);

    return HI_SUCCESS;
}

HI_S32 DISP_CastGetEnable(HI_HANDLE hCast, HI_BOOL *pbEnable)
{
    DISP_CAST_S *pstCast;

    pstCast = (DISP_CAST_S *)hCast;

    *pbEnable = pstCast->bEnable;

    return HI_SUCCESS;
}

HI_S32	DispCastSetAcquirePTS(HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    DISP_OS_GetTime(&pstPrivFrame->u32Pts1);

    return HI_SUCCESS;	
}


HI_S32 DISP_CastAcquireFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_S *pstCast;
    HI_U32 u32BufId;
    HI_S32 nRet;

    pstCast = (DISP_CAST_S *)hCast;

    // acquire frame
    nRet = BP_GetFullBuf(&pstCast->stBP, &u32BufId);
    if (nRet)
    {
        DISP_INFO("Cast acquire frame failed!\n");
        return nRet;
    }

    nRet = BP_DelFullBuf(&pstCast->stBP, u32BufId);
    if (nRet)
    {
        DISP_ERROR("Cast del frame failed!\n");
        return nRet;
    }

    nRet = BP_GetFrame(&pstCast->stBP, u32BufId, pstCastFrame);
    if (nRet)
    {
        DISP_ERROR("Cast get frame failed!\n");
        return nRet;
    }


    DispCastSetAcquirePTS(pstCastFrame);
#if 0
    printk("cast acquire bufid=0x%x, w=%d,h=%d,index=%d, y=0x%x, c=0x%x\n", 
    u32BufId,
    pstCastFrame->u32Width, 
    pstCastFrame->u32Height,
    pstCastFrame->u32FrmCnt,
    pstCastFrame->stBufAddr[0].u32PhyAddr_Y,
    pstCastFrame->stBufAddr[0].u32PhyAddr_C
    );
#endif

    return HI_SUCCESS;
}


HI_S32 DispCastCheckFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    if (pstPrivFrame->hCast == hCast)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_S32 DispCastGetBufId(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame, HI_U32 *pu32BufId)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCastFrame->u32Priv[0]);

    if (pstPrivFrame->hCast != hCast)
    {
        DISP_PRINT("PRIV=[%x][%x]\n", pstPrivFrame->hCast, pstPrivFrame->stPrivInfo.u32BufferID);
        return HI_FAILURE;
    }

    *pu32BufId = pstPrivFrame->stPrivInfo.u32BufferID;

    return HI_SUCCESS;
}

HI_S32 DISP_CastReleaseFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_S *pstCast;
    HI_U32 u32BufId;
    HI_S32 nRet = HI_SUCCESS;

    pstCast = (DISP_CAST_S *)hCast;

    if(DispCastGetBufId(hCast, pstCastFrame, &u32BufId))
    {
        DISP_ERROR("Cast release frame invalid!\n");
    return nRet;
    }

    /*
    printk("cast rel bufid=0x%x, w=%d,h=%d,index=%d\n", 
    u32BufId,
    pstCastFrame->u32Width, 
    pstCastFrame->u32Height,
    pstCastFrame->u32FrmCnt
    );
    */
    // release frame
    nRet = BP_AddEmptyBuf(&pstCast->stBP, u32BufId);
    if (nRet)
    {
        DISP_ERROR("Cast release frame failed!\n");
    }

    nRet = BP_SetBufEmpty(&pstCast->stBP, u32BufId);
    if (nRet)
    {
        DISP_ERROR("Cast release frame failed!\n");
    }

    return nRet;
}


HI_S32 DispSetFrameDemoPartB(HI_HANDLE hCast, HI_U32 u32Rate, HI_DRV_COLOR_SPACE_E eColorSpace, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    DISP_CAST_S *pstCast;
    DISP_CAST_PRIV_FRAME_S *pstPriv;

    pstCast = (DISP_CAST_S *)hCast;

    pstPriv = (DISP_CAST_PRIV_FRAME_S *)&(pstFrame->u32Priv[0]);

    // set frame demo
    pstFrame->u32FrameRate = u32Rate;
    pstPriv->stPrivInfo.eColorSpace = eColorSpace;

    return HI_SUCCESS;
}

HI_VOID DISP_CastCBSetDispMode(HI_HANDLE hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    DISP_CAST_S *pstCast;
    HI_U32 Rate;

    pstCast = (DISP_CAST_S *)hCast;

    // set display info
    pstCast->stDispInfo = pstInfo->stDispInfo;

    pstCast->stAttr.stIn = pstInfo->stDispInfo.stOrgRect;
    pstCast->stAttr.bInterlace = pstInfo->stDispInfo.bInterlace;
    pstCast->stAttr.eInColorSpace = pstInfo->stDispInfo.eColorSpace;
    //Todo
    pstCast->stAttr.eOutColorSpace = pstCast->stAttr.eInColorSpace;

    Rate = pstInfo->stDispInfo.u32RefreshRate;
    pstCast->stAttr.u32InRate = Rate;

    pstCast->u32Periods = 1;
    while(Rate > DISP_CAST_MAX_FRAME_RATE)
    {
        pstCast->u32Periods = pstCast->u32Periods << 1;
        Rate = Rate >> 1;
    }
    pstCast->stAttr.u32OutRate = Rate;

    DispSetFrameDemoPartB(hCast, Rate, pstCast->stAttr.eOutColorSpace, &pstCast->stAttr.stFrameDemo);

    // set flag
    //pstCast->bDispSet    = HI_TRUE;
    //pstCast->bDispUpdate = HI_TRUE;

    DISP_PRINT("CAST: iw=%d, ih=%d, ow=%d, oh=%d, or=%d\n ",  
                pstCast->stAttr.stIn.s32Width,
                pstCast->stAttr.stIn.s32Height,
                pstCast->stAttr.stOut.s32Width,
                pstCast->stAttr.stOut.s32Height,
                Rate);

    return ;
}


HI_S32 DispCastSetFrameInfo(DISP_CAST_S *pstCast, HI_DRV_VIDEO_FRAME_S *pstCurFrame)
{
    DISP_CAST_PRIV_FRAME_S *pstPrivFrame;

    pstCast->stAttr.stFrameDemo.u32FrameIndex = pstCast->u32FrameCnt;
    pstCast->stAttr.stFrameDemo.stBufAddr[0] = pstCurFrame->stBufAddr[0];

    //printk("Y=0x%x, C=0x%x\n", 
    //        pstCast->stAttr.stFrameDemo.stBufAddr[0].u32PhyAddr_Y,
    //        pstCast->stAttr.stFrameDemo.stBufAddr[0].u32PhyAddr_C);

    pstPrivFrame = (DISP_CAST_PRIV_FRAME_S *)&(pstCast->stAttr.stFrameDemo.u32Priv[0]);
    pstPrivFrame->stPrivInfo.u32FrmCnt   = pstCast->u32FrameCnt;
    pstPrivFrame->stPrivInfo.u32BufferID = pstCast->u32LastCfgBufId;
    //printk("ID=0x%x\n", pstPrivFrame->u32BufId);

    DISP_OS_GetTime(&pstCast->stAttr.stFrameDemo.u32Pts);

    return HI_SUCCESS;
}


HI_S32 DispCastSendTask(DISP_CAST_S *pstCast)
{
    DISP_CAST_ATTR_S *pstAttr;

    pstAttr = &pstCast->stAttr;

    // config pixformat
    pstCast->stIntfOpt.PF_SetWbcPixFmt(pstCast->eWBC, pstAttr->stFrameDemo.ePixFormat);

    // printk("DispCastSendTask eFormat=%d\n", pstAttr->stFrameDemo.ePixFormat);
    // config irect	and outrect	
    pstCast->stIntfOpt.PF_SetWbcIORect(pstCast->eWBC, &pstAttr->stIn, &pstAttr->stOut);

    // config csc
    pstCast->stIntfOpt.PF_SetWbcColorSpace(pstCast->eWBC, pstAttr->eInColorSpace, pstAttr->eOutColorSpace);

    // config addr
    pstCast->stIntfOpt.PF_SetWbcAddr(pstCast->eWBC, &(pstAttr->stFrameDemo.stBufAddr[0]));

    // set enable
    pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_TRUE);
    pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);

    //printk("Start a cast task!\n");

    return HI_SUCCESS;
}


HI_VOID DISP_CastCBWork(HI_HANDLE hCast, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_VIDEO_FRAME_S stCurFrame;
    DISP_CAST_S *pstCast;
    HI_U32 u32BufId;
    HI_S32 nRet;

    pstCast = (DISP_CAST_S *)hCast;

    if ( (pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND)
       )
    {
        pstCast->bMasked  = HI_TRUE;
        pstCast->bToGetDispInfo = HI_TRUE;
    }
    else
    {
        pstCast->bMasked  = HI_FALSE;
    }

    if (pstInfo->eEventType == HI_DRV_DISP_C_OPEN)
    {
        pstCast->bToGetDispInfo = HI_TRUE;
    }

    if (pstCast->bToGetDispInfo)
    {
        DISP_CastCBSetDispMode(hCast, pstInfo);

        pstCast->bToGetDispInfo = HI_FALSE;
    }


    // check state
    if(!pstCast->bEnable || pstCast->bMasked)
    {
        // set enable
        pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_FALSE);
        pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);

        //printk("[z:%d,%d,%d] ", pstCast->bEnable, pstCast->bMasked, pstCast->bDispSet);
        return;
    }

    if (pstInfo->eEventType == HI_DRV_DISP_C_VT_INT)
    {
        // set enable
        pstCast->stIntfOpt.PF_SetWbcEnable(pstCast->eWBC, HI_FALSE);
        pstCast->stIntfOpt.PF_UpdateWbc(pstCast->eWBC);


        // put frame
        if (pstCast->u32LastFrameBufId)
        {
            //printk("L=%d,", pstCast->u32LastFrameBufId);
            nRet = BP_AddFullBuf(&pstCast->stBP, pstCast->u32LastFrameBufId);
            if(nRet)
            {
                DISP_ERROR("Cast ADD buf failed!\n");
                return;
            }

            pstCast->u32LastFrameBufId = 0;
        }

        if (pstCast->u32LastCfgBufId)
        {
            pstCast->u32LastFrameBufId = pstCast->u32LastCfgBufId;
            pstCast->u32LastCfgBufId = 0;
        }      

        //printk("a0.");
        // update account
        pstCast->u32TaskCount++;

        //printk("[%d,%d]", pstCast->u32TaskCount, pstCast->u32Periods);

        if ( (pstCast->u32TaskCount % pstCast->u32Periods) != 0)
        {
            //printk("a1.");
            return;
        }

        //printk("a2.");
        // get buffer
        nRet = BP_GetEmptyBuf(&pstCast->stBP, &u32BufId);
        if (nRet)
        {
            DISP_INFO("Cast buffer full\n");
            return;
        }

        //printk("a3.");
        nRet = BP_DelEmptyBuf(&pstCast->stBP, u32BufId);
        DISP_ASSERT(!nRet);

        nRet = BP_GetFrame(&pstCast->stBP, u32BufId, &stCurFrame);
        DISP_ASSERT(!nRet);

        pstCast->u32LastCfgBufId = u32BufId;
        pstCast->u32FrameCnt++;

        DispCastSetFrameInfo(pstCast, &stCurFrame);

    //printk("a5.");

        // set task	
        DispCastSendTask(pstCast);

        nRet = BP_SetFrame(&pstCast->stBP, pstCast->u32LastCfgBufId, &pstCast->stAttr.stFrameDemo);
        DISP_ASSERT(!nRet);
    }

    return;
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */





