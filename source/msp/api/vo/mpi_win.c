/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <linux/types.h>


#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_mpi_win.h"
#include "drv_win_ioctl.h"

#include "hi_mpi_avplay.h"
#include "hi_error_mpi.h"
#include "drv_struct_ext.h"
#include "drv_vdec_ext.h"


HI_VOID InitCompressor(HI_VOID);

int decompress(unsigned char *pData, int DataLen, int Width, int Height, int stride_luma, int stride_chrome, unsigned char *pFrame);

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif


static HI_S32           g_VoDevFd = -1;
static const HI_CHAR    g_VoDevName[] = "/dev/"UMAP_DEVNAME_VO;
static pthread_mutex_t  g_VoMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_VO_LOCK()     (void)pthread_mutex_lock(&g_VoMutex);
#define HI_VO_UNLOCK()   (void)pthread_mutex_unlock(&g_VoMutex);

#define CHECK_VO_INIT()\
do{\
    HI_VO_LOCK();\
    if (g_VoDevFd < 0)\
    {\
        HI_ERR_WIN("VO is not init.\n");\
        HI_VO_UNLOCK();\
        return HI_ERR_VO_NO_INIT;\
    }\
    HI_VO_UNLOCK();\
}while(0)


HI_S32 HI_MPI_WIN_Init(HI_VOID)
{
    struct stat st;

    HI_VO_LOCK();

    if (g_VoDevFd > 0)
    {
        HI_VO_UNLOCK();
        return HI_SUCCESS;
    }

    if (HI_FAILURE == stat(g_VoDevName, &st))
    {
        HI_FATAL_WIN("VO is not exist.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_DEV_NOT_EXIST;
    }

    if (!S_ISCHR (st.st_mode))
    {
        HI_FATAL_WIN("VO is not device.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_NOT_DEV_FILE;
    }

    g_VoDevFd = open(g_VoDevName, O_RDWR|O_NONBLOCK, 0);

    if (g_VoDevFd < 0)
    {
        HI_FATAL_WIN("open VO err.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_DEV_OPEN_ERR;
    }

    HI_VO_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_DeInit(HI_VOID)
{
    HI_S32 Ret;

    HI_VO_LOCK();

    if (g_VoDevFd < 0)
    {
        HI_VO_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_VoDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_FATAL_WIN("DeInit VO err.\n");
        HI_VO_UNLOCK();
        return HI_ERR_VO_DEV_CLOSE_ERR;
    }

    g_VoDevFd = -1;

    HI_VO_UNLOCK();

    return HI_SUCCESS;
}


HI_S32 HI_MPI_WIN_Create(const HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow)
{
    HI_S32           Ret;
    WIN_CREATE_S  VoWinCreate;

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

    if (pWinAttr->enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enVo is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (pWinAttr->enARCvrs >= HI_DRV_ASP_RAT_MODE_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    memcpy(&VoWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));

    Ret = ioctl(g_VoDevFd, CMD_WIN_CREATE, &VoWinCreate);
    if (Ret != HI_SUCCESS)
    {
	    HI_ERR_WIN("  HI_MPI_WIN_Create failed.\n");
        return Ret;
    }

    *phWindow = VoWinCreate.hWindow;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Destroy(HI_HANDLE hWindow)
{
    HI_S32      Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_DESTROY, &hWindow);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_GetInfo(HI_HANDLE hWin, HI_DRV_WIN_INFO_S * pstInfo)
{
    HI_S32 Ret; 
    WIN_PRIV_INFO_S WinPriv;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    CHECK_VO_INIT();
	
    WinPriv.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_INFO, &WinPriv);
	if (!Ret)
	{
	    *pstInfo = WinPriv.stPrivInfo;
	}
    
    return Ret;
}

HI_S32 HI_MPI_WIN_GetPlayInfo(HI_HANDLE hWin, HI_DRV_WIN_PLAY_INFO_S * pstInfo)
{
    HI_S32 Ret; 
    WIN_PLAY_INFO_S WinPlay;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    CHECK_VO_INIT();
	
    WinPlay.hWindow = hWin;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_PLAY_INFO, &WinPlay);
	if (!Ret)
	{
	    *pstInfo = WinPlay.stPlayInfo;
	}
    
    return Ret;
}


HI_S32 HI_MPI_WIN_SetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{
    HI_S32 Ret; 
    WIN_SOURCE_S VoWinAttach;

    if (HI_INVALID_HANDLE == hWin)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    CHECK_VO_INIT();
	
    VoWinAttach.hWindow = hWin;
    VoWinAttach.stSrc   = *pstSrc;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_SOURCE, &VoWinAttach);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_GetSource(HI_HANDLE hWin, HI_DRV_WIN_SRC_INFO_S *pstSrc)
{

    return HI_FAILURE;
}





HI_S32 HI_MPI_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32            Ret;
    WIN_ENABLE_S   VoWinEnable;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinEnable.hWindow = hWindow;
    VoWinEnable.bEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ENABLE, &VoWinEnable);
    
    return Ret;
}


HI_S32 HI_MPI_VO_SetMainWindowEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{

    return HI_FAILURE;
}


HI_S32 HI_MPI_WIN_GetEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    HI_S32            Ret;
    WIN_ENABLE_S   VoWinEnable;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pbEnable)
    {
        HI_ERR_WIN("para pbEnable is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinEnable.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ENABLE, &VoWinEnable);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pbEnable = VoWinEnable.bEnable;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VO_GetMainWindowEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{


    return HI_FAILURE;
}


HI_S32 HI_MPI_VO_GetWindowsVirtual(HI_HANDLE hWindow, HI_BOOL *pbVirutal)
{


    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_AcquireFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32              Ret;
    WIN_FRAME_S      VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pFrameinfo)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    
    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;
    
    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_ACQUIRE, &VoWinFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    
    memcpy(pFrameinfo, &(VoWinFrame.stFrame), sizeof(HI_DRV_VIDEO_FRAME_S));

    return HI_SUCCESS;

}

HI_S32 HI_MPI_WIN_ReleaseFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32              Ret;
    WIN_FRAME_S      VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    
    CHECK_VO_INIT();
    
    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrameinfo;
    
    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_RELEASE, &VoWinFrame);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetAttr(HI_HANDLE hWindow, const HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32           Ret;
    WIN_CREATE_S  VoWinCreate;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }


    if (pWinAttr->enARCvrs >= HI_DRV_ASP_RAT_MODE_BUTT)
    {
        HI_ERR_WIN("para pWinAttr->enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinCreate.hWindow = hWindow;
    memcpy(&VoWinCreate.WinAttr, pWinAttr, sizeof(HI_DRV_WIN_ATTR_S));

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ATTR, &VoWinCreate);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_GetAttr(HI_HANDLE hWindow, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32           Ret;
    WIN_CREATE_S  VoWinCreate;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinCreate.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ATTR, &VoWinCreate);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pWinAttr, &VoWinCreate.WinAttr, sizeof(HI_DRV_WIN_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_SetZorder(HI_HANDLE hWindow, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_S32            Ret;
    WIN_ZORDER_S   VoWinZorder;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (enZFlag >= HI_DRV_DISP_ZORDER_BUTT)
    {
        HI_ERR_WIN("para enZFlag is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinZorder.hWindow = hWindow;
    VoWinZorder.eZFlag = enZFlag;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_ZORDER, &VoWinZorder);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_GetZorder(HI_HANDLE hWindow, HI_U32 *pu32Zorder)
{
    HI_S32            Ret;
    WIN_ORDER_S   VoWinOrder;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pu32Zorder)
    {
        HI_ERR_WIN("para SrcHandle is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinOrder.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_ORDER, &VoWinOrder);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32Zorder = VoWinOrder.Order;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Attach(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
#if 0
    HI_S32                  Ret;
    WIN_SOURCE_S         VoWinAttach;
    HI_HANDLE               hVdec;
    HI_HANDLE               hSync;
    HI_UNF_AVPLAY_ATTR_S    AvplayAttr;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!hSrc)
    {
        HI_ERR_WIN("para hSrc is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    if(HI_ID_VI == ((hSrc&0xff0000)>>16))
    {
        VoWinAttach.ModId = HI_ID_VI;
    }
    else if(HI_ID_AVPLAY == ((hSrc&0xff0000)>>16))
    {
        Ret = HI_MPI_AVPLAY_GetAttr(hSrc, HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE, &AvplayAttr);
        if (HI_SUCCESS != Ret)
        {        
            return HI_ERR_VO_INVALID_PARA;
        }
        
        VoWinAttach.ModId = HI_ID_AVPLAY;
    }
    else
    {
        return HI_ERR_VO_INVALID_PARA;
    }
    
    if (HI_ID_AVPLAY == VoWinAttach.ModId)
    {
        Ret = HI_MPI_AVPLAY_GetSyncVdecHandle(hSrc, &hVdec, &hSync);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AVPLAY("call HI_MPI_AVPLAY_GetVdecHandle failed.\n");
            return Ret;
        }
        VoWinAttach.hSrc = hVdec;
        VoWinAttach.hSync = hSync;
    }
    else
    {
        VoWinAttach.hSrc = hSrc;
    }

    
    VoWinAttach.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_SOURCE, &VoWinAttach);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    
    if (HI_ID_AVPLAY == VoWinAttach.ModId)
    {
        Ret = HI_MPI_AVPLAY_AttachWindow(hSrc, hWindow);
        if (Ret != HI_SUCCESS) 
        {
            HI_ERR_WIN("call HI_MPI_AVPLAY_AttachWindow failed.\n");
            (HI_VOID)ioctl(g_VoDevFd, CMD_VO_WIN_DETACH, &VoWinAttach);
            return Ret;
        }
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Detach(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
    
    return HI_FAILURE;
}

HI_S32 HI_MPI_VO_SetWindowRatio(HI_HANDLE hWindow, HI_U32 u32WinRatio)
{

    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_Freeze(HI_HANDLE hWindow, HI_BOOL bEnable, HI_DRV_WIN_SWITCH_E enWinFreezeMode)
{
    HI_S32           Ret;
    WIN_FREEZE_S  VoWinFreeze;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    if (enWinFreezeMode >= HI_DRV_WIN_SWITCH_BUTT)
    {
        HI_ERR_WIN("para enWinFreezeMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinFreeze.hWindow = hWindow;
    VoWinFreeze.bEnable = bEnable;
    VoWinFreeze.eMode   = enWinFreezeMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_FREEZE, &VoWinFreeze);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_SetFieldMode(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret;
    WIN_FRAME_S     VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrame is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow  = hWindow;
	VoWinFrame.stFrame = *pFrame;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SEND_FRAME, &VoWinFrame);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_DequeueFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32  Ret;
    WIN_FRAME_S VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_DQ_FRAME, &VoWinFrame);
    if (!Ret)
    {
		*pFrame = VoWinFrame.stFrame;
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_QueueFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret;
    WIN_FRAME_S     VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrame;


	Ret = ioctl(g_VoDevFd, CMD_WIN_QU_FRAME, &VoWinFrame);

    return Ret;
}

HI_S32 HI_MPI_WIN_QueueUselessFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32             Ret;
    WIN_FRAME_S     VoWinFrame;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pFrame)
    {
        HI_ERR_WIN("para pFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinFrame.hWindow = hWindow;
	VoWinFrame.stFrame = *pFrame;
	
    Ret = ioctl(g_VoDevFd, CMD_WIN_QU_ULSFRAME, &VoWinFrame);

    return Ret;
}


HI_S32 HI_MPI_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enWinFreezeMode)
{
    WIN_RESET_S   VoWinReset;
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (enWinFreezeMode >= HI_DRV_WIN_SWITCH_BUTT)
    {
        HI_ERR_WIN("para enWinFreezeMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinReset.hWindow = hWindow;
    VoWinReset.eMode = enWinFreezeMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_RESET, &VoWinReset);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_Pause(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    WIN_PAUSE_S   VoWinPause;
    HI_S32           Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_WIN("para bEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();

    VoWinPause.hWindow = hWindow;
    VoWinPause.bEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_PAUSE, &VoWinPause);
    
    return Ret;
}


HI_S32 HI_MPI_VO_GetWindowDelay(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pDelay)
{
    WIN_PLAY_INFO_S   VoWinDelay;
    HI_S32 Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pDelay)
    {
        HI_ERR_WIN("para pDelay is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinDelay.hWindow = hWindow;

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_PLAY_INFO, &VoWinDelay);
    if (!Ret)
    {
        *pDelay = VoWinDelay.stPlayInfo;
    }

    return Ret;
}

HI_S32 HI_MPI_WIN_SetStepMode(HI_HANDLE hWindow, HI_BOOL bStepMode)
{
    HI_S32              Ret;
    WIN_STEP_MODE_S  WinStepMode;
    
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    CHECK_VO_INIT();

    WinStepMode.hWindow = hWindow;
    WinStepMode.bStep = bStepMode;

    Ret = ioctl(g_VoDevFd, CMD_WIN_STEP_MODE, &WinStepMode);
    
    return Ret;
}

HI_S32 HI_MPI_WIN_SetStepPlay(HI_HANDLE hWindow)
{
    HI_S32      Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_STEP_PLAY, &hWindow);
    
    return Ret;
}

HI_S32 HI_MPI_VO_DisableDieMode(HI_HANDLE hWindow)
{
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SetExtBuffer(HI_HANDLE hWindow, HI_DRV_VIDEO_BUFFER_POOL_S* pstBufAttr)
{
    HI_S32      Ret;
//    HI_S32      s32Index;
    WIN_BUF_POOL_S  bufferAttr;
	
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    CHECK_VO_INIT();

    bufferAttr.hwin = hWindow;
    bufferAttr.stBufPool = *pstBufAttr;

    Ret = ioctl(g_VoDevFd, CMD_WIN_VIR_EXTERNBUF, &bufferAttr);
    
    return Ret;
}


HI_S32 HI_MPI_WIN_SetQuickOutput(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    WIN_SET_QUICK_S stQuickOutputAttr;
    HI_S32      Ret;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    CHECK_VO_INIT();

    stQuickOutputAttr.hWindow = hWindow;
    stQuickOutputAttr.bQuickEnable = bEnable;

    Ret = ioctl(g_VoDevFd, CMD_WIN_SET_QUICK, &stQuickOutputAttr);
    
    return Ret;
}



HI_S32 HI_MPI_VO_UseDNRFrame(HI_HANDLE hWindow, HI_BOOL bEnable)
{
 
    return HI_FAILURE;
}


#if 0
HI_S32 HI_MPI_VO_CapturePictureExt(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture,  VO_CAPTURE_MEM_MODE_S *pstCapMode)
{
    HI_S32            Ret;
    WIN_CAPTURE_S VoWinCapture;
    WIN_CAPTURE_S    VoWinRls;



    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }

    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!pstCapMode)
    {
        HI_ERR_WIN("para pstCapMode is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();

    VoWinCapture.hWindow = hWindow;
    memcpy(&(VoWinCapture.MemMode), pstCapMode, sizeof(VO_CAPTURE_MEM_MODE_S));

#if  defined (CHIP_TYPE_hi3712)

    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_START, &VoWinCapture);
    goto release;

#elif defined (CHIP_TYPE_hi3716h) \
    || defined (CHIP_TYPE_hi3716c)  \
    || defined (CHIP_TYPE_hi3716cv200es)  \
    ||defined (CHIP_TYPE_hi3716m)

    HI_U32  datalen,i;
    HI_UCHAR          *DecompressOutBuf = HI_NULL, *DecompressInBuf = HI_NULL, *Inptr = HI_NULL, *Outptr = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo;

    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(VoWinCapture.CapPicture.u32Private);
    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_START, &VoWinCapture);
    if (Ret != HI_SUCCESS) 
    {
        return Ret;
    }

    if (pstCapMode->enAllocMemType == VO_CAPTURE_DRIVER_ALLOC) 
    {
        /* we should remap the addr and then use it */
        DecompressOutBuf = (HI_UCHAR *)(HI_MMAP(VoWinCapture.MemMode.u32StartPhyAddr, VoWinCapture.MemMode.u32DataLen));
        VoWinCapture.MemMode.u32StartUserAddr = (HI_U32)DecompressOutBuf;
    }
    else if (pstCapMode->enAllocMemType == VO_CAPTURE_USER_ALLOC) 
    {
        DecompressOutBuf = (HI_UCHAR *)pstCapMode->u32StartUserAddr;
    }
    if (HI_TRUE == pstPrivInfo->stCompressInfo.u32CompressFlag) 
    {
        if ( HI_UNF_FORMAT_YUV_SEMIPLANAR_420 == VoWinCapture.CapPicture.enVideoFormat) 
        {
            datalen = (pstPrivInfo->stCompressInfo.s32CompFrameHeight) * (VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride) * 3 / 2;
        }
        else 
        {
            datalen = pstPrivInfo->stCompressInfo.s32CompFrameHeight * VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride * 2;
        }

        datalen += pstPrivInfo->stCompressInfo.s32CompFrameHeight * 4;
    }
    else
    {
        if ( HI_UNF_FORMAT_YUV_SEMIPLANAR_420 == VoWinCapture.CapPicture.enVideoFormat) 
        {
            datalen = (VoWinCapture.CapPicture.u32Height) * (VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride) * 3 / 2;
        }
        else 
        {
            datalen = VoWinCapture.CapPicture.u32Height * VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride * 2;
        }
    }

    if (pstCapMode->enAllocMemType != VO_CAPTURE_NO_ALLOC) 
    {
        DecompressInBuf =(HI_UCHAR *)(HI_MMAP(VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YAddr, datalen));
    }
    if (pstPrivInfo->stCompressInfo.u32CompressFlag == HI_TRUE) 
    {
        /* decompress  */
        InitCompressor();
        Ret = decompress(DecompressInBuf, 0, pstPrivInfo->stCompressInfo.s32CompFrameWidth, 
                pstPrivInfo->stCompressInfo.s32CompFrameHeight, VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride, 
                VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride, DecompressOutBuf);
#if 0
        FILE    *yuvfile=fopen("./t.yuv", "wb");
        if (yuvfile == HI_NULL)
        {
            printf("open yuv file fail\r\n");
            return HI_FAILURE;
        }
        printf("yuv file len %d \r\n", VoWinCapture.MemMode.u32DataLen);
        fwrite(DecompressOutBuf, 1, VoWinCapture.MemMode.u32DataLen, yuvfile);
        fflush(yuvfile);
#endif

        if (Ret <= 0) 
        {
            HI_ERR_WIN("decompress data fail\r\n");
            Ret  = HI_FAILURE;
        }
        else 
        {
            Ret  = HI_SUCCESS;
        }
    }
    else 
    {
        if (pstCapMode->enAllocMemType != VO_CAPTURE_NO_ALLOC) 
        {
            /* just copy */
            Inptr = DecompressInBuf; 
            Outptr = DecompressOutBuf;

            if ( (HI_NULL ==Inptr) ||(HI_NULL ==Outptr) ||(0 == VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride))
                return HI_FAILURE;

            for(i = 0 ; i < datalen / VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride; i++) 
            {
                memcpy(Outptr, Inptr, VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride);
                Inptr += VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride;
                Outptr += VoWinCapture.CapPicture.stVideoFrameAddr[0].u32YStride;
            }
        }
    }

    if (pstCapMode->enAllocMemType != VO_CAPTURE_NO_ALLOC) 
    {
        if (HI_SUCCESS != HI_MUNMAP((void*)DecompressInBuf)) 
        {
            HI_ERR_WIN("decompress buffer unmap fail\r\n");
            goto release;
        }
    }

    if (pstCapMode->enAllocMemType == VO_CAPTURE_DRIVER_ALLOC) 
    {
        if (HI_SUCCESS != HI_MUNMAP((void*)DecompressOutBuf))
        {
            HI_ERR_WIN("decompress buffer unmap fail\r\n");
            goto release;
        }
    }
#else
#error YOU MUST DEFINE  CHIP_TYPE!
#endif

release:
    /*  release freezed frame*/
    VoWinRls.hWindow = hWindow;
    memcpy(&(VoWinRls.MemMode), &(VoWinCapture.MemMode), sizeof(VO_CAPTURE_MEM_MODE_S));
    ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_RELEASE, &VoWinRls);
    if (Ret != HI_SUCCESS) 
    {
        return Ret;
    }

    memcpy(pstCapPicture, &VoWinCapture.CapPicture, sizeof(HI_DRV_VIDEO_FRAME_S));
    pstCapPicture->stVideoFrameAddr[0].u32YAddr = VoWinCapture.MemMode.u32StartPhyAddr;
    pstCapPicture->stVideoFrameAddr[0].u32CAddr = VoWinCapture.MemMode.u32StartPhyAddr + pstCapPicture->u32Height * pstCapPicture->stVideoFrameAddr[0].u32YStride;

    return HI_SUCCESS;

}
#endif

HI_S32 HI_MPI_WIN_CapturePicture(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture)
{
#if 0
    HI_S32            Ret ;
	WIN_CAPTURE_S stCap;
//  HI_SYS_VERSION_S pstVersion;
    VO_CAPTURE_MEM_MODE_S stCapMode;

#if 0
    HI_SYS_GetVersion(&pstVersion);
    if((pstVersion.enChipVersion != HI_CHIP_VERSION_V300)&&(pstVersion.enChipTypeSoft != HI_CHIP_TYPE_HI3712))
    {
        stCapMode.enAllocMemType = VO_CAPTURE_NO_ALLOC;
    }
    else
#endif
    {
        stCapMode.enAllocMemType = VO_CAPTURE_DRIVER_ALLOC;
    }
    Ret = HI_MPI_VO_CapturePictureExt(hWindow, pstCapPicture,  &stCapMode);
    return Ret;
#endif
	return HI_SUCCESS;
}


HI_S32 HI_MPI_WIN_CapturePictureRelease(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pstCapPicture)
{
#if 0
    HI_S32            Ret ;
    WIN_CAPTURE_S   VoWinRls;
    //HI_SYS_VERSION_S pstVersion;

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }


    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    CHECK_VO_INIT();
#if 0
    HI_SYS_GetVersion(&pstVersion);

    if((pstVersion.enChipVersion != HI_CHIP_VERSION_V300)&&(pstVersion.enChipTypeSoft != HI_CHIP_TYPE_HI3712))
    {
        return HI_SUCCESS;
    }
#endif
    /*  release freezed frame*/
    VoWinRls.hWindow = hWindow;

    memcpy(&(VoWinRls.CapPicture), pstCapPicture, sizeof(HI_DRV_VIDEO_FRAME_S));

    Ret = ioctl(g_VoDevFd, CMD_VO_WIN_CAPTURE_FREE, &VoWinRls);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_CapturePictureRelease fail (INVALID_PARA)\r\n");
        return Ret;
    }
#endif
    return HI_SUCCESS;

}


HI_S32 HI_MPI_WIN_SetRotation(HI_HANDLE hWindow, HI_DRV_ROT_ANGLE_E enRotation)
{
    HI_ERR_WIN("this chip version can not support this operation\n");
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_GetRotation(HI_HANDLE hWindow, HI_DRV_ROT_ANGLE_E *penRotation)
{
    HI_ERR_WIN("this chip version can not support this operation\n");
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_SetFlip(HI_HANDLE hWindow, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    HI_ERR_WIN("this chip version can not support this operation\n");
    return HI_FAILURE;
}

HI_S32 HI_MPI_WIN_GetFlip(HI_HANDLE hWindow, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip)
{
    HI_ERR_WIN("this chip version can not support this operation\n");
    return HI_FAILURE;
}

#if 0
HI_S32 HI_MPI_VO_SetWindowExtAttr(HI_HANDLE hWindow, VO_WIN_EXTATTR_E detType, HI_BOOL bEnable)
{
    HI_S32      Ret;
    VO_WIN_DETECT_S stDetType;
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    CHECK_VO_INIT();
    stDetType.hWindow = hWindow;
    stDetType.detType = detType;
    stDetType.bEnable = bEnable;
    Ret = ioctl(g_VoDevFd, CMD_VO_SET_DET_MODE, &stDetType);

    return Ret;
}

HI_S32 HI_MPI_VO_GetWindowExtAttr(HI_HANDLE hWindow, VO_WIN_EXTATTR_E detType, HI_BOOL *bEnable)
{
    HI_S32      Ret;
    VO_WIN_DETECT_S stDetType;
    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_WIN("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    CHECK_VO_INIT();

    stDetType.hWindow = hWindow;
    stDetType.detType = detType;
    Ret = ioctl(g_VoDevFd, CMD_VO_GET_DET_MODE, &stDetType);
    *bEnable = stDetType.bEnable;

    return Ret;
}
#endif

HI_S32 HI_MPI_WIN_Suspend(HI_VOID)
{
    HI_U32 u32Value = 0x88888888;
    HI_S32 Ret;

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_SUSPEND, &u32Value);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_Suspend failed\n");
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_Resume(HI_VOID)
{
    HI_U32 u32Value = 0x88888888;
    HI_S32 Ret;

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_RESUM, &u32Value);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_Resume failed\n");
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_WIN_GetHandle(WIN_GET_HANDLE_S *pstWinHandle)
{
    HI_S32 Ret;

    CHECK_VO_INIT();

    Ret = ioctl(g_VoDevFd, CMD_WIN_GET_HANDLE, pstWinHandle);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_WIN("HI_MPI_WIN_GetHandle failed\n");
    }

    return Ret;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

