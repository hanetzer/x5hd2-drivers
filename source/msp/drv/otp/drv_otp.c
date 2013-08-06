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
#include "drv_otp_ext.h"
#include "drv_otp_common.h"
#include "drv_otp_v100.h"
#include "drv_otp_v200.h"
#include "drv_otp_reg_v200.h"
#include "drv_dev_ext.h"
#include "drv_module_ext.h"
#include "drv_cipher_ext.h"

#define OTP_NAME "HI_OTP"

static OTP_RegisterFunctionlist_S s_OTPExportFunctionList =
{
	.do_apb_v200_read 		=	do_apb_v200_read,
	.do_apb_v200_read_byte	=   do_apb_v200_read_byte,
	.do_apb_v200_write		=   do_apb_v200_write,
	.do_apb_v200_write_byte	=   do_apb_v200_write_byte,
	.do_apb_v200_write_bit	=   do_apb_v200_write_bit,
	.do_apb_write_byte		=   do_apb_write_byte,
	.do_apb_write_bit		=   do_apb_write_bit,
	.set_apb_write_protect	=   set_apb_write_protect,
	.get_apb_write_protect	=   get_apb_write_protect,
	.do_apb_para_read		=   do_apb_para_read,
	.otp_get_sr_bit			=   otp_get_sr_bit,
	.otp_set_sr_bit			=   otp_set_sr_bit,
	.otp_reset				=   otp_reset,
	.otp_func_disable		=   otp_func_disable,
};

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

static OTP_VERSION_E gOTPVesion = OTP_VERSION_100;

HI_S32 DRV_OTP_Init(void)
{    
	HI_S32 ret = HI_SUCCESS;
    HI_CHIP_TYPE_E enChip;
    HI_CHIP_VERSION_E enChipVersion;
    
    ret = HI_DRV_MODULE_Register(HI_ID_OTP, OTP_NAME, (HI_VOID*)&s_OTPExportFunctionList);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_OTP("HI_DRV_MODULE_Register otp failed\n");
        return ret;
    }
    
    HI_DRV_SYS_GetChipVersion(&enChip, &enChipVersion);
    if(((HI_CHIP_TYPE_HI3712 == enChip) && (HI_CHIP_VERSION_V100 == enChipVersion)) ||
        ((HI_CHIP_TYPE_HI3716M == enChip) && (HI_CHIP_VERSION_V300 == enChipVersion)) ||
        ((HI_CHIP_TYPE_HI3716CES == enChip) && (HI_CHIP_VERSION_V200 == enChipVersion)))
    {
        gOTPVesion = OTP_VERSION_200;
    }
    else
    {
        gOTPVesion = OTP_VERSION_100;
    }
	
    return HI_SUCCESS;
}

HI_S32 DRV_OTP_DeInit(void)
{
    HI_DRV_MODULE_UnRegister(HI_ID_OTP);
    return HI_SUCCESS;
}

HI_S32 DRV_OTP_Read(HI_U32 Addr)
{
    if(gOTPVesion == OTP_VERSION_200)
    {
        return do_apb_v200_read(Addr);
    }
    else
    {
        return do_apb_para_read(Addr);
    }
}

HI_S32 DRV_OTP_Write(HI_U32 Addr, HI_U32 value)
{
    HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        HI_U8 *pByte = (HI_U8*)&value;
        ErrorReturn = do_apb_v200_write_byte((Addr + 0), pByte[0]); 
        ErrorReturn |= do_apb_v200_write_byte((Addr + 1), pByte[1]); 
        ErrorReturn |= do_apb_v200_write_byte((Addr + 2), pByte[2]); 
        ErrorReturn |= do_apb_v200_write_byte((Addr + 3), pByte[3]);
    }
    else
    {
        HI_U8 *pByte = (HI_U8*)&value;
        ErrorReturn = do_apb_write_byte((Addr + 0), pByte[0]); 
        ErrorReturn |= do_apb_write_byte((Addr + 1), pByte[1]); 
        ErrorReturn |= do_apb_write_byte((Addr + 2), pByte[2]); 
        ErrorReturn |= do_apb_write_byte((Addr + 3), pByte[3]);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Write_Byte(HI_U32 Addr, HI_U8 value)
{
    HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = do_apb_v200_write_byte(Addr, value);
    }
    else
    {
        ErrorReturn = do_apb_write_byte(Addr, value);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Write_Bit(HI_U32 Addr, HI_U32 BitPos, HI_U32 BitValue)
{
    HI_S32 ErrorReturn = HI_SUCCESS;

	if (BitPos >= 8)
	{
		HI_ERR_OTP("Write OTP bit ERROR! BitPos >= 8\n");
		return HI_FAILURE;
	}
	
	if (BitValue > 1)
	{
		HI_ERR_OTP("Write OTP bit ERROR! BitValue > 1\n");
		return HI_FAILURE;
	}
	
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = do_apb_v200_write_bit(Addr, BitPos, BitValue);
    }
    else
    {
        ErrorReturn = do_apb_write_bit(Addr, BitPos, BitValue);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Reset(HI_VOID)
{
    HI_S32 ErrorReturn = HI_SUCCESS;

    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = OTP_V200_Reset();
    }
    else
    {
        ErrorReturn = otp_reset();
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Func_Disable(HI_U32 u32SRBit)
{
	HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        /*Not supported on OTP V200*/
    }
    else
    {
        ErrorReturn = OTP_V100_Func_Disable(u32SRBit);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
	HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = OTP_V200_Set_CustomerKey(pCustomerKey);
    }
    else
    {
        ErrorReturn = OTP_V100_Set_CustomerKey(pCustomerKey);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
	HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = OTP_V200_Get_CustomerKey(pCustomerKey);
    }
    else
    {
        ErrorReturn = OTP_V100_Get_CustomerKey(pCustomerKey);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Get_DDPLUS_Flag(HI_BOOL *pDDPLUSFlag)
{
	HI_S32 ErrorReturn = HI_SUCCESS;
	
	if(gOTPVesion == OTP_VERSION_200)
	{
		ErrorReturn = OTP_V200_Get_DDPLUS_Flag(pDDPLUSFlag);
	}
	else
	{
		ErrorReturn = OTP_V100_Get_DDPLUS_Flag(pDDPLUSFlag);
	}
	
	return ErrorReturn;
}

HI_S32 DRV_OTP_Get_DTS_Flag(HI_BOOL *pDTSFlag)
{
	HI_S32 ErrorReturn = HI_SUCCESS;
	
	if(gOTPVesion == OTP_VERSION_200)
	{
		ErrorReturn = OTP_V200_Get_DTS_Flag(pDTSFlag);
	}
	else
	{
		ErrorReturn = OTP_V100_Get_DTS_Flag(pDTSFlag);
	}
	
	return ErrorReturn;
}

HI_S32 DRV_OTP_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = OTP_V200_Set_StbPrivData(pStbPrivData);
    }
    else
    {
        ErrorReturn = OTP_V100_Set_StbPrivData(pStbPrivData);
    }
    
    return ErrorReturn;
}

HI_S32 DRV_OTP_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    HI_S32 ErrorReturn = HI_SUCCESS;
    
    if(gOTPVesion == OTP_VERSION_200)
    {
        ErrorReturn = OTP_V200_Get_StbPrivData(pStbPrivData);
    }
    else
    {
        ErrorReturn = OTP_V100_Get_StbPrivData(pStbPrivData);
    }
    
    return ErrorReturn;
}

/*---------------------------------------------END--------------------------------------*/
