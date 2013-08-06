/*****************************************************************************
  Copyright (C), 2004-2050, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
  File Name     : drv_venc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/07
  Last Modified :
  Description   :
  Function List :
  History       :
  1.Date        :
  Author        : j00131665
  Modification  : Created file
 ******************************************************************************/

#include "hal_venc.h"
#include "drv_vi_ext.h"
#include "drv_win_ext.h"
#include "drv_module_ext.h"
#include "drv_file_ext.h"
#include "drv_ndpt_ext.h"
#include "drv_reg_ext.h"
#include "drv_venc_efl.h"
#include "drv_venc_osal.h"
#include "drv_venc_buf_mng.h"
#include "drv_venc.h"
#include "hi_mpi_venc.h"
#include "drv_venc_ext.h"
#include "hi_drv_video.h"
#include "drv_vpss_ext.h"
#include "hi_kernel_adapt.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

//Check null pointer
//#define VDEC_APPENDIX_LEN 16
#define VENC_MAX_FRAME_LEN 200000
OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];
VEDU_OSAL_EVENT g_VencWait_Stream[VENC_MAX_CHN_NUM];

HI_DECLARE_MUTEX(g_VencStreamMutex);
volatile HI_U32 g_VENCCrgCtrl = 0;
//HI_S32 VencThreadGetIFrame(HI_VOID * arg);

extern VPSS_EXPORT_FUNC_S *pVpssFunc;
#define D_VENC_GET_CHN(s32VeChn, hVencChn) \
    do {\
        s32VeChn = 0; \
        while (s32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[s32VeChn].hVEncHandle == hVencChn)\
            { \
                break; \
            } \
            s32VeChn++; \
        } \
    } while (0)
    
#define D_VENC_GET_PRIORITY_ID(s32VeChn, sPriorityID) \
    do {\
        sPriorityID = 0; \
        while (sPriorityID < VENC_MAX_CHN_NUM)\
        {   \
            if (PriorityTab[0][sPriorityID] == s32VeChn)\
            { \
                break; \
            } \
            sPriorityID++; \
        } \
    } while (0)

//extern HI_U32 gVpNethandle;
wait_queue_head_t gEncodeFrame;
volatile HI_U32 gencodeframe = 0;
extern HI_S32 gMax, gBMAX;
//HI_U8 gSendbuf[VENC_MAX_FRAME_LEN + VDEC_APPENDIX_LEN + 4];
HI_U8 ZeroDelaySuffix[] = {0x00, 0x00, 0x01, 0x1e, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01,
                           0x1e};
HI_U32 gVencVppayload = 0;
//extern wait_queue_head_t stSendTimeQue;
//volatile HI_U32 gSendStart = 0;

//extern HI_U32 g_VpStaticCtrl;
//extern HI_S32 g_VpStatic[];
//extern HI_S32 g_Pts;

HI_S8 PriorityTab[2][VENC_MAX_CHN_NUM]={{-1,-1,-1,-1,-1,-1,-1,-1},{}};                
///////////////////////////////////////////////////////////////
static VENC_EXPORT_FUNC_S s_VencExportFuncs =
{
    .pfnVencEncodeFrame = VENC_DRV_EflEncodeFrame,
	.pfnVencWakeUpThread= VENC_DRV_EflWakeUpThread
};

static HI_S32 Convert_FrameStructure(HI_UNF_VIDEO_FRAME_INFO_S *pstUnfImage,HI_DRV_VIDEO_FRAME_S *pstDrvImage)
{
   pstDrvImage->u32FrameIndex  = pstUnfImage->u32FrameIndex;
   pstDrvImage->stBufAddr[0].u32PhyAddr_Y = pstUnfImage->stVideoFrameAddr[0].u32YAddr;
   pstDrvImage->stBufAddr[0].u32Stride_Y  = pstUnfImage->stVideoFrameAddr[0].u32YStride;
   pstDrvImage->stBufAddr[0].u32PhyAddr_C = pstUnfImage->stVideoFrameAddr[0].u32CAddr;
   pstDrvImage->stBufAddr[0].u32Stride_C  = pstUnfImage->stVideoFrameAddr[0].u32CStride;
   pstDrvImage->stBufAddr[0].u32PhyAddr_Cr= pstUnfImage->stVideoFrameAddr[0].u32CrAddr;
   pstDrvImage->stBufAddr[0].u32Stride_Cr = pstUnfImage->stVideoFrameAddr[0].u32CrStride;

   pstDrvImage->stBufAddr[1].u32PhyAddr_Y = pstUnfImage->stVideoFrameAddr[1].u32YAddr;
   pstDrvImage->stBufAddr[1].u32Stride_Y  = pstUnfImage->stVideoFrameAddr[1].u32YStride;
   pstDrvImage->stBufAddr[1].u32PhyAddr_C = pstUnfImage->stVideoFrameAddr[1].u32CAddr;
   pstDrvImage->stBufAddr[1].u32Stride_C  = pstUnfImage->stVideoFrameAddr[1].u32CStride;
   pstDrvImage->stBufAddr[1].u32PhyAddr_Cr= pstUnfImage->stVideoFrameAddr[1].u32CrAddr;
   pstDrvImage->stBufAddr[1].u32Stride_Cr = pstUnfImage->stVideoFrameAddr[1].u32CrStride;
   pstDrvImage->u32Width = pstUnfImage->u32Width;
   pstDrvImage->u32Height= pstUnfImage->u32Height;   
   pstDrvImage->u32SrcPts= pstUnfImage->u32SrcPts;
   pstDrvImage->u32Pts   = pstUnfImage->u32Pts;
   pstDrvImage->u32AspectWidth       = pstUnfImage->u32AspectWidth;
   pstDrvImage->u32AspectHeight      = pstUnfImage->u32AspectHeight;
   pstDrvImage->u32FrameRate = pstUnfImage->stFrameRate.u32fpsInteger;

   switch(pstUnfImage->enVideoFormat)
   {
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_422:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
        break;
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_420:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV21;
        break;
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_400:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV80;
        break;
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_411:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV12_411;
        break;
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV61;
        break;
      case HI_UNF_FORMAT_YUV_SEMIPLANAR_444:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_NV42;
        break;
      case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_UYVY;
        break;
      case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUYV;
        break;
      case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YVYU;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_400:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV400;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_411:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV411;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_420:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV420p;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_422_1X2:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV422_1X2;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_422_2X1:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV422_2X1;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_444:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV_444;
        break;
      case HI_UNF_FORMAT_YUV_PLANAR_410:
        pstDrvImage->ePixFormat = HI_DRV_PIX_FMT_YUV410p;
        break;
      default:
        pstDrvImage->ePixFormat = HI_DRV_PIX_BUTT;
        break;     
   }
   pstDrvImage->bProgressive    = pstUnfImage->bProgressive;
   
   switch(pstUnfImage->enFieldMode)
   {
      case HI_UNF_VIDEO_FIELD_ALL:
        pstDrvImage->enFieldMode= HI_DRV_FIELD_ALL;
        break;
      case HI_UNF_VIDEO_FIELD_TOP:
        pstDrvImage->enFieldMode = HI_DRV_FIELD_TOP;
        break;
      case HI_UNF_VIDEO_FIELD_BOTTOM:
        pstDrvImage->enFieldMode = HI_DRV_FIELD_BOTTOM;
        break;
      default:
        pstDrvImage->enFieldMode = HI_DRV_FIELD_BUTT;
        break;
   }
   pstDrvImage->bTopFieldFirst = pstUnfImage->bTopFieldFirst;
   pstDrvImage->stDispRect.s32Height = pstUnfImage->u32DisplayHeight;
   pstDrvImage->stDispRect.s32Width  = pstUnfImage->u32DisplayWidth;
   pstDrvImage->stDispRect.s32X      = pstUnfImage->u32DisplayCenterX;
   pstDrvImage->stDispRect.s32Y      = pstUnfImage->u32DisplayCenterY;
   pstDrvImage->eFrmType = (HI_DRV_FRAME_TYPE_E)pstUnfImage->enFramePackingType;
   pstDrvImage->u32Circumrotate = pstUnfImage->u32Circumrotate;
   pstDrvImage->bToFlip_H = pstUnfImage->bHorizontalMirror;
   pstDrvImage->bToFlip_V = pstUnfImage->bVerticalMirror;
   pstDrvImage->u32ErrorLevel=pstUnfImage->u32ErrorLevel;

   memcpy(pstDrvImage->u32Priv , pstUnfImage->u32Private,sizeof(HI_U32)*64);
   return HI_SUCCESS;
}

static HI_S32 Convert_DrvFrameStructure(HI_DRV_VIDEO_FRAME_S *pstDrvImage, HI_UNF_VIDEO_FRAME_INFO_S *pstUnfImage)
{
   pstUnfImage->u32FrameIndex  = pstDrvImage->u32FrameIndex; 
   pstUnfImage->stVideoFrameAddr[0].u32YAddr = pstDrvImage->stBufAddr[0].u32PhyAddr_Y ; 
   pstUnfImage->stVideoFrameAddr[0].u32YStride =pstDrvImage->stBufAddr[0].u32Stride_Y ;
   pstUnfImage->stVideoFrameAddr[0].u32CAddr=pstDrvImage->stBufAddr[0].u32PhyAddr_C ;
   pstUnfImage->stVideoFrameAddr[0].u32CStride=pstDrvImage->stBufAddr[0].u32Stride_C ;
   pstUnfImage->stVideoFrameAddr[0].u32CrAddr = pstDrvImage->stBufAddr[0].u32PhyAddr_Cr;
   pstUnfImage->stVideoFrameAddr[0].u32CrStride = pstDrvImage->stBufAddr[0].u32Stride_Cr ;

   pstUnfImage->stVideoFrameAddr[1].u32YAddr = pstDrvImage->stBufAddr[1].u32PhyAddr_Y ; 
   pstUnfImage->stVideoFrameAddr[1].u32YStride =pstDrvImage->stBufAddr[1].u32Stride_Y ;
   pstUnfImage->stVideoFrameAddr[1].u32CAddr=pstDrvImage->stBufAddr[1].u32PhyAddr_C ;
   pstUnfImage->stVideoFrameAddr[1].u32CStride=pstDrvImage->stBufAddr[1].u32Stride_C ;
   pstUnfImage->stVideoFrameAddr[1].u32CrAddr = pstDrvImage->stBufAddr[1].u32PhyAddr_Cr;
   pstUnfImage->stVideoFrameAddr[1].u32CrStride = pstDrvImage->stBufAddr[1].u32Stride_Cr ;
   
   pstUnfImage->u32Width =pstDrvImage->u32Width; 
   pstUnfImage->u32Height=pstDrvImage->u32Height;  
   pstUnfImage->u32SrcPts=pstDrvImage->u32SrcPts; 
   pstUnfImage->u32Pts=pstDrvImage->u32Pts ;
   pstUnfImage->u32AspectWidth=pstDrvImage->u32AspectWidth;
   pstUnfImage->u32AspectHeight=pstDrvImage->u32AspectHeight;
   pstUnfImage->stFrameRate.u32fpsInteger=pstDrvImage->u32FrameRate;

   switch(pstDrvImage->ePixFormat)
   {
      case HI_DRV_PIX_FMT_NV61_2X1:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422 ;
        break;
      case HI_DRV_PIX_FMT_NV12:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420 ;
        break;
      case HI_DRV_PIX_FMT_NV21:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420 ;
        break;
      case HI_DRV_PIX_FMT_NV80:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_400;
        break;
      case HI_DRV_PIX_FMT_NV12_411:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_411;
        break;
      case HI_DRV_PIX_FMT_NV61:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2;
        break;
      case HI_DRV_PIX_FMT_NV42:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_444;
        break;
      case HI_DRV_PIX_FMT_UYVY:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_UYVY;
        break;
      case HI_DRV_PIX_FMT_YUYV:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
        break;
      case HI_DRV_PIX_FMT_YVYU:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YVYU;
        break;
      case HI_DRV_PIX_FMT_YUV400:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_400;
        break;
      case HI_DRV_PIX_FMT_YUV411:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_411;
        break;
      case HI_DRV_PIX_FMT_YUV420p:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_420;
        break;
      case HI_DRV_PIX_FMT_YUV422_1X2:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_1X2;
        break;
      case HI_DRV_PIX_FMT_YUV422_2X1:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_2X1;
        break;
      case HI_DRV_PIX_FMT_YUV_444:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_444;
        break;
      case HI_DRV_PIX_FMT_YUV410p:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_410;
        break;
      default:
        pstUnfImage->enVideoFormat = HI_UNF_FORMAT_YUV_BUTT;
        break;     
   }
   pstUnfImage->bProgressive = pstDrvImage->bProgressive;
   
   switch(pstDrvImage->enFieldMode) 
   {
      case HI_DRV_FIELD_ALL:
        pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
        break;
      case HI_DRV_FIELD_TOP:
        pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_TOP;
        break;
      case HI_DRV_FIELD_BOTTOM:
        pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_BOTTOM;
        break;
      default:
        pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_BUTT;
        break;
   }
   pstUnfImage->bTopFieldFirst=pstDrvImage->bTopFieldFirst;
   pstUnfImage->u32DisplayHeight=pstDrvImage->stDispRect.s32Height;
   pstUnfImage->u32DisplayWidth=pstDrvImage->stDispRect.s32Width;
   pstUnfImage->u32DisplayCenterX=pstDrvImage->stDispRect.s32X;
   pstUnfImage->u32DisplayCenterY=pstDrvImage->stDispRect.s32Y;
   pstUnfImage->enFramePackingType=(HI_UNF_VIDEO_FRAME_PACKING_TYPE_E)pstDrvImage->eFrmType ;
   pstUnfImage->u32Circumrotate=pstDrvImage->u32Circumrotate; 
   pstUnfImage->bHorizontalMirror =pstDrvImage->bToFlip_H;
   pstUnfImage->bVerticalMirror =pstDrvImage->bToFlip_V;
   pstUnfImage->u32ErrorLevel = pstDrvImage->u32ErrorLevel;

   memcpy( pstUnfImage->u32Private,pstDrvImage->u32Priv ,sizeof(HI_U32)*64);

   return HI_SUCCESS;
}

HI_VOID VENC_DRV_BoardInit(HI_VOID)
{
    HI_U32 u32RegValue = 0;
    HI_U32 i;

    g_VENCCrgCtrl = (HI_U32)ioremap_nocache(VENC_CRG_CONTROL, 0x4);

    if (g_VENCCrgCtrl)
    {

        /* reset */
        u32RegValue |= 0x10;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        msleep(1);
        
        /* open vedu clock */
        HI_REG_READ32(g_VENCCrgCtrl, u32RegValue);
        u32RegValue |= 0x1;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        msleep(1);

        /* config vedu clock frequency: 200Mhz */
        u32RegValue &= 0xfffffeff;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        /* config vedu clock frequency low down enable */
        u32RegValue |= 0x20000;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        msleep(1);

        /* cancel reset */
        u32RegValue &= 0xffffffef;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        for (i = 0; i < VENC_MAX_CHN_NUM; i++)
        {
            //init_waitqueue_head(&g_astVencWait[i]);
	         VENC_DRV_OsalInitEvent(&g_VencWait_Stream[i], 0);
        }

        init_waitqueue_head(&gEncodeFrame);
        gencodeframe = 0;
    }
}

HI_VOID VENC_DRV_BoardDeinit(HI_VOID)
{
    HI_U32 u32RegValue;

    if (g_VENCCrgCtrl)
    {
        /* reset */
        HI_REG_READ32(g_VENCCrgCtrl, u32RegValue);
        u32RegValue |= 0x10;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);

        msleep(1);

        /* close vedu clock */
        u32RegValue &= 0xfffffffe;
        HI_REG_WRITE32(g_VENCCrgCtrl, u32RegValue);
    }
}

HI_S32 VENC_DRV_CreateChn(HI_HANDLE *phVencChn, HI_UNF_VENC_CHN_ATTR_S *pstAttr,
                      VENC_CHN_INFO_S *pstVeInfo, struct file  *pfile)
{
    HI_S32 s32Ret = 0, i = 0;
    VeduEfl_EncCfg_S EncCfg;
    VeduEfl_EncPara_S *pstEncChnPara;
    VeduEfl_RcAttr_S RcAttrCfg;
    
    if ((HI_NULL == phVencChn) || (HI_NULL == pstAttr))
    {
        return HI_ERR_VENC_NULL_PTR;
    }

    if ((sizeof(VeduEfl_NALU_S)) > HI_VENC_STREAM_RESERV_SIZE)
    {
        HI_ERR_VENC("(sizeof(VeduEfl_NALU_S)) > HI_VENC_STREAM_RESERV_SIZE, check source code!\n");
        return HI_ERR_VENC_INVALID_PARA;
    }

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (g_stVencChn[i].hVEncHandle == HI_INVALID_HANDLE)
        {
            break;
        }
    }

    if (i == VENC_MAX_CHN_NUM)
    {
        HI_ERR_VENC("channal create err! \n");
        return HI_ERR_VENC_CREATE_ERR;
    }

    switch (pstAttr->enVencType)
    {
    case HI_UNF_VCODEC_TYPE_H264:
        EncCfg.Protocol = VEDU_H264;
        break;
    case HI_UNF_VCODEC_TYPE_H263:
        EncCfg.Protocol = VEDU_H263;
        break;
    case HI_UNF_VCODEC_TYPE_MPEG4:
        EncCfg.Protocol = VEDU_MPEG4;
        break;
    default:
        EncCfg.Protocol = VEDU_H264;
        break;
    }

    EncCfg.CapLevel      = pstAttr->enCapLevel;
    EncCfg.FrameWidth    = pstAttr->u32Width;
    EncCfg.FrameHeight   = pstAttr->u32Height;
    EncCfg.RotationAngle = pstAttr->u32RotationAngle;
    EncCfg.Priority      = pstAttr->u8Priority;
    EncCfg.streamBufSize = pstAttr->u32StrmBufSize;
	EncCfg.Gop           = pstAttr->u32Gop;
    EncCfg.QuickEncode   = pstAttr->bQuickEncode;
    EncCfg.SlcSplitEn    = (HI_TRUE == pstAttr->bSlcSplitEn) ? 1 : 0;
	if (EncCfg.SlcSplitEn)
	{
	    if (pstAttr->u32Width >= 1280)  
        {
           EncCfg.SplitSize  = 4;
        }
		else if (pstAttr->u32Width >= 720)
	    {
	       EncCfg.SplitSize  = 6;
	    }
		else 
	    {
	       EncCfg.SplitSize  = 8;
	    }			
	}
	else
	{
	    EncCfg.SplitSize  = 0;
	}
	
    //g_bPICEnd[i] = HI_FALSE;

    s32Ret = VENC_DRV_EflCreateVenc(phVencChn, &EncCfg);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    pstEncChnPara = (VeduEfl_EncPara_S*)(*phVencChn);
    RcAttrCfg.BitRate    = pstAttr->u32TargetBitRate;
    RcAttrCfg.InFrmRate  = pstAttr->u32InputFrmRate;
    RcAttrCfg.OutFrmRate = pstAttr->u32TargetFrmRate;
    RcAttrCfg.MaxQp      = pstAttr->u32MaxQp;
    RcAttrCfg.MinQp      = pstAttr->u32MinQp;
    
    s32Ret = VENC_DRV_EflRcSetAttr(*phVencChn, &RcAttrCfg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("config venc err:%#x.\n", s32Ret);
        VENC_DRV_EflDestroyVenc(*phVencChn);
        return HI_ERR_VENC_INVALID_PARA;
    }

    pstVeInfo->handle            = *phVencChn;
    pstVeInfo->u32StrmBufPhyAddr = pstEncChnPara->StrmBufAddr;
    pstVeInfo->u32BufSize        = pstEncChnPara->StrmBufSize;

    g_stVencChn[i].StrmBufAddr   = pstEncChnPara->StrmBufAddr;
    g_stVencChn[i].StrmBufSize   = pstEncChnPara->StrmBufSize;
    g_stVencChn[i].hVEncHandle   = *phVencChn;
    g_stVencChn[i].hSource       = HI_INVALID_HANDLE;
    g_stVencChn[i].stChnUserCfg  = *pstAttr;
    g_stVencChn[i].pWhichFile    = pfile;
	g_stVencChn[i].u32SliceSize            = EncCfg.SplitSize;
    g_stVencChn[i].u32FrameNumLastInput    = 0;
    g_stVencChn[i].u32FrameNumLastEncoded  = 0;
    g_stVencChn[i].u32TotalByteLastEncoded = 0;
    g_stVencChn[i].u32LastSecInputFps      = 0;
    g_stVencChn[i].u32LastSecEncodedFps    = 0;
    g_stVencChn[i].u32LastSecKbps          = 0;
    g_stVencChn[i].u32LastSecTryNum        = 0;
    g_stVencChn[i].u32LastTryNumTotal      = 0;
    g_stVencChn[i].u32LastSecPutNum        = 0;
    g_stVencChn[i].u32LastPutNumTotal      = 0;
    g_stVencChn[i].bNeedVPSS               = HI_FALSE;

    HI_INFO_VENC("create OK, Chn:%d/%#x.\n", i, g_stVencChn[i].hVEncHandle);
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_DestroyChn( HI_HANDLE hVencChn)
{
    HI_S32 s32Ret   = 0;
    HI_S32 s32VeChn = 0;
    OPTM_VENC_CHN_S *pstVenc;
    VI_EXPORT_FUNC_S *pViFunc = HI_NULL;

    D_VENC_GET_CHN(s32VeChn, hVencChn);
    D_VENC_CHECK_CHN(s32VeChn);

    pstVenc = &g_stVencChn[s32VeChn];


    //VENC must be stop working
    if (pstVenc->bEnable)
    {
        HI_WARN_VENC("Error:Destroy channel when VENC is run.\n");
        VENC_DRV_EflStopVenc(hVencChn);
    }

    s32Ret = VENC_DRV_EflDestroyVenc(hVencChn);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (g_stVencChn[s32VeChn].hSource != HI_INVALID_HANDLE)
    {
        if (HI_ID_VI == g_stVencChn[s32VeChn].enSrcModId)
        {
            HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
            if (HI_NULL != pViFunc)
            {
                s32Ret = pViFunc->pfnViPutUsrID(pstVenc->hSource & 0xff, pstVenc->u32SrcUser);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VENC("ViuPutUsrID failed, Ret=%#x.\n", s32Ret);
                    return s32Ret;
                }
            }
        }
    }

	g_stVencChn[s32VeChn].hSource     = HI_INVALID_HANDLE;
    g_stVencChn[s32VeChn].hVEncHandle = HI_INVALID_HANDLE;
    g_stVencChn[s32VeChn].bNeedVPSS   = HI_FALSE;
    HI_INFO_VENC("VENC_DestroyChn %d OK\n", s32VeChn);
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_AttachInput(HI_HANDLE hVencChn, HI_HANDLE hSrc, HI_MOD_ID_E enModId )
{
    HI_S32 ret;
    HI_S32 s32VeChn = 0;
    OPTM_VENC_CHN_S *pstVenc;
    VeduEfl_SrcInfo_S stSrcInfo;
    VI_EXPORT_FUNC_S *pViFunc = HI_NULL;
    WIN_EXPORT_FUNC_S *pVoFunc = HI_NULL;

    if (enModId >= HI_ID_BUTT)
    {
        return HI_ERR_VENC_INVALID_PARA;
    }

    D_VENC_GET_CHN(s32VeChn, hVencChn);
    D_VENC_CHECK_CHN(s32VeChn);

    if ((enModId != HI_ID_VI) && (enModId != HI_ID_VO))
    {
        HI_ERR_VENC("ModId not surpport now, enModId=%x!\n", enModId);
        return HI_ERR_VENC_INVALID_PARA;
    }

    pstVenc = &g_stVencChn[s32VeChn];
    if (pstVenc->hSource != HI_INVALID_HANDLE)
    {
        HI_ERR_VENC("Venc%d already attached to %#x!\n", s32VeChn, pstVenc->hSource);
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    switch (enModId)
    {
    case HI_ID_VI:
    {
        HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
        if (HI_NULL != pViFunc)
        {
            ret = pViFunc->pfnViGetUsrID(hSrc & 0xff, HI_ID_VENC, &(pstVenc->u32SrcUser));
            if (HI_SUCCESS != ret)
            {
                HI_ERR_VENC("Attach to VI failed, Ret=%#x.\n", ret);
                return ret;
            }

            stSrcInfo.handle = pstVenc->u32SrcUser;
            stSrcInfo.pfGetImage = (VE_IMAGE_FUNC)(pViFunc->pfnViAcquireFrame);
            stSrcInfo.pfPutImage = (VE_IMAGE_FUNC)(pViFunc->pfnViReleaseFrame);
        }

        break;
    }
    case HI_ID_VO:
    {
        HI_DRV_MODULE_GetFunction(HI_ID_VO, (HI_VOID**)&pVoFunc);
        if (HI_NULL != pVoFunc)
        {
            stSrcInfo.handle = hSrc;
            stSrcInfo.pfGetImage = (VE_IMAGE_FUNC)(pVoFunc->FN_AcquireFrame);
            stSrcInfo.pfPutImage = (VE_IMAGE_FUNC)(pVoFunc->FN_ReleaseFrame);
        }

        break;
    }
    default:
        break;
    }

    VENC_DRV_EflAttachInput(hVencChn, &stSrcInfo);

    pstVenc->enSrcModId = enModId;
    pstVenc->hSource = hSrc;

    HI_INFO_VENC("VENC%d attchInputOK, srcHdl:%#x, UserHdl:%#x.\n", s32VeChn, pstVenc->hSource, pstVenc->u32SrcUser);

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_DetachInput(HI_HANDLE hVencChn, HI_HANDLE hSrc, HI_MOD_ID_E enModId )
{
    HI_S32 s32VeChn = 0;
    HI_S32 s32Ret = HI_FAILURE;
    OPTM_VENC_CHN_S *pstVenc;
    VeduEfl_SrcInfo_S stSrcInfo;
    VI_EXPORT_FUNC_S *pViFunc = HI_NULL;

    if (enModId >= HI_ID_BUTT)
    {
        return HI_ERR_VENC_INVALID_PARA;
    }

    D_VENC_GET_CHN(s32VeChn, hVencChn);
    D_VENC_CHECK_CHN(s32VeChn);

    pstVenc = &g_stVencChn[s32VeChn];
    if (pstVenc->hSource == HI_INVALID_HANDLE)
    {
        HI_WARN_VENC("Venc%d NOT attached.\n", s32VeChn);
        return HI_SUCCESS;
    }

    if (pstVenc->hSource != hSrc)
    {
        HI_ERR_VENC("Venc%d NOT attached to %#x, but attached to %#x.\n", s32VeChn, hSrc, pstVenc->hSource);
        return HI_SUCCESS;
    }

    if ((enModId != HI_ID_VI) && (enModId != HI_ID_VO))
    {
        HI_ERR_VENC("Venc Detach, ModId not surpport now, enModId=%x!\n", enModId);
        return HI_ERR_VENC_INVALID_PARA;
    }

    //VENC must be stop working
    if (pstVenc->bEnable)
    {
        HI_ERR_VENC("CanNOT detachInput when VENC is run.\n");
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }


    switch (enModId)
    {
    case HI_ID_VI:
    {
        stSrcInfo.handle = HI_INVALID_HANDLE;
        stSrcInfo.pfGetImage = HI_NULL;
        stSrcInfo.pfPutImage = HI_NULL;

        HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
        if (HI_NULL != pViFunc)
        {
            s32Ret = pViFunc->pfnViPutUsrID(hSrc & 0xff, pstVenc->u32SrcUser);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VENC("ViuPutUsrID failed, Ret=%#x.\n", s32Ret);
                return s32Ret;
            }
        }

        break;
    }
    case HI_ID_VO:
    {
        stSrcInfo.handle = HI_INVALID_HANDLE;
        stSrcInfo.pfGetImage = HI_NULL;
        stSrcInfo.pfPutImage = HI_NULL;
        break;
    }
    default:
        break;
    }
    VENC_DRV_EflDetachInput(hVencChn, &stSrcInfo);
    HI_INFO_VENC("VENC%d dettchInputOK, srcHdl:%#x, UserHdl:%#x.\n", s32VeChn, pstVenc->hSource, pstVenc->u32SrcUser);

    g_stVencChn[s32VeChn].enSrcModId = HI_ID_BUTT;
    g_stVencChn[s32VeChn].hSource = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_AcquireStream(HI_HANDLE hVencChn, HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs,
                          VENC_BUF_OFFSET_S *pstBufOffSet)
{
    HI_S32 s32Ret   = -1;
    HI_S32 s32VeChn = 0;
    VeduEfl_NALU_S stVeduPacket;
	VeduEfl_EncPara_S *pstEncChnPara;
    extern VENC_PROC_WRITE_S g_VencProcWrite;
    static HI_BOOL bTagFirstTimeSave = HI_TRUE;
    static HI_U32 u32SaveFrameStartCount = 0;
    VeduEfl_StatInfo_S StatInfo;

    D_VENC_CHECK_PTR(pstStream);
    D_VENC_CHECK_PTR(pstBufOffSet);

    s32Ret = down_interruptible(&g_VencStreamMutex);
    D_VENC_GET_CHN(s32VeChn, hVencChn);
    //D_VENC_CHECK_CHN(s32VeChn);
	if ((0 > s32VeChn) || (VENC_MAX_CHN_NUM <= s32VeChn))
	{   
	    up(&g_VencStreamMutex);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    } 

    pstEncChnPara = (VeduEfl_EncPara_S*)hVencChn;
	pstEncChnPara->stStat.GetStreamNumTry++; 
	
    if (VENC_DRV_EflGetStreamLen(hVencChn) <= 0)
    {
        if (u32TimeoutMs == 0)
        {
            up(&g_VencStreamMutex);
            return HI_ERR_VENC_BUF_EMPTY;

        }
        else 
        {
            s32Ret = VENC_DRV_OsalWaitEvent(&g_VencWait_Stream[s32VeChn], u32TimeoutMs);
            if (HI_FAILURE == s32Ret)
            {
                up(&g_VencStreamMutex);
                return HI_ERR_VENC_BUF_EMPTY;
            }

            s32Ret = VENC_DRV_EflGetStreamLen(hVencChn);
            if (s32Ret <= 0)
            {
                up(&g_VencStreamMutex);
                return HI_ERR_VENC_BUF_EMPTY;
            }
        }
    }

    s32Ret = VENC_DRV_EflGetBitStream(hVencChn, &stVeduPacket);
    if (s32Ret != HI_SUCCESS)
    {
        up(&g_VencStreamMutex);
        return HI_ERR_VENC_BUF_EMPTY;
    }
	
    pstEncChnPara->stStat.GetStreamNumOK++;
	
    memcpy(&g_stVencChn[s32VeChn].stChnPacket, &stVeduPacket, sizeof(stVeduPacket));
    pstStream->pu8Addr      = stVeduPacket.pVirtAddr[0];
    pstStream->u32SlcLen    = stVeduPacket.SlcLen[0]+stVeduPacket.SlcLen[1];
    pstStream->u32PtsMs     = stVeduPacket.PTS0;
  
    pstStream->bFrameEnd = (0 == stVeduPacket.bFrameEnd) ? HI_FALSE : HI_TRUE;
    pstStream->enDataType.enH264EType = stVeduPacket.NaluType;
    if (pstStream->u32SlcLen > 0) 
    {
        pstBufOffSet->u32StrmBufOffset[0]
        = stVeduPacket.PhyAddr[0] - g_stVencChn[s32VeChn].StrmBufAddr;
    }

    /*
    if(HI_UNF_VENC_TYPE_H264==g_stVencChn[s32VeChn].eVencType)
    {
        pstStream->enDataType.enH264EType=*((HI_CHAR*)stVeduPacket.pVirtAddr[0]+4)&0x1f;
    }
     */

    /*
    HI_INFO_VENC("GetOK, Chn%d, %d,%#x / %d,%#x.\n", s32VeChn,
                    pstPacket->SlcLen[0], pstPacket->PhyAddr[0],
                    pstPacket->SlcLen[1], pstPacket->PhyAddr[1]);
     */
    HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_VSYNC, "PTS=%u.\n", pstStream->u32PtsMs);

    if ((HI_TRUE == g_VencProcWrite.bTimeModeRun) || (HI_TRUE == g_VencProcWrite.bFrameModeRun))
    {
        s32Ret = VENC_DRV_EflQueryStatInfo(g_stVencChn[s32VeChn].hVEncHandle, &StatInfo);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VENC("VeduEfl_QueryStatInfo failed.\n");
			up(&g_VencStreamMutex);
            return HI_FAILURE;
        }

        /* request one I frame and record u32SaveFrameStartCount to compare with g_VencSaveFrameCount when save file firstly */
        if (HI_TRUE == bTagFirstTimeSave)
        {
            VENC_DRV_EflRequestIframe(hVencChn);
            bTagFirstTimeSave = HI_FALSE;
            u32SaveFrameStartCount = StatInfo.GetFrameNumOK - StatInfo.SkipFrmNum;
			up(&g_VencStreamMutex);
            return HI_SUCCESS;
        }

        /* compare with u32FrameModeCount each time */
        if ((HI_TRUE == g_VencProcWrite.bFrameModeRun)
            && (StatInfo.GetFrameNumOK - StatInfo.SkipFrmNum - u32SaveFrameStartCount)
            > g_VencProcWrite.u32FrameModeCount)
        {
            /* time to stop save file */
            g_VencProcWrite.bFrameModeRun = HI_FALSE;
			up(&g_VencStreamMutex);
            return HI_SUCCESS;
        }
        if (pstStream->u32SlcLen > 0)
        {
            s32Ret = HI_DRV_FILE_Write(g_VencProcWrite.fpSaveFile, pstStream->pu8Addr, pstStream->u32SlcLen);
            if (s32Ret != pstStream->u32SlcLen)
            {
                HI_ERR_VENC("VeduOsal_Fwrite failed.\n");
                g_VencProcWrite.bTimeModeRun  = HI_FALSE;
                g_VencProcWrite.bFrameModeRun = HI_FALSE;
            }
        }
    }
    /* end of save file */
    else
    {
        bTagFirstTimeSave = HI_TRUE;
    }

    up(&g_VencStreamMutex);
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_ReleaseStream(HI_HANDLE hVencChn, HI_UNF_VENC_STREAM_S *pstStream)
{
    HI_S32 s32VeChn = 0;
    HI_S32 s32Ret = 0;
    VeduEfl_NALU_S stVeduPacket;

    D_VENC_CHECK_PTR(pstStream);

    D_VENC_GET_CHN(s32VeChn, hVencChn);
    D_VENC_CHECK_CHN(s32VeChn);

    memcpy(&stVeduPacket, &g_stVencChn[s32VeChn].stChnPacket, sizeof(stVeduPacket));
    s32Ret = VENC_DRV_EflSkpBitStream(hVencChn, &stVeduPacket);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("Release stream failed, ret= %#x.\n", s32Ret);
        return HI_ERR_VENC_CHN_RELEASE_ERR;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_StartReceivePic(HI_U32 EncHandle)
{
    HI_S32 s32Ret   = HI_FAILURE;
    HI_U32 u32VeChn = 0;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);
#if 0
    if (g_stVencChn[u32VeChn].hSource == HI_INVALID_HANDLE)
    {
        HI_ERR_VENC("source venc%d is NOT Attached.\n", u32VeChn);
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }
#endif
    if (HI_TRUE == g_stVencChn[u32VeChn].bEnable)
    {
        return HI_SUCCESS;
    }

    s32Ret = VENC_DRV_EflStartVenc(EncHandle);
    if (HI_SUCCESS == s32Ret)
    {
        g_stVencChn[u32VeChn].bEnable = HI_TRUE;
        do_gettimeofday(&(g_stVencChn[u32VeChn].stTimeStart));
        HI_INFO_VENC("start Chn %d/%#x. OK\n", u32VeChn, EncHandle);
        VENC_DRV_EflRequestIframe(EncHandle);
    }
    else
    {
        s32Ret = HI_ERR_VENC_INVALID_CHNID;
    }
    return s32Ret;
}

HI_S32 VENC_DRV_StopReceivePic(HI_U32 EncHandle)
{
    HI_S32 s32Ret   = HI_FAILURE;
    HI_U32 u32VeChn = 0;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);

    if (HI_FALSE == g_stVencChn[u32VeChn].bEnable)
    {
        return HI_SUCCESS;
    }

    s32Ret = VENC_DRV_EflStopVenc(EncHandle);
    if (HI_SUCCESS == s32Ret)
    {
        g_stVencChn[u32VeChn].bEnable = HI_FALSE;
        HI_INFO_VENC("stop Chn %d/%#x. OK\n", u32VeChn, EncHandle);
    }
    return s32Ret;
}

HI_S32 VENC_DRV_SetAttr(HI_U32 EncHandle, HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    HI_S32 s32Ret   = -1;
    HI_S32 s32VeChn = 0;
    HI_BOOL RcFlag  = HI_FALSE;
    OPTM_VENC_CHN_S *pstVenc;
    VeduEfl_RcAttr_S RcAttrCfg;
	HI_U32 u32TotalMb_old,u32TotalMb_new,u32PicWidthInMb,u32PicHeightInMb;
    VeduEfl_EncPara_S* pstEncChnPara = (VeduEfl_EncPara_S*)EncHandle;
    
    D_VENC_GET_CHN(s32VeChn, EncHandle);
    D_VENC_CHECK_CHN(s32VeChn);
    pstVenc = &g_stVencChn[s32VeChn];

    RcFlag |= (pstVenc->stChnUserCfg.u32TargetBitRate != pstAttr->u32TargetBitRate);
    RcFlag |= (pstVenc->stChnUserCfg.u32InputFrmRate  != pstAttr->u32InputFrmRate);
    RcFlag |= (pstVenc->stChnUserCfg.u32TargetFrmRate != pstAttr->u32TargetFrmRate);
    RcFlag |= (pstVenc->stChnUserCfg.u32MaxQp         != pstAttr->u32MaxQp);
    RcFlag |= (pstVenc->stChnUserCfg.u32MinQp         != pstAttr->u32MinQp);
    if (HI_TRUE == RcFlag)             
    {
        RcAttrCfg.BitRate    = pstAttr->u32TargetBitRate;
        RcAttrCfg.InFrmRate  = pstAttr->u32InputFrmRate;
        RcAttrCfg.OutFrmRate = pstAttr->u32TargetFrmRate;
        RcAttrCfg.MaxQp      = pstAttr->u32MaxQp;
        RcAttrCfg.MinQp      = pstAttr->u32MinQp;

        s32Ret = VENC_DRV_EflRcSetAttr(pstVenc->hVEncHandle, &RcAttrCfg);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("config venc Rate Control Attribute err:%#x.\n", s32Ret);
            return HI_FAILURE;
        }
    }
    
    if (pstVenc->stChnUserCfg.u8Priority != pstAttr->u8Priority)
    {
        HI_U32 PriorityID;
        D_VENC_GET_PRIORITY_ID(s32VeChn, PriorityID);
        PriorityTab[1][PriorityID] = pstAttr->u8Priority;
        VENC_DRV_EflSortPriority();
    }

    pstVenc->stChnUserCfg.u32Gop           = pstAttr->u32Gop;
    pstVenc->stChnUserCfg.u8Priority       = pstAttr->u8Priority;
    pstVenc->stChnUserCfg.u32TargetBitRate = pstAttr->u32TargetBitRate;
    pstVenc->stChnUserCfg.u32InputFrmRate  = pstAttr->u32InputFrmRate;
    pstVenc->stChnUserCfg.u32TargetFrmRate = pstAttr->u32TargetFrmRate;
    pstVenc->stChnUserCfg.u32MaxQp         = pstAttr->u32MaxQp;
    pstVenc->stChnUserCfg.u32MinQp         = pstAttr->u32MinQp;

    pstEncChnPara->Gop                     = pstAttr->u32Gop;
 
    if ((pstVenc->stChnUserCfg.u32Height  != pstAttr->u32Height)
        || (pstVenc->stChnUserCfg.u32Width!= pstAttr->u32Width))
    {
        u32PicWidthInMb  = D_VENC_ALIGN_UP(pstVenc->stChnUserCfg.u32Width, HI_VENC_PIC_SZIE_ALIGN)>> 4;
		u32PicHeightInMb = D_VENC_ALIGN_UP(pstVenc->stChnUserCfg.u32Height,HI_VENC_PIC_SZIE_ALIGN)>> 4;
        u32TotalMb_old   = u32PicWidthInMb * u32PicHeightInMb;
		
        u32PicWidthInMb  = D_VENC_ALIGN_UP(pstAttr->u32Width, HI_VENC_PIC_SZIE_ALIGN)>> 4;
		u32PicHeightInMb = D_VENC_ALIGN_UP(pstAttr->u32Height,HI_VENC_PIC_SZIE_ALIGN)>> 4;
        u32TotalMb_new   = u32PicWidthInMb * u32PicHeightInMb;
       
        if (u32TotalMb_new > u32TotalMb_old)
        {
            HI_ERR_VENC("Vedu not support this change of resolution ratio!\nplease change after vedu stop!\n");
            return HI_FAILURE;
        }
        //s32Ret = VENC_DRV_EflStopVenc(EncHandle);
        s32Ret = VENC_DRV_EflRequestIframe(EncHandle);
        if (HI_SUCCESS == s32Ret)
        {
            s32Ret = VENC_DRV_EflSetResolution(EncHandle, pstAttr->u32Width, pstAttr->u32Height);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VENC("VeduEfl_SetResolution err:%#x.\n", s32Ret);
            }

            //VENC_DRV_EflStartVenc(EncHandle);
            pstVenc->stChnUserCfg.u32Height = pstAttr->u32Height;
            pstVenc->stChnUserCfg.u32Width  = pstAttr->u32Width;
        }
        else
        {
            HI_ERR_VENC("Vedu change resolution ratio failed!:%#x.\n", s32Ret);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_GetAttr(HI_U32 EncHandle, HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    HI_S32 s32VeChn = 0;
    OPTM_VENC_CHN_S *pstVenc;

    D_VENC_GET_CHN(s32VeChn, EncHandle);
    D_VENC_CHECK_CHN(s32VeChn);
    pstVenc = &g_stVencChn[s32VeChn];

    *pstAttr = pstVenc->stChnUserCfg;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_RequestIFrame(HI_U32 EncHandle)
{
    HI_S32 s32Ret   = -1;
    HI_S32 s32VeChn = 0;
    OPTM_VENC_CHN_S *pstVenc;

    D_VENC_GET_CHN(s32VeChn, EncHandle);
    D_VENC_CHECK_CHN(s32VeChn);
    pstVenc = &g_stVencChn[s32VeChn];

    s32Ret = VENC_DRV_EflRequestIframe(pstVenc->hVEncHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("request IFrame err:%#x.\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_QueueFrame(HI_HANDLE hVencChn, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameInfo )
{
   HI_S32 s32Ret;
   HI_S32 s32VeChn = 0;
   HI_DRV_VIDEO_FRAME_S stFrame;
   VeduEfl_EncPara_S *pstEncChnPara;
   
   D_VENC_GET_CHN(s32VeChn, hVencChn);
   if(VENC_MAX_CHN_NUM == s32VeChn )
   {
       HI_ERR_VENC("the handle(%x) does not start or even not be create either!\n",hVencChn);
       return HI_ERR_VENC_INVALID_PARA;
   }

   pstEncChnPara = (VeduEfl_EncPara_S *)hVencChn;
   if (HI_TRUE == pstEncChnPara->bNeverEnc)
   {
       if (HI_INVALID_HANDLE == g_stVencChn[s32VeChn].hSource)   //如果未绑定则设置
        {
            pstEncChnPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
            pstEncChnPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;
        }
       else
        {
            HI_ERR_VENC("the venc had already attach another sourse!QueueFrame is  invalid!! \n");
            return HI_ERR_VENC_CHN_INVALID_STAT;
        }
   }
   s32Ret = Convert_FrameStructure(pstFrameInfo,&stFrame);
#ifdef VENC_TO_VPSS_SUPPORT
   if (HI_TRUE == pstEncChnPara->bNeverEnc)
   {
      VENC_DRV_EflJudgeVPSS(g_stVencChn[s32VeChn].hVEncHandle , &stFrame, HI_FALSE);
   }
   
   if (HI_TRUE == g_stVencChn[s32VeChn].bNeedVPSS)
   {
      //s32Ret = HI_DRV_VPSS_PutImage(g_stVencChn[s32VeChn].hVPSS,&stFrame); 
      stFrame.bProgressive = 1;
      s32Ret = (pVpssFunc->pfnVpssPutImage)(g_stVencChn[s32VeChn].hVPSS,&stFrame); 
   }
   else
   {
      s32Ret = VENC_DRV_EflQueueFrame(hVencChn, &stFrame);
   }
#else
   s32Ret = VENC_DRV_EflQueueFrame(hVencChn, &stFrame);
#endif
   

   if (HI_SUCCESS != s32Ret)
   {
        return HI_FAILURE;
   }
   return HI_SUCCESS;
}

HI_S32 VENC_DRV_DequeueFrame(HI_HANDLE hVencChn, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameInfo )
{
   HI_S32 s32Ret;
   HI_S32 s32VeChn = 0;
   HI_DRV_VIDEO_FRAME_S stFrame;
   //VeduEfl_EncPara_S *pstEncChnPara;
   D_VENC_GET_CHN(s32VeChn, hVencChn);
   if(VENC_MAX_CHN_NUM == s32VeChn )
   {
       HI_ERR_VENC("VENC_DRV_QueueFrame:: the handle(%x) does not start or even not be create either!\n",hVencChn);
       return HI_ERR_VENC_INVALID_PARA;
   }

   //pstEncChnPara = (VeduEfl_EncPara_S *)hVencChn;

    /*if (HI_INVALID_HANDLE == g_stVencChn[s32VeChn].hSource)   //如果未绑定则设置
    {
        pstEncChnPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
        pstEncChnPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;
    }
   else
    {
        HI_ERR_VENC("the venc had already attach another sourse!DequeueFrame is  invalid!! \n");
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }*/

#ifdef VENC_TO_VPSS_SUPPORT
   if (HI_TRUE == g_stVencChn[s32VeChn].bNeedVPSS)
   {
     s32Ret = (pVpssFunc->pfnVpssGetImage)(g_stVencChn[s32VeChn].hVPSS,&stFrame);
   }
   else
   {
     s32Ret = VENC_DRV_EflDequeueFrame(hVencChn, &stFrame);
   }

#else
   s32Ret = VENC_DRV_EflDequeueFrame(hVencChn, &stFrame);
#endif
   
   if (HI_SUCCESS != s32Ret)
   {
        return HI_FAILURE;
   }
   s32Ret = Convert_DrvFrameStructure(&stFrame, pstFrameInfo);
   return HI_SUCCESS;
}

/*new function interface*/

HI_S32 VENC_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    s32Ret = HI_DRV_MODULE_Register(HI_ID_VENC, "HI_VENC", (HI_VOID*)&s_VencExportFuncs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("HI_DRV_MODULE_Register failed, mode ID = 0x%08X\n", HI_ID_VENC);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID VENC_DRV_Exit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_VENC);
    return;
}


HI_S32 HI_DRV_VENC_Init(HI_VOID)
{
    HI_S32 s32Ret = 0;

    /*Init the VENC device*/
    s32Ret = VENC_DRV_Init();
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("Init VENC drv fail!\n");
        return HI_FAILURE;
    }

    /*open vedu clock*/
    VENC_DRV_BoardInit();

    /*creat thread to manage channel*/
    s32Ret = VENC_DRV_EflOpenVedu();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_VENC("VeduEfl_OpenVedu failed, ret=%d\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 HI_DRV_VENC_DeInit(HI_VOID)
{

    HI_S32 s32Ret = 0;
    HI_U32 i = 0;

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (g_stVencChn[i].hVEncHandle == HI_INVALID_HANDLE)
        {
            break;
        }
    }
    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (/*(g_stVencChn[i].pWhichFile == ffile)&& */(g_stVencChn[i].hVEncHandle != HI_INVALID_HANDLE))
        {
            HI_INFO_VENC("Try VENC_DestroyChn %d/%#x.\n", i, g_stVencChn[i].hVEncHandle);
            s32Ret = VENC_DRV_DestroyChn(g_stVencChn[i].hVEncHandle);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VENC("force DestroyChn %d failed, Ret=%#x.\n", i, s32Ret);
            }
            g_stVencChn[i].pWhichFile = HI_NULL;
        }
    }

    s32Ret = VENC_DRV_EflCloseVedu();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_VENC("VeduEfl_CloseVedu failed, ret=%d\n", s32Ret);
        return HI_FAILURE;
    }

    /*close the venc lock*/
    VENC_DRV_BoardDeinit();

    VENC_DRV_Exit();
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    if(NULL==pstAttr)
    {
        return HI_ERR_VENC_NULL_PTR;
    }
    pstAttr->u32Width=720;
    pstAttr->u32Height=576;
    pstAttr->enVencType=HI_UNF_VCODEC_TYPE_H264;
	pstAttr->enCapLevel=HI_UNF_VCODEC_CAP_LEVEL_D1;
    pstAttr->u32RotationAngle=0;

    pstAttr->bSlcSplitEn = HI_FALSE;
    //pstAttr->u32SplitSize =4;
    
    pstAttr->u32StrmBufSize = 720 * 576 * 2;   
    
    pstAttr->u32TargetBitRate = 4*1024*1024;    
    pstAttr->u32InputFrmRate = 25;    
    pstAttr->u32TargetFrmRate = 25;    
    pstAttr->u32Gop = 0x7fffffff;    
    pstAttr->u32MaxQp = 48;
    pstAttr->u32MinQp = 16;
    
    pstAttr->bQuickEncode = HI_FALSE;
    pstAttr->u8Priority   = 0;
    
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VENC_Create(HI_HANDLE *phVencChn, HI_UNF_VENC_CHN_ATTR_S *pstAttr, struct file  *pfile)
{
   VENC_CHN_INFO_S stVeInfo;   //no use output in the mode
   return VENC_DRV_CreateChn(phVencChn,pstAttr,&stVeInfo,pfile);   
}

HI_S32 HI_DRV_VENC_Destroy(HI_HANDLE hVenc)
{
   return VENC_DRV_DestroyChn(hVenc);
}

HI_S32 HI_DRV_VENC_AttachInput(HI_HANDLE hVenc,HI_HANDLE hSrc)
{
    return VENC_DRV_AttachInput(hVenc, hSrc, ((hSrc & 0xff0000) >> 16));
}

HI_S32 HI_DRV_VENC_DetachInput(HI_HANDLE hVencChn)
{
    HI_S32 s32VeChn = 0;
    HI_HANDLE hSrc = 0;
    VeduEfl_EncPara_S *pstEncChnPara = NULL;
    D_VENC_GET_CHN(s32VeChn, hVencChn);
    D_VENC_CHECK_CHN(s32VeChn);
    pstEncChnPara = (VeduEfl_EncPara_S *)hVencChn;
    hSrc = pstEncChnPara->stSrcInfo.handle;
    return VENC_DRV_DetachInput(hVencChn,hSrc, ((hSrc & 0xff0000) >> 16));
}

HI_S32 HI_DRV_VENC_Start(HI_HANDLE hVenc)
{
    return VENC_DRV_StartReceivePic(hVenc);
}

HI_S32 HI_DRV_VENC_Stop(HI_HANDLE hVenc)
{
    return VENC_DRV_StopReceivePic(hVenc);
}

HI_S32 HI_DRV_VENC_AcquireStream(HI_HANDLE hVenc,HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs)
{
   VENC_BUF_OFFSET_S stBufOffSet;    //no use output in the mode
   return VENC_DRV_AcquireStream(hVenc, pstStream, u32TimeoutMs, &stBufOffSet);
}

HI_S32 HI_DRV_VENC_ReleaseStream(HI_HANDLE hVenc, HI_UNF_VENC_STREAM_S *pstStream)
{
   return VENC_DRV_ReleaseStream(hVenc, pstStream);
}

HI_S32 HI_DRV_VENC_SetAttr(HI_HANDLE hVenc,HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
   return VENC_DRV_SetAttr(hVenc, pstAttr);
}

HI_S32 HI_DRV_VENC_GetAttr(HI_HANDLE hVenc, HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
   return VENC_DRV_GetAttr(hVenc, pstAttr);
}

HI_S32 HI_DRV_VENC_RequestIFrame(HI_HANDLE hVenc)
{
   return VENC_DRV_RequestIFrame(hVenc);
}

HI_S32 HI_DRV_VENC_QueueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
   return VENC_DRV_QueueFrame(hVenc, pstFrameinfo );
}

HI_S32 HI_DRV_VENC_DequeueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
   return VENC_DRV_DequeueFrame(hVenc, pstFrameinfo);
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

