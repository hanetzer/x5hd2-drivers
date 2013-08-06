#include "drv_venc_efl.h"
#include "drv_venc_osal.h"
#include "drv_mmz_ext.h"
#include "drv_mem_ext.h"

#include "hal_venc.h"

//#include "drv_vi_ext.h"
//#include "drv_vo_ext.h"
//#include "drv_module_ext.h"

#include "drv_vpss_ext.h"
#include "drv_vdec_ext.h"
#include "hi_drv_vpss.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************************************************************************/

#define MAX_VEDU_CHN 8

#define MAX_VEDU_QUEUE_NUM 6
#define INVAILD_CHN_FLAG   (-1)

/*******************************************************************/
/*#define VENC_YUV_420    (0)
#define VENC_YUV_422    (1)
#define VENC_YUV_444    (2)
#define VENC_YUV_NONE   (3)

#define VENC_STORE_SEMIPLANNAR (0)
#define VENC_STORE_PLANNAR     (1)
#define VENC_STORE_PACKAGE     (2)

#define VENC_PACKAGE_UY0VY1    (0b10001101)
#define VENC_PACKAGE_Y0UY1V    (0b11011000)
#define VENC_PACKAGE_Y0VY1U    (0b01111000)*/

enum {
	VENC_YUV_420	= 0,
	VENC_YUV_422	= 1,
	VENC_YUV_444    = 2,
	VENC_YUV_NONE   = 3
};

enum {
    VENC_V_U        = 0,
    VENC_U_V    	= 1
};


enum {
	VENC_STORE_SEMIPLANNAR	= 0,
	VENC_STORE_PACKAGE   	= 1,
	VENC_STORE_PLANNAR      = 2
};

enum {
	VENC_PACKAGE_UY0VY1  	= 0b10001101,
	VENC_PACKAGE_Y0UY1V	    = 0b11011000,
	VENC_PACKAGE_Y0VY1U     = 0b01111000
};

enum {
    VEDU_CAP_LEVEL_QCIF = 0, /**<The resolution of the picture to be decoded is less than or equal to 176x144.*/ /**<CNcomment: 解码的图像大小不超过176*144 */
    VEDU_CAP_LEVEL_CIF  = 1,      /**<The resolution of the picture to be decoded less than or equal to 352x288.*/ /**<CNcomment: 解码的图像大小不超过352*288 */
    VEDU_CAP_LEVEL_D1   = 2,       /**<The resolution of the picture to be decoded less than or equal to 720x576.*/ /**<CNcomment: 解码的图像大小不超过720*576 */  
    VEDU_CAP_LEVEL_720P = 3,     /**<The resolution of the picture to be decoded is less than or equal to 1280x720.*/ /**<CNcomment: 解码的图像大小不超过1280*720 */
    VEDU_CAP_LEVEL_1080P= 4,   /**<The resolution of the picture to be decoded is less than or equal to 1920x1080.*/ /**<CNcomment: 解码的图像大小不超过1920*1080 */ 
    VEDU_CAP_LEVEL_BUTT    
} ;
/*******************************************************************/

static VeduEfl_ChnCtx_S VeduChnCtx[MAX_VEDU_CHN];

static VeduEfl_IpCtx_S VeduIpCtx;

extern OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];

extern VEDU_OSAL_EVENT g_VencWait_Stream[VENC_MAX_CHN_NUM];
extern VPSS_EXPORT_FUNC_S *pVpssFunc;
wait_queue_head_t gqueue;
volatile HI_U32 gwait;

extern volatile HI_U32 gencodeframe;
extern wait_queue_head_t gEncodeFrame;

VEDU_OSAL_EVENT g_VENC_Event;

//extern HI_S32  HI_DRV_VPSS_GetPortFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);           //add
//extern HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);



extern HI_S8 PriorityTab[2][MAX_VEDU_CHN];                //add by l00228308

static HI_S32 s32LastQueryNo = INVAILD_CHN_FLAG;

#define D_VENC_GET_CHN(s32VeChn, hVencChn) \
    do {\
        s32VeChn = 0; \
        while (s32VeChn < MAX_VEDU_CHN)\
        {   \
            if (g_stVencChn[s32VeChn].hVEncHandle == hVencChn)\
            { \
                break; \
            } \
            s32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_CHN_FORM_VPSS(hVPSS, hVencChn) \
    do {\
        HI_U32 i = 0; \
        for (i = 0;i < MAX_VEDU_CHN;i++)\
        {   \
            if (g_stVencChn[i].hVPSS == hVPSS)\
            { \
                hVencChn = g_stVencChn[i].hVEncHandle;\
                break; \
            } \
        } \
    } while (0)
                        
#define D_VENC_CHECK_ALL_EQUAL(wh,wt,rh,rt,flag)\
    do{  \
         if( (wh == wt) && (rh == rt) && (wt == rh))\
            flag = HI_TRUE; \
         else               \
            flag = HI_FALSE; \
      } while (0)      

#define D_VENC_GET_CHN_FROM_TAB(u32ChnID,TabNO)\
    do{  \
         u32ChnID = PriorityTab[0][TabNO];\
      } while (0)

static HI_VOID H264e_PutTrailingBits(tBitStream *pBS)
{
    VENC_DRV_BsPutBits31(pBS, 1, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0, 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_VOID MP4e_PutTrailingBits(tBitStream *pBS)
{
    VENC_DRV_BsPutBits31(pBS, 0, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0xFF >> (pBS->totalBits & 7), 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_U32 H264e_MakeSPS(HI_U8 *pSPSBuf, const VeduEfl_H264e_SPS_S *pSPS)
{
    HI_U32 code, TotalMb, profile_idc, level_idc,direct_8x8_interence_flag;
    int bits;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pSPSBuf);

    TotalMb = pSPS->FrameWidthInMb * pSPS->FrameHeightInMb;

    if (TotalMb <= 99)
    {
        level_idc = 10;
    }
    else if (TotalMb <= 396)
    {
        level_idc = 20;
    }
    else if (TotalMb <= 792)
    {
        level_idc = 21;
    }
    else if (TotalMb <= 1620)
    {
        level_idc = 30;
    }
    else if (TotalMb <= 3600)
    {
        level_idc = 31;
    }
    else if (TotalMb <= 5120)
    {
        level_idc = 32;
    }
    else if (TotalMb <= 8192)
    {
        level_idc = 42;
    }
    else
    {
        level_idc = 0;
    }

    if (TotalMb < 1620)
    {
        direct_8x8_interence_flag = 0;
    }
    else
    {
        direct_8x8_interence_flag = 1;
    }
	

    //profile_idc = 66;
    profile_idc = pSPS->ProfileIDC;

    VENC_DRV_BsPutBits32(&BS, 1, 32);

    VENC_DRV_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    VENC_DRV_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    VENC_DRV_BsPutBits31(&BS, 7, 5); // nal_unit_type

    VENC_DRV_BsPutBits31(&BS, profile_idc, 8);
    VENC_DRV_BsPutBits31(&BS, 0x00, 8);
    VENC_DRV_BsPutBits31(&BS, level_idc, 8);

    VENC_DRV_BsPutBits31(&BS, 1, 1); // ue, sps_id = 0

   if(100 == pSPS->ProfileIDC)   //just for high profile
   {
        VENC_DRV_BsPutBits31(&BS, 0x2, 3);
        VENC_DRV_BsPutBits31(&BS, 0xC, 4);
   }

    VENC_DRV_BsPutBits31(&BS, 1, 1); // ue, log2_max_frame_num_minus4 = 0

    VENC_DRV_BsPutBits31(&BS, 3, 3); // ue, pic_order_cnt_type = 2
    VENC_DRV_BsPutBits31(&BS, 3, 3); // ue, num_ref_frames = 1 (or 2)
    VENC_DRV_BsPutBits31(&BS, 0, 1); // u1, gaps_in_frame_num_value_allowed_flag

    ue_vlc(bits, code, (pSPS->FrameWidthInMb - 1));
    VENC_DRV_BsPutBits31(&BS, code, bits);
    ue_vlc(bits, code, (pSPS->FrameHeightInMb - 1));
    VENC_DRV_BsPutBits31(&BS, code, bits);

    VENC_DRV_BsPutBits31(&BS, 1, 1); // u1, frame_mbs_only_flag = 1 (or 0)

    if (0)                // !pSPS->FrameMbsOnlyFlag
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1); // mb_adaptive_frame_field_flag = 0
        VENC_DRV_BsPutBits31(&BS, 1, 1); // direct_8x8_inference_flag
    }
    else
    {
        VENC_DRV_BsPutBits31(&BS, direct_8x8_interence_flag, 1); // direct_8x8_inference_flag
    }

    {
        int bFrameCropping = ((pSPS->FrameCropLeft | pSPS->FrameCropRight |
                               pSPS->FrameCropTop | pSPS->FrameCropBottom) != 0);

        VENC_DRV_BsPutBits31(&BS, bFrameCropping, 1);

        if (bFrameCropping)
        {
            ue_vlc(bits, code, pSPS->FrameCropLeft);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropRight);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropTop);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropBottom);
            VENC_DRV_BsPutBits31(&BS, code, bits);
        }
    }
    VENC_DRV_BsPutBits31(&BS, 0, 1); // vui_parameters_present_flag
    H264e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_U32 H264e_MakePPS(HI_U8 *pPPSBuf, const VeduEfl_H264e_PPS_S *pPPS)
{
    HI_U32 code;
    int bits;
    HI_U32 b = pPPS->H264CabacEn ? 1 : 0;

    tBitStream BS;

    HI_U8 zz_scan_table[64] = 
    {
         0,  1,  8, 16,  9,  2,  3, 10,
        17, 24, 32, 25, 18, 11,  4,  5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13,  6,  7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pPPSBuf);

    VENC_DRV_BsPutBits32(&BS, 1, 32);

    VENC_DRV_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    VENC_DRV_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    VENC_DRV_BsPutBits31(&BS, 8, 5); // nal_unit_type

    VENC_DRV_BsPutBits31(&BS, 1, 1); // pps_id = 0
    VENC_DRV_BsPutBits31(&BS, 1, 1); // sps_id = 0

    VENC_DRV_BsPutBits31(&BS, b, 1); // entropy_coding_mode_flag = 0
    VENC_DRV_BsPutBits31(&BS, 0, 1); // pic_order_present_flag
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_slice_groups_minus1
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_l0_active_minus1 = 0 (or 1)
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_l1_active_minus1 = 0
    VENC_DRV_BsPutBits31(&BS, 0, 3); // weighted_pred_flag & weighted_bipred_idc
    VENC_DRV_BsPutBits31(&BS, 3, 2); // pic_init_qp_minus26 & pic_init_qs_minus26

    se_vlc(bits, code, pPPS->ChrQpOffset); // chroma_qp_index_offset
    VENC_DRV_BsPutBits31(&BS, code, bits);

    VENC_DRV_BsPutBits31(&BS, 1, 1);                // deblocking_filter_control_present_flag
    VENC_DRV_BsPutBits31(&BS, pPPS->ConstIntra, 1); // constrained_intra_pred_flag

    VENC_DRV_BsPutBits31(&BS, 0, 1);                // redundant_pic_cnt_present_flag

    if (pPPS->H264HpEn)
    {
      int i, j;
      
      VENC_DRV_BsPutBits31(&BS, 1, 1); // transform_8x8_mode_flag
      VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_matrix_present_flag
      
      for(i = 0; i < 6; i++)
      {
        VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag
        
        se_vlc(bits, code, 8);  
		VENC_DRV_BsPutBits31(&BS, code, bits); /* all be 16 */
        for(j = 0; j < 15; j++)
		{
		    VENC_DRV_BsPutBits31(&BS, 1, 1);
        }
      }
      
      for(i = 0; i < 2; i++)
      {
        int lastScale, currScale, deltaScale;
        HI_S32 *pList = pPPS->pScale8x8;
  
        if(i==1) pList = pPPS->pScale8x8 + 64;
        
        VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag
        
        for(lastScale = 8, j = 0; j < 64; j++)
        {
          currScale  = (int)(pList[zz_scan_table[j]]);
          deltaScale = currScale - lastScale;
          if     (deltaScale < -128) deltaScale += 256;
          else if(deltaScale >  127) deltaScale -= 256;
          se_vlc(bits, code, deltaScale);
          VENC_DRV_BsPutBits31(&BS, code, bits);
          lastScale = currScale;
        }
      }
      se_vlc(bits, code, pPPS->ChrQpOffset); 
	  VENC_DRV_BsPutBits31(&BS, code, bits);
    }
	
    H264e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_U32 H264e_MakeSlcHdr(HI_U32 *pHdrBuf, HI_U32 *pReorderBuf, HI_U32 *pMarkBuf,
                               const VeduEfl_H264e_SlcHdr_S*pSlcHdr)
{
    HI_U32 code   = 0;
    HI_U32 buf[8] = {0};
    int bits, i, bitPara;

    static HI_U32 idr_pic_id = 0;
    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    ue_vlc(bits, code, pSlcHdr->slice_type);
    VENC_DRV_BsPutBits31(&BS, code, bits);                                        // slice_type, 0(P) or 2(I)

    VENC_DRV_BsPutBits31(&BS, 1, 1);                      // pic_parameter_set_id
    VENC_DRV_BsPutBits31(&BS, pSlcHdr->frame_num & 0xF, 4); // frame number

    if (pSlcHdr->slice_type == 2) // all I Picture be IDR
    {
        ue_vlc(bits, code, idr_pic_id & 0xF);
        VENC_DRV_BsPutBits31(&BS, code, bits);
        idr_pic_id++;
    }
    else if(pSlcHdr->NumRefIndex ==  0)
    {
      VENC_DRV_BsPutBits31(&BS, 0, 1); // num_ref_idx_active_override_flag 
    } 
    else
    {
      VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_active_override_flag 
      ue_vlc(bits, code, pSlcHdr->NumRefIndex); 
	  VENC_DRV_BsPutBits31(&BS, code, bits);
    } 

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    bitPara = BS.totalBits;

    /****** RefPicListReordering() ************************************/

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits31(&BS, 0, 1);/* ref_pic_list_reordering_flag_l0 = 0 ("0") */

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pReorderBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32)
        {
            pReorderBuf[i] >>= (32 - bits);
        }
    }

    bitPara |= BS.totalBits << 8;

    /****** DecRefPicMarking() *****************************************/

    VENC_DRV_BsOpenBitStream(&BS, buf);

    if (pSlcHdr->slice_type == 2)
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* no_output_of_prior_pics_flag */
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* long_term_reference_flag     */
    }
    else
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* adaptive_ref_pic_marking_mode_flag */
    }

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pMarkBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32)
        {
            pMarkBuf[i] >>= (32 - bits);
        }
    }

    bitPara |= BS.totalBits << 16;
    return (HI_U32)bitPara;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_U32 H263e_MakePicHdr(HI_U32 *pHdrBuf, const VeduEfl_H263e_PicHdr_S *pPicHdr)
{
    HI_U32 buf[8] = {0};
    int bits, i;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits31(&BS, 0x20, 22);             // start code
    VENC_DRV_BsPutBits31(&BS, pPicHdr->TR & 0xFF, 8); // temporal ref
    VENC_DRV_BsPutBits31(&BS, 0x10, 5);              // PTYPE bit 1-5
    VENC_DRV_BsPutBits31(&BS, 7, 3);                 // PTYPE bit 6-8

    /* PLUSPTYPE */
    VENC_DRV_BsPutBits31(&BS, 1, 3);                 // UFEP
    VENC_DRV_BsPutBits31(&BS, pPicHdr->SrcFmt, 3);   // OPPTYPE: Source Format
    VENC_DRV_BsPutBits31(&BS, 0, 11);                // OPPTYPE: C-PCF ~ MQ
    VENC_DRV_BsPutBits31(&BS, 8, 4);                 // OPPTYPE: Reserved
    VENC_DRV_BsPutBits31(&BS, pPicHdr->Pframe, 3);   // MPPTYPE: Pic Type - 0:I, 1:P
    VENC_DRV_BsPutBits31(&BS, 0, 3);                 // MPPTYPE: RPR, RRU, Rounding
    VENC_DRV_BsPutBits31(&BS, 1, 3);                 // MPPTYPE: Reserved

    VENC_DRV_BsPutBits31(&BS, 0, 1);                 // CPM

    if (pPicHdr->SrcFmt == 6)
    {
        VENC_DRV_BsPutBits31(&BS, 2, 4);
        VENC_DRV_BsPutBits31(&BS, pPicHdr->Wframe / 4 - 1, 9);
        VENC_DRV_BsPutBits31(&BS, 1, 1);
        VENC_DRV_BsPutBits31(&BS, pPicHdr->Hframe / 4, 9);
    }

    VENC_DRV_BsPutBits31(&BS, pPicHdr->PicQP, 5);
    VENC_DRV_BsPutBits31(&BS, 0, 1); // PEI

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    return (HI_U32)BS.totalBits;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_U32 MP4e_MakeVOL(HI_U8 *pVOLBuf, const VeduEfl_MP4e_VOL_S *pVOL)
{
    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pVOLBuf);

    VENC_DRV_BsPutBits32(&BS, 0x0101, 32);  // video_object_start_code
    VENC_DRV_BsPutBits32(&BS, 0x0120, 32);  // video_object_layer_start_code
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // random_accessible_vol
    VENC_DRV_BsPutBits31(&BS, 1, 8);        // video_object_type_indication: simple

    VENC_DRV_BsPutBits31(&BS, 0, 1);        // is_object_layer_identifier: 0
    VENC_DRV_BsPutBits31(&BS, 1, 4);        // aspect_ration_info: 0001

    VENC_DRV_BsPutBits31(&BS, 1, 1);        // vol_control_para: 1
    VENC_DRV_BsPutBits31(&BS, 1, 2);        // chroma_format: 01
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // low_dalay
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // vbv_para

    VENC_DRV_BsPutBits31(&BS, 0, 2);        // vol_shape: 00
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit
    VENC_DRV_BsPutBits31(&BS, 25, 16);      //?vop_time_increment_resolution
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // fixed_vop_rate
    //VENC_DRV_BsPutBits31(&BS, 1001, 15);      //?fixed_vop_time_increment
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit

    VENC_DRV_BsPutBits31(&BS, pVOL->Wframe, 13);
    VENC_DRV_BsPutBits31(&BS, 1, 1);
    VENC_DRV_BsPutBits31(&BS, pVOL->Hframe, 13);
    VENC_DRV_BsPutBits31(&BS, 1, 1);
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // interlaced
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // obmc_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // sprite_enable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // not_8_bit

    VENC_DRV_BsPutBits31(&BS, 0, 1);        // quant_type
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // complexity_estimation_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // resync_marker_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // data_partitioned
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // scalabilitu

    MP4e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_U32 MP4e_MakeVopHdr(HI_U32 *pHdrBuf, const VeduEfl_MP4e_VopHdr_S *pVopHdr)
{
    HI_U32 buf[8] = {0};
    int bits, i;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits32(&BS, 0x01B6, 32);           // start code
    VENC_DRV_BsPutBits31(&BS, pVopHdr->Pframe, 2);   // vop_coding_type - 0:I, 1:p
    VENC_DRV_BsPutBits31(&BS, 0, 1);                 //?modulo_time_base
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // marker_bit
    VENC_DRV_BsPutBits31(&BS, 0, 5);                 //?vop_time_increment
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // marker_bit
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // vop_coded

    if (pVopHdr->Pframe)
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);
    }                                     // vop_rounding_type

    VENC_DRV_BsPutBits31(&BS, 0, 3);                 // intra_dc_vlc_thr = 000
    VENC_DRV_BsPutBits31(&BS, pVopHdr->PicQP, 5);

    if (pVopHdr->Pframe)
    {
        VENC_DRV_BsPutBits31(&BS, pVopHdr->Fcode, 3);
    }

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    return (HI_U32)BS.totalBits;
}


/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : flag :0 -> YUVStoreType; 1 -> YUVSampleType; 2 -> PackageSel
******************************************************************************/
#ifdef __VENC_S40V200_CONFIG__  //FOR S40V200
static void Venc_SetRegDefault( VeduEfl_EncPara_S  *pEncPara )
{
    typedef struct
    {
      int  rWnd[2];
      int  thresh[2];
      int  rectMod[6];
      int  range[6][4];
  
    } intSearchIn;
    
    int i;
	HI_U8 Quant8_intra_default[64] =
    {
     6,10,13,16,18,23,25,27,
    10,11,16,18,23,25,27,29,
    13,16,18,23,25,27,29,31,
    16,18,23,25,27,29,31,33,
    18,23,25,27,29,31,33,36,
    23,25,27,29,31,33,36,38,
    25,27,29,31,33,36,38,40,
    27,29,31,33,36,38,40,42
    };

    HI_U8 Quant8_inter_default[64] =
    {
     9,13,15,17,19,21,22,24,
    13,13,17,19,21,22,24,25,
    15,17,19,21,22,24,25,27,
    17,19,21,22,24,25,27,28,
    19,21,22,24,25,27,28,30,
    21,22,24,25,27,28,30,32,
    22,24,25,27,28,30,32,33,
    24,25,27,28,30,32,33,35
    };
    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] = {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
       86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
      870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095 };
    
    static intSearchIn isrD1 = 
    {
        { 5, 2 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isr720p = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isr1080p = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isrWidth0 = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    
    static intSearchIn isrMpeg4 = 
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },
 
        {  { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, 
           {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}  }
    };
    intSearchIn *pIsr;
    
    if     (pEncPara->PicWidth >  2048 ) pIsr = &isrWidth0;
    else if(pEncPara->PicWidth >  1280 ) pIsr = &isr1080p;
    else if(pEncPara->PicWidth >   720 ) pIsr = &isr720p;
    else                                 pIsr = &isrD1;
    
    if(pEncPara->Protocol == VEDU_MPEG4) pIsr = &isrMpeg4;
    
    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 1;

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = pEncPara->H264HpEn ? 1 : 0;
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;           //可提高性能，配置成1

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 1;          //使能补搜索，配置成0可以提高性能
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = pEncPara->H264HpEn ? 0 : 1;
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 1;
    pEncPara->intpLowpowEn     = 1;
    pEncPara->fracLowpowEn     = 1;
    
    for(i = 0; i < 4; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }
	
    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;
    pEncPara->MctfStrength0    = 0;
    pEncPara->MctfStrength1    = 0;
    
    pEncPara->MctfStillEn      = 0;
    pEncPara->MctfMovEn        = 0;
    pEncPara->mctfStillMvThr   = 1;
    pEncPara->mctfRealMvThr    = 2;
    pEncPara->MctfStillCostThr = 0x300;
    pEncPara->MctfLog2mctf     = 2;
    pEncPara->MctfLumaDiffThr  = 0xa;
    pEncPara->MctfChrDiffThr   = 0x5;
    pEncPara->MctfChrDeltaThr  = 0xf;
    pEncPara->MctfStiMadiThr1  = 0x4;
    pEncPara->MctfStiMadiThr2  = 0xa;
    pEncPara->MctfMovMadiThr   = 0xa;
    pEncPara->MctfMovMad1_m    = 8;
    pEncPara->MctfMovMad1_n    = 8;
    pEncPara->MctfMovMad2_m    = 8;
    pEncPara->MctfMovMad2_n    = 8;
    pEncPara->MctfStiMad1_m    = 7;
    pEncPara->MctfStiMad1_n    = 9;
    pEncPara->MctfStiMad2_m    = 8;
    pEncPara->MctfStiMad2_n    = 8;

    pEncPara->StartQpType      = 0;
    
    pEncPara->RcQpDelta        = 4;
    pEncPara->RcMadpDelta      = -8;
    
    for(i = 0; i < 12; i++)    pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i];
    for(i = 0; i < 40; i++)    pEncPara->ModLambda[i]     = ModLambda[i];
    for(i = 0; i < 64; i++)    pEncPara->Scale8x8[i]      = Quant8_intra_default[i];
    for(i = 0; i < 64; i++)    pEncPara->Scale8x8[i+64]   = Quant8_inter_default[i];

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 1;
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 2;
    pEncPara->TimeOut          = 0;
    pEncPara->PtBitsEn         = 1;
    pEncPara->PtBits           = 128000*8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;
    
    for(i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i*5;
        pEncPara->RoiCfg.StartY [i] = i*5;
    }
    for(i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en      [i] = 0;
        pEncPara->OsdCfg.osd_absqp_en[i] = 0;
        pEncPara->OsdCfg.osd_qp      [i] = 0;
        pEncPara->OsdCfg.osd_x       [i] = i * 20;
        pEncPara->OsdCfg.osd_y       [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en [i] = 0;
        pEncPara->OsdCfg.osd_invs_w      = 1;
        pEncPara->OsdCfg.osd_invs_h      = 1;
        pEncPara->OsdCfg.osd_invs_thr    = 127;
    }
}

#elif defined(__VENC_3716CV200_CONFIG__)
static void Venc_SetRegDefault( VeduEfl_EncPara_S  *pEncPara )
{
    typedef struct
    {
      int  rWnd[2];
      int  thresh[2];
      int  rectMod[6];
      int  range[6][4];
  
    } intSearchIn;
    
    int i;
    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] = {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
       86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
      870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095 };
    
    static intSearchIn isrD1 = 
    {
        { 5, 2 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isr720p = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isr1080p = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    static intSearchIn isrWidth0 = 
    {
        { 5, 0 },  { 1000, 1000 },  { 1, 1, 1, 0, 0, 0 },
 
        {  { 4, 4, 0, 0}, { 13, 13, 1, 1}, { 2, 2, 0, 0}, 
           { 0, 0, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}  }
    };
    
    static intSearchIn isrMpeg4 = 
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },
 
        {  { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, 
           {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}  }
    };
	
	HI_U8 Quant8_intra_default[64] =
    {
     6,10,13,16,18,23,25,27,
    10,11,16,18,23,25,27,29,
    13,16,18,23,25,27,29,31,
    16,18,23,25,27,29,31,33,
    18,23,25,27,29,31,33,36,
    23,25,27,29,31,33,36,38,
    25,27,29,31,33,36,38,40,
    27,29,31,33,36,38,40,42
    };

    HI_U8 Quant8_inter_default[64] =
    {
     9,13,15,17,19,21,22,24,
    13,13,17,19,21,22,24,25,
    15,17,19,21,22,24,25,27,
    17,19,21,22,24,25,27,28,
    19,21,22,24,25,27,28,30,
    21,22,24,25,27,28,30,32,
    22,24,25,27,28,30,32,33,
    24,25,27,28,30,32,33,35
    };
	
    intSearchIn *pIsr;
    if     (pEncPara->PicWidth >  2048 ) pIsr = &isrWidth0;
    else if(pEncPara->PicWidth >  1280 ) pIsr = &isr1080p;
    else if(pEncPara->PicWidth >   720 ) pIsr = &isr720p;
    else                                 pIsr = &isrD1;
    
    if(pEncPara->Protocol == VEDU_MPEG4) pIsr = &isrMpeg4;
    
    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 1;

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = pEncPara->H264HpEn ? 1 : 0;
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 1;
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = pEncPara->H264HpEn ? 0 : 1;
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 1;
    pEncPara->intpLowpowEn     = 1;
    pEncPara->fracLowpowEn     = 1;
    
    for(i = 0; i < 3; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }
    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;

    pEncPara->MctfStillEn             =  0     ;
    pEncPara->MctfSmlmovEn            =  0     ;
    pEncPara->MctfBigmovEn            =  0     ;
    pEncPara->mctfStillMvThr          =  4     ; //        m_picOptions.mctf_still_mv_thr 
    pEncPara->mctfRealMvThr           =  4     ; //        m_picOptions.mctf_real_mv_thr 
    pEncPara->MctfStillCostThr        =  2000  ; //        m_picOptions.mctf_still_cost_thr 
    pEncPara->MctfStiLumaOrialpha     =  0     ; //        m_picOptions.mctf_sti_luma_ori_alpha 
    pEncPara->MctfSmlmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov0_luma_ori_alpha 
    pEncPara->MctfBigmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov1_luma_ori_alpha 
    pEncPara->MctfChrOrialpha         =  8     ; //        m_picOptions.mctf_chr_ori_alpha 
    pEncPara->mctfStiLumaDiffThr0     =  4     ; //        m_picOptions.mctf_sti_luma_diff_thr1 
    pEncPara->mctfStiLumaDiffThr1     =  8     ; //        m_picOptions.mctf_sti_luma_diff_thr2 
    pEncPara->mctfStiLumaDiffThr2     =  16    ; //        m_picOptions.mctf_sti_luma_diff_thr3 
    pEncPara->mctfSmlmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov0_luma_diff_thr1 
    pEncPara->mctfSmlmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov0_luma_diff_thr2 
    pEncPara->mctfSmlmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov0_luma_diff_thr3 
    pEncPara->mctfBigmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov1_luma_diff_thr1 
    pEncPara->mctfBigmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov1_luma_diff_thr2 
    pEncPara->mctfBigmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov1_luma_diff_thr3 
    pEncPara->mctfStiLumaDiffK0       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k1   
    pEncPara->mctfStiLumaDiffK1       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k2   
    pEncPara->mctfStiLumaDiffK2       =  32    ; //        m_picOptions.mctf_sti_luma_diff_k3       
    pEncPara->mctfSmlmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k1 
    pEncPara->mctfSmlmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k2 
    pEncPara->mctfSmlmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov0_luma_diff_k3 
    pEncPara->mctfBigmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k1 
    pEncPara->mctfBigmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k2 
    pEncPara->mctfBigmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov1_luma_diff_k3
    pEncPara->mctfChrDiffThr0         =  4     ; //        m_picOptions.mctf_chr_diff_thr1 
    pEncPara->mctfChrDiffThr1         =  8     ; //        m_picOptions.mctf_chr_diff_thr2 
    pEncPara->mctfChrDiffThr2         =  16    ; //        m_picOptions.mctf_chr_diff_thr3 
    pEncPara->mctfChrDiffK0           =  16    ; //        m_picOptions.mctf_chr_diff_k1 
    pEncPara->mctfChrDiffK1           =  16    ; //        m_picOptions.mctf_chr_diff_k2 
    pEncPara->mctfChrDiffK2           =  16    ; //        m_picOptions.mctf_chr_diff_k3 
    pEncPara->mctfOriRatio            =  4     ; //        m_picOptions.mctf_ori_ratio 
    pEncPara->mctfStiMaxRatio         =  4     ; //        m_picOptions.mctf_sti_max_ratio 
    pEncPara->mctfSmlmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov0_max_ratio 
    pEncPara->mctfBigmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov1_max_ratio 
    pEncPara->mctfStiMadiThr0         =  3     ; //        m_picOptions.mctf_sti_madi_thr1 
    pEncPara->mctfStiMadiThr1         =  10    ; //        m_picOptions.mctf_sti_madi_thr2 
    pEncPara->mctfStiMadiThr2         =  20    ; //        m_picOptions.mctf_sti_madi_thr3 
    pEncPara->mctfSmlmovMadiThr0      =  6     ; //        m_picOptions.mctf_mov0_madi_thr1 
    pEncPara->mctfSmlmovMadiThr1      =  16    ; //        m_picOptions.mctf_mov0_madi_thr2 
    pEncPara->mctfSmlmovMadiThr2      =  30    ; //        m_picOptions.mctf_mov0_madi_thr3 
    pEncPara->mctfBigmovMadiThr0      =  15    ; //        m_picOptions.mctf_mov1_madi_thr1 
    pEncPara->mctfBigmovMadiThr1      =  40    ; //        m_picOptions.mctf_mov1_madi_thr2 
    pEncPara->mctfBigmovMadiThr2      =  60    ; //        m_picOptions.mctf_mov1_madi_thr3 
    pEncPara->mctfStiMadiK0           =  0     ; //        m_picOptions.mctf_sti_madi_k1 
    pEncPara->mctfStiMadiK1           =  16    ; //        m_picOptions.mctf_sti_madi_k2 
    pEncPara->mctfStiMadiK2           =  16    ; //        m_picOptions.mctf_sti_madi_k3 
    pEncPara->mctfSmlmovMadiK0        =  0     ; //        m_picOptions.mctf_mov0_madi_k1 
    pEncPara->mctfSmlmovMadiK1        =  10    ; //        m_picOptions.mctf_mov0_madi_k2 
    pEncPara->mctfSmlmovMadiK2        =  16    ; //        m_picOptions.mctf_mov0_madi_k3 
    pEncPara->mctfBigmovMadiK0        =  0     ; //        m_picOptions.mctf_mov1_madi_k1 
    pEncPara->mctfBigmovMadiK1        =  10    ; //        m_picOptions.mctf_mov1_madi_k2 
    pEncPara->mctfBigmovMadiK2        =  16    ; //        m_picOptions.mctf_mov1_madi_k3 
    
    pEncPara->StartQpType      = 0;
    
    pEncPara->RcQpDelta        = 4;
    pEncPara->RcMadpDelta      = -8;
    
    for(i = 0; i < 12; i++)    pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i];
    for(i = 0; i < 40; i++)    pEncPara->ModLambda[i]     = ModLambda[i];
    for(i = 0; i < 64; i++)    pEncPara->Scale8x8[i]      = Quant8_intra_default[i];
    for(i = 0; i < 64; i++)    pEncPara->Scale8x8[i+64]   = Quant8_inter_default[i];

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 1;
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 2;
    pEncPara->TimeOut          = 0;
    pEncPara->PtBitsEn         = 1;
    pEncPara->PtBits           = 128000*8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;
    
    for(i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.Keep   [i] = 0;        
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i*5;
        pEncPara->RoiCfg.StartY [i] = i*5;
    }
    for(i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en      [i] = 0;
        pEncPara->OsdCfg.osd_absqp_en[i] = 0;
        pEncPara->OsdCfg.osd_qp      [i] = 0;
        pEncPara->OsdCfg.osd_x       [i] = i * 20;
        pEncPara->OsdCfg.osd_y       [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en [i] = 0;
        pEncPara->OsdCfg.osd_invs_w      = 1;
        pEncPara->OsdCfg.osd_invs_h      = 1;
        pEncPara->OsdCfg.osd_invs_thr    = 127;
    }
}

#endif

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : flag :0 -> YUVStoreType; 1 -> YUVSampleType; 2 -> PackageSel
******************************************************************************/
static HI_U32 Convert_PIX_Format(HI_DRV_PIX_FORMAT_E oldFormat,HI_U32 flag)
{
   HI_U32 Ret;
   if(0 == flag) /*YUVStoreType*/
   {
     switch(oldFormat)
     {
        case HI_DRV_PIX_FMT_NV61_2X1:    
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_NV21:     
        case HI_DRV_PIX_FMT_NV12:   
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_NV80:     
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_NV12_411:    
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_NV61:     
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_NV42:     
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
        case HI_DRV_PIX_FMT_UYVY:     
            Ret = VENC_STORE_PACKAGE;
            break;
        case HI_DRV_PIX_FMT_YUYV:     
            Ret = VENC_STORE_PACKAGE;
            break;
        case HI_DRV_PIX_FMT_YVYU :     
            Ret = VENC_STORE_PACKAGE;
            break;
        case HI_DRV_PIX_FMT_YUV400:     
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV411:    
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV420p:    
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:     
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1: 
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV_444:    
            Ret = VENC_STORE_PLANNAR;
            break;
        case HI_DRV_PIX_FMT_YUV410p:    
            Ret = VENC_STORE_PLANNAR;
            break;
        default:
            Ret = VENC_STORE_SEMIPLANNAR;
            break; 
     }
   }
   
   if(1 == flag) /*YUVSampleType*/
   {
     switch(oldFormat)
     {
        case HI_DRV_PIX_FMT_NV61_2X1:     
            Ret = VENC_YUV_422;
            break;
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12: 
            Ret = VENC_YUV_420;
            break;
        case HI_DRV_PIX_FMT_NV80:     //400
            Ret = VENC_YUV_NONE;
            break;
        case HI_DRV_PIX_FMT_NV12_411:    
            Ret = VENC_YUV_NONE;
            break;
        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16: 
            Ret = VENC_YUV_422;
            break;
        case HI_DRV_PIX_FMT_NV42:   
            Ret = VENC_YUV_444;
            break;
        case HI_DRV_PIX_FMT_UYVY:    
        case HI_DRV_PIX_FMT_YUYV:     
        case HI_DRV_PIX_FMT_YVYU:     
            Ret = VENC_YUV_422;
            break;
        case HI_DRV_PIX_FMT_YUV400:     
            Ret = VENC_YUV_NONE;
            break;
        case HI_DRV_PIX_FMT_YUV411:   
            Ret = VENC_YUV_NONE;
            break;
        case HI_DRV_PIX_FMT_YUV420p:    
            Ret = VENC_YUV_420;
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:   
            Ret = VENC_YUV_422;
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1:    
            Ret = VENC_YUV_422;
            break;
        case HI_DRV_PIX_FMT_YUV_444:     
            Ret = VENC_YUV_444;
            break;
        case HI_DRV_PIX_FMT_YUV410p:    
            Ret = VENC_YUV_NONE;
            break;
        default:
            Ret = VENC_YUV_NONE;
            break;
     }
   }

   if(2 == flag) /*PackageSel*/
   {
     switch(oldFormat)
     {
        case HI_DRV_PIX_FMT_NV61_2X1:     
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_NV21:
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_NV12: 
            Ret = VENC_U_V;
            break;
        case HI_DRV_PIX_FMT_NV80:     //400
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_NV12_411:    
            Ret = VENC_U_V;
            break;
        case HI_DRV_PIX_FMT_NV61:
            Ret = VENC_V_U;
            break;            
        case HI_DRV_PIX_FMT_NV16: 
            Ret = VENC_U_V;
            break;
        case HI_DRV_PIX_FMT_NV42:   
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_UYVY:    
            Ret = VENC_PACKAGE_UY0VY1;
            break;
        case HI_DRV_PIX_FMT_YUYV:     
            Ret = VENC_PACKAGE_Y0UY1V;
            break;
        case HI_DRV_PIX_FMT_YVYU :     
            Ret = VENC_PACKAGE_Y0VY1U;
            break;
        case HI_DRV_PIX_FMT_YUV400:     
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_YUV411:   
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_YUV420p:    
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:   
            Ret = VENC_U_V;
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1:    
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_YUV_444:     
            Ret = VENC_V_U;
            break;
        case HI_DRV_PIX_FMT_YUV410p:    
            Ret = VENC_V_U;
            break;
        default:
            Ret = VENC_YUV_NONE;
            break;
     }
   }
   
   return Ret;
}

static HI_S32 QuickEncode_Process(HI_HANDLE EncHandle,HI_BOOL bEvenGetImg)          //成功取帧返回 HI_SUCCESS,连一次都取不成功返回HI_FAILURE
{
    HI_BOOL bLastFrame = HI_FALSE;
    HI_DRV_VIDEO_FRAME_S stImage_temp;
    VeduEfl_EncPara_S *pEncPara;
    pEncPara = (VeduEfl_EncPara_S *)EncHandle;

	if (!bEvenGetImg)        /*never get Img before */
	{
	    pEncPara->stStat.GetFrameNumTry++;
	    if( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage))
	    {
	        pEncPara->stStat.GetFrameNumOK++;
	        while(!bLastFrame)
	        {
	           pEncPara->stStat.GetFrameNumTry++;
	           if( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(pEncPara->stSrcInfo.handle, &stImage_temp))
	           {
	                pEncPara->stStat.GetFrameNumOK++; 
	                pEncPara->stStat.PutFrameNumTry++; 
	                (*pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
	                pEncPara->stStat.PutFrameNumOK++;
	                pEncPara->stImage = stImage_temp;
	           }
	           else
	           {
	               bLastFrame = HI_TRUE;
	           }
	        }
	     }
	     else
	     {
	        return HI_FAILURE;
	     }
	 }
	 else                /* already get one Img before*/      
     {
         pEncPara->stStat.GetFrameNumTry++;
		 while(!bLastFrame)
	     {
			 if( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(pEncPara->stSrcInfo.handle, &stImage_temp))
	         {
	              pEncPara->stStat.GetFrameNumOK++; 
	              pEncPara->stStat.PutFrameNumTry++; 
	              (*pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
	              pEncPara->stStat.PutFrameNumOK++;
	              pEncPara->stImage = stImage_temp;
	         }
	         else
	         {
	             bLastFrame = HI_TRUE;
	         }
		 }
     }
     return HI_SUCCESS;
}

#ifdef VENC_TO_VPSS_SUPPORT
HI_S32 VENC_DRV_EflToVPSSGetImge(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImageInfo)   //给VPSS的回调,主动取帧
{
    HI_S32 Ret;
    HI_HANDLE EncHandle = HI_INVALID_HANDLE;
    VeduEfl_EncPara_S  *pEncPara;
    
    D_VENC_GET_CHN_FORM_VPSS(hVPSS, EncHandle);
    pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    
    Ret =(pEncPara->stSrcInfo_toVPSS.pfGetImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
    
    return Ret;
}

HI_S32 VENC_DRV_EflToVPSSRelImge(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImageInfo)   //给VPSS的回调，主动释放帧
{
    HI_S32 Ret;
    HI_HANDLE EncHandle = HI_INVALID_HANDLE;
    
    VeduEfl_EncPara_S  *pEncPara;
    D_VENC_GET_CHN_FORM_VPSS(hVPSS, EncHandle);
    pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    Ret =(*pEncPara->stSrcInfo_toVPSS.pfPutImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
    
    return Ret;
}

HI_BOOL VENC_DRV_EflJudgeVPSS( HI_HANDLE EncHandle, HI_DRV_VIDEO_FRAME_S *pstFrameInfo ,HI_BOOL bActiveMode)   //调用在获得帧信息后
{
    HI_S32 Ret;
    HI_U32 u32ChnID;
    HI_BOOL flag = HI_FALSE;
    HI_DRV_VPSS_SOURCE_FUNC_S VpssSourceFun;
    VeduEfl_EncPara_S *pEncPara;
    
    D_VENC_GET_CHN(u32ChnID,EncHandle);
    if (MAX_VEDU_CHN == u32ChnID)
    {
        HI_ERR_VENC(" the input handle(%d) is not open or even not exist!!!\n",EncHandle);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    
    flag |= (pEncPara->PicHeight != D_VENC_ALIGN_UP(pstFrameInfo->u32Height , 16) );
    flag |= (pEncPara->PicWidth  != D_VENC_ALIGN_UP(pstFrameInfo->u32Width , 16)  );
    //flag |= (pstFrameInfo->ePixFormat !=HI_DRV_PIX_FMT_NV21 )&&(pstFrameInfo->ePixFormat != HI_DRV_PIX_FMT_NV61_2X1)   /*目前只支持的源格式*/
    if (HI_TRUE == flag)  /* need VPSS to Process the Frame from now on*/
    {
        g_stVencChn[u32ChnID].bNeedVPSS = HI_TRUE;
        
        if( HI_TRUE == bActiveMode )     /* VPSS 主动取帧模式 ,对应VENC原来绑定模式*/
        {
            (*pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, pstFrameInfo);   /*把用于判断的第一帧还了*/
            
            
            VpssSourceFun.VPSS_GET_SRCIMAGE = VENC_DRV_EflToVPSSGetImge;             
            VpssSourceFun.VPSS_REL_SRCIMAGE = VENC_DRV_EflToVPSSRelImge;
            
            pEncPara->stSrcInfo.handle       = (HI_HANDLE)(g_stVencChn[u32ChnID].hPort[0]);
            pEncPara->stSrcInfo.pfGetImage   = pVpssFunc->pfnVpssGetPortFrame;
            pEncPara->stSrcInfo.pfPutImage   = pVpssFunc->pfnVpssRelPortFrame;
            Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS,VPSS_SOURCE_MODE_VPSSACTIVE, &VpssSourceFun);

        }
        else                             /* VPSS 被动收帧模式 ，对应VENC原来Queue模式*/
        {
            //HI_DRV_VPSS_PutImage(g_stVencChn[u32ChnID].hVPSS , pstFrameInfo);        /*把用户主动给的帧给VPSS*/
                      
            pEncPara->stSrcInfo.handle       = (HI_HANDLE)g_stVencChn[u32ChnID].hPort[0];
            pEncPara->stSrcInfo.pfGetImage   = pVpssFunc->pfnVpssGetPortFrame;
            pEncPara->stSrcInfo.pfPutImage   = pVpssFunc->pfnVpssRelPortFrame;
            Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS,VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
        }
        (pVpssFunc->pfnVpssEnablePort)(g_stVencChn[u32ChnID].hPort[0], HI_TRUE);
    }
    else
    {
        g_stVencChn[u32ChnID].bNeedVPSS = HI_FALSE;
    }
    pEncPara->bNeverEnc = HI_FALSE;
    return g_stVencChn[u32ChnID].bNeedVPSS;
}
#endif

HI_VOID VENC_DRV_EflWakeUpThread( HI_VOID)
{
    VENC_DRV_OsalGiveEvent(&g_VENC_Event);
    return ;
}

HI_VOID VENC_DRV_EflSortPriority(HI_VOID)
{
   HI_U32 i,j;
   for( i = 0; i < MAX_VEDU_CHN - 1; i++)
   {
      for(j =  MAX_VEDU_CHN - 1; j > i; j--)
      {
          if(PriorityTab[1][j]>PriorityTab[1][j-1])
          {
             HI_U32 temp0 = PriorityTab[0][j];
             HI_U32 temp1 = PriorityTab[1][j];
             PriorityTab[0][j]   = PriorityTab[0][j-1];
             PriorityTab[1][j]   = PriorityTab[1][j-1];
             PriorityTab[0][j-1] = temp0;
             PriorityTab[1][j-1] = temp1;
          }
      }
   }
}

HI_S32 VENC_DRV_EflCfgRegVenc( HI_U32 EncHandle )
{
    VENC_HAL_CfgReg( EncHandle );
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflReadRegVenc( HI_U32 EncHandle )
{
    VENC_HAL_ReadReg( EncHandle );
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflStartOneFrameVenc( HI_U32 EncHandle, VeduEfl_EncIn_S *pEncIn )
{
    VeduEfl_EncPara_S  *pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;

    VENC_DRV_EflRcOpenOneFrm( EncHandle );

    /* Make slice header bitstream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SlcHdr_S SlcHdr;

        pEncPara->H264FrmNum = pEncPara->IntraPic ? 0 : pEncPara->H264FrmNum + 1;
        SlcHdr.frame_num  = pEncPara->H264FrmNum;
        SlcHdr.slice_type = pEncPara->IntraPic ? 2 : 0;
		SlcHdr.NumRefIndex= pEncPara->NumRefIndex;
        pEncPara->SlcHdrBits = H264e_MakeSlcHdr(pEncPara->SlcHdrStream,
                                                pEncPara->ReorderStream,
                                                pEncPara->MarkingStream, &SlcHdr);
    }
    else if (pEncPara->Protocol == VEDU_H263)
    {
        VeduEfl_H263e_PicHdr_S PicHdr;

        PicHdr.Pframe = !pEncPara->IntraPic;
        PicHdr.TR = pEncPara->LastTR & 0xFF; /*CurrTR after RC*/
        PicHdr.SrcFmt = pEncPara->H263SrcFmt;
        PicHdr.Wframe = pEncPara->PicWidth;
        PicHdr.Hframe = pEncPara->PicHeight;
        PicHdr.PicQP = pEncPara->StartQp;

        pEncPara->SlcHdrBits = H263e_MakePicHdr(pEncPara->SlcHdrStream, &PicHdr);
    }
    else if (pEncPara->Protocol == VEDU_MPEG4)
    {
        VeduEfl_MP4e_VopHdr_S VopHdr;

        VopHdr.Pframe = !pEncPara->IntraPic;
        VopHdr.PicQP = pEncPara->StartQp;
        VopHdr.Fcode = 2;

        pEncPara->SlcHdrBits = MP4e_MakeVopHdr(pEncPara->SlcHdrStream, &VopHdr);
    }

    pEncPara->SrcYAddr   = pEncIn->BusViY;
    pEncPara->SrcCAddr   = pEncIn->BusViC;
    pEncPara->SrcVAddr   = pEncIn->BusViV;
    pEncPara->SStrideY = pEncIn->ViYStride;
    pEncPara->SStrideC = pEncIn->ViCStride;

    if (pEncPara->LowDlyMod)
    {
        pEncPara->tunlcellAddr = pEncIn->TunlCellAddr;   

        if (pEncPara->PicWidth >= 1920)  //D1
        {
           pEncPara->tunlReadIntvl = 3;
        }
		else if (pEncPara->PicWidth >= 720)
	    {
	       pEncPara->tunlReadIntvl = 2;
	    }
		else 
	    {
	       pEncPara->tunlReadIntvl = 1;
	    }
    }

    pEncPara->PTS0 = pEncIn->PTS0;
    pEncPara->PTS1 = pEncIn->PTS1;

    pEncPara->RcnIdx = !pEncPara->RcnIdx;
    pEncPara->RStrideY = pEncIn->RStrideY;
    pEncPara->RStrideC = pEncIn->RStrideC;

    if (pEncPara->IntraPic)
    {
        VeduEfl_NaluHdr_S NaluHdr;

        NaluHdr.PacketLen  = 128;
        NaluHdr.InvldByte  = 64 - pEncPara->SpsBits / 8;
        NaluHdr.bLastSlice = 0;
        NaluHdr.Type = 7;

        NaluHdr.PTS0 = pEncPara->PTS0;
        NaluHdr.PTS1 = pEncPara->PTS1;

        if ((pEncPara->Protocol == VEDU_H264) || (pEncPara->Protocol == VEDU_MPEG4))
        {
            VENC_DRV_BufWrite( pstStrBuf, &NaluHdr, 64 );
            VENC_DRV_BufWrite( pstStrBuf, pEncPara->SpsStream, 64 );
        }

        NaluHdr.InvldByte = 64 - pEncPara->PpsBits / 8;
        NaluHdr.Type = 8;

        if (pEncPara->Protocol == VEDU_H264)
        {
            VENC_DRV_BufWrite( pstStrBuf, &NaluHdr, 64 );
            VENC_DRV_BufWrite( pstStrBuf, pEncPara->PpsStream, 64 );
        }

        VENC_DRV_BufUpdateWp( pstStrBuf );
    }

    *(HI_U32 *)(pEncPara->StrmBufRpAddr + pEncPara->Vir2BusOffset) = pstStrBuf->u32RdTail;
    *(HI_U32 *)(pEncPara->StrmBufWpAddr + pEncPara->Vir2BusOffset) = pstStrBuf->u32WrHead;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflEndOneFrameVenc( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
	if (!pEncPara->LowDlyMod)  // not use low delay mode
	{
	    if (!pEncPara->VencBufFull || !pEncPara->VencPbitOverflow)
	    {
	        /* Read Wp which be changed by HW */
	        wrptr = *(HI_U32 *)(pEncPara->StrmBufWpAddr + pEncPara->Vir2BusOffset);

	        VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
	        VENC_DRV_BufUpdateWp( pstStrBuf );
	    }
    }
	else
	{
	    wrptr = *(HI_U32 *)(pEncPara->StrmBufWpAddr + pEncPara->Vir2BusOffset);
	    VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
	    VENC_DRV_BufUpdateWp( pstStrBuf );
	}
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

    VENC_DRV_EflRcCloseOneFrm( EncHandle );

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflEndOneSliceVenc( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    //if (!pEncPara->VencBufFull || !pEncPara->VencPbitOverflow)
    //{
        /* Read Wp which be changed by HW */
        wrptr = *(HI_U32 *)(pEncPara->StrmBufWpAddr + pEncPara->Vir2BusOffset);

        VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
        VENC_DRV_BufUpdateWp( pstStrBuf );
    //}

    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

    //VENC_DRV_EflRcCloseOneFrm( EncHandle );

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_S32 VENC_DRV_EflChkChnCfg( VeduEfl_EncCfg_S *pEncCfg )
{
    HI_S32 CfgErr = 0;

    CfgErr |= (pEncCfg->FrameWidth < 160) | (pEncCfg->FrameWidth > 4096);
    CfgErr |= (pEncCfg->FrameHeight < 64) | (pEncCfg->FrameHeight > 4096);

    CfgErr |= (pEncCfg->Protocol < VEDU_H264) | (pEncCfg->Protocol > VEDU_MPEG4);
	CfgErr |= (pEncCfg->CapLevel > VEDU_CAP_LEVEL_1080P);
   // CfgErr |= (pEncCfg->YuvStoreType < VEDU_SEMIPLANNAR) | (pEncCfg->YuvStoreType > VEDU_PACKAGE);

    //CfgErr |= (pEncCfg->SlcSplitEn != 0) & (pEncCfg->SplitSize > 511) & (pEncCfg->Protocol != VEDU_H263);

    if (CfgErr)
    {
        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     : EncHandle
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflCreateVenc( HI_U32 *pEncHandle, VeduEfl_EncCfg_S *pEncCfg )
{
    HI_S32 s32Ret = HI_FAILURE;
    MMZ_BUFFER_S sMBufVenc   = {0};
    MMZ_BUFFER_S sMBufVenc_Q = {0};
    VeduEfl_EncPara_S  *pEncPara;
    HI_U32 LumaSize;
    HI_U32 WidthInMb  = (pEncCfg->FrameWidth + 15) >> 4;
    HI_U32 HeightInMb = (pEncCfg->FrameHeight + 15) >> 4;

    HI_U32 BusRcnBuf, RcnBufSize; /* 16 aligned, = 2.0 frame mb-yuv */
    HI_U32 BusBitBuf, BitBufSize;
    HI_VOID *pVirBit = HI_NULL;

    /* check channel config */
    if (HI_FAILURE == VENC_DRV_EflChkChnCfg( pEncCfg ))
    {
        return HI_FAILURE;
    }

    switch (pEncCfg->CapLevel)
    {
        case VEDU_CAP_LEVEL_QCIF:
			LumaSize = 176*144;
			break;
		case VEDU_CAP_LEVEL_CIF:
			LumaSize = 352*288;
			break;
		case VEDU_CAP_LEVEL_D1:
			LumaSize = 720*576;
			break;
		case VEDU_CAP_LEVEL_720P:
			LumaSize = 1280*720;
			break;
		case VEDU_CAP_LEVEL_1080P:
			LumaSize = 1920*1088;
			break;
		default:
            return HI_ERR_VENC_INVALID_PARA;
			break;	
    }
	RcnBufSize = LumaSize * 3;
	
    /* malloc encoder parameter & reconstruction frames & bitstream */

    //    pEncPara = (VeduEfl_EncPara_S *) VENC_DRV_OsalMemMalloc( sizeof(VeduEfl_EncPara_S));
    pEncPara = (VeduEfl_EncPara_S *)HI_KMALLOC(HI_ID_VENC, sizeof(VeduEfl_EncPara_S), GFP_KERNEL);
	if (HI_NULL == pEncPara)
	{
		HI_ERR_VENC("HI_KMALLOC failed, size = %d\n", sizeof(VeduEfl_EncPara_S));
		return HI_FAILURE;
	}
	
    memset(pEncPara, 0, sizeof(VeduEfl_EncPara_S));

    if (pEncCfg->SlcSplitEn)
    {
	   BitBufSize = pEncCfg->streamBufSize + D_VENC_ALIGN_UP((LumaSize*3/2/4), 64);
    }
    else
    {
        BitBufSize = pEncCfg->streamBufSize+ D_VENC_ALIGN_UP((LumaSize*3/2),64);
    }
    s32Ret = HI_DRV_MMZ_Alloc("VENC_SteamBuf", HI_NULL, BitBufSize + RcnBufSize, 64, &sMBufVenc);
    if (HI_SUCCESS == s32Ret)
    {
        BusBitBuf = sMBufVenc.u32StartPhyAddr;
    }
	else
	{
		HI_ERR_VENC("HI_DRV_MMZ_Alloc failed\n");
		return HI_FAILURE;
	}
	

    //    BusBitBuf = VENC_DRV_OsalBufMalloc(BitBufSize + RcnBufSize, 64 );
    BusRcnBuf = BusBitBuf + BitBufSize;

    if ((NULL == pEncPara) || (0 == BusBitBuf))
    {
        if (BusBitBuf)
        {
            HI_DRV_MMZ_Release(&sMBufVenc);

            //            VENC_DRV_OsalBufFree( BusBitBuf );
        }

        if (pEncPara)
        {
            HI_KFREE(HI_ID_VENC, pEncPara);

            //            VENC_DRV_OsalMemFree( pEncPara  );
        }

        return HI_FAILURE;
    }

    /* creat stream buffer lock */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &pEncPara->pStrmBufLock ))
    {
        HI_KFREE(HI_ID_VENC, pEncPara);

        //        VENC_DRV_OsalMemFree(pEncPara);
        return HI_FAILURE;
    }

    /* ArrangeChnBuf -> rcn & bits */
    pEncPara->RcnYAddr[0] = BusRcnBuf;
    pEncPara->RcnCAddr[0] = BusRcnBuf + LumaSize;
    pEncPara->RcnYAddr[1] = BusRcnBuf + LumaSize * 3 / 2;
    pEncPara->RcnCAddr[1] = BusRcnBuf + LumaSize * 5 / 2;

    pEncPara->StrmBufRpAddr = BusBitBuf;
    pEncPara->StrmBufWpAddr = BusBitBuf  + 16; /* 16-byte aligned */
    pEncPara->StrmBufAddr   = BusBitBuf  + 64;
    pEncPara->StrmBufSize   = pEncCfg->streamBufSize - 64;  
    s32Ret = HI_DRV_MMZ_Map(&sMBufVenc);
    if (HI_SUCCESS == s32Ret)
    {
        pVirBit = (HI_VOID *)(sMBufVenc.u32StartVirAddr);
    }

    //    pVirBit = VENC_DRV_OsalBufMap( BusBitBuf, BitBufSize );

    //    pEncPara->Vir2BusOffset = (HI_U32)pVirBit - BusBitBuf;
    pEncPara->Vir2BusOffset = sMBufVenc.u32StartVirAddr - BusBitBuf;

    /* init cycle buffer for stream */
    if (HI_FAILURE == VENC_DRV_BufInit(&pEncPara->stCycBuf, pVirBit+64 , pEncPara->StrmBufSize, 64))
    {
        HI_DRV_MMZ_Unmap(&sMBufVenc);
        HI_DRV_MMZ_Release(&sMBufVenc);
        HI_KFREE(HI_ID_VENC, pEncPara);

        /*        VENC_DRV_OsalBufUnmap( pVirBit   );
                VENC_DRV_OsalBufFree ( BusBitBuf );
                VENC_DRV_OsalMemFree ( pEncPara  );
         */
        return HI_FAILURE;
    }
/************************************* add ******************************************/ 
    s32Ret = HI_DRV_MMZ_Alloc("VENC_FrmQueue", HI_NULL, sizeof(HI_DRV_VIDEO_FRAME_S) * MAX_VEDU_QUEUE_NUM, 64, &sMBufVenc_Q);
    if (HI_FAILURE == s32Ret)
	{
		return HI_FAILURE;
	}
    if (sMBufVenc_Q.u32StartPhyAddr == 0)
    {
        HI_DRV_MMZ_Release(&sMBufVenc_Q);
        return HI_FAILURE;
    }
    s32Ret = HI_DRV_MMZ_Map(&sMBufVenc_Q);           //映射
    if (HI_FAILURE == s32Ret)
    {
        return HI_FAILURE;
    }
    /* init cycle buffer for stream */

    pEncPara->stCycQueBuf.u32BufLen        = sizeof(HI_DRV_VIDEO_FRAME_S) * MAX_VEDU_QUEUE_NUM;
    pEncPara->stCycQueBuf.u32Vir2PhyOffset = sMBufVenc_Q.u32StartVirAddr - sMBufVenc_Q.u32StartPhyAddr;
    pEncPara->stCycQueBuf.pBase            = (HI_VOID *)(sMBufVenc_Q.u32StartVirAddr); 
    pEncPara->stCycQueBuf.u32RdHead        = 0;  
    pEncPara->stCycQueBuf.u32RdTail        = 0;  
    pEncPara->stCycQueBuf.u32WrHead        = 0;  
    pEncPara->stCycQueBuf.u32WrTail        = 0;   
/**********************************************************************************/

    /* get channel para */
    pEncPara->Protocol  = pEncCfg->Protocol;
    pEncPara->PicWidth  = WidthInMb  << 4;
    pEncPara->PicHeight = HeightInMb << 4;

    pEncPara->RotationAngle = VEDU_ROTATION_0;
    pEncPara->SlcSplitEn = pEncCfg->SlcSplitEn;
    pEncPara->SplitSize  = pEncCfg->SplitSize;
    pEncPara->QuickEncode= pEncCfg->QuickEncode;
    
    pEncPara->Priority   = pEncCfg->Priority;
    pEncPara->Gop        = pEncCfg->Gop;
    pEncPara->WaitingIsr = 0;
    pEncPara->pRegBase = VeduIpCtx.pRegBase;

	//pEncPara->YuvStoreType  = pEncCfg->YuvStoreType;                  //
    //pEncPara->YuvSampleType = VEDU_YUV420;                            //
    //pEncPara->PackageSel = pEncCfg->PackageSel;
   
    /* init RC para */
    pEncPara->IntraPic = 1;

	/* other */
	pEncPara->bNeverEnc   = HI_TRUE;
	pEncPara->H264HpEn    = 0;                                          //can't support high profile,use main profile
	pEncPara->H264CabacEn = 1;                                          //打开cabac 编码
	pEncPara->SlcSplitMod = 1;                                          //just choose the mb line Mode 
    pEncPara->NumRefIndex = 0;

    Venc_SetRegDefault(pEncPara);

    /* make sps & pps & VOL stream */
    if (pEncCfg->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SPS_S sps;
        VeduEfl_H264e_PPS_S pps;

        sps.ProfileIDC      = pEncPara->H264HpEn ? 100 : 77;
        sps.FrameWidthInMb  = WidthInMb;
        sps.FrameHeightInMb = HeightInMb;
        sps.FrameCropLeft = 0;
        sps.FrameCropTop    = 0;
        sps.FrameCropRight  = (sps.FrameWidthInMb * 16 - pEncCfg->FrameWidth) >> 1;
        sps.FrameCropBottom = (sps.FrameHeightInMb * 16 - pEncCfg->FrameHeight) >> 1;

        pps.ChrQpOffset = pEncPara->ChrQpOffset;
        pps.ConstIntra  = pEncPara->ConstIntra;
        pps.H264HpEn    = pEncPara->H264HpEn;
        pps.H264CabacEn = pEncPara->H264CabacEn;
        pps.pScale8x8   = pEncPara->Scale8x8;

        pEncPara->SpsBits = H264e_MakeSPS(pEncPara->SpsStream, &sps);
        pEncPara->PpsBits = H264e_MakePPS(pEncPara->PpsStream, &pps);
    }
    else if (pEncCfg->Protocol == VEDU_H263)
    {
        int w = pEncCfg->FrameWidth, srcFmt;
        int h = pEncCfg->FrameHeight;

        if ((w == 128) && (h == 96))
        {
            srcFmt = 1;
        }
        else if ((w == 176) && (h == 144))
        {
            srcFmt = 2;
        }
        else if ((w == 352) && (h == 288))
        {
            srcFmt = 3;
        }
        else if ((w == 704) && (h == 576))
        {
            srcFmt = 4;
        }
        else if ((w == 1408) && (h == 1152))
        {
            srcFmt = 5;
        }
        else
        {
            srcFmt = 6;
        }

        pEncPara->H263SrcFmt = srcFmt;
    }
    else if (pEncCfg->Protocol == VEDU_MPEG4)
    {
        VeduEfl_MP4e_VOL_S vol;

        vol.Wframe = pEncCfg->FrameWidth;
        vol.Hframe = pEncCfg->FrameHeight;

        pEncPara->SpsBits = MP4e_MakeVOL(pEncPara->SpsStream, &vol);
    }

    /* init RC para */
    pEncPara->IntraPic = 1;
    //pEncPara->StartQp = pEncPara->Protocol == VEDU_H264 ? 24 : 5;

    /* init stat info */
    pEncPara->stStat.GetFrameNumTry  = 0;
    pEncPara->stStat.PutFrameNumTry  = 0;
    pEncPara->stStat.GetStreamNumTry = 0;
    pEncPara->stStat.PutStreamNumTry = 0;
    pEncPara->stStat.GetFrameNumOK  = 0;
    pEncPara->stStat.PutFrameNumOK  = 0;
    pEncPara->stStat.GetStreamNumOK = 0;
    pEncPara->stStat.PutStreamNumOK = 0;
    pEncPara->stStat.BufFullNum = 0;
    pEncPara->stStat.SkipFrmNum = 0;
    pEncPara->stStat.StreamTotalByte = 0;
    
    /* init src info */
#if 0
    pEncPara->stSrcInfo.pfGetImage = HI_NULL;
    pEncPara->stSrcInfo.pfPutImage = HI_NULL;
#else
    pEncPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
    pEncPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;
#endif
    pEncPara->stSrcInfo.handle = HI_INVALID_HANDLE;

    /* get return val */
    *pEncHandle = (HI_U32)pEncPara;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflDestroyVenc( HI_U32 EncHandle )
{
    MMZ_BUFFER_S sMBufVenc  = {0};
    MMZ_BUFFER_S sMBufVenc_Q  = {0};
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    if (pEncPara == NULL)
    {
        return HI_FAILURE;
    }

    VENC_DRV_OsalLockDestroy( pEncPara->pStrmBufLock );

    sMBufVenc.u32StartVirAddr = pEncPara->StrmBufRpAddr + pEncPara->Vir2BusOffset;
    sMBufVenc.u32StartPhyAddr = pEncPara->StrmBufRpAddr;
    sMBufVenc_Q.u32StartVirAddr = (HI_U32)pEncPara->stCycQueBuf.pBase ;
    sMBufVenc_Q.u32StartPhyAddr = (HI_U32)(pEncPara->stCycQueBuf.pBase - pEncPara->stCycQueBuf.u32Vir2PhyOffset);   
    HI_DRV_MMZ_Unmap(&sMBufVenc);
    HI_DRV_MMZ_Release(&sMBufVenc);
    HI_DRV_MMZ_Unmap(&sMBufVenc_Q);
    HI_DRV_MMZ_Release(&sMBufVenc_Q);
    HI_KFREE(HI_ID_VENC, pEncPara);

    /*    VENC_DRV_OsalBufUnmap((HI_VOID*)(pEncPara->StrmBufAddr + pEncPara->Vir2BusOffset));
        VENC_DRV_OsalBufFree ( pEncPara->StrmBufAddr );
        VENC_DRV_OsalMemFree ( pEncPara );
     */
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflAttachInput( HI_U32 EncHandle, VeduEfl_SrcInfo_S *pSrcInfo )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    if ((pSrcInfo->pfGetImage == NULL) || (pSrcInfo->pfPutImage == NULL))
    {
        return HI_FAILURE;
    }

    pEncPara->stSrcInfo = *pSrcInfo;
    pEncPara->stSrcInfo_toVPSS = *pSrcInfo;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflDetachInput( HI_U32 EncHandle, VeduEfl_SrcInfo_S *pSrcInfo )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    pEncPara->stSrcInfo = *pSrcInfo;
    pEncPara->stSrcInfo_toVPSS = *pSrcInfo;
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflRequestIframe( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    pEncPara->InterFrmCnt = pEncPara->Gop - 1;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcGetAttr( HI_U32 EncHandle, VeduEfl_RcAttr_S *pRcAttr )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    pRcAttr->BitRate = pEncPara->BitRate;
    pRcAttr->OutFrmRate = pEncPara->VoFrmRate;
    pRcAttr->InFrmRate = pEncPara->ViFrmRate;
    
    pRcAttr->MaxQp  = pEncPara->MaxQp;
    pRcAttr->MinQp  = pEncPara->MinQp;
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcSetAttr( HI_U32 EncHandle, VeduEfl_RcAttr_S *pRcAttr )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    HI_U32 i;

    if ((pRcAttr->InFrmRate > 60) || (pRcAttr->InFrmRate < pRcAttr->OutFrmRate))
    {
        return HI_FAILURE;
    }

    if (pRcAttr->OutFrmRate == 0)
    {
        return HI_FAILURE;
    }

    pEncPara->BitRate   = pRcAttr->BitRate;
    pEncPara->VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->MaxQp     = pRcAttr->MaxQp;
    pEncPara->MinQp     = pRcAttr->MinQp;

    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*initialize frame rate control parameter*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                       pEncPara->BitRate / pEncPara->ViFrmRate);

    pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;
    pEncPara->TrCount = pEncPara->ViFrmRate;

    for (i = 0; i < pEncPara->ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->VoFrmRate;

        if (pEncPara->TrCount > pEncPara->ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    /*initialize re-start parameter*/
    pEncPara->RcStart = 1;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcFrmRateCtrl( HI_U32 EncHandle, HI_U32 TR )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    HI_U32 i, DiffTR = (TR - pEncPara->LastTR);

    if (pEncPara->RcStart)
    {
        pEncPara->TrCount  = pEncPara->ViFrmRate;
        pEncPara->TrCount += pEncPara->VoFrmRate;
        pEncPara->LastTR   = TR;
        pEncPara->IntraPic = 1;
    }
    else
    {
        /* don't run too many loop */
        if (DiffTR < VEDU_TR_STEP)
        {
            return HI_FAILURE;
        }
        else if (DiffTR > 0x1f)
        {
            DiffTR = 0x1f;
        }

        /* LOST frames into fifo */
        for (i = 0; i < DiffTR - VEDU_TR_STEP; i += VEDU_TR_STEP)
        {
            pEncPara->TrCount += pEncPara->VoFrmRate;
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
        }

        /* this frame */
        pEncPara->TrCount += pEncPara->VoFrmRate;
        pEncPara->LastTR = TR;

        /* don't care too many Lost frames */
        if (pEncPara->TrCount > pEncPara->VoFrmRate + pEncPara->ViFrmRate)
        {
            pEncPara->TrCount = pEncPara->VoFrmRate + pEncPara->ViFrmRate;
        }

        /* skip this frame */
        if ((pEncPara->TrCount <= pEncPara->ViFrmRate)      /* time to skip */
            || (pEncPara->stBitsFifo.Sum > pEncPara->BitRate * 2) /* too many bits */
            || (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < 64 * 8))/* too few buffer */
        {
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
            return HI_FAILURE;
        }

        /* intra or inter based gop */
        if (pEncPara->InterFrmCnt >= pEncPara->Gop - 1)
        {
            pEncPara->IntraPic = 1;
        }
        else
        {
            pEncPara->IntraPic = 0;
        }
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcOpenOneFrm( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

#define VEDU_CLIP3(x, y, z) (((x) < (y)) ? (y) : (((x) > (z)) ? (z) : (x)))

    //pEncPara->MinQp = (pEncPara->Protocol == VEDU_H264 ? 16 :  2);                   //由用户外部赋值
    //pEncPara->MaxQp = (pEncPara->Protocol == VEDU_H264 ? 48 : 30);                   //由用户外部赋值

    if (pEncPara->RcStart)
    {
        pEncPara->StartQp = pEncPara->Protocol == VEDU_H264 ? 36 : 12;
    }
    else
    {
        /* TargetBits */
        if (pEncPara->stBitsFifo.Sum > pEncPara->BitRate)
        {
            pEncPara->TargetBits = pEncPara->MeanBit -
                                   (pEncPara->stBitsFifo.Sum - pEncPara->BitRate) / pEncPara->VoFrmRate;
        }
        else
        {
            pEncPara->TargetBits = pEncPara->MeanBit +
                                   (pEncPara->BitRate - pEncPara->stBitsFifo.Sum) / pEncPara->VoFrmRate;
        }

        if (pEncPara->IntraPic)
        {
            /* StartQp */
            if (pEncPara->Gop > 5)
            {
                pEncPara->StartQp -= 1;
            }
            else if (pEncPara->PicBits > pEncPara->TargetBits)
            {
                if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 8)
                {
                    pEncPara->StartQp++;
                }
            }
            else
            {
                if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 8)
                {
                    pEncPara->StartQp--;
                }
            }
        }
        else
        {
            /* StartQp */
            if (pEncPara->PicBits > pEncPara->TargetBits)
            {
                if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 8)
                {
                    pEncPara->StartQp++;
                }

                if (pEncPara->Protocol == VEDU_H264)
                {
                    if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 4)
                    {
                        pEncPara->StartQp++;
                    }
                }
            }
            else
            {
                if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 8)
                {
                    pEncPara->StartQp--;
                }

                if (pEncPara->Protocol == VEDU_H264)
                {
                    if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 4)
                    {
                        pEncPara->StartQp--;
                    }
                }
            }

            /* VBR */
            if (pEncPara->VBRflag)
            {
                pEncPara->MinQp += pEncPara->PicLevel * (pEncPara->Protocol == VEDU_H264 ? 2 : 1);
            }
        }
    }

    if (pEncPara->Protocol == VEDU_H264)
    {
        pEncPara->StartQp = VEDU_CLIP3(pEncPara->StartQp, pEncPara->MinQp, 40);
    }
    else
    {
        pEncPara->StartQp = VEDU_CLIP3(pEncPara->StartQp, pEncPara->MinQp, pEncPara->MaxQp);
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcCloseOneFrm( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    if (pEncPara->VencBufFull ||pEncPara->VencPbitOverflow) /* Buffer Full -> LOST */
    {
        pEncPara->H264FrmNum--;
        pEncPara->RcnIdx = !pEncPara->RcnIdx;

        if (!pEncPara->RcStart)
        {
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0 );
        }

        pEncPara->stStat.BufFullNum++;

        return HI_FAILURE;
    }
    else
    {
        pEncPara->RcStart  = 0;
        pEncPara->TrCount -= pEncPara->ViFrmRate;

        if (pEncPara->IntraPic)
        {
            pEncPara->InterFrmCnt = 0;
        }
        else
        {
            pEncPara->InterFrmCnt++;
        }

        VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, pEncPara->PicBits );
    }

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflSetResolution( HI_U32 EncHandle, HI_U32 FrameWidth, HI_U32 FrameHeight )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    VeduEfl_RcAttr_S RcAttr;

    HI_U32 WidthInMb  = (FrameWidth + 15) >> 4;
    HI_U32 HeightInMb = (FrameHeight + 15) >> 4;
    HI_U32 LumaSize = (WidthInMb * HeightInMb) << 8;

    /* check config */
    if (LumaSize > pEncPara->RcnCAddr[0] - pEncPara->RcnYAddr[0])
    {
        return HI_FAILURE;
    }

    VENC_DRV_EflRcGetAttr( EncHandle, &RcAttr );
    VENC_DRV_EflRcSetAttr( EncHandle, &RcAttr );

    pEncPara->H264HpEn    = 0;                                          //打开High profile
	pEncPara->H264CabacEn = 1;                                          //打开cabac 编码
	pEncPara->SlcSplitMod = 1;                                  
    pEncPara->NumRefIndex = 0;
    /* make sps & pps & VOL stream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SPS_S sps;
        VeduEfl_H264e_PPS_S pps;

		sps.ProfileIDC      = pEncPara->H264HpEn ? 100 : 77;
        sps.FrameWidthInMb  = WidthInMb;
        sps.FrameHeightInMb = HeightInMb;
        sps.FrameCropLeft   = 0;
        sps.FrameCropTop    = 0;
        sps.FrameCropRight  = (sps.FrameWidthInMb * 16 - FrameWidth) >> 1;
        sps.FrameCropBottom = (sps.FrameHeightInMb * 16 - FrameHeight) >> 1;

        pps.ChrQpOffset = pEncPara->ChrQpOffset;
        pps.ConstIntra  = pEncPara->ConstIntra;
        pps.H264HpEn    = pEncPara->H264HpEn;
        pps.H264CabacEn = pEncPara->H264CabacEn;
        pps.pScale8x8   = pEncPara->Scale8x8;

        pEncPara->SpsBits = H264e_MakeSPS(pEncPara->SpsStream, &sps);
        pEncPara->PpsBits = H264e_MakePPS(pEncPara->PpsStream, &pps);
    }
    else if (pEncPara->Protocol == VEDU_H263)
    {
        int w = FrameWidth, srcFmt;
        int h = FrameHeight;

        if ((w == 128) && (h == 96))
        {
            srcFmt = 1;
        }
        else if ((w == 176) && (h == 144))
        {
            srcFmt = 2;
        }
        else if ((w == 352) && (h == 288))
        {
            srcFmt = 3;
        }
        else if ((w == 704) && (h == 576))
        {
            srcFmt = 4;
        }
        else if ((w == 1408) && (h == 1152))
        {
            srcFmt = 5;
        }
        else
        {
            srcFmt = 6;
        }

        pEncPara->H263SrcFmt = srcFmt;
    }
    else if (pEncPara->Protocol == VEDU_MPEG4)
    {
        VeduEfl_MP4e_VOL_S vol;

        vol.Wframe = FrameWidth;
        vol.Hframe = FrameHeight;

        pEncPara->SpsBits = MP4e_MakeVOL(pEncPara->SpsStream, &vol);
    }

    pEncPara->PicWidth  = WidthInMb << 4;
    pEncPara->PicHeight = HeightInMb << 4;

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/

HI_S32 VENC_DRV_EflStartVenc( HI_U32 EncHandle )
{
    HI_U32 i,j;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S *pEncPara = NULL;
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == (HI_U32)NULL)
        {
            VeduChnCtx[i].EncHandle = EncHandle;
            pEncPara = (VeduEfl_EncPara_S*)EncHandle;
            break;
        }
    }

    for (j = 0;(i < MAX_VEDU_CHN)&&(j < MAX_VEDU_CHN); j++)
    {
        if( INVAILD_CHN_FLAG ==PriorityTab[0][j])
        {
           PriorityTab[0][j] = i;
           PriorityTab[1][j] = pEncPara->Priority;
           VENC_DRV_EflSortPriority();
           break;
        }
    }
    pEncPara->bNeverEnc = HI_TRUE;
    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (i == MAX_VEDU_CHN)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflStopVenc( HI_U32 EncHandle )
{
    HI_U32 i,j;
    VEDU_LOCK_FLAG flag;
	VeduEfl_EncPara_S *pEncPara = NULL;

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == EncHandle)
        {
            VeduChnCtx[i].EncHandle = (HI_U32)NULL;
            break;
        }
    }
    for (j = 0;(i < MAX_VEDU_CHN)&&(j < MAX_VEDU_CHN); j++)
    {
        if( i ==PriorityTab[0][j])
        {
           PriorityTab[0][j] = INVAILD_CHN_FLAG;
           PriorityTab[1][j] = 0;
           VENC_DRV_EflSortPriority();
           break;
        }
    }
    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (i == MAX_VEDU_CHN)
    {
        return HI_FAILURE;
    }

	/* rfresh the queue of the Img */
	pEncPara = (VeduEfl_EncPara_S*)EncHandle;
	pEncPara->stCycQueBuf.u32RdHead        = 0;  
    pEncPara->stCycQueBuf.u32RdTail        = 0;  
    pEncPara->stCycQueBuf.u32WrHead        = 0;  
    pEncPara->stCycQueBuf.u32WrTail        = 0;   
	
    /* wait finish last frame */
    while (((VeduEfl_EncPara_S *)EncHandle)->WaitingIsr)
    {
        msleep(1);
    }

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_S32 VENC_DRV_EflCheckImgCfg(const HI_DRV_VIDEO_FRAME_S *pstPreImage,HI_U32 yuvStoreType)
{
    HI_BOOL flag = 0;

	flag |= (pstPreImage->u32Width > 1920)||(pstPreImage->u32Width < 176);
	flag |= (pstPreImage->u32Height> 1088)||(pstPreImage->u32Height< 144);
    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Y) || (pstPreImage->stBufAddr[0].u32PhyAddr_Y % 16);
	flag |= (!pstPreImage->stBufAddr[0].u32Stride_Y ) || (pstPreImage->stBufAddr[0].u32Stride_Y  % 16);
	if (VENC_STORE_PLANNAR == yuvStoreType)
	{
	    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C %16);
	    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Cr) || (pstPreImage->stBufAddr[0].u32PhyAddr_Cr%16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  %16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_Cr)  || (pstPreImage->stBufAddr[0].u32Stride_Cr %16);

		flag |= (pstPreImage->stBufAddr[0].u32Stride_Cr != pstPreImage->stBufAddr[0].u32Stride_C);
    }
	else if (VENC_STORE_SEMIPLANNAR == yuvStoreType)
    {
        flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C %16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  %16);
    }

	if (HI_TRUE == flag)
    {
       return HI_FAILURE;  
    }
	else return HI_SUCCESS;
}

//HI_UNF_VIDEO_FRAME_INFO_S stPreImage;
//HI_DRV_VIDEO_FRAME_S stPreImage;
static HI_U32 VENC_DRV_EflQueryChn( VeduEfl_EncIn_S *pEncIn )
{
    HI_U32 u32StartQueryNo = s32LastQueryNo + 1;
    HI_S32 s32StartChnID   = 0;
    VeduEfl_EncPara_S *pEncPara;
	
    HI_DRV_VIDEO_PRIVATE_S *pImagePriv;

    
    /* start from last query channel */
    if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        u32StartQueryNo = 0;
    }

    for (; u32StartQueryNo < MAX_VEDU_CHN; u32StartQueryNo++)
    {
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID,u32StartQueryNo);
        if ( INVAILD_CHN_FLAG == s32StartChnID )
        {
            continue;
        }
        pEncPara = (VeduEfl_EncPara_S *) VeduChnCtx[s32StartChnID].EncHandle;
        if( HI_INVALID_HANDLE == pEncPara->stSrcInfo.handle )
        {
            pEncPara->stSrcInfo.handle = VeduChnCtx[s32StartChnID].EncHandle;
        }

        if(pEncPara->QuickEncode)
        {
            if ( HI_FAILURE == QuickEncode_Process(VeduChnCtx[s32StartChnID].EncHandle,HI_FALSE))
            { continue; } 
        }
        else
        {
            pEncPara->stStat.GetFrameNumTry++;
            if (HI_SUCCESS != (pEncPara->stSrcInfo.pfGetImage)(pEncPara->stSrcInfo.handle, &(pEncPara->stImage)))
            {
                continue;
            }
            /* don't re-get */
            if (pEncPara->PTS0 == pEncPara->stImage.u32Pts)
            {
                pEncPara->stStat.PutFrameNumTry++;              //add
                (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &(pEncPara->stImage));
                pEncPara->stStat.PutFrameNumOK++;               //add
                continue;
            }
            else
            {
                pEncPara->stStat.GetFrameNumOK++;
            }   
       }

#ifdef __VENC_3716CV200_CONFIG__              //for lowdelay 

       if (pEncPara->stImage.u32Priv[2])      //待明确!!
       {
            QuickEncode_Process(VeduChnCtx[s32StartChnID].EncHandle,HI_TRUE);    
			if (pEncPara->stImage.u32Priv[2]) 
		    {
		         pEncPara->LowDlyMod = HI_TRUE; 
				 pEncIn->TunlCellAddr = pEncPara->stImage.u32Priv[2];  
		    }
       }
#endif		

#ifdef VENC_TO_VPSS_SUPPORT
       if( HI_TRUE == pEncPara->bNeverEnc)      /* just do this when venc get FrameInfo at the first time*/
       {
           HI_BOOL bVencAttachMode = ((pEncPara->stSrcInfo.handle >> 16)== HI_ID_VENC )? HI_FALSE : HI_TRUE;
           if (HI_TRUE == bVencAttachMode)
           {
              VENC_DRV_EflJudgeVPSS(VeduChnCtx[s32StartChnID].EncHandle,&(pEncPara->stImage), bVencAttachMode);
           }
           pEncPara->bNeverEnc = HI_FALSE;
       } 
#endif
        /* video encoder does frame rate control by two value: input frame rate and target frame rate */
        /* input frame rate is calculated by timer mechanism accurately */
        /* target frame rate is input by user and can be changed dynamiclly */

        /* skip - frame rate ctrl */
        pImagePriv = (HI_DRV_VIDEO_PRIVATE_S *)pEncPara->stImage.u32Priv;
        if (HI_SUCCESS
            != VENC_DRV_EflRcFrmRateCtrl( VeduChnCtx[s32StartChnID].EncHandle, pImagePriv->u32FrmCnt/*pstPrivInfo->u32SeqFrameCnt*/))
        {
            pEncPara->stStat.SkipFrmNum++;
            pEncPara->stStat.PutFrameNumTry++;
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
            pEncPara->stStat.PutFrameNumOK++;
            pEncPara->PTS0 = pEncPara->stImage.u32Pts;
            continue;
        }

		
        pEncPara->YuvStoreType  = Convert_PIX_Format(pEncPara->stImage.ePixFormat,0);
        pEncPara->YuvSampleType = Convert_PIX_Format(pEncPara->stImage.ePixFormat,1);//SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
        pEncPara->PackageSel    = Convert_PIX_Format(pEncPara->stImage.ePixFormat,2);//SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
        pEncPara->StoreFmt      = pEncPara->YuvStoreType;

        /* check the stride ,addr info first*/
        if ( HI_SUCCESS != VENC_DRV_EflCheckImgCfg(&pEncPara->stImage, pEncPara->YuvStoreType) )
        {
            HI_ERR_VENC("stImg cfg erro!!\n");
            pEncPara->stStat.SkipFrmNum++;
            pEncPara->stStat.PutFrameNumTry++;
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
            pEncPara->stStat.PutFrameNumOK++;
            pEncPara->PTS0 = pEncPara->stImage.u32Pts;
            continue;            
        }
		
        pEncIn->BusViY = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
        pEncIn->BusViC = pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;
        if (pEncPara->StoreFmt == VENC_STORE_PLANNAR)
        {
           pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
        }

        if((VENC_STORE_SEMIPLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
        {
            pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C*2;
            pEncIn->RStrideY  = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->RStrideC  = pEncPara->stImage.stBufAddr[0].u32Stride_C;   //pEncPara->stImage.stBufAddr[0].u32Stride_C*2 ??
        }
        else if ((VENC_STORE_PACKAGE == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))
        {
            pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
            pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//D_VENC_ALIGN_UP(pEncPara->stImage.stBufAddr[0].u32Stride_Y/2,16);
            pEncIn->RStrideC  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//D_VENC_ALIGN_UP(pEncPara->stImage.stBufAddr[0].u32Stride_Y/2,16);
        }
		else if ((VENC_STORE_PLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_420 == pEncPara->YuvSampleType))  
	    {
            pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
            pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->RStrideC  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//pEncPara->stImage.stBufAddr[0].u32Stride_Y;
	    }
		else 
	    {
	        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
            pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//pEncPara->stImage.stBufAddr[0].u32Stride_Y;
            pEncIn->RStrideC  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//pEncPara->stImage.stBufAddr[0].u32Stride_C; 
	    }
		
        pEncIn->PTS0 = pEncPara->stImage.u32Pts;
        pEncIn->PTS1 = 0;
        //memcpy(&stPreImage, &pEncPara->stImage, sizeof(HI_DRV_VIDEO_FRAME_S));
        pEncPara->WaitingIsr = 1;
        break; /* find channel:s32StartChnID  to enc */
    }

    s32LastQueryNo = u32StartQueryNo;

    if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        s32LastQueryNo = -1;
        return (HI_U32)HI_NULL;
    }
    else
    {
        return VeduChnCtx[s32StartChnID].EncHandle;
    }
}

/*
HI_S32 g_start,g_end,gMax = 0,gBMAX =0,g_isrtime =0;
HI_U32 gu32VeChn;
HI_U8 SendBuf[100004];
HI_U32 gu32SeqNo = 0;
extern HI_U32 gVencVppayload;
HI_U32 gu32Pts;
HI_VOID VencIsr_tasklet(HI_VOID)
{
    HI_S32 i,s32Ret;
    HI_U32 u32Pts = 0;
    VeduEfl_NALU_S        stVeduPacket;
    HI_U32 offset = 0;
    HI_U8 *Stream;

    Stream = &SendBuf[12];
    for(i = 0; i < 3;i++)
    {
        if(!VeduEfl_GetStreamLen(VeduIpCtx.CurrHandle))
        {
            continue;
        }
        s32Ret = VeduEfl_GetBitStream(VeduIpCtx.CurrHandle, &stVeduPacket);
        if(s32Ret!=HI_SUCCESS)
        {
            return;
        }
        memcpy((HI_VOID *)&Stream[offset],stVeduPacket.pVirtAddr[0],stVeduPacket.SlcLen[0]);
        offset += stVeduPacket.SlcLen[0];
        HI_ASSERT(offset < 100000);
        if(stVeduPacket.SlcLen[1])
        {
            memcpy((HI_VOID *)&Stream[offset],stVeduPacket.pVirtAddr[0],stVeduPacket.SlcLen[0]);
            offset += stVeduPacket.SlcLen[0];
            HI_ASSERT(offset < 100000);
        }
        u32Pts = stVeduPacket.PTS0;
        VeduEfl_SkpBitStream(VeduIpCtx.CurrHandle, &stVeduPacket);
    }

    if(offset)
    {
        Stream[offset + 0] = 0;
        Stream[offset + 1] = 0;
        Stream[offset + 2] = 1;
        Stream[offset + 3] = 0;
        offset += 4;
        VENCSendMediaData(g_stVencChn[gu32VeChn].u32NetHandle,u32Pts,offset,Stream,gVencVppayload,&gu32SeqNo);
    }
}
DECLARE_TASKLET(Venc_tasklet, VencIsr_tasklet, 0);
extern OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];
 */
 
static HI_VOID Venc_ISR( HI_VOID )
{
    HI_U32 EncHandle;
    VeduEfl_EncIn_S EncIn;
    VEDU_LOCK_FLAG  flag;
    HI_U32 u32VeChn;
	VeduEfl_EncPara_S *pEncPara;
	S_VEDU_REGS_TYPE *pAllReg;
    D_VENC_GET_CHN(u32VeChn, VeduIpCtx.CurrHandle);

	pEncPara = (VeduEfl_EncPara_S *) VeduIpCtx.CurrHandle;
	pAllReg  = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;

#ifdef __VENC_3716CV200_CONFIG__
	if (pAllReg->VEDU_RAWINT.bits.VeduSliceInt)			   //低延时模式下slice级中断处理
    {
         //此处为slice级级别中断发生的地方，可以在此次添加打印信息，对应帧序号为 pEncPara->stImage.u32SeqFrameCnt, add by liminqi
		 //add by zz
		 //REC_POS(pEncPara->stImage.u32SeqFrameCnt);
		 VENC_DRV_EflEndOneSliceVenc( VeduIpCtx.CurrHandle );
		 pAllReg->VEDU_INTCLR.u32 = 0x400;           //清中断 	 
    } 
#endif


    if ( pAllReg->VEDU_RAWINT.bits.VencEndOfPic )        //帧级中断
    {
	    /* release image encoded */
	    {
	        //HI_U32 regread;
	        
	        pEncPara->stStat.PutFrameNumTry++;
	        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
	        pEncPara->stStat.PutFrameNumOK++;

	        pEncPara->WaitingIsr = 0;

	        //S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;
	        //regread = pAllReg->VEDU_TIMER;
	    }

	    gencodeframe = 1;
	    wake_up(&gEncodeFrame);

	    /* postprocess frame encoded */
	    VENC_DRV_EflReadRegVenc    ( VeduIpCtx.CurrHandle );
		pAllReg->VEDU_INTCLR.u32 = 0xFFFFFFFF;//0xFFFFFBFF;
	    VENC_DRV_EflEndOneFrameVenc( VeduIpCtx.CurrHandle );


	    /* next frame to encode */                                
	    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
	    EncHandle = VENC_DRV_EflQueryChn( &EncIn );
	    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

	    if( EncHandle != (HI_U32)NULL )
	    {
	        VeduIpCtx.IpFree = 0;
	        VeduIpCtx.CurrHandle = EncHandle;

	        VENC_DRV_EflStartOneFrameVenc( EncHandle, &EncIn );
	        VENC_DRV_EflCfgRegVenc       ( EncHandle );
	        //g_start = OSAL_GetTimeInMs();

	    }
	    else
	    {
	        VeduIpCtx.IpFree = 1;
	    }

        VENC_DRV_OsalGiveEvent(&g_VencWait_Stream[u32VeChn]);
    }

}

HI_S32 VENC_DRV_EflEncodeFrame(HI_VOID)
{
    HI_U32 EncHandle;
    VeduEfl_EncIn_S EncIn;
    VEDU_LOCK_FLAG flag;

    if (HI_NULL == VeduChnCtx[0].EncHandle)
    {
        return 0;
    }
    if (!VeduIpCtx.StopTask)
    {
        if (VeduIpCtx.IpFree)
        {
            /* if ipfree, don't irqlock */
            VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
            EncHandle = VENC_DRV_EflQueryChn( &EncIn );
            VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

            if (EncHandle != (HI_U32)NULL)
            {
                VeduIpCtx.IpFree = 0;
                VeduIpCtx.CurrHandle = EncHandle;

                VENC_DRV_EflStartOneFrameVenc( EncHandle, &EncIn );
                VENC_DRV_EflCfgRegVenc       ( EncHandle );
            }
        }
    }

    return 0;
}

static HI_VOID VENC_DRV_EflTask( HI_VOID )
{
    HI_U32 EncHandle;
	HI_S32 s32Ret = 0;
    VeduEfl_EncIn_S EncIn;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S *pEncPara = HI_NULL;
    HI_U32 i = 0;
    HI_BOOL bTmpValue = HI_FALSE;
    HI_BOOL bVoAttach = HI_FALSE;
    HI_BOOL bQueueMode = HI_TRUE;
    VeduIpCtx.TaskRunning = 1;

    /* 初始化等待队列头*/
	VENC_DRV_OsalInitEvent(&g_VENC_Event, 0);

    /* wait for venc start */
    while (!VeduIpCtx.StopTask)
    {
        for (i = 0; i < MAX_VEDU_CHN; i++)
        {
            bTmpValue |= g_stVencChn[i].bEnable;
        }

        if (HI_FALSE == bTmpValue)
        {
            msleep(10);
        }
        else
        {
            break;
        }
    }

    /* find valid venc handle */
    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        pEncPara = (VeduEfl_EncPara_S *)g_stVencChn[i].hVEncHandle;
        if ((HI_NULL == pEncPara) || (HI_INVALID_HANDLE == (HI_U32)pEncPara))
        {
            continue;
        }
        else
        {
            switch (g_stVencChn[i].enSrcModId)
            {
            case HI_ID_VI:
                bQueueMode = HI_FALSE;
                break;
            case HI_ID_VO:
                bVoAttach = HI_TRUE;
                bQueueMode = HI_FALSE;
                break;
            default:
                break;
            }
        }
    }

    if (HI_TRUE == bVoAttach || HI_TRUE == bQueueMode)
    {
        while (!VeduIpCtx.StopTask)
        {
            if (VeduIpCtx.IpFree)
            {
                /* if ipfree, don't irqlock */
                VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
                EncHandle = VENC_DRV_EflQueryChn( &EncIn );
                VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

                if (EncHandle != (HI_U32)NULL)
                {
                    VeduIpCtx.IpFree = 0;
                    VeduIpCtx.CurrHandle = EncHandle;

                    VENC_DRV_EflStartOneFrameVenc( EncHandle, &EncIn );
                    VENC_DRV_EflCfgRegVenc( EncHandle );
                }
                else
                {
                    /* if channel not ready, sleep */
#if 0
                    msleep(5);
#else
                    /*g_waitsignal = 0;
                    ret = wait_event_interruptible_timeout(stWaitTimeQue,
                                                           g_waitsignal,
                                                           msecs_to_jiffies(100));
                    if (0 == ret)
                    {
                        HI_WARN_VENC("wait time out!\n");
                    }*/

                    s32Ret = VENC_DRV_OsalWaitEvent(&g_VENC_Event, msecs_to_jiffies(30));
                    if (0 != s32Ret)
                    {
                        HI_WARN_VENC("wait time out!\n");
                    }
#endif
                }
            }
            else
            {
                /* if ipfree, sleep */
#if 0
                msleep(5);
#else
                /*ret = wait_event_interruptible_timeout(stWaitTimeQue,
                                                         g_waitsignal,
                                                         msecs_to_jiffies(100));
                if (0 == ret)
                {
                    HI_WARN_VENC("wait time out!\n");
                }*/
                s32Ret = VENC_DRV_OsalWaitEvent(&g_VENC_Event, msecs_to_jiffies(50));
                if (0 != s32Ret)
                {
                    HI_WARN_VENC("wait time out!\n");
                }
#endif 
            }
        }
    }
    else
    {
        VeduIpCtx.TaskRunning = 0;
        return;
    }

    VeduIpCtx.TaskRunning = 0;
    return;
}

/******************************************************************************
Function   :
Description: IP-VEDU & IP-JPGE Open & Close
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflOpenVedu( HI_VOID )
{
    HI_U32 i;

    /* creat channel control mutex */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &VeduIpCtx.pChnLock ))
    {
        return HI_FAILURE;
    }

    /* map reg_base_addr to virtual address */

    VeduIpCtx.pRegBase = (HI_U32 *)ioremap( VEDU_REG_BASE_ADDR, 0x8000 );

    //    VeduIpCtx.pRegBase = (HI_U32 *)VENC_DRV_OsalMapRegisterAddr(VEDU_REG_BASE_ADDR, 0x8000);
    if (HI_NULL == VeduIpCtx.pRegBase)
    {
        VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );
        return HI_FAILURE;
    }

    /* set ip free status */
    VeduIpCtx.IpFree = 1;

    /* clear channel status */
    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        VeduChnCtx[i].EncHandle = (HI_U32)NULL;
    }

    /* init IRQ */
    VENC_HAL_DisableAllInt((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_HAL_ClrAllInt    ((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));

    if (HI_FAILURE == VENC_DRV_OsalIrqInit(VEDU_IRQ_ID, Venc_ISR))
    {
        return HI_FAILURE;
    }

    /* creat thread to manage channel */
    VeduIpCtx.StopTask = 0;
    VeduIpCtx.TaskRunning = 0;
    init_waitqueue_head(&gqueue);
    gwait = 0;

    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask, "Vedu", VENC_DRV_EflTask );
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflCloseVedu( HI_VOID )
{
    VeduIpCtx.StopTask = 1;
    while (VeduIpCtx.TaskRunning)
    {
        msleep(1);
    }

    VENC_DRV_OsalDeleteTask(VeduIpCtx.pTask);
    VENC_HAL_DisableAllInt((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_HAL_ClrAllInt    ((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_DRV_OsalIrqFree( VEDU_IRQ_ID );

    //    VENC_DRV_OsalUnmapRegisterAddr( VeduIpCtx.pRegBase );
    iounmap(VeduIpCtx.pRegBase);

    VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : VENC_DRV_Resume
 Description  : VENC resume function
 Input        : None
 Output       : None
 Return Value : None
 Others       : Delete initialization of global value compared with VeduEfl_OpenVedu
*****************************************************************************/
HI_S32 VENC_DRV_EflResumeVedu(HI_VOID)
{
    /* init IRQ */
    if (HI_FAILURE == VENC_DRV_OsalIrqInit(VEDU_IRQ_ID, Venc_ISR))
    {
        return HI_FAILURE;
    }

    /* creat channel control mutex */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &VeduIpCtx.pChnLock ))
    {
        return HI_FAILURE;
    }

    /* map reg_base_addr to virtual address */

    //    VeduIpCtx.pRegBase = (HI_U32 *)VENC_DRV_OsalMapRegisterAddr(VEDU_REG_BASE_ADDR, 0x8000);
    VeduIpCtx.pRegBase = ioremap( VEDU_REG_BASE_ADDR, 0x8000 );

    /* creat thread to manage channel */
    VeduIpCtx.StopTask = 0;
    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask, "venc", VENC_DRV_EflTask );

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description: check if bs len > 0, before call VeduEfl_GetBitStream
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_U32 VENC_DRV_EflGetStreamLen( HI_U32 EncHandle )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    HI_U32 u32BsLen;

    u32BsLen = VENC_DRV_BufGetDataLen( &(pEncPara->stCycBuf));

    return u32BsLen;
}

/******************************************************************************
Function   :
Description: Get One Nalu address & length(include 64-byte packet header)
Calls      :
Input      :
Output     :
Return     :
Others     : Read Head pointer be changed.
******************************************************************************/
HI_S32 VENC_DRV_EflGetBitStream_X( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu )
{
    VeduEfl_NaluHdr_S  *pNaluHdr;
    VeduEfl_EncPara_S  *pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;
    VALG_CB_RDINFO_S stRdInfo;

    //pEncPara->stStat.GetStreamNumTry++;    //change by l00228308

    if (VENC_DRV_BufIsVaild(pstStrBuf) != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    if (0 == VENC_DRV_BufGetDataLen(pstStrBuf))    /* no bs rdy   */
    {
        return HI_FAILURE;
    }

    pNaluHdr = (VeduEfl_NaluHdr_S*) VENC_DRV_BufGetRdHead( pstStrBuf );  /* parse the head   */

    /******* get addr and len, updata readhead *******/
    if (HI_SUCCESS != VENC_DRV_BufRead( pstStrBuf, pNaluHdr->PacketLen, &stRdInfo ))
    {
        return HI_FAILURE;
    }

    pNalu->pVirtAddr[0] = stRdInfo.pSrc  [0];
    pNalu->pVirtAddr[1] = stRdInfo.pSrc  [1];
    pNalu->SlcLen   [0] = stRdInfo.u32Len[0];
    pNalu->SlcLen   [1] = stRdInfo.u32Len[1];
    pNalu->InvldByte    = pNaluHdr->InvldByte;
    if (pNalu->SlcLen[1] > 0)
    {
        pNalu->SlcLen[1] -= pNaluHdr->InvldByte;
    }
    else
    {
        pNalu->SlcLen[0] -= pNaluHdr->InvldByte;
    }

    pNalu->PTS0 = pNaluHdr->PTS0;
    pNalu->PTS1 = pNaluHdr->PTS1;
    pNalu->bFrameEnd = pNaluHdr->bLastSlice;
    pNalu->NaluType = pNaluHdr->Type;

    /* add by j35383, discard nal header of 64 byte */
    pNalu->SlcLen   [0] -= 64;
    pNalu->pVirtAddr[0] = (HI_VOID *)((HI_U32)pNalu->pVirtAddr[0] + 64);

    pNalu->PhyAddr[0] = (HI_U32)pNalu->pVirtAddr[0] - pEncPara->Vir2BusOffset;
    pNalu->PhyAddr[1] = (HI_U32)pNalu->pVirtAddr[1] - pEncPara->Vir2BusOffset;

    //pEncPara->stStat.GetStreamNumOK++;
    pEncPara->stStat.StreamTotalByte += pNalu->SlcLen   [0];
    pEncPara->stStat.StreamTotalByte += pNalu->SlcLen   [1];

    if (pNalu->SlcLen[1] > 0)
    {
        memcpy((pNalu->pVirtAddr[0]+pNalu->SlcLen[0]),pNalu->pVirtAddr[1],pNalu->SlcLen[1]);
    }
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflGetBitStream( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu )
{
    VEDU_LOCK_FLAG flag;
    HI_S32 ret;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflGetBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

    return ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : Read Tail pointer be changed
******************************************************************************/
HI_S32 VENC_DRV_EflSkpBitStream_X( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu )
{
    VeduEfl_EncPara_S  *pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;

    HI_U32 u32InputLen;

    pEncPara->stStat.PutStreamNumTry++;

    if (VENC_DRV_BufIsVaild(pstStrBuf) != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    u32InputLen = pNalu->SlcLen[0] + pNalu->SlcLen[1] + 64; /* add by j35383 (+64) */
    //u32InputLen = u32InputLen & 63 ? (u32InputLen | 63) + 1 : u32InputLen;
    u32InputLen += pNalu->InvldByte;
    /******* check start addr *******/  /* add by j35383 (-64) */
    if ((HI_VOID *)((HI_U32)pNalu->pVirtAddr[0] - 64) != VENC_DRV_BufGetRdTail(pstStrBuf))
    {
        return HI_FAILURE;
    }

    /******* update Read index *******/
    if (HI_FAILURE == VENC_DRV_BufUpdateRp(pstStrBuf, u32InputLen))
    {
        return HI_FAILURE;
    }

    pEncPara->stStat.PutStreamNumOK++;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflSkpBitStream( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu )
{
    VEDU_LOCK_FLAG flag;
    HI_S32 ret;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflSkpBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

    return ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflQueryStatInfo( HI_U32 EncHandle, VeduEfl_StatInfo_S *pStatInfo )
{
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    if ((pEncPara == NULL) || (pStatInfo == NULL))
    {
        return HI_FAILURE;
    }

    *pStatInfo = pEncPara->stStat;

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32  VENC_DRV_EflQueueFrame( HI_U32 EncHandle, HI_DRV_VIDEO_FRAME_S  *pstFrame)
{
    VeduEfl_EncPara_S  *pEncPara ;
    VALG_CRCL_QUE_BUF_S *pstCycQueBuf;
    VEDU_LOCK_FLAG flag; 
    HI_BOOL bEmptyFlag;
    HI_U32 i;
    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((g_stVencChn[i].hVEncHandle == EncHandle) && (g_stVencChn[i].hVEncHandle != HI_INVALID_HANDLE))
        {
            pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
            break;
        }
    }
    if( i == MAX_VEDU_CHN )
    {
        HI_ERR_VENC(" bad handle!!\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    
    pstCycQueBuf =&(pEncPara->stCycQueBuf);

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
    D_VENC_CHECK_ALL_EQUAL(pstCycQueBuf->u32WrHead,pstCycQueBuf->u32WrTail,
                                                   pstCycQueBuf->u32RdHead,pstCycQueBuf->u32RdTail,bEmptyFlag);

    if(HI_TRUE == bEmptyFlag)
    {   
        memcpy(pstCycQueBuf->u32WrHead+pstCycQueBuf->pBase,pstFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
        pstCycQueBuf->u32WrHead += sizeof(HI_DRV_VIDEO_FRAME_S);
        
    }
    else if(pstCycQueBuf->u32WrHead != pstCycQueBuf->u32WrTail)
    {
        memcpy(pstCycQueBuf->u32WrHead+pstCycQueBuf->pBase,pstFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
        pstCycQueBuf->u32WrHead += sizeof(HI_DRV_VIDEO_FRAME_S);
    }
    else
    {
       VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);
       return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if(pstCycQueBuf->u32BufLen == pstCycQueBuf->u32WrHead)
    {
        pstCycQueBuf->u32WrHead = 0;
    }
    
    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32  VENC_DRV_EflDequeueFrame( HI_U32 EncHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VeduEfl_EncPara_S  *pEncPara ;
    VALG_CRCL_QUE_BUF_S *pstCycQueBuf;
    VEDU_LOCK_FLAG flag; 
    HI_U32 i;
    
    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((g_stVencChn[i].hVEncHandle == EncHandle) && (g_stVencChn[i].hVEncHandle != HI_INVALID_HANDLE))
        {
            pEncPara  = (VeduEfl_EncPara_S *)EncHandle;
            break;
        }
    }
    if( i == MAX_VEDU_CHN )
    {
        HI_ERR_VENC(" bad handle!!\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    pstCycQueBuf =&(pEncPara->stCycQueBuf);
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);

    if( pstCycQueBuf->u32WrTail == pstCycQueBuf->u32RdTail)
    {
       VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);
       return HI_ERR_VENC_CHN_INVALID_STAT;
    }
    else
    {
       
       memcpy(pstFrame,pstCycQueBuf->u32WrTail+pstCycQueBuf->pBase,sizeof(HI_DRV_VIDEO_FRAME_S));
       pstCycQueBuf->u32WrTail += sizeof(HI_DRV_VIDEO_FRAME_S);
     
    }
    
    if( pstCycQueBuf->u32WrTail == pstCycQueBuf->u32BufLen )
    {
           pstCycQueBuf->u32WrTail = 0;
    }
    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflGetImage(HI_S32 EncHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S *pEncPara;
    VALG_CRCL_QUE_BUF_S *pstCycQueBuf;
    D_VENC_GET_CHN(u32ChnID,EncHandle);
    if (MAX_VEDU_CHN == u32ChnID)
    {
        HI_ERR_VENC(" the input handle(%d) is not open or even not exist!!!\n",EncHandle);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    pEncPara      = (VeduEfl_EncPara_S *)EncHandle;
    pstCycQueBuf = &pEncPara->stCycQueBuf;
    if( pstCycQueBuf->u32RdHead== pstCycQueBuf->u32WrHead)
    { 
       return HI_FAILURE;
    }
    else
    {
       memcpy(pstFrame,pstCycQueBuf->u32RdHead+pstCycQueBuf->pBase,sizeof(HI_DRV_VIDEO_FRAME_S));
       pstCycQueBuf->u32RdHead += sizeof(HI_DRV_VIDEO_FRAME_S);
    }
    
    if( pstCycQueBuf->u32RdHead == pstCycQueBuf->u32BufLen )
    {
           pstCycQueBuf->u32RdHead = 0;
    }
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflPutImage(HI_S32 EncHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S *pEncPara;
    VALG_CRCL_QUE_BUF_S *pstCycQueBuf ;
    D_VENC_GET_CHN(u32ChnID,EncHandle);
    if (MAX_VEDU_CHN == u32ChnID)
    {
        HI_ERR_VENC(" the input handle(%d) is not open or even not exist!!!\n",EncHandle);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    pEncPara      = (VeduEfl_EncPara_S *)EncHandle;
    pstCycQueBuf =&pEncPara->stCycQueBuf;
    if( pstCycQueBuf->u32RdTail == pstCycQueBuf->u32RdHead)
    {
       return HI_FAILURE;
    }
    else
    {
       pstCycQueBuf->u32RdTail += sizeof(HI_DRV_VIDEO_FRAME_S);
    }
    
    if( pstCycQueBuf->u32RdTail == pstCycQueBuf->u32BufLen )
    {
           pstCycQueBuf->u32RdTail = 0;
    }
    return HI_SUCCESS;
}

/*************************************************************************************/
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
