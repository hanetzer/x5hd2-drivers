/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_hal.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : 
Function List 	: JpegHalCheckVersion
                : JpgHalInit
			    : JpgHalExit
			    : JpgHalGetIntStatus
			    : JpgHalSetIntMask
                : JpgHalSetIntMask
                : JpgHalGetIntMask
			  		  
History       	:
Date				Author        		Modification
2011/05/11		    y00181162		                	
******************************************************************************/

#ifndef  _JPG_HAL_H_
#define  _JPG_HAL_H_


#include "jpg_common.h"
/** get chip version **/
#include "drv_sys_ext.h"
#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */



/*****************************************************************************
* func            : JpegHalCheckVersion
* description     : how to deal with the clock
* param[in]       : 
* retval          : HI_JPEG_CLOCK_TYPE
*****************************************************************************/
HI_JPEG_CLOCK_TYPE JpegHalCheckVersion(\
                           const HI_CHIP_TYPE_E     enChipType, \
                           const HI_CHIP_VERSION_E  enChipVersion);
                               
/*****************************************************************************
* func            : JpgHalInit
* description     : initial the jpeg device
* param[in]       : none
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalInit(HI_VOID);



 /*****************************************************************************
* func            : JpgHalExit
* description     : exit initial the jpeg device
* param[in]       : none
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalExit(HI_VOID);



/*****************************************************************************
* func            : JpgHalGetIntStatus
* description     : get halt status
* param[in]       : none
* retval          : none
* output          : pIntStatus  the value of halt state
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalGetIntStatus(HI_U32 *pIntStatus);



/*****************************************************************************
* func            : JpgHalSetIntMask
* description     : set halt mask
* param[in]       : IntMask     halt mask
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalSetIntMask(HI_U32 IntMask);


/*****************************************************************************
* func            : JpgHalGetIntMask
* description     : get halt mask
* param[in]       : none
* retval          : none
* output          : pIntMask   halt mask
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalGetIntMask(HI_U32 *pIntMask);

 
/*****************************************************************************
* func            : JpgHalSetIntStatus
* description     : set halt status
* param[in]       : IntStatus    the halt value
* retval          : none
* output          : none
* others:	      : nothing
*****************************************************************************/

HI_VOID JpgHalSetIntStatus(HI_U32 IntStatus);


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */


#endif /*_JPG_HAL_H_ */
