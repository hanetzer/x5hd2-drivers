/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	        : png_accelerate.c
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/18
Description	: libpng�����ʵ��
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2010/10/18		z00141204		Created file      	
******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include "hipng_accelerate.h"
#include "pngpriv.h"
#include "hi_common.h"
#include "hi_png_config.h"
#define PNG_CHUNK_HEAD_LEN 8
#define PNG_CHUNK_TAIL_LEN 4

#define PNG_INFLEXION_SIZE (40*40)

/*****************************************************************
* func:	Create hardware decoder CNcomment:����Ӳ��������Ϣ�ṹ��
* in:	      user_png_ver,error_ptr,png_error_ptr and warn_fn are useless, only for keep
            the same style with hipng_create_read_struct
            CNcomment:user_png_ver,error_ptr,png_error_ptr,warn_fn ����,
            ֻ��Ϊ�˺�libpng����ʽ����һ��
* out:	A pointer to struct hipng_struct_hwctl_s
            CNcomment:�ṹ��ָ��
* ret:	HI_SUCCESS	Success     CNcomment:�����ɹ�
*		HI_FAILURE	Failure      CNcomment:����ʧ��
* others:	
*****************************************************************/
hipng_struct_hwctl_s *hipng_create_read_struct_hw(png_const_charp user_png_ver, png_voidp error_ptr,
   png_error_ptr error_fn, png_error_ptr warn_fn)
{
    HI_PNG_HANDLE s32Handle = -1;
    HI_S32 s32Ret = HI_SUCCESS;
    hipng_struct_hwctl_s *pstPngStruct = NULL;

    /*Only for deleting compile warnings*/
    /*CNcomment:�����澯*/
    user_png_ver = user_png_ver;
    error_ptr = error_ptr;
    error_fn = error_fn;
    warn_fn = warn_fn;
    
    s32Ret = HI_PNG_Open();
    if (s32Ret < 0)
    {
        return NULL;
    }

    s32Ret = HI_PNG_CreateDecoder(&s32Handle);
    if (s32Ret < 0)
    {
        HI_PNG_Close();
        return NULL;
    }
    
    pstPngStruct = (hipng_struct_hwctl_s *)malloc(sizeof(hipng_struct_hwctl_s));
    if (NULL == pstPngStruct)
    {
        HI_PNG_DestroyDecoder(s32Handle);
        HI_PNG_Close();
        return NULL;
    }

    memset(pstPngStruct, 0, sizeof(hipng_struct_hwctl_s));

    pstPngStruct->u8DecMode |= (HIPNG_SWDEC_MASK | HIPNG_HWDEC_MASK);
    pstPngStruct->u8UserDecMode = (HIPNG_SWDEC_MASK | HIPNG_HWDEC_MASK);
    pstPngStruct->s32Handle = s32Handle;
    pstPngStruct->u32InflexionSize = PNG_INFLEXION_SIZE;

    return pstPngStruct;
}

/*****************************************************************
* func:	Destroy hardware decoder
            CNcomment:����Ӳ��������Ϣ�ṹ��
* in:	pstHwctlStruct A pointer to decoder struct  CNcomment:������Ϣ�ṹ��
* out:	none
* ret:   none
* others:	
*****************************************************************/
HI_VOID hipng_destroy_read_struct_hw(hipng_struct_hwctl_s *pstHwctlStruct)
{
    if (NULL == pstHwctlStruct)
    {
        return;
    }

    HI_PNG_DestroyDecoder(pstHwctlStruct->s32Handle);

    if (pstHwctlStruct->pallateaddr != 0)
    {
        HI_MMZ_Delete(pstHwctlStruct->pallateaddr);
        pstHwctlStruct->pallateaddr = 0;
    }

    HI_PNG_Close();

    free(pstHwctlStruct);

    return;
}

/*Read stream into decoder buf*/
/*CNcomment:������buf��д������*/
HI_S32 hipng_read_stream(png_structp png_ptr)
{
    HI_PNG_BUF_S stBuf = {0, 0};
    HI_S32 s32Ret = HI_SUCCESS;
    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
    HI_UCHAR ucChunkHead[PNG_CHUNK_HEAD_LEN] = {0, 0, 0, 0, 'I', 'D', 'A', 'T'};
    HI_UCHAR *pBuf = NULL;      /*Pointer point to buf head*//*CNcomment:����bufͷָ��*/
    HI_UCHAR *pWrite = NULL;    /*Pointer ponit to write position*//*CNcomment:����bufдָ��*/
    HI_BOOL bNewChunk = HI_TRUE;    /*Write head data flag*//*CNcomment: �Ƿ�дͷ������*/
    HI_U8 u8ReadHeadCount = 0;      /*Written length of chunk header*//*CNcomment:��д��buf��chunkͷ���� */
    HI_U32 u32IdatSize = png_ptr->idat_size + PNG_CHUNK_HEAD_LEN + PNG_CHUNK_TAIL_LEN;
    HI_U32 u32Len = 0;

    HI_PNG_BUF_S stTempBuf = {0};
    HI_BOOL bEnd = HI_FALSE;

    ucChunkHead[0] = (png_ptr->idat_size >> 24) & 0xff;
    ucChunkHead[1] = (png_ptr->idat_size >> 16) & 0xff;
    ucChunkHead[2] = (png_ptr->idat_size >> 8) & 0xff;
    ucChunkHead[3] = png_ptr->idat_size & 0xff;

    while(!bEnd)
    {
        if ((0 == stBuf.u32Size) && (0 == stTempBuf.u32Size))
        {
            stBuf.u32Size = u32IdatSize;
            s32Ret = HI_PNG_AllocBuf(pstHwStruct->s32Handle, &stBuf);
            if (s32Ret < 0)
            {
                return HI_FAILURE;
            }
            pBuf = (HI_UCHAR *)stBuf.pVir;
            pWrite = pBuf;
        }
        else if (0 == stBuf.u32Size)
        {
            memcpy(&stBuf, &stTempBuf, sizeof(HI_PNG_BUF_S));
            memset(&stTempBuf, 0, sizeof(HI_PNG_BUF_S));
            pBuf = (HI_UCHAR *)stBuf.pVir;
            pWrite = pBuf;
        }

        /*Write IDAT header*/
        /*CNcomment:дIDATͷ������*/
        if (bNewChunk)
        {
            u32Len = (PNG_CHUNK_HEAD_LEN - u8ReadHeadCount) < stBuf.u32Size ? (PNG_CHUNK_HEAD_LEN - u8ReadHeadCount):stBuf.u32Size;
            memcpy(pWrite, &ucChunkHead[u8ReadHeadCount], u32Len);
            pWrite += u32Len;
            u32IdatSize -= u32Len;
            stBuf.u32Size -= u32Len;
            u8ReadHeadCount += u32Len;
            if (u8ReadHeadCount == PNG_CHUNK_HEAD_LEN)
            {
                bNewChunk = HI_FALSE;
            }
        }

        /*Write IDAT data*/
        /*CNcomment:дIDAT����*/
        if (u32IdatSize)
        {
            u32Len = (u32IdatSize < stBuf.u32Size)?u32IdatSize:stBuf.u32Size;
            png_read_data(png_ptr, pWrite, u32Len);
            u32IdatSize -= u32Len;
            stBuf.u32Size -= u32Len;
            pWrite += u32Len;             
        }

        /*Before reading next chunk data, checking if these is enough buf to write chunk head
        is required.If buf is not enough, alloc buf.*/
        /*CNcomment:�ڶ���һ��chunkǰ,�����ȱ�֤���㹻�Ŀռ�дͷ��;�������
        ʣ��ռ䲻��,������buf*/
        else if ((stBuf.u32Size < PNG_CHUNK_HEAD_LEN) && (HI_NULL == stTempBuf.pVir))
        {
            s32Ret = HI_PNG_AllocBuf(pstHwStruct->s32Handle, &stTempBuf);
            if (s32Ret < 0)
            {
                //stBuf.u32Size = pWrite - pBuf;
                s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);
                return HI_FAILURE;
            }
        }
        /*Read next chunk*/
        /*CNcomment:��ȡ��һ��chunk */
        else
        {
            PNG_IDAT;
            png_ptr->idat_size = png_read_chunk_header(png_ptr);
            if (png_memcmp(png_ptr->chunk_name, png_IDAT, 4) == 0)
            {
                bNewChunk = HI_TRUE;
                u8ReadHeadCount = 0;
                u32IdatSize = png_ptr->idat_size + PNG_CHUNK_HEAD_LEN + PNG_CHUNK_TAIL_LEN;
                
                ucChunkHead[0] = (png_ptr->idat_size >> 24) & 0xff;
                ucChunkHead[1] = (png_ptr->idat_size >> 16) & 0xff;
                ucChunkHead[2] = (png_ptr->idat_size >> 8) & 0xff;
                ucChunkHead[3] = png_ptr->idat_size & 0xff;
            }
            else
            {
                bEnd = HI_TRUE;
                //stBuf.u32Size = pWrite - pBuf;
                s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);

                pstHwStruct->idat_size = png_ptr->idat_size;
                pstHwStruct->crc = png_ptr->crc;
                memcpy(pstHwStruct->chunk_name, png_ptr->chunk_name, 5);
            }
        }

        /*Set stream length*/
        /*CNcomment:������������*/
        if (0 == stBuf.u32Size)
        {
            //stBuf.u32Size = pWrite - pBuf;
            s32Ret = HI_PNG_SetStreamLen(pstHwStruct->s32Handle, stBuf.u32PhyAddr, pWrite - pBuf);
            memset(&stBuf, 0, sizeof(HI_PNG_BUF_S));
        }
    }

    return HI_SUCCESS;
}

HI_S32 hipng_set_transform(png_structp png_ptr, HI_PNG_TRANSFORM_S *pstTransform)
{
    HI_U32 *pu32Clut = HI_NULL;
    HI_U32 i = 0;
    HI_U8 u8Alpha = 0;
    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)(png_ptr->private_ptr);
    
    pstTransform->e124To8Mode = HI_PNG_124TO8_ACCURATE;
    pstTransform->e16To8Mode = HI_PNG_16TO8_BRIEF;

    /*1.If image is gray with depth=1,2,4 bit, transform image to 8 bits;transparence process
        2.If image is palette, transform image to true color*/
    /*CNcommetn:1.����1/2/4bit�ĻҶ�ͼ����չΪ8bit; ͸��ɫ����
        2.���ǵ�ɫ��ͼ��תΪRGBͼ */
    if (png_ptr->transformations & PNG_EXPAND)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADJUSTPIXELORDER;
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_GRAY124TO8;
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_CLUT2RGB;
        if (png_ptr->color_type & PNG_COLOR_MASK_PALETTE)
        {
            /*Alloc palette*/
            /*CNcomment:�����ɫ��*/
            pstTransform->u32ClutPhyaddr = (HI_U32)HI_MMZ_New(256 * 4, 16, HI_NULL, "PNG_PALETTE");
            if (0 == pstTransform->u32ClutPhyaddr)
            {
                return HI_FAILURE;
            }

            pu32Clut = (HI_U32 *)HI_MMZ_Map(pstTransform->u32ClutPhyaddr, 0);
            if (HI_NULL == pu32Clut)
            {
                HI_MMZ_Delete(pstTransform->u32ClutPhyaddr);
                return HI_FAILURE;
            }

            pstHwStruct->pallateaddr = pstTransform->u32ClutPhyaddr;

            if (png_ptr->num_trans || (png_ptr->transformations & PNG_ADD_ALPHA))
            {
                pstHwStruct->bPallateAlpha = HI_TRUE;
            }
            else
            {
                pstHwStruct->bPallateAlpha = HI_FALSE;
            }

            pstTransform->bClutAlpha = pstHwStruct->bPallateAlpha;
            
            for (i = 0; i < png_ptr->num_palette; i++)
            {
                if (png_ptr->num_trans)
                {
                    if (i < png_ptr->num_trans)
                    {
                        u8Alpha = png_ptr->trans_alpha[i];
                    }
                    else
                    {
                        u8Alpha = 0xff;
                    }
                }
                else
                {
                    if (png_ptr->transformations & PNG_ADD_ALPHA)
                    {
                        u8Alpha = png_ptr->filler;
                    }
                    else
                    {
                        u8Alpha = 0xff;
                    }
                }
                *(pu32Clut + i) = (u8Alpha << 24) | (png_ptr->palette[i].red << 16)
                    | (png_ptr->palette[i].green << 8) | png_ptr->palette[i].blue;
            }

            HI_MMZ_Unmap(pstTransform->u32ClutPhyaddr);
        }
    }

    /*Swap pixels*/
    /*CNcomment:��������˳��*/
    if (png_ptr->transformations & PNG_PACKSWAP)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADJUSTPIXELORDER;
    }

    if (png_ptr->transformations & PNG_GRAY_TO_RGB)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_GRAY2RGB;
    }

    if ((png_ptr->transformations & PNG_EXPAND_tRNS) && png_ptr->num_trans)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADDALPHA;
        pstTransform->eAddAlphaMode = HI_PNG_ADDALPHA_TRNS;
        if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
        {
            pstTransform->sTrnsInfo.u16Red = png_ptr->trans_color.red;
            pstTransform->sTrnsInfo.u16Green = png_ptr->trans_color.green;
            pstTransform->sTrnsInfo.u16Blue = png_ptr->trans_color.blue;
        }
        else
        {
            pstTransform->sTrnsInfo.u16Blue = png_ptr->trans_color.gray;
        }
        pstTransform->u16Filler = 0xffff;
    }
    else if (png_ptr->transformations & PNG_ADD_ALPHA)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_ADDALPHA;
        pstTransform->eAddAlphaMode = HI_PNG_ADDALPHA_BRIEF;
        pstTransform->u16Filler = png_ptr->filler;
    }

    if (png_ptr->flags & PNG_FLAG_STRIP_ALPHA)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_STRIPALPHA;
    }

    if (png_ptr->transformations & PNG_BGR)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_BGR2RGB;
    }

    if (png_ptr->transformations & PNG_SWAP_ALPHA)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_SWAPALPHA;
    }

    if (png_ptr->transformations & PNG_16_TO_8)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_16TO8;
    }

    if (png_ptr->transformations & HIPNG_ARGB1555)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
        pstTransform->eOutFmt = HI_PNG_COLORFMT_ARGB1555;
    }

    if (png_ptr->transformations & HIPNG_ARGB4444)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
        pstTransform->eOutFmt = HI_PNG_COLORFMT_ARGB4444;
    }

    if (png_ptr->transformations & HIPNG_RGB565)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
        pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB565;
    }

    if (png_ptr->transformations & HIPNG_RGB555)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
        pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB555;
    }

    if (png_ptr->transformations & HIPNG_RGB444)
    {
        pstTransform->u32Transform |= HI_PNG_TRANSFORM_8TO4;
        pstTransform->eOutFmt = HI_PNG_COLORFMT_RGB444;
    }
    if(png_ptr->transformations & PNG_PREMULTI_ALPHA)
    {   
        #ifdef CONFIG_PNG_PREMULTIALPHA_ENABLE
            pstTransform->u32Transform |= HI_PNG_TRANSFORM_PREMULTI_ALPHA;
        #else
            return HI_FAILURE;
        #endif
    }
    
    return HI_SUCCESS;
}

/*****************************************************************
* func:	Start hardware decode       CNcomment:pngӲ������
* in:	png_ptr png   decoder ptr     CNcomment:����ṹ��
* out:	none
* ret:	HI_SUCCESS	Success     CNcomment:����ɹ�
*		HI_FAILURE	Failure     CNcomment:����ʧ��
* others:	
*****************************************************************/
HI_S32 hipng_read_image_hw(png_structp png_ptr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)png_ptr->private_ptr;
    HI_PNG_DECINFO_S stInfo = {0};

    /*Read stream*/
    /*CNcomment:��ȡ����*/
    s32Ret = hipng_read_stream(png_ptr);
    if (s32Ret < 0)
    {
        return HI_FAILURE;
    }

    s32Ret = hipng_set_transform(png_ptr, &stInfo.stTransform);
    if (s32Ret < 0)
    {
        return HI_FAILURE;
    }

    stInfo.bSync = pstHwStruct->bSyncDec;

    stInfo.stPngInfo.eColorFmt = png_ptr->color_type;
    stInfo.stPngInfo.u32Width = png_ptr->width;
    stInfo.stPngInfo.u32Height = png_ptr->height;
    stInfo.stPngInfo.u8BitDepth = png_ptr->bit_depth;
    //stInfo.stPngInfo.u8InterlaceType = png_ptr->interlaced;

    stInfo.u32Phyaddr = pstHwStruct->u32Phyaddr;
    stInfo.u32Stride = (pstHwStruct->u32Stride + 0xf) & 0xfffffff0;
    
    s32Ret = HI_PNG_Decode(pstHwStruct->s32Handle, &stInfo);
    if (s32Ret < 0)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hipng_get_readfn_hw(png_structp png_ptr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)png_ptr->private_ptr;
    
    s32Ret = HI_PNG_GetReadPtr(pstHwStruct->s32Handle, &pstHwStruct->read_data_fn_hw);
    if (s32Ret < 0)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hipng_get_result(png_structp png_ptr, HI_BOOL bBlock)
{
    HI_S32 s32Ret = HI_SUCCESS;
    hipng_struct_hwctl_s *pstHwStruct = (hipng_struct_hwctl_s *)png_ptr->private_ptr;
    
    s32Ret = HI_PNG_GetResult(pstHwStruct->s32Handle, bBlock, &pstHwStruct->eState);
    if (s32Ret < 0)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
