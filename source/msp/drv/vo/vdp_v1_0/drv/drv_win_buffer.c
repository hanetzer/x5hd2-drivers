
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_hw.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_win_buffer.h"
#include "drv_disp_bufcore.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

WB_DEBUG_INFO_S * WinBuf_DebugCreate(HI_U32 recordnum)
{
    WB_DEBUG_INFO_S *pstInfo;
    
    if (0 == recordnum)
    {
        return HI_NULL;
    }

    pstInfo = (WB_DEBUG_INFO_S *)DISP_MALLOC(sizeof(WB_DEBUG_INFO_S));
    if (!pstInfo)
    {
        return HI_NULL;
    }

    DISP_MEMSET(pstInfo, 0 , sizeof(WB_DEBUG_INFO_S));

    pstInfo->u32RecordNumber = recordnum;
    return pstInfo;
}


HI_VOID WinBuf_DebugDestroy(WB_DEBUG_INFO_S *pstInfo)
{
    if (pstInfo)
    {
        DISP_FREE(pstInfo);
    }
}

HI_VOID WinBuf_DebugAddInput(WB_DEBUG_INFO_S *pstInfo, HI_U32 u32FrameId)
{
    pstInfo->u32InputFrameID[pstInfo->u32InputPos] = u32FrameId;

    pstInfo->u32InputPos = (pstInfo->u32InputPos + 1)%pstInfo->u32RecordNumber;
    pstInfo->u32Input++;
}

HI_VOID WinBuf_DebugAddCfg(WB_DEBUG_INFO_S *pstInfo, HI_U32 u32FrameId)
{
    pstInfo->u32CfgFrameID[pstInfo->u32CfgPos] = u32FrameId;

    pstInfo->u32CfgPos = (pstInfo->u32CfgPos + 1)%pstInfo->u32RecordNumber;
    pstInfo->u32Config++;
}

HI_VOID WinBuf_DebugAddRls(WB_DEBUG_INFO_S *pstInfo, HI_U32 u32FrameId)
{
    pstInfo->u32RlsFrameID[pstInfo->u32RlsPos] = u32FrameId;

    pstInfo->u32RlsPos = (pstInfo->u32RlsPos + 1)%pstInfo->u32RecordNumber;
    pstInfo->u32Release++;
}

HI_VOID WinBuf_DebugAddTryQF(WB_DEBUG_INFO_S *pstInfo)
{
    pstInfo->u32TryQueueFrame++;
}

HI_VOID WinBuf_DebugAddQFOK(WB_DEBUG_INFO_S *pstInfo)
{
    pstInfo->u32QueueFrame++;
}

HI_VOID WinBuf_DebugAddUnderload(WB_DEBUG_INFO_S *pstInfo)
{
    pstInfo->u32Underload++;
}

HI_VOID WinBuf_DebugAddDisacard(WB_DEBUG_INFO_S *pstInfo)
{
    pstInfo->u32Disacard++;
}

HI_S32 WinBuf_Create(HI_U32 u32BufNum, HI_U32 u32MemType, WIN_BUF_ALLOC_PARA_S *pstAlloc, WB_POOL_S *pstWinBP)
{
    HI_S32 nRet;

    WIN_CHECK_NULL_RETURN(pstWinBP);

    if(u32MemType != WIN_BUF_MEM_SRC_SUPPLY)
    {
        WIN_FATAL("Win buffer NOT SURPPORT ALLOC MEMORY NOW!\n");
        return HI_FAILURE;
    }

    if( (u32BufNum == 0) || (u32BufNum > DISP_BUF_NODE_MAX_NUMBER))
    {
        WIN_FATAL("Win buffer number is invalid = %d\n", u32BufNum);
        return HI_FAILURE;
    }

    DISP_MEMSET(pstWinBP, 0, sizeof(WB_POOL_S));
    DISP_ASSERT(sizeof(HI_DRV_VIDEO_FRAME_S) < (sizeof(HI_U32) * DISP_BUF_DATA_SIZE));

    pstWinBP->u32BufNumber = u32BufNum;
    pstWinBP->u32MemType   = u32MemType;
    
    nRet = DispBuf_Create(&pstWinBP->stBuffer, u32BufNum);
    if(nRet != HI_SUCCESS)
    {
        WIN_FATAL("Win create buffer failed\n");
        return HI_FAILURE;
    }

    if(u32MemType == WIN_BUF_MEM_FB_SUPPLY)
    {
        DISP_BUF_NODE_S *pstNode;
        HI_U32 u;

        for(u=0; u<pstWinBP->u32BufNumber; u++)
        {
            nRet = DispBuf_GetNodeContent(&pstWinBP->stBuffer, u, &pstNode);

            // todo : alloc memory and initial node
        }
        // TODO
        pstWinBP->u32MemType = WIN_BUF_MEM_FB_SUPPLY;
    }

    pstWinBP->pstDisplay = HI_NULL;
    pstWinBP->pstConfig  = HI_NULL;

    pstWinBP->u32RlsWrite = 0;
    pstWinBP->u32RlsRead  = 0;

    pstWinBP->pstDebugInfo = WinBuf_DebugCreate(WB_BUFFER_DEBUG_FRAME_RECORD_NUMBER);
    if (!pstWinBP->pstDebugInfo)
    {
        goto __ERR_EXIT_;
    }

    return HI_SUCCESS;

__ERR_EXIT_:
    if(pstWinBP->u32MemType == WIN_BUF_MEM_FB_SUPPLY)
    {
        DISP_BUF_NODE_S *pstNode;
        HI_U32 u;

        for(u=0; u<pstWinBP->u32BufNumber; u++)
        {
            nRet = DispBuf_GetNodeContent(&pstWinBP->stBuffer, u, &pstNode);

            // todo : release memory
        }
    }

    nRet = DispBuf_Destoy(&pstWinBP->stBuffer);

    return HI_FAILURE;
}

HI_S32 WinBuf_Destroy(WB_POOL_S *pstWinBP)
{
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);

    WinBuf_DebugDestroy(pstWinBP->pstDebugInfo);
    
    if(pstWinBP->u32MemType == WIN_BUF_MEM_FB_SUPPLY)
    {
        DISP_BUF_NODE_S *pstNode;
        HI_U32 u;

        for(u=0; u<pstWinBP->u32BufNumber; u++)
        {
            nRet = DispBuf_GetNodeContent(&pstWinBP->stBuffer, u, &pstNode);

            // todo : release memory
        }
    }


    nRet = DispBuf_Destoy(&pstWinBP->stBuffer);

    return HI_SUCCESS;
}

HI_S32 WinBuf_Reset(WB_POOL_S *pstWinBP)
{
//    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);

    // todo
    

    
    pstWinBP->pstDisplay = HI_NULL;
    pstWinBP->pstConfig  = HI_NULL;

    pstWinBP->u32RlsWrite = 0;
    pstWinBP->u32RlsRead  = 0;

    return HI_SUCCESS;
}

HI_S32 WinBuf_SetSource(WB_POOL_S *pstWinBP, WB_SOURCE_INFO_S *pstSrc)
{
    WIN_CHECK_NULL_RETURN(pstWinBP);
    WIN_CHECK_NULL_RETURN(pstSrc);

    pstWinBP->stSrcInfo = *pstSrc;
    
    return HI_SUCCESS;
}

HI_S32 WinBuf_PutNewFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 nRet;
    DISP_BUF_NODE_S *pstNode;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);
    WIN_CHECK_NULL_RETURN(pstFrame);

    WinBuf_DebugAddTryQF(pstWinBP->pstDebugInfo);
    nRet = DispBuf_GetEmptyNode(&pstWinBP->stBuffer, &pstNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    nRet = DispBuf_DelEmptyNode(&pstWinBP->stBuffer, pstNode);
    if (nRet != HI_SUCCESS)
    {
        WIN_ERROR("DispBuf_DelEmptyNode failed, ID=0x%x\n", pstNode->u32ID);
        return HI_FAILURE;
    }

    memcpy(pstNode->u32Data, pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));

    nRet = DispBuf_AddFullNode(&pstWinBP->stBuffer, pstNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    WinBuf_DebugAddInput(pstWinBP->pstDebugInfo, pstFrame->u32FrameIndex);
    WinBuf_DebugAddQFOK(pstWinBP->pstDebugInfo);
    
    return HI_SUCCESS;
}

HI_DRV_VIDEO_FRAME_S *WinBuf_GetConfigFrame(WB_POOL_S *pstWinBP)
{
    DISP_BUF_NODE_S *pstNode;
    HI_DRV_VIDEO_FRAME_S *pstFrm;
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    nRet = DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstNode);
    if (nRet != HI_SUCCESS)
    {
        WinBuf_DebugAddUnderload(pstWinBP->pstDebugInfo);
        return HI_NULL;
    }

    nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    pstWinBP->pstConfig = pstNode;
    pstFrm = (HI_DRV_VIDEO_FRAME_S *)pstNode->u32Data;

    WinBuf_DebugAddCfg(pstWinBP->pstDebugInfo, pstFrm->u32FrameIndex);

    return  pstFrm;
}


// release frame that has been displayed and set configed frame as displayed frame.
HI_S32 WinBuf_RlsAndUpdateUsingFrame(WB_POOL_S *pstWinBP)
{
    HI_DRV_VIDEO_FRAME_S *pstDispFrame, *pstCfgFrame;
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);

    if (pstWinBP->pstDisplay == pstWinBP->pstConfig)
    {
        pstWinBP->pstConfig  = HI_NULL;
    }

    if (pstWinBP->pstDisplay != HI_NULL)
    {
        // get displayed frame
        pstDispFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstDisplay->u32Data;
        
        if (pstWinBP->pstConfig != HI_NULL)
        {
            // get last config frame, the frame is being displayed.
            pstCfgFrame = (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;
            
            // check whether disp-frame and conf-frame are same one.
            if (   pstWinBP->stSrcInfo.pfRlsFrame 
                && (   pstDispFrame->stBufAddr[0].u32PhyAddr_Y
                    != pstCfgFrame->stBufAddr[0].u32PhyAddr_Y)
                )
            {
                // if disp-frame and conf-frame are not same, release disp-frame
                pstWinBP->stSrcInfo.pfRlsFrame(pstWinBP->stSrcInfo.hSrc, pstDispFrame);
                WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstDispFrame->u32FrameIndex);
            }

            // release disp buffer
            nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWinBP->pstDisplay);
            DISP_ASSERT(nRet == HI_SUCCESS);

            pstWinBP->pstDisplay = pstWinBP->pstConfig;

            pstWinBP->pstConfig  = HI_NULL;
        }
#if 0
        else
        {
            if ( pstWinBP->stSrcInfo.pfRlsFrame )
            {
                // if not configed frame, may be EOS, release, release disp-frame
                pstWinBP->stSrcInfo.pfRlsFrame(pstWinBP->stSrcInfo.hSrc, pstDispFrame);
            }

            // release disp buffer
            nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWinBP->pstDisplay);
            DISP_ASSERT(nRet == HI_SUCCESS);

            pstWinBP->pstDisplay = HI_NULL;
        }
#endif
    }
    else if (pstWinBP->pstConfig != HI_NULL)
    {
        pstWinBP->pstDisplay = pstWinBP->pstConfig;

        pstWinBP->pstConfig = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 WinBuf_RepeatDisplayedFrame(WB_POOL_S *pstWinBP)
{
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    pstWinBP->pstConfig = pstWinBP->pstDisplay;

    return HI_NULL;
}

HI_DRV_VIDEO_FRAME_S *WinBuf_GetDisplayedFrame(WB_POOL_S *pstWinBP)
{
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    if (pstWinBP->pstDisplay != HI_NULL)
    {
        return (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstDisplay->u32Data;
    }

    return HI_NULL;
}

HI_DRV_VIDEO_FRAME_S *WinBuf_GetConfigedFrame(WB_POOL_S *pstWinBP)
{
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    if (pstWinBP->pstConfig != HI_NULL)
    {
        return (HI_DRV_VIDEO_FRAME_S *)pstWinBP->pstConfig->u32Data;
    }

    return HI_NULL;
}

HI_S32 WinBuf_ForceReleaseFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    WIN_CHECK_NULL_RETURN(pstWinBP);
    
   if (    (pstWinBP->stSrcInfo.pfRlsFrame != HI_NULL) && pstFrame)
    {
        pstWinBP->stSrcInfo.pfRlsFrame(pstWinBP->stSrcInfo.hSrc, pstFrame);
        WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstFrame->u32FrameIndex);
    }

    return HI_SUCCESS;
}

HI_S32 WinBuf_ReleaseOneFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstPreFrame)
{
    DISP_BUF_NODE_S *pstWBNode, *pstWBNextNode;
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame, *pstNextFrame;
    HI_U32 u32UsingAddrY;
    HI_S32 nRet;

    WIN_CHECK_NULL_RETURN(pstWinBP);

    if (pstPreFrame)
    {
        u32UsingAddrY = pstPreFrame->stBufAddr[0].u32PhyAddr_Y;
    }
    else
    {
        u32UsingAddrY = 0;
    }

    nRet = DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstWBNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;
    pstNextFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNextNode->u32Data;
    
    if (   (pstWinBP->stSrcInfo.pfRlsFrame != HI_NULL)
         &&(pstCurrFrame->stBufAddr[0].u32PhyAddr_Y != u32UsingAddrY)
         &&(pstCurrFrame->stBufAddr[0].u32PhyAddr_Y !=
            pstNextFrame->stBufAddr[0].u32PhyAddr_Y)
        )
    {
        // if the current frame is different with next frame and not using,
        // release it.
        pstWinBP->stSrcInfo.pfRlsFrame(pstWinBP->stSrcInfo.hSrc, pstCurrFrame);
        WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
    }

    // release node of current frame and add to empty array.
    nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    WinBuf_DebugAddDisacard(pstWinBP->pstDebugInfo);
        
    return HI_SUCCESS;
}

HI_S32 WinBuf_FlushWaitingFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstPreFrame)
{
    DISP_BUF_NODE_S *pstWBNode, *pstWBNextNode;
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame;
    HI_U32 u32UsingAddrY;
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);

    if (pstPreFrame)
    {
        u32UsingAddrY = pstPreFrame->stBufAddr[0].u32PhyAddr_Y;
    }
    else
    {
        u32UsingAddrY = 0;
    }

    nRet = DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstWBNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_SUCCESS;
    }

    nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);

    while(nRet == HI_SUCCESS)
    {
        WinBuf_ReleaseOneFrame(pstWinBP, pstPreFrame);

        // update position, 'pstWBNextNode' becomes current node, and get new next node
        pstWBNode = pstWBNextNode;

        nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);        
    }

    // all node is clean exept the last one, now release it.

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;
    
    if (   (pstWinBP->stSrcInfo.pfRlsFrame != HI_NULL)
         &&(pstCurrFrame->stBufAddr[0].u32PhyAddr_Y != u32UsingAddrY)
        )
    {
        pstWinBP->stSrcInfo.pfRlsFrame(pstWinBP->stSrcInfo.hSrc, pstCurrFrame);
        WinBuf_DebugAddRls(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
    }

    nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    nRet = DispBuf_AddEmptyNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    return HI_SUCCESS;
}

HI_DRV_VIDEO_FRAME_S * WinBuf_GetFrameByMaxID(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstRefFrame,HI_U32 u32RefID)
{
    DISP_BUF_NODE_S *pstWBNode, *pstWBNextNode;
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame, *pstNextFrame;
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    nRet = DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstWBNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_NULL;
    }

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;

    if ( (u32RefID == 0) || ((pstCurrFrame->u32FrameIndex == u32RefID)))
    {
        // release node of current frame and add to empty array.
        nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWBNode);
        DISP_ASSERT(nRet == HI_SUCCESS);

        pstWinBP->pstConfig = pstWBNode;

        // add to debug record array
        WinBuf_DebugAddCfg(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
        
        return pstCurrFrame;
    }

    // if current frame ID is bigger than refID, reserve and not use now.
    if (pstCurrFrame->u32FrameIndex > u32RefID)
    {
        return HI_NULL;
    }

    nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);
    while(nRet == HI_SUCCESS)
    {
        pstNextFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNextNode->u32Data;
        
        if (pstNextFrame->u32FrameIndex > u32RefID)
        {
            break;
        }

        WinBuf_ReleaseOneFrame(pstWinBP, pstRefFrame);

        // update position, 'pstWBNextNode' becomes current node, and get new next node
        pstWBNode = pstWBNextNode;

        nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);        
    }

    // get node
    nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    // all node is clean exept the last one, now release it.
    pstWinBP->pstConfig = pstWBNode;

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;

    WinBuf_DebugAddCfg(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
    
    return pstCurrFrame;
}


HI_DRV_VIDEO_FRAME_S * WinBuf_GetNewestFrame(WB_POOL_S *pstWinBP, HI_DRV_VIDEO_FRAME_S *pstRefFrame)
{
    DISP_BUF_NODE_S *pstWBNode, *pstWBNextNode;
    HI_DRV_VIDEO_FRAME_S *pstCurrFrame, *pstNextFrame;
    HI_S32 nRet;
    
    WIN_CHECK_NULL_RETURN_NULL(pstWinBP);

    nRet = DispBuf_GetFullNode(&pstWinBP->stBuffer, &pstWBNode);
    if (nRet != HI_SUCCESS)
    {
        return HI_NULL;
    }

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;

    nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);
    while(nRet == HI_SUCCESS)
    {
        pstNextFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNextNode->u32Data;
        
        WinBuf_ReleaseOneFrame(pstWinBP, pstRefFrame);

        // update position, 'pstWBNextNode' becomes current node, and get new next node
        pstWBNode = pstWBNextNode;

        nRet = DispBuf_GetNextFullNode(&pstWinBP->stBuffer, &pstWBNextNode);        
    }

    // get node
    nRet = DispBuf_DelFullNode(&pstWinBP->stBuffer, pstWBNode);
    DISP_ASSERT(nRet == HI_SUCCESS);

    // all node is clean exept the last one, now release it.
    pstWinBP->pstConfig = pstWBNode;

    pstCurrFrame = (HI_DRV_VIDEO_FRAME_S *)pstWBNode->u32Data;

    WinBuf_DebugAddCfg(pstWinBP->pstDebugInfo, pstCurrFrame->u32FrameIndex);
    
    return pstCurrFrame;
}



HI_S32 WinBuf_GetFullBufNum(WB_POOL_S *pstWinBP, HI_U32 *pu32BufNum)
{
    WIN_CHECK_NULL_RETURN(pstWinBP);
    WIN_CHECK_NULL_RETURN(pu32BufNum);

    return DispBuf_GetFullNodeNumber(&pstWinBP->stBuffer, pu32BufNum);
}

HI_S32 WinBuf_GetStateInfo(WB_POOL_S *pstWinBP, WB_STATE_S *pstWinBufState)
{
    DISP_BUF_NODE_S *pstNode;
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    HI_S32 i, nRet;
    
    WIN_CHECK_NULL_RETURN(pstWinBP);
    WIN_CHECK_NULL_RETURN(pstWinBufState);

    pstWinBufState->u32Number = pstWinBP->u32BufNumber;
    
    pstWinBufState->u32EmptyRPtr = pstWinBP->stBuffer.u32EmptyReadPos;
    pstWinBufState->u32EmptyWPtr = pstWinBP->stBuffer.u32EmptyWritePos;
    pstWinBufState->u32FullRPtr  = pstWinBP->stBuffer.u32FullReaddPos;
    pstWinBufState->u32FullWPtr  = pstWinBP->stBuffer.u32FullWritePos;

    for (i=0; i<pstWinBP->u32BufNumber; i++)
    {
        nRet = DispBuf_GetNodeContent(&pstWinBP->stBuffer, i, &pstNode);
        if ( nRet== HI_SUCCESS)
        {
            pstWinBufState->stNode[i].u32State = pstNode->u32State;
            pstWinBufState->stNode[i].u32Empty = pstNode->u32EmptyCount;
            pstWinBufState->stNode[i].u32Full  = pstNode->u32FullCount;

            pstFrame = (HI_DRV_VIDEO_FRAME_S *)pstNode->u32Data;
            pstWinBufState->stNode[i].u32FrameIndex = pstFrame->u32FrameIndex;
        }
        else
        {
            pstWinBufState->stNode[i].u32State = 0;
            pstWinBufState->stNode[i].u32Empty = 0;
            pstWinBufState->stNode[i].u32Full  = 0;
            pstWinBufState->stNode[i].u32FrameIndex = 0;
        }
    }

    memcpy(&pstWinBufState->stRecord, pstWinBP->pstDebugInfo, sizeof(WB_DEBUG_INFO_S));

    memcpy(&pstWinBufState->u32EmptyArray, pstWinBP->stBuffer.u32EmptyArray, sizeof(HI_U32)*DISP_BUF_NODE_MAX_NUMBER);
    memcpy(&pstWinBufState->u32FullArray, pstWinBP->stBuffer.u32FullArray, sizeof(HI_U32)*DISP_BUF_NODE_MAX_NUMBER);

    pstFrame = WinBuf_GetDisplayedFrame(pstWinBP);
    if (pstFrame)
    {
        memcpy(&pstWinBufState->stCurrentFrame, pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




