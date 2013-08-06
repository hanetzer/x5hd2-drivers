/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_ao_op_func.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 ********************************************************************************/
#include <asm/setup.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_stat_ext.h"

#include "hi_module.h"
#include "drv_mmz_ext.h"
#include "drv_stat_ext.h"
#include "drv_sys_ext.h"
#include "drv_proc_ext.h"
#include "drv_module_ext.h"
#include "drv_mem_ext.h"
#include "hi_error_mpi.h"

#include "hi_drv_hdmi.h"
#include "drv_hdmi_ext.h"

#include "audio_util.h"
#include "drv_ao_op.h"
#include "drv_ao_track.h"
#include "hal_aoe_func.h"
#include "hal_aiao_func.h"
#include "hal_aiao_common.h"
#include "hal_aoe_common.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"
#include "hal_tianlai_adac.h"


#if !defined(HI_SND_DRV_TEMPLATE_OPT)
static HI_U32 g_u32SndTrackRS = 0;
HI_U32   TRACK_RS_GetFreeId(HI_VOID)
{
    HI_U32 TrackId;

    for (TrackId = 0; TrackId < AO_MAX_TOTAL_TRACK_NUM; TrackId++)
    {
        if (!(g_u32SndTrackRS & ((HI_U32)1L << TrackId)))
        {
            return TrackId;
        }
    }

    return AO_MAX_TOTAL_TRACK_NUM;
}

HI_VOID   TRACK_RS_Init(HI_VOID)
{
    g_u32SndTrackRS = 0;
}

HI_VOID   TRACK_RS_DeInit(HI_VOID)
{
    if (g_u32SndTrackRS)
    {
        HI_ERR_AO("g_u32SndTrackRS (%d) should be zero n", g_u32SndTrackRS);
    }

    g_u32SndTrackRS = 0;
}

HI_VOID   TRACK_RS_RegisterId(HI_U32 TrackId)
{
    g_u32SndTrackRS |= ((HI_U32)1L << TrackId);
}

HI_VOID   TRACK_RS_DeRegisterId(HI_U32 TrackId)
{
    g_u32SndTrackRS &= ~((HI_U32)1L << TrackId);
}
#endif

/******************************Track Engine process FUNC*************************************/

static HI_VOID TrackDestroyEngine(HI_HANDLE hSndEngine)
{
    SND_ENGINE_STATE_S *state = (SND_ENGINE_STATE_S *)hSndEngine;

    if (!state)
    {
        return;
    }

    HAL_AOE_ENGINE_Stop(state->enEngine);
    HAL_AOE_ENGINE_Destroy(state->enEngine);
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
}

HI_VOID TRACK_DestroyEngine(SND_CARD_STATE_S *pCard)
{
    HI_U32 Id;

    for (Id = 0; Id < SND_ENGINE_TYPE_BUTT; Id++)
    {
        if (pCard->hSndEngine[Id])
        {
            TrackDestroyEngine(pCard->hSndEngine[Id]);
            pCard->hSndEngine[Id] = HI_NULL;
        }
    }
}


HI_S32 TrackCreateEngine(HI_HANDLE *phSndEngine, AOE_ENGINE_CHN_ATTR_S *pstAttr, SND_ENGINE_TYPE_E enType)
{
    SND_ENGINE_STATE_S *state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    AOE_ENGINE_ID_E enEngine;

    *phSndEngine = HI_NULL;

    state = HI_KMALLOC(HI_ID_AO, sizeof(SND_ENGINE_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AIAO("HI_KMALLOC CreateEngine failed\n");
        goto CreateEngine_ERR_EXIT;
    }

    memset(state, 0, sizeof(SND_ENGINE_STATE_S));

    if (HI_SUCCESS != HAL_AOE_ENGINE_Create(&enEngine, pstAttr))
    {
        HI_ERR_AO("Create engine failed!\n");
        goto CreateEngine_ERR_EXIT;
    }

    state->stUserEngineAttr = *pstAttr;
    state->enEngine = enEngine;
    state->enEngineType = enType;
    *phSndEngine = (HI_HANDLE)state;

    return HI_SUCCESS;

CreateEngine_ERR_EXIT:
    *phSndEngine = (HI_HANDLE)HI_NULL;
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}



SND_ENGINE_TYPE_E  TrackGetEngineType(HI_HANDLE hSndEngine)
{
    SND_ENGINE_STATE_S *state = (SND_ENGINE_STATE_S *)hSndEngine;

    return (state->enEngineType);
}

HI_HANDLE  TrackGetEngineHandlebyType(SND_CARD_STATE_S *pCard, SND_ENGINE_TYPE_E enType)
{
    HI_HANDLE hSndEngine;

    hSndEngine = pCard->hSndEngine[enType];
    if (hSndEngine)
    {
        return hSndEngine;
    }

    return HI_NULL;
}


/******************************Track process FUNC*************************************/
HI_U32 TrackGetPcmSize(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    HI_U32 channels=pstAOFrame->u32Channels;

    if(pstAOFrame->u32Channels > AO_TRACK_NORMAL_CHANNELNUM)
    {
        channels = AO_TRACK_NORMAL_CHANNELNUM;
    }
    return pstAOFrame->u32PcmSamplesPerFrame*AUTIL_CalcFrameSize(channels, pstAOFrame->s32BitPerSample);
}

HI_U32 TrackGetMultiPcmSize(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    if(pstAOFrame->u32Channels > AO_TRACK_NORMAL_CHANNELNUM)
    {
        /* allways 8 ch */
        return pstAOFrame->u32PcmSamplesPerFrame*AUTIL_CalcFrameSize(AO_TRACK_MUTILPCM_CHANNELNUM, pstAOFrame->s32BitPerSample);
    }
    else
    {
        return 0;
    }
}


HI_U32 TrackGetLbrSize(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    HI_U32 LbrRawBytes = 0;

    LbrRawBytes = (pstAOFrame->u32BitsBytesPerFrame & 0xffff);
    return LbrRawBytes;
}

HI_U32 TrackGetHbrSize(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    HI_U32 HbrRawBytes = 0;

    if (pstAOFrame->u32BitsBytesPerFrame & 0xffff0000)
    {
        HbrRawBytes = (pstAOFrame->u32BitsBytesPerFrame >> 16);
    }
    else
    {
        HbrRawBytes = (pstAOFrame->u32BitsBytesPerFrame & 0xffff);
    }

    return HbrRawBytes;
}

HI_U32 TrackGetPcmChannels(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    if(AO_TRACK_NORMAL_CHANNELNUM == pstAOFrame->u32Channels|| 1 == pstAOFrame->u32Channels)
    {
        return (HI_U32)pstAOFrame->u32Channels;
    }
    else if(pstAOFrame->u32Channels> AO_TRACK_NORMAL_CHANNELNUM)
    {
        return (HI_U32)(AO_TRACK_NORMAL_CHANNELNUM);
    }
    else
    {
        return 0;
    }
}

HI_U32 TrackGetMultiPcmChannels(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    if(pstAOFrame->u32Channels<=AO_TRACK_NORMAL_CHANNELNUM)
    {
        return 0;
    }

    return pstAOFrame->u32Channels-AO_TRACK_NORMAL_CHANNELNUM;
}

HI_U32 TrackGetPcmBufAddr(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    return (HI_U32)pstAOFrame->ps32PcmBuffer;
}
/*
  |----Interleaved dmx 2.0 frame----|--Interleaved multi 7.1 frame--|
  |----Interleaved 7.1-----------------------------|
  |----Interleaved 5.1----------------- padding 0/0|
  
*/
HI_U32 TrackGetMultiPcmAddr(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    HI_U32 u32base = (HI_U32)pstAOFrame->ps32PcmBuffer;
    if(pstAOFrame->u32Channels <= AO_TRACK_NORMAL_CHANNELNUM)
    {
        return (HI_U32)0;
    }
    else
    {
        /* dmx allways 2 ch */
        return u32base + pstAOFrame->u32PcmSamplesPerFrame*AUTIL_CalcFrameSize(AO_TRACK_NORMAL_CHANNELNUM, pstAOFrame->s32BitPerSample);
    }    
    return 0;
}

HI_U32 TrackGetLbrBufAddr(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    if (pstAOFrame->u32BitsBytesPerFrame & 0xffff)
    {
        return (HI_U32)pstAOFrame->ps32BitsBuffer;
    }

    return 0;
}

HI_U32 TrackGetHbrBufAddr(HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    HI_U32 Addr;

    Addr = (HI_U32)pstAOFrame->ps32BitsBuffer;
    if (pstAOFrame->u32BitsBytesPerFrame & 0xffff0000)
    {
        Addr += (pstAOFrame->u32BitsBytesPerFrame & 0xffff);
    }

    return Addr;
}

//for both passthrough-only(no pcm output) and simul mode
HI_VOID TrackBuildPcmAttr(HI_UNF_AO_FRAMEINFO_S *pstAOFrame, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    if(pstAOFrame->u32PcmSamplesPerFrame)
    {
        pstStreamAttr->u32PcmSampleRate = pstAOFrame->u32SampleRate;
        pstStreamAttr->u32PcmBitDepth = pstAOFrame->s32BitPerSample;
        pstStreamAttr->u32PcmSamplesPerFrame = pstAOFrame->u32PcmSamplesPerFrame;
        pstStreamAttr->u32PcmBytesPerFrame = TrackGetPcmSize(pstAOFrame);
        pstStreamAttr->u32PcmChannels = TrackGetPcmChannels(pstAOFrame);
        pstStreamAttr->pPcmDataBuf = (HI_VOID*)TrackGetPcmBufAddr(pstAOFrame);
    }
    else
    {
        HI_U32 u32BitWidth;
        if(16 == pstAOFrame->s32BitPerSample)
        {
            u32BitWidth = sizeof(HI_U16);
        }
        else
        {
            u32BitWidth = sizeof(HI_U32);
        }
        if(pstStreamAttr->pLbrDataBuf)
        {
            pstStreamAttr->u32PcmSampleRate = pstAOFrame->u32SampleRate;
            pstStreamAttr->u32PcmBitDepth = pstAOFrame->s32BitPerSample;
            pstStreamAttr->u32PcmBytesPerFrame = pstStreamAttr->u32LbrBytesPerFrame;
            pstStreamAttr->u32PcmChannels = AO_TRACK_NORMAL_CHANNELNUM;
            pstStreamAttr->u32PcmSamplesPerFrame = pstStreamAttr->u32PcmBytesPerFrame/pstStreamAttr->u32PcmChannels/u32BitWidth;
            pstStreamAttr->pPcmDataBuf = (HI_VOID*)HI_NULL;
        }
        else if(pstStreamAttr->pHbrDataBuf)
        {
            HI_U32 u32HbrSamplesPerFrame = pstStreamAttr->u32HbrBytesPerFrame/pstStreamAttr->u32HbrChannels/u32BitWidth;
            pstStreamAttr->u32PcmSamplesPerFrame = u32HbrSamplesPerFrame >> 2;
            pstStreamAttr->u32PcmSampleRate = pstAOFrame->u32SampleRate;
            pstStreamAttr->u32PcmBitDepth = pstAOFrame->s32BitPerSample;
            pstStreamAttr->u32PcmChannels = AO_TRACK_NORMAL_CHANNELNUM;
            pstStreamAttr->u32PcmBytesPerFrame = pstStreamAttr->u32PcmSamplesPerFrame*pstStreamAttr->u32PcmChannels*u32BitWidth;
            pstStreamAttr->pPcmDataBuf = (HI_VOID*)HI_NULL;
        }
    }
}

static HI_VOID TRACKDbgCountTrySendData(SND_TRACK_STATE_S *pTrack)
{
    pTrack->u32SendTryCnt++;
}

static HI_VOID TRACKDbgCountSendData(SND_TRACK_STATE_S *pTrack)
{
    pTrack->u32SendCnt++;
}

HI_VOID TRACKBuildStreamAttr(SND_CARD_STATE_S *pCard, HI_UNF_AO_FRAMEINFO_S *pstAOFrame, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    HI_U32 u32IEC61937DataType;
    HI_U32 u32Status = HI_FALSE;
    HI_S32 s32Ret = HI_FAILURE;
    memset(pstStreamAttr, 0, sizeof(SND_TRACK_STREAM_ATTR_S));

    // lbr
    u32IEC61937DataType = AUTIL_IEC61937DataType((HI_U16*)TrackGetLbrBufAddr(pstAOFrame), TrackGetLbrSize(pstAOFrame));
    pstStreamAttr->u32LbrFormat = IEC61937_DATATYPE_NULL;
    if (u32IEC61937DataType && !(AUTIL_isIEC61937Hbr(u32IEC61937DataType, pstAOFrame->u32SampleRate)))
    {
        pstStreamAttr->u32LbrSampleRate = pstAOFrame->u32SampleRate;
        pstStreamAttr->u32LbrBitDepth = AO_TRACK_BITDEPTH_LOW;
        pstStreamAttr->u32LbrChannels = AO_TRACK_NORMAL_CHANNELNUM;
        pstStreamAttr->u32LbrFormat = u32IEC61937DataType;
        pstStreamAttr->u32LbrBytesPerFrame = TrackGetLbrSize(pstAOFrame);
        pstStreamAttr->pLbrDataBuf = (HI_VOID*)TrackGetLbrBufAddr(pstAOFrame);
    }

    // hbr
    if(pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiGetPlayStus)
    {
        (pCard->pstHdmiFunc->pfnHdmiGetPlayStus)(HI_UNF_HDMI_ID_0, &u32Status);
    }
    if (u32Status == HI_TRUE)
    {
        u32IEC61937DataType = AUTIL_IEC61937DataType((HI_U16*)TrackGetHbrBufAddr(pstAOFrame), TrackGetHbrSize(pstAOFrame));
        pstStreamAttr->u32HbrFormat = IEC61937_DATATYPE_NULL;
        if ((AUTIL_isIEC61937Hbr(u32IEC61937DataType, pstAOFrame->u32SampleRate)))
        {
            pstStreamAttr->u32HbrBitDepth = AO_TRACK_BITDEPTH_LOW;
            pstStreamAttr->u32HbrChannels = ((IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS==u32IEC61937DataType)?2:8);
            pstStreamAttr->u32HbrFormat = u32IEC61937DataType;
            pstStreamAttr->u32HbrSampleRate = pstAOFrame->u32SampleRate * 4;  /* hbr 4*samplerate */
            pstStreamAttr->u32HbrBytesPerFrame = TrackGetHbrSize(pstAOFrame);
            pstStreamAttr->pHbrDataBuf = (HI_VOID*)TrackGetHbrBufAddr(pstAOFrame);
        }
        else if(TrackGetMultiPcmChannels(pstAOFrame))
        {
            HI_UNF_HDMI_SINK_CAPABILITY_S stSinkCap;
            /*get the capability of the max pcm channels of the output device*/
            if(pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiGetSinkCapability)
            {
                s32Ret = (pCard->pstHdmiFunc->pfnHdmiGetSinkCapability)(HI_UNF_HDMI_ID_0, &stSinkCap);
            }
            if(HI_SUCCESS == s32Ret)
            {
                if (stSinkCap.u32MaxPcmChannels > AO_TRACK_NORMAL_CHANNELNUM)
                {
                    pstStreamAttr->u32HbrBitDepth = pstAOFrame->s32BitPerSample;
                    pstStreamAttr->u32HbrSampleRate = pstAOFrame->u32SampleRate;  
                    pstStreamAttr->u32HbrFormat = IEC61937_DATATYPE_71_LPCM;
                    pstStreamAttr->u32HbrChannels = AO_TRACK_MUTILPCM_CHANNELNUM; 
                    pstStreamAttr->u32OrgMultiPcmChannels = TrackGetMultiPcmChannels(pstAOFrame); 
                    pstStreamAttr->u32HbrBytesPerFrame = TrackGetMultiPcmSize(pstAOFrame); 
                    pstStreamAttr->pHbrDataBuf = (HI_VOID*)TrackGetMultiPcmAddr(pstAOFrame); 
                }
            }
        }
    }

    // pcm
    TrackBuildPcmAttr(pstAOFrame,pstStreamAttr);
}

static SND_HDMI_MODE_E TRACKHdmiEdidChange(HI_UNF_HDMI_SINK_CAPABILITY_S *pstSinkCap, HI_U32 u32Format)
{
    switch(u32Format)
    {
        case IEC61937_DATATYPE_NULL:
            return SND_HDMI_MODE_PCM;
            
        case IEC61937_DATATYPE_DOLBY_DIGITAL:
            if(HI_TRUE == pstSinkCap->bAudioFmtSupported[AO_HDMI_CAPABILITY_AC3])
            {
                return SND_HDMI_MODE_LBR;
            }
            else
            {
                return SND_HDMI_MODE_PCM;
            }
            
        case IEC61937_DATATYPE_DTS_TYPE_I:
        case IEC61937_DATATYPE_DTS_TYPE_II:
        case IEC61937_DATATYPE_DTS_TYPE_III: 
        case IEC61937_DATATYPE_DTSCD:   
            if(HI_TRUE == pstSinkCap->bAudioFmtSupported[AO_HDMI_CAPABILITY_DTS])
            {
                return SND_HDMI_MODE_LBR;
            }
            else
            {
                return SND_HDMI_MODE_PCM;
            }

        case IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS:
            if(HI_TRUE == pstSinkCap->bAudioFmtSupported[AO_HDMI_CAPABILITY_DDP])
            {
                return SND_HDMI_MODE_HBR;
            }
            else
            {
                return SND_HDMI_MODE_LBR;
            }             
            
        case IEC61937_DATATYPE_DTS_TYPE_IV:
            if(HI_TRUE == pstSinkCap->bAudioFmtSupported[AO_HDMI_CAPABILITY_DTSHD])
            {
                return SND_HDMI_MODE_HBR;
            }
            else
            {
                return SND_HDMI_MODE_LBR;
            }
                
        case IEC61937_DATATYPE_DOLBY_TRUE_HD:
            if(HI_TRUE == pstSinkCap->bAudioFmtSupported[AO_HDMI_CAPABILITY_MAT])
            {
                return SND_HDMI_MODE_HBR;
            }
            else
            {
                return SND_HDMI_MODE_LBR;
            }

        case IEC61937_DATATYPE_71_LPCM:
            return SND_HDMI_MODE_HBR;  

        default:
            HI_WARN_AO("Failed to judge edid cabability of format %d\n", u32Format);
            return SND_HDMI_MODE_PCM;       
    }
}

TRACK_STREAMMODE_CHANGE_E GetHdmiChangeMode(SND_CARD_STATE_S *pCard, SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    SND_OP_ATTR_S stSndPortAttr;
    HI_HANDLE hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
    TRACK_STREAMMODE_CHANGE_E enChange = TRACK_STREAMMODE_CHANGE_NONE;
    SND_HDMI_MODE_E mode  = SND_HDMI_MODE_PCM;
    HI_BOOL bdisPassThrough = HI_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stSndPortAttr);

    /* ui */
    if (HI_UNF_SND_HDMI_MODE_LPCM == pCard->enUserHdmiMode)
    {
        bdisPassThrough = HI_TRUE;
    }

    /* no raw data at stream */
    if (!pstAttr->u32LbrFormat && !pstAttr->u32HbrFormat)
    {
        bdisPassThrough = HI_TRUE;
    }

    if (bdisPassThrough)
    {
        /* disable hdmi pass-through, switch to pcm */
        mode = SND_HDMI_MODE_PCM;
    }
    else
    {
        HI_UNF_HDMI_SINK_CAPABILITY_S stSinkCap;
        mode = SND_HDMI_MODE_LBR;
        if(pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiGetSinkCapability)
        {
            s32Ret = (pCard->pstHdmiFunc->pfnHdmiGetSinkCapability)(HI_UNF_HDMI_ID_0, &stSinkCap);
        }

        if (pstAttr->u32HbrFormat)   
        {
            mode = SND_HDMI_MODE_HBR; 
            if (HI_UNF_SND_HDMI_MODE_HBR2LBR == pCard->enUserHdmiMode)  
            {
                /* hbr2lbr */
                if(pstAttr->u32LbrFormat)
                {
                    mode = SND_HDMI_MODE_LBR;
                }
                else
                {
                    mode = SND_HDMI_MODE_PCM;
                }
            }
            if((HI_UNF_SND_HDMI_MODE_AUTO == pCard->enUserHdmiMode) && (HI_SUCCESS == s32Ret))
            {               
                mode = TRACKHdmiEdidChange(&stSinkCap, pstAttr->u32HbrFormat);
            } 
        }
        if(SND_HDMI_MODE_LBR == mode)
        {
            if((HI_UNF_SND_HDMI_MODE_AUTO == pCard->enUserHdmiMode) && (HI_SUCCESS == s32Ret))
            {
                mode = TRACKHdmiEdidChange(&stSinkCap, pstAttr->u32LbrFormat);
            }
        }

    }

    if (SND_HDMI_MODE_PCM == mode)
    {
        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_LBR2PCM;
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_HBR2PCM;
        }
    }
    else if (SND_HDMI_MODE_LBR == mode)
    {
        if (SND_HDMI_MODE_PCM == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_PCM2LBR;
        }
        else if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            if (pstAttr->u32LbrSampleRate != stSndPortAttr.u32SampleRate)
            {
                enChange = TRACK_STREAMMODE_CHANGE_LBR2LBR;
            }
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_HBR2LBR;
        }
    }
    else if (SND_HDMI_MODE_HBR == mode)
    {
        if (SND_HDMI_MODE_PCM == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_PCM2HBR;
        }
        else if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_LBR2HBR;
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            if (pstAttr->u32HbrSampleRate != stSndPortAttr.u32SampleRate)
            {
                enChange = TRACK_STREAMMODE_CHANGE_HBR2HBR;
            }
        }
    }

    return enChange;
}

TRACK_STREAMMODE_CHANGE_E GetSpdifChangeMode(SND_CARD_STATE_S *pCard, SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    SND_OP_ATTR_S stSndPortAttr;
    HI_HANDLE hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_SPDIF);
    HI_BOOL bdisPassThrough = HI_FALSE;
    TRACK_STREAMMODE_CHANGE_E enChange = TRACK_STREAMMODE_CHANGE_NONE;

    SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stSndPortAttr);

    /* ui or stream */
    if ((HI_UNF_SND_SPDIF_MODE_LPCM == pCard->enUserSpdifMode) || (IEC61937_DATATYPE_NULL == pstAttr->u32LbrFormat))
    {
        bdisPassThrough = HI_TRUE;
    }

    if (bdisPassThrough)
    {
        /* disable spdif pass-through */
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough)
        {
            enChange = TRACK_STREAMMODE_CHANGE_LBR2PCM;
        }
    }
    else
    {
        /* enable spdif pass-through */
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough)
        {
            if (pstAttr->u32LbrSampleRate != stSndPortAttr.u32SampleRate)
            {
                enChange = TRACK_STREAMMODE_CHANGE_LBR2LBR;  /* shtream change samplerate */
            }
        }
        else
        {
            enChange = TRACK_STREAMMODE_CHANGE_PCM2LBR;     /* atcive pass-through */
        }
    }

    return enChange;
}

HI_VOID DetectStreamModeChange(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, SND_TRACK_STREAM_ATTR_S *pstAttr,
                               STREAMMODE_CHANGE_ATTR_S *pstChange)
{
    SND_TRACK_STREAM_ATTR_S *pstAttr_old = &pTrack->stStreamAttr;

    pstChange->enPcmChange   = TRACK_STREAMMODE_CHANGE_NONE;
    pstChange->enSpdifChange = TRACK_STREAMMODE_CHANGE_NONE;
    pstChange->enHdmiChnage  = TRACK_STREAMMODE_CHANGE_NONE;

    // pcm stream attr
    if (pstAttr_old->u32PcmBitDepth != pstAttr->u32PcmBitDepth)
    {
        pstChange->enPcmChange = TRACK_STREAMMODE_CHANGE_PCM2PCM;
    }

    if (pstAttr_old->u32PcmChannels != pstAttr->u32PcmChannels)
    {
        pstChange->enPcmChange = TRACK_STREAMMODE_CHANGE_PCM2PCM;
    }

    if (pstAttr_old->u32PcmSampleRate != pstAttr->u32PcmSampleRate)
    {
        pstChange->enPcmChange = TRACK_STREAMMODE_CHANGE_PCM2PCM;
    }

    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        if (SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
        {
            pstChange->enSpdifChange = GetSpdifChangeMode(pCard, pstAttr);
        }

        if (SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
        {
            pstChange->enHdmiChnage  = GetHdmiChangeMode(pCard, pstAttr);
        }
    }
}

HI_VOID SndProcPcmRoute(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, TRACK_STREAMMODE_CHANGE_E enMode,
                        SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    AOE_AIP_CHN_ATTR_S stAipAttr;

    if (TRACK_STREAMMODE_CHANGE_NONE == enMode)
    {
        return;
    }

    HAL_AOE_AIP_GetAttr(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &stAipAttr);
    stAipAttr.stBufInAttr.u32BufBitPerSample = pstAttr->u32PcmBitDepth;
    stAipAttr.stBufInAttr.u32BufSampleRate = pstAttr->u32PcmSampleRate;
    stAipAttr.stBufInAttr.u32BufChannels   = pstAttr->u32PcmChannels;
    stAipAttr.stBufInAttr.u32BufDataFormat = 0;
    HAL_AOE_AIP_SetAttr(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &stAipAttr);

    memcpy(&pTrack->stStreamAttr, pstAttr, sizeof(SND_TRACK_STREAM_ATTR_S));
    return;
}


static HI_VOID TranslateOpAttr(SND_OP_ATTR_S *pstOpAttr,  TRACK_STREAMMODE_CHANGE_E enMode, SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    HI_U32 u32PeriondMs;
    HI_U32 u32FrameSize;
    HI_U32 BitDepth,Channels,Format,Rate;

    
    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode)
        || (TRACK_STREAMMODE_CHANGE_HBR2LBR == enMode))
    {
        BitDepth = pstAttr->u32LbrBitDepth;
        Channels = pstAttr->u32LbrChannels;
        Format = pstAttr->u32LbrFormat;
        Rate = pstAttr->u32LbrSampleRate;
    }
    else if ((TRACK_STREAMMODE_CHANGE_PCM2HBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2HBR == enMode)
             || (TRACK_STREAMMODE_CHANGE_HBR2HBR == enMode))
    {
        BitDepth = pstAttr->u32HbrBitDepth;
        Channels = pstAttr->u32HbrChannels;
        Format = pstAttr->u32HbrFormat;
        Rate = pstAttr->u32HbrSampleRate;
    }    
    else 
    {
        BitDepth = pstAttr->u32PcmBitDepth;
        Channels = pstAttr->u32PcmChannels;
        Rate = pstAttr->u32PcmSampleRate;
        Format = 0;
    }
#if 1
    /* recaculate PeriodBufSize */
    u32FrameSize = AUTIL_CalcFrameSize(pstOpAttr->u32Channels, pstOpAttr->u32BitPerSample);
    u32PeriondMs = AUTIL_ByteSize2LatencyMs(pstOpAttr->u32PeriodBufSize, u32FrameSize, pstOpAttr->u32SampleRate);
    u32FrameSize = AUTIL_CalcFrameSize(Channels, BitDepth);
    pstOpAttr->u32PeriodBufSize = AUTIL_LatencyMs2ByteSize(u32PeriondMs, u32FrameSize, Rate);
#endif    
    pstOpAttr->u32BitPerSample = BitDepth;
    pstOpAttr->u32SampleRate = Rate;
    pstOpAttr->u32Channels   = Channels;
    pstOpAttr->u32DataFormat = Format;
}


//verify should simple , can change hdmi attr according to op attr
static HI_VOID  HDMIAudioChange(SND_CARD_STATE_S *pCard, TRACK_STREAMMODE_CHANGE_E enMode,
                          SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    HDMI_AUDIOINTERFACE_E enHdmiSoundIntf;
    HDMI_AUDIO_ATTR_S stHDMIAtr;
    HI_U32 Channels,Rate;

    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode)
        || (TRACK_STREAMMODE_CHANGE_HBR2LBR == enMode))
    {
        Channels = pstAttr->u32LbrChannels;
        Rate = pstAttr->u32LbrSampleRate;
        enHdmiSoundIntf = HDMI_AUDIO_INTERFACE_SPDIF;
    }
    else if ((TRACK_STREAMMODE_CHANGE_PCM2HBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2HBR == enMode)
             || (TRACK_STREAMMODE_CHANGE_HBR2HBR == enMode))
    {
        Rate = pstAttr->u32HbrSampleRate;
        Channels = pstAttr->u32HbrChannels;
        enHdmiSoundIntf = HDMI_AUDIO_INTERFACE_HBR;
        if(IEC61937_DATATYPE_71_LPCM==pstAttr->u32HbrFormat)
        {
            Channels = pstAttr->u32OrgMultiPcmChannels;
            enHdmiSoundIntf = HDMI_AUDIO_INTERFACE_I2S;   //verify
        }
    
        if (IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS == pstAttr->u32HbrFormat)
        {
            enHdmiSoundIntf = HDMI_AUDIO_INTERFACE_SPDIF;
        }
    }    
    else if ((TRACK_STREAMMODE_CHANGE_LBR2PCM == enMode) || (TRACK_STREAMMODE_CHANGE_HBR2PCM == enMode))
    {
        HI_HANDLE hSndPcmOnlyOp;
        SND_OP_ATTR_S stPcmOnlyOpAttr;
        hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
        if (!hSndPcmOnlyOp)
        {
            hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        }

        if (!hSndPcmOnlyOp)
        {
            Channels   = AO_TRACK_NORMAL_CHANNELNUM;
            Rate = HI_UNF_SAMPLE_RATE_48K;
        }
        else
        {
            SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndPcmOnlyOp), &stPcmOnlyOpAttr);  
            Channels = stPcmOnlyOpAttr.u32Channels;
            Rate = stPcmOnlyOpAttr.u32SampleRate;
        }
        enHdmiSoundIntf = HDMI_AUDIO_INTERFACE_I2S;
    }
    else
    {
        return;
    }
    
    /*if the channels of the frame have changed , set the attribute of HDMI*/
    if(pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiGetAoAttr)
    {
        (pCard->pstHdmiFunc->pfnHdmiGetAoAttr)(HI_UNF_HDMI_ID_0, &stHDMIAtr);
    }

    stHDMIAtr.enSoundIntf  = enHdmiSoundIntf;
    stHDMIAtr.enSampleRate = (HI_UNF_SAMPLE_RATE_E)Rate;
    stHDMIAtr.u32Channels  = Channels;

    /*get the capability of the max pcm channels of the output device*/
    if(pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiAudioChange)
    {
        (pCard->pstHdmiFunc->pfnHdmiAudioChange)(HI_UNF_HDMI_ID_0,&stHDMIAtr);
    }
}

HI_VOID SndProcHdmifRoute(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, TRACK_STREAMMODE_CHANGE_E enMode,
                          SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    AOE_AIP_CHN_ATTR_S stAipAttr;
    HI_HANDLE hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
    AOE_AOP_ID_E Aop;
    SND_OP_ATTR_S stOpAttr;
    AOE_ENGINE_CHN_ATTR_S stEngineAttr;
    SND_ENGINE_TYPE_E enEngineNew;
    SND_ENGINE_TYPE_E enEngineOld;
    HI_HANDLE hEngineNew;
    HI_HANDLE hEngineOld;
	SND_ENGINE_STATE_S *state;
    if (TRACK_STREAMMODE_CHANGE_NONE == enMode)
    {
        return;
    }

    Aop = SND_OpGetAopId(hSndOp);
    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode)
        || (TRACK_STREAMMODE_CHANGE_HBR2LBR == enMode))
    {
        //set op
        if (TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode)
        {
            enEngineOld = SND_ENGINE_TYPE_PCM;
        }
        else
        {
            enEngineOld = SND_ENGINE_TYPE_HDMI_RAW;
        }

        enEngineNew = SND_ENGINE_TYPE_HDMI_RAW;
        hEngineOld = TrackGetEngineHandlebyType(pCard, enEngineOld);
        hEngineNew = TrackGetEngineHandlebyType(pCard, enEngineNew);
        SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);

        SND_StopOp(pCard, SND_GetOpOutputport(hSndOp));
		state = (SND_ENGINE_STATE_S *)hEngineOld;
		HAL_AOE_ENGINE_DetachAop(state->enEngine, Aop);
        TranslateOpAttr(&stOpAttr,enMode,pstAttr);
        SND_SetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        Aop = SND_OpGetAopId(hSndOp);  //verify
        state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAop(state->enEngine, Aop);
        SND_StartOp(pCard, SND_GetOpOutputport(hSndOp));

        //set aip
        HAL_AOE_AIP_GetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
        stAipAttr.stBufInAttr.u32BufBitPerSample = pstAttr->u32LbrBitDepth;
        stAipAttr.stBufInAttr.u32BufSampleRate = pstAttr->u32LbrSampleRate;
        stAipAttr.stBufInAttr.u32BufChannels   = pstAttr->u32LbrChannels;
        stAipAttr.stBufInAttr.u32BufDataFormat = pstAttr->u32LbrFormat;
        HAL_AOE_AIP_SetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
		state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAip(state->enEngine, pTrack->enAIP[enEngineNew]);

        pCard->u32HdmiDataFormat = stAipAttr.stBufInAttr.u32BufDataFormat;
        //set engine
        state = (SND_ENGINE_STATE_S *)hEngineNew;
		HAL_AOE_ENGINE_Stop(state->enEngine);
        stEngineAttr.u32BitPerSample = pstAttr->u32LbrBitDepth;
        stEngineAttr.u32Channels   = pstAttr->u32LbrChannels;
        stEngineAttr.u32SampleRate = pstAttr->u32LbrSampleRate;
        stEngineAttr.u32DataFormat = pstAttr->u32LbrFormat;
		HAL_AOE_ENGINE_SetAttr(state->enEngine, &stEngineAttr);
		HAL_AOE_ENGINE_Start(state->enEngine);
    }
    else if ((TRACK_STREAMMODE_CHANGE_PCM2HBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2HBR == enMode)
             || (TRACK_STREAMMODE_CHANGE_HBR2HBR == enMode))
    {
        //set op
        if (TRACK_STREAMMODE_CHANGE_PCM2HBR == enMode)
        {
            enEngineOld = SND_ENGINE_TYPE_PCM;
        }
        else
        {
            enEngineOld = SND_ENGINE_TYPE_HDMI_RAW;
        }

        enEngineNew = SND_ENGINE_TYPE_HDMI_RAW;
        hEngineOld = TrackGetEngineHandlebyType(pCard, enEngineOld);
        hEngineNew = TrackGetEngineHandlebyType(pCard, enEngineNew);
        SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        SND_StopOp(pCard, SND_GetOpOutputport(hSndOp));
		state = (SND_ENGINE_STATE_S *)hEngineOld;//verify
		HAL_AOE_ENGINE_DetachAop(state->enEngine, Aop);
        TranslateOpAttr(&stOpAttr,enMode,pstAttr);
        SND_SetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        Aop = SND_OpGetAopId(hSndOp); //verify
        state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAop(state->enEngine, Aop);
        SND_StartOp(pCard, SND_GetOpOutputport(hSndOp));

        //set aip
        HAL_AOE_AIP_GetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
        stAipAttr.stBufInAttr.u32BufBitPerSample = pstAttr->u32HbrBitDepth;
        stAipAttr.stBufInAttr.u32BufSampleRate = pstAttr->u32HbrSampleRate;
        stAipAttr.stBufInAttr.u32BufChannels   = pstAttr->u32HbrChannels;
        stAipAttr.stBufInAttr.u32BufDataFormat = pstAttr->u32HbrFormat;
        HAL_AOE_AIP_SetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
		state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAip(state->enEngine, pTrack->enAIP[enEngineNew]);

        pCard->u32HdmiDataFormat = stAipAttr.stBufInAttr.u32BufDataFormat;
        //set engine
        state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_Stop(state->enEngine);
        stEngineAttr.u32BitPerSample = pstAttr->u32HbrBitDepth;
        stEngineAttr.u32Channels   = pstAttr->u32HbrChannels;
        stEngineAttr.u32SampleRate = pstAttr->u32HbrSampleRate;
        stEngineAttr.u32DataFormat = pstAttr->u32HbrFormat;
		HAL_AOE_ENGINE_SetAttr(state->enEngine, &stEngineAttr);
		HAL_AOE_ENGINE_Start(state->enEngine);
    }
    else if ((TRACK_STREAMMODE_CHANGE_LBR2PCM == enMode) || (TRACK_STREAMMODE_CHANGE_HBR2PCM == enMode))
    {
        HI_HANDLE hSndPcmOnlyOp;
        SND_OP_ATTR_S stPcmOnlyOpAttr;
        enEngineOld = SND_ENGINE_TYPE_HDMI_RAW;
        enEngineNew = SND_ENGINE_TYPE_PCM;
        hEngineOld = TrackGetEngineHandlebyType(pCard, enEngineOld);
        hEngineNew = TrackGetEngineHandlebyType(pCard, enEngineNew);

        //set op
        SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        SND_StopOp(pCard, SND_GetOpOutputport(hSndOp));
		state = (SND_ENGINE_STATE_S *)hEngineOld; //verify
		HAL_AOE_ENGINE_DetachAop(state->enEngine, Aop);
		HAL_AOE_ENGINE_DetachAip(state->enEngine, pTrack->enAIP[enEngineOld]);
		HAL_AOE_ENGINE_Stop(state->enEngine);

        // reset attr
        hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
        if (!hSndPcmOnlyOp)
        {
            hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        }

        if (!hSndPcmOnlyOp)
        {
            stOpAttr.u32BitPerSample = AO_TRACK_BITDEPTH_LOW;
            stOpAttr.u32Channels   = AO_TRACK_NORMAL_CHANNELNUM;
            stOpAttr.u32SampleRate = HI_UNF_SAMPLE_RATE_48K;
            stOpAttr.u32DataFormat = 0;
            stOpAttr.u32PeriodBufSize = AO_SNDOP_PERIODBUFSIZE;
            stOpAttr.u32LatencyThdMs = AO_SNDOP_LATENCY_THDMS;
        }
        else
        {
            SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndPcmOnlyOp), &stPcmOnlyOpAttr);  //verify
            stOpAttr.u32BitPerSample = stPcmOnlyOpAttr.u32BitPerSample;
            stOpAttr.u32Channels   = stPcmOnlyOpAttr.u32Channels;
            stOpAttr.u32SampleRate = stPcmOnlyOpAttr.u32SampleRate;
            stOpAttr.u32PeriodBufSize = stPcmOnlyOpAttr.u32PeriodBufSize;
            stOpAttr.u32LatencyThdMs = stPcmOnlyOpAttr.u32LatencyThdMs;
            stOpAttr.u32DataFormat = 0;
        }

        SND_SetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        Aop = SND_OpGetAopId(hSndOp); //verify
        state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAop(state->enEngine, Aop);
        SND_StartOp(pCard, SND_GetOpOutputport(hSndOp));
        pCard->u32HdmiDataFormat = 0;
    }

    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode)
        || (TRACK_STREAMMODE_CHANGE_HBR2LBR == enMode))
    {
        pCard->enHdmiPassthrough = SND_HDMI_MODE_LBR;
    }
    else if ((TRACK_STREAMMODE_CHANGE_PCM2HBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2HBR == enMode)
             || (TRACK_STREAMMODE_CHANGE_HBR2HBR == enMode))
    {
        pCard->enHdmiPassthrough = SND_HDMI_MODE_HBR;
    }
    else
    {
        pCard->enHdmiPassthrough = SND_HDMI_MODE_PCM;
    }
    HDMIAudioChange(pCard,enMode,pstAttr);

    memcpy(&pTrack->stStreamAttr, pstAttr, sizeof(SND_TRACK_STREAM_ATTR_S));
}

HI_VOID SndProcSpidfRoute(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, TRACK_STREAMMODE_CHANGE_E enMode,
                          SND_TRACK_STREAM_ATTR_S *pstAttr)
{
    AOE_AIP_CHN_ATTR_S stAipAttr;
    HI_HANDLE hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_SPDIF);
    AOE_AOP_ID_E Aop;
    SND_OP_ATTR_S stOpAttr;
    AOE_ENGINE_CHN_ATTR_S stEngineAttr;
    SND_ENGINE_TYPE_E enEngineNew;
    SND_ENGINE_TYPE_E enEngineOld;
    HI_HANDLE hEngineNew;
    HI_HANDLE hEngineOld;
	SND_ENGINE_STATE_S *state;

    if (TRACK_STREAMMODE_CHANGE_NONE == enMode)
    {
        return;
    }

    Aop = SND_OpGetAopId(hSndOp);
    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode))
    {
        /* enable pass-through */

        //set op
        if (TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode)
        {
            enEngineOld = SND_ENGINE_TYPE_PCM;
        }
        else
        {
            enEngineOld = SND_ENGINE_TYPE_SPDIF_RAW;
        }

        enEngineNew = SND_ENGINE_TYPE_SPDIF_RAW;
        hEngineOld = TrackGetEngineHandlebyType(pCard, enEngineOld);
        hEngineNew = TrackGetEngineHandlebyType(pCard, enEngineNew);

        SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        SND_StopOp(pCard, SND_GetOpOutputport(hSndOp));
		state = (SND_ENGINE_STATE_S *)hEngineOld;
		HAL_AOE_ENGINE_DetachAop(state->enEngine, Aop);
        TranslateOpAttr(&stOpAttr,enMode,pstAttr);
        SND_SetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
		state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAop(state->enEngine, Aop);
        SND_StartOp(pCard, SND_GetOpOutputport(hSndOp));

        //set aip
        HAL_AOE_AIP_GetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
        stAipAttr.stBufInAttr.u32BufBitPerSample = pstAttr->u32LbrBitDepth;
        stAipAttr.stBufInAttr.u32BufSampleRate = pstAttr->u32LbrSampleRate;
        stAipAttr.stBufInAttr.u32BufChannels   = pstAttr->u32LbrChannels;
        stAipAttr.stBufInAttr.u32BufDataFormat = pstAttr->u32LbrFormat;
        HAL_AOE_AIP_SetAttr(pTrack->enAIP[enEngineNew], &stAipAttr);
		state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAip(state->enEngine, pTrack->enAIP[enEngineNew]);

        pCard->u32SpdifDataFormat = stAipAttr.stBufInAttr.u32BufDataFormat;
            
        //set engine
        state = (SND_ENGINE_STATE_S *)hEngineNew;
		HAL_AOE_ENGINE_Stop(state->enEngine);
        stEngineAttr.u32BitPerSample = pstAttr->u32LbrBitDepth;
        stEngineAttr.u32Channels   = pstAttr->u32LbrChannels;
        stEngineAttr.u32SampleRate = pstAttr->u32LbrSampleRate;
        stEngineAttr.u32DataFormat = pstAttr->u32LbrFormat;
		HAL_AOE_ENGINE_SetAttr(state->enEngine, &stEngineAttr);
		HAL_AOE_ENGINE_Start(state->enEngine);
    }
    else if (TRACK_STREAMMODE_CHANGE_LBR2PCM == enMode)
    {
        /* disable pass-through */
        HI_HANDLE hSndPcmOnlyOp;
        SND_OP_ATTR_S stPcmOnlyOpAttr;
        enEngineOld = SND_ENGINE_TYPE_SPDIF_RAW;
        enEngineNew = SND_ENGINE_TYPE_PCM;
        hEngineOld = TrackGetEngineHandlebyType(pCard, enEngineOld);
        hEngineNew = TrackGetEngineHandlebyType(pCard, enEngineNew);

        //set op
        SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
        SND_StopOp(pCard, SND_GetOpOutputport(hSndOp));
		state = (SND_ENGINE_STATE_S *)hEngineOld;
		HAL_AOE_ENGINE_DetachAop(state->enEngine, Aop);
		HAL_AOE_ENGINE_DetachAip(state->enEngine, pTrack->enAIP[enEngineOld]);
		HAL_AOE_ENGINE_Stop(state->enEngine);

        // reset attr
        hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
        if (!hSndPcmOnlyOp)
        {
            hSndPcmOnlyOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        }

        if (!hSndPcmOnlyOp)
        {
            stOpAttr.u32BitPerSample = AO_TRACK_BITDEPTH_LOW;
            stOpAttr.u32Channels   = AO_TRACK_NORMAL_CHANNELNUM;
            stOpAttr.u32SampleRate = HI_UNF_SAMPLE_RATE_48K;
            stOpAttr.u32DataFormat = 0;
            stOpAttr.u32PeriodBufSize = AO_SNDOP_PERIODBUFSIZE;
            stOpAttr.u32LatencyThdMs = AO_SNDOP_LATENCY_THDMS;
        }
        else
        {
            SND_GetOpAttr(pCard, SND_GetOpOutputport(hSndPcmOnlyOp), &stPcmOnlyOpAttr);
            stOpAttr.u32BitPerSample = stPcmOnlyOpAttr.u32BitPerSample;
            stOpAttr.u32Channels   = stPcmOnlyOpAttr.u32Channels;
            stOpAttr.u32SampleRate = stPcmOnlyOpAttr.u32SampleRate;
            stOpAttr.u32PeriodBufSize = stPcmOnlyOpAttr.u32PeriodBufSize;
            stOpAttr.u32LatencyThdMs = stPcmOnlyOpAttr.u32LatencyThdMs;
            stOpAttr.u32DataFormat = 0;
        }

        SND_SetOpAttr(pCard, SND_GetOpOutputport(hSndOp), &stOpAttr);
		state = (SND_ENGINE_STATE_S *)hEngineNew;//verify
		HAL_AOE_ENGINE_AttachAop(state->enEngine, Aop);
        SND_StartOp(pCard, SND_GetOpOutputport(hSndOp));

        pCard->u32SpdifDataFormat = 0;
    }

    if ((TRACK_STREAMMODE_CHANGE_PCM2LBR == enMode) || (TRACK_STREAMMODE_CHANGE_LBR2LBR == enMode))
    {
        pCard->enSpdifPassthrough = SND_SPDIF_MODE_LBR; 
    }
    else
    {
        pCard->enSpdifPassthrough = SND_SPDIF_MODE_PCM; 
    }

    memcpy(&pTrack->stStreamAttr, pstAttr, sizeof(SND_TRACK_STREAM_ATTR_S));
    return;
}

//zgjiere; u32BufLevelMs需要细心检查异常情况，避免堵塞，u32BufLevelMs异常时，认为无流控
HI_BOOL TrackisBufFree(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    HI_U32 Free = 0;
    HI_U32 DelayMs = 0;
    HI_U32 FrameSize = 0;
    HI_U32 FrameMs = 0;
    HI_U32 PcmFrameBytes = 0;
    HI_U32 SpdifRawBytes = 0;
    HI_U32 HdmiRawBytes = 0;

    PcmFrameBytes = pstStreamAttr->u32PcmBytesPerFrame;
    Free = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
    if (Free <= PcmFrameBytes)
    {
        return HI_FALSE;
    }
#if 1
    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &DelayMs);
    FrameSize = AUTIL_CalcFrameSize(pstStreamAttr->u32PcmChannels, pstStreamAttr->u32PcmBitDepth);
    FrameMs = AUTIL_ByteSize2LatencyMs(PcmFrameBytes, FrameSize, pstStreamAttr->u32PcmSampleRate);
    if (DelayMs + FrameMs >= pTrack->stUserTrackAttr.u32BufLevelMs)
    {
        return HI_FALSE;
    }
#else
    HI_U32 TrackDelayMs = 0;
    HI_U32 TrackFiFoDelayMs = 0;
    HI_U32 SndFiFoDelayMs = 0;

    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &TrackDelayMs);
    HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &TrackFiFoDelayMs); // for aip fifo delay
    SND_GetDelayMs(pTrack->enSound ,&SndFiFoDelayMs);
    DelayMs = TrackDelayMs + TrackFiFoDelayMs + SndFiFoDelayMs;

    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &DelayMs);
    if (DelayMs >= pTrack->stUserTrackAttr.u32BufLevelMs)
    {
        return HI_FALSE;
    }
#endif
    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough)
        {
            SpdifRawBytes = pstStreamAttr->u32LbrBytesPerFrame;
            Free = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
            if (Free <= SpdifRawBytes)
            {
                return HI_FALSE;
            }

            HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW], &DelayMs);  //verify pcm controled , passthrough need control
            if (DelayMs >= pTrack->stUserTrackAttr.u32BufLevelMs)
            {
                return HI_FALSE;
            }
        }

        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            HdmiRawBytes = pstStreamAttr->u32LbrBytesPerFrame;
            Free = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            if (Free <= HdmiRawBytes)
            {
                return HI_FALSE;
            }

            HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], &DelayMs);
            if (DelayMs >= pTrack->stUserTrackAttr.u32BufLevelMs)
            {
                return HI_FALSE;
            }
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)

        {
            HdmiRawBytes = pstStreamAttr->u32HbrBytesPerFrame;
            Free = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            if (Free <= HdmiRawBytes)
            {
                return HI_FALSE;
            }

            HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], &DelayMs);
            if (DelayMs >= pTrack->stUserTrackAttr.u32BufLevelMs)
            {
                return HI_FALSE;
            }
        }
    }

    return HI_TRUE;
}

HI_VOID TRACKStartAip(SND_TRACK_STATE_S *pTrack)
{
    HI_U32 u32DelayMs;
    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &u32DelayMs);
    if(SND_TRACK_STATUS_START == pTrack->enCurnStatus)
    {
        if((HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType && u32DelayMs >= AO_TRACK_AIP_START_LATENCYMS) //for master, 50ms start
            || (HI_UNF_SND_TRACK_TYPE_SLAVE == pTrack->stUserTrackAttr.enTrackType) //for slave, immediately start
            || (HI_TRUE == pTrack->bEosFlag)) //if set eosflag, immediately start
        {
            HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
            if(HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
            {
                HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
                HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            }
        }
    }

    return;
}

HI_VOID TRACKPcmUnifyProcess(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    // todo
    // step 1 , note Interleaved to Interleaved

    // step 2,  7.1+2.0 PCM  
}

static HI_VOID TRACKWriteFrame(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    HI_U32 Write = 0;
    HI_U32 PcmFrameBytes = 0;
    HI_U32 SpdifRawBytes = 0;
    HI_U32 HdmiRawBytes = 0;

    TRACKPcmUnifyProcess(pCard,pTrack, pstStreamAttr);

    PcmFrameBytes = pstStreamAttr->u32PcmBytesPerFrame;
    Write = HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_PCM], (HI_U8 *)pstStreamAttr->pPcmDataBuf,
                                  PcmFrameBytes);
    if (Write != PcmFrameBytes)
    {
        HI_ERR_AO("HAL_AOE_AIP_WriteBufData fail write(%d) actual(%d)\n", PcmFrameBytes, Write);
    }

    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {  
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough)
        {
            SpdifRawBytes = pstStreamAttr->u32LbrBytesPerFrame;
            Write = HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW],
                                             (HI_U8 *)pstStreamAttr->pLbrDataBuf, SpdifRawBytes);
            if (Write != SpdifRawBytes)
            {
                HI_ERR_AO("HAL_AOE_AIP_WriteBufData fail write(%d) actual(%d)\n", SpdifRawBytes, Write);
            }
        }

        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            HdmiRawBytes = pstStreamAttr->u32LbrBytesPerFrame;
            Write = HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW],
                                             (HI_U8 *)pstStreamAttr->pLbrDataBuf, HdmiRawBytes);
            if (Write != HdmiRawBytes)
            {
                HI_ERR_AO("HAL_AOE_AIP_WriteBufData fail write(%d) actual(%d)\n", HdmiRawBytes, Write);
            }
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            HdmiRawBytes = pstStreamAttr->u32HbrBytesPerFrame;
            Write = HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW],
                                             (HI_U8 *)pstStreamAttr->pHbrDataBuf, HdmiRawBytes);
            if (Write != HdmiRawBytes)
            {
                HI_ERR_AO("HAL_AOE_AIP_WriteBufData(%d) fail\n", HdmiRawBytes);
            }
        }
    }

    return;
}

static HI_VOID TRACKWriteMuteFrame(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *pTrack, SND_TRACK_STREAM_ATTR_S * pstStreamAttr)
{
    HI_U32 SpdifRawFree = 0, HdmiRawFree = 0;
    HI_U32 SpdifRawBusy = 0, HdmiRawBusy = 0;
    HI_U32 SpdifRawData = 0, HdmiRawData = 0;
    HI_U32 PcmDelayMs = 0, SpdifDelayMs = 0, HdmiDelayMs = 0;
    HI_U32 FrameSize = 0;

    if(HI_UNF_SND_TRACK_TYPE_MASTER != pTrack->stUserTrackAttr.enTrackType)
    {
        return;
    }

    if (SND_SPDIF_MODE_PCM >= pCard->enSpdifPassthrough && SND_SPDIF_MODE_PCM >= pCard->enHdmiPassthrough)
    {
        return;
    }
    
    SpdifRawBusy = HAL_AOE_AIP_QueryBufData(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
    HdmiRawBusy = HAL_AOE_AIP_QueryBufData(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);

    //When the data of raw AIP less than two frame, send mute frame      
    if ((SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough && SpdifRawBusy < 2 * pstStreamAttr->u32LbrBytesPerFrame) 
           || (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough && HdmiRawBusy < 2 * pstStreamAttr->u32LbrBytesPerFrame)
           || (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough && HdmiRawBusy < 2 * pstStreamAttr->u32HbrBytesPerFrame))
    {
        HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &PcmDelayMs);
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough) 
        {
            HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW], &SpdifDelayMs);
            if(PcmDelayMs > SpdifDelayMs)
            {
                FrameSize = AUTIL_CalcFrameSize(pstStreamAttr->u32LbrChannels, pstStreamAttr->u32LbrBitDepth);
                SpdifRawData = AUTIL_LatencyMs2ByteSize(PcmDelayMs - SpdifDelayMs, FrameSize, pstStreamAttr->u32LbrSampleRate); //mute frame size is difference value between delayms
                if(SpdifRawData < pstStreamAttr->u32LbrBytesPerFrame)  
                {
                    SpdifRawData = pstStreamAttr->u32LbrBytesPerFrame;   //if  it is less than a frame, send a frame
                }
                SpdifRawFree = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
                if(SpdifRawData > SpdifRawFree)
                {
                    return;
                }
            }         
        }
        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            
            HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], &HdmiDelayMs);
            if(PcmDelayMs > HdmiDelayMs)
            FrameSize = AUTIL_CalcFrameSize(pstStreamAttr->u32LbrChannels, pstStreamAttr->u32LbrBitDepth);
            HdmiRawData = AUTIL_LatencyMs2ByteSize(PcmDelayMs - HdmiDelayMs, FrameSize, pstStreamAttr->u32LbrSampleRate);
            if(HdmiRawData < pstStreamAttr->u32LbrBytesPerFrame)
            {
                HdmiRawData = pstStreamAttr->u32LbrBytesPerFrame;
            }
            HdmiRawFree = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            if(HdmiRawData > HdmiRawFree)
            {
                return;
            }
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], &HdmiDelayMs);
            if(PcmDelayMs > HdmiDelayMs)
            FrameSize = AUTIL_CalcFrameSize(pstStreamAttr->u32HbrChannels, pstStreamAttr->u32HbrBitDepth);
            HdmiRawData = AUTIL_LatencyMs2ByteSize(PcmDelayMs - HdmiDelayMs, FrameSize, pstStreamAttr->u32HbrSampleRate);
            if(HdmiRawData < pstStreamAttr->u32HbrBytesPerFrame)
            {
                HdmiRawData = pstStreamAttr->u32HbrBytesPerFrame;
            }
            HdmiRawFree = HAL_AOE_AIP_QueryBufFree(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            if(HdmiRawData > HdmiRawFree)
            {
                return;
            }
        }

        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough) 
        {
            HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW], HI_NULL, SpdifRawData);
        }
        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], HI_NULL, HdmiRawData);
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            HAL_AOE_AIP_WriteBufData(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW], HI_NULL, HdmiRawData);
        }

        pTrack->u32AddMuteFrameNum++;
    }

    return; 
}

HI_VOID TrackSetAipRbfAttr(AOE_RBUF_ATTR_S *pRbfAttr, MMZ_BUFFER_S *pstRbfMmz)
{
    pRbfAttr->u32BufPhyAddr = pstRbfMmz->u32StartPhyAddr;
    pRbfAttr->u32BufVirAddr = pstRbfMmz->u32StartVirAddr;
    pRbfAttr->u32BufSize = pstRbfMmz->u32Size;
    pRbfAttr->u32BufWptrRptrFlag = 0;  /* cpu write */
}

HI_VOID TrackGetAipPcmDfAttr(AOE_AIP_CHN_ATTR_S *pstAipAttr, MMZ_BUFFER_S *pstRbfMmz,
                             HI_UNF_AUDIOTRACK_ATTR_S *pstUnfAttr)
{
    TrackSetAipRbfAttr(&pstAipAttr->stBufInAttr.stRbfAttr, pstRbfMmz);
    pstAipAttr->stBufInAttr.u32BufBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stBufInAttr.u32BufSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAipAttr->stBufInAttr.u32BufChannels     = AO_TRACK_NORMAL_CHANNELNUM;
    pstAipAttr->stBufInAttr.u32BufDataFormat   = 0;
    pstAipAttr->stBufInAttr.u32BufLatencyThdMs = pstUnfAttr->u32BufLevelMs;
    pstAipAttr->stBufInAttr.u32FadeinMs  = pstUnfAttr->u32FadeinMs;
    pstAipAttr->stBufInAttr.u32FadeoutMs = pstUnfAttr->u32FadeoutMs;
    pstAipAttr->stBufInAttr.bFadeEnable = HI_FALSE;
    pstAipAttr->stBufInAttr.bAlsaEnable = HI_FALSE;
    if (pstUnfAttr->u32FadeinMs | pstUnfAttr->u32FadeoutMs)
    {
        pstAipAttr->stBufInAttr.bFadeEnable = HI_TRUE;
    }

    pstAipAttr->stFifoOutAttr.u32FifoBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stFifoOutAttr.u32FifoSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAipAttr->stFifoOutAttr.u32FifoChannels     = AO_TRACK_NORMAL_CHANNELNUM;
    pstAipAttr->stFifoOutAttr.u32FifoDataFormat   = 0;
    pstAipAttr->stFifoOutAttr.u32FiFoLatencyThdMs = AIP_FIFO_LATENCYMS_DEFAULT;
}

HI_VOID TrackGetAipLbrDfAttr(AOE_AIP_CHN_ATTR_S *pstAipAttr, MMZ_BUFFER_S *pstRbfMmz,
                             HI_UNF_AUDIOTRACK_ATTR_S *pstUnfAttr)
{
    TrackSetAipRbfAttr(&pstAipAttr->stBufInAttr.stRbfAttr, pstRbfMmz);
    pstAipAttr->stBufInAttr.u32BufBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stBufInAttr.u32BufSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAipAttr->stBufInAttr.u32BufChannels     = AO_TRACK_NORMAL_CHANNELNUM;
    pstAipAttr->stBufInAttr.u32BufDataFormat   = 1;
    pstAipAttr->stBufInAttr.u32BufLatencyThdMs = pstUnfAttr->u32BufLevelMs;
    pstAipAttr->stBufInAttr.bFadeEnable = HI_FALSE;

    pstAipAttr->stFifoOutAttr.u32FifoBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stFifoOutAttr.u32FifoSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAipAttr->stFifoOutAttr.u32FifoChannels     = AO_TRACK_NORMAL_CHANNELNUM;
    pstAipAttr->stFifoOutAttr.u32FifoDataFormat   = 1;
    pstAipAttr->stFifoOutAttr.u32FiFoLatencyThdMs = AIP_FIFO_LATENCYMS_DEFAULT;
}

HI_VOID TrackGetAipHbrDfAttr(AOE_AIP_CHN_ATTR_S *pstAipAttr, MMZ_BUFFER_S *pstRbfMmz,
                             HI_UNF_AUDIOTRACK_ATTR_S *pstUnfAttr)
{
    TrackSetAipRbfAttr(&pstAipAttr->stBufInAttr.stRbfAttr, pstRbfMmz);
    pstAipAttr->stBufInAttr.u32BufBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stBufInAttr.u32BufSampleRate = HI_UNF_SAMPLE_RATE_192K;
    pstAipAttr->stBufInAttr.u32BufChannels     = AO_TRACK_MUTILPCM_CHANNELNUM;
    pstAipAttr->stBufInAttr.u32BufDataFormat   = 1;
    pstAipAttr->stBufInAttr.u32BufLatencyThdMs = pstUnfAttr->u32BufLevelMs;;
    pstAipAttr->stBufInAttr.bFadeEnable = HI_FALSE;

    pstAipAttr->stFifoOutAttr.u32FifoBitPerSample = AO_TRACK_BITDEPTH_LOW;
    pstAipAttr->stFifoOutAttr.u32FifoSampleRate = HI_UNF_SAMPLE_RATE_192K;
    pstAipAttr->stFifoOutAttr.u32FifoChannels     = AO_TRACK_MUTILPCM_CHANNELNUM;
    pstAipAttr->stFifoOutAttr.u32FifoDataFormat   = 1;
    pstAipAttr->stFifoOutAttr.u32FiFoLatencyThdMs = AIP_FIFO_LATENCYMS_DEFAULT;
}

HI_S32 TrackCreateMaster(SND_CARD_STATE_S *pCard, SND_TRACK_STATE_S *state, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr)
{
    HI_S32 Ret;
    AOE_AIP_ID_E enAIP;
    AOE_AIP_CHN_ATTR_S stAipAttr;
    MMZ_BUFFER_S stRbfMmz;

    Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                 &stRbfMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap failed\n");
        goto ALLOC_PCM_ERR_EXIT;
    }

    TrackGetAipPcmDfAttr(&stAipAttr, &stRbfMmz, pstAttr);
    Ret = HAL_AOE_AIP_Create(&enAIP, &stAipAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_AIP_Create(%d) failed\n", enAIP);
        goto CREATE_PCM_ERR_EXIT;
    }

    state->enAIP[SND_ENGINE_TYPE_PCM] = enAIP;
    state->stAipRbfMmz[SND_ENGINE_TYPE_PCM] = stRbfMmz;
    state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_PCM] = HI_FALSE;

    if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
    {
        Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipSpdRaw", MMZ_OTHERS, AO_TRACK_LBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                     &stRbfMmz);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("MMZ_AllocAndMap failed\n");
            goto ALLOC_SPDIF_ERR_EXIT;
        }

        TrackGetAipLbrDfAttr(&stAipAttr, &stRbfMmz, pstAttr);
        Ret = HAL_AOE_AIP_Create(&enAIP, &stAipAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AIP_Create(%d) failed\n", enAIP);
            goto CREATE_SPDIF_ERR_EXIT;
        }

        state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW] = enAIP;
        state->stAipRbfMmz[SND_ENGINE_TYPE_SPDIF_RAW] = stRbfMmz;
        state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_SPDIF_RAW] = HI_FALSE;
    }

    if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
    {
        Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipHdmiRaw", MMZ_OTHERS, AO_TRACK_HBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                     &stRbfMmz);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("MMZ_AllocAndMap failed\n");
            goto ALLOC_HDMI_ERR_EXIT;
        }

        TrackGetAipHbrDfAttr(&stAipAttr, &stRbfMmz, pstAttr);
        Ret = HAL_AOE_AIP_Create(&enAIP, &stAipAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AIP_Create failed\n");
            goto CREATE_HDMI_ERR_EXIT;
        }

        state->enAIP[SND_ENGINE_TYPE_HDMI_RAW] = enAIP;
        state->stAipRbfMmz[SND_ENGINE_TYPE_HDMI_RAW] = stRbfMmz;
        state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_HDMI_RAW] = HI_FALSE;
    }
    
    state->enCurnStatus = SND_TRACK_STATUS_STOP;
    return HI_SUCCESS;

CREATE_HDMI_ERR_EXIT: 
    if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
    {
        HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_HDMI_RAW]);
    }
ALLOC_HDMI_ERR_EXIT: 
    if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
    }
CREATE_SPDIF_ERR_EXIT:   
    if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_SPDIF_RAW]);
    }
ALLOC_SPDIF_ERR_EXIT:    
    HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_PCM]);  
CREATE_PCM_ERR_EXIT:
    HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_PCM]);    
ALLOC_PCM_ERR_EXIT:
    return HI_FAILURE;
}

HI_S32 TrackCreateSlave(SND_TRACK_STATE_S *state, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr, 
                        HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf)
{
    HI_S32 Ret;
    AOE_AIP_ID_E enAIP;
    AOE_AIP_CHN_ATTR_S stAipAttr;
    MMZ_BUFFER_S stRbfMmz;

    if (HI_FALSE == bAlsaTrack)
    {
        Ret = HI_DRV_MMZ_AllocAndMap("AO_SAipPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                     &stRbfMmz);
        if (HI_SUCCESS != Ret)
        {
            return HI_FAILURE;
        }

        TrackGetAipPcmDfAttr(&stAipAttr, &stRbfMmz, pstAttr);
        Ret = HAL_AOE_AIP_Create(&enAIP, &stAipAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_DRV_MMZ_UnmapAndRelease(&stRbfMmz);
            return HI_FAILURE;
        }

        state->enAIP[SND_ENGINE_TYPE_PCM] = enAIP;
        state->stAipRbfMmz[SND_ENGINE_TYPE_PCM] = stRbfMmz;
        state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_PCM] = HI_FALSE;
        state->enCurnStatus = SND_TRACK_STATUS_STOP;
    }
    else
    {
        stRbfMmz.u32StartPhyAddr = pstBuf->u32BufPhyAddr;
        stRbfMmz.u32StartVirAddr = pstBuf->u32BufVirAddr;
        stRbfMmz.u32Size = pstBuf->u32BufSize;
#if  0
        TRP(stRbfMmz.u32StartPhyAddr);
        TRP(stRbfMmz.u32StartVirAddr);
        TRP(stRbfMmz.u32Size);
#endif
        TrackGetAipPcmDfAttr(&stAipAttr, &stRbfMmz, pstAttr);
        stAipAttr.stBufInAttr.bAlsaEnable = HI_TRUE;
        Ret = HAL_AOE_AIP_Create(&enAIP, &stAipAttr);
        if (HI_SUCCESS != Ret)
        {
            return HI_FAILURE;
        }

        state->enAIP[SND_ENGINE_TYPE_PCM] = enAIP;
        state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_PCM] = HI_TRUE;
        state->enCurnStatus = SND_TRACK_STATUS_STOP;
    }

    return HI_SUCCESS;
}

HI_VOID SndOpBing2Engine(SND_CARD_STATE_S *pCard, HI_HANDLE hEngine)
{
    HI_HANDLE hSndOp;
    HI_U32 op;
    AOE_AOP_ID_E enAOP;
    SND_ENGINE_TYPE_E enEngineType = TrackGetEngineType(hEngine);
	SND_ENGINE_STATE_S *state = (SND_ENGINE_STATE_S *)hEngine;

    for (op = 0; op < HI_UNF_SND_OUTPUTPORT_MAX; op++)
    {
        if(pCard->hSndOp[op])
        {
            hSndOp = pCard->hSndOp[op];
            if (enEngineType == SND_GetOpGetOutType(hSndOp))
            {
                enAOP = SND_OpGetAopId(hSndOp);
				HAL_AOE_ENGINE_AttachAop(state->enEngine, enAOP);
            }
        }
    }
}

HI_VOID TrackBing2Engine(SND_CARD_STATE_S *pCard, HI_HANDLE hSndTrack)
{
    AOE_AIP_CHN_ATTR_S stAipAttr;
    AOE_ENGINE_CHN_ATTR_S stEngineAttr;
    HI_HANDLE hEngine;
    SND_TRACK_STATE_S *state = (SND_TRACK_STATE_S *)hSndTrack;
    SND_ENGINE_STATE_S *pstEnginestate;

    if (pCard->hSndEngine[SND_ENGINE_TYPE_PCM])
    {
        hEngine = pCard->hSndEngine[SND_ENGINE_TYPE_PCM];
        pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;//verify
        HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_PCM]);
    }
    else
    {
        HAL_AOE_AIP_GetAttr(state->enAIP[SND_ENGINE_TYPE_PCM], &stAipAttr);
        stEngineAttr.u32BitPerSample = stAipAttr.stFifoOutAttr.u32FifoBitPerSample;
        stEngineAttr.u32Channels   = stAipAttr.stFifoOutAttr.u32FifoChannels;
        stEngineAttr.u32SampleRate = stAipAttr.stFifoOutAttr.u32FifoSampleRate;
        stEngineAttr.u32DataFormat = stAipAttr.stFifoOutAttr.u32FifoDataFormat;

        TrackCreateEngine(&hEngine, &stEngineAttr, SND_ENGINE_TYPE_PCM);
        pCard->hSndEngine[SND_ENGINE_TYPE_PCM] = hEngine;
        pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;//verify
        HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_PCM]);
        HAL_AOE_ENGINE_Start(pstEnginestate->enEngine);
        SndOpBing2Engine(pCard, hEngine);
    }

    if (HI_UNF_SND_TRACK_TYPE_MASTER == state->stUserTrackAttr.enTrackType)
    {
        if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
        {
            if (pCard->hSndEngine[SND_ENGINE_TYPE_SPDIF_RAW])
            {
                hEngine = pCard->hSndEngine[SND_ENGINE_TYPE_SPDIF_RAW];
                pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;//verify
                HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
            }
            else
            {
                HAL_AOE_AIP_GetAttr(state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW], &stAipAttr);
                stEngineAttr.u32BitPerSample = stAipAttr.stFifoOutAttr.u32FifoBitPerSample;
                stEngineAttr.u32Channels   = stAipAttr.stFifoOutAttr.u32FifoChannels;
                stEngineAttr.u32SampleRate = stAipAttr.stFifoOutAttr.u32FifoSampleRate;
                stEngineAttr.u32DataFormat = stAipAttr.stFifoOutAttr.u32FifoDataFormat;
                TrackCreateEngine(&hEngine, &stEngineAttr, SND_ENGINE_TYPE_SPDIF_RAW);
                pCard->hSndEngine[SND_ENGINE_TYPE_SPDIF_RAW] = hEngine;
                pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;
                HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
                HAL_AOE_ENGINE_Start(pstEnginestate->enEngine);
                SndOpBing2Engine(pCard, hEngine);
            }
        }

        if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
        {
            if (pCard->hSndEngine[SND_ENGINE_TYPE_HDMI_RAW])
            {
                hEngine = pCard->hSndEngine[SND_ENGINE_TYPE_HDMI_RAW];
                pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;
                HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            }
            else
            {
                HAL_AOE_AIP_GetAttr(state->enAIP[SND_ENGINE_TYPE_HDMI_RAW], &stAipAttr);
                stEngineAttr.u32BitPerSample = stAipAttr.stFifoOutAttr.u32FifoBitPerSample;
                stEngineAttr.u32Channels   = stAipAttr.stFifoOutAttr.u32FifoChannels;
                stEngineAttr.u32SampleRate = stAipAttr.stFifoOutAttr.u32FifoSampleRate;
                stEngineAttr.u32DataFormat = stAipAttr.stFifoOutAttr.u32FifoDataFormat;
                TrackCreateEngine(&hEngine, &stEngineAttr, SND_ENGINE_TYPE_HDMI_RAW);
                pCard->hSndEngine[SND_ENGINE_TYPE_HDMI_RAW] = hEngine;
                pstEnginestate = (SND_ENGINE_STATE_S *)hEngine;
                HAL_AOE_ENGINE_AttachAip(pstEnginestate->enEngine, state->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
                HAL_AOE_ENGINE_Start(pstEnginestate->enEngine);
                SndOpBing2Engine(pCard, hEngine);
            }
        }
    }
}

/******************************AO Track FUNC*************************************/
HI_U32 TRACK_GetMasterId(SND_CARD_STATE_S *pCard)
{
    HI_U32 TrackId;
    SND_TRACK_STATE_S *state;

    for (TrackId = 0; TrackId < AO_MAX_TOTAL_TRACK_NUM; TrackId++)
    {
        if (pCard->uSndTrackInitFlag & ((HI_U32)1L << TrackId))
        {
            state = (SND_TRACK_STATE_S *)pCard->hSndTrack[TrackId];
            if (HI_UNF_SND_TRACK_TYPE_MASTER == state->stUserTrackAttr.enTrackType)
            {
                return TrackId;
            }
        }
    }

    return AO_MAX_TOTAL_TRACK_NUM;
}

#if !defined(HI_SND_DRV_TEMPLATE_OPT)
HI_S32 TRACK_Create(SND_CARD_STATE_S *pCard, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr,
                    HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf, HI_HANDLE *phTrack)
{
    SND_TRACK_STATE_S *state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    HI_U32 TrackId;

    if (!pCard)
    {
        return HI_FAILURE;
    }

    *phTrack = HI_NULL;
    TrackId = TRACK_RS_GetFreeId();
    if (TrackId >= AO_MAX_TOTAL_TRACK_NUM)
    {
        HI_FATAL_AO("SndTrack source is not enough\n");
        return HI_FAILURE;
    }

    state = HI_KMALLOC(HI_ID_AO, sizeof(SND_TRACK_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("HI_KMALLOC TRACK_Create failed\n");
        goto SndTrackCreate_ERR_EXIT;
    }

    memset(state, 0, sizeof(SND_TRACK_STATE_S));

    switch (pstAttr->enTrackType)
    {
    case HI_UNF_SND_TRACK_TYPE_MASTER:
        if (AO_MAX_TOTAL_TRACK_NUM != TRACK_GetMasterId(pCard))  //judge if master track exist
        {
            goto SndTrackCreate_ERR_EXIT;
        }
        
        if (HI_SUCCESS != TrackCreateMaster(state, pstAttr))
        {
            goto SndTrackCreate_ERR_EXIT;
        }

        break;

    case HI_UNF_SND_TRACK_TYPE_SLAVE:
        if (HI_SUCCESS != TrackCreateSlave(state, pstAttr, bAlsaTrack, pstBuf))
        {
            goto SndTrackCreate_ERR_EXIT;
        }

        break;

    default:
        HI_ERR_AO("dont support tracktype(%d)\n ", pstAttr->enTrackType);
        goto SndTrackCreate_ERR_EXIT;
    }

    state->TrackId = TrackId;
    memcpy(&state->stUserTrackAttr, pstAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    state->stTrackGain.bLinearMode = HI_TRUE;
    state->stTrackGain.s32Gain = AO_MAX_LINEARVOLUME;
    state->u32SendTryCnt = 0;
    state->u32SendCnt = 0;
    *phTrack = TrackId;
    pCard->hSndTrack[TrackId] = (HI_HANDLE)state;
    pCard->uSndTrackInitFlag |= ((HI_U32)1L << TrackId);
    TRACK_RS_RegisterId(TrackId);
    TrackBing2Engine(pCard, pCard->hSndTrack[TrackId]);

    return HI_SUCCESS;
    
SndTrackCreate_ERR_EXIT:
    *phTrack = HI_NULL;
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}
#endif

#if defined(HI_SND_DRV_TEMPLATE_OPT)
HI_S32 TRACK_CreateNew(SND_CARD_STATE_S *pCard, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr,
                    HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf, HI_U32 TrackId)
{
    SND_TRACK_STATE_S *state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    if (!pCard)
    {
        HI_ERR_AO("pCard is null!\n");
        return HI_FAILURE;
    }

    if(pstAttr->enTrackType >= HI_UNF_SND_TRACK_TYPE_BUTT)
    {
        HI_ERR_AO("dont support tracktype(%d)\n",pstAttr->enTrackType);
        return HI_FAILURE;
    }

    if(pstAttr->u32BufLevelMs < AO_TRACK_MASTER_MIN_BUFLEVELMS || pstAttr->u32BufLevelMs > AO_TRACK_MASTER_MAX_BUFLEVELMS)
    {
        HI_ERR_AO("Invalid u32BufLevelMs(%d), Min(%d), Max(%d)\n",pstAttr->u32BufLevelMs, 
            AO_TRACK_MASTER_MIN_BUFLEVELMS, AO_TRACK_MASTER_MAX_BUFLEVELMS);
        return HI_FAILURE;
    }


    state = HI_KMALLOC(HI_ID_AO, sizeof(SND_TRACK_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("HI_KMALLOC TRACK_Create failed\n");
        goto SndTrackCreate_ERR_EXIT;
    }

    memset(state, 0, sizeof(SND_TRACK_STATE_S));

    switch (pstAttr->enTrackType)
    {
    case HI_UNF_SND_TRACK_TYPE_MASTER:
        if (AO_MAX_TOTAL_TRACK_NUM != TRACK_GetMasterId(pCard))  //judge if master track exist
        {
            HI_ERR_AO("Master track exist!\n");
            goto SndTrackCreate_ERR_EXIT;
        }
        
        if (HI_SUCCESS != TrackCreateMaster(pCard, state, pstAttr))
        {
            goto SndTrackCreate_ERR_EXIT;
        }

        break;

    case HI_UNF_SND_TRACK_TYPE_SLAVE:
        if (HI_SUCCESS != TrackCreateSlave(state, pstAttr, bAlsaTrack, pstBuf))
        {
            goto SndTrackCreate_ERR_EXIT;
        }

        break;

    default:
        HI_ERR_AO("dont support tracktype(%d)\n ", pstAttr->enTrackType);
        goto SndTrackCreate_ERR_EXIT;
    }

    state->TrackId = TrackId;
    memcpy(&state->stUserTrackAttr, pstAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    state->stTrackGain.bLinearMode = HI_TRUE;
    state->stTrackGain.s32Gain = AO_MAX_LINEARVOLUME;
    state->u32SendTryCnt = 0;
    state->u32SendCnt = 0;
    state->u32AddMuteFrameNum = 0;
    state->bEosFlag = HI_FALSE;
    state->bAlsaTrack = bAlsaTrack;
    pCard->hSndTrack[TrackId] = (HI_HANDLE)state;
    pCard->uSndTrackInitFlag |= ((HI_U32)1L << TrackId);
    TrackBing2Engine(pCard, pCard->hSndTrack[TrackId]);

    return HI_SUCCESS;
    
SndTrackCreate_ERR_EXIT:
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}
#endif


HI_S32 TRACK_Destroy(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID)
{
    SND_TRACK_STATE_S *state;



    state = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (HI_NULL == state)
    {
        return HI_FAILURE;
    }

    switch (state->stUserTrackAttr.enTrackType)
    {
    case HI_UNF_SND_TRACK_TYPE_MASTER:
        HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_PCM]);
        HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_PCM]);

        if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
        {
            HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
            HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_SPDIF_RAW]);
        }

        if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
        {
            HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_HDMI_RAW]);
        }
        break;

    case HI_UNF_SND_TRACK_TYPE_SLAVE:

        HAL_AOE_AIP_Destroy(state->enAIP[SND_ENGINE_TYPE_PCM]);

        if (HI_TRUE != state->bAipRbfExtDmaMem[SND_ENGINE_TYPE_PCM])
        {
            HI_DRV_MMZ_UnmapAndRelease(&state->stAipRbfMmz[SND_ENGINE_TYPE_PCM]);
        }

        break;

    default:
        break;
    }

    pCard->uSndTrackInitFlag &= ~((HI_U32)1L << state->TrackId);
#if !defined(HI_SND_DRV_TEMPLATE_OPT)
    TRACK_RS_DeRegisterId(state->TrackId);
#endif
    pCard->hSndTrack[state->TrackId] = HI_NULL;
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return HI_SUCCESS;
}

HI_S32 TRACK_SendData(SND_CARD_STATE_S *pCard,HI_U32 u32TrackID, HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    SND_TRACK_STATE_S *pTrack;
    SND_TRACK_STREAM_ATTR_S stStreamAttr;
    STREAMMODE_CHANGE_ATTR_S stChange;

    if (HI_NULL == pstAOFrame)
    {
        return HI_FAILURE;
    }

    //todo , check attr
    
    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    //Ao track state check
    if(SND_TRACK_STATUS_BUTT <= pTrack->enCurnStatus)
    {
        HI_ERR_AO("Invalid ao track status\n");
        return HI_FAILURE;
    }
    if (SND_TRACK_STATUS_PAUSE == pTrack->enCurnStatus)
    {
        return HI_ERR_AO_PAUSE_STATE;
    }
    if (SND_TRACK_STATUS_STOP == pTrack->enCurnStatus)
    {
        HI_ERR_AO("Ao track stop status, can't send data\n");
        return HI_FAILURE;
    }

    TRACKDbgCountTrySendData(pTrack);

    TRACKBuildStreamAttr(pCard, (HI_UNF_AO_FRAMEINFO_S *)pstAOFrame, &stStreamAttr);
    DetectStreamModeChange(pCard, pTrack, &stStreamAttr, &stChange);
    
    TRACKStartAip(pTrack);

    if(HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        if (stChange.enPcmChange || stChange.enSpdifChange || stChange.enHdmiChnage)
        {
            HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
            if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
                HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
            if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
                HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
            
            SndProcPcmRoute(pCard, pTrack, stChange.enPcmChange, &stStreamAttr);
            if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
                SndProcSpidfRoute(pCard, pTrack, stChange.enSpdifChange, &stStreamAttr);
            if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
                SndProcHdmifRoute(pCard, pTrack, stChange.enHdmiChnage, &stStreamAttr);
            
            HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
            if(SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
                HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
            if(SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
                HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
        }
    }
    else if (HI_UNF_SND_TRACK_TYPE_SLAVE == pTrack->stUserTrackAttr.enTrackType)
    {
        if (stChange.enPcmChange)
        {
            HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
            SndProcPcmRoute(pCard, pTrack, stChange.enPcmChange, &stStreamAttr);
            HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
        }
    }
    else
    {
        //verify virtual 
    }

    if (HI_FALSE == TrackisBufFree(pCard, pTrack, &stStreamAttr))
    {
        return HI_ERR_AO_OUT_BUF_FULL;
    }

    TRACKWriteFrame(pCard, pTrack, &stStreamAttr);

    if(AO_SND_SPEEDADJUST_SRC == pTrack->enUserSpeedType && 0 > pTrack->s32UserSpeedRate)
    {
        TRACKWriteMuteFrame(pCard, pTrack, &stStreamAttr);
    }
    
    TRACKDbgCountSendData(pTrack);

    return HI_SUCCESS;
}

HI_S32 TRACK_Start(SND_CARD_STATE_S *pCard,HI_U32 u32TrackID)
{
    SND_TRACK_STATE_S *pTrack;



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (HI_NULL == pTrack)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (SND_TRACK_STATUS_START == pTrack->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if(HI_TRUE == pTrack->bAlsaTrack)
    {
        HAL_AOE_AIP_Start(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
    }

    pTrack->enCurnStatus = SND_TRACK_STATUS_START;

    return HI_SUCCESS;
}

HI_S32 TRACK_Stop(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID)
{
    SND_TRACK_STATE_S *pTrack;



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (HI_NULL == pTrack)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (SND_TRACK_STATUS_STOP == pTrack->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
        HAL_AOE_AIP_Stop(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
        if (SND_SPDIF_MODE_LBR == pCard->enSpdifPassthrough)
        {
            SndProcSpidfRoute(pCard, pTrack, TRACK_STREAMMODE_CHANGE_LBR2PCM, &pTrack->stStreamAttr);
        }

        if (SND_HDMI_MODE_LBR == pCard->enHdmiPassthrough)
        {
            SndProcHdmifRoute(pCard, pTrack, TRACK_STREAMMODE_CHANGE_LBR2PCM, &pTrack->stStreamAttr);
        }
        else if (SND_HDMI_MODE_HBR == pCard->enHdmiPassthrough)
        {
            SndProcHdmifRoute(pCard, pTrack, TRACK_STREAMMODE_CHANGE_HBR2PCM, &pTrack->stStreamAttr);
        }
    }

    pTrack->enCurnStatus = SND_TRACK_STATUS_STOP;

    return HI_SUCCESS;
}

HI_S32 TRACK_Pause(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID)
{
    SND_TRACK_STATE_S *pTrack;



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (HI_NULL == pTrack)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (SND_TRACK_STATUS_PAUSE == pTrack->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    HAL_AOE_AIP_Pause(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        HAL_AOE_AIP_Pause(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
        HAL_AOE_AIP_Pause(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
    }

    pTrack->enCurnStatus = SND_TRACK_STATUS_PAUSE;

    return HI_SUCCESS;
}

HI_S32 TRACK_Flush(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID)
{
    SND_TRACK_STATE_S *pTrack;



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    HAL_AOE_AIP_Flush(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);
    if (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType)
    {
        HAL_AOE_AIP_Flush(pTrack->enAIP[SND_ENGINE_TYPE_SPDIF_RAW]);
        HAL_AOE_AIP_Flush(pTrack->enAIP[SND_ENGINE_TYPE_HDMI_RAW]);
    }

    return HI_SUCCESS;
}

HI_S32 TRACK_SetAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr)
{
    SND_TRACK_STATE_S *pTrack;

    if (HI_NULL == pstTrackAttr)
    {
        return HI_FAILURE;
    }



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if (SND_TRACK_STATUS_START == pTrack->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if (pTrack->stUserTrackAttr.enTrackType == pstTrackAttr->enTrackType)
    {
        // todo
    }
    else
    {
        // todo
#if 0
        HI_U32 MasterId = TRACK_GetMasterId(snd);
        if (AO_MAX_TOTAL_TRACK_NUM != MasterId)
        {
            hSndTrack = pCard->hSndTrack[MasterId];
        }
#endif

    }

    //memcpy(&pTrack->stUserTrackAttr,pstTrackAttr,sizeof(HI_UNF_AUDIOTRACK_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 TRACK_SetWeight(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S *pstTrackGain)
{
    SND_TRACK_STATE_S *pTrack;
    HI_U32 u32dBReg;

    if (HI_NULL == pstTrackGain)
    {
        return HI_FAILURE;
    }

    if(HI_TRUE == pstTrackGain->bLinearMode)
    {
        CHECK_AO_LINEARVOLUME(pstTrackGain->s32Gain);
        u32dBReg = AUTIL_VolumeLinear2RegdB((HI_U32)pstTrackGain->s32Gain);
    }
    else
    {
        CHECK_AO_ABSLUTEVOLUME(pstTrackGain->s32Gain);
        u32dBReg = AUTIL_VolumedB2RegdB(pstTrackGain->s32Gain);
    }


    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    HAL_AOE_AIP_SetVolume(pTrack->enAIP[SND_ENGINE_TYPE_PCM], u32dBReg);

    pTrack->stTrackGain.bLinearMode = pstTrackGain->bLinearMode;
    pTrack->stTrackGain.s32Gain = pstTrackGain->s32Gain;

    return HI_SUCCESS;
}

HI_S32 TRACK_GetWeight(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S *pstTrackGain)
{
    SND_TRACK_STATE_S *pTrack;

    if (HI_NULL == pstTrackGain)
    {
        return HI_FAILURE;
    }



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    pstTrackGain->bLinearMode = pTrack->stTrackGain.bLinearMode;
    pstTrackGain->s32Gain = pTrack->stTrackGain.s32Gain;
    
    return HI_SUCCESS;
}


HI_S32 TRACK_SetSpeedAdjust(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, AO_SND_SPEEDADJUST_TYPE_E enType, HI_S32 s32Speed)
{
    SND_TRACK_STATE_S *pTrack;

    CHECK_AO_SPEEDADJUST(s32Speed);
    
    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    if(AO_SND_SPEEDADJUST_SRC == enType)
    {
        HAL_AOE_AIP_SetSpeed(pTrack->enAIP[SND_ENGINE_TYPE_PCM], s32Speed); //verify no pcm need speedadjust?
    }
    else if(AO_SND_SPEEDADJUST_MUTE == enType)
    {
        //verify  avplay not use
    }
    pTrack->enUserSpeedType  = enType;
    pTrack->s32UserSpeedRate = s32Speed;

    return HI_SUCCESS;
}

HI_S32 TRACK_GetDelayMs(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32DelayMs)
{
    SND_TRACK_STATE_S *pTrack;
    HI_U32 u32TrackDelayMs=0;
    HI_U32 u32TrackFiFoDelayMs=0;
    HI_U32 u32SndFiFoDelayMs=0;
        
    if (HI_NULL == pu32DelayMs)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &u32TrackDelayMs);
    HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &u32TrackFiFoDelayMs); // for aip fifo delay
    SND_GetDelayMs(pCard,&u32SndFiFoDelayMs);
    *pu32DelayMs = u32TrackDelayMs + u32TrackFiFoDelayMs + u32SndFiFoDelayMs;

    return HI_SUCCESS;
}

HI_S32 TRACK_IsBufEmpty(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL *pbBufEmpty)
{
    SND_TRACK_STATE_S *pTrack;
    HI_U32 u32TrackDelayMs=0;
    HI_U32 u32TrackFiFoDelayMs=0;
        
    if (HI_NULL == pbBufEmpty)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &u32TrackDelayMs);
    HAL_AOE_AIP_GetFiFoDelayMs(pTrack->enAIP[SND_ENGINE_TYPE_PCM], &u32TrackFiFoDelayMs); // for aip fifo delay
    if(u32TrackDelayMs + u32TrackFiFoDelayMs <= AO_TRACK_BUF_EMPTY_THRESHOLD_MS)
    {
        *pbBufEmpty = HI_TRUE;
    }
    else  
    {
        *pbBufEmpty = HI_FALSE;
    }

    return HI_SUCCESS;
}
HI_S32 TRACK_SetEosFlag(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL bEosFlag)
{
    SND_TRACK_STATE_S *pTrack;
    
    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    pTrack->bEosFlag = bEosFlag;
    
    return HI_SUCCESS;
}

HI_S32 TRACK_GetStatus(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_VOID *pstParam)
{
    //TO DO
    //s1:HAL_AOE_AOP_GetStatus
    return HI_SUCCESS;
}

HI_S32 TRACK_GetDefAttr(HI_UNF_AUDIOTRACK_ATTR_S * pstDefAttr)
{
    //zgjiere;u32OutputBufSize 目前被忽略，AIP按照最大能力创建，避免频繁内存操作
    switch (pstDefAttr->enTrackType)
    {
    case HI_UNF_SND_TRACK_TYPE_MASTER:
    {
        pstDefAttr->u32BufLevelMs = AO_TRACK_MASTER_DEFATTR_BUFLEVELMS;
        pstDefAttr->u32OutputBufSize = AO_TRACK_MASTER_DEFATTR_BUFSIZE;        //verify
        pstDefAttr->u32FadeinMs  = AO_TRACK_MASTER_DEFATTR_FADEINMS;
        pstDefAttr->u32FadeoutMs = AO_TRACK_MASTER_DEFATTR_FADEOUTMS;
        break;
    }

    case HI_UNF_SND_TRACK_TYPE_SLAVE:
    {
        pstDefAttr->u32BufLevelMs = AO_TRACK_SLAVE_DEFATTR_BUFLEVELMS;
        pstDefAttr->u32OutputBufSize = AO_TRACK_SLAVE_DEFATTR_BUFSIZE;        //verify
        pstDefAttr->u32FadeinMs  = AO_TRACK_SLAVE_DEFATTR_FADEINMS;
        pstDefAttr->u32FadeoutMs = AO_TRACK_SLAVE_DEFATTR_FADEOUTMS;
        break;
    }

    case HI_UNF_SND_TRACK_TYPE_VIRTUAL:
    {
        pstDefAttr->u32OutputBufSize = AO_TRACK_VIRTUAL_DEFATTR_BUFSIZE;
        break;
    }

    default:

        //todo
        HI_ERR_AO("Get DefaultTrackAttr failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 TRACK_GetAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr)
{
    SND_TRACK_STATE_S *pTrack;

    if (HI_NULL == pstTrackAttr)
    {
        return HI_FAILURE;
    }

    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    memcpy(pstTrackAttr, &pTrack->stUserTrackAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));//verify  TrackType
    
    return HI_SUCCESS;
}

#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
HI_S32 TRACK_GetSetting(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_TRACK_STATE_S *pTrack;
    
    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    pstSndSettings->enCurnStatus = pTrack->enCurnStatus;
    pstSndSettings->enType = pTrack->enUserSpeedType;
    pstSndSettings->s32Speed = pTrack->s32UserSpeedRate;
    memcpy(&pstSndSettings->stTrackGain, &pTrack->stTrackGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));
    memcpy(&pstSndSettings->stTrackAttr, &pTrack->stUserTrackAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    
    return HI_SUCCESS;
}

HI_S32 TRACK_RestoreSetting(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_TRACK_STATE_S *pTrack;
    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    if(SND_TRACK_STATUS_START == pstSndSettings->enCurnStatus)
    {
        TRACK_Start(pCard, u32TrackID);
    }
    else if(SND_TRACK_STATUS_STOP == pstSndSettings->enCurnStatus)
    {
        TRACK_Stop(pCard, u32TrackID);
    }
    else if(SND_TRACK_STATUS_PAUSE == pstSndSettings->enCurnStatus)
    {
        TRACK_Pause(pCard, u32TrackID);
    }

    TRACK_SetSpeedAdjust(pCard, u32TrackID, pstSndSettings->enType, pstSndSettings->s32Speed);
    TRACK_SetWeight(pCard, u32TrackID, &pstSndSettings->stTrackGain);
    
    return HI_SUCCESS;
}
#endif

HI_S32 Track_ShowProc( struct seq_file* p, SND_CARD_STATE_S *pCard )
{
    SND_TRACK_STATE_S *pTrack;
    SND_ENGINE_TYPE_E enEngine;
    AOE_AIP_CHN_ATTR_S stAipAttr;
    HI_U32 i;
    HI_U32 u32DelayMs;
    
    for(i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        if(pCard->uSndTrackInitFlag & ((HI_U32)1L << i))
        {
            pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[i];
            seq_printf(p,
               "Track(%d): Type(%s), Status(%s), Vol(%.3d%s), SpeedRate(%.2d), AddMuteFrames(%.4d), SendCnt(Try/OK)(%.6u/%.6u)\n",
                pTrack->TrackId,
               (HI_CHAR*)((HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType) ? "master" : ((HI_UNF_SND_TRACK_TYPE_SLAVE == pTrack->stUserTrackAttr.enTrackType) ? "slave" : "virtual")),
               (HI_CHAR*)((SND_TRACK_STATUS_START == pTrack->enCurnStatus) ? "start" : ((SND_TRACK_STATUS_STOP == pTrack->enCurnStatus) ? "stop" : "pause")),
                pTrack->stTrackGain.s32Gain,  
               (HI_TRUE == pTrack->stTrackGain.bLinearMode)?"":"dB",
                pTrack->s32UserSpeedRate,
                pTrack->u32AddMuteFrameNum,
                pTrack->u32SendTryCnt,
                pTrack->u32SendCnt);
            
            for(enEngine = SND_ENGINE_TYPE_PCM; enEngine < SND_ENGINE_TYPE_BUTT; enEngine++)
            {
                if((SND_ENGINE_TYPE_PCM == enEngine) || (HI_UNF_SND_TRACK_TYPE_MASTER == pTrack->stUserTrackAttr.enTrackType))
                {
                    if((SND_ENGINE_TYPE_SPDIF_RAW == enEngine && SND_SPDIF_MODE_PCM >= pCard->enSpdifPassthrough)
                        || (SND_ENGINE_TYPE_HDMI_RAW == enEngine && SND_HDMI_MODE_PCM >= pCard->enHdmiPassthrough))
                    {
                        continue;
                    }
                    HAL_AOE_AIP_GetAttr(pTrack->enAIP[enEngine], &stAipAttr);
                    seq_printf(p,
                       "AIP(%x): Engine(%s), Fs(%.6d), Ch(%.2d), Bit(%2d), Format(%s)\n",
                       (HI_U32)pTrack->enAIP[enEngine],
                        AUTIL_Engine2Name(enEngine),
                        stAipAttr.stBufInAttr.u32BufSampleRate,
                        stAipAttr.stBufInAttr.u32BufChannels,
                        stAipAttr.stBufInAttr.u32BufBitPerSample,
                        AUTIL_Format2Name(stAipAttr.stBufInAttr.u32BufDataFormat));

                    HAL_AOE_AIP_GetBufDelayMs(pTrack->enAIP[enEngine], &u32DelayMs);
                    seq_printf(p,
                       "        EmptyCnt(%.6u), EmptyWarningCnt(%.6u), Latency/Threshold(%.3dms/%.3dms)\n",
                        0,  //verify
                        0,  //verify
                        u32DelayMs,
                        stAipAttr.stBufInAttr.u32BufLatencyThdMs);
                 }  
            }
            seq_printf(p,"\n");
         }
    }
    
    return HI_SUCCESS;
}

HI_S32 TRACK_UpdateWptrPos(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32WptrLen)    //for alsa
{
    SND_TRACK_STATE_S *pTrack;
    /*
    SND_TRACK_STREAM_ATTR_S stStreamAttr;
    STREAMMODE_CHANGE_ATTR_S stChange;
    HI_U32 Write = 0;
    */
    if (HI_NULL == pu32WptrLen)
    {
        return HI_FAILURE;
    }

    //todo , check attr


    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    /*
    if (SND_TRACK_STATUS_START != pTrack->enCurnStatus)
    {
        HI_ERR_AO("Ao track status(%d) should be SND_TRACK_STATUS_START  \n", pTrack->enCurnStatus);
        //return HI_FAILURE;
    }
    */
    HAL_AOE_AIP_UpdateWritePos(pTrack->enAIP[SND_ENGINE_TYPE_PCM], pu32WptrLen);

    return  HI_SUCCESS;
}

HI_S32 TRACK_GetReadPos(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32ReadPos)    //for alsa
{
    SND_TRACK_STATE_S *pTrack;
    /*
    SND_TRACK_STREAM_ATTR_S stStreamAttr;
    STREAMMODE_CHANGE_ATTR_S stChange;
    HI_U32 Write = 0;
    */
    if (HI_NULL == pu32ReadPos)
    {
        return HI_FAILURE;
    }

    //todo , check attr


    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];
    /*
    if (SND_TRACK_STATUS_START != pTrack->enCurnStatus)
    {
        HI_ERR_AO("Ao track status(%d) should be SND_TRACK_STATUS_START  \n", pTrack->enCurnStatus);
        //return HI_FAILURE;
    }
    */
    HAL_AOE_AIP_GetReadPos(pTrack->enAIP[SND_ENGINE_TYPE_PCM], pu32ReadPos);

    return  HI_SUCCESS;
}


HI_S32 TRACK_FlushBuf(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID)    //for alsa
{
    SND_TRACK_STATE_S *pTrack;
    //todo , check attr



    pTrack = (SND_TRACK_STATE_S *)pCard->hSndTrack[u32TrackID];

    HAL_AOE_AIP_FlushBuf(pTrack->enAIP[SND_ENGINE_TYPE_PCM]);

    return  HI_SUCCESS;
}
