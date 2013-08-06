/**
\file
\brief DRV_OTP
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author  x57522
\date 2010-12-21
*/

#ifndef __DRV_OTP_EXT_H__
#define __DRV_OTP_EXT_H__

//#include "drv_otp_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* otp v200 hal interface */
typedef HI_U32 (*fp_do_apb_v200_read)(HI_U32 addr);
typedef HI_U8 (*fp_do_apb_v200_read_byte)(HI_U32 addr);
typedef HI_S32 (*fp_do_apb_v200_write)(HI_U32 addr,HI_U32 tdata);
typedef HI_S32 (*fp_do_apb_v200_write_byte)(HI_U32 addr, HI_U8 tdata);
typedef HI_S32 (*fp_do_apb_v200_write_bit)(HI_U32 addr, HI_U32 bit_pos, HI_U32  bit_value);

/* otp v100 hal interface */
typedef HI_S32 (*fp_do_apb_write_byte)(HI_U32 addr, HI_U32 tdata);
typedef HI_S32 (*fp_do_apb_write_bit)(HI_U32 addr, HI_U32 bit_pos, HI_U32 bit_value);
typedef HI_S32 (*fp_set_apb_write_protect)(HI_VOID);
typedef HI_S32 (*fp_get_apb_write_protect)(HI_U32 *penable);
typedef HI_U32 (*fp_do_apb_para_read)(HI_U32 addr);
typedef HI_VOID (*fp_otp_get_sr_bit)(HI_S32 pos,HI_S32 *pvalue);
typedef HI_S32 (*fp_otp_set_sr_bit)(HI_S32 pos);
typedef HI_S32 (*fp_otp_func_disable)(HI_U32 bit_pos, HI_U32 bit_value);
typedef HI_S32 (*fp_otp_reset)(void);

/* set hdcp key interface */
typedef HI_S32 (*fp_OTP_IOCTL_Set_HDCPKey)(HI_U8 *HDCPData);
typedef HI_S32 (*fp_OTP_IOCTL_Get_HDCPFlag)(HI_U32 *pu32HDCPKeyBurnFlag);

typedef struct s_OTP_RegisterFunctionlist
{
/* otpv200, read word */
    fp_do_apb_v200_read         do_apb_v200_read;
/* otpv200, allow not 4bytes alined */
    fp_do_apb_v200_read_byte    do_apb_v200_read_byte;
/* otpv200, write word, addr should be 4bytes alined */
    fp_do_apb_v200_write        do_apb_v200_write;
/* otpv200, write byte */
    fp_do_apb_v200_write_byte   do_apb_v200_write_byte;
/* otpv200, write bit, addr should be 4bytes alined */ 
    fp_do_apb_v200_write_bit    do_apb_v200_write_bit;
/* otpv100, write byte */
    fp_do_apb_write_byte        do_apb_write_byte;
/* otpv100, write bit */
    fp_do_apb_write_bit         do_apb_write_bit;
/* otpv100, set write protect */
    fp_set_apb_write_protect    set_apb_write_protect;
/* otpv100, get write protect */
    fp_get_apb_write_protect    get_apb_write_protect;
/* otpv100, read word */
    fp_do_apb_para_read         do_apb_para_read;
/* otpv100, get sr bit */
    fp_otp_get_sr_bit           otp_get_sr_bit;
/* otpv100, set sr bit */
    fp_otp_set_sr_bit           otp_set_sr_bit;
/* otpv100, function disable */
    fp_otp_func_disable         otp_func_disable;
/* otpv100, reset otp */
    fp_otp_reset				otp_reset;
}OTP_RegisterFunctionlist_S;

HI_S32	OTP_DRV_ModInit(HI_VOID);
HI_VOID	OTP_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
}
#endif
#endif /* __DRV_OTP_EXT_H__ */

