/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : otp_reg_v200.h
  Version       : Initial Draft
  Author        : 
  Created       : 
  Last Modified :
  Description   : OTP REG DEFINE
  Function List :
  History       :
******************************************************************************/
#ifndef __OTP_DRV_V200_H__
#define __OTP_DRV_V200_H__

#include "hi_common.h"
#include "hi_type.h"
#include "drv_otp_common.h"

HI_S32 OTP_V200_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey);
HI_S32 OTP_V200_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey);
HI_S32 OTP_V200_Get_DDPLUS_Flag(HI_BOOL *pu8DDPlusFlag);
HI_S32 OTP_V200_Get_DTS_Flag(HI_BOOL *pu8DTSFlag);
HI_S32 OTP_V200_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);
HI_S32 OTP_V200_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);
HI_S32 OTP_V200_SetHdcpRootKey(HI_U8 *pu8Key);
HI_S32 OTP_V200_GetHdcpRootKey(HI_U8 *pu8Key);
HI_S32 OTP_V200_SetHdcpRootKeyLock(HI_VOID);
HI_S32 OTP_V200_GetHdcpRootKeyLock(HI_BOOL *pBLock);
HI_S32 OTP_V200_SetSTBRootKey(HI_U8 u8Key[16]);
HI_S32 OTP_V200_GetSTBRootKey(HI_U8 u8Key[16]);
HI_S32 OTP_V200_LockSTBRootKey(HI_VOID);
HI_S32 OTP_V200_GetSTBRootKeyLockFlag(HI_BOOL *pBLock);
HI_S32 OTP_V200_Reset(HI_VOID);

#ifdef SDK_OTP_ARCH_VERSION_V3
/* hal v200 interface */
HI_U32 do_apb_v200_read(HI_U32 addr);
HI_U8 do_apb_v200_read_byte(HI_U32 addr);
HI_S32 do_apb_v200_write(HI_U32 addr,HI_U32 tdata);
HI_S32 do_apb_v200_write_byte(HI_U32 addr,HI_U8 tdata);
HI_S32 do_apb_v200_write_bit(HI_U32 addr, HI_U32 bit_pos, HI_U32  bit_value);
#endif

#endif /* __OTP_DRV_V200_H__ */
