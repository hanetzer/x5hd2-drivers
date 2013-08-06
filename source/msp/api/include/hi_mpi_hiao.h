/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name             :   hi_mpi_hiao.h
  Version               :   Initial Draft
  Author                :   Hisilicon multimedia software group
  Created               :   2010/10/12
  Last Modified         :
  Description           :
  Function List         :
  History               :
  1.Date                :   2010/10/12
    Author              :   z40717
Modification            :   Created file
******************************************************************************/

#ifndef  __MPI_HIAO_H__
#define  __MPI_HIAO_H__

#include "hi_type.h"
#include "hi_unf_sound.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

/* HIAO support two Channel Audio Input */
#ifndef HI_SND_SLAVEPORT_MAXNUM
#define HI_SND_SLAVEPORT_MAXNUM 1
#endif
#define MAX_HIAO_INPUT_PORT_NUMBER (HI_SND_SLAVEPORT_MAXNUM + 1)  /* main and slave */
//#define MAX_HIAO_INPUT_PORT_NUMBER 16  /* main and slave */
#define HIAO_MAIN_PORD_ID 0  /* support PCM/IEC61937 Data Input at the same time */
#define HIAO_SLAV_PORD_ID 1  /* support PCM stereo Input */

#define HIAO_MAX_VITRUAL_TRACK_NUMBER 2 //max vitural channel number per trual channel
#define HIAO_MAX_TOTAL_TRACK_NUMBER MAX_HIAO_INPUT_PORT_NUMBER * (HIAO_MAX_VITRUAL_TRACK_NUMBER + 1)

#define HIAO_MIXER_ENGINE_SUPPORT
#ifdef  HIAO_MIXER_ENGINE_SUPPORT
 #define HIAO_MAX_MIXER_NUMBER (HI_SND_SLAVEPORT_MAXNUM)
 #define HIAO_MAX_MIXER_BUFFER_SIZE (1024 * 1024)
 #define HIAO_MIN_MIXER_BUFFER_SIZE (1024 * 16)

 #define HIAO_PortID2MixerID(PortID)  (PortID - HIAO_SLAV_PORD_ID)
 #define HIAO_MixerID2PortID(MixerID) (MixerID + HIAO_SLAV_PORD_ID)
 
/* Audio Mixer attribute                                               			*/
typedef struct hiHIAO_MIXER_ATTR_S
{
    HI_U32 u32MixerWeight;   /* 0~99 */
    HI_U32 u32MixerBufSize;   /* HIAO_MIN_MIXER_BUFFER_SIZE ~ HIAO_MAX_MIXER_BUFFER_SIZE */
} HIAO_MIXER_ATTR_S;

typedef struct hiMixer_STATUSINFO_S
{
    HI_S32 u32MixerID;

    /* Mixer state */
    HI_S32 s32Working;   /* 0 : stop, 1: pause, 2: running */

    /* Buffer status */
    HI_U32  u32BufferSize;           /* Total buffer size, in the unit of byte.*/
    HI_U32  u32BufferByteLeft;     /* LeftBytes, in the unit of byte.*/
    HI_BOOL bDataNotEnough;      /* HI_FALSE : data is enough, HI_TRUE: data is not enough,need more data */
    HI_U32  u32BufDurationMs;
} Mixer_STATUSINFO_S;

#endif

#define VIRTUAL_MAX_STORED_PTS_NUM   (2*1024)   //(512 *1024) / 320  (VIRTUAL_MAX_OUTBUF_SIZE/MIN_FRAME_SIZE)

typedef struct hiVIRTUAL_PTS_S
{
    HI_U32 u32PtsMs;        /* Play Time Stamp  */
    HI_U32 u32BegPtr;      /* Stream start address of PTS */
    HI_U32 u32EndPtr;      /* Stream end   address of PTS */
} VIRTUAL_PTS_S;

typedef struct hiVIRTUAL_PTS_QUE_S
{
    HI_U32   u32LastPtsMs;
    HI_U32   u32PTSreadIdx;     /* PTS buffer read  ptr */
    HI_U32   u32PTSwriteIdx;    /* PTS buffer write ptr */
    VIRTUAL_PTS_S stPTSArry[VIRTUAL_MAX_STORED_PTS_NUM];
} VIRTUAL_PTS_QUE_S;

typedef struct hiHIAO_TRACK_BUFFUR_S
{
    /*buf ptr info*/
    HI_U8 *      pu8BufBase;
    HI_U32       u32Start;
    HI_U32       u32End;
    HI_U32       u32Write;
    HI_U32       u32Read;
    
    /*buf data info*/
    HI_U32       u32Channel;
    HI_S32       s32BitPerSample;
    HI_U32       u32SampleRate;
    HI_U32       u32PcmSamplesPerFrame;
    
    VIRTUAL_PTS_QUE_S stPTSQue;
}HIAO_TRACK_BUFFUR_S;

typedef struct hiTRACK_S
{
    HI_U32 u32PortID;
    HI_UNF_SND_TRACK_TYPE_E enTrackType;
    HI_U32 u32OutputBufSize;
    HI_U32                  u32FadeinMs;
    HI_U32                  u32FadeoutMs;
    HI_U32                  u32BufLevelMs;
    HIAO_TRACK_BUFFUR_S *pstTrackBuf;
}TRACK_S;

typedef struct tagMPI_TRACK_INFO_S
{
    HI_UNF_SND_TRACK_TYPE_E    enTrackType;
}MPI_TRACK_INFO_S;

#define BYTE_PER_SAMPLE(BitPerSample) (16==BitPerSample? (2) : (4))

/* the type of Adjust Audio */
typedef enum hiHI_SND_SPEEDADJUST_TYPE_E
{
    HI_UNF_SND_SPEEDADJUST_SRC,     /**<samplerate convert */
    HI_UNF_SND_SPEEDADJUST_PITCH,   /**<Sola speedadjust, reversed */
    HI_UNF_SND_SPEEDADJUST_MUTE,    /**<mute */
    HI_UNF_SND_SPEEDADJUST_BUTT 
} HI_SND_SPEEDADJUST_TYPE_E;
typedef enum hiMPI_HIAO_INTERFACE_E
{
    HI_MPI_HIAO_INTERFACE_I2S,
    HI_MPI_HIAO_INTERFACE_SPDIF, 
    HI_MPI_HIAO_INTERFACE_HBR, 
    HI_MPI_HIAO_INTERFACE_BUTT
}HI_MPI_HIAO_INTERFACE_E;

/* Audio output attribute                                               			*/
typedef struct hiHIAO_ATTR_S
{
    HI_BOOL             bEnableLineout; /* enable line output or not, Lineout band to AUD_OUTPORT_DAC0	*/
    HI_BOOL             bEnableSpdifout; /* enable spdif outpot or not */
    HI_BOOL             bEnableSpdifPassThrough; /* enable spdif pass through or not */
    HI_BOOL             bEnableAutoSRC; /* enable automatic adaption of sample rate or not */
#ifdef  HI_SND_SMARTVOLUME_SUPPORT
	HI_BOOL 			bEnableSmartvolume; /* enable smart volume effect */
#endif
	HI_SND_SPEEDADJUST_TYPE_E enSpeedAdjust;
    HI_S32              s32SpeedAdjust; /* slight speed adjust, 0 is no adjusting, unit: 0.1% */
	HI_U32 				u32TotalAddPcmSamples;
    HI_BOOL             bMute;              /* HI_TRUE:mute, HI_FALSE: unmute			   	*/
    HI_BOOL             bADACMute;              /* HI_TRUE:mute, HI_FALSE: unmute			   	*/
    HI_U32              u32Volume;       /* 0~100 */
	HI_S32              s32AbsVolume;       /* absolute volume vary from-70dB to 0dB */
    HI_U32              u32MixMasterWeight; /* 0~100 */
    HI_U32              u32MixSlaveWeight; /* 0~100 */
    HI_U32              u32MainFadeInTime;    /* Main ourport fade in time. unit: ms */
    HI_U32              u32MainFadeOutTime;  /* Main ourport fade out time. unit: ms */
    HI_U32              u32SlaveFadeInTime;   /* Slave fade in time. unit: ms */
    HI_U32              u32SlaveFadeOutTime; /* Slave fade out time. unit: ms */
    HI_U32              u32BitDepth;     /* Bit per sample */
    HI_U32              u32PcmSampleRate; /* PCM playback samplerate */
    HI_U32              u32SpdifSampleRate; /* SPDIF playback samplerate*/
    HI_UNF_TRACK_MODE_E enTrackMode;   /* Audio playback track mode */
    HI_UNF_TRACK_MODE_E enUserTrackMode;   /* Audio playback user track mode */
    HI_U32              u32CpcmOutputBufSize; /* Audio playback buffer size, unit: byte, note: size must 8 time word32 */
    HI_U32              u32DpcmOutputBufSize; /* Audio playback buffer size, unit: byte, note: size must 8 time word32 */
    HI_U32              u32MpcmOutputBufSize; /* Audio playback buffer size, unit: byte, note: size must 8 time word32 */
    HI_BOOL             bMasterPause;              /* support independent pause.HIAO_MAIN_PORD_ID, HI_TRUE:pause, HI_FALSE: unpause			   	*/
    HI_BOOL             bSlavePause;              /* support independent pause.HIAO_SLAV_PORD_ID,HI_TRUE:pause, HI_FALSE: unpause			   	*/
    HI_S32              s32PcmFifoThreshold;          /*  0=<s32PcmFifoThreshold < 10, defatult 6, no threshold: 0 */
    HI_S32              s32DownmixMode;  /* 0: none  1: auto downmix to stereo 2: auto downmix to stereo + multich(max 7.1) */
    HI_BOOL bEnableHBRPassThrough; /* enable HBR pass through or not */
#if  defined (CHIP_TYPE_hi3712)
    HI_U32				u32Spdif1SampleRate; /*HDMI SPDIF passthrough samplerate */
    HI_U32              u32S2pcmOutputBufSize;
#endif
                                       
    HI_BOOL bDiscardIec61937LbrData;   /* HI_TRUE: dont discard, HI_FALSE: discard,default: HI_FALSE */
    HI_BOOL bDiscardIec61937HbrData;	/* HI_TRUE: dont discard, HI_FALSE: discard,default: HI_FALSE */
	HI_BOOL bSpdifCompatible;
	HI_BOOL bUserSettingSPDIFPinOut;

    HI_S32 	s32IEC61937DataFormat;
	HI_U32 	u32UserSettingSampleRate;
    HI_MPI_HIAO_INTERFACE_E enHDMISoundIntf;
	HI_UNF_SAMPLE_RATE_E  enHDMISampleRate;
    HI_U32  u32HDMIChannels;
    HI_U32                    u32PortNum;
    HI_UNF_SND_OUTPORT_S stOutport[HI_UNF_SND_OUTPUTPORT_MAX];
} HIAO_ATTR_S;


#ifdef  HIAO_MIXER_ENGINE_SUPPORT
HI_S32   HI_MPI_HIAO_SendTrackData(HI_HANDLE hTrack, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
#endif

/* cut it off next version */
//#define ORIGINAL_HIAO_PCM_SUPPORT

HI_S32   HI_MPI_HIAO_GetOpenDefaultParam(HIAO_ATTR_S *pstDefalutPara);

/* pstPara:     reserve                               */
HI_S32   HI_MPI_HIAO_Init(HI_VOID);

HI_S32   HI_MPI_HIAO_DeInit(HI_VOID);

HI_S32   HI_MPI_HIAO_Open(const HI_VOID *pstPara);

HI_S32   HI_MPI_HIAO_Close(HI_VOID);

HI_S32   HI_MPI_HIAO_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr);
HI_S32   HI_MPI_HIAO_CreateTrack(const HI_UNF_AUDIOTRACK_ATTR_S *pTrackAttr, HI_HANDLE *phTrack);

HI_S32   HI_MPI_HIAO_DestroyTrack(HI_HANDLE hTrack);

HI_S32   HI_MPI_HIAO_AcquireTrackFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

HI_S32   HI_MPI_HIAO_ReleaseTrackFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

HI_S32   HI_MPI_HIAO_GetTrackInfo(HI_HANDLE hTrack, MPI_TRACK_INFO_S *pstTrackInfo);


HI_S32   HI_MPI_HIAO_SetLineOut(HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_SetSpdif(HI_BOOL bEnable);


HI_S32   HI_MPI_HIAO_SetPassThrough(HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_SetSampleRate(HI_UNF_SAMPLE_RATE_E enRate);

HI_S32   HI_MPI_HIAO_GetSampleRate(HI_UNF_SAMPLE_RATE_E *penRate);
HI_S32   HI_MPI_HIAO_SetAutoSRC(HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_SetSmartVolume(HI_UNF_SND_OUTPUTPORT_E enOutPort,HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_GetSmartVolume(HI_UNF_SND_OUTPUTPORT_E enOutPort,HI_BOOL *pbEnable);
HI_S32   HI_MPI_HIAO_TriggerSmartVolume(HI_VOID);

HI_S32   HI_MPI_HIAO_SetEosFlag(HI_HANDLE hTrack, HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_SetMute(HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute);

HI_S32   HI_MPI_HIAO_GetMute(HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL *pbMute);

HI_S32   HI_MPI_HIAO_SetTrackMode(HI_UNF_SND_OUTPUTPORT_E enOutPort,HI_UNF_TRACK_MODE_E enMode);

HI_S32   HI_MPI_HIAO_GetTrackMode(HI_UNF_SND_OUTPUTPORT_E enOutPort,HI_UNF_TRACK_MODE_E *penMode);

HI_S32   HI_MPI_HIAO_SetVolume(HI_UNF_SND_OUTPUTPORT_E enOutPort, const HI_UNF_SND_GAIN_ATTR_S stGain);

HI_S32   HI_MPI_HIAO_GetVolume(HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S *pstGain);

HI_S32   HI_MPI_HIAO_SetTrackWeight(HI_HANDLE hTrack,const HI_UNF_SND_GAIN_ATTR_S stMixWeightGain);

HI_S32   HI_MPI_HIAO_GetTrackWeight(HI_HANDLE hTrack,HI_UNF_SND_GAIN_ATTR_S *pstMixWeightGain);

HI_S32   HI_MPI_HIAO_SetSpeedAdjust(HI_S32 s32Speed, HI_SND_SPEEDADJUST_TYPE_E eType);

HI_S32   HI_MPI_HIAO_SendData(const HI_UNF_AO_FRAMEINFO_S *pstAOFrame, const HI_HANDLE hTrack);

HI_S32   HI_MPI_HIAO_Reset(const HI_HANDLE hTrack);

HI_S32   HI_MPI_HIAO_SetPause(const HI_HANDLE hTrack, HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_GetDelayMs(const HI_HANDLE hTrack, HI_U32 *pDelay);

HI_S32   HI_MPI_HIAO_GetCurrentPlayPTS(HI_U32 *pu32PtsMs);

HI_S32   HI_MPI_HIAO_GetDataSize(HI_U32 *pu32CpcmSize, HI_U32 *pu32DpcmSize, HI_U32 *pu32MpcmSize);

HI_S32   HI_MPI_HIAO_SetSpdifPinOut(HI_BOOL bEnable);

HI_S32   HI_MPI_HIAO_SetHdmiPassThroughMode(HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable);
HI_S32   HI_MPI_HIAO_GetDefaultOpenAttr(HI_UNF_SND_ATTR_S *pstAttr);
/* CBB Debug Interface */
#define HIAO_VERIFICATION_SUPPORT
#ifdef  HIAO_VERIFICATION_SUPPORT

typedef struct hiMPI_HIAO_PROC_INFO_S
{
    /* current CPCM PTS                                                     */
    HI_U32 u32CurrentPTS_C;

    /* current DPCM PTS                                                     */
    HI_U32 u32CurrentPTS_D;

    /* current MPCM PTS                                                     */
    HI_U32 u32CurrentPTS_M;

    /* current CPCM(Underflow) counter                                      */
    HI_U32 u32FifoC_UnderflowCounter;

    /* current DPCM(Underflow) counter                                      */
    HI_U32 u32FifoD_UnderflowCounter;

    /* current MPCM(Underflow) counter                                      */
    HI_U32 u32FifoM_UnderflowCounter;

#if  defined (CHIP_TYPE_hi3712)
    HI_U32 u32CurrentPTS_S2;

    HI_U32 u32FifoS2_UnderflowCounter;
#endif
}  MPI_HIAO_PROC_INFO_S;

HI_S32   HI_MPI_HIAO_GetProcInfo(MPI_HIAO_PROC_INFO_S *pstProcInfo);
HI_S32   HI_MPI_HIAO_SPDIF_ReadReg(HI_U32 uRegAddr, HI_U32 *pu32Reg);
HI_S32   HI_MPI_HIAO_SPDIF_WriteReg(HI_U32 uRegAddr, HI_U32 u32Reg);

HI_S32   HI_MPI_HIAO_SPDIF1_WriteReg(HI_U32 uRegAddr, HI_U32 u32Reg);

HI_S32   HI_MPI_HIAO_ReadReg(HI_U32 uRegAddr, HI_U32 *pu32Reg);
HI_S32   HI_MPI_HIAO_WriteReg(HI_U32 uRegAddr, HI_U32 u32Reg);

#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif //__MPI_AO_H__
