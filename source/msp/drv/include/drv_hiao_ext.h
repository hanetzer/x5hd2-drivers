/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		: 	mpi_priv_hiao_ext.h
  Version        		: 	Initial Draft
  Author         		: 	Hisilicon multimedia software group
  Created       		: 	2009/09/29
  Last Modified		    :
  Description  		    :
  Function List 		:
  History       		:
  1.Date        		: 	2009/09/29
    Author      		: 	z40717
    Modification   	    :	Created file

******************************************************************************/

#ifndef __HI_DRV_HIAO_H__
#define __HI_DRV_HIAO_H__

#include <linux/seq_file.h>
#include "hi_mpi_hiao.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */



HI_S32      HI_DRV_HIAO_Init(HI_VOID);
HI_VOID     HI_DRV_HIAO_DeInit(HI_VOID);
HI_S32      HI_DRV_HIAO_GetOpenDefaultParam(HIAO_ATTR_S *pstDefalutPara);
HI_S32      HI_DRV_HIAO_Open(HIAO_ATTR_S   *pstAoAttr);
HI_S32      HI_DRV_HIAO_Close(HI_VOID);
HI_S32      HI_DRV_HIAO_SetVolume(HI_U32 u32MixWeight);
HI_S32		HI_DRV_HIAO_SetTrackMode(HI_UNF_TRACK_MODE_E enMode);
HI_S32		HI_DRV_HIAO_Reset(HI_VOID);
HI_S32		HI_DRV_HIAO_SetPause(HI_BOOL bEnable);
HI_S32      HI_DRV_HIAO_GetHandle(HI_HANDLE *phHandle, HI_S32 s32PortIdx);
HI_S32		HI_DRV_HIAO_GetDelayMs(HI_U32 *pDelay);
HI_S32		HI_DRV_HIAO_SetSpeedAdjust(HI_S32 s32Speed);
HI_S32		HI_DRV_HIAO_SendData(HI_HANDLE hHandle, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

typedef HI_S32 (*FN_HIAO_GetHandle)(HI_HANDLE *phHandle, HI_S32 s32PortIdx);
typedef HI_S32 (*FN_HIAO_SendData)(HI_HANDLE, const HI_UNF_AO_FRAMEINFO_S *);
typedef struct 
{
    FN_HIAO_GetHandle                   pfnHiaoGetHandle;
    FN_HIAO_SendData                    pfnHiaoSendData;
} HIAO_EXT_FUNC_S;

HI_S32		AIAO_DRV_ModInit(HI_VOID);
HI_S32		AIAO_DRV_ModExit(HI_VOID);

HI_S32		HIAO_DRV_ModInit(HI_VOID);
HI_VOID		HIAO_DRV_ModExit(HI_VOID);
HI_S32      AIAO_DRV_Init();
HI_S32      AIAO_DRV_Exit();
HI_S32		HIAO_DRV_Proc(struct seq_file *p, HI_VOID *v);

//extern HI_S32 hi_gpio_dirset_bit(HI_U32 u32GpioPathNum, HI_U32 u32BitX, HI_U32 u32DirBit);
//extern HI_S32 HI_DRV_GPIO_WriteBit(HI_U32 u32GpioPathNum, HI_U32 u32BitX, HI_U32 u32BitValue);
HI_VOID HIAO_MUTECTL_CFG(HI_VOID);
HI_VOID HIAO_MUTECTL_CFG_ENABLE(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif //__HI_DRV_HIAO_H__
