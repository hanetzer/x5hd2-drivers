/*  extdrv/interface/hdmi/hi_hdmi.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * History:
 *      19-April-2006 create this file
 *      hi_struct.h
 *      hi_debug.h

 */
#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <mach/hardware.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

//#include "common_dev.h" 头文件改名
#include "drv_dev_ext.h"
//#include "common_proc.h"
#include "drv_proc_ext.h"


//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "drv_hdmi.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_edid.h"
#include "si_phy.h"

//#include "hi_common_id.h"
#include "hi_module.h"
//#include "hi_common_log.h"
#include "hi_debug.h"
//#include "common_module_drv.h"
#include "drv_module_ext.h"
#include "drv_hdmi_ext.h"
#include "si_timer.h"

#ifdef ANDROID_SUPPORT

#include <linux/switch.h>

struct switch_dev hdmi_tx_sdev =
{     
    .name = "hdmi",  
};

#endif


#define HDMI_NAME                      "HI_HDMI"
#if defined (SUPPORT_FPGA)
#include "hdmi_fpga.h"
#endif
HI_S32 DRV_HDMI_ReadPhy(void)
{
    HI_U32 u32Ret;
       
    u32Ret = SI_TX_PHY_GetOutPutEnable();
    
    return u32Ret;
}

//extern HI_U8 OutputState;
extern HI_U32 unStableTimes;

/*****************************************************************************
 Prototype    : hdmi_Proc
 Description  : HDMI status in /proc/msp/hdmi
 Input        : None
 Output       : None
 Return Value :
 Calls        :
*****************************************************************************/
static HI_S32 hdmi_Proc(struct seq_file *p, HI_VOID *v)
{
    HI_U32 Ret;
    HI_U32 u32Reg, index, offset;
    HI_UNF_HDMI_SINK_CAPABILITY_S sinkCap;
	HDMI_ATTR_S			          stHDMIAttr; 
#if defined (CEC_SUPPORT) 	
    HI_UNF_HDMI_CEC_STATUS_S      CECStatus;
#endif
    HI_U32 u32DefHDMIMode;

    p += seq_printf(p, "\n########### Hisi HDMI Dev Stat ###########\n");
    
    Ret = DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHDMIAttr);
    if(Ret != HI_SUCCESS)
    {
        p += seq_printf(p, "HDMI driver do not Open\n" );
        p += seq_printf(p, "\n#################### END ##################\n");
        return HI_SUCCESS;
    }
    
    /* HPD Status */
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV0, 0x09);// 0x72:0x09
    p += seq_printf(p, "HPD Status     : ");
    if ((u32Reg & 0x02) != 0x02)
    {
        p += seq_printf(p, "Out\n");
    }
    else
    {
        p += seq_printf(p, "IN\n");
    }
    /* HDMI Start mode */
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV0, 0x08);// 0x72:0x08
    if ((u32Reg & 0x01) != 0x01)
    {
        p += seq_printf(p, "HDMI do not Start!\n");
    }
    else
    {        
        /* HDMI Status */
        DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0, &sinkCap);
        p += seq_printf(p, "TV             : Power ");
        if (sinkCap.bIsSinkPowerOn == HI_TRUE)
        {
            p += seq_printf(p, "On\n");
        }
        else
        {
            p += seq_printf(p, "Off\n");
        }


        DRV_HDMI_GetPlayStatus(0,&u32Reg);
        if(u32Reg == HI_TRUE)
        {
            p += seq_printf(p, "Play Status    : Start  \n");            
        }
        else
        {
            p += seq_printf(p, "Play Status    : STOP  \n");
        }
        
        
        p += seq_printf(p, "EDID Status    : ");
        if (sinkCap.bIsRealEDID == HI_TRUE)
        {
            p += seq_printf(p, "Ok\n");
        }
        else
        {
            p += seq_printf(p, "Fail\n");
        }
        
        p += seq_printf(p, "ManufactureName: %s\n", sinkCap.u8IDManufactureName);

        p += seq_printf(p, "HDMI Sink video capability :\n");        
        for (index = 0; index < HI_UNF_ENC_FMT_BUTT; index++)
        {
            if(HI_TRUE == sinkCap.bVideoFmtSupported[index])
            {
                if (index == HI_DRV_DISP_FMT_1080P_60)
                {
                    p += seq_printf(p, "Fmt: 1080P_60\n");
                }
                if (index == HI_DRV_DISP_FMT_1080P_50)
                {
                    p += seq_printf(p, "Fmt: 1080P_50\n");
                }
                if (index == HI_DRV_DISP_FMT_1080P_30)
                {
                    p += seq_printf(p, "Fmt: 1080P_30\n");
                }
                if (index == HI_DRV_DISP_FMT_1080P_25)
                {
                    p += seq_printf(p, "Fmt: 1080P_25\n");
                }
                if (index == HI_DRV_DISP_FMT_1080P_24)
                {
                    p += seq_printf(p, "Fmt: 1080P_24\n");
                }
                if (index == HI_DRV_DISP_FMT_1080i_60)
                {
                    p += seq_printf(p, "Fmt: 1080i_60\n");
                }
                if (index == HI_DRV_DISP_FMT_1080i_50)
                {
                    p += seq_printf(p, "Fmt: 1080i_50\n");
                }
                if (index == HI_DRV_DISP_FMT_720P_60)
                {
                    p += seq_printf(p, "Fmt: 720P_60\n");
                }
                if (index == HI_DRV_DISP_FMT_720P_50)
                {
                    p += seq_printf(p, "Fmt: 720P_50\n");
                }
                if (index == HI_DRV_DISP_FMT_576P_50)
                {
                    p += seq_printf(p, "Fmt: 576P_50\n");
                }
                if (index == HI_DRV_DISP_FMT_480P_60)
                {
                    p += seq_printf(p, "Fmt: 480P_60\n");
                }
                if (index == HI_DRV_DISP_FMT_PAL)
                {
                    p += seq_printf(p, "Fmt: 576i_50\n");
                }
                if (index == HI_DRV_DISP_FMT_NTSC)
                {
                    p += seq_printf(p, "Fmt: 480i_60\n");
                }
                if (index == HI_DRV_DISP_FMT_861D_640X480_60)
                {
                    p += seq_printf(p, "Fmt: 640X480_60\n");
                }
#if defined (DVI_SUPPORT)
                if (index == HI_DRV_DISP_FMT_VESA_800X600_60)
                    p += seq_printf(p, "Fmt: 800X600_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_1024X768_60)
                    p += seq_printf(p, "Fmt: 1024X768_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_1366X768_60)
                    p += seq_printf(p, "Fmt: 1366X768_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_1440X900_60)
                    p += seq_printf(p, "Fmt: 1400X900_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_1440X900_60_RB)
                    p += seq_printf(p, "Fmt: 1400X900_60_RB\n");

                if (index == HI_DRV_DISP_FMT_VESA_1600X1200_60)
                    p += seq_printf(p, "Fmt: 1600X1200_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_1920X1200_60)
                    p += seq_printf(p, "Fmt: 1920X1200_60\n");

                if (index == HI_DRV_DISP_FMT_VESA_2048X1152_60)
                    p += seq_printf(p, "Fmt: 2048X1152_60\n");
#endif

#if 0
                if(sinkCap.aspect_ratio[index][0] == 1)
                    p += seq_printf(p, "aspect ratio:         4:3 \n");
                else if(sinkCap.aspect_ratio[index][1] == 2)
                    p += seq_printf(p, "aspect ratio:               16:9\n");
#endif
            }
        }
        
        p += seq_printf(p, "HDMI Sink Audio capability \n");    
        p += seq_printf(p, "Audio Fmt support : ");    
        for (index = 0; index < HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT; index++)
        {
            if (sinkCap.bAudioFmtSupported[index] == HI_TRUE)
            {
                 switch (index)
                {
                case 1:
                    p += seq_printf(p, "LiniarPCM ");
                    break;
                case 2:
                    p += seq_printf(p, "AC3 ");
                    break;
                case 3:
                    p += seq_printf(p, "MPEG1 ");
                    break;
                case 4:
                    p += seq_printf(p, "MP3 ");
                    break;
                case 5:
                    p += seq_printf(p, "MPEG2 ");
                    break;
                case 6:
                    p += seq_printf(p, "ACC ");
                    break;
                case 7:
                    p += seq_printf(p, "DTS ");
                    break;
                case 8:
                    p += seq_printf(p, "ATRAC ");
                    break;
                case 9:
                    p += seq_printf(p, "OneBitAudio ");
                    break;
                case 10:
                    p += seq_printf(p, "DD+ ");
                    break;
                case 11:
                    p += seq_printf(p, "DTS_HD ");
                    break;
                case 12:
                    p += seq_printf(p, "MAT ");
                    break;
                case 13:
                    p += seq_printf(p, "DST ");
                    break;
                case 14:
                    p += seq_printf(p, "WMA ");
                    break;
                default:
                    p += seq_printf(p, "reserved ");          
                }
            }
        }
        p += seq_printf(p, "\n");

        p += seq_printf(p, "Max Audio PCM channels: %d\n", sinkCap.u32MaxPcmChannels);
        p += seq_printf(p, "Support Audio Sample Rates:");
        for (index = 0; index < HI_UNF_HDMI_MAX_AUDIO_SMPRATE_COUNT; index++)
        {
            if(sinkCap.u32AudioSampleRateSupported[index] != 0)
            {
                p += seq_printf(p, " %d ", sinkCap.u32AudioSampleRateSupported[index]);
            }
        }
        p += seq_printf(p, "\n");
        if(sinkCap.u8Speaker != HI_NULL)
        {
            p += seq_printf(p, "Support Audio channels:");
            if(sinkCap.u8Speaker & 0x01)
                p += seq_printf(p, " FL/FR ");
            if(sinkCap.u8Speaker & 0x02)
                p += seq_printf(p, " LFE ");
            if(sinkCap.u8Speaker & 0x04)
                p += seq_printf(p, " FC ");
            if(sinkCap.u8Speaker & 0x08)
                p += seq_printf(p, " RL/RR ");
            if(sinkCap.u8Speaker & 0x10)
                p += seq_printf(p, " RC ");
            if(sinkCap.u8Speaker & 0x20)
                p += seq_printf(p, " FLC/FRC ");
            if(sinkCap.u8Speaker & 0x40)
                p += seq_printf(p, " RLC/RRC ");    
            p += seq_printf(p, "\n");
        }
     
        
#if defined (CEC_SUPPORT)    
        DRV_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &CECStatus);
        if(CECStatus.bEnable == HI_TRUE)
        {
            p += seq_printf(p, "CEC Status     : ON\n");
            p += seq_printf(p, "CEC Phy Add    : %01d.%01d.%01d.%01d\n", CECStatus.u8PhysicalAddr[0],
                    CECStatus.u8PhysicalAddr[1], CECStatus.u8PhysicalAddr[2], CECStatus.u8PhysicalAddr[3]);
            p += seq_printf(p, "CEC Logical Add: %01d\n", CECStatus.u8LogicalAddr);
        }
        else
        {
            p += seq_printf(p, "CEC Status     : OFF\n");
        }
#endif
#if defined (HDCP_SUPPORT)       
        p += seq_printf(p, "HDMI Output Attribute:\n");
        u32Reg = DRV_ReadByte_8BA(0, TX_SLV0, 0x0F);  // 0x72:0x0F  
        if (0X01 == (u32Reg & 0X01))
        {
            p += seq_printf(p, "HDCP Encryption: ON\n");
        }
        else
        {
            p += seq_printf(p, "HDCP Encryption: OFF\n");
        }

        if(stHDMIAttr.stAttr.bHDCPEnable == HI_TRUE)
        {
            p += seq_printf(p, "HDCP Enable: ON\n");
        }
        else 
        {
            p += seq_printf(p, "HDCP Enable: OFF\n");
        }
#endif

        p += seq_printf(p, "PHY Output     : ");
        if (HI_TRUE == DRV_HDMI_ReadPhy())
        {
            p += seq_printf(p, "Enable\n");
        }
        else
        {
            p += seq_printf(p, "Disable\n");
            p += seq_printf(p, "*****HDMI is abnormal, HDMI don't Output******\n");
        }
                
        DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHDMIAttr);
        
        p += seq_printf(p, "Video Output   : ");
        if (stHDMIAttr.stAttr.bEnableVideo == HI_TRUE)
        {
            p += seq_printf(p, "Enable\n");
        }
        else
        {
            p += seq_printf(p, "Disable\n");
        }
        
        p+= seq_printf(p, "Current Fmt    : ");
        if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080P_60)
        {
            p += seq_printf(p, "1080P_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080P_50)
        {
            p += seq_printf(p, "1080P_50\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080P_30)
        {
            p += seq_printf(p, "1080P_30\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080P_25)
        {
            p += seq_printf(p, "1080P_25\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080P_24)
        {
            p += seq_printf(p, "1080P_24\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080i_60)
        {
            p += seq_printf(p, "1080i_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_1080i_50)
        {
            p += seq_printf(p, "1080i_50\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_720P_60)
        {
            p += seq_printf(p, "720P_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_720P_50)
        {
            p += seq_printf(p, "720P_50\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_576P_50)
        {
            p += seq_printf(p, "576P_50\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_480P_60)
        {
            p += seq_printf(p, "480P_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_PAL)
        {
            p += seq_printf(p, "576i_50\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_NTSC)
        {
            p += seq_printf(p, "480i_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_861D_640X480_60)
        {
            p += seq_printf(p, "640X480_60\n");
        }
#if defined (DVI_SUPPORT)
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_800X600_60)
        {
            p += seq_printf(p, "800X600_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1024X768_60)
        {
            p += seq_printf(p, "1024X768_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1280X1024_60)
        {
            p += seq_printf(p, "1280X1024_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1366X768_60)
        {
            p += seq_printf(p, "1366X768_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1440X900_60)
        {
            p += seq_printf(p, "1440X900_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1440X900_60_RB)
        {
            p += seq_printf(p, "1440X900_60_RB\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1600X1200_60)
        {
            p += seq_printf(p, "1600X1200_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_1920X1200_60)
        {
            p += seq_printf(p, "1920X1200_60\n");
        }
        else if (stHDMIAttr.stAttr.enVideoFmt == HI_DRV_DISP_FMT_VESA_2048X1152_60)
        {
            p += seq_printf(p, "2048X1152_60\n");
        }
#endif
        else
        {
            p += seq_printf(p, "Unknown:%d\n", stHDMIAttr.stAttr.enVideoFmt);
        }
        
        p += seq_printf(p, "Color Space    : ");
        if (stHDMIAttr.stAttr.enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_RGB444)
        {
            p += seq_printf(p, "RGB444\n");
        }
        else if (stHDMIAttr.stAttr.enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
        {
            p += seq_printf(p, "YCbCr422\n");
        }
        else if (stHDMIAttr.stAttr.enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR444)
        {
            p += seq_printf(p, "YCbCr444\n");
        }
        else 
		{
            p += seq_printf(p, "Unknown:%d\n", stHDMIAttr.stAttr.enVidOutMode);
        }
        
        p += seq_printf(p, "DeepColor      : ");
        if (stHDMIAttr.stAttr.enDeepColorMode == HI_UNF_HDMI_DEEP_COLOR_30BIT)
        {
            p += seq_printf(p, "30bit\n");
        }
        else if (stHDMIAttr.stAttr.enDeepColorMode == HI_UNF_HDMI_DEEP_COLOR_36BIT)
        {
            p += seq_printf(p, "36bit\n");
        }
        else
        {
            p += seq_printf(p, "24bit\n");
        }
                
        p += seq_printf(p, "AUD Output     : ");
        if (stHDMIAttr.stAttr.bEnableAudio == HI_TRUE)
        {
            p += seq_printf(p, "Enable\n");
        }
        else
        {
            p += seq_printf(p, "Disable\n");
        }
        
        p += seq_printf(p, "AUD Input Type : ");
        if (stHDMIAttr.enSoundIntf == HDMI_AUDIO_INTERFACE_I2S)
        {
            p += seq_printf(p, "I2S\n");
        }
        else if (stHDMIAttr.enSoundIntf == HDMI_AUDIO_INTERFACE_SPDIF)
        {
            p += seq_printf(p, "SPDIF\n");
        }
        else if (stHDMIAttr.enSoundIntf == HDMI_AUDIO_INTERFACE_HBR)
        {
            p += seq_printf(p, "HighBitRate\n");
        }
        else
        {
            p += seq_printf(p, "Unknown:%d\n", stHDMIAttr.enSoundIntf);
        }
        p += seq_printf(p, "AUD Sample Rate: %dHz\n", stHDMIAttr.stAttr.enSampleRate);
        p += seq_printf(p, "AUD Bit Depth  : %dbit\n", stHDMIAttr.stAttr.enBitDepth);
        p += seq_printf(p, "AUD Trace Mode : ");
        if(stHDMIAttr.stAttr.bIsMultiChannel == HI_FALSE)
        {
           p += seq_printf(p, "Stereo\n");
        }
        else
        {
           p += seq_printf(p, "Multichannel\n");
        }

        u32Reg = ReadByteHDMITXP1(0x05);  // 0x7A:0x05
        u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x04);  // 0x7A:0x04
        u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x03);  // 0x7A:0x03

        p += seq_printf(p, "HDMI AUDIO N   : 0x%x(%d)\n",u32Reg,u32Reg);

        u32Reg = ReadByteHDMITXP1(0x0b);  // 0x7A:0x0b
        u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x0a);  // 0x7A:0x0a
        u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x09);  // 0x7A:0x09

        p += seq_printf(p, "HDMI AUDIO CTS : 0x%x(%d)\n",u32Reg,u32Reg);

        u32Reg = ReadByteHDMITXP1(AUD_MODE_ADDR);
        p += seq_printf(p, "AUD Mode       : 0x%02x\n",u32Reg);

		u32DefHDMIMode = DRV_Get_Def_HDMIMode();
		if(HI_UNF_HDMI_DEFAULT_ACTION_HDMI == u32DefHDMIMode)
		{
			p += seq_printf(p, "Default HDMI Mode: HDMI\n");
		}
		else if(HI_UNF_HDMI_DEFAULT_ACTION_DVI == u32DefHDMIMode)
		{
			p += seq_printf(p, "Default HDMI Mode: DVI\n");
		}
		else if(HI_UNF_HDMI_DEFAULT_ACTION_NULL == u32DefHDMIMode)
		{
			p += seq_printf(p, "Default HDMI Mode: NULL\n");
		}
		else
		{
    		p += seq_printf(p, "Default HDMI Mode: Unknow\n");
		}
        
        /* HDMI Mode */
        p += seq_printf(p, "Output Mode    : ");
        u32Reg = ReadByteHDMITXP1(0x2F);  // 0x7A:0x2F
        if ((u32Reg & 0x01) != 0x01)
        {
            p += seq_printf(p, "DVI\n");
        }
        else
        {
            HI_BOOL VendorSpecInfoFlag = HI_FALSE;
			HI_BOOL bAudInfoFrmaeFlag  = HI_FALSE;
			HI_BOOL bAVIInfoFrameFlag  = HI_FALSE;
            
            p += seq_printf(p, "HDMI\n");
            
           // p += seq_printf(p, "AVI Infoframe: ");
            u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3E);  // 0x7A:0x3E
            if ( 0x03 == (u32Reg & 0x03))
            {
                bAVIInfoFrameFlag = HI_TRUE;
            }
       
            if ( 0x30 == (u32Reg & 0x30))
            {
                bAudInfoFrmaeFlag = HI_TRUE;
            }
           
            if ( 0xc0 == (u32Reg & 0xc0))
            {
                VendorSpecInfoFlag = HI_TRUE;
            }
            
            p += seq_printf(p, "Gamut Meta     : ");
            u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3F);  // 0x7A:0x3F
            if ( 0xC0 == (u32Reg & 0xC0))
            {
                p += seq_printf(p, "Enable\n");
            }
            else
            {
                p += seq_printf(p, "Disable\n");
            }
            p += seq_printf(p, "Generic Packet : ");
            if ( 0x03 == (u32Reg & 0x03))
            {
                p += seq_printf(p, "Enable\n");
            }
            else
            {
                p += seq_printf(p, "Disable\n");
            }
            
            p += seq_printf(p, "AVMUTE         : ");
            u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0xDF);  // 0x7A:0xDF
            if ( 0x10 == (u32Reg & 0x10))
            {
                p += seq_printf(p, "Disable\n");
            }
            else if ( 0x01 == (u32Reg & 0x01))
            {
                p += seq_printf(p, "Enable\n");
            }
            else
            {
                p += seq_printf(p, " Unknown:%d\n", u32Reg);
            }

            if(bAVIInfoFrameFlag)
	    {
		    /* AVI InfoFrame */
		    p += seq_printf(p, "AVI Inforframe:\n");
		    for(index = 0; index < 17; index ++)
		    {
			    offset = 0x40 + index;//0x7A:0x40
			    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
			    p += seq_printf(p, "0x%02x,", u32Reg);
		    }
		    p += seq_printf(p, "\n");
	    }
	    if(bAudInfoFrmaeFlag)
	    {
		    /* AUD InforFrame */
		    p += seq_printf(p, "AUD Inforframe:\n");
		    for(index = 0; index < 9; index ++)
		    {
			    offset = 0x80 + index;//0x7A:0x80
			    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
			    p += seq_printf(p, "0x%02x,", u32Reg);
		    }
		    p += seq_printf(p, "\n");
	    }
	    if (VendorSpecInfoFlag == HI_TRUE)
	    {
		    HI_U8 Packettype = 0, VideoFormat = 0, _3Dtype = 0;
		    /* MPg/VendorSpec InforFrame */
		    p += seq_printf(p, "MPg/VendorSpec Inforframe:\n");
		    for(index = 0; index < 12; index ++)
		    {
			    offset = 0xa0 + index;//0x7A:0xA0
			    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
			    p += seq_printf(p, "0x%02x,", u32Reg);
			    if (index == 0)
			    {
				    Packettype = u32Reg;
			    }
			    else if (index == 7)
			    {
				    VideoFormat = (u32Reg & 0xe0) >> 5;
			    }
			    else if (index == 8)
			    {
				    _3Dtype = (u32Reg & 0xf0) >> 4;
			    }
		    }
		    if ((Packettype == 0x81) && (VideoFormat == 0x02))
		    {
			    //3D format
			    switch (_3Dtype)
			    {
				    case 0:
					    p += seq_printf(p, " (3D:FramePacking) ");
					    break;
				    case 6:
					    p += seq_printf(p, " (3D:Top-and-Bottom) ");
					    break;
				    case 8:
					    p += seq_printf(p, " (3D:Side-By-Side half) ");
					    break;
			    }
		    }
		    p += seq_printf(p, "\n");                
	    }
	}      

        p += seq_printf(p, "InitNum        : %d \n", DRV_HDMI_InitNum(0));
        p += seq_printf(p, "ProcNum        : %d \n", DRV_HDMI_ProcNum(0));

        u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x48);
        u32Reg = u32Reg & 0x0f;
        if(u32Reg != 0x00)
        {
            p += seq_printf(p, "Pixel Repeat   : 2x \n");
        }
        else
        {
            p += seq_printf(p, "Pixel Repeat   : 1x(No repeat) \n");
        }

        u32Reg = ReadByteHDMITXP0(TX_STAT_ADDR);

        if((u32Reg & BIT_HDMI_PSTABLE)!=0)
        {
            p += seq_printf(p, "Pixel Clk      : stable \n");            
        }
        else
        {
            p += seq_printf(p, "Pixel Clk      : !!Warnning!! Clock Unstable\n");
        }     
        
        p += seq_printf(p, "Unstable Times : %d \n",unStableTimes);

        u32Reg = ReadByteHDMITXP0(0x3b);
        u32Reg = (u32Reg << 8) | ReadByteHDMITXP0(0x3a);        
        p += seq_printf(p, "H Total        : %d ( 0x%x )\n",u32Reg,u32Reg);

        u32Reg = ReadByteHDMITXP0(0x3d);
        u32Reg = (u32Reg << 8) | ReadByteHDMITXP0(0x3c);
        
        p += seq_printf(p, "V Total        : %d ( 0x%x )\n",u32Reg,u32Reg);

        u32Reg = ReadByteHDMITXP0(INTERLACE_POL_DETECT);
        if((u32Reg & BIT_I_DETECTR)!=0)
        {
            p += seq_printf(p, "InterlaceDetect: interlace\n");
        }
        else
        {
            p += seq_printf(p, "InterlaceDetect: progress\n");
        }

        u32Reg = ReadByteHDMITXP1(DIAG_PD_ADDR);

        p += seq_printf(p, "Power State    : 0x%02x\n",u32Reg);

        if(sinkCap.bHDMI_Video_Present == HI_TRUE)
        {
            p += seq_printf(p, "sink 3D support: support \n");
        }
        else
        {
            p += seq_printf(p, "sink 3D support: not support \n");
        }

        u32Reg = ReadByteHDMITXP0(0xf6);
        p += seq_printf(p, "DDC Delay Count: 0x%02x(%d)\n",u32Reg,u32Reg);

        //60Mhz osc clk
        p += seq_printf(p, "DDC Speed      : %dHz\n",(60000000 / (u32Reg * 30)));
	/* print EDID data */
        if (sinkCap.bIsRealEDID == HI_TRUE)
        {
            HI_U32 index;
            HI_U8  Data[256];

            p += seq_printf(p, "EDID Raw Data:\n");
            memset(Data, 0, 256);
            SI_Proc_ReadEDIDBlock(Data, 256);
            for (index = 0; index < 256; index ++)
            {
                p += seq_printf(p, "%02x ", Data[index]);
                if (0 == ((index + 1) % 16))
                {
                    p += seq_printf(p, "\n");
                }
            }
            
        }
        
    }
    p += seq_printf(p, "\n#################### END ##################\n" );
    return HI_SUCCESS;
}

#define DEF_FILE_NAMELENGTH 20

HI_S32 hdmi_GetProcArg(HI_CHAR*  chCmd,HI_CHAR*  chArg,HI_U32 u32ArgIdx)
{
    HI_U32 u32Count;
    HI_U32 u32CmdCount;
    HI_U32 u32LogCount;
    HI_U32 u32NewFlag;
    HI_CHAR chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR chArg2[DEF_FILE_NAMELENGTH] = {0};
    u32CmdCount = 0;

    /*清除前面的空格*/
    u32Count = 0;
    u32CmdCount = 0;
    u32LogCount = 1;
    u32NewFlag = 0;
    while(chCmd[u32Count] != 0 && chCmd[u32Count] != '\n' )
    {
        if (chCmd[u32Count] != ' ')
        {
            u32NewFlag = 1;
        }
        else
        {
            if(u32NewFlag == 1)
            {
                u32LogCount++;
                u32CmdCount= 0;
                u32NewFlag = 0;
            }
        }
        
        if (u32NewFlag == 1)
        {
            switch(u32LogCount)
            {
                case 1:
                    chArg1[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 2:
                    chArg2[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                default:
                    break;
            }
            
        }
        u32Count++;
    }
    
    switch(u32ArgIdx)
    {
        case 1:
            memcpy(chArg,chArg1,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        case 2:
            memcpy(chArg,chArg2,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

HI_S32 hdmi_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    //struct seq_file   *p = file->private_data;
    //DRV_PROC_ITEM_S  *pProcItem = s->private;
    HI_CHAR  chCmd[60] = {0};
    HI_CHAR  chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg2[DEF_FILE_NAMELENGTH] = {0};

    
    if(count > 40)
    {   
        printk("Error:Echo too long.\n");
        return HI_FAILURE;
    }
    
    if(copy_from_user(chCmd,buf,count))
    {
        printk("copy from user failed\n");
        return HI_FAILURE;
    }

    hdmi_GetProcArg(chCmd, chArg1, 1);
    hdmi_GetProcArg(chCmd, chArg2, 2);

    if(chArg1[0] == '0'&& chArg1[1] == '0' && chArg1[2] == '1' && chArg1[3] == '0')
    {
        printk("hdmi resetting... ... ... \n");
        SI_SW_ResetHDMITX();
    }
    else if(chArg1[0] == '0'&& chArg1[1] == '0' && chArg1[2] == '2' && chArg1[3] == '0')
    {
        if(chArg2[0] == '1')
        {
            printk("hdmi TmdsClk invert...  \n");
            WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(chArg2[0] == '0')
        {
            printk("hdmi TmdsClk not invert... \n");
            WriteByteHDMITXP1(0x3d,0x17);
        }
    }
    else if(chArg1[0] == '0'&& chArg1[1] == '0' && chArg1[2] == '3' && chArg1[3] == '0')
    {
        if(chArg2[0] == '1')
        {
            printk("mute...  \n");
            DRV_HDMI_SetAVMute(0,HI_TRUE);
            //WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(chArg2[0] == '0')
        {
            printk("unmute... \n");
            DRV_HDMI_SetAVMute(0,HI_FALSE);
            //WriteByteHDMITXP1(0x3d,0x17);
        }
    }
    else if(chArg1[0] == '0'&& chArg1[1] == '0' && chArg1[2] == '4' && chArg1[3] == '0')
    {
        if(chArg2[0] == '0')
        {
            printk("mute...  \n");
            HI_DRV_HDMI_Set3DMode(0,HI_FALSE,HI_UNF_3D_MAX_BUTT);
            //WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(chArg2[0] == '1')
        {
            printk("Frame Packing... \n");
            HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_FRAME_PACKETING);
            //WriteByteHDMITXP1(0x3d,0x17);
        }
        else if(chArg2[0] == '2')
        {
            printk("Side by side(half)... \n");
            HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_SIDE_BY_SIDE_HALF);
            //WriteByteHDMITXP1(0x3d,0x17);
        }
        else if(chArg2[0] == '3')
        {
            printk("Top and bottom... \n");
            HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_TOP_AND_BOTTOM);
            //WriteByteHDMITXP1(0x3d,0x17);
        }
    }
    else if(chArg1[0] == '0'&& chArg1[1] == '0' && chArg1[2] == '0' && chArg1[3] == '1')
    {
        HI_U32 u32Reg = 0;;
        if(chArg2[0] == '0')
        {
            printk("colorbar disable...  \n");
            DRV_HDMI_ReadRegister(0xf8ccc000,&u32Reg);
            u32Reg = u32Reg & (~0x70000000);
            DRV_HDMI_WriteRegister(0xf8ccc000,(u32Reg | 0x1));
        }
        else if(chArg2[0] == '1')
        {
            printk("colorbar enable.. \n");
            DRV_HDMI_ReadRegister(0xf8ccc000,&u32Reg);
            u32Reg = u32Reg | 0x70000000;
            DRV_HDMI_WriteRegister(0xf8ccc000,(u32Reg | 0x1));
        }
    }
    else 
    {
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        printk("---------------HDMI PROC HELP---------------------\n"
        "echo arg1 arg2 > /proc/msp/hdmi\n"
        "arg1 == 'reset' : software reset\n"
        //"usage     :arg2 == PortID arg3 == path\n"
        "eg              : echo reset > /proc/msp/hdmi\n"
        "arg1 == 'invert': Tmds Clk invert \n"
        "usage           : arg2 == 1 tmds invert(0x1f) \n"
        "usage           : arg2 == 0 tmds not invert(0x17) \n"
        "eg              : echo invert 1 > /proc/msp/hdmi\n");
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

        printk("--------------------HDMI debug options------------------------\n");                                                     
        printk("you can perform HDMI debug with such commond:\n");                                                                      
        printk("echo [arg1] [arg2] > /proc/msp/hdmi \n\n");                                                                             
        printk("debug action                      arg1         arg2\n");                                                                
        printk("------------------------------    --------    ---------------------\n");                                                
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        printk("set print enable                  0x0         no support\n"); 
        printk("set err_thr                       0x2         no support\n");   
        printk("set dec order output              0x4         no support\n"); 
        printk("set dec_mode(0/1/2=IPB/IP/I)      0x5         no support\n");  
        printk("set discard_before_dec_thr        0x7         no support\n"); 
        printk("set postprocess options           0xa         no support\n");                                                           
        printk("set frame/adaptive storage        0xb         no support\n");                                                           
        printk("pay attention to the channel      0xd         no support\n");                                                           
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        printk("colorbar                          0001        0 disable / 1 enable \n");
        printk("software reset                    0010        no param \n"); 
        printk("invert Tmds clk                   0020        0 not invert / 1 invert \n");  
        printk("Avmute                            0030        0 unmute / 1 mute \n");
        printk("Set 3D Fmt                        0040        0 disable 3d / 1 FP / 2 SBS / 3 TAB  \n");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        printk("start/stop stream seg saving      0200        chan_id\n");                                                              
        printk("start/stop 2D yuv saving          0202        chan_id\n");                                                              
        printk("save a single 2D frame            0203        frame phy addr\n");                                                       
        printk("save a single 1D frame            0204        frame phy addr width height=(height+PicStructure)\n");                    
        printk("set dec_task_schedule_delay       0400        schedual_delay_time(ms)\n");                                              
        printk("set dnr_active_interval           0401        dnr_active_interval(ms)\n");                                              
        printk("stop/start syntax dec             0402        do not care\n");                                                          
        printk("set trace controller              0500        vfmw_state_word in /proc/vfmw_prn\n");                                    
        printk("set bitstream control period      0501        period (ms)\n");                                                          
        printk("set frame control period          0502        period (ms)\n");                                                          
        printk("set rcv/rls img control period    0503        period (ms)\n");                                                          
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        printk("---------------------------------------------------------------\n"); 
    }
    return count;
}


extern HI_S32 hdmi_Open(struct inode *inode, struct file *filp);
extern HI_S32 hdmi_Close(struct inode *inode, struct file *filp);
extern HI_S32 hdmi_Ioctl(struct inode *inode, struct file *file,
                           unsigned int cmd, HI_VOID *arg);
extern HI_S32 hdmi_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
extern HI_S32 hdmi_Resume(PM_BASEDEV_S *pdev);

static HDMI_EXPORT_FUNC_S s_stHdmiExportFuncs = {
    .pfnHdmiInit = HI_DRV_HDMI_Init,
    .pfnHdmiDeinit = HI_DRV_HDMI_Deinit,
    .pfnHdmiOpen  = HI_DRV_HDMI_Open,
    .pfnHdmiClose = HI_DRV_HDMI_Close,
    .pfnHdmiGetPlayStus = HI_DRV_HDMI_PlayStus,
    .pfnHdmiGetAoAttr = HI_DRV_AO_HDMI_GetAttr,
    .pfnHdmiGetSinkCapability = HI_DRV_HDMI_GetSinkCapability,
    .pfnHdmiGetAudioCapability = HI_DRV_HDMI_GetAudioCapability,
    .pfnHdmiAudioChange = HI_DRV_HDMI_AudioChange,
    .pfnHdmiPreFormat = HI_DRV_HDMI_PreFormat,
    .pfnHdmiSetFormat = HI_DRV_HDMI_SetFormat,
    .pfnHdmiSet3DMode = HI_DRV_HDMI_Set3DMode,
};

static long  hdmi_Drv_Ioctl(struct file *file,unsigned int cmd, unsigned long arg) 
{
	return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, hdmi_Ioctl);
}

static struct file_operations hdmi_FOPS =
{
    owner   : THIS_MODULE,
    open    : hdmi_Open,
    unlocked_ioctl   : hdmi_Drv_Ioctl,
    release : hdmi_Close,
};

static /*struct*/ PM_BASEOPS_S  hdmi_DRVOPS = {
	.probe        = NULL,
	.remove       = NULL,
	.shutdown     = NULL,
	.prepare      = NULL,
	.complete     = NULL,
	.suspend      = hdmi_Suspend,
	.suspend_late = NULL,
	.resume_early = NULL,
	.resume       = hdmi_Resume,
};

static UMAP_DEVICE_S   g_hdmiRegisterData;

HI_S32 HDMI_ModeInit(HI_VOID)
{
    DRV_PROC_ITEM_S  *pProcItem;

    /* Register hdmi device */
    sprintf(g_hdmiRegisterData.devfs_name, UMAP_DEVNAME_HDMI);
    g_hdmiRegisterData.fops   = &hdmi_FOPS;
    g_hdmiRegisterData.drvops = &hdmi_DRVOPS;
    g_hdmiRegisterData.minor  = UMAP_MIN_MINOR_HDMI;
    g_hdmiRegisterData.owner  = THIS_MODULE;
    if (HI_DRV_DEV_Register(&g_hdmiRegisterData) < 0)
    {
        HI_FATAL_HDMI("register hdmi failed.\n");
        return HI_FAILURE;
    }
    /* Register Proc hdmi Status */
    pProcItem = HI_DRV_PROC_AddModule("hdmi", hdmi_Proc, NULL);
    pProcItem->write = hdmi_ProcWrite;
    return HI_SUCCESS;
}

extern HI_S32  HDMI_DRV_Init(HI_VOID);

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
extern HI_S32  HI_DRV_HDMI_Init(HI_VOID);
extern HI_S32  HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi);
extern HI_U32  HI_DRV_HDMI_DeInit(HI_U32 FromUserSpace);
extern HI_U32  HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/




HI_VOID HDMI_ModeExit(HI_VOID)
{
    /* Unregister hdmi device */
    HI_DRV_PROC_RemoveModule("hdmi");
    HI_DRV_DEV_UnRegister(&g_hdmiRegisterData);
    return;
}

int HDMI_DRV_ModInit(void)
{
    HI_S32 ret;
    
#if defined (SUPPORT_FPGA)
    SocHdmiInit();
#endif
    HDMI_ModeInit();
#ifndef HI_MCE_SUPPORT    
    HDMI_DRV_Init();
#endif

    ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_HDMI,HDMI_NAME,(HI_VOID *)&s_stHdmiExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_HDMI("HI_DRV_MODULE_Register failed\n");
        return ret;
    }
    
#ifdef ANDROID_SUPPORT
    //android 特有
	if (switch_dev_register(&hdmi_tx_sdev))
    {
		HI_WARN_HDMI("\n Warning:! registering HDMI switch device Failed \n");
		//return -EINVAL;
	}
#endif

	printk("Load hi_hdmi.ko success.\t(%s)\n", VERSION_STRING);

    return 0;
}

extern HI_VOID  HDMI_DRV_EXIT(HI_VOID);
void HDMI_DRV_ModExit(void)
{
    HI_U32 hdmiStatus;
    hdmiStatus = DRV_HDMI_InitNum(HI_UNF_HDMI_ID_0);
    if(hdmiStatus > 0)
    {
        HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_0);
    }

    HDMI_ModeExit();
    
#ifndef HI_MCE_SUPPORT        
    HDMI_DRV_EXIT();
#endif
    //HI_DRV_HDMI_PlayStus(HI_UNF_HDMI_ID_0,&temp);
    //if(temp == HI_TRUE)
    //{
        //HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_0);
    //}
#ifdef ANDROID_SUPPORT
    //android 特有
	switch_dev_unregister(&hdmi_tx_sdev);
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_HDMI);
    return;
}

#ifdef MODULE
module_init(HDMI_DRV_ModInit);
module_exit(HDMI_DRV_ModExit);
#endif
MODULE_LICENSE("GPL");




