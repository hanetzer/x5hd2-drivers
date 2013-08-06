/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_otp_v200.c
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
#include <linux/poll.h>
#include "hi_kernel_adapt.h"
#include "hi_type.h"
#include "drv_otp.h"
#include "drv_otp_common.h"
#include "drv_otp_reg_v200.h"
#include "drv_otp_v200.h"
#ifdef SDK_OTP_ARCH_VERSION_V3
#include "drv_otp_ext.h"
#else
#include "otp_drv.h"
#endif

static HI_U32 DRV_OTP_GetOTPTimeValue(HI_VOID)
{
	HI_U32 u32Value = 0;

	#ifdef CHIP_TYPE_hi3712
	    u32Value = 0x33f;		//OTP_FREQ_24M
	#else
		u32Value = 0x34f;		//OTP_FREQ_28_8M
	#endif
	
	return u32Value;
}

HI_DECLARE_MUTEX(g_OtpV200Mutex);

#define DRV_OTPV200_LOCK() do{									\
    	HI_S32 s32Ret = 0;									\
    	s32Ret = down_interruptible(&g_OtpV200Mutex);		\
	    if (0 != s32Ret)									\
	    {													\
	        HI_FATAL_OTP("Down_interruptible error!\n"); 	\
	        return -1;										\
	    }													\
    }while(0)
    
#define DRV_OTPV200_UNLOCK() do{		\
		up(&g_OtpV200Mutex);		\
	}while(0)

HI_U32 do_apb_v200_read(HI_U32 addr)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_RADDR_U       RAddr;
    OTP_V200_RDATA_U       Redata;
    HI_U32 u32OTPTimeValue = 0;
    
    DRV_OTPV200_LOCK();    
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 0, set rd_enable to 1, read can only do by word
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x00;
    CPURWCtrl.bits.rd_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x02;    
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);

    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);
    u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;
    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_RADDR
    RAddr.u32 = 0;
    RAddr.bits.raddr = addr;
    otp_write_reg(OTP_V200_RADDR, RAddr.u32);
    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    Redata.u32 = otp_read_reg(OTP_V200_RDATA);
    DRV_OTPV200_UNLOCK();
    
    return Redata.u32;
}

HI_U8 do_apb_v200_read_byte(HI_U32 addr)
{
    HI_U32 Value = 0;
    HI_U32 readableAddr = 0;

    readableAddr = addr & (~0x3);
    Value = do_apb_v200_read(readableAddr);
    return (Value >> ((addr & 0x3)*8)) & 0xff ;
//    return (HI_U8)(Value & 0xff);    
}

HI_S32  do_apb_v200_write(HI_U32 addr, HI_U32 tdata)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_WADDR_U       WAddr;
    OTP_V200_WDATA_U       WDATA;
    OTP_V200_WR_START_U    WRStart;
	HI_U32 u32OTPTimeValue = 0;
    
    DRV_OTPV200_LOCK();
    
    /* check if addr is 4bytes aligned or not */
    if( 0 != (addr & 0x3))
    {
        HI_FATAL_OTP("Addr must be aligned with 4 Bytes!\n");
    	DRV_OTPV200_UNLOCK();
    	return HI_FAILURE;
    }
    
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 1, set wr_enable to 1, set cpu_size to 2'b10(Word Operation)
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x01;
    CPURWCtrl.bits.wr_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x02;    
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);
    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);
	
	u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;
    
    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_WADDR
    WAddr.u32 = 0;
    WAddr.bits.waddr = addr;
    otp_write_reg(OTP_V200_WADDR, WAddr.u32);
    //Set OTP_V200_WDATA
    WDATA.bits.wdata = tdata;
    otp_write_reg(OTP_V200_WDATA, WDATA.u32);
    //Set WR_START bit start to 1
    WRStart.u32 = otp_read_reg(OTP_V200_WR_START);
    WRStart.bits.start = 1;
    otp_write_reg(OTP_V200_WR_START, WRStart.u32);
    
    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }

    if (0 != CtrlStaut.bits.fail_flag)
    {
        HI_FATAL_OTP("Write OTP failed!\n");
        DRV_OTPV200_UNLOCK();
        return HI_FAILURE;
    }

    DRV_OTPV200_UNLOCK();
	
    return HI_SUCCESS;
}

HI_S32 do_apb_v200_write_byte(HI_U32 addr, HI_U8 tdata)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_WADDR_U       WAddr;
    OTP_V200_WDATA_U       WDATA;
    OTP_V200_WR_START_U    WRStart;
	HI_U32 u32OTPTimeValue = 0;
    
    DRV_OTPV200_LOCK();    
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 1, set wr_enable to 1, set cpu_sizeΪ2'b01(Byte operation)
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x01;
    CPURWCtrl.bits.wr_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x01;    
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);
    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);
    
    u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;
    
    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_WADDR
    WAddr.u32 = 0;
    WAddr.bits.waddr = addr;
    otp_write_reg(OTP_V200_WADDR, WAddr.u32);
    //Set OTP_V200_WDATA
    WDATA.bits.wdata = (HI_U32)tdata;
    otp_write_reg(OTP_V200_WDATA, WDATA.u32);
    //Set WR_START bit start to 1
    WRStart.u32 = otp_read_reg(OTP_V200_WR_START);
    WRStart.bits.start = 1;
    otp_write_reg(OTP_V200_WR_START, WRStart.u32);
    
    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }

    if (0 != CtrlStaut.bits.fail_flag)
    {
        HI_FATAL_OTP("Write OTP failed!\n");
        DRV_OTPV200_UNLOCK();
        return HI_FAILURE;
    }

    DRV_OTPV200_UNLOCK();

    return HI_SUCCESS;
}

HI_S32  do_apb_v200_write_bit(HI_U32 addr, HI_U32 bit_pos, HI_U32 bit_value)
{
    HI_U8 u8Data = 0;

    if(bit_value == 1)
    {
        u8Data = do_apb_v200_read_byte(addr);
        u8Data |= (1 << bit_pos);

        return do_apb_v200_write_byte(addr, u8Data);
    }
    else
    {
        //Do nothing when bit_value is 0.
    }
    return HI_SUCCESS;
}

EXPORT_SYMBOL(do_apb_v200_read);
EXPORT_SYMBOL(do_apb_v200_read_byte);
EXPORT_SYMBOL(do_apb_v200_write);
EXPORT_SYMBOL(do_apb_v200_write_byte);
EXPORT_SYMBOL(do_apb_v200_write_bit);

/*-------------------------------------END--------------------------------------*/

