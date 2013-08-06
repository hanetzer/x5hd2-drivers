#ifndef __VPSS_COMMON_H__
#define __VPSS_COMMON_H__

#include"drv_mem_ext.h"
#include"drv_vdec_ext.h"
#include "hi_debug.h"
#include <linux/list.h>
#include <linux/io.h>
#include <linux/delay.h>


#define DEF_HI_DRV_VPSS_VERSION 0x101  
/*S40V200 Çý¶¯*/
/*
#ifdef VPSS_S40  
#endif
*/
 /*CV200 Çý¶¯*/
#ifdef VPSS_CV200
#define DEF_HI_DRV_VPSS_VERSION 0x102 
#endif

#define DEF_FILE_NAMELENGTH 20

typedef struct list_head LIST;

/*
    300M/20 = 15M = 0xe4e1c0
    300M/60 = 5M = 0x4c4b40
    300M/100 = 3M = 0x2dc6c0
*/
//#define DEF_LOGIC_TIMEOUT  0x4c4b40
//#define DEF_LOGIC_TIMEOUT  0x2dc6c0
#define DEF_LOGIC_TIMEOUT  0xe4e1c0

#define VPSS_NAME  "HI_VPSS"

#define VPSS_KMALLOC(fmt...)      HI_KMALLOC  (HI_ID_VPSS, fmt)
#define VPSS_KFREE(fmt...)        HI_KFREE    (HI_ID_VPSS, fmt)
#define VPSS_VMALLOC(fmt...)       HI_VMALLOC   (HI_ID_VPSS, fmt)
#define VPSS_VFREE(fmt...)       HI_VFREE   (HI_ID_VPSS, fmt)

#define VPSS_FATAL(fmt...) \
            HI_FATAL_PRINT(HI_ID_VPSS, fmt)

#define VPSS_ERROR(fmt...) \
            HI_ERR_PRINT(HI_ID_VPSS, fmt)

#define VPSS_WARN(fmt...) \
            HI_WARN_PRINT(HI_ID_VPSS, fmt)

#define VPSS_INFO(fmt...) \
            HI_INFO_PRINT(HI_ID_VPSS, fmt)

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define OPTM_ALG_MIN2(x, y) (((x) < (y)) ? (x) : (y))
#define OPTM_ALG_MAX2(x, y) (((x) > (y)) ? (x) : (y))
#define OPTM_ALG_CLIP3(low, high, x) (OPTM_ALG_MAX2( OPTM_ALG_MIN2((x), (high)), (low)))
#define OPTM_ALG_ROUND(x)   (((x % 10) > 4) ? (x / 10 + 1) * 10 : x)
#endif
