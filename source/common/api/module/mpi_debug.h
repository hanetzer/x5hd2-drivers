/******************************************************************************
Copyright (C), 2012-2062, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : module_debug.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module debug MACRO.
Function List : None.
History       :
******************************************************************************/
#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

#define COLOR_START      "\33[32m"
#define COLOR_START_HEAD "\33[35m" /* pink text color*/

#define COLOR_START_RED "\33[31m"  /* red text color */
#define COLOR_END       "\33[0m"

#define THIS_NAME "module_mgr"

#if 0
#define THIS_FATAL_PRINT(fmt, arg...) printf(COLOR_START_RED);printf("[%s:%s:%d]: ", THIS_NAME, __func__, __LINE__);printf(COLOR_END);printf(fmt, ##arg)
#define THIS_ERR_PRINT(  fmt, arg...) printf(COLOR_START_RED);printf("[%s:%s:%d]: ", THIS_NAME, __func__, __LINE__);printf(COLOR_END);printf(fmt, ##arg)
#define THIS_WARN_PRINT( fmt, arg...) printf(COLOR_START_RED);printf("[%s:%s:%d]: ", THIS_NAME, __func__, __LINE__);printf(COLOR_END);printf(fmt, ##arg)
#define THIS_INFO_PRINT( fmt, arg...) printf(COLOR_START_RED);printf("[%s:%s:%d]: ", THIS_NAME, __func__, __LINE__);printf(COLOR_END);printf(fmt, ##arg)
#else
#define THIS_FATAL_PRINT(fmt, arg...)
#define THIS_ERR_PRINT(  fmt, arg...)
#define THIS_WARN_PRINT( fmt, arg...)
#define THIS_INFO_PRINT( fmt, arg...)
#endif


#ifdef __cplusplus
}
#endif

#endif
