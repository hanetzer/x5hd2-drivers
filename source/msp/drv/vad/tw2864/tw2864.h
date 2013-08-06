#ifndef __TW2864_H__
#define __TW2864_H__

#include "hi_debug.h"

#define HI_FATAL_TW2864(fmt...) HI_FATAL_PRINT(HI_ID_TW2864, fmt)
#define HI_ERR_TW2864(fmt...) HI_ERR_PRINT(HI_ID_TW2864, fmt)
#define HI_WARN_TW2864(fmt...) HI_WARN_PRINT(HI_ID_TW2864, fmt)
#define HI_INFO_TW2864(fmt...) HI_INFO_PRINT(HI_ID_TW2864, fmt)
#define HI_DBG_TW2864(fmt...) HI_DBG_PRINT(HI_ID_TW2864, fmt)

#define TW2864_NAME "HI_TW2864"

#define AUTOMATICALLY 0
#define NTSC 1
#define PAL 2

#define TW2815_READ_REG 0xbF01
#define TW2815_WRITE_REG 0xbF02
#define TW2815_SET_ADA_PLAYBACK_SAMPLERATE 0xbF03
#define TW2815_SET_ADA_PLAYBACK_BITWIDTH 0xbF04
#define TW2815_SET_ADA_PLAYBACK_BITRATE 0xbF05
#define TW2815_SET_D1 0xbF06
#define TW2815_SET_2_D1 0xbF07
#define TW2815_SET_4HALF_D1 0xbF08
#define TW2815_SET_4_CIF 0xbF09
#define TW2815_SET_AUDIO_OUTPUT 0xbF0a
#define TW2815_SET_AUDIO_MIX_OUT 0xbF0b
#define TW2815_SET_AUDIO_RECORD_M 0xbF0c
#define TW2815_SET_MIX_MUTE 0xbF0d
#define TW2815_CLEAR_MIX_MUTE 0xbF0e
#define TW2815_SET_ADA_SAMPLERATE 0xbF0f
#define TW2815_SET_ADA_BITWIDTH 0xbF10
#define TW2815_SET_ADA_BITRATE 0xbF11
#define TW2815_SET_VIDEO_MODE 0xbF12

#define TW2815_GET_ADA_PLAYBACK_SAMPLERATE 0xcF03
#define TW2815_GET_ADA_PLAYBACK_BITWIDTH 0xcF04
#define TW2815_GET_ADA_PLAYBACK_BITRATE 0xcF05
#define TW2815_GET_D1 0xcF06
#define TW2815_GET_2_D1 0xcF07
#define TW2815_GET_4HALF_D1 0xcF08
#define TW2815_GET_4_CIF 0xcF09
#define TW2815_GET_AUDIO_OUTPUT 0xcF0a
#define TW2815_GET_AUDIO_MIX_OUT 0xcF0b
#define TW2815_GET_AUDIO_RECORD_M 0xcF0c
#define TW2815_GET_MIX_MUTE 0xcF0d
#define TW2815_GET_MIX_MUTE_CLEAR 0xcF0e
#define TW2815_GET_ADA_SAMPLERATE 0xcF0f
#define TW2815_GET_ADA_BITWIDTH 0xcF10
#define TW2815_GET_ADA_BITRATE 0xcF11
#define TW2815_GET_VIDEO_MODE 0xcF12

#define SET_8K_SAMPLERATE 0
#define SET_16K_SAMPLERATE 1
#define SET_8_BITWIDTH 0
#define SET_16_BITWIDTH 1
#define SET_256_BITRATE 0
#define SET_384_BITRATE 1

/*define for TW2815A*/
#define TW2815A_I2C_ADDR 0x50
#define TW2815_ID 0x59

/*define for TW2815B*/
#define TW2815B_I2C_ADDR 0x52
#define TW2815_ID 0x59

#define serial_control 0x62
#define serial_playback_control 0x6c

/*process of bit*/
#define SET_BIT(x, y) ((x) |= (y))
#define CLEAR_BIT(x, y) ((x) &= ~(y))
#define CHKBIT_SET(x, y) (((x) & (y)) == (y))
#define CHKBIT_CLR(x, y) (((x) & (y)) == 0)

/*==================================================================================*/

//==================================================================================
//						TW2815 initialize table description
//==================================================================================
//==================================================================================

//--------------------------		NTSC		------------------------------//
//=================================================================================

unsigned char tbl_ntsc_tw2815_common[] = {
    //												CH1			CH2			CH3			CH4
    0x00, 0x48, 0x20, 0xd0,        //...		0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
    0x06, 0xf0, 0x08, 0x80,        //...		0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
    0x80, 0x80, 0x80, 0x02,        //...		0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
    0x06, 0x00, 0x11              //...		0x0c~0x0e	0x1c~0x1e	0x2c~0x2e	0x3c~0x3e
};

unsigned char tbl_ntsc_tw2815_sfr1[] = {
    0x00, 0x00, 0x40, 0xc0,        //...		0x40~0x43//sync code(0x42)set to 0x1
    0x45, 0xa0, 0xd0, 0x2f,        //...		0x44~0x47
    0x64, 0x80, 0x80, 0x82,        //...		0x48~0x4b
    0x82, 0x00, 0x00, 0x00     //...		0x4c~0x4f
};

unsigned char tbl_ntsc_tw2815_sfr2[] = {
    0x00, 0x0f, 0x05, 0x00,        //...		0x50~0x53
    0x00, 0x80, 0x06, 0x00,        //...		0x54~0x57
    0x00, 0x00                   //...		0x58~0x59
};

//=================================================================================

//--------------------------		PAL		------------------------------//
//=================================================================================

unsigned char tbl_pal_tw2815_sfr1[] = {
    0x00, 0x00, 0x40, 0xc0,        //...		0x40~0x43
    0x45, 0xa0, 0xd0, 0x2f,        //...		0x44~0x47
    0x64, 0x80, 0x80, 0x82,        //...		0x48~0x4b
    0x82, 0x00, 0x00, 0x00     //...		0x4c~0x4f
};

unsigned char tbl_pal_tw2815_sfr2[] = {
    0x00, 0x0f, 0x05, 0x00,        //...		0x50~0x53
    0x00, 0x80, 0x06, 0x00,        //...		0x54~0x57
    0x00, 0x00                   //...		0x58~0x59
};

//=================================================================================
//=================================================================================
unsigned char tbl_tw2815_audio[] = {
    0x00, 0xff, //...		0x5a~0x5b
    0x8f, 0x00, 0x00, 0x00,        //...		0x5c~0x5f
    0x88, 0x88, 0xc0, 0x00,        //...		0x60~0x63
    0x10, 0x32, 0x54, 0x76,        //...		0x64~0x67
    0x98, 0xba, 0xdc, 0xfe,        //...		0x68~0x6b
    0x00, 0x00, 0x88, 0x88,        //...		0x6c~0x6f
    0x88, 0x11, 0x40, 0x88,        //...		0x70~0x73
    0x88, 0x00                   //...		0x74~0x75
};

//=================================================================================

/*==================================================================================*/

//==================================================================================
//						TW2864 initialize table description
//==================================================================================
//==================================================================================

//--------------------------		PUBLIC		------------------------------//
//=================================================================================

unsigned char tbl_tw2864_common_0x80[] = {
    0x3F, 0x02, 0x10, 0xCC, 0x00, 0x30, 0x44, 0x50, 0x42, 0x00, 0xD8, 0xBC, 0xB8, 0x44, 0x2A, 0x00
};

unsigned char tbl_tw2864_common_0x90[] = {
    0x00, 0x68, 0x4C, 0x30, 0x14, 0xA5, 0xE6, 0x05, 0x00, 0x28, 0x44, 0x00, 0x20, 0x90, 0x52, 0x77
};

unsigned char tbl_tw2864_common_0xa4[] = {
    0x1A, 0x1A, 0x1A, 0x1A, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x00
};

unsigned char tbl_tw2864_common_0xb0[] = {
    0x00
};

// modify
//0x00,0x00,0x00,0x00,0xFF,0xFF,0x05,0x01,0x0e,0x04,0x40,0x41
unsigned char tbl_tw2864_common_0xc4[] = {
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0x00, 0x0d, 0x08, 0x40, 0x00
};

unsigned char tbl_tw2864_common_0xd0[] = {
    0x22, 0x22, 0x02, 0x20, 0x64, 0xa8, 0xec, 0x31, 0x75, 0xb9, 0xfd, 0xC1, 0x1F, 0x00, 0x00, 0x80
};

unsigned char tbl_tw2864_common_0xe0[] = {
    0x00, 0xC0, 0xAA, 0xAA, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00, 0x00, 0x11, 0x00
};

unsigned char tbl_tw2864_common_0xf0[] = {
    0x83, 0xB5, 0x09, 0x78, 0x85, 0x00, 0x01, 0x20, 0x64, 0x11, 0x4a, 0x0F, 0xFF, 0x00, 0x00, 0x00
};

//==================================================================================

//--------------------------		PAL		------------------------------//
//=================================================================================
unsigned char tw2864_pal_channel[] =
{
    //0    1    2    3    4    5    6    7    8    9    A    B    C    D    E  F
    0x00, 0x00, 0x64, 0x11, 0x80, 0x80, 0x00, 0x12, 0x12, 0x20, 0x0c, 0xD0, 0x00, 0x00, 0x07, 0x7F
};

//==================================================================================

//--------------------------		NTSC		------------------------------//
//=================================================================================
unsigned char tw2864_ntsc_channel[] =
{
    //0    1    2    3    4    5    6    7    8    9    A    B    C    D    E  F
    0x00, 0x00, 0x64, 0x11, 0x80, 0x80, 0x00, 0x02, 0x12, 0xF0, 0x0c, 0xD0, 0x00, 0x00, 0x07, 0x7F
};

struct tw2815_w_reg
{
    unsigned char addr;
    unsigned char value;
};

struct tw2815_set_2d1
{
    unsigned char ch1;
    unsigned char ch2;
};

int          tw2864_video_mode_init(unsigned chip_addr, unsigned char video_mode, unsigned char ch);
void         set_2_d1(unsigned char chip_addr, unsigned char ch1, unsigned char ch2);

/*static int __init tw2864_device_video_init(void);*/
void         tw2864_setd1(unsigned char chip_addr);
void         tw2864_set4d1(unsigned char chip_addr);
unsigned int tw2864_chipID(unsigned int uiPortId);

#endif //__TW2864_H__
