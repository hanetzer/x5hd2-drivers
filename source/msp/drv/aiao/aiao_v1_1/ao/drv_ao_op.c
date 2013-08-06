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

#include "drv_ao_op.h"
#include "audio_util.h"


HI_UNF_SND_OUTPUTPORT_E SndOpGetOutport(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enOutPort;
}

SND_OUTPUT_TYPE_E SndOpGetOutType(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enOutType;
}

HI_HANDLE SNDGetOpHandleByOutPort(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_S32 u32PortNum;
    HI_HANDLE hSndOp;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        hSndOp = pCard->hSndOp[u32PortNum];
        if (hSndOp)
        {
            if (enOutPort == SndOpGetOutport(hSndOp))
            {
                return hSndOp;
            }
        }
    }

    return HI_NULL;
}

SND_ENGINE_TYPE_E SND_GetOpGetOutType(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enEngineType[state->ActiveId];
}

HI_HANDLE SND_GetOpHandlebyOutType(SND_CARD_STATE_S *pCard, SND_OUTPUT_TYPE_E enOutType)
{
    HI_S32 u32PortNum;
    HI_HANDLE hSndOp;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        hSndOp = pCard->hSndOp[u32PortNum];
        if (hSndOp)
        {
            if (enOutType == SndOpGetOutType(hSndOp))
            {
                return hSndOp;
            }
        }
    }

    return HI_NULL;
}

HI_VOID SND_GetDelayMs(SND_CARD_STATE_S *pCard, HI_U32 *pdelayms)
{
    HI_HANDLE hSndOp;
    SND_OP_STATE_S *state;
    
    hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
    if(hSndOp)
    {
        state = (SND_OP_STATE_S *)hSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        return;
    }

    hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
    if(hSndOp)
    {
        state = (SND_OP_STATE_S *)hSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        return;
    }

    hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_SPDIF);
    if(hSndOp)
    {
        state = (SND_OP_STATE_S *)hSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        return;
    }
    
    hSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
    if(hSndOp)
    {
        state = (SND_OP_STATE_S *)hSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        return;
    }

    *pdelayms = 0;

    return ;
}

static HI_S32 SndOpCreateAop(SND_OP_STATE_S *state, HI_UNF_SND_OUTPORT_S *pstAttr, SND_AOP_TYPE_E enType)
{
    HI_S32 Ret;
    AIAO_PORT_USER_CFG_S stHwPortAttr;
    AIAO_PORT_ID_E enPort;
    AOE_AOP_ID_E enAOP;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    MMZ_BUFFER_S stRbfMmz;
    HI_U32 u32BufSize;
    AIAO_RBUF_ATTR_S stRbfAttr;
    HI_U32 u32AopId;
    HI_UNF_SND_OUTPUTPORT_E enOutPort;

    enOutPort = pstAttr->enOutPort;
    switch(enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
            enPort = AIAO_PORT_TX2;
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Adac", MMZ_OTHERS, AO_DAC_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;
            
        case HI_UNF_SND_OUTPUTPORT_I2S0:
            enPort = AIAO_PORT_TX0;
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            HI_ASSERT(u32BufSize < AO_I2S_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_I2s", MMZ_OTHERS, AO_I2S_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;  
            
        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            enPort = AIAO_PORT_SPDIF_TX1;
            HAL_AIAO_P_GetTxSpdDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            HI_ASSERT(u32BufSize < AO_SPDIF_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Spidf", MMZ_OTHERS, AO_SPDIF_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;
            
        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            if(SND_AOP_TYPE_I2S == enType)
            {
                enPort = AIAO_PORT_TX3;
                HAL_AIAO_P_GetHdmiI2SDfAttr(enPort, &stHwPortAttr);
                u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 32;  //verify considerate from 48k 2ch 16bit to 192k 8ch 32bit for example 24bit LPCM
                HI_ASSERT(u32BufSize < AO_HDMI_MMZSIZE_MAX);
                Ret = HI_DRV_MMZ_AllocAndMap("AO_HdmiI2s", MMZ_OTHERS, AO_HDMI_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            }
            else
            {
                enPort = AIAO_PORT_SPDIF_TX0;
                HAL_AIAO_P_GetTxSpdDfAttr(enPort, &stHwPortAttr);
                u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 4; //verify considerate frome 48k to 192k  for example ddp
                HI_ASSERT(u32BufSize < AO_SPDIF_MMZSIZE_MAX);
                Ret = HI_DRV_MMZ_AllocAndMap("AO_HdmiSpidf", MMZ_OTHERS, AO_SPDIF_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            }
            break;   
            
        default:
            HI_ERR_AO("Outport is invalid!\n");
            goto SndReturn_ERR_EXIT;
    }
    if (HI_SUCCESS != Ret)
    {
        goto SndReturn_ERR_EXIT;
    }

    stHwPortAttr.bExtDmaMem = HI_TRUE;
    stHwPortAttr.stExtMem.u32BufPhyAddr = stRbfMmz.u32StartPhyAddr;
    stHwPortAttr.stExtMem.u32BufVirAddr = stRbfMmz.u32StartVirAddr;
    stHwPortAttr.stExtMem.u32BufSize = u32BufSize;
    Ret = HAL_AIAO_P_Open(enPort, &stHwPortAttr);
    if (HI_SUCCESS != Ret)
    {
        goto SndMMZRelease_ERR_EXIT;
    }

    HAL_AIAO_P_GetRbfAttr(enPort, &stRbfAttr);
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyAddr = stRbfAttr.u32BufPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirAddr = stRbfAttr.u32BufVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyWptr = stRbfAttr.u32BufPhyWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirWptr = stRbfAttr.u32BufVirWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyRptr = stRbfAttr.u32BufPhyRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirRptr = stRbfAttr.u32BufVirRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag = 1;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize = stRbfAttr.u32BufSize;
    stAopAttr.stRbfOutAttr.u32BufBitPerSample = stHwPortAttr.stIfAttr.enBitDepth;
    stAopAttr.stRbfOutAttr.u32BufChannels   = stHwPortAttr.stIfAttr.enChNum;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = stHwPortAttr.stIfAttr.enRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = 0;
    stAopAttr.stRbfOutAttr.bRbfHwPriority = HI_TRUE;
    stAopAttr.stRbfOutAttr.u32BufLatencyThdMs = AOE_AOP_BUFF_LATENCYMS_DF;
    Ret = HAL_AOE_AOP_Create(&enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
       goto SndClosePort_ERR_EXIT;
    }

    if(HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort || SND_AOP_TYPE_SPDIF != enType)  //spdif interface of hdmi don't start aop
    {
        Ret = HAL_AIAO_P_Start(enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPort);
            goto SndDestroyAOP_ERR_EXIT;
        }

        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            goto SndStopPort_ERR_EXIT;
        }
    }

    u32AopId = (HI_U32)enType;
    state->u32OpMask |= 1 << u32AopId;
    if(HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort || SND_AOP_TYPE_SPDIF != enType)  //spdif interface of hdmi is not active
    {
        state->ActiveId  = u32AopId;
    }
    state->enPortID[u32AopId] = enPort;
    state->enAOP[u32AopId] = enAOP;
    state->stRbfMmz[u32AopId]   = stRbfMmz;
    state->stPortUserAttr[u32AopId] = stHwPortAttr;
    state->enEngineType[u32AopId] = SND_ENGINE_TYPE_PCM;
    return HI_SUCCESS;
    
SndStopPort_ERR_EXIT:
    HAL_AIAO_P_Stop(enPort, AIAO_STOP_IMMEDIATE);
SndDestroyAOP_ERR_EXIT:
    HAL_AOE_AOP_Destroy(enAOP);
SndClosePort_ERR_EXIT:
    HAL_AIAO_P_Close(enPort);
SndMMZRelease_ERR_EXIT:
    HI_DRV_MMZ_UnmapAndRelease(&stRbfMmz);
SndReturn_ERR_EXIT:
    return HI_FAILURE;
}

SND_ENGINE_TYPE_E SND_OpGetEngineType(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enEngineType[state->ActiveId];
}
#ifdef HI_SND_CAST_SUPPORT
static HI_S32 SndOpCreateCast(HI_HANDLE *phSndOp, HI_HANDLE *phCast,  HI_UNF_SND_CAST_ATTR_S *pstUserCastAttr, HI_U32 *pu32PhyAddr)
{
    HI_S32 Ret;
    AOE_AOP_ID_E enAOP;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    MMZ_BUFFER_S stRbfMmz;
    HI_U32 uBufSize, uFrameSize;
    AIAO_CAST_ATTR_S stCastAttr;
    AIAO_CAST_ID_E enCast;
    AIAO_CAST_RBUF_ATTR_S stRbfAttr;
    SND_OP_STATE_S *state = HI_NULL;
    HI_UNF_SND_CAST_ATTR_S stUserCastAttr;
    
    state = HI_KMALLOC(HI_ID_AO, sizeof(SND_OP_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AIAO("HI_KMALLOC SndOpCreate failed\n");
        return HI_FAILURE;
    }
    memset(state, 0, sizeof(SND_OP_STATE_S));
    
#if 1    
    memcpy(&stUserCastAttr, pstUserCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));

    stCastAttr.u32BufChannels = 2;
    stCastAttr.u32BufBitPerSample = 16;
    stCastAttr.u32BufSampleRate = 48000;
#endif    
    //stCastAttr->u32BufLatencyThdMs =  stUserCastAttr.u32LatencyThdMs;;

    uFrameSize = AUTIL_CalcFrameSize(stCastAttr.u32BufChannels, stCastAttr.u32BufBitPerSample);
    //uBufSize = AUTIL_LatencyMs2ByteSize(stCastAttr.u32BufLatencyThdMs, uFrameSize, stCastAttr.u32BufSampleRate);
    uBufSize = stUserCastAttr.u32PcmFrameMaxNum * stUserCastAttr.u32PcmSamplesPerFrame * uFrameSize;


    stCastAttr.u32BufLatencyThdMs = AUTIL_ByteSize2LatencyMs(uBufSize, uFrameSize, stCastAttr.u32BufSampleRate);
    
#if 0    
    HI_ERR_AO("stCastAttr->u32BufChannels = 0x%x\n", stCastAttr.u32BufChannels);
    HI_ERR_AO("stCastAttr->s32BitPerSample = 0x%x\n", stCastAttr.u32BufBitPerSample);
    HI_ERR_AO("stCastAttr->u32BufSampleRate = 0x%x\n", stCastAttr.u32BufSampleRate);
    HI_ERR_AO("stCastAttr->u32BufLatencyThdMs = 0x%x\n", stCastAttr.u32BufLatencyThdMs);

    HI_ERR_AO("uBufSize(0x%x) = u32PcmFrameMaxNum(0x%x), * u32PcmSamplesPerFrame(0x%x) * uFrameSize(0x%x)\n", 
            uBufSize, stUserCastAttr.u32PcmFrameMaxNum, stUserCastAttr.u32PcmSamplesPerFrame, uFrameSize);
#endif


    HI_ASSERT((uBufSize + CAST_DMABUF_RESERVED_SIZE)< AO_CAST_MMZSIZE_MAX);
    Ret = HI_DRV_MMZ_AllocAndMap("sound_cast", MMZ_OTHERS, AO_CAST_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    stCastAttr.extDmaMem.u32BufPhyAddr = stRbfMmz.u32StartPhyAddr;
    stCastAttr.extDmaMem.u32BufVirAddr = stRbfMmz.u32StartVirAddr;
    stCastAttr.extDmaMem.u32BufSize = uBufSize;
    Ret = HAL_CAST_Create(&enCast, &stCastAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_MMZ_UnmapAndRelease(&stRbfMmz);
        return HI_FAILURE;
    }

    HAL_CAST_GetRbfAttr(enCast, &stRbfAttr);
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyAddr = stRbfAttr.u32BufPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirAddr = stRbfAttr.u32BufVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyWptr = stRbfAttr.u32BufPhyWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirWptr = stRbfAttr.u32BufVirWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyRptr = stRbfAttr.u32BufPhyRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirRptr = stRbfAttr.u32BufVirRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag = 1;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize = stRbfAttr.u32BufSize;
    stAopAttr.stRbfOutAttr.u32BufBitPerSample = stCastAttr.u32BufBitPerSample;
    stAopAttr.stRbfOutAttr.u32BufChannels   = stCastAttr.u32BufChannels;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = stCastAttr.u32BufSampleRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = 0;
#if 1    
    stAopAttr.stRbfOutAttr.bRbfHwPriority = HI_FALSE;
#else
    stAopAttr.stRbfOutAttr.bRbfHwPriority = HI_TRUE;
#endif
#if   0  
    stAopAttr.stRbfOutAttr.u32BufLatencyThdMs = stCastAttr.u32BufLatencyThdMs;
#else
    stAopAttr.stRbfOutAttr.u32BufLatencyThdMs = AOE_AOP_BUFF_LATENCYMS_DF;
#endif
    Ret = HAL_AOE_AOP_Create(&enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        HAL_CAST_Destroy(enCast);
        HI_DRV_MMZ_UnmapAndRelease(&stRbfMmz);
        return HI_FAILURE;
    }

    state->u32OpMask |= 1<<SND_AOP_TYPE_CAST;
    state->ActiveId = 0;
    state->enAOP[0] = enAOP;
    
    //state->stRbfMmz[0]  = stRbfMmz;
    //state->stCastAttr = stCastAttr;
    memcpy(&state->stRbfMmz[0], &stRbfMmz, sizeof(MMZ_BUFFER_S));
    memcpy(&state->stCastAttr, &stCastAttr, sizeof(AIAO_CAST_ATTR_S));
    
    state->CastId = enCast;
    state->enEngineType[0] = SND_ENGINE_TYPE_PCM;
    state->enCurnStatus = SND_OP_STATUS_STOP;
    state->enOutType = SND_OUTPUT_TYPE_CAST;

    //HI_ERR_AO("Cast enAOP = 0x%x\n", enAOP);
    *pu32PhyAddr = stRbfAttr.u32BufPhyAddr;
    //HI_ERR_AO("pstUserCastAttr->u32PhyAddr = 0x%x\n", pstUserCastAttr->u32PhyAddr);

    *phCast = (HI_HANDLE )enCast;
    *phSndOp = (HI_HANDLE )state;
    
    return HI_SUCCESS;
}
#endif

static HI_VOID SndOpGetSubFormatAttr(SND_OP_STATE_S *state, SND_OP_ATTR_S *pstSndPortAttr)
{
    AOE_AOP_CHN_ATTR_S stAopAttr;
    AOE_AOP_OUTBUF_ATTR_S *pstAttr;
    AIAO_BufAttr_S *pstAiAOBufAttr;
    
    HAL_AOE_AOP_GetAttr(state->enAOP[state->ActiveId], &stAopAttr);
    pstAttr = &stAopAttr.stRbfOutAttr;
    pstAiAOBufAttr = &state->stPortUserAttr[state->ActiveId].stBufConfig;

    pstSndPortAttr->u32Channels     = pstAttr->u32BufChannels;
    pstSndPortAttr->u32SampleRate   = pstAttr->u32BufSampleRate;
    pstSndPortAttr->u32BitPerSample = pstAttr->u32BufBitPerSample;
    pstSndPortAttr->u32DataFormat   = pstAttr->u32BufDataFormat;
    pstSndPortAttr->u32LatencyThdMs = pstAttr->u32BufLatencyThdMs;
    if (SND_OUTPUT_TYPE_CAST != state->enOutType)
    {
        pstSndPortAttr->u32PeriodBufSize = pstAiAOBufAttr->u32PeriodBufSize;
        pstSndPortAttr->u32PeriodNumber  = pstAiAOBufAttr->u32PeriodNumber;
    }
    return;
}

/*
//zgjiere, unf proc ÖÐ¶Ï£¬×Ô¶¯×¢²á AIAO_IsrFunc      *pIsrFunc;
//zgjiere, alsa ÖÐ¶ÏÈçºÎ×¢²á£¬alsaÖÐ¶ÏÔÚtrackÈ·¶¨£¬aiaoÖÐ¶ÏÔÚport´ò¿ªÈ·¶¨?
1) alsaÄÜ·ñ²ÉÓÃDSP2ARMÖÐ¶Ï?
2) ÔËÐÐ¹ý³Ì£¬ÐÞ¸ÄÖÐ¶Ï·þÎñ³ÌÐòÏ
 */

HI_S32 SndOpStart(HI_HANDLE hSndOp, HI_VOID *pstParams)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    AOE_AOP_ID_E enAOP = state->enAOP[state->ActiveId];
    HI_S32 Ret;

    if (SND_OP_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
#ifdef HI_SND_CAST_SUPPORT
        Ret = HAL_CAST_Start(state->CastId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_CAST_Start(%d) failed\n", state->CastId);
            return HI_FAILURE;
        }
        
        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
#endif        
    }
    else
    {
        Ret = HAL_AIAO_P_Start(enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPort);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }

    state->enCurnStatus = SND_OP_STATUS_START;

    return HI_SUCCESS;
}

HI_S32 SndOpStop(HI_HANDLE hSndOp, HI_VOID *pstParams)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    AOE_AOP_ID_E enAOP = state->enAOP[state->ActiveId];
    HI_S32 Ret;

    if (SND_OP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
#ifdef HI_SND_CAST_SUPPORT
        Ret = HAL_CAST_Stop(state->CastId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_CAST_Stop(%d) failed\n", state->CastId);
            return HI_FAILURE;
        }
        
        Ret = HAL_AOE_AOP_Stop(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Stop(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
#endif
    }
    else
    {
        Ret = HAL_AIAO_P_Stop(enPort, AIAO_STOP_IMMEDIATE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Stop(%d) failed\n", enPort);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Stop(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Stop(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }
    state->enCurnStatus = SND_OP_STATUS_STOP;
    return HI_SUCCESS;
}

static HI_VOID SndOpDestroyAop(SND_OP_STATE_S *state, SND_AOP_TYPE_E enType)
{
    HAL_AOE_AOP_Destroy(state->enAOP[enType]);
    HAL_AIAO_P_Close(state->enPortID[enType]);
    HI_DRV_MMZ_UnmapAndRelease(&state->stRbfMmz[enType]);
    return;
}

static HI_VOID SndOpDestroy(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    SND_AOP_TYPE_E type;

    if(0 == state->u32OpMask)
    {
        return;
    }

    SndOpStop(hSndOp, HI_NULL);

    if(state->enOutType == SND_OUTPUT_TYPE_CAST)
    {
#ifdef HI_SND_CAST_SUPPORT
        HAL_AOE_AOP_Destroy(state->enAOP[0]);
        HAL_CAST_Destroy(state->CastId);
        HI_DRV_MMZ_UnmapAndRelease(&state->stRbfMmz[0]);
#endif
    }
    else
    {
        for(type = SND_AOP_TYPE_I2S; type < SND_AOP_TYPE_CAST; type++)
        {
            if (state->u32OpMask & (1 << type))
            {
                SndOpDestroyAop(state, type);
            }
        }
        if(state->enOutType == SND_OUTPUT_TYPE_DAC)
        {
            ADAC_TIANLAI_DeInit();
        }
    }

    memset(state, 0, sizeof(SND_OP_STATE_S));
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return;
}

static HI_VOID SndOpInitState(SND_OP_STATE_S *state)
{
    HI_U32 idx;
    
    memset(state, 0, sizeof(SND_OP_STATE_S));
    for(idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        state->enPortID[idx] = AIAO_PORT_BUTT;
        state->enAOP[idx] = AOE_AOP_BUTT;
        state->enEngineType[idx] = SND_ENGINE_TYPE_BUTT;
    }
    
    return;
}
static HI_S32 SndOpCreate(HI_HANDLE *phSndOp, HI_UNF_SND_OUTPORT_S *pstAttr)
{
    SND_OP_STATE_S *state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    SND_AOP_TYPE_E AopType;

    state = HI_KMALLOC(HI_ID_AO, sizeof(SND_OP_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AIAO("HI_KMALLOC SndOpCreate failed\n");
        return HI_FAILURE;
    }

    SndOpInitState(state);

    for(AopType = SND_AOP_TYPE_I2S; AopType < SND_AOP_TYPE_CAST; AopType++)
    {
        if(SND_AOP_TYPE_I2S == AopType && HI_UNF_SND_OUTPUTPORT_SPDIF0 == pstAttr->enOutPort)
        {
            continue;
        }

        if(SND_AOP_TYPE_SPDIF == AopType && HI_UNF_SND_OUTPUTPORT_SPDIF0 != pstAttr->enOutPort && HI_UNF_SND_OUTPUTPORT_HDMI0 != pstAttr->enOutPort)
        {
            continue;
        }
        
        if(HI_SUCCESS != SndOpCreateAop(state, pstAttr, AopType))
        {
            goto SndCreatePort_ERR_EXIT;
        }
    }

    SndOpGetSubFormatAttr(state, &state->stSndPortAttr);
    switch (pstAttr->enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_DAC0:
        memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_UNF_SND_DAC_ATTR_S));
        state->enOutType = SND_OUTPUT_TYPE_DAC;
        //init tianlai
        ADAC_TIANLAI_Init(state->stSndPortAttr.u32SampleRate);
        break;
        
    case HI_UNF_SND_OUTPUTPORT_I2S0:
        memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_UNF_SND_I2S_ATTR_S));
        state->enOutType = SND_OUTPUT_TYPE_I2S;
        break;
        
    case HI_UNF_SND_OUTPUTPORT_SPDIF0:
        memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_UNF_SND_SPDIF_ATTR_S));
        state->enOutType = SND_OUTPUT_TYPE_SPDIF;
        break;

    case HI_UNF_SND_OUTPUTPORT_HDMI0:
        memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_UNF_SND_HDMI_ATTR_S));
        state->enOutType = SND_OUTPUT_TYPE_HDMI;
        break;

    default:
        HI_ERR_AIAO("Err OutPort Type!\n");
        goto SndCreatePort_ERR_EXIT;
    }

    state->enOutPort = pstAttr->enOutPort;
    state->enCurnStatus = SND_OP_STATUS_START;
    state->u32UserMute = 0;
    state->enUserTrackMode = HI_UNF_TRACK_MODE_STEREO;
    state->stUserGain.bLinearMode = HI_FALSE;
    state->stUserGain.s32Gain = 0;
    *phSndOp = (HI_HANDLE)state;

    return HI_SUCCESS;
    
SndCreatePort_ERR_EXIT:
    *phSndOp = (HI_HANDLE)HI_NULL;
    SndOpDestroy((HI_HANDLE)state);
    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}

HI_S32 SndOpSetAttr(HI_HANDLE hSndOp, SND_OP_ATTR_S *pstSndPortAttr)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    HI_S32 Ret = HI_FAILURE;
    AIAO_PORT_ID_E enPortID;
    AOE_AOP_ID_E enAOP;
    AIAO_PORT_ATTR_S stAiaoAttr;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    AIAO_RBUF_ATTR_S stRbfAttr;

    if (SND_OP_STATUS_STOP != state->enCurnStatus)
    {
        return HI_FAILURE;
    }

#if defined(SND_CAST_SUPPORT) 
    /* note: noly spdif & hdmi support set attr as pass-through switch */
    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        return HI_FAILURE;
    }
#endif
    if (SND_OUTPUT_TYPE_DAC == state->enOutType)
    {
        return HI_FAILURE;
    }

    if (SND_OUTPUT_TYPE_I2S == state->enOutType)
    {
        return HI_FAILURE;
    }

    /* note: hdmi tx use spdif or i2s interface at diffrerent data format */
    if (SND_OUTPUT_TYPE_HDMI == state->enOutType)
    {
        if(!pstSndPortAttr->u32DataFormat)  
        {
            state->ActiveId = SND_AOP_TYPE_I2S;  //2.0 pcm
        }
        else if (AUTIL_isIEC61937Hbr(pstSndPortAttr->u32DataFormat, pstSndPortAttr->u32SampleRate))
        {
            state->ActiveId = SND_AOP_TYPE_I2S;       // hbr
            if (IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS == pstSndPortAttr->u32DataFormat)
            {
                state->ActiveId = SND_AOP_TYPE_SPDIF; // lbr or hbr(ddp)
            }
        }
        else if(IEC61937_DATATYPE_71_LPCM == pstSndPortAttr->u32DataFormat)
        {
            state->ActiveId = SND_AOP_TYPE_I2S;   //7.1 lpcm
        }
        else
        {
            state->ActiveId = SND_AOP_TYPE_SPDIF;     // lbr or hbr(ddp)
        }
    }

    enPortID = state->enPortID[state->ActiveId];
    enAOP = state->enAOP[state->ActiveId];

    HAL_AIAO_P_GetAttr(enPortID, &stAiaoAttr);
    HAL_AOE_AOP_GetAttr(enAOP, &stAopAttr);

    stAiaoAttr.stIfAttr.enBitDepth = (AIAO_BITDEPTH_E)pstSndPortAttr->u32BitPerSample;
    stAiaoAttr.stIfAttr.enChNum = (AIAO_I2S_CHNUM_E)pstSndPortAttr->u32Channels;
    stAiaoAttr.stIfAttr.enRate = (AIAO_SAMPLE_RATE_E)pstSndPortAttr->u32SampleRate;
    if (pstSndPortAttr->u32DataFormat)
    {
        AIAO_HAL_P_SetBypass(enPortID, HI_TRUE);
        if (2 == pstSndPortAttr->u32Channels)
        {
            state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_SPDIF_RAW;
        }
        else
        {
            state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_HDMI_RAW;
        }
    }
    else
    {
        AIAO_HAL_P_SetBypass(enPortID, HI_FALSE);
        state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_PCM;
    }

    //todo, optimize u32PeriodBufSize & u32PeriodNumber
    stAiaoAttr.stBufConfig.u32PeriodBufSize = pstSndPortAttr->u32PeriodBufSize;
    stAiaoAttr.stBufConfig.u32PeriodNumber = pstSndPortAttr->u32PeriodNumber;
    Ret = HAL_AIAO_P_SetAttr(enPortID, &stAiaoAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AO("HAL_AIAO_P_SetAttr port:0x%x\n", enPortID);
        return HI_FAILURE;
    }
    HAL_AIAO_P_GetRbfAttr(enPortID, &stRbfAttr);
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyAddr = stRbfAttr.u32BufPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirAddr = stRbfAttr.u32BufVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyWptr = stRbfAttr.u32BufPhyWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirWptr = stRbfAttr.u32BufVirWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufPhyRptr = stRbfAttr.u32BufPhyRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufVirRptr = stRbfAttr.u32BufVirRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize    = stRbfAttr.u32BufSize;

    stAopAttr.stRbfOutAttr.u32BufBitPerSample = pstSndPortAttr->u32BitPerSample;
    stAopAttr.stRbfOutAttr.u32BufChannels   = pstSndPortAttr->u32Channels;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = pstSndPortAttr->u32SampleRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = pstSndPortAttr->u32DataFormat;
    Ret = HAL_AOE_AOP_SetAttr(enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    state->stPortUserAttr[state->ActiveId].stIfAttr = stAiaoAttr.stIfAttr;
    state->stPortUserAttr[state->ActiveId].stBufConfig = stAiaoAttr.stBufConfig;
    memcpy(&state->stSndPortAttr, pstSndPortAttr, sizeof(SND_OP_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 SndOpGetAttr(HI_HANDLE hSndOp, SND_OP_ATTR_S *pstSndPortAttr)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    memcpy(pstSndPortAttr, &state->stSndPortAttr, sizeof(SND_OP_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 SndOpGetStatus(HI_HANDLE hSndOp, AIAO_PORT_STAUTS_S *pstPortStatus)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    HI_S32 Ret;

    Ret = HAL_AIAO_P_GetStatus(enPort, pstPortStatus);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_GetStatus (port:%d) failed\n", (HI_U32)enPort);
        return Ret;
    }

    return HI_SUCCESS;

}


AOE_AOP_ID_E SND_OpGetAopId(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enAOP[state->ActiveId];
}

HI_UNF_SND_OUTPUTPORT_E SND_GetOpOutputport(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;

    return state->enOutPort;
}

HI_S32 SndOpSetSampleRate(HI_HANDLE hSndOp, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];

    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast SampleRate
        return HI_SUCCESS;
    }

    Ret = HAL_AIAO_P_SetSampleRate(enPort, enSampleRate);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetSampleRate port:%d \n", enPort);
        return HI_FAILURE;
    }

    state->enSampleRate = enSampleRate;

    return HI_SUCCESS;
}

HI_S32 SndOpSetVolume(HI_HANDLE hSndOp, HI_UNF_SND_GAIN_ATTR_S stGain)
{
    HI_U32 idx;
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    HI_S32 Ret = HI_FAILURE;
    HI_U32 u32dBReg;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast Volume
        return HI_SUCCESS;
    }

    if (HI_TRUE == stGain.bLinearMode)
    {
        u32dBReg = AUTIL_VolumeLinear2RegdB((HI_U32)stGain.s32Gain);
    }
    else
    {
        u32dBReg = AUTIL_VolumedB2RegdB(stGain.s32Gain);
    }

    for(idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enPort = state->enPortID[idx];
        if(enPort < AIAO_PORT_BUTT)
        {
            Ret = HAL_AIAO_P_SetVolume(enPort, u32dBReg);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_AIAO("HAL_AIAO_P_SetVolume port:%d, VolunedB: %d\n", (HI_U32)enPort, stGain.s32Gain);
                return HI_FAILURE;
            }
        }
    }

    state->stUserGain.bLinearMode = stGain.bLinearMode;
    state->stUserGain.s32Gain = stGain.s32Gain;

    return HI_SUCCESS;
}

HI_S32 SndOpSetTrackMode(HI_HANDLE hSndOp, HI_UNF_TRACK_MODE_E enMode)
{
    HI_U32 idx;
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast TrackMode
        return HI_SUCCESS;
    }

    for(idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enPort = state->enPortID[idx];
        if(enPort < AIAO_PORT_BUTT)
        {
            Ret = HAL_AIAO_P_SetTrackMode(enPort, (AIAO_TRACK_MODE_E)enMode);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_AIAO("HAL_AIAO_P_SetTrackMode port:%d\n", (HI_U32)enPort);
                return HI_FAILURE;
            }
        }
    }

    state->enUserTrackMode = enMode;

    return HI_SUCCESS;
}

HI_S32 SndOpSetMute(HI_HANDLE hSndOp, HI_U32 u32Mute)
{
    HI_U32 idx;
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    HI_BOOL bMute = (u32Mute == 0) ? HI_FALSE : HI_TRUE;
    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast mute
        return HI_SUCCESS;
    }

    for(idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enPort = state->enPortID[idx];
        if(enPort < AIAO_PORT_BUTT)
        {
            Ret = HAL_AIAO_P_Mute(enPort, bMute);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_AIAO("HAL_AIAO_P_Mute port:%d, Mute: %d\n", (HI_U32)enPort, (HI_U32)bMute);
                return HI_FAILURE;
            }
        }
    }

    state->u32UserMute = u32Mute;

    return HI_SUCCESS;
}

HI_S32 SndOpGetMute(HI_HANDLE hSndOp)
{
    SND_OP_STATE_S *state = (SND_OP_STATE_S *)hSndOp;
    return state->u32UserMute;
}


HI_VOID SND_DestroyOp(SND_CARD_STATE_S *pCard)	
{
    HI_S32 u32PortNum;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        if (pCard->hSndOp[u32PortNum])
        {
            SndOpDestroy(pCard->hSndOp[u32PortNum]);
            pCard->hSndOp[u32PortNum] = HI_NULL;
        }
    }

    return;
}

HI_S32 SND_CreateOp(SND_CARD_STATE_S *pCard, HI_UNF_SND_ATTR_S *pstAttr)
{
    HI_U32 u32PortNum;
    HI_HANDLE hSndOp;

    for (u32PortNum = 0; u32PortNum < pstAttr->u32PortNum; u32PortNum++)
    {
        if (HI_SUCCESS != SndOpCreate(&hSndOp, &pstAttr->stOutport[u32PortNum]))
        {
            goto SND_CreateOp_ERR_EXIT;
        }

        pCard->hSndOp[u32PortNum] = hSndOp;
    }

    memcpy(&pCard->stUserOpenParam, pstAttr, sizeof(HI_UNF_SND_ATTR_S));
    pCard->enUserSampleRate = pstAttr->enSampleRate;
    pCard->enUserHdmiMode = HI_UNF_SND_HDMI_MODE_LPCM;
    pCard->enUserSpdifMode = HI_UNF_SND_SPDIF_MODE_LPCM;
    pCard->u32HdmiDataFormat = 0;
    pCard->u32SpdifDataFormat = 0;
    
    return HI_SUCCESS;

SND_CreateOp_ERR_EXIT:
    SND_DestroyOp(pCard);
    return HI_FAILURE;
}

HI_S32 SND_SetOpMute(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    HI_HANDLE hSndOp;
    HI_U32    u32Mute = 0;
    HI_S32 Ret = HI_SUCCESS;

    if(HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            hSndOp = pCard->hSndOp[u32PortNum];
            if (hSndOp)
            {
                u32Mute = SndOpGetMute(hSndOp);
                u32Mute &= (~(1L << AO_SNDOP_GLOBAL_MUTE_BIT));
                u32Mute |= (HI_U32)bMute << AO_SNDOP_GLOBAL_MUTE_BIT;
                Ret |= SndOpSetMute(hSndOp, u32Mute);
            }
        }
        return Ret;
    }
    else
    {
        hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (hSndOp)
        {
            u32Mute = SndOpGetMute(hSndOp);
            u32Mute &= (~(1L << AO_SNDOP_LOCAL_MUTE_BIT));
            u32Mute |= (HI_U32)bMute << AO_SNDOP_LOCAL_MUTE_BIT;
            return SndOpSetMute(hSndOp, u32Mute);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
}

HI_S32 SND_GetOpMute(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL *pbMute)
{
    HI_HANDLE hSndOp;
    HI_U32    u32Mute;
    HI_U32    u32MuteBit;
    
    if(HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        enOutPort = pCard->stUserOpenParam.stOutport[0].enOutPort;
        u32MuteBit = AO_SNDOP_GLOBAL_MUTE_BIT; 
    }
    else
    {
        u32MuteBit = AO_SNDOP_LOCAL_MUTE_BIT;
    }
    
    hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (hSndOp)
    {
        u32Mute = SndOpGetMute(hSndOp);
        *pbMute = (HI_BOOL)((u32Mute >> u32MuteBit) & 1) ;
        return HI_SUCCESS;
    }
    return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
}

HI_S32 SND_SetOpHdmiMode(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if(!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_HDMI0:
        pCard->enUserHdmiMode = enMode;
        break;
    default:
       HI_ERR_AO("Hdmi mode don't support OutPort(%d)", (HI_U32)enOutPort);
       return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_SetOpSpdifMode(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E enMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if(!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_SPDIF0:
        pCard->enUserSpdifMode = enMode;
        break;
    default:
       HI_ERR_AO("Spdif mode don't support OutPort(%d)", (HI_U32)enOutPort);
       return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_SetOpVolume(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S stGain)
{
    HI_HANDLE hSndOp;
    HI_S32 Ret = HI_SUCCESS;
    if(HI_UNF_SND_OUTPUTPORT_ALL==enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            hSndOp = pCard->hSndOp[u32PortNum];
            if (hSndOp)
            {
                Ret |= SndOpSetVolume(hSndOp, stGain);
            }
        }
        return Ret;
    }
    else
    {
        hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (hSndOp)
        {
            return SndOpSetVolume(hSndOp, stGain);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpVolume(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S *pstGain)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if(HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get trackmode of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (hSndOp)
    {
        pstGain->bLinearMode = ((SND_OP_STATE_S *)hSndOp)->stUserGain.bLinearMode;
        pstGain->s32Gain = ((SND_OP_STATE_S *)hSndOp)->stUserGain.s32Gain;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpSampleRate(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
#if 1
    if(!pCard)
    {
        HI_ERR_AO( "Card is not open!");
        return HI_FAILURE;
    }
    if(pCard->enUserSampleRate == enSampleRate)
    {
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_AO( "SetSampleRate(%d) failed, can't change samplerate at running!",enSampleRate);
        return HI_FAILURE;
    }
#else
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(enSound, enOutPort);
    HI_S32 Ret = HI_FAILURE;
    if (hSndOp)
    {
        return SndOpSetSampleRate(hSndOp, enSampleRate);
    }
    return Ret;
#endif
}

HI_S32 SND_GetOpSampleRate(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SAMPLE_RATE_E *penSampleRate)
{
    *penSampleRate = pCard->enUserSampleRate;
    return HI_SUCCESS;
}

HI_S32 SND_SetOpTrackMode(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    HI_HANDLE hSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if(HI_UNF_SND_OUTPUTPORT_ALL==enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            hSndOp = pCard->hSndOp[u32PortNum];
            if (hSndOp)
            {
                Ret |= SndOpSetTrackMode(hSndOp, enMode);
            }
        }
        return Ret;
    }
    else
    {
        hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (hSndOp)
        {
            return SndOpSetTrackMode(hSndOp, enMode);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpTrackMode(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E *penMode)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if(HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get trackmode of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (hSndOp)
    {
        *penMode = ((SND_OP_STATE_S *)hSndOp)->enUserTrackMode;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpAttr(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_OP_ATTR_S *pstSndPortAttr)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (hSndOp)
    {
        return SndOpSetAttr(hSndOp, pstSndPortAttr);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

}

HI_S32 SND_GetOpAttr(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_OP_ATTR_S *pstSndPortAttr)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (hSndOp)
    {
        return SndOpGetAttr(hSndOp, pstSndPortAttr);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

#if 0
HI_S32 SND_GetOpStatus(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, AIAO_PORT_STAUTS_S *pstPortStatus)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    HI_S32 Ret = HI_FAILURE;

    if (hSndOp)
    {
        return SndOpGetStatus(hSndOp, pstPortStatus);
    }

    return Ret;
}
#endif

HI_S32 SND_StopOp(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (hSndOp)
    {
        return SndOpStop(hSndOp, HI_NULL);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_StartOp(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (hSndOp)
    {
        return SndOpStart(hSndOp, HI_NULL);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

#ifdef HI_SND_CAST_SUPPORT

static HI_U32 Cast_GetSysTime(HI_VOID)
{
    HI_U64   SysTime;

    SysTime = sched_clock();

    do_div(SysTime, 1000000);

    return (HI_U32)SysTime;
}

HI_S32 SND_StopCastOp(SND_CARD_STATE_S *pCard, HI_S32 s32CastID)
{
    HI_HANDLE hSndOp = pCard->hCastOp[s32CastID];
    HI_S32 Ret = HI_FAILURE;

    if (hSndOp)
    {
        return SndOpStop(hSndOp, HI_NULL);
    }
    return Ret;
}
HI_S32 SND_StartCastOp(SND_CARD_STATE_S *pCard, HI_S32 s32CastID)
{
    HI_HANDLE hSndOp = pCard->hCastOp[s32CastID];
    HI_S32 Ret = HI_FAILURE;

    if (hSndOp)
    {
        return SndOpStart(hSndOp, HI_NULL);
    }
    return Ret;
}

HI_S32 SND_CreateCastOp(SND_CARD_STATE_S *pCard,  HI_S32 *ps32CastId, HI_UNF_SND_CAST_ATTR_S *pstAttr, HI_U32 *pu32PhyAddr)
{
    HI_HANDLE hSndOp;

   if (HI_SUCCESS != SndOpCreateCast(&hSndOp,  ps32CastId, pstAttr, pu32PhyAddr))
    {
        HI_ERR_AIAO("SndOpCreateCast Failed\n");
        return HI_FAILURE;
    }
    //HI_ERR_AIAO("hSndOp=%p\n", hSndOp);
    pCard->hCastOp[*ps32CastId] = hSndOp;

    return HI_SUCCESS;
}

HI_S32 SND_DestoryCastOp(SND_CARD_STATE_S *pCard,  HI_U32 CastId)
{

    if (pCard->hCastOp[CastId])
    {
        SndOpDestroy(pCard->hCastOp[CastId]);
        pCard->hCastOp[CastId] = HI_NULL;
    }
    else
    {
        HI_ERR_AIAO("SND_DestoryCastOp Null pointer Failed\n");
    }
    
    return HI_SUCCESS;   
}
  
HI_U32 SND_ReadCastData(SND_CARD_STATE_S *pCard, HI_S32 s32CastId, AO_Cast_Data_Param_S *pstCastData)
{
    SND_OP_STATE_S *state;
    HI_U32 u32ReadBytes = 0;
    HI_U32 u32NeedBytes = 0;

    state = (SND_OP_STATE_S *)pCard->hCastOp[s32CastId];
    if (state)
    {
        if (SND_OUTPUT_TYPE_CAST != state->enOutType)
        {
            return HI_FAILURE;
        }

        u32NeedBytes = pstCastData->u32FrameBytes;
        //HI_ERR_AO( "u32NeedBytes  0x%x  !\n",u32NeedBytes);

        u32ReadBytes = HAL_CAST_ReadData(state->CastId, &pstCastData->u32DataOffset, u32NeedBytes);
#if 0        
        if (u32ReadBytes)
        {
            HI_ERR_AO( "u32NeedBytes  0x%x  , u32ReadBytes 0x%x !\n",u32NeedBytes, u32ReadBytes);
        }
#endif

#if 1   //add  pts to cast frame
        pstCastData->stAOFrame.u32PtsMs = Cast_GetSysTime();
#endif

        pstCastData->stAOFrame.u32PcmSamplesPerFrame = u32ReadBytes  / pstCastData->u32SampleBytes;
        //HI_ERR_AO( "pstCastData->u32DataOffset=0x%x ,stAOFrame.u32PcmSamplesPerFrame   0x%x\n",pstCastData->u32DataOffset ,(int)(pstCastData->stAOFrame.u32PcmSamplesPerFrame));
    }

    return u32ReadBytes;
}

HI_U32 SND_ReleaseCastData(SND_CARD_STATE_S *pCard, HI_S32 s32CastId, AO_Cast_Data_Param_S *pstCastData)
{
    SND_OP_STATE_S *state;
    HI_U32 u32RleaseBytes = 0;

    //*state = ( SND_OP_STATE_S *)SNDGetOpHandleByOutPort(pCard, HI_UNF_SND_OUTPUTPORT_CAST);
    state = (SND_OP_STATE_S *)pCard->hCastOp[s32CastId];
    if (state)
    {
        if (SND_OUTPUT_TYPE_CAST != state->enOutType)
        {
            return HI_FAILURE;
        }

        u32RleaseBytes = pstCastData->u32FrameBytes;
        u32RleaseBytes = HAL_CAST_ReleaseData(state->CastId, u32RleaseBytes);
        //HI_ERR_AO( "u32RleaseBytes  0x%x  !\n",u32RleaseBytes);

        return u32RleaseBytes;
    }

    return HI_FAILURE;
}

#endif


#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
HI_S32 Snd_GetOpSetting(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enPort, SND_OUTPORT_ATTR_S *pstPortAttr)
{
    SND_OP_STATE_S *state;
    
    state = (SND_OP_STATE_S *)SNDGetOpHandleByOutPort(pCard, enPort);
    if(!state)
    {
        return HI_FAILURE;
    }
    pstPortAttr->enCurnStatus = state->enCurnStatus;
    pstPortAttr->u32UserMute = state->u32UserMute;
    pstPortAttr->enUserTrackMode = state->enUserTrackMode;
    memcpy(&pstPortAttr->stUserGain, &state->stUserGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));
    
    return HI_SUCCESS;
}

HI_S32 SND_GetSetting(SND_CARD_STATE_S *pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32Port;
    
    pstSndSettings->enUserHdmiMode = pCard->enUserHdmiMode;
    pstSndSettings->enUserSpdifMode = pCard->enUserSpdifMode;
    memcpy(&pstSndSettings->stUserOpenParam, &pCard->stUserOpenParam, sizeof(HI_UNF_SND_ATTR_S));
    for(u32Port = 0; u32Port < pCard->stUserOpenParam.u32PortNum; u32Port++)
    {
        Snd_GetOpSetting(pCard, pCard->stUserOpenParam.stOutport[u32Port].enOutPort, &pstSndSettings->stPortAttr[u32Port]);
    }
    
    return HI_SUCCESS; 
}

HI_S32 SND_RestoreOpSetting(SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enPort, SND_OUTPORT_ATTR_S *pstPortAttr)
{
    HI_HANDLE hSndOp = SNDGetOpHandleByOutPort(pCard, enPort);
    if(!hSndOp)
    {
        return HI_FAILURE;
    }
    if(SND_OP_STATUS_START == pstPortAttr->enCurnStatus)
    {
        SndOpStart(hSndOp, HI_NULL);
    }
    else if(SND_OP_STATUS_STOP == pstPortAttr->enCurnStatus)
    {
        SndOpStop(hSndOp, HI_NULL);
    }

    SndOpSetMute(hSndOp, pstPortAttr->u32UserMute);
    SndOpSetTrackMode(hSndOp, pstPortAttr->enUserTrackMode);
    SndOpSetVolume(hSndOp, pstPortAttr->stUserGain);

    return HI_SUCCESS;
}

HI_S32 SND_RestoreSetting(SND_CARD_STATE_S *pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32Port;
        
    pCard->enUserHdmiMode = pstSndSettings->enUserHdmiMode;
    pCard->enUserSpdifMode = pstSndSettings->enUserSpdifMode;
    for(u32Port = 0; u32Port < pstSndSettings->stUserOpenParam.u32PortNum; u32Port++)
    {
        SND_RestoreOpSetting(pCard, pstSndSettings->stUserOpenParam.stOutport[u32Port].enOutPort, &pstSndSettings->stPortAttr[u32Port]);
    }
    
    return HI_SUCCESS; 
}
#endif

static inline const HI_CHAR *AOPort2Name(HI_UNF_SND_OUTPUTPORT_E enPort)
{
    const HI_CHAR *apcName[HI_UNF_SND_OUTPUTPORT_ARC0 + 1] = 
    {
        "ADAC0",
        "I2S0",
        "I2S1",
        "SPDIF0",
        "HDMI0",
        "ARC0",
    };

    if (enPort <= HI_UNF_SND_OUTPUTPORT_ARC0)
    {
        return apcName[enPort];
    }
    
    return "UnknownPort";
}

HI_S32 SND_ShowOpProc(struct seq_file* p, SND_CARD_STATE_S *pCard, HI_UNF_SND_OUTPUTPORT_E enPort)
{
    AIAO_PORT_STAUTS_S stStatus;
    HI_HANDLE hSndOp;
    SND_OP_STATE_S *state;
    //SND_OP_ATTR_S stOpAttr;

    hSndOp = SNDGetOpHandleByOutPort(pCard, enPort);

    if (HI_NULL == hSndOp)
    {
        return HI_FAILURE;
    }

    SndOpGetStatus(hSndOp, &stStatus);
    state = (SND_OP_STATE_S *)hSndOp;  
#if 0
    if(HI_SUCCESS != SND_GetOpAttr(enSnd, enPort, &stOpAttr))
    {
        return HI_FAILURE;
    }
#endif
    seq_printf(p,
               "%s: status(%s), Mute(%s/0x%x), Vol(%.2ddB), TrackMode(%s), Fs(%.6d), Ch(%.2d), Bit(%2d)\n",
                AUTIL_Port2Name(enPort),
               (HI_CHAR*)((AIAO_PORT_STATUS_START == stStatus.enStatus) ? "start" : ((AIAO_PORT_STATUS_STOP == stStatus.enStatus) ? "stop" : "stopping")),
               (HI_TRUE == stStatus.stUserConfig.bMute)?"on":"off",
               state->u32UserMute,
               (HI_S32)stStatus.stUserConfig.u32VolumedB - VOLUME_0dB,
                AUTIL_TrackMode2Name(stStatus.stUserConfig.enTrackMode),
                stStatus.stUserConfig.stIfAttr.enRate,
                stStatus.stUserConfig.stIfAttr.enChNum,
                stStatus.stUserConfig.stIfAttr.enBitDepth);
    seq_printf(p,
                  "      Engine(%s), AOP(0x%x), PortID(0x%x)\n",
                   AUTIL_Engine2Name(state->enEngineType[state->ActiveId]),
                   (HI_U32)state->enAOP[state->ActiveId],
                   (HI_U32)state->enPortID[state->ActiveId]);
            
    seq_printf(p,
               "      DmaCnt(%.6u), BufEmptyCnt(%.6u), BufEmptyWarningCnt(%.6u), IfFiFoEmptyCnt(%.6u)\n\n",
               stStatus.stProcStatus.uDMACnt,
               stStatus.stProcStatus.uBufEmptyCnt, 
               stStatus.stProcStatus.uBufEmptyWarningCnt, 
               stStatus.stProcStatus.uInfFiFoEmptyCnt);
    
    return HI_SUCCESS;
}


