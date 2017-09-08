/******************************************************************************

  Copyright (C), 2013-2020, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpeg_hdec_mem.h
Version		    : Initial Draft
Author		    : 
Created		    : 2013/06/20
Description	    : the mem mangage
                  CNcomment: �ڴ���� CNend\n
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2013/06/20		    y00181162  		    Created file      	
******************************************************************************/
#ifndef __JPEG_HDEC_MEM_H__
#define __JPEG_HDEC_MEM_H__


/*********************************add include here******************************/

#include  "jpeglib.h"
#include  "hi_type.h"

#include "jpeg_hdec_api.h"
#include "hi_jpeg_config.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */


    /***************************** Macro Definition ******************************/
    /** \addtogroup 	 JPEG MEM MACRO */
    /** @{ */  /** <!-- ��JPEG MEM MACRO�� */

	 /** this macro is from the make menuconfig */
	 /** CNcomment:������Щ�����������֮make menuconfig
				   ע��Ӳ��buf��ַ��СҪ64�ֽڶ��룬�洢����bufferû��
				   ����Ҫ�󣬵�Ҫ��Ӳ��buf��Χ֮��himd.l 0x60100000  0x20
				   ƫ�Ƶ�ַ��ȷ����ǰ������buf��ַ��������������buf��
				   ַ�������ʱbufferҪ��Զ���ڹ�Ӳ��ʹ�� */
	 #ifndef  CFG_HI_JPEG6B_STREAMBUFFER_SIZE
	 /** the hard buffer size */
	 /** CNcomment:Ӳ��buffer��С,Ҫ64�ֽڶ��� */
	 #define JPGD_HARD_BUFFER				      (1024 * 1024)
	 /** the save stream size,1M is the best,the buffer size should >= INPUT_BUF_SIZE */
	 /** CNcomment:�洢������buffer��С����������1M����õ�,-64��Ϊ�˱�֤��Ӳ��buf��Χ֮��,
	               ����buffer��С������� INPUT_BUF_SIZE = 4096 */
	 #define JPGD_STREAM_BUFFER				  (JPGD_HARD_BUFFER - 64)
	 #else
	 #define JPGD_HARD_BUFFER					  (CFG_HI_JPEG6B_STREAMBUFFER_SIZE) > (4096 + 64) ? (CFG_HI_JPEG6B_STREAMBUFFER_SIZE) : (4096 + 64)
	 #define JPGD_STREAM_BUFFER				  (JPGD_HARD_BUFFER - 64)
	 #endif


	 /** 2bytes align */
	 /** CNcomment:2�ֽڶ��� */
	#define JPGD_HDEC_MMZ_ALIGN_2BYTES 	  2
	 
	 /** 3bytes align */
	 /** CNcomment:3�ֽڶ��� */
	#define JPGD_HDEC_MMZ_ALIGN_3BYTES 	  3

	
	 /** 4bytes align */
	 /** CNcomment:4�ֽڶ��� */
	#define JPGD_HDEC_MMZ_ALIGN_4BYTES 	  4

	 /** 4bytes align */
	 /** CNcomment:16�ֽڶ��� */
	 #define JPGD_HDEC_MMZ_ALIGN_16BYTES 	  16

	 /** 24bytes align */
	 /** CNcomment:24�ֽڶ��� */
	 #define JPGD_HDEC_MMZ_ALIGN_24BYTES   24
	 
	 /** 64bytes align */
	 /** CNcomment:64�ֽڶ��� */
	#define JPGD_HDEC_MMZ_ALIGN_64BYTES 	  64
	 
	 
	 /** 128bytes align */
	 /** CNcomment:128�ֽڶ��� */
	#define JPGD_HDEC_MMZ_ALIGN_128BYTES 	  128

	 /** @} */	/*! <!-- Macro Definition end */


	 /*************************** Enum Definition ****************************/

	/** \addtogroup      JPEG MEM ENUM */
    /** @{ */  /** <!-- ��JPEG MEM ENUM�� */


	
    /** @} */  /*! <!-- enum Definition end */

	/*************************** Structure Definition ****************************/

	/** \addtogroup      JPEG MEM STRUCTURE */
    /** @{ */  /** <!-- ��JPEG MEM STRUCTURE�� */

	/** @} */  /*! <!-- Structure Definition end */

	
    /********************** Global Variable declaration **************************/
 
    /******************************* API declaration *****************************/

	/** \addtogroup      JPEG MEM API */
    /** @{ */  /** <!-- ��JPEG MEM API�� */
	

	/*****************************************************************************
	* func			: JPEG_HDEC_GetStreamMem
	* description	: alloc the stream buffer mem
					  CNcomment: ��������buffer�ڴ� CNend\n
	* param[in] 	: u32MemSize   CNcomment: Ҫ������ڴ��С	  CNend\n
	* param[out]	: pOutPhyAddr  CNcomment: ����õ��������ַ  CNend\n
	* param[out]	: pOutVirAddr  CNcomment: ����õ��������ַ  CNend\n
	* retval		: HI_SUCCESS   CNcomment: �ɹ�	CNend\n
	* retval		: HI_FAILURE   CNcomment: ʧ��	 CNend\n
	* others:		: NA
	*****************************************************************************/
    #ifdef CONFIG_JPEG_REALIZEFUNC_BEFORMAINFUNC
	HI_S32	JPEG_HDEC_GetStreamMem(const HI_U32 u32MemSize,HI_CHAR **pOutPhyAddr,HI_CHAR **pOutVirAddr);
    #else
	HI_S32	JPEG_HDEC_GetStreamMem(JPEG_HDEC_HANDLE_S_PTR	 pJpegHandle,const HI_U32 u32MemSize);
    #endif

	
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_FreeStreamMem
	 * description	 : free the stream buffer mem
					   CNcomment: �ͷ�����buffer�ڴ� CNend\n
	 * param[in]	 : pInPhyAddr	 CNcomment: Ҫ�ͷŵ�����buffer�����ַ CNend\n
	 * retval		 : NA
	 * others:		 : NA
	 *****************************************************************************/
	 #ifdef CONFIG_JPEG_REALIZEFUNC_BEFORMAINFUNC
	 HI_VOID JPEG_HDEC_FreeStreamMem(HI_CHAR *pInPhyAddr);
	 #else
     HI_VOID JPEG_HDEC_FreeStreamMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle);
	 #endif
	
	
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_GetYUVMem
	 * description	 : get the hard decode output mem
					   CNcomment: ��ȡӲ������������ڴ� CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ��������� CNend\n
	 * retval		 : HI_SUCCESS	 CNcomment: �ɹ�       CNend\n
	 * retval		 : HI_FAILURE	 CNcomment: ʧ��       CNend\n
	 * others:		 : NA
	 *****************************************************************************/
	 HI_S32 JPEG_HDEC_GetYUVMem(JPEG_HDEC_HANDLE_S_PTR	 pJpegHandle);
	
	
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_FreeYUVMem
	 * description	 : free the hard decode output mem
					   CNcomment: �ͷ�Ӳ����������ĵ�ַ  CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ���������  CNend\n
	 * retval		 : NA
	 * others:		 : NA
	 *****************************************************************************/
	 HI_VOID JPEG_HDEC_FreeYUVMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle);
	
	 #ifdef CONFIG_JPEG_HARDDEC2ARGB
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_GetMinMem
	 * description	 : get dec output argb min memory
					   CNcomment: ��ȡӲ���������ΪARGB����buffer CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ��������� CNend\n
	 * retval		 : HI_SUCCESS	 CNcomment: �ɹ�       CNend\n
	 * retval		 : HI_FAILURE	 CNcomment: ʧ��       CNend\n
	 * others:		 : NA
	 *****************************************************************************/
	 HI_S32 JPEG_HDEC_GetMinMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle);
	
	
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_FreeMinMem
	 * description	 : free dec output argb min memory
					   CNcomment: �ͷ�Ӳ���������ΪARGB����buffer  CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ���������  CNend\n
	 * retval		 : NA
	 * others:		 : NA
	 *****************************************************************************/
	 HI_VOID JPEG_HDEC_FreeMinMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle);
	 #endif

	 
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_GetOutMem
	 * description	 : get the output buffer
	                   CNcomment: ��������������ڴ�      CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ��������� CNend\n
	 * retval		 : HI_SUCCESS    CNcomment: �ɹ�       CNend\n
	 * retval		 : HI_FAILURE    CNcomment: ʧ��       CNend\n
	 * others:		 : NA
	 *****************************************************************************/
	 HI_S32 JPEG_HDEC_GetOutMem(const struct jpeg_decompress_struct *cinfo);
	 
	
	 /*****************************************************************************
	 * func 		 : JPEG_HDEC_FreeOutMem
	 * description	 : free the output buf
	                   CNcomment: �ͷ�����������ڴ�        CNend\n
	 * param[in]	 : pJpegHandle   CNcomment: ���������  CNend\n
	 * retval		 : NA
	 * others:		 : NA
	 *****************************************************************************/
	 HI_VOID JPEG_HDEC_FreeOutMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle);


	/** @} */  /*! <!-- API declaration end */
	
    /****************************************************************************/



#ifdef __cplusplus
    
#if __cplusplus
   
}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_MEM_H__*/
