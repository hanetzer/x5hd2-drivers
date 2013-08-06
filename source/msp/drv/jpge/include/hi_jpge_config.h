/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpge_config.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/05/14
Description	    : config the gao an macro and other CFLASS
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2011/11/23		            		    Created file      	
******************************************************************************/

#ifndef __HI_JPGE_CONFIG__
#define __HI_JPGE_CONFIG__


/*********************************add include here******************************/
/** 使用common模块内存分配，设备注册，打印 **/
#define JPGE_USE_COMMON_FUNC


#ifdef JPGE_USE_COMMON_FUNC
#include "hi_module.h"
#include "hi_debug.h"

#ifdef __KERNEL__
#include "drv_mmz_ext.h"
#include "drv_mem_ext.h"
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


    #ifndef JPGE_USE_COMMON_FUNC
	 
	    /** 打印 **/
	    #if defined(ADVCA_SUPPORT) && defined(CONFIG_SUPPORT_CA_RELEASE)
	       #define HIJPEG_GAO_AN_VERSION 
           #define HIJPGE_TRACE(fmt, args... )
        #else
           #ifdef HIJPEG_DEBUG
				#ifdef __KERNEL__
			        #define HIJPGE_TRACE(fmt, args... )\
			        do { \
			           printk(fmt, ##args );\
			        } while (0)
		        #else
			        #define HIJPGE_TRACE(fmt, args... )\
			        do { \
			           printf("%s\n %s(): %d Line\n: ", __FILE__,  __FUNCTION__,  __LINE__ );\
			           printf(fmt, ##args );\
			        } while (0)
		        #endif
		   #else
                #define HIJPGE_TRACE(fmt, args... )
		   #endif
        #endif	
    /** kmalloc,kfree,vmalloc,vfree 等等 **/
	#define HI_ID_JPGENC  1
    #define JPGE_KMALLOC(module_id, size, flags)      kmalloc(size, flags)
    #define JPGE_KFREE(module_id, addr)               kfree(addr)
    #define JPGE_VMALLOC(module_id, size)             vmalloc(size)
    #define JPGE_VFREE(module_id, addr)               vfree(addr)

	#else

	/** 打印 **/
	#define HIJPGE_FATAL_TRACE(fmt, args... )      HI_FATAL_PRINT(HI_ID_JPGENC, fmt)
    #define HIJPGE__ERR_TRACE(fmt, args... )       HI_ERR_PRINT(HI_ID_JPGENC, fmt)
    #define HIJPGE_WARN_TRACE(fmt, args... )       HI_WARN_PRINT(HI_ID_JPGENC, fmt)
	      #ifdef HIJPEG_DEBUG
             #define HIJPGE_TRACE(fmt, args... )   HI_INFO_PRINT(HI_ID_JPGENC, fmt)
		  #else
		     #define HIJPGE_TRACE(fmt, args... )
		  #endif

    /** kmalloc,kfree,vmalloc,vfree 等等 **/
	#define JPGE_KMALLOC(module_id, size, flags)      HI_KMALLOC(module_id, size, flags)
	#define JPGE_KFREE(module_id, addr)               HI_KFREE(module_id, addr)
	#define JPGE_VMALLOC(module_id, size)             HI_VMALLOC(module_id, size)
	#define JPGE_VFREE(module_id, addr)               HI_VFREE(module_id, addr)
	#endif




	#ifndef JPGE_USE_COMMON_FUNC
		 #define JPGE_REG_MAP_NOCACHE(base, size)  \
                       ioremap_nocache((base), (size))
                       
		 #define JPGE_REG_MAP(base, size)  \
                       ioremap((base), (size))                       
	     #define JPGE_REG_UNMAP(base)  \
 		               iounmap((HI_VOID*)(base))

	    #define JPGE_FREE_MMB(phyaddr) \
	    do{\
	        if(MMB_ADDR_INVALID != phyaddr)\
	        {\
	            delete_mmb(phyaddr);\
	        }\
	    }while(0)

	    #define JPGE_GET_PHYADDR_MMB(name, size, phyaddr) \
	    do{\
	        phyaddr = new_mmb(name, size, 16, NULL);\
	        if(MMB_ADDR_INVALID == phyaddr)\
	        {\
	            HIJPGE_TRACE("new_mmb failed!");\
	            phyaddr = 0; \
	        }\
	    }while(0)

	    #define JPGE_REMAP_MMB(phyaddr, virtaddr) \
	    do{\
	        virtaddr = (HI_U32)remap_mmb(phyaddr);\
	        if(0 == virtaddr)\
	        {\
	            JPGE_FREE_MMB(phyaddr); \
	            return -1;\
	        }\
	    }while(0)

	    #define JPGE_UNMAP_MMB(virtaddr) \
			           unmap_mmb((HI_VOID*)virtaddr)

    #else
	

                       
		 #define JPGE_REG_MAP_NOCACHE(base, size)  \
                       ioremap_nocache((base), (size))
                       
		 #define JPGE_REG_MAP(base, size)  \
                       ioremap((base), (size))                       
	     #define JPGE_REG_UNMAP(base)  \
 		               iounmap((HI_VOID*)(base))

		 
	    #define JPGE_FREE_MMB(pmmb, phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        HI_DRV_MMZ_Release(&stBuffer);                      \
	    }while(0)

	    #define JPGE_GET_PHYADDR_MMB(name, size, phyaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        if(HI_SUCCESS == HI_DRV_MMZ_Alloc(name, NULL, size, 16, &stBuffer))   \
	        {                                                           \
	            phyaddr = stBuffer.u32StartPhyAddr;                     \
	        }                                                           \
	        else                                                        \
	        {                                                           \
	            HIJPGE_TRACE("new_mmb failed!");                        \
	            phyaddr = 0;                                            \
	        }                                                           \
	    }while(0)

	    #define JPGE_REMAP_MMB(phyaddr, virtaddr) \
	    do{\
	        MMZ_BUFFER_S stBuffer;                              \
	        stBuffer.u32StartPhyAddr = phyaddr;                 \
	        if(HI_SUCCESS == HI_DRV_MMZ_Map(&stBuffer))         \
	        {                                                   \
	            virtaddr = stBuffer.u32StartVirAddr;            \
	        }                                                   \
	        else                                                \
	        {                                                   \
	            JPGE_FREE_MMB(phyaddr);                   \
	            return -1;                                      \
	        }                                                   \
	    }while(0)

	    #define JPGE_UNMAP_MMB(virtaddr) \
		do{\
		    MMZ_BUFFER_S stBuffer;                              \
		    stBuffer.u32StartVirAddr = virtaddr;                \
		    HI_DRV_MMZ_Unmap(&stBuffer);                        \
		}while(0)

	#endif

    /** 设备注册 **/
	#ifdef JPGE_USE_COMMON_FUNC
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

#endif /* __HI_JPGE_CONFIG__*/
