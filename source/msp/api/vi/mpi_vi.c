/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/26
  Description   :
  History       :
  1.Date        : 2010/03/26
    Author      : j00131665
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
#include <pthread.h>

#include "drv_struct_ext.h"
#include "hi_drv_vi.h"
#include "hi_mpi_vi.h"
#include "hi_mpi_mem.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
 #endif
#endif

static HI_S32 g_ViDevFd = -1;
static const HI_CHAR g_ViDevName[] = "/dev/" UMAP_DEVNAME_VI;
static pthread_mutex_t g_ViMutex = PTHREAD_MUTEX_INITIALIZER;
//static HI_UNF_VI_BUF_S struViBuf[VIU_FB_MAX_NUM][MAX_VI_CHN];

#define HI_VI_LOCK() (HI_VOID)pthread_mutex_lock(&g_ViMutex);
#define HI_VI_UNLOCK() (HI_VOID)pthread_mutex_unlock(&g_ViMutex);

#define CHECK_VI_INIT() \
    do {\
        HI_VI_LOCK(); \
        if (g_ViDevFd < 0)\
        {\
            HI_VI_UNLOCK(); \
            return HI_ERR_VI_NO_INIT; \
        } \
        HI_VI_UNLOCK(); \
    } while (0)

#define CHECK_VI_NULL_PTR(ptr) \
    do {\
        if (NULL == (ptr))\
        {\
            HI_ERR_VI("PTR('%s') is NULL.\n", # ptr); \
            return HI_ERR_VI_NULL_PTR; \
        } \
    } while (0)

//todo, get port and channel ID
#define CHECK_VI_HANDLE_AND_GET_VIPORT(hVi, enViPort) \
    do { \
        enViPort = (HI_UNF_VI_E)((hVi) & 0xffff); \
        if ((HI_INVALID_HANDLE == hVi) || (HI_NULL == hVi))\
        {\
            HI_ERR_VI("VI handle(%#x) is invalid.\n", hVi); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if ((HI_UNF_VI_PORT0 > enViPort) || (HI_UNF_VI_BUTT <= enViPort))\
        {\
            HI_ERR_VI("VI handle(%#x) is invalid, port error.\n", hVi); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if (HI_ID_VI != ((hVi) >> 16)) \
        {\
            HI_ERR_VI("VI handle(%#x) is invalid, modID error.\n", hVi); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
    } while (0)

HI_S32 HI_MPI_VI_Init(HI_VOID)
{
    struct stat st;

    HI_VI_LOCK();

    /* Already Opened in the process*/
    if (g_ViDevFd > 0)
    {
        HI_VI_UNLOCK();
        return HI_SUCCESS;
    }

    if (HI_FAILURE == stat(g_ViDevName, &st))
    {
        HI_FATAL_VI("VI is not exist.\n");
        HI_VI_UNLOCK();
        return HI_ERR_VI_DEV_NOT_EXIST;
    }

    if (!S_ISCHR (st.st_mode))
    {
        HI_FATAL_VI("VI is not device.\n");
        HI_VI_UNLOCK();
        return HI_ERR_VI_NOT_DEV_FILE;
    }

    g_ViDevFd = open(g_ViDevName, O_RDWR | O_NONBLOCK, 0);

    if (g_ViDevFd < 0)
    {
        HI_FATAL_VI("open VI err.\n");
        HI_VI_UNLOCK();
        return HI_FAILURE;
    }

    HI_VI_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_DeInit(HI_VOID)
{
    HI_VI_LOCK();

    if (g_ViDevFd < 0)
    {
        HI_VI_UNLOCK();
        return HI_SUCCESS;
    }

    close(g_ViDevFd);

    g_ViDevFd = -1;
    HI_VI_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_SetAttr(HI_HANDLE handle, const HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 Ret;
    VI_ATTR_S stViAttr;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pstAttr);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    stViAttr.enVi = enVi;
    memcpy(&(stViAttr.stAttr), pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_SET_ATTR, &stViAttr);
    return Ret;
}

HI_S32 HI_MPI_VI_GetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 Ret;
    VI_ATTR_S stViAttr;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pstAttr);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    stViAttr.enVi = enVi;

    Ret = ioctl(g_ViDevFd, CMD_VI_GET_ATTR, &stViAttr);
    if (HI_SUCCESS == Ret)
    {
        memcpy(pstAttr, &(stViAttr.stAttr), sizeof(HI_UNF_VI_ATTR_S));
    }

    return Ret;
}

//HI_S32 HI_MPI_VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phVi)
HI_S32 HI_MPI_VI_Create(HI_UNF_VI_E enViPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phVi)
{
//    HI_U32 i;
    HI_S32 Ret;
    VI_CREATE_INFO stCreateInfo;
    HI_SYS_VERSION_S stVersion;
//    HI_VOID *pViVirAddr = NULL;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(phVi);
    CHECK_VI_NULL_PTR(pstAttr);

#if 0
    if ((HI_UNF_VI_PORT0 > pstAttr->enViPort) || (HI_UNF_VI_PORT1 < pstAttr->enViPort))
    {
        HI_ERR_VI("invalid vi port %d.\n", pstAttr->enViPort);
        return HI_ERR_VI_INVALID_PARA;
    }
#endif
    if ((HI_UNF_VI_PORT0 > enViPort) || (HI_UNF_VI_PORT1 < enViPort))
    {
        HI_ERR_VI("invalid vi port %d.\n", enViPort);
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->stInputRect.s32X < 0)
        || (pstAttr->stInputRect.s32Y < 0)
        || (pstAttr->stInputRect.s32Width <= 0)
        || (pstAttr->stInputRect.s32Height <= 0))
    {
        HI_ERR_VI("invalid vi input rect x(%d) y(%d) width(%d) height(%d).\n", pstAttr->stInputRect.s32X,
                  pstAttr->stInputRect.s32Y, pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->stInputRect.s32X > VIU_WIDTH_MAX)
        || (pstAttr->stInputRect.s32Y > VIU_HIGHT_MAX)
        || (pstAttr->stInputRect.s32Width >= VIU_WIDTH_MAX)
        || (pstAttr->stInputRect.s32Height >= VIU_HIGHT_MAX))
    {
        HI_ERR_VI("invalid vi input rect x(%d) y(%d) width(%d) height(%d).\n", pstAttr->stInputRect.s32X,
                  pstAttr->stInputRect.s32Y, pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->enInputMode > HI_UNF_VI_MODE_BT1120_1080P_60) || (pstAttr->enInputMode < HI_UNF_VI_MODE_BT656_576I))
    {
        HI_ERR_VI("invalid vi input mode %d.\n", pstAttr->enInputMode);
        return HI_ERR_VI_INVALID_PARA;
    }
#if 0
    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (pstAttr->enChnYC == HI_UNF_VI_CHN_YC_SEL_C))
    {
        HI_ERR_VI("invalid vi input mode %d with YC selcet mode.\n", pstAttr->enInputMode);
        return HI_ERR_VI_INVALID_PARA;
    }
#endif
    if (HI_SUCCESS == HI_SYS_GetVersion(&stVersion))
    {
/*
        if (((HI_CHIP_TYPE_HI3716M == stVersion.enChipTypeSoft) && (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT656_576I)
             && (pstAttr->enInputMode <= HI_UNF_VI_MODE_BT601))
            || ((HI_CHIP_TYPE_HI3716M == stVersion.enChipTypeSoft)
                && (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
                && (pstAttr->enInputMode <= HI_UNF_VI_MODE_BT1120_1080P)))
        {
            HI_ERR_VI("HI3716M does not support BT656/BT1120 input mode.\n");
            return HI_ERR_VI_INVALID_PARA;
        }
*/
        if ((HI_CHIP_TYPE_HI3716M == stVersion.enChipTypeSoft) && (HI_FALSE == pstAttr->bVirtual))
        {
            HI_ERR_VI("HI3716M does not support BT656/BT601/BT1120 input mode.\n");
            return HI_ERR_VI_INVALID_PARA;
        }
        if ((HI_CHIP_TYPE_HI3716H == stVersion.enChipTypeSoft) && (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            && (pstAttr->enInputMode <= HI_UNF_VI_MODE_BT1120_1080P_60))
        {
            HI_ERR_VI("HI3716H does not support BT1120 input mode.\n");
            return HI_ERR_VI_INVALID_PARA;
        }
    }
    else
    {
        HI_ERR_VI("HI_SYS_GetVersion failed.\n");
        return HI_ERR_VI_INVALID_PARA;
    }
	
#if 0
    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (pstAttr->enViPort == HI_UNF_VI_PORT1))
    {
        HI_ERR_VI("invalid vi port with bt1120 mode.\n");
        return HI_ERR_VI_INVALID_PARA;
    }

    if (pstAttr->enStoreMethod >= HI_UNF_VI_STORE_METHOD_BUTT)
    {
        HI_ERR_VI("invalid vi store method %d.\n", pstAttr->enStoreMethod);
        return HI_ERR_VI_INVALID_PARA;
    }

    if (pstAttr->enStoreMode >= HI_UNF_VI_STORE_BUTT)
    {
        HI_ERR_VI("invalid vi store mode %d.\n", pstAttr->enStoreMode);
        return HI_ERR_VI_INVALID_PARA;
    }
#endif
    if (pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_BUTT)
    {
        HI_ERR_VI("invalid vi video format %d.\n", pstAttr->enVideoFormat);
        return HI_ERR_VI_INVALID_PARA;
    }
#if 0
    if (pstAttr->enFieldMode >= HI_UNF_VIDEO_FIELD_BUTT)
    {
        HI_ERR_VI("invalid vi filed mode %d.\n", pstAttr->enFieldMode);
        return HI_ERR_VI_INVALID_PARA;
    }
    if (pstAttr->enCapSel >= HI_UNF_VI_CAPSEL_BUTT)
    {
        HI_ERR_VI("invalid vi capture select mode %d.\n", pstAttr->enCapSel);
        return HI_ERR_VI_INVALID_PARA;
    }
#endif

    if (pstAttr->enBufMgmtMode >= HI_UNF_VI_BUF_BUTT)
    {
        HI_ERR_VI("invalid vi buf management mode %d.\n", pstAttr->enBufMgmtMode);
        return HI_ERR_VI_INVALID_PARA;
    }
#if 0
    if ((pstAttr->enBufMgmtMode == HI_UNF_VI_BUF_BUTT) && (pstAttr->enInputMode != HI_UNF_VI_MODE_USB_CAM))
    {
        HI_ERR_VI("not support vi input mode %d with buf management mode %d.\n", pstAttr->enInputMode,
                  pstAttr->enBufMgmtMode);
        return HI_ERR_VI_NOT_SUPPORT;
    }
#endif
    memcpy(&(stCreateInfo.stViAttr), pstAttr, sizeof(HI_UNF_VI_ATTR_S));
	stCreateInfo.enViPort = enViPort;

    Ret = ioctl(g_ViDevFd, CMD_VI_OPEN, &stCreateInfo);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pstAttr, &(stCreateInfo.stViAttr), sizeof(HI_UNF_VI_ATTR_S));

//    *phVi = ((HI_ID_VI << 16) | stCreateInfo.enViPort); //todo, viport and channel ID
	*phVi = ((HI_ID_VI << 16) | enViPort);

	/* delete virtual address, because the frame info do not use virtual addr, use phyaddr instead */
#if 0	
    /* save vi buffer addr, for user use */
    if (HI_UNF_VI_BUF_ALLOC == pstAttr->enBufMgmtMode)
    {
        pViVirAddr = HI_MMAP(pstAttr->u32ViBufAddr, pstAttr->u32BufSize);
        HI_INFO_VI("pViVirAddr = 0x%08x\n", pViVirAddr);
        if (!pViVirAddr)
        {
            HI_ERR_VI("vibuf HI_MMAP() failed, size=%d , PhyAddr=%x\r\n",
                      pstAttr->u32BufSize,
                      pstAttr->u32ViBufAddr);

            ioctl(g_ViDevFd, CMD_VI_CLOSE, *phVi);

            return HI_ERR_VI_CHN_INIT_BUF_ERR;
        }

        HI_INFO_VI("vi frame size=%d, bufsize=%d, PhyAddr=%x, VirAddr=%x, YStride=%d, CStride=%d\r\n",
                   pstAttr->u32FrameSize,
                   pstAttr->u32BufSize,
                   pstAttr->u32ViBufAddr,
                   (HI_U32)pViVirAddr,
                   pstAttr->u32YStride,
                   pstAttr->u32CStride);

        for (i = 0; i < pstAttr->u32BufNum; i++)
        {
            HI_INFO_VI("Y addr %x ,C addr %x\n", pstAttr->u32ViBufAddr + i * pstAttr->u32FrameSize,
                       pstAttr->u32BufNum + i * pstAttr->u32FrameSize);

            if (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            {
                struViBuf[i][pstAttr->enViPort].pVirAddr[0] = (HI_U32*)(pstAttr->u32ViBufAddr + i
                                                                        * pstAttr->u32FrameSize);
                struViBuf[i][pstAttr->enViPort].pVirAddr[1] = (HI_U32*)(pstAttr->u32BufSize + i * pstAttr->u32FrameSize);
                struViBuf[i][pstAttr->enViPort].pVirAddr[2] = HI_NULL;
            }
            else
            {
                struViBuf[i][pstAttr->enViPort].pVirAddr[0] = (HI_U32*)pViVirAddr + i * pstAttr->u32FrameSize;
                struViBuf[i][pstAttr->enViPort].pVirAddr[1] = HI_NULL;
                struViBuf[i][pstAttr->enViPort].pVirAddr[2] = HI_NULL;
            }

            if ((HI_UNF_VI_STORE_METHOD_PNYUV == pstAttr->enStoreMethod)
                || (HI_UNF_VI_STORE_METHOD_SPNYC == pstAttr->enStoreMethod))
            {
                struViBuf[i][pstAttr->enViPort].pVirAddr[1] = (HI_U8*)struViBuf[i][pstAttr->enViPort].pVirAddr[0]
                                                              + (HI_U32)pstAttr->stInputRect.s32Height
                                                              * pstAttr->u32YStride;
            }

            if (HI_UNF_VI_STORE_METHOD_PNYUV == pstAttr->enStoreMethod)
            {
                struViBuf[i][pstAttr->enViPort].pVirAddr[2] = (HI_U8*)struViBuf[i][pstAttr->enViPort].pVirAddr[1]
                                                              + (HI_U32)pstAttr->stInputRect.s32Height
                                                              * pstAttr->u32CStride;
            }
        }
    }
    else
    {
        for (i = 0; i < pstAttr->u32BufNum; i++)
        {
            struViBuf[0][pstAttr->enViPort].pVirAddr[0] = HI_NULL;
            struViBuf[0][pstAttr->enViPort].pVirAddr[1] = HI_NULL;
            struViBuf[0][pstAttr->enViPort].pVirAddr[2] = HI_NULL;
        }
    }
#endif
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Destroy(HI_HANDLE hVi)
{
    HI_S32 Ret;

    HI_UNF_VI_E enVi = HI_UNF_VI_BUTT;

    CHECK_VI_INIT();
    CHECK_VI_HANDLE_AND_GET_VIPORT(hVi, enVi);
#if 0
    if (struViBuf[0][enVi].pVirAddr[0])
    {
        HI_MUNMAP(struViBuf[0][enVi].pVirAddr[0]);
    }
#endif
    Ret = ioctl(g_ViDevFd, CMD_VI_CLOSE, &enVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Start(HI_HANDLE hVi)
{
    HI_S32 Ret;

    HI_UNF_VI_E enVi = HI_UNF_VI_BUTT;

    CHECK_VI_INIT();
    CHECK_VI_HANDLE_AND_GET_VIPORT(hVi, enVi);

    Ret = ioctl(g_ViDevFd, CMD_VI_START, &enVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_Stop(HI_HANDLE hVi)
{
    HI_S32 Ret;

    HI_UNF_VI_E enVi = HI_UNF_VI_BUTT;

    CHECK_VI_INIT();
    CHECK_VI_HANDLE_AND_GET_VIPORT(hVi, enVi);

    Ret = ioctl(g_ViDevFd, CMD_VI_STOP, &enVi);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_SetExternBuffer(HI_HANDLE handle, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr)
{
	HI_S32 Ret;
	VI_BUF_ATTR_S stViBufAttr;
	HI_UNF_VI_E enVi;

	CHECK_VI_INIT();
	CHECK_VI_NULL_PTR(pstBufAttr);
	CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

	stViBufAttr.enVi = enVi;
	memcpy(&(stViBufAttr.stBufAttr), pstBufAttr, sizeof(HI_UNF_VI_BUFFER_ATTR_S));

	Ret = ioctl(g_ViDevFd, CMD_VI_SET_BUF, &stViBufAttr);
	return Ret;
}
#if 1
HI_S32 HI_MPI_VI_QueueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S ViFrame;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_VI_HANDLE_AND_GET_VIPORT(hVI, enVi);

    ViFrame.enVi   = enVi;
    ViFrame.u32Uid = 0;
    memcpy(&(ViFrame.stViFrame), pFrameInfo, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_Q_FRAME, &ViFrame);

    return Ret;
}
HI_S32 HI_MPI_VI_DequeueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S stViFrameInfo;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_VI_HANDLE_AND_GET_VIPORT(hVI, enVi);

    stViFrameInfo.enVi   = enVi;
    stViFrameInfo.u32Uid = 0;
    Ret = ioctl(g_ViDevFd, CMD_VI_DQ_FRAME, &stViFrameInfo);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pFrameInfo, &(stViFrameInfo.stViFrame), sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    if (pFrameInfo->u32FrameIndex >= VIU_FB_MAX_NUM)
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    return HI_SUCCESS;
}
#else

HI_S32 HI_MPI_VI_GetFrame(HI_HANDLE handle, HI_UNF_VI_BUF_S * pViBuf)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S stViFrameInfo;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pViBuf);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    stViFrameInfo.enVi   = enVi;
    stViFrameInfo.u32Uid = 0;
    Ret = ioctl(g_ViDevFd, CMD_VI_GET_FRAME, &stViFrameInfo);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pViBuf, &(stViFrameInfo.stViBuf), sizeof(HI_UNF_VI_BUF_S));

    if (pViBuf->u32FrameIndex >= VIU_FB_MAX_NUM)
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    if (HI_UNF_VI_BUF_ALLOC == pViBuf->enBufMode)
    {
        pViBuf->pVirAddr[0] = struViBuf[pViBuf->u32FrameIndex][enVi].pVirAddr[0];
        pViBuf->pVirAddr[1] = struViBuf[pViBuf->u32FrameIndex][enVi].pVirAddr[1];
        pViBuf->pVirAddr[2] = struViBuf[pViBuf->u32FrameIndex][enVi].pVirAddr[2];
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_PutFrame(HI_HANDLE handle, const HI_UNF_VI_BUF_S * pViBuf)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S ViFrame;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pViBuf);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    ViFrame.enVi   = enVi;
    ViFrame.u32Uid = 0;
    memcpy(&(ViFrame.stViBuf), pViBuf, sizeof(HI_UNF_VI_BUF_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_PUT_FRAME, &ViFrame);

    return Ret;
}
#endif

HI_S32 HI_MPI_VI_AcquireFrame(HI_HANDLE handle, HI_U32 u32Uid, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S stViFrameInfo;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    stViFrameInfo.enVi   = enVi;
    stViFrameInfo.u32Uid = u32Uid;
    Ret = ioctl(g_ViDevFd, CMD_VI_ACQUIRE_FRAME, &stViFrameInfo);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pFrameInfo, &(stViFrameInfo.stViFrame), sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    if (pFrameInfo->u32FrameIndex >= VIU_FB_MAX_NUM)
    {
        return HI_ERR_VI_INVALID_PARA;
    }
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_ReleaseFrame(HI_HANDLE handle, HI_U32 u32Uid, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 Ret;
    VI_FRAME_INFO_S ViFrame;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pFrameInfo);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    ViFrame.enVi   = enVi;
    ViFrame.u32Uid = u32Uid;
    memcpy(&(ViFrame.stViFrame), pFrameInfo, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    Ret = ioctl(g_ViDevFd, CMD_VI_RELEASE_FRAME, &ViFrame);

    return Ret;
}

HI_S32 HI_MPI_VI_GetUsrID(HI_HANDLE handle, HI_U32 *pu32UId)
{
    HI_S32 s32Ret = HI_FAILURE;
    VI_UID_INFO_S stUidInfo;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_NULL_PTR(pu32UId);
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    stUidInfo.enVi = enVi;
    s32Ret = ioctl(g_ViDevFd, CMD_VI_GET_UID, &stUidInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    *pu32UId = stUidInfo.u32UsrID;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_VI_PutUsrID(HI_HANDLE handle, HI_U32 u32UId)
{
    HI_S32 s32Ret = HI_FAILURE;
    VI_UID_INFO_S stUidInfo;
    HI_UNF_VI_E enVi;

    CHECK_VI_INIT();
    CHECK_VI_HANDLE_AND_GET_VIPORT(handle, enVi);

    s32Ret = ioctl(g_ViDevFd, CMD_VI_PUT_UID, &stUidInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

//follow functions are for testing
static FILE *g_pVpDumpFile = NULL;

HI_S32 yuv_dump_start(char *filename)
{
    if (filename == NULL)
    {
        HI_ERR_VI("yuv_dump_start: filename == NULL!\n");
        return HI_FAILURE;
    }

    g_pVpDumpFile = fopen(filename, "wb");
    if (g_pVpDumpFile == NULL)
    {
        HI_ERR_VI("Error create file '%s'.\n", filename);
        return HI_FAILURE;
    }

    HI_INFO_VI("open dump file[%s] ok\r\n", filename);

    return HI_SUCCESS;
}

HI_S32 yuv_dump_end(HI_VOID)
{
    if (g_pVpDumpFile)
    {
        fclose(g_pVpDumpFile);
        g_pVpDumpFile = NULL;
    }

    return HI_SUCCESS;
}
#if 0
/* this functin shows HOW TO save YUV file form VIDEO_BUFFER_S */
/*only support HI_UNF_VI_STORE_METHOD_SPNYC*/
HI_S32 yuv_dump(const HI_UNF_VI_BUF_S * pVBuf)
{
    HI_S32 s32Ret = HI_FAILURE;
    unsigned int w, h;
    char * pVBufVirt_Y;
    char * pVBufVirt_C;
    char * pMemContent;
    char TmpBuff[1024];

    if (NULL == g_pVpDumpFile)
    {
        return HI_FAILURE;
    }

    memset(TmpBuff, 0, 1024);
    pVBufVirt_Y = (char *)HI_MMAP(pVBuf->u32PhyAddr[0], pVBuf->u32Height * pVBuf->u32Stride[0] );
    pVBufVirt_C = (char *)HI_MMAP(pVBuf->u32PhyAddr[1], pVBuf->u32Height * pVBuf->u32Stride[1] );

    if (!pVBufVirt_Y || !pVBufVirt_C)
    {
        HI_ERR_VI("can NOT map mem for YUV dump.\n");

        if (pVBufVirt_Y)
        {
            if (HI_SUCCESS != HI_MUNMAP(pVBufVirt_Y))
            {
                HI_ERR_VI("VI HI_MUNMAP failed.\n");
                return HI_FAILURE;
            }
        }

        if (pVBufVirt_C)
        {
            if (HI_SUCCESS != HI_MUNMAP(pVBufVirt_C))
            {
                HI_ERR_VI("VI HI_MUNMAP failed.\n");
                return HI_FAILURE;
            }
        }

        return HI_FAILURE;
    }

    //    printf("index=%d, w=%d, h=%d\tYVirtA=0x%08x, CVirtA=0x%08x\r\n",pVBuf->u32FrameIndex,pVBuf->u32Width,pVBuf->u32Height,pVBufVirt_Y,pVBufVirt_C);

    //    printf("saving......Y......");
    for (h = 0; h < pVBuf->u32Height; h++)
    {
        pMemContent = pVBufVirt_Y + h * pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, g_pVpDumpFile);
    }

    s32Ret = fflush(g_pVpDumpFile);

    //    printf("U......");
    for (h = 0; h < pVBuf->u32Height / 2; h++)
    {
        pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];

        pMemContent += 1;

        for (w = 0; w < pVBuf->u32Width / 2; w++)
        {
            TmpBuff[w]   = *pMemContent;
            pMemContent += 2;
        }

        fwrite(TmpBuff, pVBuf->u32Width / 2, 1, g_pVpDumpFile);
    }

    s32Ret = fflush(g_pVpDumpFile);

    //    printf("V......");
    for (h = 0; h < pVBuf->u32Height / 2; h++)
    {
        pMemContent = pVBufVirt_C + h * pVBuf->u32Stride[1];

        for (w = 0; w < pVBuf->u32Width / 2; w++)
        {
            TmpBuff[w]   = *pMemContent;
            pMemContent += 2;
        }

        fwrite(TmpBuff, pVBuf->u32Width / 2, 1, g_pVpDumpFile);
    }

    s32Ret = fflush(g_pVpDumpFile);

    if (HI_SUCCESS != HI_MUNMAP(pVBufVirt_Y))
    {
        HI_ERR_VI("VI HI_MUNMAP failed.\n");
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HI_MUNMAP(pVBufVirt_C))
    {
        HI_ERR_VI("VI HI_MUNMAP failed.\n");
        return HI_FAILURE;
    }

    //    printf("done!\n");
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

//test end
#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
