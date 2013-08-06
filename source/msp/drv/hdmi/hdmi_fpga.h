#ifndef __HDMI_DRV_H__
#define __HDMI_DRV_H__

#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif

#include "hi_unf_hdmi.h"

#define HI_HDMI_PRINT printk

#define STATIC 

typedef enum
{
    HDMI_TX_DP_8BIT = 0,
    HDMI_TX_DP_10BIT,
    HDMI_TX_DP_12BIT,
    HDMI_TX_DP_BUTT    
} VIDEO_DPCOLOR_MODE_E;

typedef enum
{
    HDMI_TX_RPT_1X = 0,
    HDMI_TX_RPT_2X,
    HDMI_TX_RPT_4X,
    HDMI_TX_RPT_BUTT    
} VIDEO_RPT_MODE_E;

#define HDMI_PHY_BASE_ADDR 0xf8ce0000L
//#define VOU_PHY_BASE_ADDR 0x10110000L
#define SYS_PHY_BASE_ADDR 0xf8a22000L//0x101f5000L 实际为CRG

#define HDMI_WriteReg(base, offset, value) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)) = (value))
#define HDMI_ReadReg(base, offset) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)))



HI_VOID SocHdmiInit(HI_U32 hdcp);
HI_VOID SocHdmiDeInit(void);



////////////////////     视频部分    ///////////////////
// 1 edid; 2 cec; 3 sw-intr; else all 
HI_VOID CheckFunction(HI_S32 funnum);

HI_S32 hdmi_edid_test(void);
HI_VOID hdmi_cec_test(void);
HI_VOID soft_intr_test(void);
HI_VOID hdcp_en(HI_U32 enable);

HI_VOID set_fmt(HI_UNF_ENC_FMT_E enFmt);

HI_VOID ColorBar720P50(void);
#if 0
HI_VOID ColorBar720P60();
HI_VOID ColorBar1080I50();
HI_VOID ColorBar480P60();
HI_VOID ColorBar576P50();
HI_VOID ColorBarPAL();
HI_VOID ColorBarNTSC();
#endif

HI_VOID SetDeepColor(VIDEO_DPCOLOR_MODE_E dpmode);
HI_VOID PixelRpt(VIDEO_RPT_MODE_E rptmode);


HI_VOID avi_frame_en(void);
HI_VOID audio_frame_en(void);
HI_VOID ge_frame_en(void);
HI_VOID ge_frame2_en(void);
HI_VOID vendor_frame_en(void);


////////////////////     音频部分    ///////////////////
//#define AUDIO_MODE
//audio
//#ifdef AUDIO_MODE
HI_VOID SI_SetAudioPath( HI_U8 *abAudioPath);
HI_VOID AudioInit(HI_U8 audFmt,HI_U8 freq,HI_U8 inLen,HI_U8 mclk,HI_U8 mclk_EN);
//#endif  

//HI_S32 HI_DRV_I2C_Init(HI_VOID);
//HI_S32 HI_DRV_I2C_Write(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData,
    //                    HI_U32 DataLen);

//HI_S32 HI_DRV_I2C_Read(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData,
    //                   HI_U32 DataLen);


////////////////////test接口//////////////////////
void ReadPhyAll(void);
HI_VOID RegRWTest(void);

void testTxIsrSetup(void);
void testTxIsrExit(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
