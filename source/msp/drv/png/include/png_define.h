/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: png_define.h
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/11
Description	: public macro definition
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2010/10/11		z00141204		Created file      	
******************************************************************************/

#ifndef __PNG_DEFINE_H__
#define __PNG_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif	/* __cplusplus */
#endif	/* __cplusplus */

#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/hardirq.h>

#include "hi_kernel_adapt.h"

#include "hi_png_config.h"

#define PNG_FATAL(fmt...)      HI_FATAL_PRINT(HI_ID_PNG, fmt)
#define PNG_ERROR(fmt...)      HI_ERR_PRINT(HI_ID_PNG, fmt)
#define PNG_WARNING(fmt...)    HI_WARN_PRINT(HI_ID_PNG, fmt)
#define PNG_INFO(fmt...)       HI_INFO_PRINT(HI_ID_PNG, fmt)


#ifdef DEBUG
#define PNG_ASSERT(EXP)  do {\
    if(!(EXP))\
    {\
        PNG_ERROR( "Assertion [%s] failed! %s:%s(line=%d)\n",#EXP,__FILE__,__FUNCTION__,__LINE__);\
        panic("Assertion panic\n");\
    }\
} while(0)
#else
#define PNG_ASSERT(EXP)
#endif

//#define PNG_DECLARE_MUTEX(mutex)    DECLARE_MUTEX(mutex)
#define PNG_DECLARE_MUTEX(mutex)    HI_DECLARE_MUTEX(mutex)


#define PNG_DOWN_INTERRUPTIBLE(pmutex)	do\
{\
    if(!in_interrupt())\
    {\
        if (down_interruptible(pmutex) < 0)\
        {\
            PNG_FATAL("[%s:%d]down_interruptiblie failed!\n", __FUNCTION__, __LINE__);\
        }\
    }\
}while(0)

#define PNG_UP(pmutex) up(pmutex)

#define PNG_UP_INT(pmutex) do\
{\
    if(!in_interrupt())\
    {\
        up(pmutex);\
    }\
}while(0)

#define PNG_DECLARE_WAITQUEUE(queue_head) DECLARE_WAIT_QUEUE_HEAD(queue_head)
#define PNG_WAIT_EVENT_INTERRUPTIBLE(queue_head, condition) wait_event_interruptible(queue_head, (condition))        
#define PNG_WAKE_UP_INTERRUPTIBLE(queue_head_p) wake_up_interruptible(queue_head_p)

#ifdef __cplusplus
#if __cplusplus
}
#endif	/* __cplusplus */
#endif	/* __cplusplus */

#endif
