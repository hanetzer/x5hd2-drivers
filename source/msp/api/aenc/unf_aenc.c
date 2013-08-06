/**
\file
\brief unf of sound 
\copyright Shenzhen Hisilicon Co., Ltd.
\date 
\version
\author 
\date 
*/
//#include "hi_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_error_mpi.h"
#include "hi_unf_aenc.h"
#include "mpi_aenc.h"
#include "hi_drv_aenc.h"
#include "hi_mpi_avplay.h"

#define AENC_INSTANCE_MASK 0xffff

#define API_AENC_CheckHandle(hAenc) \
    do{ \
        if ((HI_ID_AENC != (hAenc>>16)) || ((hAenc&AENC_INSTANCE_MASK) >= AENC_INSTANCE_MAXNUM)) \
        { \
            HI_ERR_AENC("invalid Aenc handle(%d).\n", hAenc); \
            return HI_ERR_AENC_INVALID_PARA; \
        } \
    }while(0)

#define API_AENC_CheckStart(i) \
    do                                                  \
    {                                                   \
        if (!g_stAencInfo[i].bStart)                                  \
        {                                               \
            HI_ERR_AENC("hAenc(%d) not start.\n", g_stAencInfo[i].hAenc); \
            return HI_ERR_AENC_CH_NOT_SUPPORT;              \
        }                                               \
    } while (0)

static AENC_INFO_ATTACH_S g_stAencInfo[AENC_INSTANCE_MAXNUM];

HI_S32 HI_UNF_AENC_Init(HI_VOID)
{  
    HI_U32 i;
    
    for (i=0; i<AENC_INSTANCE_MAXNUM; i++)
    {
        g_stAencInfo[i].hAenc = HI_INVALID_HANDLE;
    }
    
    return HI_MPI_AENC_Init(NULL);
}

HI_S32 HI_UNF_AENC_DeInit(HI_VOID)
{
    HI_U32 i;
    
    for (i=0; i<AENC_INSTANCE_MAXNUM; i++)
    {
        if (HI_INVALID_HANDLE != g_stAencInfo[i].hAenc)
        {
            HI_ERR_AENC("please destroy hAenc(%d) first.\n", g_stAencInfo[i].hAenc);
            return HI_ERR_AENC_CH_NOT_SUPPORT;
        }
    }
    
    return HI_MPI_AENC_DeInit();
}

HI_S32 HI_UNF_AENC_Create(const HI_UNF_AENC_ATTR_S *pstAencAttr, HI_HANDLE *phAenc)
{
    AENC_ATTR_S stAencAttr;
    HI_U32 i;
    HI_S32 ret;

    if (NULL == pstAencAttr)
    {
        return HI_ERR_AENC_NULL_PTR;
    }

    stAencAttr.u32CodecID    = pstAencAttr->enAencType;
    stAencAttr.sOpenParam    = pstAencAttr->sOpenParam;
    stAencAttr.u32InBufSize = AENC_DEFAULT_INPUT_BUFFER_SIZE;
    stAencAttr.u32OutBufNum = AENC_DEFAULT_OUTBUF_NUM;
    ret = HI_MPI_AENC_Open(phAenc, &stAencAttr);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_AENC("call HI_MPI_AENC_Open failed:%#x.\n", ret);           
        return ret;
    }

    HI_INFO_AENC("*phAenc=%#x\n", *phAenc);

    i = *phAenc & AENC_INSTANCE_MASK;
    g_stAencInfo[i].hAenc = *phAenc;
    g_stAencInfo[i].hTrack = HI_INVALID_HANDLE;
    g_stAencInfo[i].bStart = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_AENC_Destroy(HI_HANDLE hAenc)
{
    HI_U32 i;
    HI_S32 ret;
    
    API_AENC_CheckHandle(hAenc);
    
    i = hAenc & AENC_INSTANCE_MASK;
    if (g_stAencInfo[i].bStart)
    {
        HI_ERR_AENC("please stop aenc first.\n");
        return HI_ERR_AENC_CH_NOT_SUPPORT;
    }
    if (HI_INVALID_HANDLE != g_stAencInfo[i].hTrack) //first detach aenc
    {
        HI_ERR_AENC("please detach aenc first.\n");
        return HI_ERR_AENC_CH_NOT_SUPPORT;
    }
    
    ret = HI_MPI_AENC_Close(hAenc);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_AENC("call HI_UNF_AENC_Destroy failed:%#x.\n", ret);           
        return ret;
    }
    
    g_stAencInfo[i].hAenc = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_AENC_AttachInput(HI_HANDLE hAenc, HI_HANDLE hTrack)
{
    HI_ERR_AENC("Now Don't Support HI_UNF_AENC_AttachInput!\n");
    return HI_FAILURE;
}

HI_S32 HI_UNF_AENC_DetachInput(HI_HANDLE hAenc, HI_HANDLE hTrack)
{
    HI_ERR_AENC("Now Don't Support HI_UNF_AENC_DetachInput!\n");
    return HI_FAILURE;
}

HI_S32 HI_UNF_AENC_Start(HI_HANDLE hAenc)
{
    HI_U32 i;

    API_AENC_CheckHandle(hAenc);

    i = hAenc & AENC_INSTANCE_MASK;
    g_stAencInfo[i].bStart = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_AENC_Stop(HI_HANDLE hAenc)
{
    HI_U32 u32BufType = 0x0;
    HI_U32 i;
    HI_S32 ret;

    API_AENC_CheckHandle(hAenc);

    i = hAenc & AENC_INSTANCE_MASK;
    if (!g_stAencInfo[i].bStart)
    {
        return HI_SUCCESS;
    }
    
    g_stAencInfo[i].bStart = HI_FALSE;
    
    ret = HI_MPI_AENC_ResetBuf(hAenc, u32BufType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_AENC("call HI_MPI_AENC_ResetBuf failed:%#x.\n", ret);            
        return ret;
    }

    return HI_SUCCESS;
}


HI_S32 HI_UNF_AENC_SendFrame(HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame)
{
    HI_U32 i;
    
    API_AENC_CheckHandle(hAenc);

    i = hAenc & AENC_INSTANCE_MASK;
    API_AENC_CheckStart(i);
    return HI_MPI_AENC_SendBuffer(hAenc, pstAOFrame);
}

HI_S32 HI_UNF_AENC_AcquireStream(HI_HANDLE hAenc, HI_UNF_ES_BUF_S *pstStream, HI_U32 u32TimeoutMs)
{
    HI_U32 i;
    
    API_AENC_CheckHandle(hAenc);

    i = hAenc & AENC_INSTANCE_MASK;
    if (!g_stAencInfo[i].bStart)
    {
        HI_INFO_AENC("hAenc(%d) not start.\n", g_stAencInfo[i].hAenc);
        return HI_ERR_AENC_CH_NOT_SUPPORT;
    }
    return HI_MPI_AENC_ReceiveStream(hAenc, (AENC_STREAM_S *)pstStream);
}

HI_S32 HI_UNF_AENC_ReleaseStream(HI_HANDLE hAenc, const HI_UNF_ES_BUF_S *pstStream)
{
    HI_U32 i;
    
    API_AENC_CheckHandle(hAenc);

    i = hAenc & AENC_INSTANCE_MASK;
    API_AENC_CheckStart(i);
    return HI_MPI_AENC_ReleaseStream(hAenc, (AENC_STREAM_S *)pstStream);
}

HI_S32 HI_UNF_AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName)
{
    return HI_MPI_AENC_RegisterEncoder(pszCodecDllName);
}



