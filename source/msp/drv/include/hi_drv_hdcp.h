/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_hdcp.h
  Version       : Initial Draft
  Author        : advca
  Created       : 2013/04/12
  Description   : 
  History       :
  1.Date        : 2013/04/12
    Author      : z00213260
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_HDCP_H__
#define __HI_DRV_HDCP_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define HI_FATAL_HDCP(fmt...)    HI_FATAL_PRINT  (HI_ID_HDCP, fmt)
#define HI_ERR_HDCP(fmt...)      HI_ERR_PRINT    (HI_ID_HDCP, fmt)
#define HI_WARN_HDCP(fmt...)     HI_WARN_PRINT   (HI_ID_HDCP, fmt)
#define HI_INFO_HDCP(fmt...)     HI_INFO_PRINT   (HI_ID_HDCP, fmt)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

