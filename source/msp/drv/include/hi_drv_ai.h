/******************************************************************************

Copyright (C), 2009-2019, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_ai.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : ai
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_DRV_AI_H__
 #define __HI_DRV_AI_H__

#ifdef __cplusplus
#if __cplusplus
 extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_unf_ai.h"

/*Define Debug Level For HI_ID_AI                     */

 #define HI_FATAL_AI(fmt...) \
    HI_FATAL_PRINT(HI_ID_AI, fmt)

 #define HI_ERR_AI(fmt...) \
    HI_ERR_PRINT(HI_ID_AI, fmt)

 #define HI_WARN_AI(fmt...) \
    HI_WARN_PRINT(HI_ID_AI, fmt)

 #define HI_INFO_AI(fmt...) \
    HI_INFO_PRINT(HI_ID_AI, fmt)

#define CHECK_AI_NULL_PTR(p)                                \
    do {                                                    \
            if(HI_NULL == p)                                \
            {                                               \
                HI_ERR_AI("NULL pointer \n");               \
                return HI_ERR_AI_NULL_PTR;                   \
            }                                               \
         } while(0)
         
#define CHECK_AI_CREATE(state)                              \
    do                                                      \
    {                                                       \
        if (0 > state)                                      \
        {                                                   \
            HI_WARN_AI("AI  device not open!\n");           \
            return HI_ERR_AI_NOT_INIT;                \
        }                                                   \
    } while (0)
    
#define CHECK_AI_PORT(port)                                  \
    do                                                          \
    {                                                           \
        if (HI_UNF_AI_BUTT <= port)                            \
        {                                                       \
            HI_WARN_AI(" Invalid snd id %d\n", port);           \
            return HI_ERR_AI_INVALID_ID;                       \
        }                                                       \
    } while (0)


#define CHECK_AI_SAMPLERATE(outrate )                   \
    do                                                  \
    {                                                   \
        switch (outrate)                                \
        {                                               \
        case  HI_UNF_SAMPLE_RATE_8K:                    \
        case  HI_UNF_SAMPLE_RATE_11K:                   \
        case  HI_UNF_SAMPLE_RATE_12K:                   \
        case  HI_UNF_SAMPLE_RATE_16K:                   \
        case  HI_UNF_SAMPLE_RATE_22K:                   \
        case  HI_UNF_SAMPLE_RATE_24K:                   \
        case  HI_UNF_SAMPLE_RATE_32K:                   \
        case  HI_UNF_SAMPLE_RATE_44K:                   \
        case  HI_UNF_SAMPLE_RATE_48K:                   \
        case  HI_UNF_SAMPLE_RATE_88K:                   \
        case  HI_UNF_SAMPLE_RATE_96K:                   \
        case  HI_UNF_SAMPLE_RATE_176K:                  \
        case  HI_UNF_SAMPLE_RATE_192K:                  \
            break;                                      \
        default:                                        \
            HI_WARN_AI("invalid sample out rate %d\n", outrate);    \
            return HI_ERR_AI_INVALID_PARA;                        \
            }                                                       \
            } while (0)   


 #define HI_AI_LOCK(mutex) (void)pthread_mutex_lock(mutex);
 #define HI_AI_UNLOCK(mutex) (void)pthread_mutex_unlock(mutex);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

 #endif
