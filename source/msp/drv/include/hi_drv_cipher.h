/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/03
  Description   : 
  History       :
  1.Date        : 2013/04/03
    Author      : l00185424
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_CIPHER_H__
#define __HI_DRV_CIPHER_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/***************************** Macro Definition ******************************/
#define CIPHER_SOFT_CHAN_NUM      CIPHER_CHAN_NUM

#define CIPHER_INVALID_CHN        (0xffffffff)

#define HI_FATAL_CIPHER(fmt...)             HI_FATAL_PRINT  (HI_ID_CIPHER, fmt)
#define HI_ERR_CIPHER(fmt...)               HI_ERR_PRINT    (HI_ID_CIPHER, fmt)
#define HI_WARN_CIPHER(fmt...)              HI_WARN_PRINT   (HI_ID_CIPHER, fmt)
#define HI_INFO_CIPHER(fmt...)              HI_INFO_PRINT   (HI_ID_CIPHER, fmt)
#define HI_DEBUG_CIPHER(fmt...)             HI_DBG_PRINT    (HI_ID_CIPHER, fmt)

/***/
typedef struct hiCIPHER_DATA_INFO_S
{
    HI_U32  u32src;          /**< */
    HI_U32  u32dest;         /**< */
    HI_U32  u32length;       /**< */
    HI_BOOL bDecrypt;        /**< */
    HI_U32  u32DataPkg[4];   /**< */
}HI_DRV_CIPHER_DATA_INFO_S;

typedef struct hiCIPHER_TASK_S
{
    HI_DRV_CIPHER_DATA_INFO_S         	stData2Process;
    HI_U32                      u32CallBackArg;
}HI_DRV_CIPHER_TASK_S;

typedef enum hiCIPHER_HDCP_MODE_E
{
    CIPHER_HDCP_MODE_NO_HDCP_KEY                = 0x0,
    CIPHER_HDCP_MODE_HDCP_KEY         			= 0x1,
    CIPHER_HDCP_MODE_BUTT,
}HI_DRV_CIPHER_HDCP_MODE_E;

typedef enum hiCIPHER_HDCP_KEY_RAM_MODE_E
{
    CIPHER_HDCP_KEY_RAM_MODE_READ         		= 0x0,
    CIPHER_HDCP_KEY_RAM_MODE_WRITE       		= 0x1,
    CIPHER_HDCP_KEY_RAM_MODE_BUTT,
}HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E;

typedef enum hiCIPHER_HDCP_KEY_TYPE_E
{
    CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY           = 0x0,
    CIPHER_HDCP_KEY_TYPE_HISI_DEFINED           = 0x1,               
    CIPHER_HDCP_KEY_TYPE_HDCP_HOST_ROOT_KEY 	= 0x2,
    CIPHER_HDCP_KEY_TYPE_BUTT                   = 0x3,
}HI_DRV_CIPHER_HDCP_KEY_TYPE_E;

typedef struct hiCIPHER_FLASH_ENCRYPT_HDCPKEY_S
{
    HI_U8 u8Key[332];
}HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S;



#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* End of #ifndef __HI_DRV_CIPHER_H__*/

