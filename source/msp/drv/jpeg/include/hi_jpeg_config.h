/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpeg_config.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : config the gao an macro and other CFLASS
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2012/10/31		    y00181162  		    Created file      	
******************************************************************************/

#ifndef __HI_JPEG_CONFIG_H__
#define __HI_JPEG_CONFIG_H__


/*********************************add include here******************************/


/** 使用common模块内存分配，设备注册，打印 **/
#define JPEG_USE_COMMON_FUNC


#ifdef JPEG_USE_COMMON_FUNC
#include "hi_module.h"
#include "hi_debug.h"


#include "drv_mmz_ext.h"
#include "drv_mem_ext.h"
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


    #ifndef JPEG_USE_COMMON_FUNC
	 
	    /** 打印 **/
	    #if defined(ADVCA_SUPPORT) && defined(CONFIG_SUPPORT_CA_RELEASE)
	       #define GRAPHICS_ADVCA_VERSION 
           #define HIJPEG_TRACE(fmt, args... )
        #else
           #ifdef HIJPEG_DEBUG
				#ifdef __KERNEL__
			        #define HIJPEG_TRACE(fmt, args... )\
			        do { \
			           printk(fmt, ##args );\
			        } while (0)
		        #else
			        #define HIJPEG_TRACE(fmt, args... )\
			        do { \
			           printf("%s\n %s(): %d Line\n: ", __FILE__,  __FUNCTION__,  __LINE__ );\
			           printf(fmt, ##args );\
			        } while (0)
		        #endif
		   #else
                #define HIJPEG_TRACE(fmt, args... )
		   #endif
        #endif	
    /** kmalloc,kfree,vmalloc,vfree 等等 **/
	#define HI_ID_JPGDEC  0
    #define JPEG_KMALLOC(module_id, size, flags)      kmalloc(size, flags)
    #define JPEG_KFREE(module_id, addr)               kfree(addr)
    #define JPEG_VMALLOC(module_id, size)             vmalloc(size)
    #define JPEG_VFREE(module_id, addr)               vfree(addr)

	#else

	/** 打印 **/
	#define HIJPEG_FATAL_TRACE(fmt, args... )      HI_FATAL_PRINT(HI_ID_JPGDEC, fmt)
    #define HIJPEG_ERR_TRACE(fmt, args... )        HI_ERR_PRINT(HI_ID_JPGDEC, fmt)
    #define HIJPEG_WARN_TRACE(fmt, args... )       HI_WARN_PRINT(HI_ID_JPGDEC, fmt)
	      #ifdef HIJPEG_DEBUG
             #define HIJPEG_TRACE(fmt, args... )   HI_INFO_PRINT(HI_ID_JPGDEC, fmt)
		  #else
		     #define HIJPEG_TRACE(fmt, args... )
		  #endif

    /** kmalloc,kfree,vmalloc,vfree 等等 **/
	#define JPEG_KMALLOC(module_id, size, flags)      HI_KMALLOC(module_id, size, flags)
	#define JPEG_KFREE(module_id, addr)               HI_KFREE(module_id, addr)
	#define JPEG_VMALLOC(module_id, size)             HI_VMALLOC(module_id, size)
	#define JPEG_VFREE(module_id, addr)               HI_VFREE(module_id, addr)
	
	#endif




	#ifndef JPEG_USE_COMMON_FUNC
		 #define JPEG_REG_MAP(base, size)  \
                       ioremap_nocache((base), (size))

	     #define JPEG_REG_UNMAP(base)  \
 		               iounmap((HI_VOID*)(base))

	    #define JPEG_FREE_MMB(phyaddr) \
	    do{\
	        if(MMB_ADDR_INVALID != phyaddr)\
	        {\
	            delete_mmb(phyaddr);\
	        }\
	    }while(0)

	    #define JPEG_GET_PHYADDR_MMB(name, size, phyaddr) \
	    do{\
	        phyaddr = new_mmb(name, size, 16, NULL);\
	        if(MMB_ADDR_INVALID == phyaddr)\
	        {\
	            HIJPEG_TRACE("new_mmb failed!");\
	            phyaddr = 0; \
	        }\
	    }while(0)

	    #define JPEG_REMAP_MMB(phyaddr, virtaddr) \
	    do{\
	        virtaddr = (HI_U32)remap_mmb(phyaddr);\
	        if(0 == virtaddr)\
	        {\
	            JPEG_FREE_MMB(phyaddr); \
	            return -1;\
	        }\
	    }while(0)

	    #define JPEG_UNMAP_MMB(virtaddr) \
			           unmap_mmb((HI_VOID*)virtaddr)

    #else
	
		 #define JPEG_REG_MAP(base, size)  \
                       ioremap_nocache((base), (size))

	     #define JPEG_REG_UNMAP(base)  \
 		               iounmap((HI_VOID*)(base))

		 
	    #define JPEG_FREE_MMB(phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        HI_DRV_MMZ_Release(&stBuffer);                      \
	    }while(0)

	    #define JPEG_GET_PHYADDR_MMB(name, size, phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        if(HI_SUCCESS == HI_DRV_MMZ_Alloc(name, NULL, size, 16, &stBuffer))   \
	        {                                                           \
	            phyaddr = stBuffer.u32StartPhyAddr;                     \
	        }                                                           \
	        else                                                        \
	        {                                                           \
	            HIJPEG_TRACE("new_mmb failed!");                        \
	            phyaddr = 0;                                            \
	        }                                                           \
	    }while(0)

	    #define JPEG_REMAP_MMB(phyaddr, virtaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        if(HI_SUCCESS == HI_DRV_MMZ_Map(&stBuffer))         \
	        {                                                   \
	            virtaddr = stBuffer.u32StartVirAddr;            \
	        }                                                   \
	        else                                                \
	        {                                                   \
	            JPEG_FREE_MMB(phyaddr);                   \
	            return -1;                                      \
	        }                                                   \
	    }while(0)

	    #define JPEG_UNMAP_MMB(virtaddr) \
		do{\
		    MMZ_BUFFER_S stBuffer;                              \
		    stBuffer.u32StartVirAddr = virtaddr;                \
		    HI_DRV_MMZ_Unmap(&stBuffer);                        \
		}while(0)

	#endif

    /** 设备注册 **/
	#ifdef JPEG_USE_COMMON_FUNC
	/** 不管是否开机动画都使用himedia,假如想修改就在这里修改 **/
        #define USE_HIMEDIA_DEVICE
	#else
        #define USE_HIMEDIA_DEVICE
	#endif

    /*************************** Structure Definition ****************************/


    /***************************  The enum of Jpeg image format  ******************/

    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/


    #ifdef __cplusplus

        #if __cplusplus



}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __HI_JPEG_CONFIG_H__*/
