/*****************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      2009-11-24   z40717     NULL         Create this file.
*
*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_aenc.h"
#include "mpi_aenc.h"
#include "hi_error_mpi.h"

static HI_S32 g_s32MPIAencInitCnt = 0;

#define AENCGetRealChn(hAdec) do {hAenc = hAenc & 0xffff;} while (0)

HI_S32 HI_MPI_AENC_Init(const HI_CHAR* pszCodecNameTable[])
{
    HI_S32 retval;

    if (!g_s32MPIAencInitCnt)
    {
        retval = AENC_Init(pszCodecNameTable);
        if (retval != HI_SUCCESS)
        {
            return retval;
        }
    }

    g_s32MPIAencInitCnt++;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName)
{
    HI_S32 retval;

    retval = AENC_RegisterEncoder(pszCodecDllName);

    return retval;
}

HI_S32 HI_MPI_AENC_ShowRegisterEncoder(HI_VOID)
{
    HI_S32 retval;

    retval = AENC_ShowRegisterEncoder();

    return retval;
}

HI_S32 HI_MPI_AENC_DeInit(HI_VOID)
{
    HI_S32 retval;

    if (!g_s32MPIAencInitCnt)
    {
        return HI_SUCCESS;
    }

    g_s32MPIAencInitCnt--;

    if (!g_s32MPIAencInitCnt)
    {
        retval = AENC_deInit();
        return retval;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AENC_Open(HI_HANDLE *phAenc, const AENC_ATTR_S *pstAencAttr)
{
    HI_S32 retval;

    retval = AENC_Open(phAenc, pstAencAttr);
    if (HI_SUCCESS == retval)
    {
        *phAenc = *phAenc | (HI_ID_AENC << 16);
    }

    return retval;
}

HI_S32 HI_MPI_AENC_Close(HI_HANDLE hAenc)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_Close(hAenc);
    return retval;
}

#if 0
HI_S32 HI_MPI_AENC_Reset(HI_HANDLE hAenc)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_Reset(hAenc);

    return retval;
}

#endif
HI_S32 HI_MPI_AENC_SendBuffer(HI_U32 hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SendBuffer(hAenc, pstAOFrame);
    return retval;
}

HI_S32 HI_MPI_AENC_Pull(HI_HANDLE hAenc)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_Pull(hAenc);
    return retval;
}

HI_S32 HI_MPI_AENC_ReceiveStream(HI_HANDLE hAenc, AENC_STREAM_S *pstStream)
{
    HI_S32 retval;
    HI_U32 u32InBufDataSize = 0;
    HI_U32 u32EncodeInDataSize = 0;

    AENCGetRealChn(hAenc);
    retval = AENC_ReceiveStream(hAenc, pstStream);

    if (HI_ERR_AENC_OUT_BUF_EMPTY == retval)
    {
        u32InBufDataSize = AENC_GetInBufDataSize(hAenc);
        u32EncodeInDataSize = AENC_GetEncodeInDataSize(hAenc);
        HI_INFO_AENC("u32InBufDataSize=%d, u32EncodeInDataSize=%d\n", u32InBufDataSize, u32EncodeInDataSize);
        if (u32InBufDataSize > u32EncodeInDataSize) //if equal, can not exit when AENC_IN_PACKET_SIZE=1024
        {
            retval = HI_ERR_AENC_IN_BUF_UNEMPTY;
        }
    }

    return retval;
}

HI_S32 HI_MPI_AENC_ReleaseStream(HI_HANDLE hAenc, const AENC_STREAM_S *pstStream)
{
    HI_S32 retval;
    HI_U32 u32InBufDataSize = 0;
    HI_U32 u32EncodeInDataSize = 0;

    AENCGetRealChn(hAenc);
    retval = AENC_ReleaseStream (hAenc, pstStream);
    if (HI_SUCCESS == retval)
    {
        u32InBufDataSize = AENC_GetInBufDataSize(hAenc);
        u32EncodeInDataSize = AENC_GetEncodeInDataSize(hAenc);
        if (u32InBufDataSize >= u32EncodeInDataSize)
        {
            AENC_Pull(hAenc);
        }
    }
        
    return retval;
}

HI_S32 HI_MPI_AENC_SetAutoSRC(HI_HANDLE hAenc, HI_BOOL bEnable)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetAutoSRC (hAenc, bEnable);
    return retval;
}

HI_S32 HI_MPI_AENC_SetConfigEncoder(HI_HANDLE hAenc, HI_VOID *pstConfigStructure)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_SetConfigEncoder (hAenc, pstConfigStructure);
    return retval;
}

HI_S32 HI_MPI_AENC_ResetBuf(HI_HANDLE hAenc, HI_U32 u32BufType)
{
    HI_S32 retval;

    AENCGetRealChn(hAenc);
    retval = AENC_ResetBuf(hAenc, u32BufType);
    return retval;
}

HI_S32 HI_MPI_AENC_EncodeFrame( HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAoFrame, AENC_STREAM_S *pstAencFrame)
{
    HI_S32 nRet;

    AENCGetRealChn(hAenc);
    nRet = AENC_SendBuffer(hAenc, pstAoFrame);
    if (HI_SUCCESS != nRet)
    {
        return nRet;
    }

    nRet = AENC_Pull(hAenc);

    nRet = AENC_ReceiveStream (hAenc, pstAencFrame);
    if (HI_SUCCESS == nRet)
    {
        nRet = AENC_ReleaseStream (hAenc, pstAencFrame);
    }
    else
    {
        AENC_ResetBuf(hAenc, 0x3);    /* reset input and output buffer */
    }
    return nRet;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
