/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_sys_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/1/31
  Description   : hi3511 system private Head File
  History       :
  1.Date        : 2007/1/31
    Author      : c42025
    Modification: Created file

******************************************************************************/
#ifndef  __DRV_SYS_IOCTL_H__
#define  __DRV_SYS_IOCTL_H__

#ifdef __cplusplus
extern "C"{
#endif /* End of #ifdef __cplusplus */

#include "hi_debug.h"

#if defined(CHIP_TYPE_hi3712)
#define HI_DOLBY_REG    0x10180084
#define HI_DOLBY_BIT    (1 << 21)
#elif defined(CHIP_TYPE_hi3716cv200es)
#define HI_DOLBY_REG    0xf8a201e0
#define HI_DOLBY_BIT    (1)
#else
#define HI_DOLBY_REG    0x10180084
#define HI_DOLBY_BIT    (1 << 21)
#endif

typedef enum hiIOC_NR_SYS_E
{
    IOC_NR_SYS_INIT = 0,
    IOC_NR_SYS_EXIT,
    IOC_NR_SYS_SETCONFIG,
    IOC_NR_SYS_GETCONFIG,
    IOC_NR_SYS_GETVERSION,
    IOC_NR_SYS_GETTIMESTAMPMS
} IOC_NR_SYS_E;


#define SYS_INIT_CTRL         _IO (HI_ID_SYS, IOC_NR_SYS_INIT)
#define SYS_EXIT_CTRL         _IO (HI_ID_SYS, IOC_NR_SYS_EXIT)
#define SYS_SET_CONFIG_CTRL   _IOW(HI_ID_SYS, IOC_NR_SYS_SETCONFIG, HI_SYS_CONF_S)
#define SYS_GET_CONFIG_CTRL   _IOR(HI_ID_SYS, IOC_NR_SYS_GETCONFIG, HI_SYS_CONF_S)
#define SYS_GET_SYS_VERSION   _IOR(HI_ID_SYS, IOC_NR_SYS_GETVERSION, HI_SYS_VERSION_S)
#define SYS_GET_TIMESTAMPMS   _IOR(HI_ID_SYS, IOC_NR_SYS_GETTIMESTAMPMS, HI_U32)


/* Define Debug Level For SYS */
#define HI_FATAL_SYS(fmt...) HI_FATAL_PRINT(HI_ID_SYS, fmt)
#define HI_ERR_SYS(  fmt...) HI_ERR_PRINT(HI_ID_SYS, fmt)
#define HI_WARN_SYS( fmt...) HI_WARN_PRINT(HI_ID_SYS, fmt)
#define HI_INFO_SYS( fmt...) HI_INFO_PRINT(HI_ID_SYS, fmt)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_SYS_IOCTL_H__ */
