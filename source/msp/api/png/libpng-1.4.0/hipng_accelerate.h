/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: hipng_accelerate.h
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/18
Description	: libpng�����
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2010/10/18		z00141204		Created file      	
******************************************************************************/

#ifndef __HIPNG_ACCELERATE_H__
#define __HIPNG_ACCELERATE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"

#define PNG_NO_PEDANTIC_WARNINGS
#include "png.h"
#include "hi_png_type.h"
#include "hi_png_api.h"

#define HIPNG_SWDEC_MASK 0x1
#define HIPNG_HWDEC_MASK 0x2
#define HIPNG_DEC_COPY 0x4

/* Ӳ��������ƽṹ�� */
typedef struct tag_hipng_struct_hwctl_s
{
    HI_U8 u8DecMode;         /*Decode mode*//*CNcomment:����ģʽ*/

    /*Decode mode specified by user*/
    /*CNcomment:�û�ָ���Ľ���ģʽ*/
    HI_U8 u8UserDecMode; /* �û�ָ���Ľ���ģʽ */
    HI_BOOL bSyncDec;       /*Sync decode flag*//*CNcomment:�Ƿ�ͬ������*/
    HI_PNG_HANDLE s32Handle; /*Decoder handle*//*CNcomment:Ӳ�����������*/
    HI_PNG_STATE_E eState;  /*Decoder state*//*CNcomment:������״̬ */

    /*IO function of software decode*/
    /*CNcomment:libpngԭ����IO����ָ��*/
    png_rw_ptr read_data_fn;    /* libpngԭ����IO����ָ�� */

    /*IO function of hardware decode*/
    /*CNcomment:Ӳ����IO����ָ��*/
    HI_PNG_READ_FN read_data_fn_hw; /* Ӳ����IO����ָ�� */
    png_bytepp image;       /*Output buf*//*CNcomment:Ŀ������ڴ� */
    png_uint_32 idat_size;     /* current IDAT size for read */
    png_uint_32 crc;           /* current chunk CRC value */
    png_byte chunk_name[5];
    png_uint_32 pallateaddr;
    HI_BOOL bPallateAlpha;
    HI_U32 u32Phyaddr;
    HI_U32 u32Stride;
    HI_U32 u32InflexionSize;
}hipng_struct_hwctl_s;


/*****************************************************************
* func:	����Ӳ��������Ϣ�ṹ��
* in:	      user_png_ver,error_ptr,png_error_ptr and warn_fn are useless, only for keep
            the same style with hipng_create_read_struct
            CNcomment:user_png_ver,error_ptr,png_error_ptr,warn_fn ����,
            ֻ��Ϊ�˺�libpng����ʽ����һ��
* out:	A pointer to struct hipng_struct_hwctl_s
            CNcomment:�ṹ��ָ��
* ret:   none
* others:	
*****************************************************************/
hipng_struct_hwctl_s *hipng_create_read_struct_hw(png_const_charp user_png_ver, png_voidp error_ptr,
   png_error_ptr error_fn, png_error_ptr warn_fn);

/*****************************************************************
* func:	Destroy hardware decoder
            CNcomment:����Ӳ��������Ϣ�ṹ��
* in:	pstHwctlStruct A pointer to decoder struct  CNcomment:������Ϣ�ṹ��
* out:	none
* ret:   none
* others:	
*****************************************************************/
HI_VOID hipng_destroy_read_struct_hw(hipng_struct_hwctl_s *pstHwctlStruct);

/*****************************************************************
* func:	Start hardware decode       CNcomment:pngӲ������
* in:	png_ptr png   decoder ptr     CNcomment:����ṹ��
* out:	none
* ret:	HI_SUCCESS	Success     CNcomment:����ɹ�
*		HI_FAILURE	Failure     CNcomment:����ʧ��
* others:	
*****************************************************************/
HI_S32 hipng_read_image_hw(png_structp png_ptr);

HI_S32 hipng_get_readfn_hw(png_structp png_ptr);

HI_S32 hipng_get_result(png_structp png_ptr, HI_BOOL bBlock);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HIPNG_ACCELERATE_H__ */

