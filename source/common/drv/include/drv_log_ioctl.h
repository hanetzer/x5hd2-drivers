/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_log_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/03/10
  Description   :
  History       :
  1.Date        : 2006/03/10
    Author      : f47391
    Modification: Created file

******************************************************************************/

#ifndef __DRV_LOG_IOCTL_H__
#define __DRV_LOG_IOCTL_H__

#ifdef __cplusplus
extern "C"{
#endif /* End of #ifdef __cplusplus */

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_log.h"

#define LOG_MAX_TRACE_LEN 256

/*define the buffer size for log*/
#define DEBUG_MSG_BUF_RESERVE   (1024 * 4)
#define LOG_CONFIG_BUF_SIZE   (1024 * 8)

typedef enum hiLOG_OUTPUT_POS_E
{
    LOG_OUTPUT_SERIAL,
    LOG_OUTPUT_NETWORK,
    LOG_OUTPUT_UDISK,

    LOG_OUTPUT_BUTT
}LOG_OUTPUT_POS_E;


#ifdef LOG_UDISK_SUPPORT
#define LOG_OUTPUT_POS_DEFAULT (LOG_OUTPUT_UDISK)
#else
#define LOG_OUTPUT_POS_DEFAULT (LOG_OUTPUT_SERIAL)
#endif

/*structure of mode log level */
/*CNcomment: 模块打印级别控制信息结构 */
typedef struct hiLOG_CONFIG_INFO_S
{
    HI_U32 u32LogLevel;         /*log level*//*CNcomment:  模块打印级别控制 */
    HI_U32 u32LogPrintPos;      /*log output location, 0:serial port; 1:network;2:u-disk*//*CNcomment:  模块打印位置控制 0:串口 1:网络 2:U盘 */
    // TODO: Check length
    HI_U8 ModName[16];          /*mode name*/
    HI_BOOL bUdiskFlag;         /* u-disk log flag */
}LOG_CONFIG_INFO_S;

typedef struct hiLOG_BUF_READ_S
{
    HI_U8           *pMsgAddr;
    HI_U32          BufLen;
    HI_U32          CopyedLen;
}LOG_BUF_READ_S;

typedef struct hiLOG_BUF_WRITE_S
{
    HI_U8           *pMsgAddr;
    HI_U32          MsgLen;
}LOG_BUF_WRITE_S;


#define UMAP_CMPI_LOG_INIT              _IOR  (HI_ID_LOG, 0, HI_U32)
#define UMAP_CMPI_LOG_EXIT              _IO   (HI_ID_LOG, 1)
#define UMAP_CMPI_LOG_READ_LOG          _IOWR (HI_ID_LOG, 2, LOG_BUF_READ_S)
#define UMAP_CMPI_LOG_WRITE_LOG         _IOW  (HI_ID_LOG, 3, LOG_BUF_WRITE_S)
#define UMAP_CMPI_LOG_SET_PATH          _IOW  (HI_ID_LOG, 4, LOG_PATH_S)
#define UMAP_CMPI_LOG_SET_STORE_PATH    _IOW  (HI_ID_LOG, 5, STORE_PATH_S)



/*Define Debug Level For LOG                 */
#define HI_FATAL_LOG(fmt...)  HI_FATAL_PRINT(HI_ID_LOG, fmt)

#define HI_ERR_LOG(fmt...) HI_ERR_PRINT(HI_ID_LOG, fmt)

#define HI_WARN_LOG(fmt...) HI_WARN_PRINT(HI_ID_LOG, fmt)
#define HI_INFO_LOG(fmt...) HI_INFO_PRINT(HI_ID_LOG, fmt)


#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __DRV_LOG_IOCTL_H__ */

