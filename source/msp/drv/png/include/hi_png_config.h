/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_png_config.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/05/14
Description	    : config the gao an macro and other CFLASS
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2011/11/23		            		    Created file      	
******************************************************************************/

#ifndef __HI_PNG_CONFIG__
#define __HI_PNG_CONFIG__


/*********************************add include here******************************/
/** 使用common模块内存分配，设备注册，打印 **/
#define PNG_USE_COMMON_FUNC


#ifdef PNG_USE_COMMON_FUNC
#include "hi_module.h"
#include "hi_debug.h"


#ifdef __KERNEL__
#include "drv_mmz_ext.h"
#include "drv_mem_ext.h"
#include "drv_module_ext.h"
#endif
#endif


/*****************************************************************************/


/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/
     #if 0
	 #ifdef ADVCA_SUPPORT
          #define __INIT__
          #define __EXIT__
     #else
          #define __INIT__  __init
          #define __EXIT__  __exit
     #endif
	 #endif



	/** 打印 **/
	#define HIPNG_FATAL_TRACE(fmt, args... )      HI_FATAL_PRINT(HI_ID_PNG, fmt)
    #define HIPNG__ERR_TRACE(fmt, args... )       HI_ERR_PRINT(HI_ID_PNG, fmt)
    #define HIPNG_WARN_TRACE(fmt, args... )       HI_WARN_PRINT(HI_ID_PNG, fmt)
	      #ifdef HIPNG_DEBUG
             #define HIPNG_TRACE(fmt, args... )   HI_INFO_PRINT(HI_ID_PNG, fmt)
		  #else
		     #define HIPNG_TRACE(fmt, args... )
		  #endif

    /** kmalloc,kfree,vmalloc,vfree 等等 **/
	#define PNG_KMALLOC(module_id, size, flags)      HI_KMALLOC(module_id, size, flags)
	#define PNG_KFREE(module_id, addr)               HI_KFREE(module_id, addr)
	#define PNG_VMALLOC(module_id, size)             HI_VMALLOC(module_id, size)
	#define PNG_VFREE(module_id, addr)               HI_VFREE(module_id, addr)


		 #define PNG_REG_MAP(base, size)  \
                       ioremap_nocache((base), (size))

	     #define PNG_REG_UNMAP(base)  \
 		               iounmap((HI_VOID*)(base))

		 
	    #define PNG_FREE_MMB(phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        HI_DRV_MMZ_Release(&stBuffer);                      \
	    }while(0)

	    #define PNG_GET_PHYADDR_MMB(name, size, Align, phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        if(HI_SUCCESS == HI_DRV_MMZ_Alloc(name, NULL, size, Align, &stBuffer))   \
	        {                                                           \
	            phyaddr = stBuffer.u32StartPhyAddr;                     \
	        }                                                           \
	        else                                                        \
	        {                                                           \
	            HIPNG_TRACE("new_mmb failed!");                         \
	            phyaddr = 0;                                            \
	        }                                                           \
	    }while(0)

	    #define PNG_REMAP_MMB(phyaddr, virtaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        if(HI_SUCCESS == HI_DRV_MMZ_Map(&stBuffer))         \
	        {                                                   \
	            virtaddr = (HI_VOID*)stBuffer.u32StartVirAddr;            \
	        }                                                   \
	        else                                                \
	        {                                                   \
	            PNG_FREE_MMB(phyaddr);                          \
	            return -1;                                      \
	        }                                                   \
	    }while(0)

	    #define PNG_UNMAP_MMB(virtaddr) \
		do{\
		    MMZ_BUFFER_S stBuffer;                              \
		    stBuffer.u32StartVirAddr = virtaddr;                \
		    HI_DRV_MMZ_Unmap(&stBuffer);                        \
		}while(0)

#define USE_HIMEDIA_DEVICE
    /*************************** Structure Definition ****************************/


    /***************************  The enum of Jpeg image format  ******************/

    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/


    #ifdef __cplusplus

        #if __cplusplus



}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __HI_PNG_CONFIG__*/
