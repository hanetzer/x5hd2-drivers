/**
 \file
 \brief aenc inner mpi head
 \copyright Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version draft
 \author QuYaxin 46153
 \date 2010-1-29
 */

#ifndef __MPI_PRIV_AENC_H__
#define __MPI_PRIV_AENC_H__

/* add include here */
#include "mpi_aenc.h"
#include "hi_unf_sound.h"
#include "hi_module.h"
#include "hi_debug.h"
#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define AENC_INSTANCE_MAXNUM 3     /* max encoder instance */

#define AENC_MIN_INPUT_BUFFER_SIZE (1024 * 256)
#define AENC_MAX_INPUT_BUFFER_SIZE (1024 * 512 * 4)
#define AENC_DEFAULT_INPUT_BUFFER_SIZE (1024 * 512)
#define AENC_DEFAULT_OUTBUF_NUM 32

#define AENC_MAX_SRC_FRAC (48000 / 8000)
#define AENC_MAX_CHANNELS 2
#define ANEC_MAX_SMAPLEPERFRAME (2048)
#define AENC_MAX_POSTPROCESS_FRAME (ANEC_MAX_SMAPLEPERFRAME * AENC_MAX_CHANNELS * AENC_MAX_SRC_FRAC)

typedef struct hi_AENC_INFO_ATTACH_S
{
	HI_HANDLE hAenc;
	HI_HANDLE hTrack;
	HI_BOOL   bStart;
	//AENC_ATTR_S stAencAttr;
}AENC_INFO_ATTACH_S;

typedef struct hiAENC_PROC_ITEM_S
{
    HI_BOOL bAdecWorkEnable;
    HI_U32  u32CodecID;
    HI_CHAR        szCodecType[32];
    HI_U32  u32SampleRate;
    HI_U32  u32BitWidth;
    HI_U32  u32Channels;

    HI_BOOL bAutoSRC;
    HI_U32  u32EncFrame;
    HI_U32  u32ErrFrame;

    HI_U32 u32InBufSize;
    HI_U32 u32InBufRead;
    HI_U32 u32InBufWrite;

    HI_U32 u32OutFrameNum;
    HI_U32 u32OutFrameRIdx;
    HI_U32 u32OutFrameWIdx;
    HI_U32 u32DbgSendBufCount_Try;
    HI_U32 u32DbgSendBufCount;
    HI_U32 u32DbgReceiveStreamCount_Try;
    HI_U32 u32DbgReceiveStreamCount;
    HI_U32 u32DbgTryEncodeCount;
} AENC_PROC_ITEM_S;

/* use macro to check parameter */
#define  HI_MPI_AENC_RetUserErr(DrvErrCode, aenc_mutex) \
    do                                                  \
    {                                                   \
        HI_S32 retvalerr;                               \
        if (HI_SUCCESS != DrvErrCode)                   \
        {                                               \
            switch (DrvErrCode)                         \
            {                                           \
            case  HI_ERR_AENC_IN_BUF_FULL:           \
            case  HI_ERR_AENC_DEV_NOT_OPEN:          \
            case  HI_ERR_AENC_NULL_PTR:          	 \
            case  HI_ERR_AENC_INVALID_PARA:          \
            case  HI_ERR_AENC_OUT_BUF_FULL:          \
            case  HI_ERR_AENC_INVALID_OUTFRAME:      \
            case  HI_ERR_AENC_DATASIZE_EXCEED:       \
            case  HI_ERR_AENC_OUT_BUF_EMPTY:         \
                retvalerr = DrvErrCode;              \
                break;                               \
            default:                                 \
                retvalerr = HI_FAILURE;              \
                break;                               \
            }                                        \
            if (HI_ERR_AENC_IN_BUF_FULL == retvalerr)          \
                HI_INFO_AENC(" DriverErrorCode =0x%x\n",retvalerr); \
            else if (HI_ERR_AENC_OUT_BUF_EMPTY == retvalerr)   \
                HI_INFO_AENC(" DriverErrorCode =0x%x\n",retvalerr); \
            else                                               \
                HI_ERR_AENC(" DriverErrorCode =0x%x\n",retvalerr);  \
            AENC_UNLOCK(aenc_mutex); \
            return retvalerr;                        \
        }                                            \
    } while (0)

#define  HI_MPI_AENC_RetUserErr2(DrvErrCode, aenc_mutex) \
    do                                                   \
    {                                                    \
   	    HI_ERR_AENC(" DriverErrorCode =0x%x\n",DrvErrCode); \
        AENC_UNLOCK(aenc_mutex); \
        return DrvErrCode; \
    } while (0)			

#define CHECK_AENC_CH_CREATE(hAenc) \
    do                                                  \
    {                                                   \
        if (!g_s32AencInitCnt)  \
        {  \
            HI_ERR_AENC("AENC  device state err: please int aenc init first\n");  \
            return HI_FAILURE;  \
        }  \
        if (hAenc >= AENC_INSTANCE_MAXNUM)             \
        {                                               \
            HI_ERR_AENC(" AENC  device not open handleAenc=%d !\n",  hAenc);          \
            return HI_ERR_AENC_DEV_NOT_OPEN;            \
        }                                                \
        if (HI_FALSE == g_pstAencChan[hAenc]->beAssigned)  \
        {                                               \
            HI_ERR_AENC("AENC  device not open!\n");          \
            return HI_ERR_AENC_DEV_NOT_OPEN;              \
        }                                               \
    } while (0)

#define CHECK_AENC_NULL_PTR(ptr) \
    do                                                  \
    {                                                   \
        if (NULL == ptr)                             \
        {                                               \
            HI_ERR_AENC("invalid NULL poiner!\n");          \
            return HI_FAILURE;                  \
        }                                               \
    } while (0)

#define CHECK_AENC_OPEN_FORMAT(ch, width, bInterleaved) \
    do                                                  \
    {                                                   \
        if (HI_FALSE == bInterleaved)    \
        {                                           \
            HI_ERR_AENC("invalid  Pcm Format: HA Encoder only support 16bit-Interleaved format \n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
        if (16 != width)    \
        {                                           \
            HI_ERR_AENC("invalid  Pcm Format: HA Encoder only support 16bit-Interleaved format \n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
        if (ch > 2)                  \
        {                                           \
            if ((ch != 6) && (ch != 8))                  \
            {                                           \
                HI_ERR_AENC("invalid Pcm Format: HA Encoder  only support 5.1 or 7.1 format channel=%d\n", ch);   \
                return HI_ERR_AENC_INVALID_PARA;           \
            }                                           \
        }                                           \
    } while (0)

#define CHECK_AENC_PCM_SAMPLESIZE(PcmSamplesPerFrame) \
    do                                                  \
    {                                                   \
        if (PcmSamplesPerFrame > ANEC_MAX_SMAPLEPERFRAME)    \
        {                                           \
            HI_ERR_AENC("invalid  AO Pcm Format: Pcm SamplesPerFrame  =%d \n",PcmSamplesPerFrame);   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
    } while (0)

#define CHECK_AENC_PCM_FORMAT(ch, bInterleaved) \
    do                                                  \
    {                                                   \
        if (HI_FALSE == bInterleaved)    \
        {                                           \
            if (ch > 2)                  \
            {                                           \
                HI_ERR_AENC("invalid  Pcm Format: if none-Interleaved, must sure channel <=2 ! \n");   \
                return HI_ERR_AENC_INVALID_PARA;           \
            }                                           \
        }                                           \
        if (ch > 2)                  \
        {                                           \
            if ((ch != 6) && (ch != 8))                  \
            {                                           \
                HI_ERR_AENC("invalid Pcm Format: HA Encoder  only support 5.1 or 7.1 format channel=%d\n", ch);   \
                return HI_ERR_AENC_INVALID_PARA;           \
            }                                           \
        }                                           \
    } while (0)

/*Define Debug Level For HI_ID_AO                     */
#define HI_FATAL_AENC(fmt...) \
    HI_FATAL_PRINT(HI_ID_AENC, fmt)

#define HI_ERR_AENC(fmt...) \
    HI_ERR_PRINT(HI_ID_AENC, fmt)

#define HI_WARN_AENC(fmt...) \
    HI_WARN_PRINT(HI_ID_AENC, fmt)

#define HI_INFO_AENC(fmt...) \
    HI_INFO_PRINT(HI_ID_AENC, fmt)

/********************** Global Variable declaration **************************/
#define DRV_AENC_DEVICE_NAME "hi_aenc"

/* 'IOC_TYPE_ADEC' means ADEC magic macro */
#define     DRV_AENC_PROC_INIT _IOW(HI_ID_AENC, 0, AENC_PROC_ITEM_S *)
#define     DRV_AENC_PROC_EXIT _IO(HI_ID_AENC, 1)

/******************************* API declaration *****************************/
extern HI_S32 AENC_Close (HI_U32 hAenc);
extern HI_S32 AENC_deInit(HI_VOID);
extern HI_S32 AENC_Init(const HI_CHAR* pszodecNameTable[]);
extern HI_S32 AENC_Open(HI_HANDLE *phAenc, const AENC_ATTR_S *pstAencAttr);
extern HI_S32 AENC_Pull(HI_HANDLE hAenc);
extern HI_S32 AENC_ReceiveStream (HI_HANDLE hAenc, AENC_STREAM_S *pstStream);
extern HI_S32 AENC_ReleaseStream(HI_HANDLE hAenc, const AENC_STREAM_S *pstStream);
extern HI_S32 AENC_Reset(HI_HANDLE hAenc);
extern HI_S32 AENC_SendBuffer (HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
extern HI_S32 AENC_SetAutoSRC (HI_HANDLE hAenc, HI_BOOL bEnable);
extern HI_S32 AENC_SetConfigEncoder(HI_HANDLE hAenc, HI_VOID *pstConfigStructure);
extern HI_S32 AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName);
extern HI_S32 AENC_ShowRegisterEncoder(HI_VOID);
extern HI_S32 AENC_ResetBuf(HI_HANDLE hAenc, HI_U32 u32BufType);
extern HI_U32 AENC_GetInBufDataSize(HI_HANDLE hAenc);
extern HI_U32 AENC_GetEncodeInDataSize(HI_HANDLE hAenc);

#ifdef __cplusplus
}
#endif
#endif /* __MPI_PRIV_AENC_H__ */
