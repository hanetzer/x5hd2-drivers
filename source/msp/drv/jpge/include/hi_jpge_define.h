/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpge_define.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/05/14
Description	    : config the gao an macro and other CFLASS
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2011/11/23		            		    Created file      	
******************************************************************************/

#ifndef __HI_JPGE_DEFINE__
#define __HI_JPGE_DEFINE__


/*********************************add include here******************************/

/*****************************************************************************/


/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/
 
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

     #if 0
	 #ifdef ADVCA_SUPPORT
          #define __INIT__
          #define __EXIT__
     #else
          #define __INIT__  __init
          #define __EXIT__  __exit
     #endif
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

#endif /* __HI_JPGE_DEFINE__*/
