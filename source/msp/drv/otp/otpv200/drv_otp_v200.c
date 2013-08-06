/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : otp_drv_v200.c
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
#include "drv_otp_common.h"
#include "drv_otp_reg_v200.h"
#include "drv_otp_v200.h"
#ifdef SDK_OTP_ARCH_VERSION_V3
#include "hi_error_mpi.h"
#include "drv_otp_ext.h"
#else
#include "otp_drv.h"
#endif

#define OTP_CUSTOMER_KEY_ADDR    0x2c0
#define OTP_STB_PRIV_DATA_ADDR    0x2b0

HI_S32 OTP_V200_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    HI_S32 ret = HI_SUCCESS;
	HI_U32 i;
    HI_U8 *pu8CustomerKey = NULL;
    HI_U32 pu32CustomerKey[4] = {0};

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    for(i = 0; i < 4; i++)
    {
        pu32CustomerKey[i] = do_apb_v200_read(OTP_CUSTOMER_KEY_ADDR + i * 4);
        if (0x0 != pu32CustomerKey[i])
        {
            break;
        }
    }
    
    if (i >= 4)
    {
        pu8CustomerKey = (HI_U8*)(pCustomerKey->u32CustomerKey);
        for(i = 0; i < 16; i++)
        {
            ret = do_apb_v200_write_byte(OTP_CUSTOMER_KEY_ADDR + i, pu8CustomerKey[i]);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_OTP("ERROR: Set Customer Key Error!\n");
                return ret;
            }
        }
    }
    else
    {
        HI_ERR_OTP("ERROR: Customer Key already set!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V200_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
	HI_U32 i;
    HI_U32 *pu32CustomerKey = NULL;

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    pu32CustomerKey = (HI_U32*)(pCustomerKey->u32CustomerKey);
    for(i = 0; i < 4; i++)
    {
        pu32CustomerKey[i] = do_apb_v200_read(OTP_CUSTOMER_KEY_ADDR + i * 4);
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V200_Get_DDPLUS_Flag(HI_BOOL *pu32DDPlusFlag)
{
	HI_S32    ret = HI_SUCCESS;
	OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32DDPlusFlag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }
    
	PV_1.u32 = do_apb_v200_read(OTP_V200_INTERNAL_PV_1);

    if (1 == PV_1.bits.dolby_flag)  /*0:support DDPLUS(default); 1: do not support DDPLUS*/
    {
        *pu32DDPlusFlag = HI_FALSE;
    }
    else
    {
        *pu32DDPlusFlag = HI_TRUE;
    }

    return ret;
}

HI_S32 OTP_V200_Get_DTS_Flag(HI_BOOL *pu32DTSFlag)
{
	HI_S32    ret = HI_SUCCESS;
	OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32DTSFlag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }
    
	PV_1.u32 = do_apb_v200_read(OTP_V200_INTERNAL_PV_1);

    if (1 == PV_1.bits.dts_flag)  /*0:do not support DTS(default); 1: support DTS*/
    {
        *pu32DTSFlag = HI_TRUE;
    }
    else
    {
        *pu32DTSFlag = HI_FALSE;
    }

    return ret;
}


HI_S32 OTP_V200_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    return do_apb_v200_write_byte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset, pStbPrivData->u8Data);
}

HI_S32 OTP_V200_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    pStbPrivData->u8Data = do_apb_v200_read_byte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset);

    return HI_SUCCESS;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_SetHdcpRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U8     u8HdcpOTPKey[OTP_HDCP_ROOT_KEY_LEN] = {0};
    HI_BOOL OTPMatchingFlag = HI_TRUE;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if locked or not */
    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.esck_lock)
    {
        HI_ERR_OTP("HDCPKEY set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
	for (i = 0; i < OTP_HDCP_ROOT_KEY_LEN; i++)
	{
	    do_apb_v200_write_byte(OTP_V200_INTERNAL_ESCK_0 + i, pu8Key[i]);
	}

    /* read from otp */
    for ( i = 0 ; i < OTP_HDCP_ROOT_KEY_LEN ; i++ )
	{
        u8HdcpOTPKey[i] = (HI_U8)do_apb_v200_read_byte(OTP_V200_INTERNAL_ESCK_0 + i);
	}
    
    /* Compare HDCP key */
    OTPMatchingFlag = HI_TRUE;
	for(i = 0; i < OTP_HDCP_ROOT_KEY_LEN; i ++)
	{
	    if(u8HdcpOTPKey[i] != pu8Key[i])
	    {
	        OTPMatchingFlag = HI_FALSE; //not equal£¡
	        break;
	    }
	}

    /* Check Compare result! */
	if(HI_FALSE == OTPMatchingFlag)
	{
	    HI_ERR_OTP("Error: Burn HDCP Root key Error\n");
	    ret = HI_FAILURE;
	}

	return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetHdcpRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.esck_lock)
    {
        HI_ERR_OTP("HDCP Root KEY get ERROR! Hdcp root key lock!\n");
        return HI_FAILURE;
    }
    
    /* read from OTP */
	for (i = 0; i < OTP_HDCP_ROOT_KEY_LEN; i++)
	{
	    pu8Key[i] = do_apb_v200_read_byte(OTP_V200_INTERNAL_ESCK_0 + i);
	}

	return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_SetHdcpRootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.esck_lock = 1;
    do_apb_v200_write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);

	return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetHdcpRootKeyLock(HI_BOOL *pBLock)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pBLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    *pBLock = DataLock_0.bits.esck_lock;

	return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_SetSTBRootKey(HI_U8 u8Key[16])
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U8     u8STBRootKey[16] = {0};
    HI_BOOL OTPMatchingFlag = HI_TRUE;

    if (NULL == u8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if locked or not */
    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.stb_rootkey_lock)
    {
        HI_ERR_OTP("STB root key set ERROR! Is locked!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
	for (i = 0; i < OTP_STB_ROOT_KEY_LEN; i++)
	{
	    do_apb_v200_write_byte(OTP_V200_INTERNAL_STB_ROOTKEY_0 + i, u8Key[i]);
	}

    /* read from otp */
    for ( i = 0 ; i < OTP_STB_ROOT_KEY_LEN ; i++ )
	{
        u8STBRootKey[i] = (HI_U8)do_apb_v200_read_byte(OTP_V200_INTERNAL_STB_ROOTKEY_0 + i);
	}
    
    /* Compare */
    OTPMatchingFlag = HI_TRUE;
	for(i = 0; i < OTP_STB_ROOT_KEY_LEN; i ++)
	{
	    if(u8STBRootKey[i] != u8Key[i])
	    {
	        OTPMatchingFlag = HI_FALSE;
	        break;
	    }
	}

    /* Check Compare result! */
	if(HI_FALSE == OTPMatchingFlag)
	{
	    HI_ERR_OTP("Error: Burn STB root key Error\n");
	    ret = HI_FAILURE;
	}

	return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetSTBRootKey(HI_U8 u8Key[16])
{
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == u8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.stb_rootkey_lock)
    {
        HI_ERR_OTP("STB root key get ERROR! Is locked!\n");
        return HI_FAILURE;
    }
    
    /* read from OTP */
	for (i = 0; i < OTP_STB_ROOT_KEY_LEN; i++)
	{
	    u8Key[i] = do_apb_v200_read_byte(OTP_V200_INTERNAL_STB_ROOTKEY_0 + i);
	}

	return HI_SUCCESS;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_LockSTBRootKey(HI_VOID)
{
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.stb_rootkey_lock = 1;
    do_apb_v200_write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    
	return HI_SUCCESS;
}

HI_S32 OTP_V200_GetSTBRootKeyLockFlag(HI_BOOL *pBLock)
{
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pBLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = do_apb_v200_read(OTP_V200_INTERNAL_DATALOCK_0);
    *pBLock = DataLock_0.bits.stb_rootkey_lock;

	return HI_SUCCESS;
}

HI_S32 OTP_V200_Reset(HI_VOID)
{
#ifndef CHIP_TYPE_hi3716cv200es
    OTP_V200_CRG_HDMI_CTRL_U OtpV200CrgCtrl;
#else
    OTP_V200_CRG_CA_CTRL_U OtpV200CrgCtrl;
#endif
    
    OtpV200CrgCtrl.u32 = 0;

/* Reset */
#ifndef CHIP_TYPE_hi3716cv200es
    OtpV200CrgCtrl.u32 = otp_read_reg(OTP_V200_CRG_HDMI_ADDR);
    OtpV200CrgCtrl.bits.otp_reset = 1;
    otp_write_reg(OTP_V200_CRG_HDMI_ADDR, OtpV200CrgCtrl.u32);
#else
    OtpV200CrgCtrl.u32 = otp_read_reg(OTP_V200_CRG_CA_ADDR);
    OtpV200CrgCtrl.bits.otp_srst_req = 1;
    otp_write_reg(OTP_V200_CRG_CA_ADDR, OtpV200CrgCtrl.u32);
#endif

    otp_wait(1000);

/* Cancel Reset */
#ifndef CHIP_TYPE_hi3716cv200es
    OtpV200CrgCtrl.u32 = otp_read_reg(OTP_V200_CRG_HDMI_ADDR);
    OtpV200CrgCtrl.bits.otp_reset = 0;
    otp_write_reg(OTP_V200_CRG_HDMI_ADDR, OtpV200CrgCtrl.u32);
#else
    OtpV200CrgCtrl.u32 = otp_read_reg(OTP_V200_CRG_CA_ADDR);
    OtpV200CrgCtrl.bits.otp_srst_req = 0;
    otp_write_reg(OTP_V200_CRG_CA_ADDR, OtpV200CrgCtrl.u32);
#endif

	return HI_SUCCESS;
}
/*--------------------------------------END--------------------------------------*/

EXPORT_SYMBOL(OTP_V200_Reset);
