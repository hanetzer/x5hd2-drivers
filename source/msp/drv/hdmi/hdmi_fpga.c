/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name               : 	drv_hdmi.c
  Version                : 	Initial Draft
  Author                 : 	Hisilicon multimedia software group
  Created               : 	2011/07/19
  Last Modified	    :
  Description          :      hdmi interface of module.
  Function List         :
  History               :
  1.Date                : 	2011/07/19
    Author              : 	dsh
    Modification           :	Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/seq_file.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/interrupt.h>


#include "hi_type.h"

//#include "common_mem.h"
#include "drv_mmz_ext.h"
//#include "common_stat.h"
#include "drv_stat_ext.h"
//#include "common_sys.h"
#include "drv_sys_ext.h"
//#include "common_proc.h"
#include "drv_proc_ext.h"
////#include "common_log.h"
//#include "drv_log_ext.h"
////#include "common_event.h"
//#include "drv_event_ext.h"

#include "hi_unf_common.h"


//需要后续添加、并调用相应的i2c读写函数，初始化phy
//#include "gpio_i2c.h"
#include "drv_gpioi2c_ext.h"
#include "hdmi_fpga.h"

//#include "vdp_driver.h"
//#include "vdp_module.h"

//#define FPGA_BOARD //è?1?ó?FPGA°?￡??ò?¨ò??a??
#define AUDIO_MODE //è?1?2aê?ò??μ￡??ò?¨ò??a??

#define HDMI_PHY_BASE_ADDR 0xf8ce0000L
//#define VOU_PHY_BASE_ADDR 0x10110000L
#define SYS_PHY_BASE_ADDR 0xf8a22000L//0x101f5000L 实际为CRG

#define MAX_SIZE 0x3000L

#define __S40_FPGA__

/////////////////////视频部分///////////////////////
/* hdmi parameter */
STATIC  unsigned int de_dly; //0x33 0x32
STATIC  unsigned int de_top; //0x34
STATIC  unsigned int de_cnt; //0x37 0x36
STATIC  unsigned int de_lin; //0x39 0x38

//STATIC  unsigned int hs_pol; //0x33 bit4
//STATIC  unsigned int vs_pol; //0x33 bit5

STATIC  unsigned int de_gen; //0x33 bit6
   
STATIC  unsigned int hbit_2_hsync; //0x41 0x40
STATIC  unsigned int field2_ofst; //0x43 0x42
STATIC  unsigned int hwidth; //0x45 0x44
STATIC  unsigned int vbit_2_vsync; //0x46
STATIC  unsigned int vwidth; //0x47

//STATIC  unsigned int iclk;//0x48 bit0-1
//STATIC  unsigned int cscsel;//0x48 bit4
//STATIC  unsigned int extn;//0x48 bit5

STATIC  unsigned int syncext; //0x4a bit0
STATIC  unsigned int demux; //0x4a bit1
STATIC  unsigned int upsmp; //0x4a bit2
STATIC  unsigned int csc; //0x4a bit3

//STATIC  unsigned int range;//0x4a bit4
//STATIC  unsigned int dither;//0x4a bit5



#define DE_DLY 0x32*4
#define DE_CTRL 0x33*4
#define DE_TOP 0x34*4
#define DE_CNTH 0x37*4
#define DE_CNTL 0x36*4
#define DE_LINH 0x39*4
#define DE_LINL 0x38*4
#define HBIT_2HSYNC1 0x40*4
#define HBIT_2HSYNC2 0x41*4
#define FLD2_HS_OFSTH 0x43*4
#define FLD2_HS_OFSTL 0x42*4
#define HWIDTH1 0x44*4
#define HWIDTH2 0x45*4
#define VBIT_TO_VSYNC 0x46*4
#define VWIDTH 0x47*4
#define VID_MODE 0x4a*4

/////////////////////音频部分///////////////////////
/*hdmi audio parameter */
enum SiI_AudioPath {
    SiI_SPDIF,
    SiI_I2S,
    SiI_DSD,
    SiI_HBAudio,
    SiI_AudioModesSelect = 0x0F,   // first four bits used for type of audio interface
    SiI_MultyChannel = 0x80
};

// Table is derived from "Encoded Audio Sampling Frequency" in the 9134 PR. 
//Undefined entries are set to 0:
const HI_U32 N_ValCPY[] = { 6272,            //0:44.1 kHz
                            0,
                            6144,         //2:48 kHz
                            4096,         //3:32 kHz
                            0,
                            0,
                            0,
                            0,
                            12544,        //8:88 kHz
                            0,
                            12288,        //a:96 kHz
                            0,
                            25088,        //c:176.4kHz
                            0,
                            24576         //e:192 kHz
                        };

#define EN_AUDIO    0X01
#define MUTE_AUDIO  0x02
#define BIT_LAYOUT1 0x02
#define  SD_0_3_EN 0xf0
#define BIT_SPDIF_SELECT 0x02
#define BIT_DSD_SELECT 0x08
#define SD_0 0x10
#define BIT_EN_AUDIO 0x01
#define	BIT_SPDIF_SAMPLE		0x04
#define	BIT_HBR_ON	        	0x80
#define	SCK_RISING	        	0x40	                        
#define	BIT_CBIT_ORDER	    	0x20
#define	BIT_COMPRESSED	    	0x10
#define	SETUP_ENABLE_HBRA		0xF1	            
#define HBRA_IN_CTRL_VAL	    	0x92
#define HBRA_ZERO_PLUS	    	0x01	                    
#define NON_PCM_TYPE	    	0x04	                    
#define HBRA_FOR_CHST4	    	0x09
#define HBRA_FOR_CHST5	    	0xE2
#define HBR_SPR_MASK	        	0x00
#define ENABLE_ALL	        	0x00




#define DATA_CTRL                    0x0d*4

#define ACR_CTRL          0x01*4 + 0x400
#define FREQ_SVAL       0x02*4 + 0x400
#define N_SVAL1         0x03*4 + 0x400
#define N_SVAL2         0x04*4 + 0x400
#define N_SVAL3         0x05*4 + 0x400
#define AUD_MODE        0x14*4 + 0x400
#define SPDIF_CTRL      0x15*4 + 0x400
#define HW_SPDIF_FS      0x18*4 + 0x400
#define  SWAP_I2S           0x19*4 + 0x400
#define SPDIF_ERTH           0x1b*4 + 0x400
#define  I2S_IN_MAP           0x1c*4 + 0x400
#define  I2S_IN_CTRL           0x1d*4 + 0x400
#define  I2S_IN_LEN          0x24*4 + 0x400
#define  AUDP_TXCTRL          0x2f*4 + 0x400
#define I2S_CHST0	    	0x1E*4 + 0x400
#define I2S_CHST1	    	0x1F*4 + 0x400
#define I2S_CHST2	    	0x20*4 + 0x400
#define I2S_CHST4	    	0x21*4 + 0x400
#define I2S_CHST5	    	0x22*4 + 0x400

#define SAMPLE_RATE_CONVERSION		0x23*4 + 0x400

#define HDMI_I2C_CHANNEL 1


HI_U32 hdcp[320]={0x00, 0xc2, 0x43, 0xfc,
            0xb1, 0xd8, 0x78, 0x6d, 
            0xa8, 0xad, 0x29, 0x9a, 
            0x79, 0x63, 0xd8, 0xf6,
            0xd5, 0xd8, 0x5e, 0x14, 
            0xe1, 0xb2, 0x97, 0xed, 
            0x9d, 0x3a, 0x13, 0xd3,
            0x27, 0xe9, 0xc8, 0x33,
            0xf5, 0x40, 0x76, 0x21, 
            0x20, 0x44, 0x21, 0xea, 
            0x70, 0x4a, 0x60, 0x21, 
            0xa3, 0x18, 0x4e, 0x6d,
            0x78, 0x0a, 0x01, 0x91, 
            0x7f, 0xfd, 0x7e, 0x76, 
            0x1b, 0xd5, 0x83, 0x2c, 
            0x5e, 0x82, 0x6e, 0xf6,
            0x68, 0xd4, 0x14, 0x1e, 
            0xe9, 0x37, 0x56, 0xff, 
            0x6d, 0x24, 0xbc, 0x0e, 
            0x1b, 0xcf, 0xb1, 0x88,
            0xc8, 0xbe, 0x47, 0x9c, 
            0x8a, 0x8e, 0xa5, 0x97, 
            0xff, 0xf7, 0xfe, 0x2d, 
            0xc3, 0x78, 0x97, 0x0d,
            0x69, 0xc6, 0x35, 0xc0, 
            0x42, 0xb2, 0x92, 0x90, 
            0xa0, 0x1a, 0x59, 0xe7, 
            0x51, 0xb4, 0x06, 0xfa,
            0x7d, 0x9c, 0x1c, 0xa0, 
            0x27, 0x89, 0xa3, 0x98, 
            0xcb, 0x80, 0x5b, 0x1c, 
            0x65, 0x95, 0x12, 0x2d,
            0xf1, 0x8f, 0x55, 0xf9, 
            0x68, 0xe5, 0x36, 0x45, 
            0x9f, 0x6d, 0x21, 0xf8, 
            0xc2, 0x93, 0x70, 0x54,
            0x95, 0xb6, 0xc9, 0x3b, 
            0x6f, 0x59, 0xec, 0xdb, 
            0x40, 0x90, 0x23, 0xac, 
            0xf0, 0xc3, 0x48, 0x10,
            0x37, 0x92, 0x58, 0x1e, 
            0xa6, 0x09, 0xd0, 0x0b, 
            0xcb, 0x45, 0x5b, 0xa4, 
            0xef, 0xef, 0x0c, 0x48,
            0xfc, 0x9c, 0x89, 0x0f, 
            0x6f, 0x11, 0xb9, 0x63, 
            0x7d, 0xf1, 0xda, 0x11, 
            0x59, 0x61, 0x7e, 0x92,
            0xb0, 0xf2, 0xe4, 0x03, 
            0x94, 0xae, 0x3f, 0x9b, 
            0x70, 0xeb, 0xd8, 0x00, 
            0x41, 0x8a, 0x18, 0x6e,
            0x17, 0x25, 0x7e, 0x49, 
            0x5d, 0x77, 0x54, 0xec, 
            0xd8, 0xc9, 0x8e, 0x4c, 
            0xaf, 0xfb, 0xe1, 0xaf,
            0x6b, 0xed, 0x2a, 0xac, 
            0xa0, 0x39, 0x56, 0x65, 
            0xdc, 0x10, 0xb8, 0x62, 
            0xfd, 0x88, 0x73, 0x84,
            0xad, 0x34, 0xff, 0x31, 
            0x6c, 0x3b, 0x7e, 0x38, 
            0xda, 0xca, 0xb9, 0x63, 
            0x9e, 0xd9, 0xa1, 0x7a,
            0xbf, 0xd1, 0xe6, 0x83, 
            0x68, 0x64, 0x30, 0xdd, 
            0xa7, 0x27, 0x68, 0xaf, 
            0x8a, 0x70, 0x39, 0x5a,
            0xe0, 0x96, 0x64, 0x44, 
            0x31, 0x85, 0x0d, 0x44, 
            0xb9, 0x28, 0xb5, 0x39, 
            0x7a, 0x65, 0x72, 0x16,
            0x55, 0x14, 0xf7, 0x61, 
            0x03, 0xb7, 0x59, 0x45, 
            0xe3, 0x0c, 0x7d, 0xb4, 
            0x45, 0x19, 0xea, 0x8f,
            0xd2, 0x89, 0xee, 0xbd, 
            0x90, 0x21, 0x8b, 0x05, 
            0xe0, 0x4e, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00};


// read/write reg
#if 0

#define HDMI_WriteReg(base, offset, value) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)) = (value))
#define HDMI_ReadReg(base, offset) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)))

#endif
//#define Print printf
//#ifdef FPGA_BOARD

#if 1

#ifdef __S40_FPGA__

extern HI_S32 HI_DRV_I2C_Write(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData,
                        HI_U32 DataLen);

extern HI_S32 HI_DRV_I2C_Read(HI_U32 I2cNum, HI_U8 I2cDevAddr, HI_U32 I2cRegAddr, HI_U32 I2cRegAddrByteNum, HI_U8 *pData,
                       HI_U32 DataLen);

#endif

unsigned char read_tx_phy(unsigned char devaddress, unsigned char address)
{
    unsigned char data;
    //HI_DRV_GPIOI2C_Read(devaddress,address);
    #ifdef __S40_FPGA__
        HI_DRV_I2C_Read(HDMI_I2C_CHANNEL,devaddress,address,1,&data,1);
    #else 
        data = HDMI_ReadReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1800 + address * 4);
    #endif
    return data;
}

void write_tx_phy(unsigned char devaddress, unsigned char address, unsigned char data)
{
    #ifdef __S40_FPGA__
        HI_DRV_I2C_Write(HDMI_I2C_CHANNEL,devaddress,address,1,&data,1);
        msleep(50);
    #else
        HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1800 + address * 4, data);
    #endif
    //return 0;
}
/*
    unsigned char read_tx_phy(unsigned char devaddress, unsigned char address)
    {
        return HI_DRV_GPIOI2C_Read(devaddress,address);//return 0;
    }
    
    void write_tx_phy(unsigned char devaddress, unsigned char address, unsigned char data)
    {
        HI_DRV_GPIOI2C_Write(devaddress,address,data);
    
        
        //return 1;
    }
*/

/*FPGA 板上的HDMI扣板 PHY初始化*/
HI_VOID SocHdmiPhyInit(HI_VOID)
{
    int ret;
    HI_U32 u32BaseAddr;
    unsigned char i;
    //fpga use,asic use apb cfg
/*
	ret=read_tx_phy(0x18,0x0);
	ret=read_tx_phy(0x18,0x01);
	ret=read_tx_phy(0x18,0x02);
	ret=read_tx_phy(0x18,0x03);
    */
    //
    //HI_DRV_GPIOI2C_Init();
    #ifdef __S40_FPGA__
    
        u32BaseAddr = 0xf8a20000;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10f4, 0x01);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10f8, 0x01);
                   
        ret=read_tx_phy(0x60,0x01);
        HI_HDMI_PRINT("\n I2C0x60 0x01 before 0x%x \n",ret);
        
    	write_tx_phy(0x60,0x01,0x28);
        msleep(10);
    	ret=read_tx_phy(0x60,0x01);
         HI_HDMI_PRINT("\n I2C0x60 0x01 now should be 0x%x && 0x28 \n",ret);


        ret=read_tx_phy(0x60,0x08);
        HI_HDMI_PRINT("\n I2C0x60 0x08 before 0x%x \n",ret);
        
        //限定 aclk_dig 的范围到113 ~ 220 Mhz
    	write_tx_phy(0x60,0x08,0x60);
        msleep(10);
    	ret=read_tx_phy(0x60,0x08);
        HI_HDMI_PRINT("\n I2C0x60 0x08 now should be 0x%x && 0x60 \n",ret);


        ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n I2C0x60 0x02 before 0x%x \n",ret);
        
    	write_tx_phy(0x60,0x02,0xa9);
        msleep(10);
    	ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n I2C0x60 0x02 now should be 0x%x && 0xa9 \n",ret);

        ret=read_tx_phy(0x60,0x03);
        HI_HDMI_PRINT("\n I2C0x60 0x03 before 0x%x \n",ret);

    	write_tx_phy(0x60,0x03,0x40);
        msleep(10);
        ret=read_tx_phy(0x60,0x03);
        HI_HDMI_PRINT("\n I2C0x60 0x03 now should be 0x%x && 0x40 \n",ret);

    #else
        // pll ctrl -deep color
        write_tx_phy(0x60,0x02,0x24);
        ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n phy:0x02 now && should be : 0x%x && 0x24 \n",ret);

        // oe && pwr_down
        write_tx_phy(0x60,0x05,0x32);
        ret=read_tx_phy(0x60,0x05);
        HI_HDMI_PRINT("\n phy:0x05 now && should be : 0x%x && 0x32 \n",ret);

        // enc_bypass == nobypass
        write_tx_phy(0x60,0x0d,0x00); 
        ret=read_tx_phy(0x60,0x0d);
        HI_HDMI_PRINT("\n phy:0x0d now && should be : 0x%x && 0x00 \n",ret);

        write_tx_phy(0x60,0x06,0x89); 
        write_tx_phy(0x60,0x0e,0x03); 
        write_tx_phy(0x60,0x07,0x81); 
        write_tx_phy(0x60,0x09,0x1a); 
        write_tx_phy(0x60,0x0a,0x07); 
        write_tx_phy(0x60,0x0b,0x51); 
        write_tx_phy(0x60,0x02,0x24); 
        write_tx_phy(0x60,0x05,0x32); 
        write_tx_phy(0x60,0x08,0x40); 
        //write_tx_phy(0x60,0x0e,0x01); // term_en 
    #endif
}

HI_VOID ReadPhyAll()
{
    int ret;
    HI_U32 u32BaseAddr;
    unsigned char i;
#ifdef __S40_FPGA__
    u32BaseAddr = 0xf8a20000;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10f4, 0x01);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10f8, 0x01);
#endif

    for (i = 0; i < 0x10 ; i++)
    {
        //write_tx_phy(0x60,i,0x1);
        ret = read_tx_phy(0x60,i);
        HI_HDMI_PRINT("\n I2C0x60 0x%x before 0x%x \n",i,ret); 
        msleep(10);
    }
}

#endif

int g_MultyChn = 0;
int g_Fmt = 0; // 0 720P50 1 720P60 2 1080i
//STATIC  void avi_frame_en(void);
//STATIC  void audio_frame_en(void);


//AUDIO_MODE
#if 1
// 0 = 2ch   1 = 8ch

HI_VOID AudioInit(HI_U8 audFmt,HI_U8 freq,HI_U8 inLen,HI_U8 mclk,HI_U8 mclk_EN)
{
    HI_U32 	u32Tmp;	
    volatile HI_U32  *pu32VirAddr;
    HI_U8 bRegVal1, bRegVal2, bRegVal3;
    HI_U8  abAudioPath[6];

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR); 

    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // abAudioPath[0] = Audio format:       0 - SPDIF;  1 - I2S;    2 - DSD;    3 - HBR
    //                  Bit 7:      0 - Layout 0 (stereo);  1 - Layout 1 (up to 8 channels)
    // abAudioPath[1] = Sampling Freq Fs:  0 - 44KHz;   2 - 48KHz ...
    // abAudioPath[2] = Sample length : 2, 4, 8, 0xA, 0xC +1 for Max 24. Even values are Max 20. Odd: Max 24.
    // abAudioPath[3] = I2S control bits (for 0x7A:0x1D)
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /* abAudioPath[1] set Sampling Freq Fs */
    /*
    CH_ST4          Fs Sampling Frequency
    3   2   1   0  <--- bit
    0   0   0   0   44.1 kHz    (0x00)
    1   0   0   0   88.2 kHz    (0x08)
    1   1   0   0   176.4 kHz   (0x0c)
    0   0   1   0   48 kHz          (0x02)
    1   0   1   0   96 kHz          (0x0a)
    1   1   1   0   192 kHz         (0x0e)
    0   0   1   1   32 kHz            (0x03)
    0   0   0   1   not indicated
    */
    

    /* abAudioPath[2] set Sample length */
    /*
       IN_LENGTH 3:0 <--- sample length
            (0xff)0b1111 - 0b1110 = N/A
            (0x0d)0b1101 = 21 bit
            (0x0c)0b1100 = 17 bit
            (0x0b)0b1011 = 24 bit
            (0x0a)0b1010 = 20 bit
            (0x09)0b1001 = 23 bit
            (0x08)0b1000 = 19 bit
            (0x07)0b0111 - 0b0110 = N/A
            (0x05)0b0101 = 22 bit
            (0x04)0b0100 = 18 bit
            0b0011 = N/A
            (0x02)0b0010 = 16 bit
            0b0001 - 0b0000 = N/A
    */
    
    /* abAudioPath[3] = I2S control bits (for 0x7A:0x1D) */
    /* 0 = Uncompressed */
    /* 1 = Compressed */

    //MCLK_EN
    
    abAudioPath[0] = audFmt;       
    abAudioPath[1] = freq;//48kHz
    abAudioPath[2] = inLen;//16bit
    abAudioPath[3] = 0x00;
    abAudioPath[4] = mclk;
    abAudioPath[5] = mclk_EN;// 1 内部产生 0 外部输入
    //abAudioPath[0] = 0x01;
    //abAudioPath[1] = 0x02;//48kHz
    //abAudioPath[2] = 0x02;//16bit
    //abAudioPath[3] = 0x00;
    //abAudioPath[4] = 0x01;
    
    /* hdmi audio enable */
    // HDMI mode enable
    // 0b100 = 24 bits per pixel (8 bits per pixel; no packing)  
    HDMI_WriteReg(pu32VirAddr, 0x4bc, 0x21);   /*7A: 0x2f*/
    //I2S control bits && PCM 在这里设置不起效，会被覆盖
    //HDMI_WriteReg(pu32VirAddr, 0x474, 0x40);   /*7A: 0x1D*/


    if(abAudioPath[5] == 0)
    {
        //临时值，正式SDK需要改正
        //采用外部mclk
        HDMI_WriteReg(pu32VirAddr, ACR_CTRL, 0x02);
    }
    else
    {
        //临时值，正式SDK需要改正
        //产生内部mclk
        HDMI_WriteReg(pu32VirAddr, ACR_CTRL, 0x06);
    }
    //SI_SetAudioPath( abAudioPath);
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // abAudioPath[0] = Audio format:       0 - SPDIF;  1 - I2S;    2 - DSD;    3 - HBR
    //                  Bit 7:      0 - Layout 0 (stereo);  1 - Layout 1 (up to 8 channels)
    // abAudioPath[1] = Sampling Freq Fs:  0 - 44KHz;   2 - 48KHz ...
    // abAudioPath[2] = Sample length : 2, 4, 8, 0xA, 0xC +1 for Max 24. Even values are Max 20. Odd: Max 24.
    // abAudioPath[3] = I2S control bits (for 0x7A:0x1D)
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // disable audio input stream
    u32Tmp = HDMI_ReadReg(pu32VirAddr,AUD_MODE); //0x450 // 0x7A:0x14
    u32Tmp &= ~BIT_EN_AUDIO; 		
    HDMI_WriteReg(pu32VirAddr,AUD_MODE, u32Tmp);
    // disable output audio packets
    HDMI_WriteReg(pu32VirAddr,DATA_CTRL, MUTE_AUDIO);		 

    //SI_BlockWriteEEPROM(4, EE_TX_AUDIOPATH_ADDR, abAudioPath);

    //Mclk 
    HDMI_WriteReg(pu32VirAddr,FREQ_SVAL,abAudioPath[4]);
    
    //SI_WriteNValue
    u32Tmp = N_ValCPY[abAudioPath[1]];
    HDMI_WriteReg(pu32VirAddr,N_SVAL1,(HI_U8)u32Tmp & 0xFF);
    HDMI_WriteReg(pu32VirAddr,N_SVAL2,((HI_U8)(u32Tmp >> 8) & 0xFF));
    HDMI_WriteReg(pu32VirAddr,N_SVAL3,((HI_U8)(u32Tmp >> 16) & 0xFF));

    //Sampling frequency as set by software,
    //which is inserted into the HDMI audio stream if FS_OVERRIDE (0x7A:0x15[1]) is enabled
    HDMI_WriteReg(pu32VirAddr,I2S_CHST4,abAudioPath[1]);

    bRegVal1 = HDMI_ReadReg(pu32VirAddr,AUD_MODE);          // 0x7A:0x14
    bRegVal2 = HDMI_ReadReg(pu32VirAddr,AUDP_TXCTRL);       // 0x7A:0x2F
    bRegVal2 = bRegVal2 | 0x21;//hdmi mode && 24bit

    if (abAudioPath[0] == SiI_SPDIF)
    {
        bRegVal2 &= (~BIT_LAYOUT1);
        // SD 0-3 disable
        bRegVal1 &= (~SD_0_3_EN);
        // S/PDIF input stream enable
        bRegVal1 |= BIT_SPDIF_SELECT;
        HI_HDMI_PRINT("use SDIF\n");
    }
    else         // not SPDIF
    {
        // Input I2S sample length
        bRegVal3 = abAudioPath[2];             
        // Number of valid bits in the input I2S stream. Used for the 
        // extraction of the I2S data from the input stream. 
        // (0x02)0b0010 = 16 bit
        HDMI_WriteReg(pu32VirAddr,I2S_IN_LEN, bRegVal3);

        bRegVal1 &= (~BIT_SPDIF_SELECT);
        // SD 0-3 disable
        bRegVal1 &= (~SD_0_3_EN);       // Clear bits 7:4

        if (abAudioPath[0] & SiI_MultyChannel)
        {   
            HI_HDMI_PRINT("MultyChannel\n");
            // All other modes need to enable I2S channel inputs
            bRegVal1 |= SD_0_3_EN;          
            bRegVal2 |= BIT_LAYOUT1;
            g_MultyChn = 1;
        }
        else 
        {
            HI_HDMI_PRINT("Open SD0 channel\n");
            // All other modes need to enable I2S channel inputs
            bRegVal1 |= SD_0;          
            bRegVal2 &= (~BIT_LAYOUT1);
            g_MultyChn = 0;
        }
        
        if ((abAudioPath[0] & 0x0f) == SiI_DSD)   
        {
            HI_HDMI_PRINT("DSD \n");
            // DSD Audio
            bRegVal1 |= BIT_DSD_SELECT;
        }
        else                            
        {
            // Not DSD Audio. Could be I2S or HBR
            bRegVal1 &= (~BIT_DSD_SELECT);

            //if ( (abAudioPath[0] & SiI_I2S) == SiI_I2S)
            if ( (abAudioPath[0] & SiI_I2S) == SiI_I2S)
            {
                HI_HDMI_PRINT("Path I2S\n");
                HDMI_WriteReg(pu32VirAddr,I2S_CHST4, abAudioPath[1]);   // Fs
                HDMI_WriteReg(pu32VirAddr,I2S_CHST5, (abAudioPath[2] | ((abAudioPath[1] << 4) & 0xF0)));   // "Original Fs" and Length
                HDMI_WriteReg(pu32VirAddr,I2S_IN_CTRL, abAudioPath[3] | 0x40);
            }
            else          // HBRA              
            {
                HI_HDMI_PRINT("set HBRA in Audio Path\n");
                bRegVal3 = (BIT_HBR_ON | SCK_RISING | BIT_CBIT_ORDER | BIT_COMPRESSED);
                // Write 0xF0 to 0x7A:0x1D
                HDMI_WriteReg(pu32VirAddr,I2S_IN_CTRL, bRegVal3);                 

                // Write 0xF1 to 0x7A:0x14
                bRegVal1 = SETUP_ENABLE_HBRA; 
                // Write 0x92 to 0x7A:0x24
                HDMI_WriteReg(pu32VirAddr,I2S_IN_LEN, HBRA_IN_CTRL_VAL);   
                // Write 0x01 to0x7A:0x2F 前面处理的没问题
                //bRegVal2 = HBRA_ZERO_PLUS;   
                

                // Write 0x04 to0x7A:0x1E to set 0x1E[1] (workaround)
                HDMI_WriteReg(pu32VirAddr,I2S_CHST0, NON_PCM_TYPE);
                 // Write 0x09 to0x7A:0x21
                HDMI_WriteReg(pu32VirAddr,I2S_CHST4, HBRA_FOR_CHST4);
                // Write 0xE2 to0x7A:0x22
                HDMI_WriteReg(pu32VirAddr,I2S_CHST5, HBRA_FOR_CHST5);
                // Write 0x0 to 0x7A:0x23
                HDMI_WriteReg(pu32VirAddr,SAMPLE_RATE_CONVERSION, HBR_SPR_MASK);
            }
        }
        HI_HDMI_PRINT("use I2S\n");
    }

    // allow FIFO to flush
    msleep(1);
    // Enable audio
    bRegVal1 |= BIT_EN_AUDIO;
    HDMI_WriteReg(pu32VirAddr,AUD_MODE, bRegVal1);
    HI_HDMI_PRINT("Set AUDP_TXCTRL_ADDR to AUD_MODE_ADDR:0x%x AUDP_TXCTRL_ADDR:0x%x\n", bRegVal1, bRegVal2);
    HDMI_WriteReg(pu32VirAddr,AUDP_TXCTRL, bRegVal2 );
    HI_HDMI_PRINT("\nover1\n");
    // Enable output audio packets
    HDMI_WriteReg(pu32VirAddr,DATA_CTRL, ENABLE_ALL);
    HI_HDMI_PRINT("\nover2\n");

    
    audio_frame_en();
}


//extern HI_VOID SI_SetAudioPath( HI_U8 *abAudioPath);

#endif    


    
STATIC  void write_ddc(HI_U32 addr,HI_U32 offset,HI_U32 write_data) //write one ddc data
{
    volatile HI_U32 *pu32VirAddr;
    HI_U32 ddc_status;
    HI_U32 time_out;

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR); 
    HDMI_WriteReg(pu32VirAddr, 0x3d8, 0x00000064); //DDC_DELAY_CNT 
    
    HDMI_WriteReg(pu32VirAddr, 0x3b4, addr);//addr
    HDMI_WriteReg(pu32VirAddr, 0x3bc, offset);//offset
    HDMI_WriteReg(pu32VirAddr, 0x3c0, 1);//write data count only one
    HDMI_WriteReg(pu32VirAddr, 0x3d0, write_data);//write data
    HDMI_WriteReg(pu32VirAddr, 0x3cc, 6);//start write ddc
    
    //msleep(10);
    time_out = 10;
    ddc_status = HDMI_ReadReg(pu32VirAddr, 0x3c8);
    while(ddc_status != 4 && time_out != 0)
    {
        ddc_status = HDMI_ReadReg(pu32VirAddr, 0x3c8);
        HI_HDMI_PRINT("F:%s,L:%d-w status(%#x)\n",__FUNCTION__, __LINE__, ddc_status);
        msleep(10);
        time_out --;
    }
    return;
}


STATIC  HI_U32 read_ddc(HI_U32 addr,HI_U32 offset) //read one ddc data
{
    volatile HI_U32 *pu32VirAddr;
    HI_U32 ddc_data;

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR); 
    HDMI_WriteReg(pu32VirAddr, 0x3d8, 0x00000064); //DDC_DELAY_CNT 
    
    HDMI_WriteReg(pu32VirAddr, 0x3b4, addr);//addr
    HDMI_WriteReg(pu32VirAddr, 0x3bc, offset);//offset
    HDMI_WriteReg(pu32VirAddr, 0x3c0, 1);//read data count only one
    HDMI_WriteReg(pu32VirAddr, 0x3cc, 0x9);//clear ddc fifo
    HDMI_WriteReg(pu32VirAddr, 0x3cc, 2);//start read ddc
    
    msleep(10);
    ddc_data = HDMI_ReadReg(pu32VirAddr, 0x3d0);
    return ddc_data;    
}

STATIC  HI_VOID hdmi_read_reg(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    volatile HI_U32 *pu32VirAddr;
    HI_U32 reg_data;

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(u32RegAddr); 
    
    reg_data = HDMI_ReadReg(pu32VirAddr, 0);//addr
    *pu32Value = reg_data;
}

/*STATIC */HI_U32 hdmi_RegDebug(HI_U32 Debugflag)
{
    HI_U32 index;
    HI_U32 hdmiregister = 0;
    
    if (Debugflag == 0)
    {
        HI_HDMI_PRINT("Debug hdmi DDC register:\n");
        for (index=0xEC; index<=0xF5; index++)
        {
            hdmi_read_reg((HDMI_PHY_BASE_ADDR + index * 4), &hdmiregister);
            HI_HDMI_PRINT("0x%02x(0x72, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + index * 4), index, hdmiregister);
        }
        HI_HDMI_PRINT("\nFinish\n");
    }
    
    else if (Debugflag == 1)
    {
        {
            HI_U8 index;
            HI_U32 hdmiregister = 0;
            HI_HDMI_PRINT("Debug hdmi audio register:\n");
            for (index=0; index<=0x30; index++)
            {
                hdmi_read_reg((HDMI_PHY_BASE_ADDR + 0x400 + index * 4), &hdmiregister);
                HI_HDMI_PRINT("0x%02x(0x7a, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + 0x400 + index * 4), index, hdmiregister);
            }
            HI_HDMI_PRINT("\n");
        }
        {
            HI_U8 index;
            HI_U32 hdmiregister = 0;
            HI_HDMI_PRINT("Debug hdmi audio infoframe:\n");
            for (index=0x80; index<=0x8D; index++)
            {
                hdmi_read_reg((HDMI_PHY_BASE_ADDR + 0x400 + index * 4), &hdmiregister);
                HI_HDMI_PRINT("0x%02x(0x7a, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + 0x400 + index * 4), index, hdmiregister);
            }
            HI_HDMI_PRINT("\n");
        }        
    }
    else
    {
        HI_HDMI_PRINT("Debug hdmi All 0x72 register:\n");
        for (index=0x00; index<=0xFF; index++)
        {
            hdmi_read_reg((HDMI_PHY_BASE_ADDR + index * 4), &hdmiregister);
            HI_HDMI_PRINT("0x%02x(0x72, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + index * 4), index, hdmiregister);
            msleep(10);
        }
        msleep(100);
        HI_HDMI_PRINT("Debug hdmi All 0x7a register:\n");
        for (index=0x00; index<=0xFF; index++)
        {
            hdmi_read_reg((HDMI_PHY_BASE_ADDR + 0x400 + index * 4), &hdmiregister);
            HI_HDMI_PRINT("0x%02x(0x7a, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + 0x400 + index * 4), index, hdmiregister);
            msleep(10);
        }
        msleep(100);
        HI_HDMI_PRINT("Debug hdmi All 0xCC register:\n");
        for (index=0x80; index<=0xF2; index++)
        {
            hdmi_read_reg((HDMI_PHY_BASE_ADDR + 0x800 + index * 4), &hdmiregister);
            HI_HDMI_PRINT("0x%02x(0xCC, 0x%02x):0x%02x\n", (HI_U32)(HDMI_PHY_BASE_ADDR + 0x800 + index * 4), index, hdmiregister);
            msleep(10);
        }
        HI_HDMI_PRINT("\nFinish\n");
    }
    return 0;
}

STATIC  HI_VOID hdcp_en(HI_U32 enable)
{
    volatile HI_U32 *pu32VirAddr;
    volatile HI_U32 epst,ddc_data,hdcp_ctrl,rx_ri1,tx_ri1,rx_ri2,tx_ri2;
    int i,j,ones;
    unsigned char bksv_data[5];
    unsigned char  mask;
    HI_U32 time_out;    
    HI_HDMI_PRINT("\nstart check HDCP\n");
        
    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);

    //0x27  set ri disable
    HDMI_WriteReg(pu32VirAddr, 0x9c, 0); //RI CMD

    //0x0f  //Content protection reset:
    HDMI_WriteReg(pu32VirAddr, 0x3c, 0);//HDCP CTRL

    if(enable == 0){
        HI_HDMI_PRINT("\nHDCP Unable return\n");
        return;
    }

    //    Content protection reset:
    //  0 = Reset
    //  1 = Normal operation    
    HDMI_WriteReg(pu32VirAddr, 0x3c, 0x4); //HDCP_CTRL

    // load ksv
    //0xF9 
    HDMI_WriteReg(pu32VirAddr, 0x3e4, 0x0);
    //0xFA  1 = Enable loading of KSV from OTP 
    HDMI_WriteReg(pu32VirAddr, 0x3e8, 0x20);
    epst = HDMI_ReadReg(pu32VirAddr, 0x3e4);
    HI_HDMI_PRINT("L[%d] epst = %#x.\n", __LINE__, epst);
    msleep(10);

    time_out = 4;
    // 1 = Command Done (last operation completed successfully)
    epst = HDMI_ReadReg(pu32VirAddr, 0x3e4);
    HI_HDMI_PRINT("L[%d] epst = %#x.\n", __LINE__, epst);
    while((epst & 0x1) != 1 && time_out != 0)
    {
        epst = HDMI_ReadReg(pu32VirAddr, 0x3e4);
        msleep(10);
        HI_HDMI_PRINT("L[%d] epst = %#x.\n", __LINE__, epst);
        time_out --;
    }
    if((epst & 0x7f) != 1)
    	HI_HDMI_PRINT("error(1)\n");
    //read hdcp key from otp
    HI_HDMI_PRINT("L[%d] aksv = %#x.\n", __LINE__, HDMI_ReadReg(pu32VirAddr, 0x74));
    HI_HDMI_PRINT("L[%d] aksv = %#x.\n", __LINE__, HDMI_ReadReg(pu32VirAddr, 0x78));
    HI_HDMI_PRINT("L[%d] aksv = %#x.\n", __LINE__, HDMI_ReadReg(pu32VirAddr, 0x7c));
    HI_HDMI_PRINT("L[%d] aksv = %#x.\n", __LINE__, HDMI_ReadReg(pu32VirAddr, 0x80));
    HI_HDMI_PRINT("L[%d] aksv = %#x.\n", __LINE__, HDMI_ReadReg(pu32VirAddr, 0x84));
    

    // check crc
    //
    HDMI_WriteReg(pu32VirAddr, 0x3e4, 0x0);
    // 0b00100 = Run only CRC test  
    HDMI_WriteReg(pu32VirAddr, 0x3e8, 0x4);
    msleep(10);
    time_out = 4;
    epst = HDMI_ReadReg(pu32VirAddr, 0x3e4);
    HI_HDMI_PRINT("L[%d] epst = %#x.\n", __LINE__, epst);
    while((epst & 0x1) != 1 && time_out != 0)
    {
        epst = HDMI_ReadReg(pu32VirAddr, 0x3e4);
        msleep(10);
        HI_HDMI_PRINT("L[%d] epst = %#x.\n", __LINE__, epst);
        time_out --;
    }
    
    if((epst & 0x02) ==0x2)
    	printk("hdcp crc check is error!\n");


#if 1
    //hdcp hand shake
    
    //AN control: 
    //When cleared, the cipher engine is allowed to free run and the AN 
    //registers cycle through pseudo-random values. 
    //When set, the cipher engines stops and the AN register may be read 
    //and initialized in the HDCP-capable Receiver. 
    HDMI_WriteReg(pu32VirAddr, 0x3c, 0xc);//HDCP_CTRL stop AN generation

    //for(i=0; i<4; i++)
    {
        HI_HDMI_PRINT("L[%d] write AN from Tx to Rx\n", __LINE__);
        //0x15
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x54);
        write_ddc(0x74,0x18,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x58);
        write_ddc(0x74,0x19,ddc_data);    
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x5c);
        write_ddc(0x74,0x1a,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x60);
        write_ddc(0x74,0x1b,ddc_data);        
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x64);
        write_ddc(0x74,0x1c,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x68);
        write_ddc(0x74,0x1d,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x6c);
        write_ddc(0x74,0x1e,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x70);
        write_ddc(0x74,0x1f,ddc_data);
     
        HI_HDMI_PRINT("L[%d] write AKSV from Tx to Rx\n", __LINE__);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x74);
        write_ddc(0x74,0x10,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x78);
        write_ddc(0x74,0x11,ddc_data);    
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x7c);
        write_ddc(0x74,0x12,ddc_data);
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x80);
        write_ddc(0x74,0x13,ddc_data);        
        ddc_data = HDMI_ReadReg(pu32VirAddr, 0x84);
        write_ddc(0x74,0x14,ddc_data);
     
        HI_HDMI_PRINT("L[%d] read BKSV from Rx and write to Tx BKSV regs\n", __LINE__);
        ddc_data = read_ddc(0x74,0x0);
        HI_HDMI_PRINT("BKSV1 = %#x\n",ddc_data);
        bksv_data[0] = ddc_data & 0xff;
        HDMI_WriteReg(pu32VirAddr, 0x40, ddc_data);
        ddc_data = read_ddc(0x74,0x1);
        HI_HDMI_PRINT("BKSV2 = %#x\n",ddc_data); 
        bksv_data[1] = ddc_data & 0xff;   
        HDMI_WriteReg(pu32VirAddr, 0x44, ddc_data);
        ddc_data = read_ddc(0x74,0x2);
        HI_HDMI_PRINT("BKSV3 = %#x\n",ddc_data);    
        bksv_data[2] = ddc_data & 0xff;
        HDMI_WriteReg(pu32VirAddr, 0x48, ddc_data);
        ddc_data = read_ddc(0x74,0x3);
        HI_HDMI_PRINT("BKSV4 = %#x\n",ddc_data);    
        bksv_data[3] = ddc_data & 0xff;
        HDMI_WriteReg(pu32VirAddr, 0x4c, ddc_data);
        ddc_data = read_ddc(0x74,0x4);
        HI_HDMI_PRINT("BKSV5 = %#x\n",ddc_data);    
        bksv_data[4] = ddc_data & 0xff;
        HDMI_WriteReg(pu32VirAddr, 0x50, ddc_data);

        HI_HDMI_PRINT("wait Tx read key memory\n");
        time_out = 4;
        hdcp_ctrl = HDMI_ReadReg(pu32VirAddr, 0x3c);//HDCP CTRL
        HI_HDMI_PRINT("hdcp_ctrl = %#x\n",hdcp_ctrl);
        while((hdcp_ctrl & 2) != 2 && time_out != 0)
        {
            hdcp_ctrl = HDMI_ReadReg(pu32VirAddr, 0x3c);
            HI_HDMI_PRINT("hdcp_ctrl = %#x\n",hdcp_ctrl);
            msleep(10);
            time_out --;
        }
        HI_HDMI_PRINT("check ri value\n");
        ddc_data = read_ddc(0x74,0x8);
        rx_ri1 = ddc_data;
        tx_ri1 = HDMI_ReadReg(pu32VirAddr, 0x88);
        if(tx_ri1 != rx_ri1) 
        {
            HI_HDMI_PRINT("Ri1 check error\n");
            //return;
            //continue;
        }
        HI_HDMI_PRINT("rx_ri1 = %#x   tx_ri1 = %#x\n",rx_ri1,tx_ri1);
        ddc_data = read_ddc(0x74,0x9);
        rx_ri2 = ddc_data;
        tx_ri2 = HDMI_ReadReg(pu32VirAddr, 0x8c);
        if(tx_ri2 != rx_ri2){
            HI_HDMI_PRINT("Ri2 check error\n");
            //return;
            //continue;
        }
        HI_HDMI_PRINT("rx_ri2 = %#x   tx_ri2 = %#x\n",rx_ri2,tx_ri2);
        //break;
    }

    //check bksv valid
    ones = 0;
    for(i = 0; i < 5; i++) {
       mask = 0x01;
       for(j = 0; j < 8; j++){
          if(bksv_data[i] & mask)
             ones++;
          mask <<= 1;
       }
    }
    if(ones != 20)
        HI_HDMI_PRINT("RX Bksv is not valid ones = %d\n",ones);    
        
    HI_HDMI_PRINT("now enable the encryption\n");
    HDMI_WriteReg(pu32VirAddr, 0x3c, 0xd); //HDCP CTRL
    HDMI_WriteReg(pu32VirAddr, 0xa0, 1); //RI START
    HDMI_WriteReg(pu32VirAddr, 0x9c, 1); //RI CMD

    HDMI_WriteReg(pu32VirAddr, 0x14, 1);
    msleep(10);
    HDMI_WriteReg(pu32VirAddr, 0x14, 0);    
        
#endif    
}


STATIC  HI_S32 hdmi_edid_test(void)
{
//    HI_U32 u32BaseAddr;
    
    HI_U32 time_out;    
	HI_U32 	u32Tmp;    
    volatile HI_U32  *pu32VirAddr;
    volatile HI_U32  ddc_status,edid_Tag,edid_Reversion_Num,remain_cnt;   
    

    #if 0
    /*HDMI CRG , 撤消HDMI复位*/
    u32BaseAddr = SYS_PHY_BASE_ADDR;
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x048);
	u32Tmp = 0x00000300;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x048, u32Tmp);
    #endif
    
    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR); 

    //HDMI_WriteReg(pu32VirAddr, 0x3d8, 0x00000078); //DDC_DELAY_CNT 
    HDMI_WriteReg(pu32VirAddr, 0x3d8, 0x00000064); //DDC_DELAY_CNT 

    HDMI_WriteReg(pu32VirAddr, 0x3b4, 0x000000a0); //read edid
    //edid offset怎么算?
    HDMI_WriteReg(pu32VirAddr, 0x3bc, 0x00000012); //edid offset
    HDMI_WriteReg(pu32VirAddr, 0x3c0, 0x0000000a); //read data count
    HDMI_WriteReg(pu32VirAddr, 0x3cc, 0x00000009); //clear ddc fifo
    HDMI_WriteReg(pu32VirAddr, 0x3cc, 0x00000002); //start read edid
    HDMI_WriteReg(pu32VirAddr, 0x3b4, 0x000000a0); //read edid
    
    ddc_status = HDMI_ReadReg(pu32VirAddr, 0x3c8);

    HI_HDMI_PRINT("status(%x)\n",ddc_status);
    time_out = 10;
    while((ddc_status & 0x10) != 0 && time_out != 0)
    {
        ddc_status = HDMI_ReadReg(pu32VirAddr, 0x3c8);
        HI_HDMI_PRINT("status(%x)\n",ddc_status);
        time_out--;
    }
       

    edid_Tag = HDMI_ReadReg(pu32VirAddr, 0x3d0);
    
    if(edid_Tag != 0x01)
        HI_HDMI_PRINT("EDID Tag is wrong(%x)\n",edid_Tag);
    else
        HI_HDMI_PRINT("EDID Tag is right(%x)\n",edid_Tag);

    edid_Reversion_Num = HDMI_ReadReg(pu32VirAddr, 0x3d0);
    
    time_out = 10;
    while((remain_cnt = HDMI_ReadReg(pu32VirAddr, 0x3d4)) != 8 && time_out != 0)
    {
    	HI_HDMI_PRINT("remain cnt %x\n",remain_cnt);
        edid_Reversion_Num = HDMI_ReadReg(pu32VirAddr, 0x3d0);
        time_out --;
    }
    
    if(edid_Reversion_Num != 0x03)
        HI_HDMI_PRINT("EDID Reversion Num is wrong(%x)\n",edid_Reversion_Num);
    else
        HI_HDMI_PRINT("EDID Reversion Num is right(%x)\n",edid_Reversion_Num);
    return 0;
}

STATIC  HI_VOID hdmi_cec_test(void)
{
    HI_U32 u32BaseAddr;
    HI_U32 	u32Tmp;
    volatile HI_U32  cec_end,cec_status;   

#if 0    
	u32BaseAddr = SYS_PHY_BASE_ADDR;
    
    u32Tmp = 0x00000300;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x048, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x048);

    if(0x00000300 != u32Tmp)
        HI_HDMI_PRINT("\n CecTest SYSPhy 0x048 now&&should be %d && 0x00000300 \n",u32Tmp);
#endif

    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;


    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0xa38);
    //pu32VirAddr[0] = 0x00000004;
    //(0xa38 -800) / 4 = 0xc0下0x8e  
    // CEC_FORCE_NON_CALIB = 1
    // must be set to '1' if calibration is not needed 
    // (If a stable 2MHz crystal clock is available)
    // CEC passthru register = 0
    u32Tmp = 0x00000004;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xa38, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xa38);

    /*if(0x00000004 != u32Tmp)
        HI_HDMI_PRINT("\n CecTest HDMI 0xa38 now&&should be %d && 0x00000004 \n",u32Tmp);*/
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0xb80); //clear cec ping
    //pu32VirAddr[0] = 0x00000002;
    //AUTO_PING_CLEAR
    u32Tmp = 0x00000002;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xb80, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xb80);

    /*if(0x00000002 != u32Tmp)
        HI_HDMI_PRINT("\n CecTest HDMI 0xb80 now&&should be %d && 0x00000002 \n",u32Tmp);*/
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0xb80); //start cec ping
    //pu32VirAddr[0] = 0x00000001;
    //AUTO_PING_START
    u32Tmp = 0x00000001;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xb80, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xb80);

    /*if(0x00000001 != u32Tmp)
        HI_HDMI_PRINT("\n CecTest HDMI 0xb80 now&&should be %d && 0x00000001 \n",u32Tmp);*/
    

    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0xb80);
    //cec_end = *pu32VirAddr;
    //AUTO_PING_START寄存器开始后，ping 15次后把bit7的AUTO_PING_DONE 制为1
    cec_end = HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xb80);
    while((cec_end & 0x80) == 0)
    {
    	HI_HDMI_PRINT("cec %x auto pinging...\n",cec_end);
        msleep(100);
        cec_end = HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xb80);
    }
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0xb84);

    //cec_status = *pu32VirAddr;
    cec_status = HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0xb84);
    
    if(cec_status == 0)
        HI_HDMI_PRINT("No CEC Device connected \n");
    else if((cec_status & 1) == 1)
        HI_HDMI_PRINT("A TV is connected \n");
    else
        HI_HDMI_PRINT("CECtest %d:A unknown CEC Device is connected \n",cec_status);
}

STATIC  HI_VOID soft_intr_test(void)
{
    HI_U32 u32BaseAddr;
    HI_U32 u32Tmp;
    HI_U32 errNum = 0;
    
    /*HDMI模块配置*/
    #if 0
	u32BaseAddr = SYS_PHY_BASE_ADDR;
    
    u32Tmp = 0x00000300;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x048, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x48);

    if(0x00000300 != u32Tmp)
        HI_HDMI_PRINT("\n intrTest SYSPhy 0x048 now&&should be %d && 0x00000300 \n",u32Tmp);
    #endif
    
    #if 0
    u32BaseAddr = HDMI_PHY_BASE_ADDR;

    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x1d4);
    //pu32VirAddr[0] = 0x00000080;
    //INTR5 0x75->0x76
    u32Tmp = 0x00000080;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1d8, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1d8);

    if(0x00000080 != u32Tmp)
        HI_HDMI_PRINT("\n intrTest HDMI 0x1d4 now&&should be %d && 0x00000080 \n",u32Tmp);
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x1e4);
    //pu32VirAddr[0] = 0x00000008;
    //原INT_CTRL寄存器地址从0x79转变为0x7B 
    //SOFT_INTR 设置为 1
    u32Tmp = 0x00000008;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1ec, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1ec);

    if(0x00000008 != u32Tmp)
        HI_HDMI_PRINT("\n intrTest HDMI 0x1e4 now&&should be %d && 0x00000008 \n",u32Tmp);
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x1e4);
    //pu32VirAddr[0] = 0x00000000;    
    //原INT_CTRL寄存器地址从0x79转变为0x7B 
    //SOFT_INTR 设置为 1
    u32Tmp = 0x00000000;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1ec, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1ec);

    if(0x00000000 != u32Tmp)
	    HI_HDMI_PRINT("\n intrTest HDMI 0x1ec now&&should be %d && 0x00000000 \n",u32Tmp);
    #endif
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1c0, u32Tmp);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1d8);
    
    //0x70 (1c0) INTR_STATE 0 
    //himm 0x71(1c4) 0xff ????×′ì?
    //0x71 ????3é1|???? error
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c0);
    if(u32Tmp != 0x00)
    {
        HI_HDMI_PRINT("INTR_STATE erro1 \n");
        errNum++;
    }

    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1c4, 0xff);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c4);
    if(u32Tmp != 0x00)
    {
        HI_HDMI_PRINT("Claer Intr erro2 \n");
        errNum++;
    }
    
    //himm 0x7B(1ec) 0x0a ?a??èí?D??éú3é
    //0x71(1c4)  != 0x80 error
    //himm 0x71 0xff ????×′ì?
    //0x71  != 0x80 error
    //0x70 INTR_STATE == 0
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1ec, 0x0a);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c4);
    if(u32Tmp != 0x80)
    {
        HI_HDMI_PRINT("Sw Intr erro3 \n");
        errNum++;
    }
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1c4, 0xff);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c4);
    if(u32Tmp != 0x80)
    {
        HI_HDMI_PRINT("Sw Intr erro4 \n");
        errNum++;
    }
    
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c0);
    if(u32Tmp != 0x00)
    {
        HI_HDMI_PRINT("INTR_STATE erro5 \n");
        errNum++;
    }

    //himm 0x76(1d8) 0x80  mask
    //0x70 INTR_STATE == 1
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1d8, 0x80);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c0);
    if(u32Tmp != 0x01)
    {
        HI_HDMI_PRINT("INTR_STATE erro6 \n");
        errNum++;
    }

    //himm 0x7B 0x02
    //himm 0x71 0xff ????×′ì?
    //0x71  != 0x00 error
    //0x70 INTR_STATE == 0
    //himm 0x76 0x00 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1ec, 0x02);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1c4, 0xff);
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c4);
    if(u32Tmp != 0x00)
    {
        HI_HDMI_PRINT("Claer Intr erro7 \n");
        errNum++;
    }

    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1c0);
    if(u32Tmp != 0x00)
    {
        HI_HDMI_PRINT("INTR_STATE erro8 \n");
        errNum++;
    }

    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1d8, 0x00);
    
    if(errNum > 0)
    {
        HI_HDMI_PRINT("\n  --- %d error found in soft_intr_test--- \n",errNum);
    }
    else
    {
        HI_HDMI_PRINT("\n  ---soft_intr_test  success--- \n");
    }
}

//720P50
STATIC  HI_VOID avi_frame_en(void)
{
    
    HI_U32 u32BaseAddr;
    char    *pVirAddr;
    //volatile HI_U32 *pu32VirAddr;

    //u32Size = MAX_SIZE;
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    pVirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);
    HI_HDMI_PRINT("\navi_frame_en g_Fmt : %d \n",g_Fmt);
    //#if 1
/*
    mv300 480P60
    AVI Inforframe:
    0x82,0x02,0x0d,0xc5,0x50,0x58,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
*/
    if(g_Fmt == 2)
    {
     /*
        mv300 1080i50
        AVI Inforframe:
        0x82,0x02,0x0d,0x63,0x50,0xa8,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */

    /*
        mv300 1080i60
        AVI Inforframe:        
        0x82,0x02,0x0d,0x72,0x50,0xa8,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */
        //1080i50  
        //AVI_TYPE
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x500);
        //pu32VirAddr[0] = 0x82;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x500, 0x82);
        //AVI_VERS
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x504);
        //pu32VirAddr[0] = 0x02;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x504, 0x02);
        //AVI_LEN
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x508);
        //pu32VirAddr[0] = 0x0d;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x508, 0x0d);
        //AVI InfoFrame Checksum. 怎么算出来的
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x50c);
        //pu32VirAddr[0] = 0xea;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x50c, 0x63);
        //AVI_DBYTE1 ~ AVI_DBYTE4 
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x510);
        //pu32VirAddr[0] = 0x02;
        //RGB && No format data present. && Bar data not valid. 
        // &&  0x02 Underscanned (computer).  
        //0x41 YCbCr 4:4:4  Overscanned (television). 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x510, 0x50);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x514);
        //pu32VirAddr[0] = 0x60; 
        //SMTPE 170M and ITU 601 (for standard definition TV) && aspectRatio 16:9
        //&& Active Format Aspect Ratio:Per DVB AFD active_format field. 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x514, 0xa8);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x518);
        //pu32VirAddr[0] = 0x05; 
        //Picture has been scaled horizontally
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x518, 0x00);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x51c);
        //pu32VirAddr[0] = 0x1e; 
        //???????a??×?á?ê2?′ vicD′è?
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x51c, 0x14);
    }
    else if(g_Fmt == 3)
    {
    //  mv300 PAL
    /*
        0x82,0x02,0x0d,0xb1,0x50,0x58,0x00,0x15,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */ 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x500, 0x82);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x504, 0x02);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x508, 0x0d);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x50c, 0xb1);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x510, 0x50);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x514, 0x58);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x518, 0x00);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x51c, 0x15);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x520, 0x01);
    }
    else if(g_Fmt == 5)
    {
    //  mv300 NTSC
    /*
        0x82,0x02,0x0d,0xc1,0x50,0x58,0x00,0x16,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */ 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x500, 0x82);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x504, 0x02);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x508, 0x0d);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x50c, 0xc0);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x510, 0x50);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x514, 0x58);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x518, 0x00);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x51c, 0x06);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x520, 0x01);
    }
    else if(g_Fmt == 4)
    {
    //  mv300 480P60
    //AVI Inforframe:
    //  0x82,0x02,0x0d,0xc5,0x50,0x58,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x500, 0x82);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x504, 0x02);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x508, 0x0d);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x50c, 0xc5);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x510, 0x50);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x514, 0x58);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x518, 0x00);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x51c, 0x02);
    }
    else 
    {
    //  mv300 720P50
    /*
        AVI InfoFrame
        0x82,0x02,0x0d,0x64,0x50,0xa8,0x00,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */   
     //   AVI Inforframe:
     //   0x82,0x02,0x0d,0x64,0x50,0xa8,0x00,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        //AVI_TYPE
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x500);
        //pu32VirAddr[0] = 0x82;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x500, 0x82);
        //AVI_VERS
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x504);
        //pu32VirAddr[0] = 0x02;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x504, 0x02);
        //AVI_LEN
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x508);
        //pu32VirAddr[0] = 0x0d;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x508, 0x0d);
        //AVI InfoFrame Checksum. 怎么算出来的
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x50c);
        //pu32VirAddr[0] = 0xea;
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x50c, 0x64);
        //AVI_DBYTE1 ~ AVI_DBYTE4 
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x510);
        //pu32VirAddr[0] = 0x02;
        //RGB && No format data present. && Bar data not valid. 
        // &&  0x02 Underscanned (computer).  
        //0x41 YCbCr 4:4:4  Overscanned (television). 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x510, 0x50);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x514);
        //pu32VirAddr[0] = 0x60; 
        //SMTPE 170M and ITU 601 (for standard definition TV) && aspectRatio 16:9
        //&& Active Format Aspect Ratio:Per DVB AFD active_format field. 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x514, 0xa8);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x518);
        //pu32VirAddr[0] = 0x05; 
        //Picture has been scaled horizontally
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x518, 0x00);
        //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x51c);
        //pu32VirAddr[0] = 0x1e; 
        //没搞懂这个做了什么 vic写入
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x51c, 0x13);
    //#endif
    }
    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x4f8);
    //pu32VirAddr[0] = 0x33;
    //avi && aud infoframe 使能
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4f8, 0x33);
} 

STATIC HI_VOID audio_frame_en(void)
{
    HI_U32 u32BaseAddr;
    char    *pVirAddr;
    //volatile HI_U32 *pu32VirAddr;

    //AUD Inforframe:
    //0x84,0x01,0x0a,0x70,0x01,0x00,0x00,0x00,0x00,

    //u32Size = MAX_SIZE;
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    pVirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);
    HI_HDMI_PRINT("\n audio_frame_en \n");
    
    //hdmi mode
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4bc, 0x01);
    //AUDIO_TYPE
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x600, 0x84);
    //AUDIO_VERS
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x604, 0x01);
    //AUDIO_LEN
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x608, 0x0a);

    
    //AUDIO_DBYTE1 
    if(g_MultyChn == 0)
    {   
        //AUDIO InfoFrame Checksum. 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x60c, 0x70);
        //2ch
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x610, 0x01);

        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x61c, 0x00);
    }
    else
    {   
        //AUDIO InfoFrame Checksum. 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x60c, 0x4b);
        //8ch
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x610, 0x07);
        
        //Speaker Mapping and Down-mix Information CEA-861-D p90
        //0x1f  FRC  FLC  RR  RL  FC  LFE  FR  FL   
        //0x13  RRC  RLC  RR  RL  FC  LFE  FR  FL 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x61c, 0x1f);
    }

    //AUDIO_DBYTE2 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x614, 0x00);
    //AUDIO_DBYTE3 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x618, 0x00);
    //AUDIO_DBYTE4 
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x61c, 0x00);
    //AUDIO_DBYTE5 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x620, 0x00);
    //AUDIO_DBYTE6 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x624, 0x00);
    //AUDIO_DBYTE7 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x628, 0x00);
    //AUDIO_DBYTE8 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x62c, 0x00);
    //AUDIO_DBYTE9 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x630, 0x00);
    //AUDIO_DBYTE10 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x634, 0x00);


    //pu32VirAddr = (HI_U32 *)(pVirAddr + 0x4f8);
    //pu32VirAddr[0] = 0x33;
    //avi && aud infoframe 使能
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4f8, 0x33);

    // audio InfoFrame 0x7a:0x80
    //himd.l 0x10170600  


} 

STATIC HI_VOID ge_frame_en(void)
{
    HI_U32 u32BaseAddr;
    char    *pVirAddr;
    //volatile HI_U32 *pu32VirAddr;

    //u32Size = MAX_SIZE;
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    pVirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);
    HI_HDMI_PRINT("\ngeneral_frame_en\n");

    //hdmi mode
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4bc, 0x01);

    //  mv300 720P50
    /*
        AVI InfoFrame
        0x82,0x02,0x0d,0x64,0x50,0xa8,0x00,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    */
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x700, 0x82);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x704, 0x02);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x708, 0x0d);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x70c, 0x64);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x710, 0x50);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x714, 0xa8);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x718, 0x00);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x71c, 0x13);
    
    //ge infoframe 使能   
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4fc, 0x33);

}

STATIC HI_VOID ge_frame2_en()
{
    HI_U32 u32BaseAddr;
    char    *pVirAddr;
    //volatile HI_U32 *pu32VirAddr;

    //u32Size = MAX_SIZE;
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    pVirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);
    HI_HDMI_PRINT("\ngeneral_frame_en\n");

    //hdmi mode
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4bc, 0x01);

    //AUDIO_TYPE
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x780, 0x84);
    //AUDIO_VERS
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x784, 0x01);
    //AUDIO_LEN
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x788, 0x0a);
    //AUDIO InfoFrame Checksum. 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x78c, 0x70);
    //AUDIO_DBYTE1 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x780, 0x01);

    
    //ge infoframe 使能   
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4fc, 0x33);

}

    
STATIC HI_VOID vendor_frame_en()
{
      HI_U32 u32BaseAddr;
    char    *pVirAddr;
    //volatile HI_U32 *pu32VirAddr;

    //u32Size = MAX_SIZE;
    
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    pVirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR);
    HI_HDMI_PRINT("\nvendor_frame_en\n");

    //hdmi mode
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4bc, 0x01);


    #if 0
    //  mv300 720P50
    /*
       side by side (half)
    */
    //VENDORSPEC_TYPE 0x81
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x700, 0x81);
    //Version 0x01
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x704, 0x01);
    
    //length
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x708, 0x09);// will be change
    //checkSum
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x70c, 0xa6);
    //PB1
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x710, 0x03);
    //PB2
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x714, 0x0c);
    //PB3
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x718, 0x00);
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x71c, (0x02 << 5));//0x40
    // u83D_Structure (side-by-side(half))  &  u83D_Meta_present
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x720, (0X08 << 4) | (0X00 << 3) );//0x80
    // 3D_Ext_Data = 0x00; //Old/Left picture, Odd/Right picture
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x724, (0x00 << 4));//0x00
    // u83D_Metadat_type & u83D_Metadata_Length
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x728, ((0x00 << 5) | 0x00) );

    #else
    //  mv300 720P50
    /*
       frame packing
    */
    //VENDORSPEC_TYPE 0x81
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x700, 0x81);//ok
    //Version 0x01
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x704, 0x01);//ok
    
    //length
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x708, 0x09);// will be change
    //checkSum
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x70c, 0x26);
    //PB1
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x710, 0x03);//ok
    //PB2
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x714, 0x0c);//ok
    //PB3
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x718, 0x00);//ok
    //3D present 0x02 << 5
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x71c, (0x02 << 5));//0x40 ok
    // u83D_Structure (FramePacking)  &  u83D_Meta_present
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x720, (0X00 << 4) | (0X00 << 3) );//0x00 ok
    // 3D_Ext_Data = 0x00; //Old/Left picture, Odd/Right picture
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x724, (0x00 << 4));//0x00
    // u83D_Metadat_type & u83D_Metadata_Length
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x728, ((0x00 << 5) | 0x00) );

    
    #endif
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x72c, (0x02 << 5));
    
    //ge infoframe 使能   
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4fc,0x03);

}


HI_VOID CRGHdmiInit(void)
{
    HI_U32	u32BaseAddr;
    u32BaseAddr = SYS_PHY_BASE_ADDR;//f8a22000

    //vdp crg
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0d8, 0x05E107FF);
    
	//u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x048);
	//u32Tmp = 0x00010300;
	//HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x048, u32Tmp);
    HI_HDMI_PRINT("\n HDMI Controller CRG Initial Begin  \n");
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10c, 0x33f);   //Controller CRG rst-> unrst
    HI_HDMI_PRINT("\n HDMI CRG  0x10C = 0x3FF  : HDMI Tx Soft Rst enable \n");

    msleep(10);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10c, 0x3f);
    HI_HDMI_PRINT("\n HDMI CRG  0x10C = 0x3F  : HDMI Tx Soft Rst Disable \n");
    HI_HDMI_PRINT("\n HDMI Controller CRG Initial End  \n");

//Phy CRG rst -> unrst is unnecessary 
/*
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x110, 0x11);    // Phy CRG RST -> Unrst
    msleep(10);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x110, 0x01);  */

}

HI_VOID PINHdmiInit(void)
{
    HI_U32	u32BaseAddr;
    u32BaseAddr = 0xf8a21000;

    //himm 0xF8A21000 0x01 HDMI_TX_SCL
    //himm 0xF8A21004 0x01 HDMI_TX_SDA
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x00, 0x01);   //DDC 
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04, 0x01);
    
    //himm 0xF8A21008 0x01 hpd
    //himm 0xF8A2100C 0x01 cec
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x08, 0x01);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0C, 0x01);

#ifdef __S40_FPGA__
    //0xf8a210f4 0x01 i2c管脚 
    //0xf8a210f8 0x01 i2c管脚
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xf4, 0x01);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xf8, 0x01);
#endif
    HI_HDMI_PRINT("\n PINHdmiInit \n");
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_VOID ColorBar576P50(void)
{
    HI_U32	u32BaseAddr;
    g_Fmt = 4;
    u32BaseAddr = 0xf8cc0000;
    
    HI_HDMI_PRINT("\nColorBar576P60\n");
    //g_Fmt = 0; 
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_576P);
    VDP_DISP_SetRegUp(0);
   

    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe00f8011);

    //avi_frame_en();
}

HI_VOID ColorBarPAL(void)
{
    HI_U32	u32BaseAddr;
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    // 0x48 bit[0,1]  Iclk 0x00 0x01 2x  0x03 4x N、P外其他Timing需要清零
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0120, 0x01);
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBarPAL\n");
    g_Fmt = 3;
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_PAL);
    //VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_NTSC);
    VDP_DISP_SetRegUp(0);
    //hmid 300-1     ????
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0xc00c,0x012b000b);
    //hmid 600-1
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0xc00c,0x0257000b);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0x0100,0x08);
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe0000001);
    //avi_frame_en();
}

HI_VOID ColorBarNTSC(void)
{
    HI_U32	u32BaseAddr;
    u32BaseAddr = HDMI_PHY_BASE_ADDR;
    // 0x48 bit[0,1]  Iclk 0x00 0x01 2x  0x03 4x N、P外其他Timing需要清零
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0120, 0x01);
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBarNTSC\n");
    g_Fmt = 5;
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_NTSC);
    VDP_DISP_SetRegUp(0);
    //hmid 310-1     ????
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0xc00c,0x0135000b);
    //hmid 620-1     ????
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0xc00c,0x026b000b);
    
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0x0100,0x08);

    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe0000001);
    //avi_frame_en();
}


HI_VOID ColorBar480P60(void)
{
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBar480P60\n");
    g_Fmt = 4;
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_480P);
    VDP_DISP_SetRegUp(0);
    
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0x0100,0x00);
    

    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe00f8011);

    //avi_frame_en();
}


//extern HI_VOID SI_SetAudioPath( HI_U8 *abAudioPath);

HI_VOID ColorBar720P50(void)
{
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBar720P50\n");
    g_Fmt = 0; // 0 720P50 1 720P60 2 1080i
    
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc008, 0x10304ff);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc00c, 0x1b7);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc004, 0x4182cf);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc010, 0x0);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc014, 0x40027);

    
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_720P);
    VDP_DISP_SetRegUp(0);

    //dac ????
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0104, 0x654);
    //DHD
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc020 ,0x2000);
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe00f8011);
 
    
    //avi_frame_en();
    //vendor_frame_en();
}

HI_VOID ColorBar720P60(void)
{
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBar720P60\n");
    g_Fmt = 1; // 0 720P50 1 720P60 2 1080i
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc008, 0x10304ff);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc00c, 0x6d);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc004, 0x4182cf);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc010, 0xfffffff);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc014, 0x10007);

    //VDP_ENV_INIT();
    //VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_720P_60);
    //VDP_DISP_SetRegUp(0);
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0104, 0x654);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc020 ,0x2000);

    //HY 6d
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe00f8011);

    //avi_frame_en();
    
}

HI_VOID ColorBar1080I50(void)
{
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8cc0000;
    HI_HDMI_PRINT("\nColorBar1080I50\n");
    g_Fmt = 2; // 0 720P50 1 720P60 2 1080i
    
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0104, 0x654);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc020 ,0x2000);
    
    VDP_ENV_INIT();
    VDP_DISP_OpenTypIntf(0,VDP_DISP_DIGFMT_1080I);
    
    VDP_DISP_SetRegUp(0);
    // H Y
    //himm 0xf8ccc00c 0x468020f
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr),0xc00c,0x468020f);
    
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0xe00f8001);

    //????VDPoóDèòaé?μèò??á?ùê1ê±Dò?è?¨??à′
    msleep(50);
    if((HDMI_ReadReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR),0x00fc) & 0x04) != 0)
    {
        HI_HDMI_PRINT("interlace Detect right! \n");
    }
    else
    {
        HI_HDMI_PRINT("interlace Detect error! \n");
    }
    //avi_frame_en();
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/



HI_VOID SocHdmiInit(HI_U32 hdcp)
{
    //HI_DRV_HDMI_Init();
    //HI_DRV_HDMI_Open(0);
    HI_HDMI_PRINT("--- SocHdmiInit ---\n");
#if 1
	HI_U32	u32BaseAddr;
	HI_U32 	u32Tmp;    
    //HI_U8  abAudioPath[4];

    /*HDMI CRG , HDMI复位和撤销*/
    CRGHdmiInit();
    
    /*HDMI模块配置*/
	u32BaseAddr = HDMI_PHY_BASE_ADDR;

    PINHdmiInit();
    //HDMI -> MHL 拔低 
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x3fc, 0x00);
    
    // 0x69 bit[1,2] 0x1a4 wide bus
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x01a4, 0x04);
    
//#ifdef FPGA_BOARD
#if 1
    SocHdmiPhyInit();
#endif    

#if 1
	//软复位 && 清空audiofifo
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x14, 0x03);
    msleep(10);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x14, 0x00);

    //hdmi mode
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x4bc, 0x21);

    //VIN_MODE  10bit
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1a4, 0x04);

     #if 0 
    //输出制式设置
    //csc、RGB/Ycbcr、color range convert、dither、demux、upsmp
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x124);
	u32Tmp &=0x3f;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x124, u32Tmp);

	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x128);
	u32Tmp &=0x3f;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x128, u32Tmp);
    #endif
    
    #if 0 
    //phy的初始化
   
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1804);
	u32Tmp =0x00000028;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1804, u32Tmp);
    
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1820);
	u32Tmp =0x00000060;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1820, u32Tmp);

	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1808);
	u32Tmp =0x000000a9;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1808, u32Tmp);
    
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x180c);
	u32Tmp =0x00000040;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x180c, u32Tmp);
    
    //??
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x1818);
	u32Tmp =0x00000010;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x1818, u32Tmp);
    /*
    */
    #endif

    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04f4, 0x17);
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04f4, 0x17);
    
    #if 1
    //power up && Latch Input on Falling Edge && Input bus select: 24-bit 
    //Follow VSYNC input && Follow HSYNC input
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x020);
	u32Tmp = 0x00000035;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x020, u32Tmp);
    //ColorBar1080I50();
    //ColorBar720P50();
    //ColorBar720P60();
    #endif
    
   
    #if 0
        //AudioInit(0x01,0x02,0x02,0x01,0x01);
        //0x10170474(0x7a, 0x1d):0x00 -> 0x40
        // SCK sample edge:  1 = Sample clock is rising; SD3-SD0 and WS source should change 
        // state on the falling edge of SCK 
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x474, 0x40);
        HI_HDMI_PRINT("\nover3\n");
    #endif

    //输出所有寄存器值
    // 0 = ddc ;1 = audio;2 = all
    //hdmi_RegDebug(2);
     
#endif
    //hdmi_edid_test();
#else

    //HI_DRV_HDMI_Init();

    
#endif
}

HI_VOID SocHdmiDeInit(void)
{
    HI_U32	u32BaseAddr;    
    u32BaseAddr = 0xf8cc0000;
    CRGHdmiInit();
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0xc000, 0x000f8001);
    HI_HDMI_PRINT("Tx DeInit ok \n");
}

#if 1
HI_VOID SetDeepColor (VIDEO_DPCOLOR_MODE_E dpmode)
{
    HI_U8 ret = 0;
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8ce0000;
    // 0x69 bit[1,2] 0x1a4 wide bus
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x01a4, 0x04);

    if(dpmode == HDMI_TX_DP_8BIT)
    {
        //0x7A:0x2f bit[6] bit[5-3] 0x69 bit30    0x71 36bit  packet
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04bc, 0x21);

        //0x4a dither himm 0x10170128 0x60 10bit  0xa0 12bit
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0128, 0x20);

        //phy 
        // 0xa9 8bit  0xab 10bit  0xad 12bit 
        write_tx_phy(0x60,0x02,0xa9);
        msleep(10);
        ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n I2C0x60 0x02 now should be 0x%x && 0xa9 \n",ret);
    }
    else if(dpmode == HDMI_TX_DP_10BIT)
    {
        //0x7A:0x2f bit[6] bit[5-3] 0x69 bit30    0x71 36bit  packet
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04bc, 0x69);

        //0x4a dither himm 0x10170128 0x60 10bit  0xa0 12bit
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0128, 0x60);

        //phy 
        // 0xa9 8bit  0xab 10bit  0xad 12bit 
        write_tx_phy(0x60,0x02,0xab);
        msleep(10);
        ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n I2C0x60 0x02 now should be 0x%x && 0xa9 \n",ret);
    }
    else if(dpmode == HDMI_TX_DP_12BIT)
    {
        //0x7A:0x2f bit[6] bit[5-3] 0x69 bit30    0x71 36bit  packet
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04bc, 0x71);

        //0x4a dither himm 0x10170128 0x60 10bit  0xa0 12bit
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0128, 0xa0);

        //phy 
        // 0xa9 8bit  0xab 10bit  0xad 12bit 
        write_tx_phy(0x60,0x02,0xad);
        msleep(10);
        ret=read_tx_phy(0x60,0x02);
        HI_HDMI_PRINT("\n I2C0x60 0x02 now should be 0x%x && 0xa9 \n",ret);
    }
    else
    {
        
    }

#if 0 
    //0x7A:0x2f bit[6] bit[5-3] 0x69 bit30    0x71 36bit  packet
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x04bc, 0x69);

    
    //0x4a dither himm 0x10170128 0x60 10bit  0xa0 12bit
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0128, 0x60);

    //phy 
    // 0xa9 8bit  0xab 10bit  0xad 12bit 
    write_tx_phy(0x60,0x02,0xab);
    msleep(10);
    ret=read_tx_phy(0x60,0x02);
    HI_HDMI_PRINT("\n I2C0x60 0x02 now should be 0x%x && 0xa9 \n",ret);
#endif

    return;  
}

// 1 edid; 2 cec; 3 sw-intr; 4 SocHdmiDeInit; 5 hdcp_en; 6 ColorBar720P50; 7 ColorBar720P60; 8 ColorBar1080I50; 
// 9 AudioInit; 10 avi_frame_en; 11 audio frame ; 12 ge frame;13 ge frame2 ;14 vendor frame ;15 sw intr
HI_VOID CheckFunction(HI_S32 funnum)
{
    switch(funnum)
    {
    case 1:
        hdmi_edid_test();
        break;
    case 2:
        hdmi_cec_test();
        break;
    case 3:
        soft_intr_test();
        break;
    case 4:
        SocHdmiDeInit();
        break;
    case 5:
        hdcp_en(1);
        break;
    case 6:
        //ColorBar720P50();
        break;
    case 7:
        //ColorBar720P60();
        break;
    case 8:
        //ColorBar1080I50();
        break;
    case 9:
        //AudioInit();
        break;
    case 10:
        avi_frame_en();
        break;
    case 11:
        audio_frame_en();
        break;
    case 12:
        ge_frame_en();
        break;
    case 13:
        ge_frame2_en();
        break;
    case 14:
        vendor_frame_en();
        break;
    case 15:
        soft_intr_test();
        break;
    case 16:
        //ColorBar480P60();
        break;
    case 17:
        //SetDeepColor();
        break;
    case 18:
        //ColorBar576P50();
        break;
    case 19:
        //ColorBarPAL();
        break;
    default:
        //hdmi_edid_test();
        //hdmi_cec_test();
        //soft_intr_test();
        //avi_frame_en();
        //audio_frame_en();
        break;
    }
}


HI_VOID TPIHdmiInit(HI_U32 hdcp)
{
	HI_U32	u32BaseAddr;
	HI_U32 	u32Tmp;	
	
#ifdef FPGA_BOARD
    SocHdmiFpgaPhyInit();
#endif    
    /*HDMI CRG , 撤消HDMI复位*/
    u32BaseAddr = SYS_PHY_BASE_ADDR;
	u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x048);
	u32Tmp = 0x00010300;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x048, u32Tmp);

    //TPI 初始化
    //需要替换成 TPI_PHY_BASE_ADDR
    u32BaseAddr = HDMI_PHY_BASE_ADDR;

    //0x72:0xC7 开启TPI模式
    u32Tmp = 0x00000000;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x31c, u32Tmp);

    //Detect Revision 
    u32Tmp = 0x000000B4;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x06c, u32Tmp);
    u32Tmp = 0x00000020;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x070, u32Tmp);
    u32Tmp = 0x00000030;
    HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x074, u32Tmp);

    //Power up transmitter 
    u32Tmp=HDMI_ReadReg(IO_ADDRESS(u32BaseAddr),0x078);
	u32Tmp = u32Tmp & 0xfc;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x078, u32Tmp);

    //Configure Input Bus and Pixel Repetition 0x08
    // 720P  0x70 输入极性
    //N/P 制式 极性 相反 0x60
    u32Tmp = 0x00000070;
	HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x020, u32Tmp);

    //Configure Sync Methods 
    
    
    //Select YC Input Mode 

    //DE generator
    //VSYNC Polarity || HSYNC Polarity
    //咱们的VDP只有分离信号，且分离信号上带了GE信号

    //Configure Embedded Sync Extraction
    //分离信号不需要配置这些

    //Set up Interrupt Service 
}

// mask Based on Doc: Dual-Mode_HDMI_1 4b_MHL_2 0_Transmitter_IP_PR_FINAL.pdf(version 0.4)
// y00229039
HI_U8 Mask_1st[] = 
{   
//  0x00,  0x01,  0x02, 0x03,  0x04, 0x05,  0x06, 0x07,  0x08, 0x09,  0x0a, 0x0b,  0x0c,  0x0d, 0x0e,  0x0f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 0xff, 0xff, 0x40, 0xff, 0xf9, 0xff, 0xff, 0xf8, 0xff, 0x62, //00
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //10
    0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, //20    
    0xff, 0xff, 0x00, 0x80, 0x80, 0xff, 0x00, 0xe0, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, //30
    0x00, 0xfc, 0x00, 0xe0, 0x00, 0xfc, 0xc0, 0xc0, 0x0c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, //40
    0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //60    
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfc, 0xf0, 0xf1, 0x00, 0x00, 0xff, 0xff, //70
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //80    
    0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0x00, 0x00, //90
    0x00, 0xf0, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //a0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //b0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x00, 0x00, 0x00, //c0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //d0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x01, 0x00, 0x00, //e0
    0x00, 0xfc, 0x9f, 0xc0, 0x00, 0x9f, 0x80, 0x80, 0x80, 0x98, 0xc0, 0x01, 0xff, 0xff, 0xff, 0x7e  //f0
};


HI_U8 Mask_2nd[] = 
{
//  0x00,  0x01,  0x02, 0x03,  0x04, 0x05,  0x06, 0x07,  0x08, 0x09,  0x0a, 0x0b,  0x0c,  0x0d, 0x0e,  0x0f,
    0xff, 0xf8, 0xf8, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, //00
    0xff, 0xff, 0xff, 0xff, 0x00, 0xf8, 0x00, 0xff, 0xff, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, //10
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, //20
    0x3c, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x53, 0x60, 0x00, 0x00, //30    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //40
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //50
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //60
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, //70
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, //80    
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //a0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, //b0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //c0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, //d0    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //e0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff  //f0
};

HI_U8 Mask_cec[] = 
{
//  0x00,  0x01,  0x02, 0x03,  0x04, 0x05,  0x06, 0x07,  0x08, 0x09,  0x0a, 0x0b,  0x0c,  0x0d, 0x0e,  0x0f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //00
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //10
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //20
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //30
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //40
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //50
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //60
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //70
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x78, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x00, //80    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, //90
    0xf8, 0xff, 0x00, 0x00, 0xd8, 0xf0, 0x08, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xbc, 0xff, 0xff, 0xff, //a0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //b0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //c0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //d0
    0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //e0
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  //f0
};


HI_VOID RegRWTest(void)
{
    HI_U32 index;
    volatile HI_U32  *pu32VirAddr;
    HI_U32 temp;
    HI_U32 ori;
    HI_U32 count = 255;


    
    //HDMI_WriteReg(pu32VirAddr,0x31c, 0x00);TPI Mode 下部分寄存器变为不能读写
#if 1 

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR); 
    HI_HDMI_PRINT("Debug hdmi All 0x72 register:\n\n");
    
    HI_HDMI_PRINT("Writing 0xff.......\n");
    for (index = 0x00; index <= 0xFF; index++)
    {
        
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));

        //TPI mode 需要单独测试
        if(index == 0xc7)
        {
            continue;
        }

        HDMI_WriteReg(pu32VirAddr,(index * 4),0xff);
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        if((temp | Mask_1st[index] )!= 0xff)
        {
           HI_HDMI_PRINT("\n 0x72:0x%02x == 0x%02x \n",index,temp);
           count--;
        }        

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }

    HI_HDMI_PRINT("Writing 0x00.......\n");
    for (index = 0x00; index <= 0xFF; index++)
    {
        
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        if(index == 0xc7)
        {
            continue;
        }
        
        HDMI_WriteReg(pu32VirAddr,(index * 4),0x00);
        
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        if((temp & (0xff - Mask_1st[index]) )!= 0x00)
        {
           HI_HDMI_PRINT("\n 0x72:0x%02x == 0x%02x \n",index,temp);
           count--;
        }

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }
    HI_HDMI_PRINT("Debug hdmi 0x72 register End:\n\n");
#endif

#if 1

    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR + 0x400); 

    HI_HDMI_PRINT("Debug hdmi All 0x7A register:\n\n");
    
    HI_HDMI_PRINT("Writing 0xff.......\n");
    for (index = 0x00; index <= 0xff; index++)
    {
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));           
        HDMI_WriteReg(pu32VirAddr,(index * 4),0xff);        
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        if((temp | Mask_2nd[index] )!= 0xff)
        {
           HI_HDMI_PRINT("\n 0x7A:0x%02x == 0x%02x \n",index,temp);
           count--;
        }        

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }

    //无码流时 0x3e,0x3f 不能关闭使能为正常现象，要遇到垂直时序中断 才能关闭使能
    HI_HDMI_PRINT("Writing 0x00.......\n");
    for (index = 0x00; index <= 0xff; index++)
    {
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));        
        HDMI_WriteReg(pu32VirAddr,(index * 4),0x00);        
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        if((temp & (0xff - Mask_2nd[index]) )!= 0x00)
        {
           HI_HDMI_PRINT("\n 0x7A:0x%02x == 0x%02x \n",index,temp);
           count--;
        }

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }
    HI_HDMI_PRINT("Debug hdmi 0x7A register End:\n\n");
#endif


#if 1 
    pu32VirAddr = (HI_U32 *)IO_ADDRESS(HDMI_PHY_BASE_ADDR + 0x800); 
    HI_HDMI_PRINT("Debug hdmi All 0xC0 register:\n\n");
    
    HI_HDMI_PRINT("Writing 0xff.......\n");
    for (index = 0x00; index <= 0xFF; index++)
    {
        
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));

        HDMI_WriteReg(pu32VirAddr,(index * 4),0xff);
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        if((temp | Mask_cec[index] )!= 0xff)
        {
           HI_HDMI_PRINT("\n 0xC0:0x%02x == 0x%02x \n",index,temp);
           count--;
        }        

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }

    HI_HDMI_PRINT("Writing 0x00.......\n");
    for (index = 0x00; index <= 0xFF; index++)
    {
        
        ori = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        HDMI_WriteReg(pu32VirAddr,(index * 4),0x00);
        
        temp = HDMI_ReadReg(pu32VirAddr,(index * 4));
        
        if((temp & (0xff - Mask_cec[index]) )!= 0x00)
        {
           HI_HDMI_PRINT("\n 0xC0:0x%02x == 0x%02x \n",index,temp);
           count--;
        }

        HDMI_WriteReg(pu32VirAddr,(index * 4),ori);
       
    }
    HI_HDMI_PRINT("Debug hdmi 0xC0 register End:\n\n");
#endif


    #endif
    HI_HDMI_PRINT("\n %d count\n",count);
    HI_HDMI_PRINT("\nFinish\n");
}


#if 0 /*--无效函数--*/
// HDMI_TX_RPT_1X = 0,
// HDMI_TX_RPT_2X,
// HDMI_TX_RPT_4X,
// HDMI_TX_RPT_BUTT    

HI_VOID PixelRpt(VIDEO_RPT_MODE_E rptmode)
{
    HI_U8 ret = 0;
    HI_U32	u32BaseAddr;
    
    u32BaseAddr = 0xf8ce0000;

    if(rptmode == HDMI_TX_RPT_1X)
    {
        // 0x48 bit[0,1]  Iclk 0x00 0x01 2x  0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0120, 0x00);

        //0x7A:0x2f bit[6] bit[5-3] 0x01 2x   0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0520, 0x00);

        //phy 
        // 0xa9  1x  0xb1 2X  0xb9 4X
        //write_tx_phy(0x60,0x02,0xa9);
        msleep(10);
        //ret=read_tx_phy(0x60,0x02);
    }
    else if(rptmode == HDMI_TX_RPT_2X)
    {
        // 0x48 bit[0,1]  Iclk 0x00 0x01 2x  0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0120, 0x01);

        //0x7A:0x2f bit[6] bit[5-3] 0x01 2x   0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0520, 0x00);
        msleep(10);
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0520, 0x01);

        //phy 
        // 0xa9  1x  0xb1 2X  0xb9 4X
        write_tx_phy(0x60,0x02,0xb1);
        msleep(10);
        //ret=read_tx_phy(0x60,0x02);
    }
    else if (rptmode == HDMI_TX_RPT_4X)
    {
        // 0x48 bit[0,1]  Iclk 0x00 0x01 2x  0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0120, 0x03);

        //0x7A:0x2f bit[6] bit[5-3] 0x01 2x   0x03 4x
        HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x0520, 0x03);

        //phy 
        // 0xa9  1x  0xb1 2X  0xb9 4X
        write_tx_phy(0x60,0x02,0xb9);
        msleep(10);
    }
    else
    {
        HI_HDMI_PRINT("\n error Rpt Mode \n");
    }
}
#endif /*--无效函数--*/

static HI_U32 g_HDMIIrqHandle = 0;
#define HDMI_TX_IRQ_NUM (88 + 32)

void Isr_Print(void)
{
    HI_HDMI_PRINT("\n Tx Isr triggered \n");
    HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1c4, 0x40);
}

void testTxIsrSetup(void)
{
    //HI_HDMI_PRINT("TxIsrSetup\n");
#if 1
    HI_HDMI_PRINT("TxIsrSetup\n");
    if (g_HDMIIrqHandle != 0)
    {
        HI_HDMI_PRINT("HDMI force to free HMDI Irq first\n");
        free_irq(HDMI_TX_IRQ_NUM, &g_HDMIIrqHandle);
        g_HDMIIrqHandle = 0;
    }
    CRGHdmiInit();
    //0x7b hdmi 被中断信号被取反了，低有效
    HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1ec, 0x02);
    //HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1ec, 0x00);

    //0x71 clear old intrrupt
    HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1c4, 0x40);
    //mask 0x76 
    HDMI_WriteReg(IO_ADDRESS(HDMI_PHY_BASE_ADDR), 0x1d8, 0x40);
#endif
    /* Setup HDMI Interrupt */
    if (request_irq(HDMI_TX_IRQ_NUM, (irq_handler_t)Isr_Print, IRQF_SHARED, "HDMI_Tx_IRQ", &g_HDMIIrqHandle) != 0)
    {
        HI_HDMI_PRINT("HDMI registe IRQ failed!\n");
        //return IRQ_HANDLED;
    }
    //return IRQ_HANDLED;
}

void testTxIsrExit(void)
{
    HI_HDMI_PRINT("TxIsrExit\n");
    if (g_HDMIIrqHandle != 0)
    {
        free_irq(HDMI_TX_IRQ_NUM, &g_HDMIIrqHandle);
        HI_HDMI_PRINT("close hdmi irq\n");
        g_HDMIIrqHandle = 0;

    }
}

HI_VOID set_fmt(HI_UNF_ENC_FMT_E enFmt)
{
    switch(enFmt)
    {
        case HI_UNF_ENC_FMT_1080i_50:
            g_Fmt = 2;
            break;
        case HI_UNF_ENC_FMT_720P_50:
            g_Fmt = 0;
            break;
        case HI_UNF_ENC_FMT_PAL:
            g_Fmt = 3;
            break;
        case HI_UNF_ENC_FMT_NTSC:
            g_Fmt = 5;
            break;
        case HI_UNF_ENC_FMT_480P_60:
            g_Fmt = 4;
            break;
        default:
            //720P50
            g_Fmt = 0;
            break;
    }
}
