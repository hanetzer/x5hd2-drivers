/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : otp_drv_v100.h
  Version       : Initial Draft
  Author        : 
  Created       : 
  Last Modified :
  Description   : OTP REG DEFINE
  Function List :
  History       :
******************************************************************************/
#ifndef __OTP_DRV_V100_H__
#define __OTP_DRV_V100_H__

#ifdef SDK_OTP_ARCH_VERSION_V3
#include "hi_drv_otp.h"
#else
#include "priv_otp.h"
#endif

HI_S32 OTP_V100_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey);
HI_S32 OTP_V100_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey);
HI_S32 OTP_V100_Get_DDPLUS_Flag(HI_BOOL *pDDPLUSFlag);
HI_S32 OTP_V100_Func_Disable(HI_U32 u32SRBit);
HI_S32 OTP_V100_Get_DTS_Flag(HI_BOOL *pDTSFlag);
HI_S32 OTP_V100_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);
HI_S32 OTP_V100_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData);

#ifdef SDK_OTP_ARCH_VERSION_V3
/* hal v100 interface */
HI_S32 do_apb_write_byte(HI_U32 addr, HI_U32 tdata);
HI_S32 do_apb_write_bit(HI_U32 addr, HI_U32 bit_pos, HI_U32 bit_value);
HI_S32 set_apb_write_protect(HI_VOID);
HI_S32 get_apb_write_protect(HI_U32 *penable);
HI_U32 do_apb_para_read(HI_U32 addr);
HI_VOID otp_get_sr_bit(HI_S32 pos,HI_S32 *pvalue);
HI_S32 otp_set_sr_bit(HI_S32 pos);
HI_S32 otp_func_disable(HI_U32 bit_pos, HI_U32 bit_value);
HI_S32 otp_reset(HI_VOID);
#endif

#endif /* __OTP_DRV_V100_H__ */
