/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  viu_utils.h
* Description:
*
***********************************************************************************/
#ifndef __VIU_UTILS_H__
#define __VIU_UTILS_H__
	
#include "hi_unf_vi.h"

struct file *VIU_UtilsFopen(const HI_S8 *filename, HI_S32 flags);
HI_VOID VIU_UtilsFclose(struct file *filp);
HI_S32 VIU_UtilsFread(HI_S8 *buf, HI_U32 len, struct file *filp);
HI_S32 VIU_UtilsFwrite(HI_S8 *buf, HI_S32 len, struct file *filp);

#endif // __VIU_UTILS_H__

