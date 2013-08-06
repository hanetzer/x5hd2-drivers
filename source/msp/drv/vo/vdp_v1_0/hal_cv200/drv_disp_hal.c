
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_hal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_com.h"
#include "drv_disp_hal.h"
#include "drv_disp_osal.h"
#include "drv_disp_da.h"
#ifndef __DISP_PLATFORM_BOOT__
#include "drv_disp_ua.h"
#endif

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/*==========================================
     VDP / SYS / VDAC phy-address
*/
static volatile HI_U32 *s_pu32VdpSysRegAddr  = 0;
static volatile HI_U32 *s_pu32VdpPll0RegAddr = 0;
static volatile HI_U32 *s_pu32VdpPll1RegAddr = 0;

static HI_U32 s_u32VdpBaseAddr  = 0;
static HI_U32 s_u32VdacBaseAddr = 0;
/*==========================================
     HAL global parameters
*/
static HI_S32 s_DispIntfFlag = 0;
static DISP_CAPA_S s_stDispCapability[HI_DRV_DISPLAY_BUTT];
static DISP_CH_CFG_S s_stDispConfig[HI_DRV_DISPLAY_BUTT];

static HI_DRV_DISP_VERSION_S s_stVersion = {0};
static DISP_INTF_OPERATION_S s_stIntfOps = {0};

/*==========================================
    HAL module
*/
static DISP_LOCAL_INTF_S s_stHalIntf[HI_DRV_DISP_INTF_ID_MAX];
//static DISP_LOCAL_VDAC_S s_stHalVEnc[DISP_VENC_MAX];
static DISP_LOCAL_VDAC_S s_stHalVDac[HI_DISP_VDAC_MAX_NUMBER];
static DISP_LOCAL_WBC_S  s_stWBC[DISP_WBC_BUTT];

#define DISP_CLOCK_SOURCE_SD0  0
#define DISP_CLOCK_SOURCE_HD0  1
#define DISP_CLOCK_SOURCE_HD1  2

static DISP_FMT_CFG_S s_stDispFormatParam[] =
{
/* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
/* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 0 HI_UNF_ENC_FMT_1080P_60,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080}, {0,0,1920,1080}, {16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 1 HI_UNF_ENC_FMT_1080P_60,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 2 HI_UNF_ENC_FMT_1080P_50,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,     44, 5, 1, 0,  0,  0}, /* 1080P@50Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 3 HI_UNF_ENC_FMT_1080P_30
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,       1,   1,  1,    44,  5, 1, 0,  0,  0}, /* 1080P@30Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_30, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 3000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 4 HI_UNF_ENC_FMT_1080P_25,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,    44, 5, 1,  0,  0,  0}, /* 1080P@25Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_25, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2500, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 5 HI_UNF_ENC_FMT_1080P_24 @74.25MHz,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 638,       1,   1,  1,    44, 5, 1, 0,  0,  0}, /* 1080P@24Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_24, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2400, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 6 HI_UNF_ENC_FMT_1080i_60
 { {1,   0,   2,   540,  20,  2,  1920, 192, 88,  540, 21,  2,    44,  5, 908,   0,  0,  0}, /* 1080I@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080i_60, DISP_STEREO_NONE, HI_TRUE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 7 HI_UNF_ENC_FMT_1080i_50
 { {1,   0,   2,   540,  20,  2,  1920, 192,528,  540,  21,  2,   44, 5, 1128,  0,  0,  0}, /* 1080I@50Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080i_50, DISP_STEREO_NONE, HI_TRUE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 8 HI_UNF_ENC_FMT_720P_60
 { {1,   1,   2,   720,  25,  5,  1280, 260,110,      1,   1,  1,    40,  5,  1, 0,  0,  0}, /* 720P@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1280,720}, {0,0,1280,720},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 9 HI_UNF_ENC_FMT_720P_50
 { {1,   1,   2,   720,  25,  5,  1280, 260,440,     1,   1,  1,     40, 5,  1,  0,  0,  0},  /* 720P@50Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0, 
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,1280,720}, {0,0,1280,720},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

/* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 10 HI_UNF_ENC_FMT_576P_50,
 { {1,  1,   2,   576,   44,  5,   720, 132, 12,     1,   1,  1,     64, 5,  1,  0,  0,  0}, /* 576P@50Hz */
   DISP_CLOCK_SOURCE_SD0, 
   {0x14000000, 0x02002063}, 
   {HI_DRV_DISP_FMT_576P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,720,576} , {0,0,720,576} ,{4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
    /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
  /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 11 HI_UNF_ENC_FMT_480P_60,
 { {1,  1,   2,   480,   36,  9,   720, 122, 16,     1,   1,  1,     62, 6,  1,  0,  0,  0}, /* 480P@60Hz */
   DISP_CLOCK_SOURCE_SD0, 
   {0x14000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_480P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,720,480} , {0,0,720,480} , {4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },

  // 12 HI_UNF_ENC_FMT_PAL
 { {0,   0,   0,   288,  22,  2,  720, 132, 12,     288,  23,  2,    126, 3, 0, 0,  0,  0},/* 576I(PAL) */
   DISP_CLOCK_SOURCE_SD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_PAL,     DISP_STEREO_NONE, HI_TRUE,  {0,0,720,576} ,  {0,0,720,576} , {4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  //576I: HDMI输出要求hmid=300, 而YPbPr要求hmid=0, 
  //考虑一般用户不会使用HDMI输出576I，所以不支持HDMI_567I输出，选择hmid=0
  // 13 HI_UNF_ENC_FMT_NTSC
 { {0,   0,   0,   240,  18,  4,   720, 119, 19,     240,  19,  4,    124, 3,  0, 0, 0,  0},/* 480I(NTSC) */
   DISP_CLOCK_SOURCE_SD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED},
 },
  //480I: HDMI输出要求hmid=310, 而YPbPr要求hmid=0, 
  //考虑一般用户不会使用HDMI输出480I，所以不支持HDMI_480I输出，选择hmid=0

/* ============================================= */
  // TODO:
  // 14, LCD
 { {1,   1,   2,   600,  27,  1,   800, 216, 40,       1,   1,  1,    128, 4, 1, 0,  0,  0}, /* 800*600@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 15
 { {1,   1,   2,   768,  35,  3,  1024, 296, 24,      1,   1,  1,    136, 6, 1,  0,  0,  0}, /* 1024x768@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} , {4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 16
 { {1,   1,   2,  1024,  41,  1,  1280, 360, 48,      1,   1,  1,    112, 3, 1,  0,  0,  0}, /* 1280x1024@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 17
 { {1,   1,   2,   768,  27,  3,  1366, 356, 70,      1,   1,  1,    143, 3, 1,  0,  0,  0}, /* 1366x768@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 18
 { {1,   1,   2,   900,  23,  3,  1440, 112, 48,     1,   1,  1,    32, 6,   1, 0,  0,  0}, /* 1440x900@60Hz@88.75MHz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },

  // 19
 { {1,   1,   2,   900,  31,  3,  1440, 384, 80,    1,   1,  1,    152, 6,   1,  0,  0,  0}, /* 1440x900@60Hz@106.5MHz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 20
 { {1,   1,   2,   480,  35,  10,  640, 144, 16,       1,   1,  1,      96, 2,  1, 0,  0,  0}, /* 640*480@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} ,{0,0,720,480} , {4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  // 21
 { {1,   1,   2,   1200, 49,  1, 1600, 496, 64,       1,   1,  1,     192, 3, 1, 0,  0,  0}, /* 1600*12000@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },

  //22 HI_UNF_ENC_FMT_PAL_TEST
 { {0,   0,   2,   288,  22,  2,  1440, 132, 12,    288,  23,  2,    126, 3,  0, 0,  0,  0},/* 576I(PAL) */
   DISP_CLOCK_SOURCE_HD0, 
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },

/* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 23 HI_UNF_ENC_FMT_1080P_24_FP @74.25MHz,
 { {1,   0,   2,  1080,  41,  4,  1920, 192, 638,   1080,  41,  4,   44, 5,   1, 0,  0,  0}, /* 1080P@24Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_24_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2400, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 24 HI_UNF_ENC_FMT_720P_60_FP
 { {1,   0,   2,   720,  25,  5,  1280, 260,110,    720,  25,  5,    40,  5,  1,  0,  0,  0}, /* 720P@60Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_60_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1280,720},  {0,0,1280,720},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 25 HI_UNF_ENC_FMT_720P_50_FP
 /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
 { {1,   0,   2,   720,  25,  5,  1280, 260,440,    720,  25,   5,   40,  5,  1, 0,  0,  0},  /* 720P@50Hz */
   DISP_CLOCK_SOURCE_HD0, 
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_50_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1280,720},  {0,0,1280,720},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 26 HI_UNF_ENC_FMT_PAL for HDMI
 { {0,   0,   0,   288,  22,  2,  720*2, 132*2, 12*2,  288,  23,  2,    126, 3, 300, 0,  0,  0},/* 576I(PAL) */
   DISP_CLOCK_SOURCE_SD0, 
   {0x14000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_PAL,     DISP_STEREO_NONE, HI_TRUE,  {0,0,1440,576} , {0,0,720,576} ,{4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
  //576I: HDMI输出要求hmid=300, 而YPbPr要求hmid=0, 
  //考虑一般用户不会使用HDMI输出576I，所以不支持HDMI_567I输出，选择hmid=0
  // 27 HI_UNF_ENC_FMT_NTSC for HDMI
 { {0,   0,   0,   240,  18,  4,   720*2, 119*2, 19*2,  240,  19,  4,    124, 3,  310, 0, 0,  0},/* 480I(NTSC) */
   DISP_CLOCK_SOURCE_SD0, 
   {0x14000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,1440,480} ,  {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED},
 },
};



HI_S32 DispHalGetEnFmtIndex(HI_DRV_DISP_FMT_E eFmt)
{
    switch (eFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60:
            return 1;
        case HI_DRV_DISP_FMT_1080P_50:
            return 2;
        case HI_DRV_DISP_FMT_1080P_30:
            return 3;
        case HI_DRV_DISP_FMT_1080P_25:
            return 4;
        case HI_DRV_DISP_FMT_1080P_24:
            return 5;
        case HI_DRV_DISP_FMT_1080i_60:
            return 6;
        case HI_DRV_DISP_FMT_1080i_50:
            return 7;
        case HI_DRV_DISP_FMT_720P_60:
            return 8;
        case HI_DRV_DISP_FMT_720P_50:
            return 9;
        case HI_DRV_DISP_FMT_576P_50:
            return 10;
        case HI_DRV_DISP_FMT_480P_60:
            return 11;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            return 12;


        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_60:
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
            return 13;

        case HI_DRV_DISP_FMT_1080P_24_FP:
            return 23;
        case HI_DRV_DISP_FMT_720P_60_FP:
            return 24;
        case HI_DRV_DISP_FMT_720P_50_FP:
            return 25;
        case HI_DRV_DISP_FMT_1440x576i_50:
            return 26;
        case HI_DRV_DISP_FMT_1440x480i_60:
            return 27;

        default :
            return 0;
    }
    
    return 0;
}


/********************************************/
/* usual */
HI_S32 DISP_HAL_GetFmtAspectRatio(HI_DRV_DISP_FMT_E eFmt, HI_U32 *pH, HI_U32 *pV)
{
    HI_S32 index;

    index = DispHalGetEnFmtIndex(eFmt);

    *pH = s_stDispFormatParam[index].stInfo.stAR.u8ARw;
    *pV = s_stDispFormatParam[index].stInfo.stAR.u8ARh;

    return HI_SUCCESS;
}

HI_S32 DISP_HAL_GetFmtColorSpace(HI_DRV_DISP_FMT_E eFmt, HI_DRV_COLOR_SPACE_E  *penColorSpace)
{
    HI_S32 index;

    index = DispHalGetEnFmtIndex(eFmt);

    *penColorSpace = s_stDispFormatParam[index].stInfo.enColorSpace;

    return HI_SUCCESS;
}

HI_S32 DISP_HAL_GetEncFmtPara(HI_DRV_DISP_FMT_E eFmt, DISP_HAL_ENCFMT_PARAM_S *pstFmtPara)
{
    *pstFmtPara = s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)].stInfo;
    return HI_SUCCESS;
}

VDP_CBM_MIX_E DISP_HAL_GetMixID(HI_DRV_DISPLAY_E eChn)
{
    switch(eChn)
    {
        case HI_DRV_DISPLAY_0:
            return VDP_CBM_MIX1;
        case HI_DRV_DISPLAY_1:
            return VDP_CBM_MIX0;
        default :
            return VDP_CBM_MIX0;
    }
}

HI_U32 DISP_HAL_GetChID(HI_DRV_DISPLAY_E eChn)
{
    switch(eChn)
    {
        case HI_DRV_DISPLAY_0:
            return 1;
        case HI_DRV_DISPLAY_1:
            return 0;
        default :
            return 0;
    }
}


/*
HI_S32 DISP_HAL_GetPll(HI_DRV_DISP_FMT_E eFmt, HI_U32 *pLow, HI_U32 *pHigh)
{
    *pLow  = s_u32PllConfig[DispHalGetEnFmtIndex(eFmt)][0];
    *pHigh = s_u32PllConfig[DispHalGetEnFmtIndex(eFmt)][1];
    return HI_SUCCESS;
}
*/


/********************************************/
HI_VOID DISP_HAL_ResetDispState(HI_VOID);
HI_VOID DISP_HAL_ResetIntfState(HI_VOID);
//HI_VOID DISP_HAL_ResetVEncState(HI_VOID);
HI_VOID DISP_HAL_ResetVDacState(HI_VOID);
HI_VOID InitWbcState(HI_VOID);

HI_S32 DISP_HAL_InitVDPState(HI_VOID)
{
    // s1 get sysctrl address
    s_pu32VdpSysRegAddr = (HI_U32 *)DISP_IOADDRESS(DISP_X5HD2_MPW_SYSCTRL_BASE);

    // s2 get pll address
    s_pu32VdpPll0RegAddr = (HI_U32 *)DISP_IOADDRESS(DISP_X5HD2_MPW_PLL0_BASE);
    s_pu32VdpPll1RegAddr = (HI_U32 *)DISP_IOADDRESS(DISP_X5HD2_MPW_PLL1_BASE);

    // s3 get vdac address
    s_u32VdacBaseAddr = (HI_U32)DISP_IOADDRESS(DISP_X5HD2_MPW_VDAC_BASE);

    // s4 set virtual addresss to hal
    VDP_DRIVER_SetVirtualAddr(s_u32VdpBaseAddr);
    VDAC_DRIVER_SetVirtualAddr(s_u32VdacBaseAddr);
    VDPSYSCTRL_DRIVER_SetVirtualAddr((HI_U32)s_pu32VdpSysRegAddr);

    if (!s_pu32VdpSysRegAddr || !s_pu32VdpPll0RegAddr || !s_pu32VdpPll1RegAddr || !s_u32VdacBaseAddr)
    {
        return HI_FAILURE;
    }

    // s5 reset module
    
    // s5.1 reset disp
    DISP_HAL_ResetDispState();

    // s5.2 reset intf
    DISP_HAL_ResetIntfState();

    // s5.3 reset venc
    //DISP_HAL_ResetVEncState();

    // s5.4 reset vdac
    DISP_HAL_ResetVDacState();
    
    // s5.5 reset wbc
    InitWbcState();

    return HI_SUCCESS;
}


HI_S32 PF_ResetVdpHardware(HI_VOID)
{
    HI_U32 v;
    // init sysreg
    v = s_pu32VdpSysRegAddr[0];

    s_pu32VdpSysRegAddr[0] = 0;

    //s_pu32VdpSysRegAddr[0] = v & (~0x40000000); 
    s_pu32VdpSysRegAddr[0] = DISP_X5HD2_MPW_SYSCTRL_RESET_VALUE | DISP_X5HD2_MPW_SYSCTRL_RESET_BIT; 

    DISP_MSLEEP(5);
    
    s_pu32VdpSysRegAddr[0] = DISP_X5HD2_MPW_SYSCTRL_RESET_VALUE; 

    // init pll
/*
    s_pu32VdpPll0RegAddr[0] = DISP_X5HD2_MPW_PLL0_RESET_VALUE_A;
    s_pu32VdpPll0RegAddr[1] = DISP_X5HD2_MPW_PLL0_RESET_VALUE_B;
    
    s_pu32VdpPll1RegAddr[0] = DISP_X5HD2_MPW_PLL1_RESET_VALUE_A;
    s_pu32VdpPll1RegAddr[1] = DISP_X5HD2_MPW_PLL1_RESET_VALUE_B;
*/
    // init vdp
    VDP_DRIVER_Initial();

    // init vdac
    //VDAC_DRIVER_Initial();
    VDP_VDAC_Reset();

    return HI_SUCCESS;
}

/********************************************/
/* Display config */
#define DISP_HAL_FUNCTION_START_HAERE
HI_VOID DISP_HAL_ResetDispState(HI_VOID)
{
    memset(&s_stDispCapability[0], 0, sizeof(DISP_CAPA_S) * HI_DRV_DISPLAY_BUTT);

    s_stDispCapability[HI_DRV_DISPLAY_0].bSupport = HI_TRUE;
    s_stDispCapability[HI_DRV_DISPLAY_0].bHD      = HI_TRUE;
    s_stDispCapability[HI_DRV_DISPLAY_0].bWbc    = HI_FALSE;

    s_stDispCapability[HI_DRV_DISPLAY_1].bSupport = HI_TRUE;
    s_stDispCapability[HI_DRV_DISPLAY_1].bHD      = HI_TRUE;
    s_stDispCapability[HI_DRV_DISPLAY_1].bWbc     = HI_TRUE;

    s_stDispCapability[HI_DRV_DISPLAY_2].bSupport = HI_FALSE;


    memset(&s_stDispConfig[0], 0, sizeof(DISP_CH_CFG_S) * HI_DRV_DISPLAY_BUTT);


    return;
}

HI_BOOL PF_TestChnSupport(HI_DRV_DISPLAY_E eChn)
{

    return s_stDispCapability[eChn].bSupport;
}


HI_BOOL PF_TestChnSupportHD(HI_DRV_DISPLAY_E eChn)
{

    return s_stDispCapability[eChn].bHD;
}

HI_BOOL PF_TestChnSupportCast(HI_DRV_DISPLAY_E eChn)
{

    return s_stDispCapability[eChn].bWbc;
}

HI_BOOL PF_TestIntfSupport(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_INTF_ID_E eIntf)
{
    if (HI_DRV_DISPLAY_0 == eChn)
    {
        if (  (HI_DRV_DISP_INTF_SVIDEO0 == eIntf)
            ||(HI_DRV_DISP_INTF_CVBS0   == eIntf)
            ||(HI_DRV_DISP_INTF_VGA0    == eIntf)
            )
        {
            return HI_TRUE;
        }
    }

    if (HI_DRV_DISPLAY_1 == eChn)
    {
        if (  (HI_DRV_DISP_INTF_YPBPR0 == eIntf)
            ||(HI_DRV_DISP_INTF_HDMI0  == eIntf)
            ||(HI_DRV_DISP_INTF_VGA0   == eIntf)
            ||(HI_DRV_DISP_INTF_LCD0   == eIntf)
            )
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_BOOL PF_TestChnEncFmt(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_FMT_E eFmt)
{
    if( (eChn == HI_DRV_DISPLAY_1) && (eFmt <= HI_DRV_DISP_FMT_861D_640X480_60))
    {
        return HI_TRUE;
    }

    if(  (eChn == HI_DRV_DISPLAY_0)
       &&(eFmt >= HI_DRV_DISP_FMT_PAL)
       &&(eFmt <= HI_DRV_DISP_FMT_SECAM_COS)
       )
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_BOOL PF_TestChnAttach(HI_DRV_DISPLAY_E enM, HI_DRV_DISPLAY_E enS)
{
    if( (enM != HI_DRV_DISPLAY_1) || (enS != HI_DRV_DISPLAY_0))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_S32 DISP_HAL_SetPll(HI_DRV_DISP_FMT_E eFmt)
{
    s_pu32VdpPll0RegAddr[0] = s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)].u32Pll[0];
    s_pu32VdpPll0RegAddr[1] = s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)].u32Pll[1];

    return HI_SUCCESS;
}

#define DISP_HPLL_BYPASS            0x04000000ul
#define DISP_HPLL_DSM_PD            0x02000000ul
#define DISP_HPLL_VDAC_PD           0x01000000ul
#define DISP_HPLL_FOUT_POST_DIV_PD  0x00800000ul
#define DISP_HPLL_FOUT_4_PHASE_PD   0x00400000ul
#define DISP_HPLL_FOUT_VCO_PD       0x00200000ul
#define DISP_HPLL_POWERDOWN         0x00100000ul

#define DISP_HPLL_0_MASK            0xFFFFFFFFul
#define DISP_HPLL_1_MASK            0x0003FFFFul

HI_S32 DISP_HAL_SetPll2(HI_DRV_DISP_FMT_E eFmt)
{
    HI_U32 uLow, uHigh;
    
    uLow  = s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)].u32Pll[0] & DISP_HPLL_0_MASK;
    uHigh = s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)].u32Pll[1] & DISP_HPLL_1_MASK;

    // bypass and power down
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] & (~DISP_HPLL_BYPASS);
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] | DISP_HPLL_FOUT_VCO_PD;
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] | DISP_HPLL_FOUT_POST_DIV_PD;
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] | DISP_HPLL_FOUT_4_PHASE_PD;

    // config div para
    s_pu32VdpPll0RegAddr[0] = s_pu32VdpPll0RegAddr[0] & (~DISP_HPLL_0_MASK);
    s_pu32VdpPll0RegAddr[0] = s_pu32VdpPll0RegAddr[0] | uLow;
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] & (~DISP_HPLL_1_MASK);
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] | uHigh;

    // power up
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] & (~DISP_HPLL_FOUT_VCO_PD);
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] & (~DISP_HPLL_FOUT_POST_DIV_PD);
    s_pu32VdpPll0RegAddr[1] = s_pu32VdpPll0RegAddr[1] & (~DISP_HPLL_FOUT_4_PHASE_PD);

    return HI_SUCCESS;
}

HI_S32 PF_ResetChn(HI_U32 u32DispId, HI_U32 bIntMode)
{
    VDP_DISP_CLIP_S stClipData;
    //HI_U32 u32DispId = DISP_HAL_GetChID(eChn);

    // s1 set dhd
    VDP_DHD_Reset(u32DispId);

    VDP_DISP_SetVtThdMode(u32DispId, 1, bIntMode);
    VDP_DISP_SetVtThdMode(u32DispId, 2, bIntMode);
    VDP_DISP_SetVtThdMode(u32DispId, 3, bIntMode);

    VDP_DISP_SetCscEnable(u32DispId, 0);

    // s2 set clip

    stClipData.bClipEn = 1;
    stClipData.u32ClipLow_y  = 0;
    stClipData.u32ClipLow_cb = 0;
    stClipData.u32ClipLow_cr = 0;

    stClipData.u32ClipHigh_y  = 1023;  
    stClipData.u32ClipHigh_cb = 1023;
    stClipData.u32ClipHigh_cr = 1023;

    VDP_DISP_SetClipCoef(u32DispId, VDP_DISP_INTF_LCD, stClipData);
    VDP_DISP_SetClipCoef(u32DispId, VDP_DISP_INTF_BT1120, stClipData);
    VDP_DISP_SetClipCoef(u32DispId, VDP_DISP_INTF_HDMI, stClipData);
    VDP_DISP_SetClipCoef(u32DispId, VDP_DISP_INTF_VGA, stClipData);

    stClipData.u32ClipLow_y  = 64;
    stClipData.u32ClipLow_cb = 64;
    stClipData.u32ClipLow_cr = 64;
    VDP_DISP_SetClipCoef(u32DispId, VDP_DISP_INTF_DATE, stClipData);

    VDP_DISP_SetRegUp(u32DispId);

    return HI_SUCCESS;
}

HI_S32 PF_ConfigChn(HI_DRV_DISPLAY_E eChn, DISP_CH_CFG_S *pstCfg)
{
    //HI_U32 u32DispId = DISP_HAL_GetChID(eChn);

    return HI_SUCCESS;
}


#define DISP_VTTHD_VIDEO_OFFSET 40
#define DISP_VTTHD_GFX_OFFSET 80

#define DISP_VTTHD_DISP0_TO_DISP1 48

#define DISP_0_CLOCK_SOURCE_MASK_SHIFT  12
#define DISP_0_CLOCK_SOURCE_MASK  0x00003000UL

#define DISP_1_CLOCK_SOURCE_MASK_SHIFT  16
#define DISP_1_CLOCK_SOURCE_MASK  0x00030000UL

#define DISP_1_CLOCK_SELECT_MASK_SHIFT  28
#define DISP_1_CLOCK_SELECT_MASK  0x30000000UL

#define DISP_1_CLOCK_SELECT_300MHZ    0
#define DISP_1_CLOCK_SELECT_400MHZ    1
#define DISP_1_CLOCK_SELECT_345P6MHZ  2
#define DISP_1_CLOCK_SELECT_200MHZ    3

HI_S32 DiSP_HAL_SelectPll(HI_DRV_DISPLAY_E eChn, HI_U32 uPllIndex)
{
    HI_U32 v;
    
    if (HI_DRV_DISPLAY_1 == eChn)
    {
        v = *s_pu32VdpSysRegAddr;
        v = v & (~DISP_1_CLOCK_SOURCE_MASK);
        v = v | ( DISP_1_CLOCK_SOURCE_MASK & (uPllIndex << DISP_1_CLOCK_SOURCE_MASK_SHIFT));

        if (DISP_CLOCK_SOURCE_SD0 == uPllIndex)
        {
            v = v & (~DISP_1_CLOCK_SELECT_MASK);
            v = v | ( DISP_1_CLOCK_SELECT_MASK & (DISP_1_CLOCK_SELECT_200MHZ << DISP_1_CLOCK_SELECT_MASK_SHIFT));
        }
        else
        {
            v = v & (~DISP_1_CLOCK_SELECT_MASK);
            v = v | ( DISP_1_CLOCK_SELECT_MASK & (DISP_1_CLOCK_SELECT_300MHZ << DISP_1_CLOCK_SELECT_MASK_SHIFT));
        }

        *s_pu32VdpSysRegAddr = v;
    }
    else
    {
    }

    return HI_SUCCESS;
}

HI_S32 PF_SetChnFmt(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_FMT_E eFmt)
{
    VDP_DISP_RECT_S DispRect;
    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    DISP_FMT_CFG_S *pstCfg;
    HI_S32 thd;

    pstCfg = &s_stDispFormatParam[DispHalGetEnFmtIndex(eFmt)];

    //DISP_PRINT(">>>>>>>>>>>>>>>>>>index=%d, bact=%d\n", DispHalGetEnFmtIndex(eFmt), pstCfg->stTiming.u32Bvact);

    //PF_ResetChn(eChn);
    // set frame packing
    if(pstCfg->stInfo.eDispMode == DISP_STEREO_FPK)
    {
        PF_ResetChn(u32DispId, DHD_VTXTHD_FRAME_MODE);
        VDP_DISP_SetFramePackingEn(u32DispId, 1);
    }
    else
    {
        PF_ResetChn(u32DispId, DHD_VTXTHD_FIELD_MODE);
        VDP_DISP_SetFramePackingEn(u32DispId, 0);
    }

    // set dhd
    /*
      timing is like that:

      ||<--------- FRRAME 0--------->||<--------- FRRAME 1--------->||
      -----------------------------------------------------------------
      || VBB0 |     VACT0     | VFB0 || VBB01 |     VACT1    | VFB1 ||  
      -----------------------------------------------------------------
                              ^
                             /|\
                              |
                NOTICE: The 'thd' is start here, NOT between VFB0 and VBB1.
                        That means here thd is '0'.
    */
    /*
      ||<--------- FRRAME 0--------->||<--------- FRRAME 1--------->||
      -----------------------------------------------------------------
      || VBB0 |     VACT0     | VFB0 || VBB01 |     VACT1    | VFB1 ||  
      -----------------------------------------------------------------
                              |---------------|<----->|
                                                  ^   Pos1
                                                 /|\
                                                  |
                                         Here equal to 'DISP_VTTHD_VIDEO_OFFSET'
                                         And Pos1 is thd1
    */
    VDP_DISP_SetTiming(u32DispId, &(pstCfg->stTiming));
    thd = pstCfg->stTiming.u32Vfb + pstCfg->stTiming.u32Vbb + DISP_VTTHD_VIDEO_OFFSET;

    if (u32DispId == 1)
    {
        thd = thd + DISP_VTTHD_DISP0_TO_DISP1;
    }
    VDP_DISP_SetVtThd(u32DispId, 1, (HI_U32)thd);

    /*
      ||<--------- FRRAME 0--------->||<--------- FRRAME 1--------->||
      -----------------------------------------------------------------
      || VBB0 |     VACT0     | VFB0 || VBB01 |     VACT1    | VFB1 ||  
      -----------------------------------------------------------------
                                |<----------->|
                               Pos2    ^   
                                      /|\
                                       |
                                 Here equal to 'DISP_VTTHD_GFX_OFFSET'
                                 Pos2 is thd2. 
                                 If DISP_VTTHD_GFX_OFFSET is bigger tha (VFB0+VBB1),
                                 Pos2 is in VACT0.
    */

    thd = pstCfg->stTiming.u32Vbb + pstCfg->stTiming.u32Vfb - DISP_VTTHD_GFX_OFFSET;
    if (thd < 0)
    {
        /* Pos2 is in VACT0 */
        thd = thd + pstCfg->stTiming.u32Vbb + pstCfg->stTiming.u32Vfb +  pstCfg->stTiming.u32Vact;
    }

    if (u32DispId == 1)
    {
        thd = thd + DISP_VTTHD_DISP0_TO_DISP1;
    }

    VDP_DISP_SetVtThd(u32DispId, 2, (HI_U32)thd);

    // set pll 
    //DISP_HAL_SetPll(eFmt);


    // set vp
    DISP_MEMSET(&DispRect, 0, sizeof(VDP_DISP_RECT_S));
    DispRect.u32DXL  = pstCfg->stInfo.stOrgRect.s32Width;
    DispRect.u32DYL  = pstCfg->stInfo.stOrgRect.s32Height;
    DispRect.u32IWth = pstCfg->stInfo.stOrgRect.s32Width;
    DispRect.u32IHgt = pstCfg->stInfo.stOrgRect.s32Height;
    DispRect.u32OWth = pstCfg->stInfo.stOrgRect.s32Width;
    DispRect.u32OHgt = pstCfg->stInfo.stOrgRect.s32Height;
    DispRect.u32VXL  = pstCfg->stInfo.stOrgRect.s32Width;
    DispRect.u32VYL  = pstCfg->stInfo.stOrgRect.s32Height;

    if (HI_DRV_DISPLAY_1 == eChn)
    {

        // selec clk
        DiSP_HAL_SelectPll(eChn, pstCfg->u32PllIndex);

        // set pll 
        DISP_HAL_SetPll(eFmt);

//printk("========================== dispmode = %d\n", pstCfg->stInfo.eDispMode);
        // set frame packing
        switch(pstCfg->stInfo.eDispMode)
        {
            case DISP_STEREO_FPK:
                VDP_VP_SetDispMode(VDP_LAYER_VP0, VDP_DISP_MODE_FP);
                break;
            case DISP_STEREO_SBS_HALF:
                VDP_VP_SetDispMode(VDP_LAYER_VP0, VDP_DISP_MODE_SBS);
                break;
            case DISP_STEREO_TAB:
                VDP_VP_SetDispMode(VDP_LAYER_VP0, VDP_DISP_MODE_TAB);
                break;
            default:
                VDP_VP_SetDispMode(VDP_LAYER_VP0, VDP_DISP_MODE_2D);
                break;
        }

        VDP_VP_SetLayerReso(VDP_LAYER_VP0, DispRect);

        VDP_VP_SetRegUp(VDP_LAYER_VP0);
    }
    else if (HI_DRV_DISPLAY_0 == eChn)
    {
        VDP_VP_SetLayerReso(VDP_LAYER_VP1, DispRect);

        VDP_VP_SetRegUp(VDP_LAYER_VP1);
    }

    VDP_DISP_SetRegUp(u32DispId);

    return HI_SUCCESS;
}

HI_S32 PF_SetChnTiming(HI_DRV_DISPLAY_E eChn, HI_DRV_DISP_TIMING_S *pstTiming)
{
//    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    return HI_SUCCESS;
}

HI_S32 PF_SetChnPixFmt(HI_DRV_DISPLAY_E eChn, HI_DRV_PIX_FORMAT_E ePix)
{
//    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    return HI_SUCCESS;
}

HI_S32 PF_SetChnBgColor(HI_DRV_DISPLAY_E eChn, HI_DRV_COLOR_SPACE_E enCS, HI_DRV_DISP_COLOR_S *pstBGC)
{
    VDP_BKG_S stBkg;
    ALG_COLOR_S stAlgC;
    DISP_DA_FUNCTION_S *pstDA;

    pstDA = DISP_DA_GetFunction();

    stAlgC.u8Red   = pstBGC->u8Red;
    stAlgC.u8Green = pstBGC->u8Green;
    stAlgC.u8Blue  = pstBGC->u8Blue;

    pstDA->PFCscRgb2Yuv(&stAlgC, &stAlgC);

    // TODO:
    stBkg.bBkType = 0;
    stBkg.u32BkgA = 0;
    stBkg.u32BkgY = ((HI_U32)stAlgC.u8Y)  << 2;
    stBkg.u32BkgU = ((HI_U32)stAlgC.u8Cb) << 2;
    stBkg.u32BkgV = ((HI_U32)stAlgC.u8Cr) << 2;
/*
    DISP_PRINT(">>>>>>>>SET BGC R=%d, G=%d, B=%d, Y=%d, PB=%d, PR=%d\n",
               stAlgC.u8Red, stAlgC.u8Green, stAlgC.u8Blue,
               stAlgC.u8Y, stAlgC.u8Cb, stAlgC.u8Cr);
*/
    if (HI_DRV_DISPLAY_0 == eChn)
    {
        VDP_CBM_SetMixerBkg(VDP_CBM_MIX0, stBkg);
    }
    else
    {
        VDP_CBM_SetMixerBkg(VDP_CBM_MIX1, stBkg);
        VDP_CBM_SetMixerBkg(VDP_CBM_MIXV0, stBkg);
    }

    

    return HI_SUCCESS;
}


HI_S32 PF_SetChnColor(HI_DRV_DISPLAY_E eChn, DISP_HAL_COLOR_S *pstColor)
{
#if 0
    ALG_CSC_DRV_PARA_S stIn;
    ALG_CSC_RTL_PARA_S stOut;
    VDP_CSC_DC_COEF_S stCscCoef;
    VDP_CSC_COEF_S stCscCoef2;
    DISP_DA_FUNCTION_S *pstDA;
    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);

    pstDA = DISP_DA_GetFunction();
    if (!pstDA)
    {
        return HI_FAILURE;
    }

    stIn.eInputCS  = HI_DRV_CS_BT709_YUV_LIMITED;
    stIn.eOutputCS = HI_DRV_CS_BT709_YUV_LIMITED;
    stIn.bIsBGRIn = HI_FALSE;

    stIn.u32Bright  = pstColor->u32Bright;
    stIn.u32Contrst = pstColor->u32Contrst;
    stIn.u32Hue     = pstColor->u32Hue;
    stIn.u32Satur   = pstColor->u32Satur;
    stIn.u32Kr = pstColor->u32Kr;
    stIn.u32Kg = pstColor->u32Kg;
    stIn.u32Kb = pstColor->u32Kb;


    DISP_PRINT(">>>>>>>>PF_SetChnColor i=%d, o=%d, B=%d, C=%d, H=%d, S=%d,KR=%d,KG=%d, KB=%d\n",
               pstColor->enInputCS, pstColor->enOutputCS, 
               pstColor->u32Bright,
               pstColor->u32Contrst,
               pstColor->u32Hue,
               pstColor->u32Satur,
               pstColor->u32Kr,
               pstColor->u32Kg,
               pstColor->u32Kb);


    pstDA->pfCalcCscCoef(&stIn, &stOut);


    DISP_PRINT(">>>>>>>>PF_SetChnColor D1=%d, D2=%d, C00=%d, C11=%d, C22=%d\n",
               stOut.s32CscDcIn_1, stOut.s32CscDcIn_2, 
               stOut.s32CscCoef_00,
               stOut.s32CscCoef_11,
               stOut.s32CscCoef_22);


    stCscCoef.csc_in_dc0 = stOut.s32CscDcIn_0;
    stCscCoef.csc_in_dc1 = stOut.s32CscDcIn_1;
    stCscCoef.csc_in_dc2 = stOut.s32CscDcIn_2;

    stCscCoef.csc_out_dc0 = stOut.s32CscDcOut_0;
    stCscCoef.csc_out_dc1 = stOut.s32CscDcOut_1;
    stCscCoef.csc_out_dc2 = stOut.s32CscDcOut_2;
    VDP_DISP_SetCscDcCoef(u32DispId, stCscCoef);

    stCscCoef2.csc_coef00 = stOut.s32CscCoef_00;
    stCscCoef2.csc_coef01 = stOut.s32CscCoef_01;
    stCscCoef2.csc_coef02 = stOut.s32CscCoef_02;

    stCscCoef2.csc_coef10 = stOut.s32CscCoef_10;
    stCscCoef2.csc_coef11 = stOut.s32CscCoef_11;
    stCscCoef2.csc_coef12 = stOut.s32CscCoef_12;

    stCscCoef2.csc_coef20 = stOut.s32CscCoef_20;
    stCscCoef2.csc_coef21 = stOut.s32CscCoef_21;
    stCscCoef2.csc_coef22 = stOut.s32CscCoef_22;

    VDP_DISP_SetCscCoef(u32DispId, stCscCoef2);
    
    VDP_DISP_SetCscEnable(u32DispId, 1);

    VDP_DISP_SetRegUp(u32DispId);
#endif

    return HI_SUCCESS;
}

HI_S32 PF_SetDispSignal(HI_DRV_DISPLAY_E eChn, HI_U32 index, HI_DRV_DISP_VDAC_SIGNAL_E eSignal)
{
//    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    
    return HI_SUCCESS;
}

#define DISP_0_CLOCK_DIV_MASK_SHIFT  14
#define DISP_0_CLOCK_DIV_MASK  0x0000C000UL

#define DISP_0_CLOCK_DIV_2  0
#define DISP_0_CLOCK_DIV_4  1
#define DISP_0_CLOCK_DIV_1  2

HI_S32 PF_SetChnEnable(HI_DRV_DISPLAY_E eChn, HI_BOOL bEnalbe)
{
    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    HI_U32 v;

    VDP_DISP_SetIntfEnable(u32DispId, bEnalbe);

    VDP_DISP_SetRegUp(u32DispId);

    // switch sd clock divide parameters otherwis DISP0 maybe die.
    if ( (eChn == HI_DRV_DISPLAY_0) && bEnalbe )
    {
        v = *s_pu32VdpSysRegAddr;
        v = v & (~DISP_0_CLOCK_SOURCE_MASK);
        v = v | ( DISP_0_CLOCK_SOURCE_MASK & (DISP_CLOCK_SOURCE_HD0 << DISP_0_CLOCK_SOURCE_MASK_SHIFT));
        *s_pu32VdpSysRegAddr = v;

        DISP_UDELAY(1000);

        v = *s_pu32VdpSysRegAddr;
        v = v & (~DISP_0_CLOCK_SOURCE_MASK);
        v = v | ( DISP_0_CLOCK_SOURCE_MASK & (DISP_CLOCK_SOURCE_SD0 << DISP_0_CLOCK_SOURCE_MASK_SHIFT));
        *s_pu32VdpSysRegAddr = v;
    }
#if 0
    else if ( (eChn == HI_DRV_DISPLAY_1) && bEnalbe )
    {
        v = *s_pu32VdpSysRegAddr;
        v = v & (~DISP_1_CLOCK_SOURCE_MASK);
        v = v | ( DISP_1_CLOCK_SOURCE_MASK & (DISP_CLOCK_SOURCE_HD0 << DISP_1_CLOCK_SOURCE_MASK_SHIFT));
        *s_pu32VdpSysRegAddr = v;

        v = *s_pu32VdpSysRegAddr;
        v = v & (~DISP_1_CLOCK_SOURCE_MASK);
        v = v | ( DISP_1_CLOCK_SOURCE_MASK & (DISP_CLOCK_SOURCE_SD0 << DISP_1_CLOCK_SOURCE_MASK_SHIFT));
        *s_pu32VdpSysRegAddr = v;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 PF_GetChnEnable(HI_DRV_DISPLAY_E eChn, HI_BOOL *pbEnalbe)
{
    HI_U32 u32DispId = DISP_HAL_GetChID(eChn);
    HI_U32 bTrue;

    VDP_DISP_GetIntfEnable(u32DispId, &bTrue);

    *pbEnalbe = bTrue ? HI_TRUE : HI_FALSE;
    return HI_SUCCESS;
}









/********************************************/
/* capability */

/********************************************/
/* interrupt */
HI_S32 PF_SetIntEnable(HI_U32 u32Int, HI_BOOL bEnable)
{
    //printk("PF_SetIntEnable   int=%d, en=%d\n", u32Int, bEnable);
    if (bEnable)
    {
        VDP_DISP_SetIntMask(u32Int);
    }
    else
    {
        VDP_DISP_SetIntDisable(u32Int);
    }
    return HI_SUCCESS;
}

HI_S32 PF_GetIntSetting(HI_U32 *pu32IntSetting)
{
    VDP_DISP_GetIntMask(pu32IntSetting);
    return HI_SUCCESS;
}

HI_S32 PF_GetMaskedIntState(HI_U32 *pu32State)
{
    *pu32State = VDP_DISP_GetMaskIntSta((HI_U32)DISP_INTERRUPT_ALL);
    return HI_SUCCESS;
}

HI_S32 PF_GetUnmaskedIntState(HI_U32 *pu32State)
{

    *pu32State = VDP_DISP_GetIntSta((HI_U32)DISP_INTERRUPT_ALL);
    return HI_SUCCESS;
}


HI_S32 PF_CleanIntState(HI_U32 u32State)
{
    VDP_DISP_ClearIntSta(u32State);
    return HI_SUCCESS;
}

HI_U32 FP_GetChnIntState(HI_DRV_DISPLAY_E enDisp, HI_U32 u32IntState)
{
    switch (enDisp)
    {
        case HI_DRV_DISPLAY_1 : 

            return (HI_U32)(u32IntState & 0x0ful);

        case HI_DRV_DISPLAY_0 : 

            return (HI_U32)(u32IntState & 0xf0ul);

        default:
            return (HI_U32)0;
    }
}

HI_U32 FP_GetChnBottomFlag(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbBtm, HI_U32 *pu32Vcnt)
{
    HI_U32 u32DispId = DISP_HAL_GetChID(enDisp);
    
    if (pbBtm && pu32Vcnt)
    {
        VDP_DISP_GetVactState(u32DispId, pbBtm, pu32Vcnt);
    }

    return 0;
}






/********************************************/
/* venc manager */
#define DISP_HAL_VENC_FUNCTION_START_HAERE

VDP_DISP_INTF_E DISP_HAL_GetHalIntfIdForVenc(DISP_VENC_E eVenc)
{
    switch (eVenc)
    {
        case DISP_VENC_HDATE0:
            return VDP_DISP_INTF_HDDATE;
        case DISP_VENC_SDATE0:
            return VDP_DISP_INTF_SDDATE;
        default:
            return VDP_DISP_INTF_BUTT;
    }
}

/********************************************/
/* VDAC */
#define DISP_HAL_VDAC_FUNCTION_START_HAERE
HI_VOID DISP_HAL_ResetVDacState(HI_VOID)
{
    HI_S32 i;

    DISP_MEMSET(&s_stHalVDac[0], 0, sizeof(DISP_LOCAL_VDAC_S)*HI_DISP_VDAC_MAX_NUMBER);

    for(i=0; i<HI_DISP_VDAC_MAX_NUMBER; i++)
    {
        s_stHalVDac[i].bSupport = HI_TRUE;
        s_stHalVDac[i].bIdle    = HI_TRUE;
    }
    
    return;
}

HI_S32 DISP_HAL_VDACIsIdle(HI_U32 uVdac)
{
    HI_U32 v0;

    v0 = uVdac & 0xff;
    
    if (v0 < HI_DISP_VDAC_MAX_NUMBER)
    {
        if (s_stHalVDac[v0].bSupport && s_stHalVDac[v0].bIdle)
        {
            return HI_TRUE;
        }
    }
    
    return HI_FALSE;
}

HI_S32 PF_AcquireVDAC(HI_U32 uVdac)
{
    HI_U32 v0;

    v0 = uVdac & 0xff;
    
    if (v0 < HI_DISP_VDAC_MAX_NUMBER)
    {
        if (s_stHalVDac[v0].bSupport && s_stHalVDac[v0].bIdle)
        {
            s_stHalVDac[v0].bIdle = HI_FALSE;
        }
        else
        {
            return HI_FAILURE;;
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 PF_ReleaseVDAC(HI_U32 uVdac)
{
    HI_U32 v0;

    v0 = uVdac & 0xff;
    
    if (v0 < HI_DISP_VDAC_MAX_NUMBER)
    {
        if (s_stHalVDac[v0].bSupport && !s_stHalVDac[v0].bIdle)
        {
            s_stHalVDac[v0].bIdle = HI_TRUE;
        }
    }

    return HI_SUCCESS;
}


#define VDP_SYSCTRL_VDAC_BIT_SHIFT 20
HI_S32 PF_AddVDacToVenc(DISP_VENC_E eVenc, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal)
{
    HI_U32 v0;

    v0 = (uVdac >> 0) & 0xff;

    if (v0 < HI_DISP_VDAC_MAX_NUMBER)
    {
        HI_U32 vdac_hd_sel = 1;
            
        VDP_VDAC_SetLink(eVenc, v0, signal);

        if (DISP_VENC_HDATE0 == eVenc)
        {
            vdac_hd_sel = 1 << (v0 + VDP_SYSCTRL_VDAC_BIT_SHIFT);

            s_pu32VdpSysRegAddr[0] = s_pu32VdpSysRegAddr[0] | vdac_hd_sel;
        }
        else
        {
            vdac_hd_sel = ~(1 << (v0 + VDP_SYSCTRL_VDAC_BIT_SHIFT));
            s_pu32VdpSysRegAddr[0] = s_pu32VdpSysRegAddr[0] & vdac_hd_sel;
        }
    }
   
    return HI_SUCCESS;
}

HI_S32 PF_AddVDacToDisp(HI_DRV_DISPLAY_E enDisp, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal)
{
    HI_U32 v0;

    v0 = (uVdac >> 0) & 0xff;
   
    return HI_SUCCESS;
}


HI_S32 DISP_HAL_SetIdleVDACDisable(HI_VOID)
{
    HI_U32 i;

    for(i=0; i<HI_DISP_VDAC_MAX_NUMBER; i++)
    {
        if (s_stHalVDac[i].bSupport && s_stHalVDac[i].bIdle)
        {
            //printk("================Vdac %d is idle and close it\n", i);
            VDP_VDAC_SetEnable(i, 0);
        }
        
    }

    return HI_SUCCESS;
}


HI_S32 PF_SetVDACEnable(HI_U32 uVdac, HI_BOOL bEnable)
{
    HI_U32 v0;
    
    v0 = (uVdac >> 0) & 0xff;
    
    if (v0 < HI_DISP_VDAC_MAX_NUMBER)
    {
        if (HI_TRUE == bEnable)
        {
            //VDP_VDAC_SetClockEnable(v0, bEnable);
            VDP_VDAC_SetEnable(v0, bEnable);
        }
        else
        {
            VDP_VDAC_SetEnable(v0, bEnable);
            //VDP_VDAC_SetClockEnable(v0, bEnable);
        }
    }

    DISP_HAL_SetIdleVDACDisable();

    return HI_SUCCESS;
}



/********************************************/
/* interface */
#define DISP_HAL_INTF_FUNCTION_START_HAERE
HI_VOID DISP_HAL_ResetIntfState(HI_VOID)
{
    DISP_MEMSET(&s_stHalIntf[0], 0, sizeof(DISP_LOCAL_INTF_S)*HI_DRV_DISP_INTF_ID_MAX);

    s_stHalIntf[HI_DRV_DISP_INTF_YPBPR0].bSupport = HI_TRUE;
    s_stHalIntf[HI_DRV_DISP_INTF_YPBPR0].bIdle    = HI_TRUE;

    s_stHalIntf[HI_DRV_DISP_INTF_SVIDEO0].bSupport = HI_FALSE;
    s_stHalIntf[HI_DRV_DISP_INTF_SVIDEO0].bIdle    = HI_TRUE;

    s_stHalIntf[HI_DRV_DISP_INTF_CVBS0].bSupport = HI_TRUE;
    s_stHalIntf[HI_DRV_DISP_INTF_CVBS0].bIdle    = HI_TRUE;       

    s_stHalIntf[HI_DRV_DISP_INTF_VGA0].bSupport = HI_TRUE;
    s_stHalIntf[HI_DRV_DISP_INTF_VGA0].bIdle    = HI_TRUE;

    s_stHalIntf[HI_DRV_DISP_INTF_HDMI0].bSupport = HI_TRUE;
    s_stHalIntf[HI_DRV_DISP_INTF_HDMI0].bIdle    = HI_TRUE;

    return;
}



HI_BOOL DISP_HAL_INTFNeedVenc(HI_DRV_DISP_INTF_ID_E eID)
{
    switch( eID)
    {
        case HI_DRV_DISP_INTF_YPBPR0:
        case HI_DRV_DISP_INTF_SVIDEO0:
        case HI_DRV_DISP_INTF_CVBS0:
            return HI_TRUE;
        default:
            return HI_FALSE;
    }
}

HI_S32 PF_AcquireIntf2(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIf)
{
    HI_DRV_DISP_INTF_S *pstIntf = &pstIf->stIf;
        
    if(    !PF_TestIntfSupport(enDisp, pstIntf->eID)
        || !s_stHalIntf[pstIntf->eID].bSupport 
        || !s_stHalIntf[pstIntf->eID].bIdle)
    {
        return HI_FAILURE;
    }

    if(DISP_HAL_INTFNeedVenc(pstIntf->eID) )
    {
        // test vdac avaible
        if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
        {
            if (!DISP_HAL_VDACIsIdle(pstIntf->u8VDAC_Y_G))
            {
                return HI_FAILURE;
            }
        }
        if (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
        {
            if (!DISP_HAL_VDACIsIdle(pstIntf->u8VDAC_Pb_B))
            {
                return HI_FAILURE;
            }
        }
        if (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
        {
            if (!DISP_HAL_VDACIsIdle(pstIntf->u8VDAC_Pr_R))
            {
                return HI_FAILURE;
            }
        }

        //acquire vdac
        if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
        {
            PF_AcquireVDAC(pstIntf->u8VDAC_Y_G);
        }

        if (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
        {
            PF_AcquireVDAC(pstIntf->u8VDAC_Pb_B);
        }

        if (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
        {
            PF_AcquireVDAC(pstIntf->u8VDAC_Pr_R);
        }
    }

    s_stHalIntf[pstIntf->eID].bIdle = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 PF_ReleaseIntf2(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIf)
{
    HI_DRV_DISP_INTF_S *pstIntf = &pstIf->stIf;
    
    if(    s_stHalIntf[pstIntf->eID].bSupport 
        && !s_stHalIntf[pstIntf->eID].bIdle)
    {

        if(DISP_HAL_INTFNeedVenc(pstIntf->eID) )
        {
            //acquire vdac
            if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
            {
                PF_ReleaseVDAC(pstIntf->u8VDAC_Y_G);
            }

            if (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
            {
                PF_ReleaseVDAC(pstIntf->u8VDAC_Pb_B);
            }

            if (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
            {
                PF_ReleaseVDAC(pstIntf->u8VDAC_Pr_R);
            }
        }

        s_stHalIntf[pstIntf->eID].bIdle = HI_TRUE;
    }

    return HI_SUCCESS;
}



VDP_DISP_INTF_E DISP_HAL_GetHalIntfId(HI_DRV_DISP_INTF_ID_E eIntf)
{
    switch (eIntf)
    {
        case HI_DRV_DISP_INTF_VGA0:
            return VDP_DISP_INTF_VGA;
        case HI_DRV_DISP_INTF_HDMI0:
            return VDP_DISP_INTF_HDMI;
        case HI_DRV_DISP_INTF_BT1120_0:
            return VDP_DISP_INTF_BT1120;
        case HI_DRV_DISP_INTF_LCD0:
            return VDP_DISP_INTF_LCD;
        default:
            return VDP_DISP_INTF_BUTT;
    }
}

HI_S32 PF_ResetIntfFmt2(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIf, HI_DRV_DISP_FMT_E eFmt)
{
    HI_DRV_DISP_INTF_S *pstIntf = &pstIf->stIf;
    DISP_VENC_E enVenc = DISP_VENC_MAX;
    HI_BOOL bNeedVenc = HI_FALSE;
    
    // s1 select venc
    if ( (eFmt >= HI_DRV_DISP_FMT_PAL) &&(eFmt <= HI_DRV_DISP_FMT_1440x480i_60) )
    {
        enVenc = DISP_VENC_SDATE0;
        bNeedVenc = HI_TRUE;
    }
    else if ( eFmt <= HI_DRV_DISP_FMT_480P_60)
    {
        enVenc = DISP_VENC_HDATE0;
        bNeedVenc = HI_TRUE;
    }

    pstIf->bLinkVenc = bNeedVenc;
    pstIf->eVencId   = enVenc;
        
    if (bNeedVenc)
    {
        // s2 add venc to disp
#ifdef __DISP_D0_FOLLOW_D1__
        if ( (HI_DRV_DISPLAY_1 == enDisp) && (enVenc == DISP_VENC_SDATE0) )
        {
            // nothing todo
        }
        else
        {
            VDP_DISP_SetIntfMuxSel(DISP_HAL_GetChID(enDisp), 
                                   DISP_HAL_GetHalIntfIdForVenc(enVenc));    

            // s3 reset venc format
            VDP_DATE_ResetFmt(enVenc, eFmt);
        }
#else
        VDP_DISP_SetIntfMuxSel(DISP_HAL_GetChID(enDisp), 
                                  DISP_HAL_GetHalIntfIdForVenc(enVenc));    

        // s3 reset venc format
        VDP_DATE_ResetFmt(enVenc, eFmt);
#endif

        // s4 add vdac to venc
        // s5 reset vdac format
        if (HI_DRV_DISP_INTF_YPBPR0 == pstIntf->eID)
        {
            if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
            {
                PF_AddVDacToVenc(enVenc, pstIntf->u8VDAC_Y_G, HI_DRV_DISP_VDAC_Y);
                //pfOpt->PF_SetVencSignal(pstIt->eVencId, pstIt->stIf.u8VDAC_Y_G, HI_DRV_DISP_VDAC_Y);

                VDP_VDAC_ResetFmt(pstIntf->u8VDAC_Y_G, eFmt);

        //printk("DispSetIntfLink  002\n");
            }

            if (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
            {
                PF_AddVDacToVenc(enVenc, pstIntf->u8VDAC_Pb_B, HI_DRV_DISP_VDAC_PB);
                //pfOpt->PF_SetVencSignal(pstIt->eVencId, pstIt->stIf.u8VDAC_Pb_B, HI_DRV_DISP_VDAC_PB);

                VDP_VDAC_ResetFmt(pstIntf->u8VDAC_Pb_B, eFmt);
            }

            if (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
            {
                PF_AddVDacToVenc(enVenc, pstIntf->u8VDAC_Pr_R, HI_DRV_DISP_VDAC_PR);
                //pfOpt->PF_SetVencSignal(pstIt->eVencId, pstIt->stIf.u8VDAC_Pr_R, HI_DRV_DISP_VDAC_PR);
                VDP_VDAC_ResetFmt(pstIntf->u8VDAC_Pr_R, eFmt);
            }
        }
        else if (HI_DRV_DISP_INTF_CVBS0 == pstIntf->eID)
        {
            if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
            {
                PF_AddVDacToVenc(enVenc, pstIntf->u8VDAC_Y_G, HI_DRV_DISP_VDAC_CVBS);
                //pfOpt->PF_SetVencSignal(pstIt->eVencId, pstIt->stIf.u8VDAC_Y_G, HI_DRV_DISP_VDAC_Y);
                VDP_VDAC_ResetFmt(pstIntf->u8VDAC_Y_G, eFmt);
            }
        }
    }
    else
    {
        // TODO: ADD VDAC to DISP for VGA INTF
        // set vdac link
        if (pstIntf->u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
        {
        }

        if (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
        {
        }

        if (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
        {
        }    
    }

    return HI_SUCCESS;
}


#if 0
HI_S32 PF_AddIntfToDisp(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_ID_E eIntf)
{
    VDP_DISP_SetIntfMuxSel(DISP_HAL_GetChID(enDisp), 
                           DISP_HAL_GetHalIntfId(eIntf));
    
    return HI_SUCCESS;
}

HI_S32 PF_SetIntfColor(HI_DRV_DISP_INTF_ID_E eIntf, DISP_HAL_COLOR_S *pstColor)
{

    return HI_SUCCESS;
}
#endif

HI_S32 PF_SetIntfEnable2(HI_DRV_DISPLAY_E enDisp, DISP_INTF_S *pstIt, HI_BOOL bEnable)
{

//printk("========PF_SetIntfEnable2 date=%d, vdac=%d\n", pstIt->bLinkVenc, pstIt->stIf.u8VDAC_Y_G);
    // set venc link
    if (pstIt->bLinkVenc)
    {
        VDP_DATE_SetEnable(pstIt->eVencId, (HI_U32)bEnable);
    }

    // set vdac link
    if (pstIt->stIf.u8VDAC_Y_G != HI_DISP_VDAC_INVALID_ID)
    {
        PF_SetVDACEnable(pstIt->stIf.u8VDAC_Y_G, bEnable);
    //printk("DispSetIntfEnable  002\n");
    }

    if (pstIt->stIf.u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
    {
        PF_SetVDACEnable(pstIt->stIf.u8VDAC_Pb_B, bEnable);
    }

    if (pstIt->stIf.u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
    {
        PF_SetVDACEnable(pstIt->stIf.u8VDAC_Pr_R, bEnable);
    }

    return HI_SUCCESS;
}





/*===========================================================*/
/* WBC manager */
HI_VOID InitWbcState(HI_VOID)
{
    DISP_WBC_E eID;

    for(eID = DISP_WBC_00; eID < DISP_WBC_BUTT; eID++)
    {
        s_stWBC[eID].bSupport = HI_TRUE;
        s_stWBC[eID].bIdle    = HI_TRUE;
    }

    return;
}


HI_S32 PF_AcquireWbcByChn(HI_DRV_DISPLAY_E eChn, DISP_WBC_E *peWbc)
{
    if (eChn == HI_DRV_DISPLAY_1)
    {
        if(  (s_stWBC[DISP_WBC_00].bSupport == HI_TRUE)
            &&(s_stWBC[DISP_WBC_00].bIdle == HI_TRUE) 
            )
        {
            s_stWBC[DISP_WBC_00].bIdle = HI_FALSE;
            *peWbc = DISP_WBC_00;

            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

HI_S32 PF_AcquireWbc(DISP_WBC_E eWbc)
{
    if(  (s_stWBC[eWbc].bSupport == HI_TRUE)
        &&(s_stWBC[eWbc].bIdle == HI_TRUE) 
        )
    {
        s_stWBC[eWbc].bIdle = HI_FALSE;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 PF_ReleaseWbc(DISP_WBC_E eWbc)
{
    if(  (s_stWBC[eWbc].bSupport == HI_TRUE)
        &&(s_stWBC[eWbc].bIdle == HI_FALSE) 
        )
    {
        s_stWBC[eWbc].bIdle = HI_TRUE;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}


static VDP_LAYER_WBC_E s_eWbcHalID[DISP_WBC_BUTT] = 
{VDP_LAYER_WBC_HD0};

/* WBC */
HI_S32 PF_SetWbcDefault(DISP_WBC_E eWbc)
{

    VDP_WBC_SetOutIntf(s_eWbcHalID[eWbc], VDP_RMODE_INTERLACE);
    VDP_WBC_SetZmeInFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_422);

    return HI_SUCCESS;
}


HI_S32 PF_SetWbcIORect(DISP_WBC_E eWbc, HI_RECT_S *in, HI_RECT_S *out)
{
    VDP_DISP_RECT_S stRect;
    HI_U32 u32Ratio;

    DISP_MEMSET(&stRect, 0, sizeof(VDP_DISP_RECT_S));
    stRect.u32DXS = (HI_U32)in->s32X;
    stRect.u32DYS = (HI_U32)in->s32Y;
    stRect.u32DXL = (HI_U32)in->s32Width + stRect.u32DXS;
    stRect.u32DYL = (HI_U32)in->s32Height + stRect.u32DYS;
    VDP_WBC_SetCropReso(s_eWbcHalID[eWbc], stRect);

    stRect.u32OWth = (HI_U32)out->s32Width;
    stRect.u32OHgt = (HI_U32)out->s32Height;
    VDP_WBC_SetLayerReso(s_eWbcHalID[eWbc], stRect);

    u32Ratio = ((HI_U32)in->s32Width << 20) / stRect.u32OWth;

    //printk("iw=%d, ow=%d, ih=%d, oh=%d\n", in->s32Width, stRect.u32OWth, in->s32Height, stRect.u32OHgt );

    //printk("PF_SetWbcIORect hratio=0x%x\n", u32Ratio);
    VDP_WBC_SetZmeHorRatio(s_eWbcHalID[eWbc], u32Ratio);

    u32Ratio = ((HI_U32)in->s32Height << 12) / stRect.u32OHgt;
    //printk("PF_SetWbcIORect vratio=0x%x\n", u32Ratio);
    VDP_WBC_SetZmeVerRatio(s_eWbcHalID[eWbc], u32Ratio);

    //VDP_WBC_SetZmeEnable(s_eWbcHalID[eWbc], VDP_ZME_MODE_ALL, 0);

    VDP_WBC_SetZmeEnable(s_eWbcHalID[eWbc], VDP_ZME_MODE_HORL, 1);
    VDP_WBC_SetZmeEnable(s_eWbcHalID[eWbc], VDP_ZME_MODE_HORC, 1);
    VDP_WBC_SetZmeEnable(s_eWbcHalID[eWbc], VDP_ZME_MODE_VERL, 1);
    VDP_WBC_SetZmeEnable(s_eWbcHalID[eWbc], VDP_ZME_MODE_VERC, 1);

    return HI_SUCCESS;
}

HI_S32 PF_SetWbcColorSpace(DISP_WBC_E eWbc, HI_DRV_COLOR_SPACE_E eSrcCS, HI_DRV_COLOR_SPACE_E eDstCS)
{
    // //TODO:
    VDP_WBC_SetCscEnable(s_eWbcHalID[eWbc], 0);
    return HI_SUCCESS;
}

HI_S32 PF_SetWbcPixFmt(DISP_WBC_E eWbc, HI_DRV_PIX_FORMAT_E eFmt)
{
    switch(eFmt)
    {
        case HI_DRV_PIX_FMT_NV21:
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_SP420);
            VDP_WBC_SetOutFmtUVOrder(s_eWbcHalID[eWbc], 0);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_420);
            break;
        case HI_DRV_PIX_FMT_NV12:
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_SP420);
            VDP_WBC_SetOutFmtUVOrder(s_eWbcHalID[eWbc], 1);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_420);
            break;
        case HI_DRV_PIX_FMT_YUYV:
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_PKG_YUYV);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_422);
            break;
        case HI_DRV_PIX_FMT_UYVY:
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_PKG_UYVY);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_422);
            break;
        case HI_DRV_PIX_FMT_YVYU:
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_PKG_YVYU);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_422);
            break;
        default :
            VDP_WBC_SetOutFmt(s_eWbcHalID[eWbc], VDP_WBC_OFMT_SP420);
            VDP_WBC_SetOutFmtUVOrder(s_eWbcHalID[eWbc], 0);
            VDP_WBC_SetZmeOutFmt(s_eWbcHalID[eWbc], VDP_PROC_FMT_SP_420);
            break;
    }


    return HI_SUCCESS;
}

HI_S32 PF_SetWbcAddr(DISP_WBC_E eWbc, HI_DRV_VID_FRAME_ADDR_S *pstAddr)
{
    VDP_WBC_SetLayerAddr(s_eWbcHalID[eWbc], 
                         pstAddr->u32PhyAddr_Y, pstAddr->u32PhyAddr_C, 
                         pstAddr->u32Stride_Y, pstAddr->u32Stride_C);
    return HI_SUCCESS;
}

HI_S32 PF_SetWbcEnable(DISP_WBC_E eWbc, HI_BOOL bEnable)
{
    VDP_WBC_SetEnable(s_eWbcHalID[eWbc], (HI_U32)bEnable);
    return HI_SUCCESS;
}

HI_S32 PF_UpdateWbc(DISP_WBC_E eWbc)
{
    VDP_WBC_SetRegUp(s_eWbcHalID[eWbc]);
    return HI_SUCCESS;
}


/*==============================================*/
HI_VOID DispGetVersion(HI_U32 *pu32VirBaseAddr, HI_DRV_DISP_VERSION_S * pVersion)
{
    if (pu32VirBaseAddr)
    {
        pVersion->u32VersionPartL = pu32VirBaseAddr[4];
        pVersion->u32VersionPartH = pu32VirBaseAddr[5];
    }
    else
    {
        pVersion->u32VersionPartL = 0;
        pVersion->u32VersionPartH = 0;
    }
    
    return;
}

#define DISP_VDP_CLOCK_BUS_CKEN     0x00000001UL
#define DISP_VDP_CLOCK_CKEN         0x00000002UL
#define DISP_VDP_CLOCK_CLKOUT_CKEN  0x00000400UL

//#define DISP_VDP_CLOCK_BITS_TOTAL   0x00000403UL
#define DISP_VDP_CLOCK_BITS_TOTAL  (DISP_VDP_CLOCK_BUS_CKEN \
                                    | DISP_VDP_CLOCK_CKEN \
                                    | DISP_VDP_CLOCK_CLKOUT_CKEN)


HI_S32 DISP_OpenClock(HI_U32 u32SysCtrlBase)
{
    HI_U32 *pu32SysReg;
    HI_U32 val;

    pu32SysReg = (HI_U32 *)DISP_IOADDRESS(u32SysCtrlBase);

    if (!pu32SysReg)
    {
        DISP_FATAL("Can not get VDP sysctrl reg virtual address!\n");
        return  HI_FAILURE;
    }

    val = *pu32SysReg;

    if (  (val & DISP_X5HD2_MPW_SYSCTRL_RESET_BIT)
        || (  (val & DISP_VDP_CLOCK_BITS_TOTAL) != DISP_VDP_CLOCK_BITS_TOTAL)
       )
    {
        // here must set 'RESET' instead of '0', else VDP will die and never relive.
        *pu32SysReg = DISP_X5HD2_MPW_SYSCTRL_RESET_VALUE | DISP_X5HD2_MPW_SYSCTRL_RESET_BIT;

        DISP_MSLEEP(5);
        
        *pu32SysReg = DISP_X5HD2_MPW_SYSCTRL_RESET_VALUE;
        DISP_FATAL("====================== Set VDP CLOCK ENABLE===============!\n");
    }

    return HI_SUCCESS;
}

HI_S32 DISP_CloseClock(HI_U32 u32SysCtrlBase)
{
    HI_U32 *pu32SysReg;

    pu32SysReg = (HI_U32 *)DISP_IOADDRESS(u32SysCtrlBase);

    if (!pu32SysReg)
    {
        DISP_FATAL("Can not get VDP sysctrl reg virtual address!\n");
        return  HI_FAILURE;
    }

   //*pu32SysReg = DISP_X5HD2_MPW_SYSCTRL_RESET_BIT;
   *pu32SysReg = 0;
   
    DISP_FATAL("====================== Set VDP CLOCK DISABLE===============!\n");
    
    return HI_SUCCESS;
}

HI_S32 DISP_HAL_Init(HI_U32 u32Base)
{
    HI_S32 nRet;
    
    // s1 if input version
    if(s_DispIntfFlag > 0)
    {
        return HI_SUCCESS;
    }

    // s1.2 open vdp clock. 
    nRet = DISP_OpenClock(DISP_X5HD2_MPW_SYSCTRL_BASE);
    if (nRet != HI_SUCCESS)
    {
        return nRet;
    }

    // s2 if input Base
    // s2.1 get virual address
    s_u32VdpBaseAddr = (HI_U32)DISP_IOADDRESS(u32Base);
        
    if (!s_u32VdpBaseAddr)
    {
        return HI_FAILURE;
    }

    DispGetVersion((HI_U32 *)s_u32VdpBaseAddr, &s_stVersion);
    //printk(">>>>>>>>>>>Base=%x, vir=%x, vh=%x, vl=%x\n", 
    //      u32Base, RegVirAddr, s_stVersion.u32VersionPartH, s_stVersion.u32VersionPartL);

    // s2.3 initial ops
    DISP_MEMSET(&s_stIntfOps, 0, sizeof(DISP_INTF_OPERATION_S));

    if (  (s_stVersion.u32VersionPartH == DISP_X5HD2_MPW_VERSION_H)
        &&(s_stVersion.u32VersionPartL == DISP_X5HD2_MPW_VERSION_L)
        )
    {
        nRet = DISP_HAL_InitVDPState();
        if (nRet)
        {
            DISP_FATAL("Hal init vdp state failed\n");
            return nRet;
        }

        //printk(">>>>>>>>>>> DISP_X5HD2_MPW_VERSION \n");
        /* reset hardware */
        s_stIntfOps.PF_ResetHardware  = PF_ResetVdpHardware;

        /* display */
        /* capability */
        s_stIntfOps.PF_TestChnSupport     = PF_TestChnSupport;
        s_stIntfOps.PF_TestChnSupportHD   = PF_TestChnSupportHD;
        s_stIntfOps.PF_TestIntfSupport    = PF_TestIntfSupport;
        s_stIntfOps.PF_TestChnSupportCast = PF_TestChnSupportCast;
        s_stIntfOps.PF_TestChnEncFmt      = PF_TestChnEncFmt;
        s_stIntfOps.PF_TestChnAttach      = PF_TestChnAttach;

        //s_stIntfOps.PF_ResetChn  = PF_ResetChn;
        //s_stIntfOps.PF_ConfigChn = PF_ConfigChn;
        s_stIntfOps.PF_SetChnFmt = PF_SetChnFmt;
        s_stIntfOps.PF_SetChnTiming = PF_SetChnTiming;

        s_stIntfOps.PF_SetChnPixFmt  = PF_SetChnPixFmt;
        s_stIntfOps.PF_SetChnBgColor = PF_SetChnBgColor;
        s_stIntfOps.PF_SetChnColor   = PF_SetChnColor;
        s_stIntfOps.PF_SetDispSignal = PF_SetDispSignal;

        s_stIntfOps.PF_SetChnEnable  = PF_SetChnEnable;
        s_stIntfOps.PF_GetChnEnable  = PF_GetChnEnable;

        /* interrypt */
        s_stIntfOps.PF_SetIntEnable = PF_SetIntEnable;
        s_stIntfOps.PF_GetMaskedIntState = PF_GetMaskedIntState;
        s_stIntfOps.PF_GetIntSetting = PF_GetIntSetting;
        s_stIntfOps.PF_GetUnmaskedIntState = PF_GetUnmaskedIntState;
        s_stIntfOps.PF_CleanIntState = PF_CleanIntState;
        s_stIntfOps.FP_GetChnIntState = FP_GetChnIntState;
        s_stIntfOps.FP_GetChnBottomFlag = FP_GetChnBottomFlag;

        /* interface manager */
        s_stIntfOps.PF_AcquireIntf2   = PF_AcquireIntf2;
        s_stIntfOps.PF_ReleaseIntf2   = PF_ReleaseIntf2;
        s_stIntfOps.PF_ResetIntfFmt2  = PF_ResetIntfFmt2;
        s_stIntfOps.PF_SetIntfEnable2 = PF_SetIntfEnable2;

        /* WBC manager */
        s_stIntfOps.PF_AcquireWbcByChn = PF_AcquireWbcByChn;
        s_stIntfOps.PF_AcquireWbc = PF_AcquireWbc;
        s_stIntfOps.PF_ReleaseWbc = PF_ReleaseWbc;

        /* WBC */
        s_stIntfOps.PF_SetWbcIORect = PF_SetWbcIORect;
        s_stIntfOps.PF_SetWbcColorSpace = PF_SetWbcColorSpace;
        s_stIntfOps.PF_SetWbcPixFmt = PF_SetWbcPixFmt;
        s_stIntfOps.PF_SetWbcAddr = PF_SetWbcAddr;
        s_stIntfOps.PF_SetWbcEnable = PF_SetWbcEnable;
        s_stIntfOps.PF_UpdateWbc= PF_UpdateWbc;
    }
    else
    {
        DISP_ERROR("Not support version : %x %x\n", 
            s_stVersion.u32VersionPartH, s_stVersion.u32VersionPartL);
    }

    s_DispIntfFlag++;

    return HI_SUCCESS;
}

HI_S32 DISP_HAL_DeInit(HI_VOID)
{

    DISP_CloseClock(DISP_X5HD2_MPW_SYSCTRL_BASE);
    
    s_DispIntfFlag = 0;

    return HI_SUCCESS;
}

HI_S32 DISP_HAL_GetOperation(DISP_INTF_OPERATION_S *pstFunction)
{
    if(s_DispIntfFlag < 0)
    {
        DISP_ERROR("DISP_INTF Not inited\n");
        return HI_FAILURE;
    }

    // *pstFunction = s_stIntfOps;
    memcpy(pstFunction, &s_stIntfOps, sizeof(DISP_INTF_OPERATION_S));

    return HI_SUCCESS;
}

DISP_INTF_OPERATION_S *DISP_HAL_GetOperationPtr(HI_VOID)
{
    if(s_DispIntfFlag < 0)
    {
        DISP_ERROR("DISP_INTF Not inited\n");
        return HI_NULL;
    }

    return &s_stIntfOps;
}

HI_S32 DISP_HAL_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion)
{
    if(s_DispIntfFlag < 0)
    {
        DISP_ERROR("DISP_INTF Not inited\n");
        return HI_FAILURE;
    }

    if (pstVersion)
    {
        *pstVersion = s_stVersion;
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}


HI_S32 DISP_DEBUG_PrintPtr(HI_VOID)
{
    HI_S32 i;
    HI_U32 *ptrf;

    ptrf = (HI_U32 *) & s_stIntfOps;

    for(i=0; i<(sizeof(s_stIntfOps)/sizeof(HI_U32)); i++)
    {
        DISP_WARN("i=%d, v=0x%x\n", i, ptrf[i]);
    }


    return HI_SUCCESS;
}




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */



