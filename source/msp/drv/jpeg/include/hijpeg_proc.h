/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpeg_proc.h
Version		    : 
Author		    : y00181162
Created		    : 2012/10/31
Description	    : 
Function List 	: 
			    : 

History       	:
Date				Author        		Modification
2012/10/31		    y00181162		    Created file      	
******************************************************************************/

#ifndef __HI_JPEG_PROC_H__
#define __HI_JPEG_PROC_H__



/*********************************add include here******************************/
#include <linux/seq_file.h>


#include "hijpeg_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus  
     extern "C" 
{
#endif
#endif /* __cplusplus */





    /***************************** Macro Definition ******************************/

   typedef HI_S32 (*JPEG_PROC_SHOW)(struct seq_file *, HI_VOID *);
   typedef HI_S32 (*JPEG_PROC_WRITE)(struct seq_file *, const HI_VOID * pBuf, size_t len);
   typedef HI_S32 (*JPEG_PROC_IOCTL)(struct seq_file *, HI_U32 cmd, HI_U32 arg);

    /*************************** Structure Definition ****************************/

   typedef struct tagJPEG_PROC_ITEM_S
   {

    	HI_CHAR entry_name[32];
    	struct proc_dir_entry *entry;
    	JPEG_PROC_SHOW  show;
    	JPEG_PROC_WRITE write; 
        JPEG_PROC_IOCTL ioctl;
    	HI_VOID *data;
		
   }JPEG_PROC_ITEM_S;

    /********************** Global Variable declaration **************************/

	
    /******************************* API declaration *****************************/

    /*****************************************************************************
    * Function     : JPEG_Proc_GetStruct
    * Description  : 
    * param[in]    :
    * param[in]    :
    * Output       :
    * retval       :
    * retval       :
    * others:	   :nothing
    *****************************************************************************/
    HI_VOID JPEG_Proc_GetStruct(JPEG_PROC_INFO_S **ppstProcInfo);

    /*****************************************************************************
    * Function     : JPEG_Proc_init
    * Description  : 
    * param[in]    :
    * param[in]    :
    * Output       :
    * retval       :
    * retval       :
    * others:	   :nothing
    *****************************************************************************/


    HI_VOID JPEG_Proc_init(HI_VOID);
    
    /*****************************************************************************
    * Function     : JPEG_Proc_Cleanup
    * Description  : 
    * param[in]    :
    * param[in]    :
    * Output       :
    * retval       :
    * retval       :
    * others:	   :nothing
    *****************************************************************************/


    HI_VOID JPEG_Proc_Cleanup(HI_VOID);
    

    /*****************************************************************************
    * Function     : JPEG_Proc_IsOpen
    * Description  : 
    * param[in]    :
    * param[in]    :
    * Output       :
    * retval       :
    * retval       :
    * others:	   :nothing
    *****************************************************************************/

    HI_BOOL JPEG_Proc_IsOpen(HI_VOID);

    /*****************************************************************************
    * Function     : JPEG_Get_Proc_Status
    * Description  : get the proc status
    * param[in]    :
    * param[in]    :
    * Output       :
    * retval       :
    * retval       :
    * others:	   :nothing
    *****************************************************************************/

    HI_VOID JPEG_Get_Proc_Status(HI_BOOL* pbProcStatus);

    /****************************************************************************/



#ifdef __cplusplus
#if __cplusplus 
}
#endif
#endif /* __cplusplus */

#endif /* __HI_JPEG_PROC_H__ */
