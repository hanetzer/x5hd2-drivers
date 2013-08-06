/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_ao.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/17
  Description   :
  History       :
  1.Date        : 2013/04/17
    Author      : zgjie
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/hardware.h>

/* Unf headers */
#include "hi_error_mpi.h"

/* Drv headers */
#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "drv_ao_ioctl.h"
#include "drv_ao_ext.h"
#include "drv_adsp_ext.h"
#include "drv_ao_private.h"

#include "hi_audsp_aoe.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"

#include "drv_ao_op.h"
#include "drv_ao_track.h"
#include "audio_util.h"

#ifdef HI_SND_CAST_SUPPORT
#include "drv_ao_cast.h"
#endif
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 AO_todofunc(HI_VOID)
{
    // todo

    return HI_SUCCESS;
}

DECLARE_MUTEX(g_AoMutex);

static AO_GLOBAL_PARAM_S s_stAoDrv =
{
    .u32TrackNum         =  0,
    .u32SndNum           =  0,
    .atmOpenCnt          = ATOMIC_INIT(0),
    .bReady              = HI_FALSE,

    .pstProcParam        = HI_NULL,

    .pAdspFunc           = HI_NULL,
    .stExtFunc           =
    {
        .pfnAiaotodofunc = AO_todofunc,
    }
};

/***************************** Original Static Definition *****************************/

static SND_CARD_STATE_S * SND_CARD_GetCard(HI_UNF_SND_E enSound)
{
    return s_stAoDrv.astSndEntity[enSound].pCard;
}


static HI_VOID AO_Snd_FreeHandle(HI_UNF_SND_E enSound, HI_U32 u32FileId)
{
    if(0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
    {
        HI_KFREE(HI_ID_AO, s_stAoDrv.astSndEntity[enSound].pCard);
        s_stAoDrv.astSndEntity[enSound].pCard   = HI_NULL;
    	s_stAoDrv.u32SndNum--;
    }

    if(u32FileId < SND_MAX_OPEN_NUM)
    {
        s_stAoDrv.astSndEntity[enSound].u32File[u32FileId] = HI_NULL;
    }

    return;
}

static HI_S32 SNDGetFreeFileId(HI_UNF_SND_E enSound)
{
    HI_U32 i;

    for(i = 0; i < SND_MAX_OPEN_NUM; i++)
    {
        if(s_stAoDrv.astSndEntity[enSound].u32File[i]  == HI_NULL)
        {
            return i;
        }
    }

    return SND_MAX_OPEN_NUM;
}



static HI_S32 AO_Snd_AllocHandle(HI_UNF_SND_E enSound, struct file *pstFile)
{
    SND_CARD_STATE_S *pCard;
    HI_U32 u32FreeId;
    
    if (enSound >= HI_UNF_SND_BUTT)
    {
        HI_ERR_AO("Bad param!\n");
        goto err0;
    }

    /* Check ready flag */
    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        goto err0;
    }

    /* Check Snd number */
    if (s_stAoDrv.u32SndNum >= AO_MAX_TOTAL_SND_NUM)
    {
        HI_ERR_AO("Too many chans:%d!\n", s_stAoDrv.u32SndNum);
        goto err0;
    }

    u32FreeId = SNDGetFreeFileId(enSound);
    if(u32FreeId >= SND_MAX_OPEN_NUM)
    {
        HI_ERR_AO("Get free file id faied!\n");
        goto err0;
    }
    ((DRV_AO_STATE_S *)(pstFile->private_data))->u32FileId = u32FreeId;
    s_stAoDrv.astSndEntity[enSound].u32File[u32FreeId] = (HI_U32)pstFile;

    /* Allocate new snd resource */
    if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
    {
        pCard = (SND_CARD_STATE_S *)HI_KMALLOC(HI_ID_AO, sizeof(SND_CARD_STATE_S), GFP_KERNEL);
        if(HI_NULL == pCard)
        {
            s_stAoDrv.astSndEntity[enSound].u32File[u32FreeId] = HI_NULL;
            HI_ERR_AO("Kmalloc card failed!\n");
            goto err0;
        }
        else
        {
            s_stAoDrv.astSndEntity[enSound].pCard = pCard;
        }

        s_stAoDrv.u32SndNum++;
    }

    return HI_SUCCESS;

err0:
    return HI_FAILURE;
}


/******************************Snd process FUNC*************************************/

HI_S32 AOGetSndDefOpenAttr(HI_UNF_SND_ATTR_S *pstSndAttr)
{
#if 1
    pstSndAttr->u32PortNum = 3;
 #ifdef HI_SND_FPGA
    pstSndAttr->stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_I2S0;
    pstSndAttr->stOutport[0].unAttr.stI2sAttr.pPara = HI_NULL;
 #else
    pstSndAttr->stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
    pstSndAttr->stOutport[0].unAttr.stDacAttr.pPara = HI_NULL;
 #endif
    pstSndAttr->stOutport[1].enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
    pstSndAttr->stOutport[1].unAttr.stSpdifAttr.pPara = HI_NULL;
    pstSndAttr->stOutport[2].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
    pstSndAttr->stOutport[2].unAttr.stHDMIAttr.pPara = HI_NULL;
    pstSndAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
#else
    pstSndAttr->u32PortNum = 1;
    pstSndAttr->stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
    pstSndAttr->stOutport[0].unAttr.stDacAttr.pPara = HI_NULL;
    pstSndAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
#endif
    return HI_SUCCESS;
}

HI_S32 AO_SND_Open( HI_UNF_SND_E enSound, HI_UNF_SND_ATTR_S *pstAttr )
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 i;
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    // check attr
    CHECK_AO_SNDCARD( enSound );
    CHECK_AO_PORTNUM( pstAttr->u32PortNum );
    CHECK_AO_SAMPLERATE( pstAttr->enSampleRate );
    memset(pCard, 0, sizeof(SND_CARD_STATE_S));
    pCard->pstHdmiFunc = HI_NULL;
    pCard->enHdmiPassthrough = SND_HDMI_MODE_NONE;
    pCard->enSpdifPassthrough = SND_SPDIF_MODE_NONE;
    for (i = 0; i < pstAttr->u32PortNum; i++)
    {
        CHECK_AO_OUTPORT( pstAttr->stOutport[i].enOutPort );
        if(HI_UNF_SND_OUTPUTPORT_HDMI0 == pstAttr->stOutport[i].enOutPort)
        {
            /* Get hdmi functions */
            Ret = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&pCard->pstHdmiFunc);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("Get hdmi function err:%#x!\n", Ret);
                return Ret;
            }
            pCard->enHdmiPassthrough = SND_HDMI_MODE_PCM;
        }
        if(HI_UNF_SND_OUTPUTPORT_SPDIF0 == pstAttr->stOutport[i].enOutPort)
        {
            pCard->enSpdifPassthrough = SND_SPDIF_MODE_PCM;
        }
    }

    Ret = SND_CreateOp( pCard, pstAttr );
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO( "Create snd op failed!" );
        return Ret;
    }

    return Ret;
}

HI_S32 AO_SND_Close(HI_UNF_SND_E enSound)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    SND_DestroyOp( pCard );
    TRACK_DestroyEngine( pCard );

    return HI_SUCCESS;
}

//zgjiere; HI_UNF_SND_OUTPUTPORT_ALL
static HI_S32 AO_SND_SetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);
    
    return SND_SetOpMute(pCard, enOutPort, bMute);
}

static HI_S32 AO_SND_GetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL *pbMute)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pbMute);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpMute(pCard, enOutPort, pbMute);
}

static HI_S32 AO_SND_SetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enMode)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_HDMIMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpHdmiMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_SetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E enMode)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_SPDIFMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpSpdifMode(pCard, enOutPort, enMode);
}

HI_S32 AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S stGain)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    if (HI_TRUE == stGain.bLinearMode)
    {
        CHECK_AO_LINEARVOLUME(stGain.s32Gain);
    }
    else
    {
        CHECK_AO_ABSLUTEVOLUME(stGain.s32Gain);
    }
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpVolume(pCard, enOutPort, stGain);
}

static HI_S32 AO_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S *pstGain)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstGain);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    //TODO Check volumn Attr

    return SND_GetOpVolume(pCard, enOutPort, pstGain);
}

//zgjiere; HAL_AIAO_P_SetSampleRate 不能动态修改，建议HI_UNF_SND_ATTR_S确定
static HI_S32 AO_SND_SetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_SAMPLERATE(enSampleRate);
    CHECK_AO_NULL_PTR(pCard);

    return SND_SetOpSampleRate(pCard, enOutPort, enSampleRate);
}

static HI_S32 AO_SND_GetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SAMPLE_RATE_E *penSampleRate)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penSampleRate);
    CHECK_AO_NULL_PTR(pCard);

    return SND_GetOpSampleRate(pCard, enOutPort, penSampleRate);
}

static HI_S32 AO_SND_SetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_TRACKMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpTrackMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_GetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E *penMode)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpTrackMode(pCard, enOutPort, penMode);
}

static HI_S32  AO_SND_SetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bSmartVolume)
{
    //TO DO
    //verify
    return HI_SUCCESS;
}

static HI_S32 AO_SND_GetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL *pbSmartVolume)
{
    //TO DO
    //verify
    return HI_SUCCESS;
}



#ifdef HI_ALSA_AO_SUPPORT
/* snd open kernel intf */
HI_S32 AO_Snd_Kopen(AO_SND_Open_Param_S_PTR arg, struct file *file)
{
    HI_S32 s32Ret;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;
    AO_SND_Open_Param_S_PTR pstSndParam = ( AO_SND_Open_Param_S_PTR )arg;
    DRV_AO_STATE_S *pAOState = file->private_data;

    enSound = pstSndParam->enSound;
    CHECK_AO_SNDCARD( enSound );

    s32Ret = down_interruptible(&g_AoMutex);
    if (HI_SUCCESS == AO_Snd_AllocHandle(enSound, file))
    {
        if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
        {
                s32Ret = AO_SND_Open( enSound, &pstSndParam->stAttr );
                if (HI_SUCCESS != s32Ret)
                {
                    AO_Snd_FreeHandle(enSound, pAOState->u32FileId);
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
        }
    }

    atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    atomic_inc(&pAOState->atmUserOpenCnt[enSound]);

    up(&g_AoMutex);

    return HI_SUCCESS;
}

/* snd close kernel intf */
HI_S32 AO_Snd_Kclose(HI_UNF_SND_E  arg, struct file *file)
{
    HI_S32 s32Ret;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;
    DRV_AO_STATE_S *pAOState = file->private_data;
    enSound = arg;
    CHECK_AO_SNDCARD_OPEN( enSound );

    s32Ret = down_interruptible(&g_AoMutex);
    if(atomic_dec_and_test(&pAOState->atmUserOpenCnt[enSound]))
    {
            if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
            {
                s32Ret = AO_SND_Close( enSound );
                if (HI_SUCCESS != s32Ret)
                {
                    atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }

                AO_Snd_FreeHandle(enSound, pAOState->u32FileId);
            }
    }
    else
	{
        atomic_dec(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    }   

    up(&g_AoMutex);

    return HI_SUCCESS;
}
#endif

/******************************Snd Track FUNC*************************************/
static SND_CARD_STATE_S * TRACK_CARD_GetCard(HI_U32 Id)
{
    HI_UNF_SND_E sndx;
    SND_CARD_STATE_S *pCard = HI_NULL;

    if (Id >= AO_MAX_TOTAL_TRACK_NUM)
    {
        return HI_NULL;
    }

    for (sndx = HI_UNF_SND_0; sndx < HI_UNF_SND_BUTT; sndx++)
    {
        pCard = SND_CARD_GetCard(sndx);
        if(pCard)
        {
            if (pCard->uSndTrackInitFlag & (1L << Id))
            {
                return pCard;
            }
        }
    }

    return HI_NULL;
}

static HI_S32 AO_Track_AllocHandle(HI_HANDLE *phHandle, struct file *pstFile)
{
    HI_U32 i;

    if (HI_NULL == phHandle)
    {
        HI_ERR_AO("Bad param!\n");
        return HI_FAILURE;
    }

    /* Check ready flag */
    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        return HI_FAILURE;
    }

    /* Check channel number */
    if (s_stAoDrv.u32TrackNum >= AO_MAX_TOTAL_TRACK_NUM)
    {
        HI_ERR_AO("Too many track:%d!\n", s_stAoDrv.u32TrackNum);
        goto err0;
    }

    /* Allocate new channel */
    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        if (0 == atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
        {
            s_stAoDrv.astTrackEntity[i].u32File = (HI_U32)HI_NULL;
            break;
        }
    }

    if (i >= AO_MAX_TOTAL_TRACK_NUM)
    {
        HI_ERR_AO("Too many track!\n");
        goto err0;
    }

    /* Allocate resource */
    s_stAoDrv.astTrackEntity[i].u32File = (HI_U32)pstFile;
    s_stAoDrv.u32TrackNum++;
    atomic_inc(&s_stAoDrv.astTrackEntity[ i].atmUseCnt);
    /*
      define of Track Handle :
      bit31                                                           bit0
        |<----   16bit --------->|<---   8bit    --->|<---  8bit   --->|
        |--------------------------------------------------------------|
        |      HI_MOD_ID_E            |  sub_mod defined  |     chnID       |
        |--------------------------------------------------------------|
      */
    *phHandle = (HI_ID_AO << 16) | (HI_ID_TRACK << 8) | i;
    return HI_SUCCESS;

err0:
    return HI_FAILURE;
}

static HI_VOID AO_Track_FreeHandle(HI_HANDLE hHandle)
{
    hHandle &= AO_TRACK_CHNID_MASK;
    s_stAoDrv.astTrackEntity[hHandle].u32File = (HI_U32)HI_NULL;
    s_stAoDrv.u32TrackNum--;
    atomic_set(&s_stAoDrv.astTrackEntity[hHandle].atmUseCnt, 0);
}

static HI_VOID AO_TRACK_SaveSuspendAttr(HI_HANDLE hHandle, AO_Track_Create_Param_S_PTR pstTrack)
{
    hHandle &= AO_TRACK_CHNID_MASK;
    s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.enSound = pstTrack->enSound;
    s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.bAlsaTrack = pstTrack->bAlsaTrack;
    memcpy(&s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.stBufAttr, &pstTrack->stBuf, sizeof(AO_BUF_ATTR_S));
}

static HI_S32 AO_Track_GetDefAttr(HI_UNF_AUDIOTRACK_ATTR_S * pstDefAttr)
{
    return TRACK_GetDefAttr(pstDefAttr);
}

static HI_S32 AO_Track_GetAttr(HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetAttr(pCard, u32TrackID, pstTrackAttr);
    }
    else
    {
        return HI_FAILURE;
    }
}

#if 0
static HI_S32 AO_Track_SetAttr(HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetAttr(pCard, u32TrackID, pstTrackAttr);
    }
    else
    {
        return HI_FAILURE;
    }
}

#endif

HI_S32 AO_Track_Create(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr,
                       HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf, HI_HANDLE hTrack)
{
    SND_CARD_STATE_S *pCard;
    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = SND_CARD_GetCard(enSound);

    return TRACK_CreateNew(pCard, pstAttr, bAlsaTrack, pstBuf, hTrack);
}

HI_S32 AO_Track_Destory(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Destroy(pCard, u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_Start(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Start(pCard, u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_Stop(HI_U32 u32TrackID)               
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Stop(pCard, u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_Pause(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Pause(pCard,u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_Flush(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Flush(pCard, u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_SendData(HI_U32 u32TrackID, HI_UNF_AO_FRAMEINFO_S * pstAOFrame)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SendData(pCard, u32TrackID, pstAOFrame);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_SetWeight(HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S stTrackGain)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetWeight(pCard, u32TrackID, &stTrackGain);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_GetWeight(HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S *pstTrackGain)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetWeight(pCard, u32TrackID, pstTrackGain);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_SetSpeedAdjust(HI_U32 u32TrackID, AO_SND_SPEEDADJUST_TYPE_E enType, HI_S32 s32Speed)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetSpeedAdjust(pCard, u32TrackID, enType, s32Speed);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_GetDelayMs(HI_U32 u32TrackID, HI_U32 *pu32DelayMs)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetDelayMs(pCard, u32TrackID, pu32DelayMs);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_IsBufEmpty(HI_U32 u32TrackID, HI_BOOL *pbBufEmpty)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_IsBufEmpty(pCard, u32TrackID, pbBufEmpty);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_SetEosFlag(HI_U32 u32TrackID, HI_BOOL bEosFlag)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetEosFlag(pCard, u32TrackID, bEosFlag);
    }
    else
    {
        return HI_FAILURE;
    }
}

#if 0
static HI_S32 AO_Track_GetStatus(HI_U32 u32TrackID, HI_VOID *pstParam)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetStatus(pCard, u32TrackID, pstParam);
    }
    else
    {
        return HI_FAILURE;
    }
}

#endif

//zgjiere; alsa统筹考虑

/*
//zgjiere, 接口是否满足标准alsa, 中断服务程序
 */

/*
//zgjiere, alsa ? start/stop/pause/flush
 */

/*
//zgjiere, alsa 数据如何写入? 目前AIP仍然根据AIP读写指针进行数据处理? ALSA标准驱动行为定时读，忽略写指针
 */

HI_S32 AO_Track_UpdateBufWptr(HI_U32 u32TrackID, HI_U32 *pu32WritePos)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_UpdateWptrPos(pCard, u32TrackID, pu32WritePos);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_GetAipReadPos(HI_U32 u32TrackID, HI_U32 *pu32ReadPos)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetReadPos(pCard, u32TrackID, pu32ReadPos);
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 AO_Track_FlushBuf(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S *pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_FlushBuf(pCard, u32TrackID);
    }
    else
    {
        return HI_FAILURE;
    }
}

#ifdef HI_ALSA_AO_SUPPORT

/* track create kernel intf */
HI_S32 AO_Track_Kcreate(AO_Track_Create_Param_S_PTR  arg, struct file *file)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    AO_Track_Create_Param_S_PTR pstTrack = (AO_Track_Create_Param_S_PTR)arg;

    s32Ret = down_interruptible(&g_AoMutex);

    if (HI_SUCCESS == AO_Track_AllocHandle(&hHandle, file))
    {
        s32Ret = AO_Track_Create(pstTrack->enSound, &pstTrack->stAttr, pstTrack->bAlsaTrack,
                              &pstTrack->stBuf,
                              hHandle);
        if (HI_SUCCESS != s32Ret)
        {
            AO_Track_FreeHandle(hHandle);
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        AO_TRACK_SaveSuspendAttr(hHandle, pstTrack);
        pstTrack->hTrack = hHandle;
    }
    up(&g_AoMutex);

    return HI_SUCCESS;
}
/* track destroy kernel intf */
HI_S32 AO_Track_Kdestory(HI_HANDLE  *arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE *)arg;

    s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Destory( hTrack );
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_AoMutex);
        return HI_FAILURE;
    }
    AO_Track_FreeHandle(hTrack);
    
    up(&g_AoMutex);
    return HI_SUCCESS;
}

/* track start kernel intf */
HI_S32 AO_Track_Kstart(HI_HANDLE  *arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE *)arg;

    //s32Ret = down_interruptible(&g_AoMutex);	//alsa atomic area

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Start(hTrack);
    
    //up(&g_AoMutex);
    return s32Ret;
}

/* track stop kernel intf */
HI_S32 AO_Track_Kstop(HI_HANDLE  *arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE *)arg;

    //s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Stop(hTrack);
    
    //up(&g_AoMutex);
    return s32Ret;
}
/* track stop kernel intf */
HI_S32 AO_Track_Kflush(HI_HANDLE  *arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE *)arg;

    //s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Flush(hTrack);

    //up(&g_AoMutex);
    return s32Ret;
}
#endif

/******************************Snd Cast FUNC*************************************/
#ifdef HI_SND_CAST_SUPPORT
static HI_S32 AO_Cast_AllocHandle(HI_HANDLE *phHandle, struct file *pstFile)
{
    HI_U32 i;

    if (HI_NULL == phHandle)
    {
        HI_ERR_AO("Bad param!\n");
        return HI_FAILURE;
    }

    /* Check ready flag */
    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        return HI_FAILURE;
    }

    /* Check channel number */
    if (s_stAoDrv.u32CastNum >= AO_MAX_CAST_NUM)
    {
        HI_ERR_AO("Too many Cast:%d!\n", s_stAoDrv.u32CastNum);
        goto err0;
    }

    /* Allocate new channel */
    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        if (0 == atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
        {
            s_stAoDrv.astCastEntity[i].u32File = (HI_U32)HI_NULL;
            break;
        }
    }

    if (i >= AO_MAX_CAST_NUM)
    {
        HI_ERR_AO("Too many Cast chans!\n");
        goto err0;
    }

    /* Allocate resource */
    s_stAoDrv.astCastEntity[i].u32File = (HI_U32)pstFile;
    s_stAoDrv.u32CastNum++;
    atomic_inc(&s_stAoDrv.astCastEntity[ i].atmUseCnt);
    *phHandle = (HI_ID_AO << 16) |(HI_ID_CAST << 8) | i;
    return HI_SUCCESS;

err0:
    return HI_FAILURE;
}

static HI_VOID AO_Cast_FreeHandle(HI_HANDLE hHandle)
{
    hHandle &= AO_CAST_CHNID_MASK;
    s_stAoDrv.astCastEntity[hHandle].u32File = (HI_U32)HI_NULL;
    s_stAoDrv.u32CastNum--;
    atomic_set(&s_stAoDrv.astCastEntity[hHandle].atmUseCnt, 0);
}

#define CHECK_AO_CAST_OPEN(Cast) \
    do                                                         \
    {                                                          \
        CHECK_AO_CAST(Cast);                             \
        if (0 == atomic_read(&s_stAoDrv.astCastEntity[Cast & AO_CAST_CHNID_MASK].atmUseCnt))   \
        {                                                       \
            HI_WARN_AO(" Invalid Cast id 0x%x\n", Cast);        \
            return HI_ERR_SND_INVALID_PARA;                       \
        }                                                       \
    } while (0)


static SND_CARD_STATE_S * CAST_CARD_GetCard(HI_U32 Id)
{
    HI_UNF_SND_E sndx;
    SND_CARD_STATE_S *pCard = HI_NULL;

    if (Id >= AO_MAX_CAST_NUM)
    {
        HI_WARN_AO(" Invalid Cast id 0x%x\n", Id); 
        return HI_NULL;
    }

    for (sndx = HI_UNF_SND_0; sndx < HI_UNF_SND_BUTT; sndx++)
    {
        pCard = SND_CARD_GetCard(sndx);
        if(pCard)
        {
            if (pCard->uSndCastInitFlag & (1L << Id))
            {
                return pCard;
            }
        }
    }

    return HI_NULL;
}


static HI_S32 AO_Cast_GetDefAttr(HI_UNF_SND_CAST_ATTR_S * pstDefAttr)
{
    return CAST_GetDefAttr(pstDefAttr);
}


HI_S32 AO_Cast_Create(HI_UNF_SND_E enSound, HI_UNF_SND_CAST_ATTR_S *pstCastAttr, HI_U32 *pu32ReqSize, HI_HANDLE hCast)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_CreateNew(pCard, pstCastAttr, pu32ReqSize, hCast);
}


HI_S32 AO_Cast_Destory(HI_HANDLE hCast)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_Destroy(pCard, hCast);
}

HI_S32 AO_Cast_SetInfo(HI_HANDLE hCast, HI_U32 u32UserVirtAddr)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_SetInfo(pCard, hCast, u32UserVirtAddr);
}


HI_S32 AO_Cast_GetInfo(HI_HANDLE hCast, AO_Cast_Info_Param_S *pstInfo)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_GetInfo(pCard, hCast, pstInfo);
}


HI_S32 AO_Cast_SetEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_SetEnable(pCard, hCast, bEnable);
}

HI_S32 AO_Cast_GetEnable(HI_HANDLE hCast, HI_BOOL *pbEnable)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_GetEnable(pCard, hCast, pbEnable);
}


static HI_S32 AO_Cast_ReadData(HI_HANDLE hCast, AO_Cast_Data_Param_S *pstCastData)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);
                  
    return CAST_ReadData(pCard, hCast, pstCastData);
}

static HI_S32 AO_Cast_ReleseData(HI_HANDLE hCast, AO_Cast_Data_Param_S *pstCastData)
{
    SND_CARD_STATE_S *pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);
                                                  
    return CAST_ReleaseData(pCard, hCast, pstCastData);
}
#endif

/********************************Driver inteface FUNC****************************************/

/*********************************** Code ************************************/
//zgjiere; proc兼容需求是否满足? 起码风格类似于V1R1
static HI_S32 AOShowSndProc( struct seq_file* p, HI_UNF_SND_E enSnd )
{
    HI_U32 i;
    HI_UNF_SND_ATTR_S* pstSndAttr;
    SND_CARD_STATE_S* pCard;

    pCard = SND_CARD_GetCard(enSnd);
    if (HI_NULL == pCard)
    {
        seq_printf( p, "\n#############################  Sound[%d] Not Open ###########################\n", (HI_U32)enSnd );
        return HI_SUCCESS;
    }

    seq_printf( p, "\n####################################  Sound[%d]  Status  #############################################\n", (HI_U32)enSnd );
    pstSndAttr = &pCard->stUserOpenParam;

    seq_printf( p,
                "SampleRate     :%d\n",
                pstSndAttr->enSampleRate );

    if (SND_SPDIF_MODE_NONE != pCard->enSpdifPassthrough)
    {
        seq_printf( p,
                    "SPDIF Status   :UserSetMode(%s) DataFormat(%s)\n",
                    AUTIL_SpdifMode2Name(pCard->enUserSpdifMode),
                    AUTIL_Format2Name(pCard->u32SpdifDataFormat));
    }
    if (SND_HDMI_MODE_NONE != pCard->enHdmiPassthrough)
    {
        seq_printf( p,
                    "HDMI Status    :UserSetMode(%s) DataFormat(%s)\n",
                    AUTIL_HdmiMode2Name(pCard->enUserHdmiMode),                 
                    AUTIL_Format2Name(pCard->u32HdmiDataFormat));
    }

    seq_printf( p, "\n######################################  OutPort Status  ######################################\n" );
    for (i = 0; i < pstSndAttr->u32PortNum; i++)
    {
        SND_ShowOpProc( p, pCard, pstSndAttr->stOutport[i].enOutPort );
    }

    seq_printf( p, "\n############  Track Status  #############\n" );
    Track_ShowProc( p, pCard );

    return HI_SUCCESS;
}

HI_S32 AO_DRV_ReadProc( struct seq_file* p, HI_VOID* v )
{
    HI_U32 u32Snd;
    DRV_PROC_ITEM_S *pstProcItem;

    pstProcItem = p->private;

    sscanf(pstProcItem->entry_name, "sound%d", &u32Snd);

    if(u32Snd > AO_MAX_TOTAL_SND_NUM)
    {
        p += seq_printf(p, "Invalid Sound ID:%d.\n", u32Snd);
        return HI_SUCCESS;
    }

    AOShowSndProc( p, (HI_UNF_SND_E)u32Snd );

    return HI_SUCCESS;
}

HI_S32 AO_DRV_WriteProc( struct file* file, const char __user* buf, size_t count, loff_t* ppos )
{
#if 0
    HI_CHAR ProcPara[10];
    HI_CHAR ProcWriteCmd[][5] = {"save", "stop"};
    static HI_U32 g_u32SavePcmCount = 0;
    HI_S8* ps8Path;

    if (count >= 10)
    {
        HI_ERR_AO( "Proc Write Command Err\n" );
        return HI_FAILURE;
    }

    if (HI_SUCCESS != copy_from_user( ProcPara, buf, count ))
    {
        HI_ERR_AO( "Proc Write Command Err\n" );
        return HI_FAILURE;
    }

    if (g_sHIAOState.psAoKernelAddr)
    {
        AO_State_S* pAoUerStatus = ( AO_State_S* )(g_sHIAOState.psAoKernelAddr);
        HIAO_RECPCM_PROC_S* pstRecProc = &(pAoUerStatus->stRecProc);
        ps8Path = pstRecProc->s8PathArray;

        if (strstr( ProcPara, ProcWriteCmd[0] ) && (HI_FALSE == pstRecProc->bRec))
        {
            if (HI_SUCCESS != HI_DRV_FILE_GetStorePath( ps8Path, HIAO_RECFILE_PATH_LEN ))
            {
                HI_ERR_AO( "get procWrite path failed\n" );
                return HI_FAILURE;
            }

            pstRecProc->bRec = HI_TRUE;
            sprintf( ps8Path, "%s/hiao_dbg_%04u.pcm", ps8Path, g_u32SavePcmCount++ );
        }
        else if ((strstr( ProcPara, ProcWriteCmd[1] )) && (HI_TRUE == pstRecProc->bRec))
        {
            pstRecProc->bRec = HI_FALSE;
            strcpy( pstRecProc->s8PathArray, "" );
        }
    }
    else
    {
        HI_ERR_AO( "Hisi AO Dev Not Create!\n" );
    }
#endif


    return count;
}

static HI_S32 AO_RegProc(HI_U32 u32Snd)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S*  pProcItem;

    /* Check parameters */
    if (HI_NULL == s_stAoDrv.pstProcParam)
    {
        return HI_FAILURE;
    }

    /* Create proc */
    sprintf(aszBuf, "sound%d", u32Snd);
    pProcItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_AO("Create ade proc entry fail!\n");
        return HI_FAILURE;
    }

    /* Set functions */
    pProcItem->read  = s_stAoDrv.pstProcParam->pfnReadProc;
    pProcItem->write = s_stAoDrv.pstProcParam->pfnWriteProc;

    HI_INFO_AO("Create Ao proc entry for OK!\n");
    return HI_SUCCESS;
}

static HI_VOID AO_UnRegProc(HI_U32 u32Snd)
{
    HI_CHAR aszBuf[16];
    sprintf(aszBuf, "sound%d", u32Snd);

    HI_DRV_PROC_RemoveModule(aszBuf);
    return;
}

static HI_S32 AO_OpenDev(HI_VOID)
{
    HI_U32 i;

    HI_S32 s32Ret;

    /* Init global track parameter */
    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        atomic_set(&s_stAoDrv.astTrackEntity[i].atmUseCnt, 0);
    }

    s_stAoDrv.u32SndNum = 0;
    /* Init global snd parameter */
    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        atomic_set(&s_stAoDrv.astSndEntity[i].atmUseTotalCnt, 0);
    }

    /* HAL_AOE_Init , Init aoe hardare */
    if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)
    {
        s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)(ADSP_CODE_AOE);
        if (HI_SUCCESS != s32Ret)
        {
            goto err;
        }
    }
    HI_INFO_AO("ADSP_COM_VIR_REG_BASE  0x%x \n", *(HI_U32 *)IO_ADDRESS(ADSP_COM_VIR_REG_BASE));
    HAL_AOE_Init(*(HI_U32 *)IO_ADDRESS(ADSP_COM_VIR_REG_BASE));

    /* HAL_AIAO_Init, Init aiao hardare */
    HAL_AIAO_Init();

#ifdef HI_SND_CAST_SUPPORT
    /* HAL_CAST_Init , Init cast hardare */
    HAL_CAST_Init();
#endif

    /* Set ready flag */
    s_stAoDrv.bReady = HI_TRUE;

    HI_INFO_AO("AO_OpenDev OK.\n");
    return HI_SUCCESS;
err:
    return HI_FAILURE;
}

static HI_S32 AO_CloseDev(HI_VOID)
{
    HI_U32 i,j;
    HI_S32 s32Ret;

    /* Reentrant */
    if (s_stAoDrv.bReady == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    /* Set ready flag */
    s_stAoDrv.bReady = HI_FALSE;
	
#ifdef HI_SND_CAST_SUPPORT
    /* Free all Cast */
    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        {
            if (atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
            {
                AO_Cast_Destory( i );
                AO_Cast_FreeHandle(i);
            }
        }
    }
#endif
    /* Free all track */
    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        {
            if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
            {
                AO_Track_Destory( i );
                AO_Track_FreeHandle(i);
            }
        }
    }

    /* Free all snd */
    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        if (s_stAoDrv.astSndEntity[i].pCard)
        {
            for(j =0; j<SND_MAX_OPEN_NUM;j++)
            {
                if(s_stAoDrv.astSndEntity[i].u32File[j] != 0)
                {
                    AO_Snd_FreeHandle(i, j);
                    if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[i].atmUseTotalCnt))
                    {
                        AO_SND_Close( i );
                    }
                }
            }
        }
    }

    /* HAL_AOE_DeInit */
    if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)
    {
        HAL_AOE_DeInit( );
        s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)(ADSP_CODE_AOE);
    }

#ifdef HI_SND_CAST_SUPPORT
    /* HAL_CAST_DeInit  */
    HAL_CAST_DeInit();
#endif

    /* HAL_AIAO_DeInit */
    HAL_AIAO_DeInit();

    return HI_SUCCESS;
}

static HI_S32 AO_ProcessCmd( struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID *arg )
{
    HI_S32 Ret = HI_SUCCESS;

    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;

    /* Check parameter in this switch */
    switch (cmd)
    {
    case CMD_AO_TRACK_DESTROY:
    case CMD_AO_TRACK_START:
    case CMD_AO_TRACK_STOP:
    case CMD_AO_TRACK_PAUSE:
    case CMD_AO_TRACK_FLUSH:
    {
        if (HI_NULL == arg)
        {
            HI_ERR_AO("CMD %p Bad arg!\n", (HI_VOID*)cmd);
            return HI_ERR_SND_INVALID_PARA;
        }

        hHandle = *(HI_HANDLE *)arg & AO_TRACK_CHNID_MASK;
        CHECK_AO_TRACK(hHandle);
        break;
    }
    case CMD_AO_TRACK_GETDEFATTR:
    case CMD_AO_TRACK_GETATTR:
    case CMD_AO_TRACK_CREATE:
    case CMD_AO_TRACK_SETWEITHT:
    case CMD_AO_TRACK_GETWEITHT:
    case CMD_AO_TRACK_SETSPEEDADJUST:
    case CMD_AO_TRACK_GETDELAYMS:
    case CMD_AO_TRACK_SETEOSFLAG:
    case CMD_AO_TRACK_SENDDATA:
    case CMD_AO_GETSNDDEFOPENATTR:
    case CMD_AO_SND_OPEN:
    case CMD_AO_SND_CLOSE:
    case CMD_AO_SND_SETMUTE:
    case CMD_AO_SND_GETMUTE:
    case CMD_AO_SND_SETHDMIMODE:
    case CMD_AO_SND_SETSPDIFMODE:
    case CMD_AO_SND_SETVOLUME:
    case CMD_AO_SND_GETVOLUME:
    case CMD_AO_SND_SETSAMPLERATE:
    case CMD_AO_SND_GETSAMPLERATE:
    case CMD_AO_SND_SETTRACKMODE:
    case CMD_AO_SND_GETTRACKMODE:
    case CMD_AO_SND_SETSMARTVOLUME:
    case CMD_AO_SND_GETSMARTVOLUME:

#ifdef HI_SND_CAST_SUPPORT
    case CMD_AO_CAST_GETDEFATTR:        
    case CMD_AO_CAST_CREATE:
    case CMD_AO_CAST_DESTROY:
    case CMD_AO_CAST_SETENABLE:
    case CMD_AO_CAST_GETENABLE:        
    case CMD_AO_CAST_SETINFO:
    case CMD_AO_CAST_GETINFO:
    case CMD_AO_CAST_ACQUIREFRAME:
    case CMD_AO_CAST_RELEASEFRAME:
#endif
    {
        if (HI_NULL == arg)
        {
            HI_ERR_AO("CMD %p Bad arg!\n", (HI_VOID*)cmd);
            return HI_ERR_SND_INVALID_PARA;
        }

        break;
    }
    }

    switch (cmd)
    {
        //Snd CMD TYPE(call hal_aiao)
    case CMD_AO_GETSNDDEFOPENATTR:
    {
        Ret = AOGetSndDefOpenAttr((HI_UNF_SND_ATTR_S *)arg);
        break;
    }
    case CMD_AO_SND_OPEN:
    {
        DRV_AO_STATE_S *pAOState = file->private_data;
        AO_SND_Open_Param_S_PTR pstSndParam = ( AO_SND_Open_Param_S_PTR )arg;
        enSound = pstSndParam->enSound;
        CHECK_AO_SNDCARD( enSound );

        if (HI_SUCCESS == AO_Snd_AllocHandle(enSound, file))
        {
            if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
            {
                    Ret = AO_SND_Open( enSound, &pstSndParam->stAttr );
                    if (HI_SUCCESS != Ret)
                    {
                        AO_Snd_FreeHandle(enSound, pAOState->u32FileId);
                        break;
                    }
            }
        }

        atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
        atomic_inc(&pAOState->atmUserOpenCnt[enSound]);
      
        break;
    }
    case CMD_AO_SND_CLOSE:
    {
        DRV_AO_STATE_S *pAOState = file->private_data;
        enSound = *( HI_UNF_SND_E *)arg;
        CHECK_AO_SNDCARD_OPEN( enSound );

        if(atomic_dec_and_test(&pAOState->atmUserOpenCnt[enSound]))
        {
                if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
                {
                    Ret = AO_SND_Close( enSound );
                    if (HI_SUCCESS != Ret)
                    {
                        atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
						atomic_inc(&pAOState->atmUserOpenCnt[enSound]);
                        break;
                    }

                    AO_Snd_FreeHandle(enSound, pAOState->u32FileId);
                }
        }
        else
		{
            atomic_dec(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
        }
        Ret = HI_SUCCESS;
        break;
    }
    case CMD_AO_SND_SETMUTE:
    {
        AO_SND_Mute_Param_S_PTR pstMute = (AO_SND_Mute_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMute->enSound );
        Ret = AO_SND_SetMute(pstMute->enSound, pstMute->enOutPort, pstMute->bMute);
        break;
    }

    case CMD_AO_SND_GETMUTE:
    {
        AO_SND_Mute_Param_S_PTR pstMute = (AO_SND_Mute_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMute->enSound );
        Ret = AO_SND_GetMute(pstMute->enSound, pstMute->enOutPort, &pstMute->bMute);
        break;
    }

    case CMD_AO_SND_SETHDMIMODE:
    {
        AO_SND_HdmiMode_Param_S_PTR pstMode = (AO_SND_HdmiMode_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
        Ret = AO_SND_SetHdmiMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
        break;
    }

    case CMD_AO_SND_SETSPDIFMODE:
    {
        AO_SND_SpdifMode_Param_S_PTR pstMode = (AO_SND_SpdifMode_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
        Ret = AO_SND_SetSpdifMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
        break;
    }

    case CMD_AO_SND_SETVOLUME:
    {
        AO_SND_Volume_Param_S_PTR pstVolume = (AO_SND_Volume_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstVolume->enSound );
        Ret = AO_SND_SetVolume(pstVolume->enSound, pstVolume->enOutPort, pstVolume->stGain);
        break;
    }

    case CMD_AO_SND_GETVOLUME:
    {
        AO_SND_Volume_Param_S_PTR pstVolume = (AO_SND_Volume_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstVolume->enSound );
        Ret = AO_SND_GetVolume(pstVolume->enSound, pstVolume->enOutPort, &pstVolume->stGain);
        break;
    }

    case CMD_AO_SND_SETSAMPLERATE:
    {
        AO_SND_SampleRate_Param_S_PTR pstSampleRate = (AO_SND_SampleRate_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstSampleRate->enSound );
        Ret = AO_SND_SetSampleRate(pstSampleRate->enSound, pstSampleRate->enOutPort, pstSampleRate->enSampleRate);
        break;
    }

    case CMD_AO_SND_GETSAMPLERATE:
    {
        AO_SND_SampleRate_Param_S_PTR pstSampleRate = (AO_SND_SampleRate_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstSampleRate->enSound );
        Ret = AO_SND_GetSampleRate(pstSampleRate->enSound, pstSampleRate->enOutPort, &pstSampleRate->enSampleRate);
        break;
    }

    case CMD_AO_SND_SETTRACKMODE:
    {
        AO_SND_TrackMode_Param_S_PTR pstMode = (AO_SND_TrackMode_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
        Ret = AO_SND_SetTrackMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
        break;
    }

    case CMD_AO_SND_GETTRACKMODE:
    {
        AO_SND_TrackMode_Param_S_PTR pstMode = (AO_SND_TrackMode_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
        Ret = AO_SND_GetTrackMode(pstMode->enSound, pstMode->enOutPort, &pstMode->enMode);
        break;
    }

    case CMD_AO_SND_SETSMARTVOLUME:
    {
        AO_SND_SmartVolume_Param_S_PTR pstSmartVol = (AO_SND_SmartVolume_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstSmartVol->enSound );
        Ret = AO_SND_SetSmartVolume(pstSmartVol->enSound, pstSmartVol->enOutPort, pstSmartVol->bSmartVolume);
        break;
    }

    case CMD_AO_SND_GETSMARTVOLUME:
    {
        AO_SND_SmartVolume_Param_S_PTR pstSmartVol = (AO_SND_SmartVolume_Param_S_PTR)arg;
        CHECK_AO_SNDCARD_OPEN( pstSmartVol->enSound );
        Ret = AO_SND_GetSmartVolume(pstSmartVol->enSound, pstSmartVol->enOutPort, &pstSmartVol->bSmartVolume);
        break;
    }

#ifdef HI_SND_CAST_SUPPORT
    case CMD_AO_CAST_GETDEFATTR:
    {
        HI_UNF_SND_CAST_ATTR_S *pstDefAttr = (HI_UNF_SND_CAST_ATTR_S *)arg;
        Ret = AO_Cast_GetDefAttr(pstDefAttr);
        break;
    }

    case CMD_AO_CAST_CREATE:
    {
        SND_CARD_STATE_S *pCard;
        AO_Cast_Create_Param_S_PTR  pstCastAttr = (AO_Cast_Create_Param_S_PTR)arg;
        
        //HI_ERR_AO("CMD_AO_CAST_CREATE\n");
        CHECK_AO_SNDCARD_OPEN( pstCastAttr->enSound);
        pCard = SND_CARD_GetCard(pstCastAttr->enSound);

        if (HI_SUCCESS == AO_Cast_AllocHandle(&hHandle, file))
        {
            Ret = AO_Cast_Create(pstCastAttr->enSound, &pstCastAttr->stCastAttr, &pstCastAttr->u32ReqSize, hHandle);
            if (HI_SUCCESS != Ret)
            {
                AO_Cast_FreeHandle(hHandle);
                break;
            }
            pstCastAttr->hCast = hHandle;
        }
        break;
    }
    case CMD_AO_CAST_DESTROY:
    {
        HI_HANDLE  hCast = *(HI_HANDLE *)arg;
        //HI_ERR_AO("CMD_AO_CAST_DESTORY\n");
        
        CHECK_AO_CAST_OPEN(hCast);
        Ret = AO_Cast_Destory(hCast);
        if (HI_SUCCESS != Ret)
        {
            break;
        }
        AO_Cast_FreeHandle(hCast);
        break;
    }

    case CMD_AO_CAST_SETINFO:
    {
        AO_Cast_Info_Param_S_PTR pstInfo = (AO_Cast_Info_Param_S_PTR)arg;
        CHECK_AO_CAST_OPEN(pstInfo->hCast);
        
        //HI_ERR_AO("CMD_AO_CAST_SETINFO  CastID=0x%x, u32UserVirtAddr=0x%x\n",pstInfo->hCast, pstInfo->u32UserVirtAddr);
        Ret = AO_Cast_SetInfo(pstInfo->hCast, pstInfo->u32UserVirtAddr);
        break;
    }
    case CMD_AO_CAST_GETINFO:
    {
        AO_Cast_Info_Param_S_PTR pstInfo = (AO_Cast_Info_Param_S_PTR)arg;
        //HI_ERR_AO("pstInfo->hCast=0x%x \n", pstInfo->hCast);
        CHECK_AO_CAST_OPEN(pstInfo->hCast);
        Ret = AO_Cast_GetInfo(pstInfo->hCast, pstInfo);
        //HI_ERR_AO("CMD_AO_CAST_GETINFO  CastID=0x%x, u32UserVirtAddr=0x%x Ret=0x%x\n",pstInfo->hCast, pstInfo->u32UserVirtAddr, Ret);
        break;
    }
    
    case CMD_AO_CAST_SETENABLE:
    {
        AO_Cast_Enable_Param_S_PTR pstEnable = (AO_Cast_Enable_Param_S_PTR)arg;
        CHECK_AO_CAST_OPEN(pstEnable->hCast);
        //HI_ERR_AO("CMD_AO_CAST_SETENABLE  CastID=0x%x, bCastEnable=0x%x\n",pstEnable->hCast, pstEnable->bCastEnable);
        Ret = AO_Cast_SetEnable(pstEnable->hCast, pstEnable->bCastEnable);
        break;
    }

    case CMD_AO_CAST_GETENABLE:
    {
        AO_Cast_Enable_Param_S_PTR pstEnable = (AO_Cast_Enable_Param_S_PTR)arg;
        CHECK_AO_CAST_OPEN(pstEnable->hCast);
        //HI_ERR_AO("CMD_AO_CAST_GETENABLE CastID=0x%x, bCastEnable=0x%x\n",pstEnable->hCast, pstEnable->bCastEnable);
        Ret = AO_Cast_GetEnable(pstEnable->hCast, &pstEnable->bCastEnable);
        break;
    }
    
    case CMD_AO_CAST_ACQUIREFRAME:
    {
        AO_Cast_Data_Param_S_PTR pstCastData = (AO_Cast_Data_Param_S_PTR)arg;
        CHECK_AO_CAST_OPEN(pstCastData->hCast);

        Ret = AO_Cast_ReadData(pstCastData->hCast, pstCastData);
        break;
    }
    case CMD_AO_CAST_RELEASEFRAME:
    {
        AO_Cast_Data_Param_S_PTR pstCastData = (AO_Cast_Data_Param_S_PTR)arg;
        CHECK_AO_CAST_OPEN(pstCastData->hCast);
        //HI_ERR_AO("CMD_AO_CAST_RELEASEFRAME  CastID=0x%x\n",pstCastData->hCast);
        Ret = AO_Cast_ReleseData(pstCastData->hCast, pstCastData);
       
        break;
    }
#endif
    case CMD_AO_SND_ATTACHTRACK:
        break;
    case CMD_AO_SND_DETACHTRACK:
        break;

        //Track CMD TYPE (call hal_aoe)
    case CMD_AO_TRACK_GETDEFATTR:
    {
        HI_UNF_AUDIOTRACK_ATTR_S *pstDefAttr = (HI_UNF_AUDIOTRACK_ATTR_S *)arg;
        Ret = AO_Track_GetDefAttr(pstDefAttr);
        break;
    }

    case CMD_AO_TRACK_GETATTR:
    {
        AO_Track_Attr_Param_S_PTR pstTrackAttr = (AO_Track_Attr_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstTrackAttr->hTrack);
        Ret = AO_Track_GetAttr(pstTrackAttr->hTrack, &pstTrackAttr->stAttr);
        break;
    }

    case CMD_AO_TRACK_CREATE:
    {
        AO_Track_Create_Param_S_PTR pstTrack = (AO_Track_Create_Param_S_PTR)arg;
        if (HI_SUCCESS == AO_Track_AllocHandle(&hHandle, file))
        {
            Ret = AO_Track_Create(pstTrack->enSound, &pstTrack->stAttr, pstTrack->bAlsaTrack,
                                  &pstTrack->stBuf,
                                  hHandle);
            if (HI_SUCCESS != Ret)
            {
                AO_Track_FreeHandle(hHandle);
                break;
            }

            AO_TRACK_SaveSuspendAttr(hHandle, pstTrack);
            pstTrack->hTrack = hHandle;
        }

        break;
    }

    case CMD_AO_TRACK_DESTROY:
    {
        HI_HANDLE hTrack = *(HI_HANDLE *)arg;
        CHECK_AO_TRACK_OPEN(hTrack);
        Ret = AO_Track_Destory( hTrack );
        if (HI_SUCCESS != Ret)
        {
            break;
        }

        AO_Track_FreeHandle(hTrack);

        break;
    }
    case CMD_AO_TRACK_START:
    {
        HI_HANDLE hTrack = *(HI_HANDLE *)arg;
        CHECK_AO_TRACK_OPEN(hTrack);
        Ret = AO_Track_Start(hTrack);
        break;
    }

    case CMD_AO_TRACK_STOP:
    {
        HI_HANDLE hTrack = *(HI_HANDLE *)arg;
        CHECK_AO_TRACK_OPEN(hTrack);
        Ret = AO_Track_Stop(hTrack);
        break;
    }

    case CMD_AO_TRACK_PAUSE:
    {
        HI_HANDLE hTrack = *(HI_HANDLE *)arg;
        CHECK_AO_TRACK_OPEN(hTrack);
        Ret = AO_Track_Pause(hTrack);
        break;
    }

    case CMD_AO_TRACK_FLUSH:
    {
        HI_HANDLE hTrack = *(HI_HANDLE *)arg;
        CHECK_AO_TRACK_OPEN(hTrack);
        Ret = AO_Track_Flush(hTrack);
        break;
    }

    case CMD_AO_TRACK_SENDDATA:
    {
        AO_Track_SendData_Param_S_PTR pstData = (AO_Track_SendData_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstData->hTrack);
        Ret = AO_Track_SendData(pstData->hTrack, &pstData->stAOFrame);
        if (HI_SUCCESS != Ret)
        {
            //to do
        }

        break;
    }

    case CMD_AO_TRACK_SETWEITHT:
    {
        AO_Track_Weight_Param_S_PTR pstWeight = (AO_Track_Weight_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstWeight->hTrack);
        Ret = AO_Track_SetWeight(pstWeight->hTrack, pstWeight->stTrackGain);
        break;
    }

    case CMD_AO_TRACK_GETWEITHT:
    {
        AO_Track_Weight_Param_S_PTR pstWeight = (AO_Track_Weight_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstWeight->hTrack);
        Ret = AO_Track_GetWeight(pstWeight->hTrack, &pstWeight->stTrackGain);
        break;
    }

    case CMD_AO_TRACK_SETSPEEDADJUST:
    {
        AO_Track_SpeedAdjust_Param_S_PTR pstSpeed = (AO_Track_SpeedAdjust_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstSpeed->hTrack);
        Ret = AO_Track_SetSpeedAdjust(pstSpeed->hTrack, pstSpeed->enType, pstSpeed->s32Speed);
        break;
    }

    case CMD_AO_TRACK_GETDELAYMS:
    {
        AO_Track_DelayMs_Param_S_PTR pstDelayMs = (AO_Track_DelayMs_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstDelayMs->hTrack);
        Ret = AO_Track_GetDelayMs(pstDelayMs->hTrack, &pstDelayMs->u32DelayMs);
        break;
    }
    
    case CMD_AO_TRACK_ISBUFEMPTY:
    {
        AO_Track_BufEmpty_Param_S_PTR pstBufEmpty = (AO_Track_BufEmpty_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstBufEmpty->hTrack);
        Ret = AO_Track_IsBufEmpty(pstBufEmpty->hTrack, &pstBufEmpty->bEmpty);
        break;
    }
    
    case CMD_AO_TRACK_SETEOSFLAG:
    {
        AO_Track_EosFlag_Param_S_PTR pstEosFlag = (AO_Track_EosFlag_Param_S_PTR)arg;
        CHECK_AO_TRACK_OPEN(pstEosFlag->hTrack);
        Ret = AO_Track_SetEosFlag(pstEosFlag->hTrack, pstEosFlag->bEosFlag);
        break;
    }

    default:
        Ret = HI_FAILURE;
        {
            HI_WARN_AO("unknown cmd: 0x%x\n", cmd);
        }
        break;
    }

    return Ret;
}

long AO_DRV_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    long s32Ret = HI_SUCCESS;

    s32Ret = down_interruptible(&g_AoMutex);

    //cmd process
    s32Ret = (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, AO_ProcessCmd);

    up(&g_AoMutex);

    return s32Ret;
}

HI_S32 AO_DRV_Open(struct inode *inode, struct file  *filp)
{
    HI_S32 s32Ret;
    HI_U32 cnt; 
    DRV_AO_STATE_S *pAOState = HI_NULL;

    s32Ret = down_interruptible(&g_AoMutex);

    pAOState = HI_KMALLOC(HI_ID_AO, sizeof(DRV_AO_STATE_S), GFP_KERNEL);
    if (!pAOState)
    {
        HI_FATAL_AO("malloc pAOState failed.\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }
    for(cnt = 0; cnt < AO_MAX_TOTAL_SND_NUM; cnt++)
    {
        atomic_set(&(pAOState->atmUserOpenCnt[cnt]), 0);
    }
    if (atomic_inc_return(&s_stAoDrv.atmOpenCnt) == 1)
    {
        s_stAoDrv.pAdspFunc = HI_NULL;

        /* Get adsp functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_ADSP, (HI_VOID**)&s_stAoDrv.pAdspFunc);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("Get adsp function err:%#x!\n", s32Ret);
            goto err;
        }

        /* Init device */
        if (HI_SUCCESS != AO_OpenDev())
        {
            HI_FATAL_AO("AO_OpenDev err!\n" );
            goto err;
        }
    }

    filp->private_data = pAOState;
    
    up(&g_AoMutex);
    return HI_SUCCESS;
err:
    HI_KFREE(HI_ID_AO, pAOState);
    atomic_dec(&s_stAoDrv.atmOpenCnt);
    up(&g_AoMutex);
    return HI_FAILURE;
}

HI_S32 AO_DRV_Release(struct inode *inode, struct file  *filp)
{
    HI_U32 i;
    HI_U32 j;

    long s32Ret = HI_SUCCESS;
    DRV_AO_STATE_S *pAOState = filp->private_data;

    s32Ret = down_interruptible(&g_AoMutex);

    /* Not the last close, only close the track & snd match with the 'filp' */
    if (atomic_dec_return(&s_stAoDrv.atmOpenCnt) != 0)
    {
#ifdef HI_SND_CAST_SUPPORT	
        /* Free all Cast */
        for (i = 0; i < AO_MAX_CAST_NUM; i++)
        {
            if (s_stAoDrv.astCastEntity[i].u32File == ((HI_U32)filp))
            {
                if (atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
                {
                    if (HI_SUCCESS != AO_Cast_Destory(i))
                    {
                        atomic_inc(&s_stAoDrv.atmOpenCnt);
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }
                    AO_Cast_FreeHandle(i);
                }
            }
        }
#endif
        /* Free all track */
        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (s_stAoDrv.astTrackEntity[i].u32File == ((HI_U32)filp))
            {
                if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
                {
                    if (HI_SUCCESS != AO_Track_Destory(i))
                    {
                        atomic_inc(&s_stAoDrv.atmOpenCnt);
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }

                    AO_Track_FreeHandle(i);
                }
            }
        }

        /* Free all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            for(j = 0; j < SND_MAX_OPEN_NUM; j++)
            {
                if (s_stAoDrv.astSndEntity[i].u32File[j] == ((HI_U32)filp))
                {
                        HI_U32 u32UserOpenCnt = 0;
                        
                        if (s_stAoDrv.astSndEntity[i].pCard)
                        {
                           u32UserOpenCnt = atomic_read(&pAOState->atmUserOpenCnt[i]);
                            if(atomic_sub_and_test(u32UserOpenCnt, &s_stAoDrv.astSndEntity[i].atmUseTotalCnt))
                            {
                                if (HI_SUCCESS != AO_SND_Close(i))
                                {
                                    atomic_inc(&s_stAoDrv.astSndEntity[i].atmUseTotalCnt);
                                    atomic_inc(&s_stAoDrv.atmOpenCnt);
                                    up(&g_AoMutex);
                                    return HI_FAILURE;
                                }
                            }

                            AO_Snd_FreeHandle(i, j);
                        }
                }
           }
            
        }
    }
    /* Last close */
    else
    {
        AO_CloseDev();
    }

    HI_KFREE(HI_ID_AO, pAOState);
    up(&g_AoMutex);
    return HI_SUCCESS;
}

#ifdef HI_ALSA_AO_SUPPORT
/* drv open  kernel intf */
HI_S32 AO_DRV_Kopen(struct file  *file)
{
    HI_S32 s32Ret;
    HI_U32 cnt; 
    DRV_AO_STATE_S *pAOState = HI_NULL;

    s32Ret = down_interruptible(&g_AoMutex);

    pAOState = HI_KMALLOC(HI_ID_AO, sizeof(DRV_AO_STATE_S), GFP_KERNEL);
    if (!pAOState)
    {
        HI_FATAL_AO("malloc pAOState failed.\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }
    for(cnt = 0; cnt < AO_MAX_TOTAL_SND_NUM; cnt++)
	{
        atomic_set(&(pAOState->atmUserOpenCnt[cnt]), 0);
	}
    if (atomic_inc_return(&s_stAoDrv.atmOpenCnt) == 1)
    {
        s_stAoDrv.pAdspFunc = HI_NULL;

        /* Get adsp functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_ADSP, (HI_VOID**)&s_stAoDrv.pAdspFunc);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("Get adsp function err:%#x!\n", s32Ret);
            goto err;
        }

        /* Init device */
        if (HI_SUCCESS != AO_OpenDev())
        {
            HI_FATAL_AO("AO_OpenDev err!\n" );
            goto err;
        }
    }

    file->private_data = pAOState;

    up(&g_AoMutex);
    return HI_SUCCESS;
err:
    HI_KFREE(HI_ID_AO, pAOState);
    atomic_dec(&s_stAoDrv.atmOpenCnt);
    up(&g_AoMutex);
    return HI_FAILURE;
}
/*drv close kernel intf */
HI_S32 AO_DRV_Krelease(struct file  *file)
{
    HI_U32 i;
    HI_U32 j;
    long s32Ret = HI_SUCCESS;
    DRV_AO_STATE_S *pAOState = file->private_data;

    s32Ret = down_interruptible(&g_AoMutex);

    /* Not the last close, only close the track & snd match with the 'filp' */
    if (atomic_dec_return(&s_stAoDrv.atmOpenCnt) != 0)
    {
        /* Free all track */
        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (s_stAoDrv.astTrackEntity[i].u32File == ((HI_U32)file))
            {
                if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
                {
                    if (HI_SUCCESS != AO_Track_Destory(i))
                    {
                        atomic_inc(&s_stAoDrv.atmOpenCnt);
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }

                    AO_Track_FreeHandle(i);
                }
            }
        }

        /* Free all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            for(j = 0; j < SND_MAX_OPEN_NUM; j++)
            {
                if (s_stAoDrv.astSndEntity[i].u32File[j] == ((HI_U32)file))
                {
                        HI_U32 u32UserOpenCnt = 0;
                        if (s_stAoDrv.astSndEntity[i].pCard)
                        {
                           u32UserOpenCnt = atomic_read(&pAOState->atmUserOpenCnt[i]);
                            if(atomic_sub_and_test(u32UserOpenCnt, &s_stAoDrv.astSndEntity[i].atmUseTotalCnt))
                            {
                                if (HI_SUCCESS != AO_SND_Close(i))
                                {
                                    atomic_inc(&s_stAoDrv.astSndEntity[i].atmUseTotalCnt);
                                    atomic_inc(&s_stAoDrv.atmOpenCnt);
                                    up(&g_AoMutex);
                                    return HI_FAILURE;
                                }
                            }

                            AO_Snd_FreeHandle(i, j);
                        }
                }
           }
            
        }
    }
    /* Last close */
    else
    {
        AO_CloseDev();
    }

    up(&g_AoMutex);
    return HI_SUCCESS;
}
#endif

HI_S32 AO_DRV_RegisterProc(AO_REGISTER_PARAM_S * pstParam)
{
    HI_U32 i;
    
    /* Check parameters */
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    s_stAoDrv.pstProcParam = pstParam;

    /* Create proc */
    for(i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        AO_RegProc(i);
    }

    return HI_SUCCESS;
}

HI_VOID AO_DRV_UnregisterProc(HI_VOID)
{
    /* Unregister proc */
    HI_U32 i;
    for(i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        AO_UnRegProc(i);
    }

    /* Clear param */
    s_stAoDrv.pstProcParam = HI_NULL;
    return;
}

#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
static HI_S32 AO_TRACK_GetSettings(HI_HANDLE hTrack, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S *pCard;
    
    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(hTrack);
    if(pCard)
    {
        return TRACK_GetSetting(pCard, hTrack, pstSndSettings);
    }
    else
    {
        HI_ERR_AO("Track(%d) don't attach card!\n",hTrack);
        return HI_FAILURE;
    }
}

static HI_S32 AO_TRACK_RestoreSettings(HI_HANDLE hTrack, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S *pCard;
    
    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(hTrack);
    if(pCard)
    {
        return TRACK_RestoreSetting(pCard, hTrack, pstSndSettings);
    }
    else
    {
        HI_ERR_AO("Track(%d) don't attach card!\n",hTrack);
        return HI_FAILURE;
    }
}

static HI_S32 AO_SND_GetSettings(HI_UNF_SND_E enSound, SND_CARD_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);
    
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pstSndSettings);
    
    return SND_GetSetting(pCard, pstSndSettings);
}

static HI_S32 AO_SND_RestoreSettings(HI_UNF_SND_E enSound, SND_CARD_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S *pCard = SND_CARD_GetCard(enSound);
    
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pstSndSettings);
    
    return SND_RestoreSetting(pCard, pstSndSettings);
}

#endif

HI_S32 AO_DRV_Suspend(PM_BASEDEV_S * pdev,
                      pm_message_t   state)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_S32 i;
    long s32Ret = HI_SUCCESS;
    HI_FATAL_AO("entering\n");

    s32Ret = down_interruptible(&g_AoMutex);
    if(HI_TRUE == s_stAoDrv.bReady)
    {
        /* Destory all track */
        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
            {
                /* Store track settings */
                AO_TRACK_GetSettings(i, &s_stAoDrv.astTrackEntity[i].stSuspendAttr);

                /* Destory track */
                s32Ret = AO_Track_Destory(i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Track_Destory fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
            }
        }

        /* Destory all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            if (s_stAoDrv.astSndEntity[i].pCard)
            {
                /* Store snd settings */
                AO_SND_GetSettings(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr);

                /* Destory snd */
                s32Ret = AO_SND_Close(i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_SND_Close fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
            }
        }


        HAL_AIAO_FreeIsr();
        if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)
        {
            s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)(ADSP_CODE_AOE);
        }
    }

    up(&g_AoMutex);
#endif
    HI_FATAL_AO("ok\n");
    return HI_SUCCESS;
}

HI_S32 AO_DRV_Resume(PM_BASEDEV_S * pdev)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_S32 i;
    long s32Ret = HI_SUCCESS;
    HI_FATAL_AO("entering\n");

    s32Ret = down_interruptible(&g_AoMutex);

    if (HI_TRUE == s_stAoDrv.bReady)
    {
        /* HAL_AOE_Init , Init aoe hardare */
        if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)
        {
            s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)(ADSP_CODE_AOE);
            if (HI_SUCCESS != s32Ret)
            {
                HI_FATAL_AO("load aoe fail\n");
                up(&g_AoMutex);
                return HI_FAILURE;
            }
        }

        /* AIAO hardare reset */
        s32Ret = HAL_AIAO_HwReset();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AIAO hw reset fail\n");
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        s32Ret = HAL_AIAO_RequestIsr();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AIAO request isr fail\n");
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        HAL_AIAO_SetTopInt(0xffffffff);  /* enable all top interrupt */
  
        /* Restore all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            if (s_stAoDrv.astSndEntity[i].pCard)
            {
                /* Recreate snd */
                s32Ret = AO_SND_Open(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr.stUserOpenParam);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_SND_Open fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }

                /* Restore snd settings*/
                AO_SND_RestoreSettings(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr);
            }
        }

        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
            {
                HI_UNF_SND_E enSound = s_stAoDrv.astTrackEntity[i].stSuspendAttr.enSound;
                HI_UNF_AUDIOTRACK_ATTR_S *pstAttr = &s_stAoDrv.astTrackEntity[i].stSuspendAttr.stTrackAttr;
                HI_BOOL bAlsaTrack = s_stAoDrv.astTrackEntity[i].stSuspendAttr.bAlsaTrack;
                AO_BUF_ATTR_S *pstBuf = &s_stAoDrv.astTrackEntity[i].stSuspendAttr.stBufAttr;

                /* Recreate track  */
                s32Ret = AO_Track_Create(enSound, pstAttr, bAlsaTrack, pstBuf, i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Track_Create(%d) fail\n", i);
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }

                /* Restore track settings*/
                AO_TRACK_RestoreSettings(i, &s_stAoDrv.astTrackEntity[i].stSuspendAttr);
            }
        }
    }
    
    up(&g_AoMutex);
#endif

    HI_FATAL_AO("ok\n");
    return HI_SUCCESS;
}

HI_S32 AO_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AoMutex);
    s32Ret = HI_DRV_MODULE_Register(HI_ID_AO, AO_NAME,
                                    (HI_VOID*)&s_stAoDrv.stExtFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("Reg module fail:%#x!\n", s32Ret);
        up(&g_AoMutex);
        return s32Ret;
    }

#ifdef ENA_AO_IRQ_PROC
    /* register ade ISR */
    if (0
        != request_irq(AO_IRQ_NUM, AO_IntVdmProc,
                       IRQF_DISABLED, "aiao",
                       HI_NULL))
    {
        HI_FATAL_AO("FATAL: request_irq for VDI VDM err!\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }
#endif


    up(&g_AoMutex);
    return HI_SUCCESS;
}

HI_VOID AO_DRV_Exit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AoMutex);
#ifdef ENA_AO_IRQ_PROC
    free_irq(AO_IRQ_NUM, HI_NULL);
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_AO);

    up(&g_AoMutex);
    return;
}

#ifdef __cplusplus
 #if __cplusplus
 #endif
#endif /* End of #ifdef __cplusplus */
