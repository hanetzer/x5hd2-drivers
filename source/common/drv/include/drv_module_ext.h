/******************************************************************************
Copyright (C), 2012-2062, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : common_module_drv.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module manager in kernel.
Function List : None.
History       :
******************************************************************************/
#ifndef __DRV_MODULE_EXT_H__
#define __DRV_MODULE_EXT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "hi_type.h"

/** @addtogroup H_MODULE */

/** @{ */

HI_S32 HI_DRV_MODULE_AllocId(HI_U8* pu8ModuleName, HI_U32 *pu32ModuleID, HI_S32 *ps32Status);
HI_S32 HI_DRV_MODULE_Register(HI_U32 u32ModuleID, const HI_U8* pu8ModuleName, HI_VOID* pFunc);
HI_S32 HI_DRV_MODULE_UnRegister(HI_U32 u32ModuleID);

#ifdef CMN_MMGR_SUPPORT
HI_U8* HI_DRV_MODULE_GetNameByID(HI_U32 u32ModuleID);
HI_U32 HI_DRV_MODULE_GetIDByName(HI_U8* pu8Name);
#endif

HI_S32 HI_DRV_MODULE_GetFunction(HI_U32 u32ModuleID, HI_VOID** ppFunc);

/** @} */

#ifdef __cplusplus
}
#endif

#endif


