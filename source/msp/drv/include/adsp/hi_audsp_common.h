/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_audsp_common.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/10/08
  Description   :
  History       :
  1.Date        : 2013/02/28
    Author      : zgjie
    Modification: Created file

 *******************************************************************************/

#ifndef __HI_AUDSP_COMMON__H__
#define __HI_AUDSP_COMMON__H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
 #endif
#endif

/* Reg Addr Definition */
#define DSP_SYSCRG_REGBASE  0xf8a220d0
#define DSP_CTL_REGBASE     0xf8cd0180
/* Macro Definition */
#define ELF_IO_SECTIONS_ADDR 0xf8a20000

/** Audio DSP Code definition*/
/** CNcomment:音频处理器模块代码标识定义 */
typedef enum hiADSP_CODEID_E
{
    /* dsp manage module */
    ADSP_CODE_SYSTEM = 0x0000,  /* dsp system  */
    ADSP_CODE_AOE,   /* audio output engine  */
    ADSP_CODE_ADE,   /* audio decode engine  */
    ADSP_CODE_AEE,   /* audio encode engine  */

    /* dsp decoder module */
    ADSP_CODE_DEC_MP2 = 0x0100,   /* MPEG audio layer 1, 2 */
    ADSP_CODE_DEC_MP3,            /* MPEG audio layer 1, 2 or 3 */
    ADSP_CODE_DEC_AAC,
    ADSP_CODE_DEC_DDP,
    ADSP_CODE_DEC_DTS,
    ADSP_CODE_DEC_TRUEHD,
    ADSP_CODE_DEC_WMASTD,
    ADSP_CODE_DEC_WMAPRO,
    ADSP_CODE_DEC_DRA,

    /* dsp encoder module */
    ADSP_CODE_ENC_AAC = 0x200,

    /* dsp codec module */
    ADSP_CODE_CODEC_AMRNB = 0x400,
    ADSP_CODE_CODEC_AMRWB,

    /* dsp SRS advance effect */
    ADSP_CODE_ADV_SRS_STUDIOSOUND_3D = 0x800,
    ADSP_CODE_ADV_SRS_STUDIOSOUND_HD = 0x800,

    /* dsp Dolby advance effect */
    ADSP_CODE_ADV_DOLBYDV258 = 0x810,
} ADSP_CODEID_E;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
