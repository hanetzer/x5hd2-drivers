
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_window.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_display.h"

#include "drv_window.h"
#include "drv_win_priv.h"
#include "drv_sys_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/******************************************************************************
    global object
******************************************************************************/
static volatile HI_S32 s_s32WindowGlobalFlag = WIN_DEVICE_STATE_CLOSE;
static DISPLAY_WINDOW_S stDispWindow;

/******************************************************************************
    local function and macro
******************************************************************************/
#define WinGetType(pstWin)    (pstWin->enType)
#define WinGetLayerID(pstWin) (pstWin->eLayer)
#define WinGetDispID(pstWin)  (pstWin->enDisp)

#define WinCheckDeviceOpen()    \
{                                \
    if (WIN_DEVICE_STATE_OPEN != s_s32WindowGlobalFlag)  \
    {                            \
        WIN_ERROR("WIN is not inited or suspended in %s!\n", __FUNCTION__); \
        return HI_ERR_VO_NO_INIT;  \
    }                             \
}

#define WinCheckNullPointer(ptr) \
{                                \
    if (!ptr)                    \
    {                            \
        WIN_ERROR("WIN Input null pointer in %s!\n", __FUNCTION__); \
        return HI_ERR_VO_NULL_PTR;  \
    }                             \
}

// 检查句柄合法性
#define WinCheckWindow(hWin, pstWin) \
{                                    \
    pstWin = WinGetWindow(hWin);     \
    if (!pstWin)                      \
    {                                \
        WIN_ERROR("WIN is not exist!\n"); \
        return HI_ERR_VO_WIN_NOT_EXIST; \
    }  \
}

// 检查从窗口状态
#define WinCheckSlaveWindow(pstWin) \
{                                   \
    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin)) \
    {                               \
        if (!pstWin->hSlvWin)        \
        {                           \
            WIN_ERROR("WIN Slave window is lost in %s\n", __FUNCTION__); \
            return HI_ERR_VO_SLAVE_WIN_LOST;    \
        }                           \
    }                               \
}

HI_VOID ISR_CallbackForDispModeChange(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);
HI_VOID ISR_CallbackForWinProcess(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo);

/******************************************************************************
    Win device function
******************************************************************************/
HI_S32 WIN_Init(HI_VOID)
{
    HI_BOOL bDispInitFlag;
    HI_DRV_DISP_VERSION_S stVerison;
//    HI_S32 nRet;
    
    if (WIN_DEVICE_STATE_CLOSE != s_s32WindowGlobalFlag)
    {
        WIN_INFO("VO has been inited!\n");
        return HI_SUCCESS;
    }


    // s1 检查DISP是否有初始化
    DISP_GetInitFlag(&bDispInitFlag);
    if (HI_TRUE != bDispInitFlag)
    {
        WIN_ERROR("Display is not inited!\n");
        return HI_ERR_VO_DEPEND_DEVICE_NOT_READY;
    }

    if (HI_SUCCESS != BP_CreateBlackFrame())
    {
        WIN_ERROR("Create Black Frame failed!\n");
        return HI_ERR_VO_MALLOC_FAILED;
    }

    // s2 初始化window，获取surface操作函数
    DISP_MEMSET(&stDispWindow, 0, sizeof(DISPLAY_WINDOW_S));
    
    DISP_GetVersion(&stVerison);

    VideoLayer_Init(&stVerison);


    // s3 设置初始化标志
    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_OPEN;

    return HI_SUCCESS;
}


HI_S32 WIN_DeInit(HI_VOID)
{
    HI_S32 i,j;

    if (WIN_DEVICE_STATE_CLOSE == s_s32WindowGlobalFlag)
    {
        WIN_INFO("VO is not inited!\n");
        return HI_SUCCESS;
    }

    // s1 关闭所有wiondw
    for(i=0; i<HI_DRV_DISPLAY_BUTT; i++)
    {
        for(j=0; j<WINDOW_MAX_NUMBER; j++)
        {
            if (stDispWindow.pstWinArray[i][j])
            {
                WIN_Destroy(stDispWindow.pstWinArray[i][j]->u32Index);
                stDispWindow.pstWinArray[i][j] = HI_NULL;
            }
        }
    }
    
    stDispWindow.u32WinNumber = 0;

    // s2 释放资源
    VideoLayer_DeInit();


    BP_DestroyBlackFrame();

    // s3 设置标志
    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_CLOSE;
    WIN_INFO("VO has been DEinited!\n");

    return HI_SUCCESS;
}

HI_S32 WIN_Suspend(HI_VOID)
{
    WinCheckDeviceOpen();

    s_s32WindowGlobalFlag = WIN_DEVICE_STATE_SUSPEND;

    return HI_SUCCESS;
}

HI_S32 WIN_Resume(HI_VOID)
{

    if (s_s32WindowGlobalFlag == WIN_DEVICE_STATE_SUSPEND)
    {
        VIDEO_LAYER_FUNCTIONG_S *pF = VideoLayer_GetFunctionPtr();

        s_s32WindowGlobalFlag = WIN_DEVICE_STATE_OPEN;

        pF->PF_SetAllLayerDefault();
    }

    return HI_SUCCESS;
}

HI_S32 WinTestAddWindow(HI_VOID)
{
    if (stDispWindow.u32WinNumber < WINDOW_MAX_NUMBER)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_U32 WinGetPrefix(HI_U32 u32WinIndex)
{
    return (HI_U32)(u32WinIndex & WIN_INDEX_PREFIX_MASK);
}

HI_U32 WinGetDispId(HI_U32 u32WinIndex)
{
    return (HI_U32)((u32WinIndex >> WIN_INDEX_DISPID_SHIFT_NUMBER) & WIN_INDEX_DISPID_MASK);
}

HI_U32 WinGetId(HI_U32 u32WinIndex)
{
    return (HI_U32)(u32WinIndex & WINDOW_INDEX_NUMBER_MASK);
}

HI_U32 WinMakeIndex(HI_DRV_DISPLAY_E enDisp, HI_U32 u32WinIndex)
{
    return (HI_U32)(   WIN_INDEX_PREFIX
                     | ( ( (HI_U32)enDisp & WIN_INDEX_DISPID_MASK) \
                          << WIN_INDEX_DISPID_SHIFT_NUMBER
                       )
                     |(u32WinIndex& WINDOW_INDEX_NUMBER_MASK)
                    );
}

HI_S32 WinAddWindow(HI_DRV_DISPLAY_E enDisp, WINDOW_S *pstWin)
{
    HI_S32 i;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        return HI_FAILURE;
    }

    for(i=0; i<WINDOW_MAX_NUMBER; i++)
    {
        if (!stDispWindow.pstWinArray[(HI_U32)enDisp][i])
        {
            pstWin->u32Index =  WinMakeIndex(enDisp, (HI_U32)i);

            stDispWindow.pstWinArray[(HI_U32)enDisp][i] = pstWin;
            stDispWindow.u32WinNumber++;
            //printk(">>>>>>>>>>>>>>>> %s, %d\n", __FUNCTION__, __LINE__);
            return HI_SUCCESS;
        }
    }

    //printk(">>>>>>>>>>>>>>>> %s, %d\n", __FUNCTION__, __LINE__);

    return HI_FAILURE;
}



HI_S32 WinDelWindow(HI_U32 u32WinIndex)
{
    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if (  WinGetDispId(u32WinIndex) >= HI_DRV_DISPLAY_BUTT)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if ( WinGetId(u32WinIndex) >= WINDOW_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_FAILURE;
    }

    if (!stDispWindow.u32WinNumber)
    {
        WIN_ERROR("Not found this window!\n");
        return HI_FAILURE;
    }

    if (stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)])
    {
        stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)] = HI_NULL;
        stDispWindow.u32WinNumber--;
    }
    else
    {
        WIN_ERROR("Not found this window!\n");
    }

    return HI_SUCCESS;
}


WINDOW_S *WinGetWindow(HI_U32 u32WinIndex)
{
    if (!stDispWindow.u32WinNumber)
    {
        WIN_ERROR("Not found this window!\n");
        return HI_NULL;
    }

    //同时检查窗口与从窗口
    if ( WinGetPrefix(u32WinIndex) != WIN_INDEX_PREFIX)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if (  WinGetDispId(u32WinIndex) >= HI_DRV_DISPLAY_BUTT)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    if ( WinGetId(u32WinIndex) >= WINDOW_MAX_NUMBER)
    {
        WIN_ERROR("Invalid window index = 0x%x\n", u32WinIndex);
        return HI_NULL;
    }

    return stDispWindow.pstWinArray[WinGetDispId(u32WinIndex)][WinGetId(u32WinIndex)];
}



/******************************************************************************
    internal function
******************************************************************************/
HI_U32 WinParamAlignUp(HI_U32 x, HI_U32 a)
{
    if (!a)
    {
        return x;
    }
    else
    {
        return ( (( x + (a-1) ) / a ) * a);
    }
}

HI_U32 WinParamAlignDown(HI_U32 x, HI_U32 a)
{
    if (!a)
    {
        return x;
    }
    else
    {
        return (( x / a ) * a);
    }
}

HI_VOID WinUpdateRectBaseRect(HI_RECT_S *Old, HI_RECT_S * New, HI_RECT_S *in, HI_RECT_S *Out)
{
    HI_RECT_S stTmp;

    if (   (New->s32X == Old->s32X) && (New->s32Y == Old->s32Y)
        && (New->s32Width == Old->s32Width) && (New->s32Height == Old->s32Height)
    )
    {
        return;
    }

    stTmp.s32X = (in->s32X * New->s32Width) / Old->s32Width;
    stTmp.s32Y = (in->s32Y * New->s32Height) / Old->s32Height;
    stTmp.s32Width = (in->s32Width * New->s32Width) / Old->s32Width;
    stTmp.s32Height = (in->s32Height * New->s32Height) / Old->s32Height;

    Out->s32X = WinParamAlignUp(stTmp.s32X, 2);
    Out->s32Y = WinParamAlignUp(stTmp.s32Y, 2);
    Out->s32Width  = WinParamAlignUp(stTmp.s32Width, 2);
    Out->s32Height = WinParamAlignUp(stTmp.s32Height, 2);

    return;
}

#if 0
HI_S32 WinProduceNewAttr(WINDOW_S *pstWin, HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
    pstWin->stNewAttr = pstWin->stCfg.stAttr;

    WinUpdateRectBaseRect(&pstWin->stCfg.stRefScreen, 
                            (HI_RECT_S *)&pstDispInfo->stOrgRect,
                            &pstWin->stCfg.stRefOutRect,
                            &pstWin->stUsingAttr.stOutRect);

    // TODO: 超出屏幕的处理

    if (!pstWin->stNewAttr.stOutRect.s32Width)
    {
        pstWin->stNewAttr.stOutRect = pstDispInfo->stOrgRect;
    }

    pstWin->bToUpdateAttr = HI_TRUE;

    return HI_SUCCESS;
}
#endif

HI_S32 WinBufferReset(WIN_BUFFER_S *pstBuffer);
//HI_VOID ISR_WinReleaseDisplayedFrame(WINDOW_S *pstWin);
//HI_VOID ISR_WinReleaseFullFrame(WINDOW_S *pstWin);
//HI_VOID ISR_WinReleaseDisplayedFrame2(WINDOW_S *pstWin);
HI_VOID ISR_WinReleaseUSLFrame(WINDOW_S *pstWin);

HI_S32 WinCreateDisplayWindow(HI_DRV_WIN_ATTR_S *pWinAttr, WINDOW_S **ppstWin)
{
    //HI_DISP_DISPLAY_INFO_S stDispInfo;
    WINDOW_S *pstWin;
    HI_S32 nRet;

    if(WinTestAddWindow())
    {
        WIN_ERROR("Reach max window number,can not create!\n");
        return HI_ERR_VO_CREATE_ERR;
    }

    // s1 创建window
    pstWin = (WINDOW_S *)DISP_MALLOC(sizeof(WINDOW_S));
    if (!pstWin)
    {
        WIN_ERROR("Malloc WINDOW_S failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    DISP_MEMSET(pstWin, 0, sizeof(WINDOW_S));

    // s2 initial parameters
/*
    nRet = DISP_GetDisplayInfo(pWinAttr->enDisp, &stDispInfo);
    if (nRet)
    {
        WIN_ERROR("DISP_GetDisplayInfo failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }
*/
    /* attribute */
    pstWin->bEnable = HI_FALSE;
    pstWin->bMasked = HI_FALSE;

    pstWin->enState = WIN_STATE_WORK;
    //pstWin->enStateBackup = pstWin->enState;
    pstWin->bUpState      = HI_FALSE;

    pstWin->enDisp = pWinAttr->enDisp;
    pstWin->enType = HI_DRV_WIN_ACTIVE_SINGLE;

    pstWin->stCfg.stAttrBuf = *pWinAttr;
    atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);

    nRet = VideoLayer_GetFunction(&pstWin->stVLayerFunc);
    if (nRet)
    {
        WIN_ERROR("VideoLayer_GetFunction failed in %s!\n", __FUNCTION__);
        goto __ERR_GET_FUNC__;
    }

    // s2 获取空闲视频层
    //nRet = pstWin->stVLayerFunc.PF_AcquireLayerByDisplay(WinGetDispID(pstWin), &pstWin->eLayer);
    nRet = pstWin->stVLayerFunc.PF_AcquireLayerByDisplay(pstWin->enDisp, &pstWin->u32VideoLayer);
    if (nRet)
    {
        WIN_ERROR("PF_AcquireLayerByDisplay failed in %s!\n", __FUNCTION__);
        goto __ERR_GET_FUNC__;
    }

    nRet = pstWin->stVLayerFunc.PF_SetDefault(pstWin->u32VideoLayer);
    if (nRet)
    {
        WIN_ERROR("PF_SetDefault failed in %s!\n", __FUNCTION__);
        goto __ERR_GET_FUNC__;
    }

    // s3 将window置于最上层
    pstWin->stVLayerFunc.PF_MovTop(pstWin->u32VideoLayer);

    // s4 create buffer
    //nRet = BP_Create(WIN_IN_FB_DEFAULT_NUMBER, HI_NULL, &pstWin->stBuffer.stBP);
    nRet = WinBuf_Create(WIN_IN_FB_DEFAULT_NUMBER, WIN_BUF_MEM_SRC_SUPPLY, HI_NULL, &pstWin->stBuffer.stWinBP);
    if(nRet)
    {
        WIN_ERROR("Create buffer pool failed\n");
        goto __ERR_GET_FUNC__;
    }

    WinBufferReset(&pstWin->stBuffer);

    // initial reset
    pstWin->bReset = HI_FALSE;
    pstWin->bConfigedBlackFrame = HI_FALSE;

    // initial quickmode
    pstWin->bQuickMode = HI_FALSE;

    // initial stepmode flag
    pstWin->bStepMode = HI_FALSE;

    *ppstWin = pstWin;

    return HI_SUCCESS;


__ERR_GET_FUNC__:

    DISP_FREE(pstWin);

    return HI_ERR_VO_CREATE_ERR;
}

HI_S32 WinDestroyDisplayWindow(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    
    // s0 flush frame
#if 0
    ISR_WinReleaseDisplayedFrame(pstWin);

    ISR_WinReleaseFullFrame(pstWin);

    ISR_WinReleaseUSLFrame(pstWin);

    ISR_WinReleaseDisplayedFrame2(pstWin);
    ISR_WinReleaseDisplayedFrame(pstWin);
#endif

    //ISR_WinReleaseDisplayedFrame(pstWin);
    WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
    
    ISR_WinReleaseUSLFrame(pstWin);

    // flush frame in full buffer pool
    pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
    WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);

    // release current frame
    WinBuf_ForceReleaseFrame(&pstWin->stBuffer.stWinBP, pstFrame);


    // s1 derstoy buffer    
    //BP_Destroy(&pstWin->stBuffer.stBP);
    WinBuf_Destroy(&pstWin->stBuffer.stWinBP);

    // s2 释放空闲视频层
    pstWin->stVLayerFunc.PF_ReleaseLayer(pstWin->u32VideoLayer);

    // s3 销毁window
    DISP_FREE(pstWin);

    return HI_SUCCESS;

}

HI_S32 WinRegCallback(WINDOW_S *pstWin)
{
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet= HI_SUCCESS;
    
    // s1 注册回调函数
    stCB.hDst  = (HI_HANDLE)pstWin;
    stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
    nRet = DISP_RegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
    if (nRet)
    {
        WIN_ERROR("WIN register callback failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    return HI_SUCCESS;
}

HI_S32 WinUnRegCallback(WINDOW_S *pstWin)
{
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet= HI_SUCCESS;
    
    // s1 注销回调函数
    stCB.hDst  = (HI_HANDLE)pstWin;
    stCB.pfDISP_Callback = ISR_CallbackForWinProcess;
    nRet = DISP_UnRegCallback(WinGetDispID(pstWin), HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);

    return HI_SUCCESS;
}


HI_VOID WinSetBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        pstWin->bConfigedBlackFrame = HI_TRUE;
    }
}

HI_VOID WinClearBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        pstWin->bConfigedBlackFrame = HI_FALSE;
    }
}

HI_BOOL WinTestBlackFrameFlag(WINDOW_S *pstWin)
{
    if (pstWin)
    {
        return pstWin->bConfigedBlackFrame;
    }
    return HI_FALSE;
}


HI_S32 WinCreateVirtualWindow(HI_DRV_WIN_ATTR_S *pWinAttr, WINDOW_S **ppstWin)
{
    // s1 创建window
#if 0
        pWindow = kmalloc(sizeof(WINDOW_S));
        if (!pWindow)
        {
            WIN_ERROR("WIN Malloc WINDOW_S failed in %s!\n", __FUNCTION__);
            return HI_ERR_VO_CREATE_ERR;
        }
#endif

    // s2 申请buffer



HI_S32 VideoSurface_GetFunction(VIDEO_LAYER_FUNCTIONG_S *pstFunc);


    return HI_SUCCESS;

}

HI_S32 WinDestroyVirtualWindow(WINDOW_S *pstWin)
{

    
    // s2 释放buffer

    // s1 销毁window

    return HI_SUCCESS;

}


//固定参数保持
HI_S32 WinCheckFixedAttr(HI_DRV_WIN_ATTR_S *pOldAttr, HI_DRV_WIN_ATTR_S *pNewAttr)
{
    if (  (pOldAttr->enDisp != pNewAttr->enDisp)
        ||(pOldAttr->bVirtual != pNewAttr->bVirtual)
        )
    {
        return HI_FAILURE;
    }

    if (pOldAttr->bVirtual)
    {
        if (  (pOldAttr->bUserAllocBuffer != pNewAttr->bUserAllocBuffer)
            ||(pOldAttr->u32BufNumber != pNewAttr->u32BufNumber)
            )
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 WinCheckAttr(HI_DRV_WIN_ATTR_S *pstAttr)
{
    //HI_DISP_DISPLAY_INFO_S stInfo;
    //DISP_GetDisplayInfo(pstAttr->eDisp, &stInfo);
    
    if (pstAttr->bVirtual)
    {
        WIN_FATAL("WIN do not support virtual window!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pstAttr->enDisp > HI_DRV_DISPLAY_1)
    {
        WIN_FATAL("WIN only support HI_DRV_DISPLAY_0!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (   ( pstAttr->stCustmAR.u8ARw > (pstAttr->stCustmAR.u8ARh * WIN_MAX_ASPECT_RATIO) )
        || ( (pstAttr->stCustmAR.u8ARw * WIN_MAX_ASPECT_RATIO) < pstAttr->stCustmAR.u8ARh)
        )
    {
        HI_ERR_WIN("bUserDefAspectRatio  error!\n");
        return HI_FAILURE;
    }

    if ( pstAttr->bUseCropRect)
    {
        if(    (pstAttr->stCropRect.u32TopOffset    > WIN_CROPRECT_MAX_OFFSET_TOP)
            || (pstAttr->stCropRect.u32LeftOffset   > WIN_CROPRECT_MAX_OFFSET_LEFT)
            || (pstAttr->stCropRect.u32BottomOffset > WIN_CROPRECT_MAX_OFFSET_BOTTOM)
            || (pstAttr->stCropRect.u32RightOffset  > WIN_CROPRECT_MAX_OFFSET_RIGHT)
           )
        {
            WIN_FATAL("WIN OutRect/InRect support 128*64 ~ 1920*1920!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }
    else
    {
        if ( !pstAttr->stInRect.s32Height || !pstAttr->stInRect.s32Width)
        {
            DISP_MEMSET(&pstAttr->stInRect, 0, sizeof(HI_RECT_S));
        }
        else if(   (pstAttr->stInRect.s32Width   < WIN_INRECT_MIN_WIDTH)
                || (pstAttr->stInRect.s32Height  < WIN_INRECT_MIN_HEIGHT)
                || (pstAttr->stInRect.s32Width   > WIN_INRECT_MAX_WIDTH)
                || (pstAttr->stInRect.s32Height  > WIN_INRECT_MAX_HEIGHT)
                )
        {
            WIN_FATAL("WIN InRect support 128*64 ~ 1920*1920!\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }

    if ( !pstAttr->stOutRect.s32Height || !pstAttr->stOutRect.s32Width)
    {
        DISP_MEMSET(&pstAttr->stOutRect, 0, sizeof(HI_RECT_S));
    }
    else if(   (pstAttr->stOutRect.s32Width   < WIN_OUTRECT_MIN_WIDTH)
            || (pstAttr->stOutRect.s32Height  < WIN_OUTRECT_MIN_HEIGHT)
            || (pstAttr->stOutRect.s32Width   > WIN_OUTRECT_MAX_WIDTH)
            || (pstAttr->stOutRect.s32Height  > WIN_OUTRECT_MAX_HEIGHT)
            )
    {
        WIN_FATAL("WIN OutRect support 128*64 ~ 1920*1920!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    /* may change when window lives */
    pstAttr->stInRect.s32X = pstAttr->stInRect.s32X & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stInRect.s32Y = pstAttr->stInRect.s32Y & HI_WIN_IN_RECT_Y_ALIGN;
    pstAttr->stInRect.s32Width = pstAttr->stInRect.s32Width & HI_WIN_IN_RECT_WIDTH_ALIGN;
    pstAttr->stInRect.s32Height = pstAttr->stInRect.s32Height & HI_WIN_IN_RECT_HEIGHT_ALIGN;

    pstAttr->stCropRect.u32LeftOffset   = pstAttr->stCropRect.u32LeftOffset   & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stCropRect.u32RightOffset  = pstAttr->stCropRect.u32RightOffset  & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stCropRect.u32TopOffset    = pstAttr->stCropRect.u32TopOffset    & HI_WIN_IN_RECT_Y_ALIGN;
    pstAttr->stCropRect.u32BottomOffset = pstAttr->stCropRect.u32BottomOffset & HI_WIN_IN_RECT_Y_ALIGN;

    pstAttr->stOutRect.s32X = pstAttr->stOutRect.s32X & HI_WIN_IN_RECT_X_ALIGN;
    pstAttr->stOutRect.s32Y = pstAttr->stOutRect.s32Y & HI_WIN_IN_RECT_Y_ALIGN;
    pstAttr->stOutRect.s32Width  = pstAttr->stOutRect.s32Width  & HI_WIN_IN_RECT_WIDTH_ALIGN;
    pstAttr->stOutRect.s32Height = pstAttr->stOutRect.s32Height & HI_WIN_IN_RECT_HEIGHT_ALIGN;
    

    return HI_SUCCESS;
}


HI_S32 WinCheckSourceInfo(HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    if (!pstSrc->pfAcqFrame && !pstSrc->pfRlsFrame && !pstSrc->pfSendWinInfo)
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 WinSendAttrToSource(WINDOW_S *pstWin, HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
    if (pstWin->stCfg.stSource.pfSendWinInfo)
    {
        HI_DRV_WIN_PRIV_INFO_S stInfo;
        HI_DRV_WIN_ATTR_S *pstAttr;
        
        DISP_MEMSET(&stInfo, 0, sizeof(HI_DRV_WIN_PRIV_INFO_S));

        pstAttr = &pstWin->stUsingAttr;

        //stInfo.ePixFmt      = pstAttr->enDataFormat;
        stInfo.ePixFmt      = HI_DRV_PIX_FMT_NV21;
        stInfo.bUseCropRect = pstAttr->bUseCropRect;
        stInfo.stInRect     = pstAttr->stInRect;

        /*
        stInfo.stInRect.s32X = 0;
        stInfo.stInRect.s32Y = 0;
        stInfo.stInRect.s32Width  = 0;
        stInfo.stInRect.s32Height = 0;
        */
        stInfo.stCropRect   = pstAttr->stCropRect;

        if (!pstAttr->stOutRect.s32Width || !pstAttr->stOutRect.s32Height)
        {
            stInfo.stOutRect = pstDispInfo->stRefRect;
        }
        else
        {
            stInfo.stOutRect = pstAttr->stOutRect;
        }

        stInfo.stScreenAR      = pstDispInfo->stAR;
        
        stInfo.stCustmAR    = pstAttr->stCustmAR;
        stInfo.enARCvrs     = pstAttr->enARCvrs;
        
        
        stInfo.bUseExtBuf   = pstAttr->bUserAllocBuffer;

        stInfo.u32MaxRate  = (pstDispInfo->u32RefreshRate > WIN_TRANSFER_CODE_MAX_FRAME_RATE) ?
                 (pstDispInfo->u32RefreshRate / 2 ) : pstDispInfo->u32RefreshRate;

        stInfo.bInterlaced = pstDispInfo->bInterlace;
        stInfo.stScreen    = pstDispInfo->stRefRect;
        //stInfo.u32Rate     = pstDispInfo->u32RefreshRate;

        /*
        SET TEST 1
        */
        #if 0
        stInfo.stInRect.s32Height = 476;
        stInfo.stInRect.s32Width  = 720;
        stInfo.stInRect.s32X      = 0;
        stInfo.stInRect.s32Y      = 50;
        stInfo.bUseCropRect = HI_FALSE;
        stInfo.stCropRect   = pstAttr->stCropRect; 
        #endif
        /*
            SET TEST 2
          */
         #if 0
        stInfo.stCropRect.u32BottomOffset = 0;
        stInfo.stCropRect.u32TopOffset    = 0;
        stInfo.stCropRect.u32LeftOffset   = 0;
        stInfo.stCropRect.u32RightOffset  = 0;
        stInfo.bUseCropRect = HI_TRUE;
        #endif
        DISP_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>will send info to source .............\n");
        pstWin->stCfg.stSource.pfSendWinInfo(pstWin->stCfg.stSource.hSrc, &stInfo);
    }

    return HI_SUCCESS;
}


HI_S32 WinCalcDispRectBaseRefandRel(HI_RECT_S *pRef, HI_RECT_S *pRel, 
                                             HI_RECT_S *pI, HI_RECT_S *pO)
{
    pO->s32X      = (pI->s32X * pRel->s32Width) / pRef->s32Width;
    pO->s32Width  = (pI->s32Width * pRel->s32Width) / pRef->s32Width;
    pO->s32Y      = (pI->s32Y* pRel->s32Height) / pRef->s32Height;
    pO->s32Height = (pI->s32Height* pRel->s32Height) / pRef->s32Height;

    pO->s32X      = pO->s32X & HI_WIN_IN_RECT_X_ALIGN;
    pO->s32Width  = pO->s32Width & HI_WIN_IN_RECT_WIDTH_ALIGN;
    pO->s32Y      = pO->s32Y & HI_WIN_IN_RECT_Y_ALIGN;
    pO->s32Height = pO->s32Height & HI_WIN_IN_RECT_HEIGHT_ALIGN;

    return HI_SUCCESS;
}

HI_S32 WinCalcCropRectBaseRefandRel(HI_RECT_S *pRef, HI_RECT_S *pRel, 
                                              HI_DRV_CROP_RECT_S *pI, 
                                              HI_DRV_CROP_RECT_S *pO)
{
    pO->u32LeftOffset   = (pI->u32LeftOffset * pRel->s32Width) / pRef->s32Width;
    pO->u32RightOffset  = (pI->u32RightOffset * pRel->s32Width) / pRef->s32Width;
    pO->u32TopOffset    = (pI->u32TopOffset * pRel->s32Height) / pRef->s32Height;
    pO->u32BottomOffset = (pI->u32BottomOffset * pRel->s32Height) / pRef->s32Height;

    pO->u32LeftOffset   = pO->u32LeftOffset   & HI_WIN_IN_RECT_X_ALIGN;
    pO->u32RightOffset  = pO->u32RightOffset  & HI_WIN_IN_RECT_X_ALIGN;
    pO->u32TopOffset    = pO->u32TopOffset    & HI_WIN_IN_RECT_Y_ALIGN;
    pO->u32BottomOffset = pO->u32BottomOffset & HI_WIN_IN_RECT_Y_ALIGN;

    return HI_SUCCESS;
}

HI_S32 WinGetSlaveWinAttr(HI_DRV_WIN_ATTR_S *pWinAttr, 
                               HI_DRV_WIN_ATTR_S *pSlvWinAttr)
{
    HI_DISP_DISPLAY_INFO_S stM, stS;
    HI_S32 nRet;

    DISP_MEMSET(pSlvWinAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    // s1 get slave display    
    nRet = DISP_GetSlave(pWinAttr->enDisp, &pSlvWinAttr->enDisp);
    if (nRet)
    {
        WIN_ERROR("Get slave Display failed\n");
        return nRet;
    }

    if (!DISP_IsOpened(pSlvWinAttr->enDisp) )
    {
        WIN_ERROR("Slave Display is not open\n");
        return HI_FAILURE;
    }

    // s2 get master and slave display info
    pSlvWinAttr->bVirtual = HI_FALSE;

    /* may change when window lives */
    pSlvWinAttr->stCustmAR = pWinAttr->stCustmAR;
    pSlvWinAttr->enARCvrs  = pWinAttr->enARCvrs;

    nRet = DISP_GetDisplayInfo(pWinAttr->enDisp, &stM);
    nRet = DISP_GetDisplayInfo(pSlvWinAttr->enDisp, &stS);

    pSlvWinAttr->bUseCropRect = pWinAttr->bUseCropRect;
    pSlvWinAttr->stInRect   = pWinAttr->stInRect;
    pSlvWinAttr->stCropRect = pWinAttr->stCropRect;

    WinCalcDispRectBaseRefandRel(&stM.stRefRect,
                                 &stS.stRefRect, 
                                 &pWinAttr->stOutRect,
                                 &pSlvWinAttr->stOutRect);

    return HI_SUCCESS;
}


HI_S32 WinGetSlaveWinAttr2(WINDOW_S *pstWin,
                                 HI_DRV_WIN_ATTR_S *pWinAttr, 
                                 HI_DRV_WIN_ATTR_S *pSlvWinAttr)
{
    HI_DISP_DISPLAY_INFO_S stM, stS;
    WINDOW_S *pstSlvWin = WinGetWindow(pstWin->hSlvWin);
    HI_S32 nRet;

    /* may change when window lives */
    pSlvWinAttr->stCustmAR = pWinAttr->stCustmAR;
    pSlvWinAttr->enARCvrs  = pWinAttr->enARCvrs;

    nRet = DISP_GetDisplayInfo(pstWin->enDisp, &stM);
    nRet = DISP_GetDisplayInfo(pstSlvWin->enDisp, &stS);

    pSlvWinAttr->bUseCropRect = pWinAttr->bUseCropRect;
    pSlvWinAttr->stInRect   = pWinAttr->stInRect;
    pSlvWinAttr->stCropRect = pWinAttr->stCropRect;

    WinCalcDispRectBaseRefandRel(&stM.stRefRect,
                                 &stS.stRefRect, 
                                 &pWinAttr->stOutRect,
                                 &pSlvWinAttr->stOutRect);

    return HI_SUCCESS;
}



HI_S32 WinTestZero(volatile HI_U32 *pLock, HI_U32 u32MaxTimeIn10ms)
{
    volatile HI_U32 nLockState;
    HI_U32 u = 0;

    while(u < u32MaxTimeIn10ms)
    {
        nLockState = *pLock;
        if (!nLockState)
        {
            return HI_SUCCESS;
        }

        DISP_MSLEEP(10);
        u++;
    }

    return HI_ERR_VO_TIMEOUT;
}


HI_S32 WinCheckFrame(HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
	HI_DRV_VIDEO_PRIVATE_S *pstPriv = (HI_DRV_VIDEO_PRIVATE_S*)&(pFrameInfo->u32Priv[0]);

/*printk("typ2=%d, fmt=%d, w=%d,h=%d, time=%d\n",   
	pFrameInfo->eFrmType,
	pFrameInfo->ePixFormat,
	pFrameInfo->u32Width,
	pFrameInfo->u32Height,
    pFrameInfo->u32PlayTime);
*/
	pstPriv->u32PlayTime = 1;

    if (pFrameInfo->eFrmType >  HI_DRV_FT_BUTT)
    {
        WIN_FATAL("Q Frame type error : %d\n", pFrameInfo->eFrmType);
    	return HI_ERR_VO_INVALID_PARA;
    }

    if (!(    (HI_DRV_PIX_FMT_NV12 == pFrameInfo->ePixFormat)
		    || (HI_DRV_PIX_FMT_NV21 == pFrameInfo->ePixFormat)
		  )
	   )
    {
        WIN_FATAL("Q Frame pixformat error : %d\n", pFrameInfo->ePixFormat);
    	return HI_ERR_VO_INVALID_PARA;
    }
   
    if (    (pFrameInfo->u32Width < WIN_FRAME_MIN_WIDTH)
		 || (pFrameInfo->u32Width > WIN_FRAME_MAX_WIDTH)
		 || (pFrameInfo->u32Height < WIN_FRAME_MIN_HEIGHT)
		 || (pFrameInfo->u32Height > WIN_FRAME_MAX_HEIGHT)
		)
    {
        WIN_FATAL("Q Frame resolution error : w=%d,h=%d\n", 
                     pFrameInfo->u32Width, pFrameInfo->u32Height);
        return HI_ERR_VO_INVALID_PARA;
    }

    pFrameInfo->stDispRect.s32X = 0;
    pFrameInfo->stDispRect.s32Y = 0;
    pFrameInfo->stDispRect.s32Width  = pFrameInfo->u32Width;
    pFrameInfo->stDispRect.s32Height = pFrameInfo->u32Height;

/*
    if (   (pFrameInfo->stDispRect.s32X < 0)
		|| (pFrameInfo->stDispRect.s32Width <  0)
		|| ((pFrameInfo->stDispRect.s32Width + pFrameInfo->stDispRect.s32X) >  pFrameInfo->u32Width)
		|| (pFrameInfo->stDispRect.s32Y < 0)
		|| (pFrameInfo->stDispRect.s32Height < 0)
		|| ((pFrameInfo->stDispRect.s32Height + pFrameInfo->stDispRect.s32Y) >  pFrameInfo->u32Height)
		)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }

    if (  (pFrameInfo->stDispAR.u8ARh > (pFrameInfo->stDispAR.u8ARw * WIN_MAX_ASPECT_RATIO))
		||(pFrameInfo->stDispAR.u8ARw > (pFrameInfo->stDispAR.u8ARh  * WIN_MAX_ASPECT_RATIO))
		)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->u32FrameRate >  WIN_MAX_FRAME_RATE)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->eColorSpace >  HI_DRV_CS_SMPT240M)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }

    if (pFrameInfo->u32PlayTime >  WIN_MAX_FRAME_PLAY_TIME)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }

    // stBufAddr[1] is right eye for stereo video 
    if (    (pFrameInfo->stBufAddr[0].u32Stride_Y < pFrameInfo->stDispRect.s32Width)
		||  (pFrameInfo->stBufAddr[0].u32Stride_C < pFrameInfo->stDispRect.s32Width)
		)
    {
    	return HI_ERR_VO_INVALID_PARA;
    }
*/

    return HI_SUCCESS;
}

/* window buffer manager */
HI_S32 WinBufferReset(WIN_BUFFER_S *pstBuffer)
{
    HI_S32 i;

    pstBuffer->u32UsingFbNum = WIN_USING_FB_MAX_NUMBER;
    for(i=0; i<(HI_S32)pstBuffer->u32UsingFbNum; i++)
    {
        pstBuffer->stUsingBufNode[i].bIdle  = HI_TRUE;
        pstBuffer->stUsingBufNode[i].enType = WIN_FRAME_NORMAL;
    }

    pstBuffer->u32DispIndex = 0;
    pstBuffer->u32CfgIndex  = pstBuffer->u32DispIndex + 1;

    pstBuffer->bWaitRelease = HI_FALSE;

    DISP_MEMSET(&pstBuffer->stUselessFrame, 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S)*WIN_USELESS_FRAME_MAX_NUMBER);

    pstBuffer->u32ULSRdPtr = 0;
    pstBuffer->u32ULSWtPtr = 0;

    pstBuffer->u32ULSIn  = 0;
    pstBuffer->u32ULSOut = 0;
    pstBuffer->u32UnderLoad= 0;

    return HI_SUCCESS;
}

HI_S32 WinBufferPutULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 WP1;

    WP1 = (pstBuffer->u32ULSWtPtr + 1) % WIN_USELESS_FRAME_MAX_NUMBER;

    if (WP1 == pstBuffer->u32ULSRdPtr)
    {
        WIN_ERROR("usl full\n");
        return HI_FAILURE;
    }

    pstBuffer->stUselessFrame[pstBuffer->u32ULSWtPtr] = *pstFrame;

    pstBuffer->u32ULSWtPtr = WP1;
    pstBuffer->u32ULSIn++;

    return HI_SUCCESS;
}

HI_S32 WinBufferGetULSFrame(WIN_BUFFER_S *pstBuffer, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if (pstBuffer->u32ULSWtPtr == pstBuffer->u32ULSRdPtr)
    {
        return HI_FAILURE;
    }

    *pstFrame = pstBuffer->stUselessFrame[pstBuffer->u32ULSRdPtr];
    pstBuffer->u32ULSRdPtr = (pstBuffer->u32ULSRdPtr + 1) % WIN_USELESS_FRAME_MAX_NUMBER;
    pstBuffer->u32ULSOut++;

    return HI_SUCCESS;
}



/******************************************************************************
    apply function
******************************************************************************/
HI_S32 WIN_Create(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWin)
{
    HI_S32 nRet = HI_SUCCESS;
    WINDOW_S *pWindow = HI_NULL;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);
    WinCheckNullPointer(phWin);

//printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  001 \n");
    //pWinAttr->enDisp = HI_DRV_DISPLAY_0;
    // s1 检查参数合法性
    nRet = WinCheckAttr(pWinAttr);
    if (nRet)
    {
        WIN_ERROR("WinAttr is invalid!\n");
        return nRet;
    }

//printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  002 \n");
    // 根据是否为虚拟窗口，走不同分支
    if (pWinAttr->bVirtual != HI_TRUE)
    {
        //HI_DRV_DISPLAY_E enSlave;
        HI_DRV_WIN_ATTR_S stSlvWinAttr;
        WINDOW_S *pSlaveWindow = HI_NULL;
        
        // 非虚拟窗口
//printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  003 \n");
        // s1 检查disp是否打开
        if (DISP_IsOpened(pWinAttr->enDisp) != HI_TRUE)
        {
            WIN_ERROR("DISP is not opened!\n");
            return HI_ERR_DISP_NOT_EXIST;
        }

        if ( DISP_IsFollowed(pWinAttr->enDisp))
        {
            // s4.1 构造slave window参数
            nRet = WinGetSlaveWinAttr(pWinAttr, &stSlvWinAttr);
            if (nRet)
            {
                WIN_ERROR("WinAttr is invalid!\n");
                return nRet;
            }

            nRet = WinCheckAttr(pWinAttr);
            if (nRet)
            {
                WIN_ERROR("WinAttr is invalid!\n");
                return nRet;
            }
        }

        // s2.1 创建window
        nRet = WinCreateDisplayWindow(pWinAttr, &pWindow);
        if (nRet)
        {
            goto __ERR_RET__;
        }

        // s3 注册回调函数
        nRet = WinRegCallback(pWindow);
        if (nRet)
        {
            goto __ERR_RET_DESTROY__;
        }

        // s3 disp是否有slave
        if ( DISP_IsFollowed(pWinAttr->enDisp))
        {
            // s4.2 创建slave window
            nRet = WinCreateDisplayWindow(&stSlvWinAttr, &pSlaveWindow);
            if (nRet)
            {
                goto __ERR_RET_UNREG_CB__;
            }
            
            // s4.3 注册slave window callback
            WinRegCallback(pSlaveWindow);
            if (nRet)
            {
                goto __ERR_RET_DESTROY_SL__;
            }

            // s5 建立绑定关系
            pSlaveWindow->enType = HI_DRV_WIN_ACTIVE_SLAVE ;

            // s4 添加到win设备
            WinAddWindow(pSlaveWindow->enDisp, pSlaveWindow);

            pWindow->hSlvWin = (HI_HANDLE)(pSlaveWindow->u32Index);
            pSlaveWindow->pstMstWin = (HI_HANDLE)pWindow;
        }

        pWindow->enType = HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE;
        //pWindow->enType = HI_DRV_WIN_ACTIVE_SINGLE;

        // s4 添加到win设备
        WinAddWindow(pWindow->enDisp, pWindow);

        //printk("Win create hwin = 0x%x\n", (HI_U32)pWindow);

        // s8 返回window句柄
        *phWin = (HI_HANDLE)(pWindow->u32Index);
        return HI_SUCCESS;    

__ERR_RET_DESTROY_SL__:
        WinDestroyDisplayWindow(pSlaveWindow);

__ERR_RET_UNREG_CB__:
        WinUnRegCallback(pWindow);

__ERR_RET_DESTROY__:
        WinDestroyDisplayWindow(pWindow);

__ERR_RET__:
        return nRet;
    }
    else
    {

        // 虚拟窗口
        // s1 创建window
        nRet = WinCreateVirtualWindow(pWinAttr, &pWindow);
        if (nRet)
        {
            return nRet;
        }

        // s2 添加到win设备
        //WinAddWindow(, pWindow);

        // s3 返回window句柄
        *phWin = (HI_HANDLE)pWindow;
        return HI_SUCCESS;
    }

}

HI_S32 WIN_Destroy(HI_HANDLE hWin)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;
    //HI_U32 t;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    //HI_DRV_SYS_GetTimeStampMs((HI_U32 *)&t);
    //printk("Start Destroy = 0X%x\n", t);
    
    // s2 关闭window
    //if (pstWin->bEnable == HI_TRUE)
    {
        nRet = WIN_SetEnable(hWin, HI_FALSE);

        DISP_MSLEEP(50);
    }
    
    // 非虚拟窗口
    if (pstWin->enType != HI_DRV_WIN_VITUAL_SINGLE)
    {

        if (pstWin->hSlvWin)
        {
            WIN_Destroy(pstWin->hSlvWin);
#if 0
            // s0 stop window
            nRet = WIN_SetEnable(pstWin->hSlvWin, HI_FALSE);

            DISP_MSLEEP(50);

            // s2 unregister
            WinUnRegCallback((WINDOW_S *)(pstWin->hSlvWin));

            // s3 删除window
            WinDelWindow((WINDOW_S *)(pstWin->hSlvWin));

            // s3 销毁window
            WinDestroyDisplayWindow((WINDOW_S *)(pstWin->hSlvWin));
#endif
        }

        // s2 unregister
        WinUnRegCallback(pstWin);

        // s1 删除window
        WinDelWindow(pstWin->u32Index);

        // s3 销毁window
        WinDestroyDisplayWindow(pstWin);
    }
    else
    {

        // 虚拟窗口

        // s1 销毁window
        //WinDestroyVirtualWindow(pstWin);

        // s2 删除window
        //WinDelWindow(pstWin);
    }

    //HI_DRV_SYS_GetTimeStampMs((HI_U32 *)&t);
    //printk("end Destroy = 0X%x\n", t);

    return HI_SUCCESS;
}

HI_S32 WIN_SetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    WINDOW_S *pstWin;
    HI_DRV_WIN_ATTR_S stSlvWinAttr;
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    HI_S32 nRet = HI_SUCCESS;
    HI_S32 t;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s1 检查参数合法性
    nRet = WinCheckFixedAttr(&pstWin->stCfg.stAttr, pWinAttr);
    if (nRet)
    {
        return nRet;
    }

    nRet = WinCheckAttr(pWinAttr);
    if (nRet)
    {
        return nRet;
    }

    if (pstWin->hSlvWin)
    {
        WIN_GetAttr(pstWin->hSlvWin, &stSlvWinAttr);

        WinGetSlaveWinAttr2(pstWin, pWinAttr, &stSlvWinAttr);

        nRet = WinCheckAttr(&stSlvWinAttr);
        if (nRet)
        {
            WIN_ERROR("The new attr cannot use on SD window!\n");
            return nRet;
        }
    }

    // s2 initial parameters
    nRet = DISP_GetDisplayInfo(pstWin->enDisp, &stDispInfo);
    if (nRet)
    {
        WIN_ERROR("DISP_GetDisplayInfo failed in %s!\n", __FUNCTION__);
        return HI_ERR_VO_CREATE_ERR;
    }

    // s3 设置属性变化标志位
   
    // down
    atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
    
    pstWin->stCfg.stAttrBuf = *pWinAttr;

    // up
    atomic_set(&pstWin->stCfg.bNewAttrFlag, 1);

    // s4 阻塞，等待生效
    t = 0;
    while( atomic_read(&pstWin->stCfg.bNewAttrFlag) )
    {
        DISP_MSLEEP(5);
        t++;

        if (t > 10)
        {
            break;
        }
    }
    
    if (pstWin->hSlvWin)
    {
        nRet = WIN_SetAttr(pstWin->hSlvWin, &stSlvWinAttr);
    }

    if (atomic_read(&pstWin->stCfg.bNewAttrFlag) )
    {
        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);
        WIN_ERROR("WIN Set Attr timeout in %s\n", __FUNCTION__);
        return HI_ERR_VO_TIMEOUT;
    }

    return HI_SUCCESS;
}



HI_S32 WIN_GetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    WINDOW_S *pstWin;
//    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pWinAttr);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s3 返回参数
    *pWinAttr = pstWin->stCfg.stAttr;

    return HI_SUCCESS;
}


//get info for source
HI_S32 WIN_GetInfo(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo)
{
    WINDOW_S *pstWin;
//    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstInfo);

    //printk("Win get info hwin = 0x%x\n", (HI_U32)hWin);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 配置参数
    pstInfo->eType = WinGetType(pstWin);
    pstInfo->hPrim = (HI_HANDLE)(pstWin->u32Index);
    pstInfo->hSec  = (HI_HANDLE)(pstWin->hSlvWin);

    //pstInfo->eType = HI_DRV_WIN_ACTIVE_SINGLE;

    return HI_SUCCESS;
}

HI_S32 WIN_SetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_DISP_DISPLAY_INFO_S stDispInfo;
    WB_SOURCE_INFO_S stSrc2Buf;
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstSrc);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 检查参数合法性
    nRet = DISP_GetDisplayInfo(WinGetDispID(pstWin), &stDispInfo);
    if (nRet)
    {
        return nRet;
    }

    pstWin->stCfg.stSource = *pstSrc;

    stSrc2Buf.hSrc = pstSrc->hSrc;
    stSrc2Buf.pfAcqFrame = pstSrc->pfAcqFrame;
    stSrc2Buf.pfRlsFrame = pstSrc->pfRlsFrame;
    nRet =  WinBuf_SetSource(&pstWin->stBuffer.stWinBP, &stSrc2Buf);
    if (nRet)
    {
        return nRet;
    }
    // send attr to source
    WinSendAttrToSource(pstWin, &stDispInfo);

    DISP_PRINT("WIN_SetSource :s=0x%x, info=0x%x, g=0x%x,==0x%x\n",
            (HI_U32)pstSrc->hSrc, (HI_U32)pstSrc->pfSendWinInfo,
            (HI_U32)pstSrc->pfAcqFrame, (HI_U32)pstSrc->pfRlsFrame);

    return HI_SUCCESS;
}

HI_S32 WIN_GetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pstSrc);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 检查参数合法性
    nRet = WinCheckSourceInfo(pstSrc);
    if (nRet)
    {
        return nRet;
    }

    // s3 change source information
    *pstSrc = pstWin->stCfg.stSource;

    return HI_SUCCESS;
}

HI_S32 WIN_SetEnable(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);
    //WinCheckSlaveWindow(pstWin);

    // s2 set enable
    pstWin->bEnable = bEnable;

    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
    {
        if (pstWin->hSlvWin)
        {
            WIN_SetEnable(pstWin->hSlvWin, bEnable);
        }
    }

    return HI_SUCCESS;
}

HI_S32 WIN_GetEnable(HI_HANDLE hWin, HI_BOOL *pbEnable)
{
    WINDOW_S *pstWin;
//    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pbEnable);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 get state
    *pbEnable = pstWin->bEnable;

    return HI_SUCCESS;
}


#define WIN_PROCESS_TIME_CIRCLE_THRESHOLD 0xF0000ul
#define WIN_PROCESS_CALC_TIME_THRESHOLD 3
#define WIN_DELAY_TIME_MAX_CIRCLE 100
HI_S32 WIN_CalcDelayTime(WINDOW_S *pstWin, HI_U32 *pu32BufNum, HI_U32 *pu32DelayMs)
{
    HI_U32 u32Num, T, Dt, Ct, Delta, Delay;
    HI_U32 L = 0;

__WIN_CALC_DELAY__:
    L++;

    T = pstWin->stDelayInfo.T;

    WinBuf_GetFullBufNum(&pstWin->stBuffer.stWinBP, &u32Num);
    Dt = pstWin->stDelayInfo.u32DisplayTime;

    HI_DRV_SYS_GetTimeStampMs(&Ct);
    
    if (Ct <= pstWin->stDelayInfo.u32CfgTime)
    {
        Delta = pstWin->stDelayInfo.u32CfgTime - Ct;
        if ( (Delta < WIN_PROCESS_TIME_CIRCLE_THRESHOLD) && (L < WIN_PROCESS_CALC_TIME_THRESHOLD))
        {
            //printk("[GP1]");
            // interrupt happen, 'pstWin->stDelayInfo.u32CfgTime > Ct' but does not pass zero
            goto __WIN_CALC_DELAY__;
        }
        else
        {
            // circle happen, Ct pass zero
            Delay = (u32Num + 1) * T;
            Delay = Delay + T - Dt;
            Delay = Delay - (0xFFFFFFFFul - pstWin->stDelayInfo.u32CfgTime + Ct + 1);
            //printk("[GP2]");
        }
    }
    else
    {
        Delta = Ct - pstWin->stDelayInfo.u32CfgTime;
        if ( (Delta > WIN_PROCESS_TIME_CIRCLE_THRESHOLD) && (L < WIN_PROCESS_CALC_TIME_THRESHOLD))
        {
            //printk("[GP3]");
            // circle happen, pstWin->stDelayInfo.u32CfgTime pass zero
            goto __WIN_CALC_DELAY__;
        }
        else
        {
            // calc
            //printk("[GP4]");
            Delay = (u32Num + 1) * T;
            Delay = Delay + T - Dt;
            Delay = Delay - (Ct - pstWin->stDelayInfo.u32CfgTime);
        }
    }

    if (Delay > (T * WIN_DELAY_TIME_MAX_CIRCLE))
    {
        //printk("[GP5]");
        //todo
        Delay = 0;
        DISP_ASSERT(!Delay);
    }

    *pu32BufNum = u32Num;
    *pu32DelayMs= Delay;
   
    return HI_SUCCESS;
}

HI_BOOL WinGetTBMatchInfo(HI_HANDLE hWin)
{
    WINDOW_S *pstWin;

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    return pstWin->stDelayInfo.bTBMatch;
}

HI_S32 WIN_GetPlayInfo(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pstInfo);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    //stPlayInfo = pstWin->stPlay;

    //BP_GetFullBufNum(&pstWin->stBuffer.stBP, &u32Num);
    //WinUpdatePlayInfo2(&stPlayInfo, HI_TRUE, u32Num+1);

    if (!pstWin->bEnable || !pstWin->stDelayInfo.u32DispRate)
    {
        WIN_ERROR("window is not ready!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    if (!pstWin->stDelayInfo.u32DispRate)
    {
        pstInfo->u32DispRate = 5000;
        pstInfo->u32DelayTime = 20;
        pstInfo->bTBMatch = HI_TRUE;
        return HI_SUCCESS;
    }

    WIN_CalcDelayTime(pstWin, &(pstInfo->u32FrameNumInBufQn), &(pstInfo->u32DelayTime));

    pstInfo->u32DispRate = pstWin->stDelayInfo.u32DispRate;
    //printk("Get Match=%d\n", pstInfo->bTBMatch);

#if 0
    if (pstWin->stDelayInfo.bInterlace == HI_FALSE)
    {
        // if master window work at progress output mode, get slave window match info
        if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
        {
            pstInfo->bTBMatch = WinGetTBMatchInfo(pstWin->hSlvWin);

            //printk("...........m=%d    ", pstInfo->bTBMatch);
        }
    }
    else
    {
        // if master window work at interlace output mode, return match info
        pstInfo->bTBMatch = pstWin->stDelayInfo.bTBMatch;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 WIN_QueueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    WINDOW_S *pstWin;
    HI_DRV_WIN_SRC_INFO_S *pstSource;
    //HI_U32 u32BufId;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pFrameInfo);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 get state
    nRet = WinCheckFrame(pFrameInfo);
    if (nRet)
    {
        WIN_ERROR("win frame parameters invalid\n");
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    pstSource = &pstWin->stCfg.stSource;

   // if (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
   //     printk("q fid=%d\n", pFrameInfo->u32FrameIndex);

    nRet = WinBuf_PutNewFrame(&pstWin->stBuffer.stWinBP, pFrameInfo);
    if (nRet)
    {
        return HI_ERR_VO_BUFQUE_FULL;
    }

    //if (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
    //    printk("q ok\n");

    return HI_SUCCESS;
}

HI_S32 WIN_QueueUselessFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pFrameInfo);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 get state
    nRet = WinCheckFrame(pFrameInfo);
    if (nRet)
    {
        WIN_ERROR("win frame parameters invalid\n");
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    nRet = WinBufferPutULSFrame(&pstWin->stBuffer, pFrameInfo);
    if (nRet)
    {
        WIN_WARN("quls failed\n");
        return HI_ERR_VO_BUFQUE_FULL;
    }

    //printk("quls fid=%d\n", pFrameInfo->u32FrmCnt);

    return HI_SUCCESS;
}

HI_S32 WIN_DequeueFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameInfo)
{
    WINDOW_S *pstWin;
    //HI_U32 u32BufId;    
    //HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pFrameInfo);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

#if 0
    // s2 get state
    // get frame and release it
    nRet = BP_GetDoneBuf(&pstWin->stBuffer.stBP, &u32BufId);
    if (nRet)
    {
        WIN_INFO("Window has no frame to release!\n");
        return HI_ERR_VO_NO_FRAME_TO_RELEASE;
    }

    nRet = BP_GetFrame(&pstWin->stBuffer.stBP, u32BufId, pFrameInfo);
    if (nRet)
    {
        WIN_FATAL("BP_SetFrame failed!\n");
        return HI_FAILURE;
    }

    nRet = BP_SetBufEmpty(&pstWin->stBuffer.stBP, u32BufId);
    if (nRet)
    {
        WIN_FATAL("BP_DelEmptyBuf failed!\n");
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}


HI_S32 WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E enZFlag)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();

    if (enZFlag >= HI_DRV_DISP_ZORDER_BUTT)
    {
        WIN_FATAL("HI_DRV_DISP_ZORDER_E invalid!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    switch(enZFlag)
    {
        case HI_DRV_DISP_ZORDER_MOVETOP:
            nRet = pstWin->stVLayerFunc.PF_MovTop(pstWin->u32VideoLayer);
            break;
        case HI_DRV_DISP_ZORDER_MOVEUP:
            nRet = pstWin->stVLayerFunc.PF_MovUp(pstWin->u32VideoLayer);
            break;
        case HI_DRV_DISP_ZORDER_MOVEBOTTOM:
            nRet = pstWin->stVLayerFunc.PF_MovBottom(pstWin->u32VideoLayer);
            break;
        case HI_DRV_DISP_ZORDER_MOVEDOWN:
            nRet = pstWin->stVLayerFunc.PF_MovDown(pstWin->u32VideoLayer);
            break;
        default :
            nRet = HI_ERR_VO_INVALID_OPT;
            break;
    }


    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
    {
        if (pstWin->hSlvWin)
        {
            WIN_SetZorder(pstWin->hSlvWin, enZFlag);
        }
    }  

    return nRet;
}

HI_S32 WIN_GetZorder(HI_HANDLE hWin, HI_U32 *pu32Zorder)
{
    WINDOW_S *pstWin;
    HI_S32 nRet = HI_SUCCESS;

    WinCheckDeviceOpen();
    WinCheckNullPointer(pu32Zorder);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

   nRet = pstWin->stVLayerFunc.PF_GetZorder(pstWin->u32VideoLayer, pu32Zorder);

   return nRet;
}


HI_S32 WIN_Freeze(HI_HANDLE hWin, HI_BOOL bEnable, HI_DRV_WIN_SWITCH_E enFrz)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);
    //WinCheckSlaveWindow(pstWin);

    if (enFrz >= HI_DRV_WIN_SWITCH_BUTT)
    {
        WIN_ERROR("Freeze mode is invalid!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    // s2 set enable
    if (pstWin->bUpState && pstWin->bEnable)
    {
        WIN_ERROR("Window is changing, can't set pause now!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    if (!pstWin->bEnable || pstWin->bReset)
    {
        WIN_ERROR("Window is DISABLE, can't set pause now!\n");
        return HI_ERR_VO_INVALID_OPT;
    }


    pstWin->bUpState = HI_FALSE;

    pstWin->enStateNew = bEnable ? WIN_STATE_FREEZE : WIN_STATE_UNFREEZE;
    if (bEnable)
    {
        pstWin->stFrz.enFreezeMode = enFrz;
    }
    pstWin->bUpState = HI_TRUE;

    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
    {
        if (pstWin->hSlvWin)
        {
            WIN_Freeze(pstWin->hSlvWin, bEnable, enFrz);
        }
    }  

    return HI_SUCCESS;
}

//HI_VOID ISR_WinReleaseFullFrame(WINDOW_S *pstWin);
HI_S32 WIN_Reset(HI_HANDLE hWin, HI_DRV_WIN_SWITCH_E enRst)
{
    WINDOW_S *pstWin;
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    HI_U32 u = 0;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);
    //WinCheckSlaveWindow(pstWin);

    if (enRst >= HI_DRV_WIN_SWITCH_BUTT)
    {
        WIN_ERROR("Reset mode is invalid!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    // s2 set enable
    if (pstWin->bReset || pstWin->bUpState)
    {
        //WIN_ERROR("Last reset is not finished!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    if (pstWin->bEnable)
    {
        //printk("01 enter win reset........\n");
        pstWin->stRst.enResetMode = enRst;
        pstWin->bReset = HI_TRUE;

        while(pstWin->bReset && (u<10))
        {
            DISP_MSLEEP(20);
            u++;
        }

        if (u >= 5)
        {
            DISP_WARN("############ RESET TIMEOUT#########\n");
        }
        //printk("01 exit win reset\n");
    }
    else
    {
        //printk("02 enter win reset........\n");
        //ISR_WinReleaseDisplayedFrame(pstWin);
        WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
        
        ISR_WinReleaseUSLFrame(pstWin);

        // flush frame in full buffer pool
        pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
        WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);

        pstWin->bReset = HI_FALSE;

        //printk("02 exit win reset\n");
    }

    return HI_SUCCESS;
}


HI_S32 WIN_Pause(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin;
    HI_U32 u;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);
    //WinCheckSlaveWindow(pstWin);

    // s2 set enable
    if (pstWin->bUpState && pstWin->bEnable)
    {
        WIN_ERROR("Window is changing, can't set pause now!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    pstWin->bUpState = HI_FALSE;

    pstWin->enStateNew = bEnable ? WIN_STATE_PAUSE : WIN_STATE_RESUME;

    pstWin->bUpState = HI_TRUE;

    u = 0;
    while(pstWin->bUpState && (u<10))
    {
        DISP_MSLEEP(5);
        u++;
    }

    if (u >= 10)
    {
        DISP_WARN("############ PAUSE TIMEOUT#########\n");
    }

    return HI_SUCCESS;
}

HI_S32 WIN_SetStepMode(HI_HANDLE hWin, HI_BOOL bStepMode)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    // s2 set enable
    if (pstWin->bUpState && pstWin->bEnable)
    {
        WIN_ERROR("Window is changing, can't set pause now!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    // set stepmode flag
    pstWin->bStepMode = bStepMode;

    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
    {
        if (pstWin->hSlvWin)
        {
            WIN_SetStepMode(pstWin->hSlvWin, bStepMode);
        }
    }  
    return HI_SUCCESS;
}

HI_S32 WIN_SetStepPlay(HI_HANDLE hWin)
{


    return HI_SUCCESS;
}

HI_S32 WIN_SetQuick(HI_HANDLE hWin, HI_BOOL bEnable)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);
    //WinCheckSlaveWindow(pstWin);

    // s2 set enable
    if (pstWin->bUpState && pstWin->bEnable)
    {
        WIN_ERROR("Window is changing, can't set pause now!\n");
        return HI_ERR_VO_INVALID_OPT;
    }

    // initial quickmode
    pstWin->bQuickMode = bEnable;

    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinGetType(pstWin))
    {
        if (pstWin->hSlvWin)
        {
            WIN_SetQuick(pstWin->hSlvWin, bEnable);
        }
    }  

    return HI_SUCCESS;
}



/* only for virtual window */
HI_S32 WIN_SetExtBuffer(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf)
{


    return HI_SUCCESS;
}

HI_S32 WIN_AcquireFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{


    return HI_SUCCESS;
}

HI_S32 WIN_ReleaseFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{


    return HI_SUCCESS;
}

//todo
HI_S32 WIN_Set3DMode(HI_HANDLE hWin, HI_BOOL b3DEnable,HI_DRV_DISP_STEREO_E eMode)
{


    return HI_SUCCESS;
}

HI_S32 WIN_SendFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{


    return HI_SUCCESS;
}

HI_S32 Win_DebugGetHandle(HI_DRV_DISPLAY_E enDisp, WIN_HANDLE_ARRAY_S *pstWin)
{
    HI_S32 i;

    WinCheckDeviceOpen();

    // s1 检查句柄合法性
    WinCheckNullPointer(pstWin);

    DISP_MEMSET(pstWin, 0, sizeof(WIN_HANDLE_ARRAY_S));

    pstWin->u32WinNumber = 0;

    for(i=0; i<WINDOW_MAX_NUMBER; i++)
    {
        if (stDispWindow.pstWinArray[(HI_U32)enDisp][i])
        {
            pstWin->ahWinHandle[pstWin->u32WinNumber] = (HI_HANDLE)(stDispWindow.pstWinArray[(HI_U32)enDisp][i]->u32Index);
            pstWin->u32WinNumber++;
        }
    }

    return HI_SUCCESS;
}


/*
HI_S32 WinUpdatePlayInfo(HI_DRV_WIN_PLAY_INFO_S *ptPlay, HI_U32 u32Rate)
{
    ptPlay->u32DispRate = u32Rate;
    return HI_SUCCESS;
}
*/



HI_S32 WinAcquireFrame(WINDOW_S *pstWin)
{
#if 0
	HI_DRV_VIDEO_FRAME_S stNewFrame;
	HI_U32 u32BufId;
    HI_S32 nRet = HI_SUCCESS;

    nRet = BQ_GetWriteNode(&pstWin->stBuffer.stBP, &u32BufId);
    if (nRet)
    {
        return HI_ERR_VO_BUFQUE_FULL;
    }

	if (pstWin->stSource.pfAcqFrame)
	{
		nRet = pstWin->stSource.pfAcqFrame(pstWin->stSource.hSrc, &stNewFrame);
        if (nRet)
        {
            WIN_ERROR("WIN Release Frame failid\n");
            return HI_ERR_VO_FRAME_RELEASE_FAILED;
        }
	}

    // s2 get state
    nRet = WinCheckFrame(&stNewFrame);
    if (nRet)
    {
        return HI_ERR_VO_FRAME_INFO_ERROR;
    }

    if (!stNewFrame.u32PlayTime)
    {
    	if (pstWin->stSource.pfRlsFrame && stNewFrame.bToRelease)
    	{
	        nRet = pstWin->stSource.pfRlsFrame(pstWin->stSource.hSrc, &stNewFrame);
	        if (nRet)
	        {
	            WIN_ERROR("WIN Release Frame failid\n");
	        }

            BQ_ReleaseRecoder(&pstWin->stBufQue, BQ_RELEASE_DISCARD);
    	}
		else
		{
			// todo
		}
    }

    nRet = BQ_PutWriteNode(&pstWin->stBufQue, &stNewFrame, BUF_FRAME_SRC_NORMAL);
#endif

    return HI_SUCCESS;
}


//#define WIN_DEBUG_PRINT_RELEASE 1
HI_S32 s_ResetPrint = 0;

HI_VOID ISR_WinReleaseUSLFrame(WINDOW_S *pstWin)
{
    HI_DRV_WIN_SRC_INFO_S *pstSource = &pstWin->stCfg.stSource;
    HI_DRV_VIDEO_FRAME_S stRlsFrm;
    HI_S32 nRet;

     /* release useless frame */

    nRet = WinBufferGetULSFrame(&pstWin->stBuffer, &stRlsFrm);
    while(!nRet)
    {
        if (pstSource->pfRlsFrame)
        {
            pstSource->pfRlsFrame(pstSource->hSrc, &stRlsFrm);
#ifdef WIN_DEBUG_PRINT_RELEASE
            if (s_ResetPrint)
            {
                printk("Rel 006 fid=%d, addr=0x%x\n", 
                        stRlsFrm.u32FrameIndex,
                        stRlsFrm.stBufAddr[0].u32PhyAddr_Y);
            }
#endif
        }

        nRet = WinBufferGetULSFrame(&pstWin->stBuffer, &stRlsFrm);
    }
    
    return;
}

HI_DRV_VIDEO_FRAME_S *ISR_SlaveWinGetConfigFrame(WINDOW_S *pstWin)
{
    HI_U32 RefID;
    WINDOW_S *pstMstWin = (WINDOW_S *)(pstWin->pstMstWin);
    HI_DRV_VIDEO_FRAME_S *pstRefF, *pstDisp, *pstNew;

    pstRefF = WinBuf_GetConfigedFrame(&pstMstWin->stBuffer.stWinBP);
    if (pstRefF)
    {
        RefID = pstRefF->u32FrameIndex;
    }
    else
    {
        RefID = 0;
    }

    pstDisp = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);

    pstNew = WinBuf_GetFrameByMaxID(&pstWin->stBuffer.stWinBP, pstDisp, RefID);

    return pstNew;
}

HI_VOID WinTestFrameMatch(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrame, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    //HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrame->u32Priv[0]);

    pstWin->stDelayInfo.bTBMatch = HI_TRUE;

    if (pstInfo->stDispInfo.bInterlace == HI_TRUE)
    {
         if(  (  (pstPriv->eOriginField == HI_DRV_FIELD_TOP) 
                &&(pstInfo->stDispInfo.bIsBottomField == HI_FALSE) )
            ||(   (pstPriv->eOriginField == HI_DRV_FIELD_BOTTOM) 
                 &&(pstInfo->stDispInfo.bIsBottomField == HI_TRUE) )
            )
        {
            pstWin->stDelayInfo.bTBMatch = HI_FALSE;
            pstWin->u32TBNotMatchCount++;
        }
    }
#if 0
    if (pstWin->enDisp == HI_DRV_DISPLAY_0)
    {
        printk("slv f=%d, tb=%d, M=%d\n", 
                                     pstPriv->eOriginField, 
                                     pstInfo->stDispInfo.bIsBottomField, 
                                     pstWin->stDelayInfo.bTBMatch);
    }
    if (pstWin->enDisp == HI_DRV_DISPLAY_1)
    {
        printk("mst f=%d, tb=%d, M=%d\n", 
                                     pstPriv->eOriginField, 
                                     pstInfo->stDispInfo.bIsBottomField, 
                                     pstWin->stDelayInfo.bTBMatch);
    }
#endif
    //if (pstWin->u32TBTestCount < (pstInfo->stDispInfo.u32RefreshRate / 100))
    if (pstWin->u32TBTestCount < (pstWin->stBuffer.stWinBP.u32BufNumber))
    {
        // inform user match-info per second.
        //pstWin->stDelayInfo.bTBMatch = HI_TRUE;
        pstWin->u32TBTestCount++;
    }
    else
    {
        // inform TB match info this time and reset test count
        pstWin->u32TBTestCount = 0;
    }

    //pstWin->stDelayInfo.bTBMatch = HI_FALSE;

    return;
}

HI_VOID ISR_WinConfigFrame(WINDOW_S *pstWin, HI_DRV_VIDEO_FRAME_S *pstFrame, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_WIN_ATTR_S *pstAttr = &pstWin->stUsingAttr;
    WIN_HAL_PARA_S stLayerPara;
    HI_DRV_DISP_COLOR_SETTING_S stColor;
    HI_S32 nRet;

#if 0
    struct timeval tv;
    HI_U32 Ct;

    do_gettimeofday(&tv);
    Ct = (HI_U32)(tv.tv_sec * 1000 + (tv.tv_usec/1000));


    if (pstWin->enType == HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE)
    {
        printk("master Fid=0x%x, Ct=0x%x\n", pstFrame->u32FrameIndex, Ct);
    }
    else
    {
        printk("slave  Fid=0x%x, Ct=0x%x\n", pstFrame->u32FrameIndex, Ct);
    }
#endif

    pstWin->stCfg.eDispMode = pstInfo->stDispInfo.eDispMode;
    pstWin->stCfg.bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;
    
//printk("     cfg Id=%d\n", pstFrame->u32FrameIndex);
    pstAttr->stInRect  = pstFrame->stDispRect;
    //printk("fw=%d, fh=%d\n", pstFrame->stDispRect.s32Width, pstFrame->stDispRect.s32Height);
    //pstWin->stUsingAttr.stInRect.s32Width = 720;
    //pstWin->stUsingAttr.stInRect.s32Height = 576;

    stLayerPara.en3Dmode = pstInfo->stDispInfo.eDispMode;
    stLayerPara.bRightEyeFirst = pstInfo->stDispInfo.bRightEyeFirst;
    stLayerPara.pstFrame = pstFrame;
    stLayerPara.bZmeUpdate = HI_TRUE;
    stLayerPara.stIn    = pstAttr->stInRect;
    stLayerPara.stDisp  = pstAttr->stOutRect;
    stLayerPara.stVideo = pstAttr->stOutRect;
    stLayerPara.pstDispInfo = (HI_DISP_DISPLAY_INFO_S *)&(pstInfo->stDispInfo);
    stLayerPara.eField = pstInfo->eField;
//printk("$=%d.", pstWin->eLayer);
    nRet = pstWin->stVLayerFunc.PF_SetFramePara(pstWin->u32VideoLayer, &stLayerPara);
    if (nRet)
    {
        pstWin->stVLayerFunc.PF_SetEnable(pstWin->u32VideoLayer, HI_FALSE);
    }
    else
    {
        pstWin->stVLayerFunc.PF_SetEnable(pstWin->u32VideoLayer, HI_TRUE);
    }

    pstWin->stVLayerFunc.PF_Update(pstWin->u32VideoLayer);

    if (pstWin->bDispInfoChange)
    {
        //stColor.enInCS  = ((HI_DRV_VIDEO_PRIVATE_S *)&pstFrame->u32Priv[0])->eColorSpace;
        stColor.enInCS  = HI_DRV_CS_BT601_YUV_LIMITED;
        stColor.enOutCS = pstInfo->stDispInfo.eColorSpace;
        stColor.u32Bright = pstInfo->stDispInfo.u32Bright;
        stColor.u32Hue    = pstInfo->stDispInfo.u32Hue;
        stColor.u32Satur  = pstInfo->stDispInfo.u32Satur;
        stColor.u32Contrst = pstInfo->stDispInfo.u32Contrst;
        stColor.u32Kr = pstInfo->stDispInfo.u32Kr;
        stColor.u32Kg = pstInfo->stDispInfo.u32Kg;
        stColor.u32Kb = pstInfo->stDispInfo.u32Kb;
        stColor.bGammaEnable       = HI_FALSE;
        stColor.bUseCustGammaTable = HI_FALSE;

        pstWin->stVLayerFunc.PF_SetColor(pstWin->u32VideoLayer, &stColor);
        pstWin->bDispInfoChange = HI_FALSE;
    }

    //if (pstWin->enDisp == HI_DRV_DISPLAY_0)
    {
        WinTestFrameMatch(pstWin, pstFrame, pstInfo);
    }

    return;
}

HI_VOID ISR_WinUpdatePlayInfo(WINDOW_S *pstWin, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    // calc delay time in buffer queue
    //HI_U32 u32Num;
    HI_U32 T;
    //struct timeval tv;

    //BP_GetFullBufNum(&pstWin->stBuffer.stBP, &u32Num);

    DISP_ASSERT(pstInfo->stDispInfo.u32RefreshRate);
    DISP_ASSERT(pstInfo->stDispInfo.stRefRect.s32Height);

    pstWin->stDelayInfo.u32DispRate = pstInfo->stDispInfo.u32RefreshRate;
    pstWin->stDelayInfo.T = (1*1000*100)/pstInfo->stDispInfo.u32RefreshRate;
    //pstWin->stDelayInfo.u32FrameNumber = u32Num;

    //pstWin->stDelayInfo.bTBMatch = HI_TRUE;
    pstWin->stDelayInfo.bInterlace = pstInfo->stDispInfo.bInterlace;

    T = pstWin->stDelayInfo.T;

    if (pstInfo->stDispInfo.bInterlace)
    {
        pstWin->stDelayInfo.u32DisplayTime = (pstInfo->stDispInfo.u32Vline *2*T)/pstInfo->stDispInfo.stOrgRect.s32Height;
    }
    else
    {
        pstWin->stDelayInfo.u32DisplayTime = (pstInfo->stDispInfo.u32Vline *T)/pstInfo->stDispInfo.stOrgRect.s32Height;
    }

    //do_gettimeofday(&tv);
    //pstWin->stDelayInfo.u32CfgTime = (HI_U32)(tv.tv_sec *1000 + (tv.tv_usec/1000));
    HI_DRV_SYS_GetTimeStampMs((HI_U32 *)&pstWin->stDelayInfo.u32CfgTime);

    return;
}


HI_VOID ISR_WinStateTransfer(WINDOW_S *pstWin)
{
    //printk("Enter ISR_WinStateTransfer......\n");

    if (pstWin->enState == WIN_STATE_WORK)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_PAUSE:
            case WIN_STATE_FREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_WORK:
            case WIN_STATE_RESUME:
            case WIN_STATE_UNFREEZE:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }
    else if(pstWin->enState == WIN_STATE_PAUSE)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_RESUME:
            case WIN_STATE_FREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_PAUSE:
            case WIN_STATE_WORK:
            case WIN_STATE_UNFREEZE:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }
    else if(pstWin->enState == WIN_STATE_FREEZE)
    {
        switch(pstWin->enStateNew)
        {
            case WIN_STATE_UNFREEZE:
            {
                pstWin->enState = pstWin->enStateNew;
                pstWin->bUpState = HI_FALSE;
                return;
            }
            case WIN_STATE_PAUSE:
            case WIN_STATE_FREEZE:
            case WIN_STATE_WORK:
            case WIN_STATE_RESUME:
            default :
                pstWin->bUpState = HI_FALSE;
                return;
        }
    }
   
    return;
}

HI_VOID ISR_WinResetState(WINDOW_S *pstWin)
{
    pstWin->enState = WIN_STATE_WORK;
    pstWin->bReset  = HI_FALSE;

    return;
}

HI_DRV_VIDEO_FRAME_S * WinGetFrameToConfig(WINDOW_S *pstWin)
{
    HI_DRV_VIDEO_FRAME_S *pstFrame;

#if 1
    if (pstWin->enType == HI_DRV_WIN_ACTIVE_SLAVE)
    {
        pstFrame = ISR_SlaveWinGetConfigFrame(pstWin);
    }
    else
#endif
    {
        if (pstWin->bQuickMode)
        {
            HI_DRV_VIDEO_FRAME_S *pstDispFrame;
            pstDispFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
            pstFrame = WinBuf_GetNewestFrame(&pstWin->stBuffer.stWinBP, pstDispFrame);
        }
        else
        {
            //pstFrame = ISR_WinGetConfigFrame(&(pstWin->stBuffer));
            pstFrame = WinBuf_GetConfigFrame(&pstWin->stBuffer.stWinBP);
        }
    }
    
    if (!pstFrame)
    {
        pstWin->stBuffer.u32UnderLoad++;

        if (WinTestBlackFrameFlag(pstWin) != HI_TRUE)
        {
            WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
            pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);
        }
    }
    else
    {
        WinClearBlackFrameFlag(pstWin);
    }

    if(!pstFrame)
    {
        pstFrame = BP_GetBlackFrameInfo();
        WinSetBlackFrameFlag(pstWin);
    }

    return pstFrame;
}


HI_VOID ISR_CallbackForWinProcess(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;
//    HI_U32 u32BufId;
    //HI_S32 nRet;
    HI_BOOL bUpDispInof = HI_FALSE;

    if (!hDst || !pstInfo )
    {
        WIN_ERROR("WIN Input null pointer in %s!\n", __FUNCTION__);
        return;
    }

    //printk("t=%d.", pstInfo->eEventType);
    //return;

    //printk("[%d]", pstInfo->stDispInfo.bIsBottomField);

    pstWin = (WINDOW_S *)hDst;


    if (pstInfo->eEventType != HI_DRV_DISP_C_VT_INT)
    {
        DISP_PRINT("@@@@@@@ DISP HI_DRV_DISP_C_event= %d, disp=%d\n", pstInfo->eEventType, pstWin->enDisp);
    }

    if (  (WIN_DEVICE_STATE_SUSPEND == s_s32WindowGlobalFlag)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_CLOSE)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_PREPARE_TO_PEND)
        )
    {
        DISP_PRINT(">>>>>>>>> mask\n");
        pstWin->bMasked = HI_TRUE;
    }
    else
    {
        pstWin->bMasked = HI_FALSE;
        //DISP_PRINT(">>>>>>>>>  mask 002\n");
    }

    if (  (pstInfo->eEventType == HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_ADJUCT_SCREEN_AREA)
        )
    {
        pstWin->bDispInfoChange = HI_TRUE;
        bUpDispInof = HI_TRUE;
        //DISP_PRINT(">>>>>>>>> DISP HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE event= %d\n", pstInfo->eEventType);;
    }


    if (  (pstInfo->eEventType == HI_DRV_DISP_C_OPEN)
        ||(pstInfo->eEventType == HI_DRV_DISP_C_RESUME)
       )
    {
        //DISP_PRINT(">>>>>>>>> DISP HI_DRV_DISP_C_OPEN event= %d\n", pstInfo->eEventType);

        bUpDispInof = HI_TRUE;
        pstWin->bDispInfoChange = HI_TRUE;
    }

    if (atomic_read(&pstWin->stCfg.bNewAttrFlag))
    {
        pstWin->stCfg.stAttr = pstWin->stCfg.stAttrBuf;

        pstWin->stCfg.stRefOutRect = pstWin->stCfg.stAttr.stOutRect;
        pstWin->stCfg.stRefScreen  = pstInfo->stDispInfo.stRefRect;

        atomic_set(&pstWin->stCfg.bNewAttrFlag, 0);

        //printk(" process new window attr!\n");

        bUpDispInof = HI_TRUE;
        pstWin->bDispInfoChange = HI_TRUE;
    }

    if (bUpDispInof)
    {
        pstWin->stUsingAttr = pstWin->stCfg.stAttr;

        WinUpdateRectBaseRect(&pstWin->stCfg.stRefScreen, 
                                (HI_RECT_S *)&(pstInfo->stDispInfo.stRefRect),
                                &pstWin->stCfg.stRefOutRect,
                                &pstWin->stUsingAttr.stOutRect);

        // TODO: 超出屏幕的处理

        if (!pstWin->stUsingAttr.stOutRect.s32Width || !pstWin->stUsingAttr.stOutRect.s32Height)
        {
            pstWin->stUsingAttr.stOutRect = pstInfo->stDispInfo.stRefRect;
        }

        WinSendAttrToSource(pstWin, (HI_DISP_DISPLAY_INFO_S *)&pstInfo->stDispInfo);

        // create window play info
        //WinUpdatePlayInfo(&pstWin->stPlay, pstInfo->stDispInfo.u32RefreshRate);

        DISP_PRINT("Display info>> M=%d,S=%d,att=%d, 3d=%d, R=%d, I=%d, w=%d, h=%d, %dvs%d, rate=%d, cs=%d\n", 
                pstInfo->stDispInfo.bIsMaster,
                pstInfo->stDispInfo.bIsSlave,
                pstInfo->stDispInfo.enAttachedDisp,
                pstInfo->stDispInfo.eDispMode,
                pstInfo->stDispInfo.bRightEyeFirst,
                pstInfo->stDispInfo.bInterlace, 
                pstInfo->stDispInfo.stOrgRect.s32Width,
                pstInfo->stDispInfo.stOrgRect.s32Height,
                pstInfo->stDispInfo.stAR.u8ARw,
                pstInfo->stDispInfo.stAR.u8ARh,
                pstInfo->stDispInfo.u32RefreshRate,
                pstInfo->stDispInfo.eColorSpace);

        //pstWin->bToUpDispInof = HI_FALSE;
        //pstWin->stVLayerFunc.PF_SetDispMode(pstWin->u32VideoLayer, pstInfo->stDispInfo.eDispMode);
    }

    // window attr info changed
/*
    if (pstWin->bToUpdateAttr)
    {
        pstWin->stUsingAttr = pstWin->stNewAttr;
        pstWin->bToUpdateAttr = HI_FALSE;
    }
*/
    if (!pstWin->bEnable || pstWin->bMasked)
    {
        pstWin->stVLayerFunc.PF_SetEnable(pstWin->u32VideoLayer, HI_FALSE);
        pstWin->stVLayerFunc.PF_Update(pstWin->u32VideoLayer);
//printk("win001,");
        return;
    }

//printk(".");

    // window process
    if (pstInfo->eEventType == HI_DRV_DISP_C_VT_INT)
    {
        HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;


        //printk("win003,");
        if (pstWin->bReset)
        {
        //printk("win004,");
            // release displayed and configed frame
            s_ResetPrint = 1;

            //ISR_WinReleaseDisplayedFrame(pstWin);
            WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
            
            ISR_WinReleaseUSLFrame(pstWin);

            // flush frame in full buffer pool
            //ISR_WinReleaseFullFrame(pstWin);
            pstFrame = WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP);
            WinBuf_FlushWaitingFrame(&pstWin->stBuffer.stWinBP, pstFrame);

            if (pstWin->stRst.enResetMode == HI_DRV_WIN_SWITCH_BLACK)
            {
                //ISR_WinReleaseDisplayedFrame2(pstWin);
                // get and config black frame
                pstFrame = BP_GetBlackFrameInfo();
                WinSetBlackFrameFlag(pstWin);
                
                ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
            }
            else
            {
                //pstFrame = ISR_WinGetLastFrame(&(pstWin->stBuffer));
                WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
                pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);

                //DISP_PRINT("RESET Frame addr=0x%x\n", pstFrame->stBufAddr[0].u32PhyAddr_Y);
                if (!pstFrame)
                {
                    pstFrame = BP_GetBlackFrameInfo();
                }

                ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                WinClearBlackFrameFlag(pstWin);
            }

            s_ResetPrint = 0;

            pstWin->enState = WIN_STATE_WORK;
            pstWin->bReset  = HI_FALSE;
        }
        else
        {
            // window state transfer
            if (pstWin->bUpState)
            {
                ISR_WinStateTransfer(pstWin);
            }

            //printk("==win state=%d>>", pstWin->enState);

            switch(pstWin->enState)
            {
                case WIN_STATE_RESUME:
                case WIN_STATE_UNFREEZE:
                {
                    pstWin->enState = WIN_STATE_WORK;
                    // no break, enter case 'WIN_STATE_WORK'
                }
                case WIN_STATE_WORK:
                {
                    //ISR_WinReleaseDisplayedFrame(pstWin);
                    WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
                    
                    ISR_WinReleaseUSLFrame(pstWin);

                    pstFrame = WinGetFrameToConfig(pstWin);

                    if(pstFrame)
                    {
                        ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                    }
                    
                    break;
                }
                case WIN_STATE_PAUSE:
                {
                    //ISR_WinReleaseDisplayedFrame(pstWin);
                    WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);

                    ISR_WinReleaseUSLFrame(pstWin);
                    
                    //pstFrame = ISR_WinGetLastFrame(&(pstWin->stBuffer));
                    WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);
                    pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);

                    if (!pstFrame)
                    {
                        pstFrame = BP_GetBlackFrameInfo();
                        WinSetBlackFrameFlag(pstWin);
                    }

                    if (pstFrame)
                    {
                        ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                    }
                    
                    break;
                }
                case WIN_STATE_FREEZE:
                {
                    HI_S32 nRet;
                    
                    //ISR_WinReleaseDisplayedFrame(pstWin);
                    WinBuf_RlsAndUpdateUsingFrame(&pstWin->stBuffer.stWinBP);
                    
                    ISR_WinReleaseUSLFrame(pstWin);
                    
                    //pstFrame = ISR_WinGetFreezeFrame(pstWin);
                    WinBuf_RepeatDisplayedFrame(&pstWin->stBuffer.stWinBP);

                    pstFrame = WinBuf_GetConfigedFrame(&pstWin->stBuffer.stWinBP);
                    nRet = WinBuf_ReleaseOneFrame(&pstWin->stBuffer.stWinBP, pstFrame);
                    if (nRet != HI_SUCCESS)
                    {
                        // u32UnderLoad happened
                        pstWin->stBuffer.u32UnderLoad++;
                    }

                    if (pstWin->stFrz.enFreezeMode == HI_DRV_WIN_SWITCH_BLACK)
                    {
                        // find black frame and set flag
                        pstFrame = BP_GetBlackFrameInfo();
                        WinSetBlackFrameFlag(pstWin);
                    }
                    else if (pstFrame)
                    {
                        // if NO_BLACK_FREEZE and has frezen frame, clear flag
                        WinClearBlackFrameFlag(pstWin);
                    }

                    if (pstFrame)
                    {
                        ISR_WinConfigFrame(pstWin, pstFrame, pstInfo);
                    }
                    
                    break;
                }
                default:
                    break;
            }
        }
    }

    ISR_WinUpdatePlayInfo(pstWin, pstInfo);

    return;
}


HI_S32 WinGetProcIndex(HI_HANDLE hWin, HI_U32 *p32Index)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(p32Index);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    *p32Index = pstWin->u32Index;

    return HI_SUCCESS;
}

HI_S32 WinGetProcInfo(HI_HANDLE hWin, WIN_PROC_INFO_S *pstInfo)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pstInfo);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    DISP_MEMSET(pstInfo, 0, sizeof(WIN_PROC_INFO_S));

    pstInfo->enType     = pstWin->enType;
    pstInfo->u32Index   = pstWin->u32Index;
    pstInfo->u32LayerId = (HI_U32)pstWin->u32VideoLayer;
    pstWin->stVLayerFunc.PF_GetZorder(pstWin->u32VideoLayer, &pstInfo->u32Zorder);
    pstInfo->bEnable   = (HI_U32)pstWin->bEnable;
    pstInfo->bMasked   = (HI_U32)pstWin->bMasked;
    pstInfo->u32WinState = (HI_U32)pstWin->enState;

    pstInfo->bReset = pstWin->bReset;
    pstInfo->enResetMode = pstWin->stRst.enResetMode;
    pstInfo->enFreezeMode = pstWin->stFrz.enFreezeMode;

    pstInfo->bQuickMode = pstWin->bQuickMode;
    pstInfo->bStepMode  = pstWin->bStepMode;

    pstInfo->hSrc          = (HI_U32)pstWin->stCfg.stSource.hSrc;
    pstInfo->pfAcqFrame    = (HI_U32)pstWin->stCfg.stSource.pfAcqFrame;
    pstInfo->pfRlsFrame    = (HI_U32)pstWin->stCfg.stSource.pfRlsFrame;
    pstInfo->pfSendWinInfo = (HI_U32)pstWin->stCfg.stSource.pfSendWinInfo;

    pstInfo->stAttr  = pstWin->stCfg.stAttr;

    pstInfo->u32TBNotMatchCount = pstWin->u32TBNotMatchCount;

    pstInfo->eDispMode = pstWin->stCfg.eDispMode;
    pstInfo->bRightEyeFirst = pstWin->stCfg.bRightEyeFirst;

    pstInfo->hSlvWin  = (HI_U32)pstWin->hSlvWin;
    pstInfo->bDebugEn = (HI_U32)pstWin->bDebugEn;

    pstInfo->u32ULSIn  = pstWin->stBuffer.u32ULSIn;
    pstInfo->u32ULSOut = pstWin->stBuffer.u32ULSOut;
    pstInfo->u32UnderLoad = pstWin->stBuffer.u32UnderLoad;

    //BP_GetBufState(&pstWin->stBuffer.stBP, (BUF_STT_S *)&(pstInfo->stBufState));
    WinBuf_GetStateInfo(&pstWin->stBuffer.stWinBP, (WB_STATE_S *)&(pstInfo->stBufState));

    return HI_SUCCESS;
}

HI_S32 WinGetCurrentImg(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    WINDOW_S *pstWin;

    WinCheckDeviceOpen();

    WinCheckNullPointer(pstFrame);

    // s1 检查句柄合法性
    WinCheckWindow(hWin, pstWin);

    DISP_MEMSET(pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    *pstFrame = *(WinBuf_GetDisplayedFrame(&pstWin->stBuffer.stWinBP));
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




