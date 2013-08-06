
/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_hal.c
Version		    : Initial Draft
Author		    : 
Created		    : 2013/03/26
Description	    : realize
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2013/03/26		    y00181162		                	
******************************************************************************/


/*********************************add include here******************************/

#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/stddef.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <asm/pgtable.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "hi_type.h"
#include "jpg_common.h"
#include "jpg_hal.h"
#include "hi_jpeg_config.h"


/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/

static HI_U32 s_u32JpgRegAddr = 0;


/******************************* API forward declarations *******************/

/******************************* API realization *****************************/


/*****************************************************************************
* func            : JpegHalCheckVersion
* description     : how to deal with the clock
* param[in]       : 
* retval          : HI_JPEG_CLOCK_TYPE
*****************************************************************************/
HI_JPEG_CLOCK_TYPE JpegHalCheckVersion(\
                               const HI_CHIP_TYPE_E     enChipType, \
                               const HI_CHIP_VERSION_E  enChipVersion)
{

    if(HI_CHIP_VERSION_V101 == enChipVersion)
    {
        if(    (HI_CHIP_TYPE_HI3716C == enChipType) 
		    || (HI_CHIP_TYPE_HI3716H == enChipType))
        {
            return HI_JPEG_CLOCK_V1;
        }

        if (HI_CHIP_TYPE_HI3716M == enChipType)
        {
            return HI_JPEG_CLOCK_V2;
        }

        if(HI_CHIP_TYPE_HI3712 == enChipType)
        {
            return HI_JPEG_CLOCK_V3;
        }
    }

    return HI_JPEG_CLOCK_BUTT;
	
	
}

HI_U32 JPGDRV_READ_REG(HI_U32 base,HI_U32 offset)
{

    return (*(volatile HI_U32 *)((HI_U32)(base) + (offset)));
    
}

HI_VOID  JPGDRV_WRITE_REG(HI_U32 base, HI_U32 offset, HI_U32 value)
{

    (*(volatile HI_U32 *)((HI_U32)(base) + (offset)) = (value));
    
}



/*****************************************************************************
* func            : JpgHalInit
* description     : initial the jpeg device
* param[in]       : none
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalInit(HI_VOID)
{

      /** 
       ** map the register address to s_u32JpgRegAddr
       **/
        HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

        #if defined(HI_S40V200_VERSION)

		    s_u32JpgRegAddr = (volatile HI_U32)ioremap_nocache(S40V200_JPGD0_REG_BASEADDR, JPG_REG_LENGTH);

		#elif defined(HI_3716CV200_VERSION)

		    s_u32JpgRegAddr = (volatile HI_U32)ioremap_nocache(HI3716CV200_JPGD0_REG_BASEADDR, JPG_REG_LENGTH);

		#else
		
			if (   (HI_CHIP_TYPE_HI3716M == enChipType)
					|| (HI_CHIP_TYPE_HI3716H == enChipType)
					|| (HI_CHIP_TYPE_HI3716C == enChipType))
			{
			    s_u32JpgRegAddr = (volatile HI_U32)ioremap_nocache(0x60100000, (JPG_REG_LENGTH));
			}
			else if (HI_CHIP_TYPE_HI3712 == enChipType)
			{
		         s_u32JpgRegAddr = (volatile HI_U32)ioremap_nocache(0x101a0000, (JPG_REG_LENGTH));
			}
			else
			{
				 return;
			}
        #endif
		
        return;
    
}

 /*****************************************************************************
* func            : JpgHalExit
* description     : exit initial the jpeg device
* param[in]       : none
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalExit(HI_VOID)
{

    /**
     ** unmap the register address and set s_u32JpgRegAddr with zero
     **/  
    iounmap((HI_VOID*)(s_u32JpgRegAddr));
    s_u32JpgRegAddr = 0;
    
    return;
}

/*****************************************************************************
* func            : JpgHalGetIntStatus
* description     : get halt status
* param[in]       : none
* retval          : none
* output          : pIntStatus  the value of halt state
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalGetIntStatus(HI_U32 *pIntStatus)
{

    /**
     ** read the halt register and write it to *pIntStatus
     **/
    *pIntStatus = JPGDRV_READ_REG(s_u32JpgRegAddr, X5_JPG_REG_INTSTATUSOFFSET);
    
    return;
    
}

/*****************************************************************************
* func            : JpgHalSetIntStatus
* description     : set halt status
* param[in]       : IntStatus    the halt value
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalSetIntStatus(HI_U32 IntStatus)
{
    /**
     ** read halt register and write it to *pIntStatus
     **/
    JPGDRV_WRITE_REG(s_u32JpgRegAddr, X5_JPG_REG_INTSTATUSOFFSET, IntStatus);
    
    return;
    
}

/*****************************************************************************
* func            : JpgHalSetIntMask
* description     : set halt mask
* param[in]       : IntMask     halt mask
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalSetIntMask(HI_U32 IntMask)
{
    /** set halt mask with IntMask **/
    JPGDRV_WRITE_REG(s_u32JpgRegAddr, X5_JPG_REG_INTMASKOFFSET, IntMask);
    
    return;
}


/*****************************************************************************
* func            : JpgHalGetIntMask
* description     : get halt mask
* param[in]       : none
* retval          : none
* output          : pIntMask   halt mask
* others:	      : nothing
*****************************************************************************/
HI_VOID JpgHalGetIntMask(HI_U32 *pIntMask)
{

    /** get halt mask and write it to *pIntMask **/
    *pIntMask = JPGDRV_READ_REG(s_u32JpgRegAddr, X5_JPG_REG_INTMASKOFFSET);

    return;
}
