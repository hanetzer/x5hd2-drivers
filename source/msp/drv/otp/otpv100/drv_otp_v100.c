/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : otp_drv_v100.c
  Version       : Initial Draft
  Author        : 
  Created       : 
  Last Modified :
  Description   : OTP REG DEFINE
  Function List :
  History       :
******************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <asm/delay.h>
#include <linux/kernel.h>
#include <mach/platform.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/memory.h>
#include "hi_type.h"
#include "drv_otp.h"
#ifdef SDK_OTP_ARCH_VERSION_V3
#include "drv_otp_ext.h"
#else
#include "otp_drv.h"
#endif
#include "drv_otp_common.h"
#include "drv_otp_reg_v100.h"
#include "drv_otp_v100.h"

#define OTP_CUSTOMER_KEY_ADDR    0x1C0
#define OTP_AUDIO_SUPPORT_ADDR   0x1F
#define OTP_STB_PRIV_DATA_ADDR    0x1D0

#define OTP_DOLBY_SUPPORT_BIT   (1 << 0)
#define OTP_DTS_SUPPORT_BIT         (1 << 1)

#define otp_rd_u32(addr, val) do { \
        val = do_apb_para_read(addr); \
    } while (0)

#define otp_wt_u32(addr, val) do { \
        HI_S32 Ret = HI_SUCCESS; \
        unsigned char *ptr = (unsigned char *)&val; \
        Ret = do_apb_write_byte((addr + 0), ptr[0]); \
        Ret |= do_apb_write_byte((addr + 1), ptr[1]); \
        Ret |= do_apb_write_byte((addr + 2), ptr[2]); \
        Ret |= do_apb_write_byte((addr + 3), ptr[3]); \
        if (HI_SUCCESS != Ret) \
        { \
            HI_ERR_OTP("Fail to write OTP\n"); \
            return Ret; \
        } \
    } while (0)

#define otp_wt_u8(addr, val) do { \
        HI_S32 Ret = HI_SUCCESS; \
        Ret = do_apb_write_byte(addr, val); \
        if (HI_SUCCESS != Ret) \
        { \
            HI_ERR_OTP("Fail to write OTP\n"); \
            return Ret; \
        } \
    } while (0)

#define otp_wt_Bit(addr, pos, val) do { \
        HI_S32 Ret = HI_SUCCESS; \
        Ret = do_apb_write_bit(addr, pos, val); \
        if (HI_SUCCESS != Ret) \
        { \
            HI_ERR_OTP("Fail to write OTP\n"); \
            return Ret; \
        } \
    } while (0)

HI_S32 OTP_V100_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    HI_U32 i;
    HI_U8 *pu8CustomerKey = NULL;

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    pu8CustomerKey = (HI_U8*)(pCustomerKey->u32CustomerKey);
    for(i = 0; i < 16; i++)
    {
        otp_wt_u8(OTP_CUSTOMER_KEY_ADDR + i, pu8CustomerKey[i]);
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V100_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    HI_U32 i;
    HI_U32 *pu8CustomerKey = NULL;

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    pu8CustomerKey = (HI_U32*)(pCustomerKey->u32CustomerKey);
    for(i = 0; i < 4; i++)
    {
        otp_rd_u32(OTP_CUSTOMER_KEY_ADDR + i * 4, pu8CustomerKey[i]);
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V100_Func_Disable(HI_U32 u32SRBit)
{
    return otp_set_sr_bit(u32SRBit);
}

HI_S32 OTP_V100_Get_DDPLUS_Flag(HI_BOOL *pDDPLUSFlag)
{
    HI_U32 u32DDPLUSFlag;

    if(NULL == pDDPLUSFlag)
    {
        return HI_FAILURE;
    }

    otp_rd_u32(OTP_AUDIO_SUPPORT_ADDR - 3, u32DDPLUSFlag); //Should align
    if((u32DDPLUSFlag >> 24) & OTP_DOLBY_SUPPORT_BIT)
    {
        *pDDPLUSFlag = HI_TRUE;  //1:support dolby(default)
    }
    else
    {
        *pDDPLUSFlag = HI_FALSE;  //0: not support dolby(default)
    }
    
    return HI_SUCCESS;
}

HI_S32 OTP_V100_Get_DTS_Flag(HI_BOOL *pDTSFlag)
{
    HI_U32 u32DTSFlag;

    if(NULL == pDTSFlag)
    {
        return HI_FAILURE;
    }

    otp_rd_u32(OTP_AUDIO_SUPPORT_ADDR - 3, u32DTSFlag);
    if ((u32DTSFlag >> 24) & OTP_DTS_SUPPORT_BIT)//1:do not support DTS(default) 0:support DTS
    {
        *pDTSFlag = HI_FALSE;
    }
    else
    {
        *pDTSFlag = HI_TRUE;
    }
    return HI_SUCCESS;
}

HI_S32 OTP_V100_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    otp_wt_u8(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset, pStbPrivData->u8Data);

    return HI_SUCCESS;
}

HI_S32 OTP_V100_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    HI_U32 u32TmpValue;

    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    otp_rd_u32(OTP_STB_PRIV_DATA_ADDR + (pStbPrivData->u32Offset / 4) * 4, u32TmpValue);
    pStbPrivData->u8Data = (HI_U8)(u32TmpValue >> ((pStbPrivData->u32Offset % 4) * 8));

    return HI_SUCCESS;
}

/*--------------------------------------END--------------------------------------*/
