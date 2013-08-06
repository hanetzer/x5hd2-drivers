/******************************************************************************
Copyright (C), 2012-2062, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : common_mem_drv.h
Version       : V1.0 Initial Draft
Author        : g00182102
Created       : 2012/6/19
Last Modified :
Description   : In kernel mode, memory allocate interfaces for other modules.
Function List : None.
History       :
******************************************************************************/

#ifndef __DRV_MEM_EXT_H__
#define __DRV_MEM_EXT_H__

/** @addtogroup H_MEM */
/** @{ */

#ifdef CMN_MMGR_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

HI_VOID*    hi_kmalloc(HI_U32 module_id, HI_U32 size, HI_S32 flags);
HI_VOID     hi_kfree(HI_U32 module_id, HI_VOID *ptr);

HI_VOID*    hi_vmalloc(HI_U32 module_id, HI_U32 size);
HI_VOID     hi_vfree(HI_U32 module_id, HI_VOID *ptr);

#define HI_KMALLOC(module_id, size, flags)      hi_kmalloc(module_id, size, flags)
#define HI_KFREE(module_id, addr)               hi_kfree(module_id, addr)
#define HI_VMALLOC(module_id, size)             hi_vmalloc(module_id, size)
#define HI_VFREE(module_id, addr)               hi_vfree(module_id, addr)
#else
#include <linux/slab.h>
#include <linux/vmalloc.h>
#define HI_KMALLOC(module_id, size, flags)      kmalloc(size, flags)
#define HI_KFREE(module_id, addr)               kfree(addr)
#define HI_VMALLOC(module_id, size)             vmalloc(size)
#define HI_VFREE(module_id, addr)               vfree(addr)
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif


