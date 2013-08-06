/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_priv_hdmi.h
  Version       : Initial Draft
  Author        : q46153 l00168554
  Created       : 2010/6/15
  Last Modified :
  Description   : hdmi ioctl and HDMI API common data structs

******************************************************************************/

#ifndef  __HI_DRV_HDMI_H__
#define  __HI_DRV_HDMI_H__

//#include "hi_common_id.h"
#include "hi_module.h"
//#include "hi_common_log.h"
#include "hi_debug.h"

#include "hi_unf_hdmi.h"
#include "hi_error_mpi.h"
#include "hi_mpi_hiao.h"

#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif

//#define HDMI_PHY_BASE_ADDR 0x10170000L
#define HDMI_HARDWARE_RESET_ADDR 0xf8a2210cL
#define HDMI_TX_PHY_BASE_ADDR 0xf8ce0000L
//#define HDMI_TX_HARDWARE_RESET_ADDR 0xf8a2210cL


/*
**HDMI Debug
*/
#ifndef CONFIG_SUPPORT_CA_RELEASE
#define HI_FATAL_HDMI(fmt...)       HI_FATAL_PRINT  (HI_ID_HDMI, fmt)
#define HI_ERR_HDMI(fmt...)         HI_ERR_PRINT    (HI_ID_HDMI, fmt)
#define HI_WARN_HDMI(fmt...)        HI_WARN_PRINT   (HI_ID_HDMI, fmt)
#define HI_INFO_HDMI(fmt...)        HI_INFO_PRINT   (HI_ID_HDMI, fmt)

#define debug_printk(fmt,args...) // printk(fmt,##args)
#else

#define HI_FATAL_HDMI(fmt...) 
#define HI_ERR_HDMI(fmt...) 
#define HI_WARN_HDMI(fmt...)
#define HI_INFO_HDMI(fmt...)
#define debug_printk(fmt,args...)  

#endif

/*hdmi audio interface */
typedef enum  hiHDMI_AUDIOINTERFACE_E
{
    HDMI_AUDIO_INTERFACE_I2S,
    HDMI_AUDIO_INTERFACE_SPDIF, 
    HDMI_AUDIO_INTERFACE_HBR, 
    HDMI_AUDIO_INTERFACE_BUTT
}HDMI_AUDIOINTERFACE_E;

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
/*hdmi struct */
typedef struct hiHDMI_ATTR_S
{
	HI_UNF_HDMI_ATTR_S 		stHDMIAttr;
	//HI_MPI_HIAO_INTERFACE_E  enSoundIntf;
    HDMI_AUDIO_ATTR_S       stAudAttr;
    HDMI_VIDEO_ATTR_S       stVidAttr;    
}HDMI_ATTR_S;


/*In order to extern ,so we define struct*/
typedef struct hiHDMI_AUDIO_ATTR_S
{
	HI_MPI_HIAO_INTERFACE_E  enSoundIntf;
	HI_UNF_SAMPLE_RATE_E 	enSampleRate;
	HI_U32 					u32Channels;
    HI_U8                   u8DownSampleParm;  
    HI_UNF_BIT_DEPTH_E      enBitDepth;
    HI_U8                   u8I2SCtlVbit;  
}HDMI_AUDIO_ATTR_S;

/*In order to extern ,so we define struct*/
typedef struct hiHDMI_VIDEO_ATTR_S
{
    HI_UNF_ENC_FMT_E        enVideoFmt; 
    HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;
}HDMI_VIDEO_ATTR_S;

#else
/*hdmi struct */
typedef struct hiHDMI_ATTR_S
{
	HI_UNF_HDMI_ATTR_S 		stAttr;
	HDMI_AUDIOINTERFACE_E  enSoundIntf;
}HDMI_ATTR_S;

typedef struct hiHDMI_AUDIO_ATTR_S
{
	HDMI_AUDIOINTERFACE_E   enSoundIntf;
	HI_UNF_SAMPLE_RATE_E 	enSampleRate;
	HI_U32 					u32Channels;
}HDMI_AUDIO_ATTR_S;

typedef struct hiHDMI_AUDIO_CAPABILITY_S
{
    HI_BOOL             bSupportHdmi;             /**<The Device suppot HDMI or not,the device is DVI when nonsupport HDMI*//**<CNcomment:设备是否支持HDMI，如果不支持，则为DVI设备.*/
    HI_BOOL             bAudioFmtSupported[HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT]; /**<Audio capability,reference EIA-CEA-861-D,table 37,HI_TRUE:support this Audio type;HI_FALSE,nonsupport this Audio type*//**<CNcomment:音频能力集, 请参考EIA-CEA-861-D 表37;HI_TRUE表示支持这种显示格式，HI_FALSE表示不支持 */
    HI_U32              u32AudioSampleRateSupported[HI_UNF_HDMI_MAX_AUDIO_SMPRATE_COUNT]; /**<PCM smprate capability,0: illegal value,other is support PCM smprate *//**<CNcomment:PCM音频采样率能力集，0为非法值，其他为支持的音频采样率 */
    HI_U32              u32MaxPcmChannels;        /**<Audio max PCM Channels number*//**CNcomment:音频最大的PCM通道数 */
    HI_U8               u8Speaker;                /**<Speaker location,please reference EIA-CEA-D the definition of SpekearDATABlock*//**<CNcomment:扬声器位置，请参考EIA-CEA-861-D中SpeakerDATABlock的定义 */
    HI_U8               u8Audio_Latency;          /**<the latency of audio*//**<CNcomment:音频延时 */
}HDMI_AUDIO_CAPABILITY_S;


#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/


/*
** HDMI IOCTL Data Structure
*/
typedef struct hiHDMI_OPEN_S
{
	HI_UNF_HDMI_ID_E                  enHdmi;
    HI_UNF_HDMI_DEFAULT_ACTION_E      enDefaultMode;
    HI_U32         u32ProcID;
}HDMI_OPEN_S;

typedef struct hiHDMI_DEINIT_S
{
    HI_U32                          NoUsed;
}HDMI_DEINIT_S;

typedef struct hiHDMI_INIT_S
{
    HI_U32                          NoUsed;
}HDMI_INIT_S;

typedef struct hiHDMI_COLSE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_CLOSE_S;

typedef struct hiHDMI_START_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_START_S;

typedef struct hiHDMI_STOP_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_STOP_S;

typedef struct hiHDMI_POLL_EVENT_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_EVENT_TYPE_E        Event;
    HI_U32                          u32ProcID;
}HDMI_POLL_EVENT_S;

typedef struct hiHDMI_SINK_CAPABILITY_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_SINK_CAPABILITY_S   SinkCap;
}HDMI_SINK_CAPABILITY_S;

typedef struct hiHDMI_PORT_ATTR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HDMI_ATTR_S                     stHDMIAttr;
}HDMI_PORT_ATTR_S;

typedef struct hiHDMI_INFORFRAME_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_INFOFRAME_TYPE_E    enInfoFrameType;
    HI_UNF_HDMI_INFOFRAME_S         InfoFrame;
}HDMI_INFORFRAME_S;

typedef struct hiHDMI_AVMUTE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          AVMuteEnable;
}HDMI_AVMUTE_S;

typedef struct hiHDMI_VIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          VideoTiming;
}HDMI_VIDEOTIMING_S;

typedef struct hiHDMI_PREVIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          VideoTiming;
}HDMI_PREVIDEOTIMING_S;

typedef struct hiHDMI_DEEPCOLOR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_DEEP_COLOR_E        enDeepColor;
}HDMI_DEEPCOLORC_S;

typedef struct hiHDMI_SET_XVYCC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          xvYCCEnable;
}HDMI_SET_XVYCC_S;

typedef struct hiHDMI_CEC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_CMD_S           CECCmd;
    HI_U32                          timeout;
}HDMI_CEC_S;


typedef struct
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_STATUS_S        stStatus;
}HDMI_CEC_STATUS;

typedef struct hiHDMI_EDID_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U8                           u8EdidValid;
    HI_U32                          u32Edidlength;
    HI_U8                           u8Edid[512];
}HDMI_EDID_S;

typedef struct hiHDMI_PLAYSTAUS_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          u32PlayStaus;
}HDMI_PLAYSTAUS_S;

typedef struct hiHDMI_CEC_ENABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_ENABlE_S;

typedef struct hiHDMI_CEC_DISABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_DISABLE_S;

typedef struct hiHDMI_REGCALLBACKFUNC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
	HI_U32							u32CallBackAddr;
}HDMI_REGCALLBACKFUNC_S;

typedef struct hiHDMI_LOADKEY_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_UNF_HDMI_LOAD_KEY_S         stLoadKey;
}HDMI_LOADKEY_S;

typedef struct hiHDMI_GETPROCID_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_U32         u32ProcID;
}HDMI_GET_PROCID_S;

enum hiIOCTL_HDMI_E
{
    IOCTL_HDMI_INIT = 0X01,
    IOCTL_HDMI_DEINIT,
    IOCTL_HDMI_OPEN,
    IOCTL_HDMI_CLOSE,
    IOCTL_HDMI_START,
    IOCTL_HDMI_STOP,
    IOCTL_HDMI_SINK_CAPABILITY,
    IOCTL_HDMI_POLL_EVENT,
    IOCTL_HDMI_GET_ATTR,
    IOCTL_HDMI_SET_ATTR,
    IOCTL_HDMI_GET_INFORFRAME,
    IOCTL_HDMI_SET_INFORFRAME,
    IOCTL_HDMI_AVMUTE,
    IOCTL_HDMI_VIDEO_TIMING,
    IOCTL_HDMI_GET_DEEPCOLOR,
    IOCTL_HDMI_SET_DEEPCOLOR,
    IOCTL_HDMI_XVYCC,
    IOCTL_HDMI_SET_CEC,
    IOCTL_HDMI_GET_CEC,
    IOCTL_HDMI_CECSTATUS,
    IOCTL_HDMI_PRE_VIDEO_TIMING,
    IOCTL_HDMI_FORCE_GET_EDID,
    IOCTL_HDMI_GET_HDMI_PLAYSTAUS,
    IOCTL_HDMI_CEC_ENABLE,
    IOCTL_HDMI_CEC_DISABLE,
    IOCTL_HDMI_REG_CALLBACK_FUNC,
    IOCTL_HDMI_LOADKEY,
    IOCTL_HDMI_GET_PROCID,
    IOCTL_HDMI_RELEASE_PROCID,
    IOCTL_HDMI_MAX,
};

/*
**IOCTRL Between User/Kernel Level
*/
#define CMD_HDMI_INIT                _IOWR(HI_ID_HDMI, IOCTL_HDMI_INIT,           HDMI_INIT_S)
#define CMD_HDMI_DEINIT              _IOWR(HI_ID_HDMI, IOCTL_HDMI_DEINIT,         HDMI_DEINIT_S)
#define CMD_HDMI_OPEN                _IOWR(HI_ID_HDMI, IOCTL_HDMI_OPEN,           HDMI_OPEN_S)
#define CMD_HDMI_CLOSE               _IOWR(HI_ID_HDMI, IOCTL_HDMI_CLOSE,          HDMI_CLOSE_S)
#define CMD_HDMI_START               _IOWR(HI_ID_HDMI, IOCTL_HDMI_START,          HDMI_START_S)
#define CMD_HDMI_STOP                _IOWR(HI_ID_HDMI, IOCTL_HDMI_STOP,           HDMI_STOP_S)
#define CMD_HDMI_SINK_CAPABILITY     _IOWR(HI_ID_HDMI, IOCTL_HDMI_SINK_CAPABILITY,HDMI_SINK_CAPABILITY_S)
#define CMD_HDMI_POLL_EVENT          _IOWR(HI_ID_HDMI, IOCTL_HDMI_POLL_EVENT,     HDMI_POLL_EVENT_S)
#define CMD_HDMI_GET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_ATTR,       HDMI_PORT_ATTR_S)
#define CMD_HDMI_SET_ATTR            _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_ATTR,       HDMI_PORT_ATTR_S)
#define CMD_HDMI_GET_INFORFRAME      _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_INFORFRAME, HDMI_INFORFRAME_S)
#define CMD_HDMI_SET_INFORFRAME      _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_INFORFRAME, HDMI_INFORFRAME_S)
#define CMD_HDMI_SET_AVMUTE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_AVMUTE,         HDMI_AVMUTE_S)
#define CMD_HDMI_VIDEO_TIMING        _IOWR(HI_ID_HDMI, IOCTL_HDMI_VIDEO_TIMING,   HDMI_VIDEOTIMING_S)
#define CMD_HDMI_GET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_DEEPCOLOR,  HDMI_DEEPCOLORC_S)
#define CMD_HDMI_SET_DEEPCOLOR       _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_DEEPCOLOR,  HDMI_DEEPCOLORC_S)
#define CMD_HDMI_SET_XVYCC           _IOWR(HI_ID_HDMI, IOCTL_HDMI_XVYCC,          HDMI_SET_XVYCC_S)
#define CMD_HDMI_GET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_CEC,        HDMI_CEC_S)
#define CMD_HDMI_SET_CEC             _IOWR(HI_ID_HDMI, IOCTL_HDMI_SET_CEC,        HDMI_CEC_S)
#define CMD_HDMI_CECSTATUS           _IOWR(HI_ID_HDMI, IOCTL_HDMI_CECSTATUS,      HDMI_CEC_STATUS)
#define CMD_HDMI_PREVTIMING          _IOWR(HI_ID_HDMI, IOCTL_HDMI_PRE_VIDEO_TIMING,    HDMI_PREVIDEOTIMING_S)
#define CMD_HDMI_FORCE_GET_EDID      _IOWR(HI_ID_HDMI, IOCTL_HDMI_FORCE_GET_EDID,      HDMI_EDID_S) 
#define CMD_HDMI_GET_HDMI_PLAYSTAUS  _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_HDMI_PLAYSTAUS,  HDMI_PLAYSTAUS_S) 
#define CMD_HDMI_CEC_ENABLE          _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_ENABLE,     HDMI_CEC_ENABlE_S) 
#define CMD_HDMI_CEC_DISABLE         _IOWR(HI_ID_HDMI, IOCTL_HDMI_CEC_DISABLE,    HDMI_CEC_DISABLE_S) 
#define CMD_HDMI_REG_CALLBACK_FUNC   _IOWR(HI_ID_HDMI, IOCTL_HDMI_REG_CALLBACK_FUNC, HDMI_REGCALLBACKFUNC_S) 
#define CMD_HDMI_LOADKEY             _IOWR(HI_ID_HDMI, IOCTL_HDMI_LOADKEY, HDMI_LOADKEY_S)
#define CMD_HDMI_GET_PROCID          _IOWR(HI_ID_HDMI, IOCTL_HDMI_GET_PROCID, HDMI_GET_PROCID_S)
#define CMD_HDMI_RELEASE_PROCID      _IOWR(HI_ID_HDMI, IOCTL_HDMI_RELEASE_PROCID, HDMI_GET_PROCID_S)


typedef struct {

    HI_U8 Bus;
    HI_U8 SlaveAddr;
    HI_U8 Flags;
    HI_U8 NBytes;
    HI_U8 RegAddrL;
    HI_U8 RegAddrH;
} I2CShortCommandType;

typedef struct {
    HI_U8 SlaveAddr;
    HI_U8 Offset;
    HI_U8 RegAddr;
    HI_U8 NBytesLSB;
    HI_U8 NBytesMSB;
    HI_U8 Dummy;
    HI_U8 Cmd;
    HI_U8 * PData;
    HI_U8 Data[6];
} MDDCType;



#define FLG_SHORT 0x01 // Used for Ri Short Read
#define FLG_NOSTOP 0x02 // Don't release IIC Bus
#define FLG_CONTD 0x04 // Continued from previous operation

//#define IIC_CAPTURED 0xF0
//#define NO_ACK_FROM_IIC_DEV 0xF1
//#define MDDC_IIC_CAPTURED 0xF2
//#define MDDC_NO_ACK_FROM_IIC_DEV 0xF3


#define _IIC_CAPTURED  1
#define _IIC_NOACK     2
#define _MDDC_CAPTURED 3
#define _MDDC_NOACK    4
#define _MDDC_FIFO_FULL  5
#define IIC_OK 0

HI_S32 DRV_HDMI_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_S32 DRV_HDMI_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value);

HI_U8 DRV_ReadByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr);
void DRV_WriteByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data);
HI_U16 DRV_ReadWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr);
HI_U8 DRV_ReadByte_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr);
HI_U16 DRV_ReadWord_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr);
void DRV_WriteWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U16 Data);
void DRV_WriteWord_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr, HI_U16 Data);
void DRV_WriteByte_16BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U16 RegAddr, HI_U8 Data);
HI_S32 DRV_BlockRead_8BAS(I2CShortCommandType * I2CCommand, HI_U8 * Data);
HI_U8 DRV_BlockWrite_8BAS( I2CShortCommandType * I2CCommand, HI_U8 * Data );
HI_U8 DRV_HDMI_HWReset(HI_U32 u32Enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
/*--------------------------END-------------------------------*/
