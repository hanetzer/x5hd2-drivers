/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_vi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/27
  Description   :
  History       :
  1.Date        : 2009/03/27
    Author      : j00131665
    Modification: Created file
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_mpi_vi.h"

HI_S32 HI_UNF_VI_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Init();

    return s32Ret;
}

HI_S32 HI_UNF_VI_DeInit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_DeInit();

    return s32Ret;
}

HI_S32 HI_UNF_VI_SetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_SetAttr(handle, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VI_GetAttr(HI_HANDLE handle, HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_GetAttr(handle, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VI_GetDefaultAttr(HI_UNF_VI_ATTR_S *pstAttr)
{
    if (!pstAttr)
    {
        return HI_ERR_VI_NULL_PTR;
    }

//    pstAttr->enCapSel = HI_UNF_VI_CAPSEL_EVEN;
    pstAttr->stInputRect.s32X = 0;
    pstAttr->stInputRect.s32Y = 0;
    pstAttr->stInputRect.s32Width  = 720;
    pstAttr->stInputRect.s32Height = 576;

    /*pstAttr->enDataWidth=HI_UNF_VI_DATA_WIDTH8;*/
    pstAttr->enInputMode = HI_UNF_VI_MODE_BT656_576I;
//    pstAttr->enViPort = HI_UNF_VI_PORT1;
//    pstAttr->enChnYC = HI_UNF_VI_CHN_YC_SEL_Y;
//    pstAttr->enStoreMethod = HI_UNF_VI_STORE_METHOD_SPNYC;
    pstAttr->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
//    pstAttr->enStoreMode = HI_UNF_VI_STORE_FIELD;
//	pstAttr->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
//    pstAttr->u32YStride = HI_UNF_VI_INVALID_PARA_U32;
//    pstAttr->u32CStride = HI_UNF_VI_INVALID_PARA_U32;
    pstAttr->u32BufNum = 6;
    pstAttr->enBufMgmtMode = HI_UNF_VI_BUF_ALLOC;
//    pstAttr->u32ViBufAddr = 0;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VI_Create(HI_UNF_VI_E enViPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phandle)
//HI_S32 HI_UNF_VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phandle)
{
    HI_S32 s32Ret;

//    s32Ret = HI_MPI_VI_Create(pstAttr, phandle);
	s32Ret = HI_MPI_VI_Create(enViPort, pstAttr, phandle);

    return s32Ret;
}

HI_S32 HI_UNF_VI_Destroy(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Destroy(handle);

    return s32Ret;
}

HI_S32 HI_UNF_VI_Start(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Start(handle);

    return s32Ret;

}
HI_S32 HI_UNF_VI_Stop(HI_HANDLE handle)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_Stop(handle);

    return s32Ret;

}
HI_S32 HI_UNF_VI_SetExternBuffer(HI_HANDLE handle, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_SetExternBuffer(handle, pstBufAttr);

    return s32Ret;
}
#if 0
HI_S32 HI_UNF_VI_GetFrame(HI_HANDLE handle, HI_UNF_VI_BUF_S *pViBuf)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_GetFrame(handle, pViBuf);

    return s32Ret;
}

HI_S32 HI_UNF_VI_PutFrame(HI_HANDLE handle, const HI_UNF_VI_BUF_S *pViBuf)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_PutFrame(handle, pViBuf);

    return s32Ret;
}
#else
HI_S32 HI_UNF_VI_DequeueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_DequeueFrame(hVI, pFrameInfo);

    return s32Ret;
}

HI_S32 HI_UNF_VI_QueueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_QueueFrame(hVI, pFrameInfo);

    return s32Ret;
}
#endif
HI_S32 HI_UNF_VI_AcquireFrame(HI_HANDLE handle, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_AcquireFrame(handle, 3, pFrameInfo, u32TimeoutMs);

    return s32Ret;
}

HI_S32 HI_UNF_VI_ReleaseFrame(HI_HANDLE handle, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_ReleaseFrame(handle, 3, pFrameInfo);

    return s32Ret;
}
#if 0
HI_S32 HI_UNF_VI_YUV_DUMP_START(HI_CHAR *filename)
{
    return yuv_dump_start(filename);
}

HI_S32 HI_UNF_VI_YUV_DUMP(const HI_UNF_VI_BUF_S *pVBuf)
{
    return yuv_dump(pVBuf);
}

HI_S32 HI_UNF_VI_YUV_DUMP_END(HI_VOID)
{
    return yuv_dump_end();
}
#endif
