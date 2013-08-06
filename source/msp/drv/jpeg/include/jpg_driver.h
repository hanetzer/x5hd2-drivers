/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_driver.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : JPEG6B decode driver file
Function List 	: HI_JPG_Open
			    : HI_JPG_Close
			    : JPGDRV_GetDevice
			    : JPGDRV_ReleaseDevice
                : JPGDRV_GetRegisterAddr
                : JPGDRV_GetIntStatus
			  		  
History       	:
Date				Author        		Modification
2012/10/31		    y00181162		                	
******************************************************************************/


#ifndef  _JPG_DRIVER_H_
#define  _JPG_DRIVER_H_

#ifdef __cplusplus
    #if __cplusplus
extern "C"{
    #endif  /* __cplusplus */
#endif  /* __cplusplus */


        /***************************** Macro Definition ******************************/


        /*************************** Structure Definition ****************************/

        /** halt state types **/

        typedef enum hiJPG_INTTYPE_E
        {

            JPG_INTTYPE_NONE = 0,    /** none halt happen     **/
            JPG_INTTYPE_CONTINUE,    /** continue stream halt **/
            JPG_INTTYPE_FINISH,      /** finish halt          **/
            JPG_INTTYPE_ERROR,       /** error halt           **/
            JPG_INTTYPE_BUTT
            
        }JPG_INTTYPE_E;


        /** get halt state struct **/

        typedef struct hiJPG_GETINTTYPE_S
        {

            JPG_INTTYPE_E IntType;    /** halt type **/
            HI_U32 TimeOut;           /** overtime  **/
            
        }JPG_GETINTTYPE_S;

 
       /******************************* API declaration *****************************/


#ifdef __cplusplus
   #if __cplusplus
}
   #endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* _JPG_DRIVER_H_*/
