/*
 * include/adv7401.h for Linux .
 *
 * This file defines adv7401 micro-definitions for user.
 * History:
 *      15-Jan-2011 create this file
 */

#ifndef __ADV7401_H__
#define __ADV7401_H__

#include "hi_debug.h"

#define HI_FATAL_ADV7401(fmt...) HI_FATAL_PRINT(HI_ID_ADV7401, fmt)
#define HI_ERR_ADV7401(fmt...) HI_ERR_PRINT(HI_ID_ADV7401, fmt)
#define HI_WARN_ADV7401(fmt...) HI_WARN_PRINT(HI_ID_ADV7401, fmt)
#define HI_INFO_ADV7401(fmt...) HI_INFO_PRINT(HI_ID_ADV7401, fmt)
#define HI_DBG_ADV7401(fmt...) HI_DBG_PRINT(HI_ID_ADV7401, fmt)

#define EBI_DQ11_ADDRESS 0x102030c8
#define GPIO6_DIR_ADDRESS 0x101ec400
#define GPIO6_DATA_ADDRESS 0x101ec020

#define MAX_ADV7401_FMT_NUM 8
#define MAX_ADV7401_REG_CFG_NUM 24

typedef enum hi_ADV7401_FMT_E
{
    ADV7401_FMT_480I,
    ADV7401_FMT_576I,
    ADV7401_FMT_480P,
    ADV7401_FMT_576P,
    ADV7401_FMT_720P_50,
    ADV7401_FMT_720P_60,
    ADV7401_FMT_1080I_50,
    ADV7401_FMT_1080I_60,
    ADV7401_FMT_BUTT
} ADV7401_FMT_E;

#define ADV7401_SET_FMT _IOW(HI_ID_ADV7401, 0x01, ADV7401_FMT_E)

#endif //__ADV7401_H__
