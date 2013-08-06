/******************************************************************************
Copyright (C), 2012-2062, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : kmodule_mgr.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : The module manager in kernel.
Function List : None.
History       :
******************************************************************************/
/** @addtogroup iKMODULE_MGR */

/** @{ */
#ifndef __KMODULE_MGR_H__
#define __KMODULE_MGR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define KMODULE_MAX_COUNT      (256)
#define KMODULE_MEM_MAX_COUNT  (256*256)

HI_S32  MMNGR_DRV_ModInit(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount);
HI_VOID MMNGR_DRV_ModExit(HI_VOID);

HI_S32  HI_DRV_MMNGR_Init(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount);
HI_VOID HI_DRV_MMNGR_Exit(HI_VOID);


#ifdef __cplusplus
}
#endif

#endif
/** @} */
