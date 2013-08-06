/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_otp.h
  Version       : Initial Draft
  Author        : advca
  Created       : 2013/04/12
  Description   : 
  History       :
  1.Date        : 2013/04/12
    Author      : z00213260
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_OTP_H__
#define __HI_DRV_OTP_H__

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/

#define OTP_CUSTOMER_KEY_LEN	16
#define OTP_HDCP_ROOT_KEY_LEN   16
#define OTP_STB_ROOT_KEY_LEN    16

#define HI_FATAL_OTP(fmt...)    HI_FATAL_PRINT  (HI_ID_OTP, fmt)
#define HI_ERR_OTP(fmt...)      HI_ERR_PRINT    (HI_ID_OTP, fmt)
#define HI_WARN_OTP(fmt...)     HI_WARN_PRINT   (HI_ID_OTP, fmt)
#define HI_INFO_OTP(fmt...)     HI_INFO_PRINT   (HI_ID_OTP, fmt)

#ifdef __cplusplus
}
#endif
#endif

