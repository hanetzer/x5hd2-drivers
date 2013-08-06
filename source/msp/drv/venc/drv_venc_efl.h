#ifndef __DRV_VENC_EFL_H__
#define __DRV_VENC_EFL_H__

#include "hi_type.h"
#include "drv_venc_buf_mng.h"
#include "hi_unf_common.h"
#include "hi_drv_video.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#ifdef __VENC_S40V200_CONFIG__
#define VENC_TO_VPSS_SUPPORT
#endif

#define VEDU_TR_STEP 1

#define D_VENC_ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )
typedef HI_S32 (*VE_IMAGE_FUNC)(HI_S32 handle, HI_DRV_VIDEO_FRAME_S *pstImage);    //liminqi

typedef struct
{
    HI_U32 ProfileIDC;
    HI_U32 FrameWidthInMb;
    HI_U32 FrameHeightInMb;
    HI_U8  FrameCropLeft;
    HI_U8  FrameCropRight;
    HI_U8  FrameCropTop;
    HI_U8  FrameCropBottom;
} VeduEfl_H264e_SPS_S;

typedef struct
{
    HI_U32  ConstIntra;
    HI_S32  ChrQpOffset;
	HI_U32  H264HpEn;
    HI_U32  H264CabacEn;
    HI_S32 *pScale8x8;
} VeduEfl_H264e_PPS_S;

typedef struct
{
    HI_U32 slice_type; /* 0-P, 2-I */
    HI_U32 frame_num;
	HI_U32 NumRefIndex; /* 0 or 1 */
} VeduEfl_H264e_SlcHdr_S;

typedef struct
{
    HI_U32 Pframe;   /* 1-P, 0-I */
    HI_U32 TR;       /* temporal ref */
    HI_U32 Wframe;
    HI_U32 Hframe;
    HI_U32 SrcFmt;
    HI_U32 PicQP;
} VeduEfl_H263e_PicHdr_S;

typedef struct
{
    HI_U32 Wframe;
    HI_U32 Hframe;
} VeduEfl_MP4e_VOL_S;

typedef struct
{
    HI_U32 Pframe;        /* 1-P, 0-I */
    HI_U32 PicQP;
    HI_U32 Fcode;
} VeduEfl_MP4e_VopHdr_S;

typedef struct
{
    HI_U32 PacketLen;     /* 64 aligned */
    HI_U32 InvldByte;     /* InvalidByteNum */
    HI_U8  Type;
    HI_U8  bBotField;
    HI_U8  bField;
    HI_U8  bLastSlice;
    HI_U32 ChnID;
    HI_U32 PTS0;
    HI_U32 PTS1;
    HI_U32 Reserved[10];
} VeduEfl_NaluHdr_S;

typedef struct
{
    HI_S32        handle;
    VE_IMAGE_FUNC pfGetImage;
    VE_IMAGE_FUNC pfPutImage;
} VeduEfl_SrcInfo_S;

typedef struct
{
    HI_U32 GetFrameNumTry;
    HI_U32 PutFrameNumTry;
    HI_U32 GetStreamNumTry;
    HI_U32 PutStreamNumTry;

    HI_U32 GetFrameNumOK;
    HI_U32 PutFrameNumOK;
    HI_U32 GetStreamNumOK;
    HI_U32 PutStreamNumOK;

    HI_U32 BufFullNum;
    HI_U32 SkipFrmNum;

    HI_U32 StreamTotalByte;
} VeduEfl_StatInfo_S;

typedef struct
{
    HI_U32 Protocol;      /* VEDU_H264, VEDU_H263 or VEDU_MPEG4 */
	HI_U32 CapLevel;      /* VEDU buffer alloc*/
    HI_U32 FrameWidth;    /* width	in pixel, 96 ~ 2048 */
    HI_U32 FrameHeight;   /* height in pixel, 96 ~ 2048 */

    HI_U32 RotationAngle; /* venc don't care */

    HI_U32 PackageSel;
    HI_U32 Priority;
    HI_U32 streamBufSize;

    HI_BOOL QuickEncode;
    HI_BOOL SlcSplitEn;    /* 0 or 1, slice split enable */
	HI_BOOL SlcSplitMod;   /* 0 or 1, 0:byte; 1:mb line */
    HI_U32 SplitSize;     /* <512 mb line @ H264, H263 don't care*/
    HI_U32 Gop;
    

} VeduEfl_EncCfg_S;

typedef struct
{
    HI_U32 BitRate;       /* 32k ~ 20M, bits per second */
    HI_U32 OutFrmRate;    /* 1 ~ InFrmRate */
    HI_U32 InFrmRate;     /* 1 ~ 30  */

    HI_U32   MaxQp;         /* H264: 0 ~ 51, Mpeg4: 1 ~ 31 */
    HI_U32   MinQp;         /* MinQp <= MaxQp */
} VeduEfl_RcAttr_S;

typedef struct
{
    HI_VOID*   pVirtAddr[2];
    HI_U32     PhyAddr[2];
    HI_U32     SlcLen[2];  /* byte */
    HI_U32     PTS0;
    HI_U32     PTS1;
    HI_U32     bFrameEnd;
    HI_U32     NaluType;   /* 1(P),5(I),6(SEI),7(SPS) or 8(PPS), only used by H264	*/
	HI_U32     InvldByte;
} VeduEfl_NALU_S;

typedef struct
{
    HI_U32   Enable [8];    /* only used at H264  */
    HI_U32   AbsQpEn[8];
    HI_S32   Qp     [8];    /* -26 ~ 25 or 0 ~ 51 */
    HI_U32   Width  [8];    /* size in MB */
    HI_U32   Height [8];    /* size in MB */
    HI_U32   StartX [8];    /* size in MB */
    HI_U32   StartY [8];    /* size in MB */
	HI_U32   Keep   [8];    
    
} VeduEfl_RoiAttr_S;

typedef struct
{
    HI_U32 osd_rgbfm;            /* 0 ~ 1 */
    HI_S32 osd_global_en   [8];
    HI_S32 osd_en          [8];
    HI_U32 osd_alpha0      [8];
    HI_U32 osd_alpha1      [8];
    HI_U32 osd_global_alpha[8];
    HI_U32 osd_x           [8]; /* pixel, 2 aligned */
    HI_U32 osd_y           [8]; /* pixel, 2 aligned */
    HI_U32 osd_w           [8]; /* pixel, 2 aligned */
    HI_U32 osd_h           [8]; /* pixel, 2 aligned */
    HI_U32 osd_addr        [8]; /* 16-byte aligned  */
    HI_U32 osd_stride      [8]; /* 64-byte aligned  */
    HI_U32 osd_layer_id    [8]; /* 0 ~ 7 */
    
    HI_S32 osd_absqp_en    [8]; /* only used at H264  */
    HI_S32 osd_qp          [8]; /* -26 ~ 25 or 0 ~ 51 */
    
    HI_U32 osd_invs_en     [8];
    HI_U32 osd_invs_w;          /* 1 ~ 4 */
    HI_U32 osd_invs_h;          /* 1 ~ 4 */
    HI_U32 osd_invs_thr;        /* 0 ~ 255 */
    HI_U32 osd_invs_mode;       /* 0 ~ 1 */
    
}VeduEfl_OsdAttr_S;

typedef struct
{
    /* channel parameter */
    HI_U32 Protocol;
    HI_U32  H264CabacEn;       //不需要外部传，内部做策略                                                                  //
    HI_U32  H264HpEn;          //不需要外部传，内部做策略：S40V200，CV200均不支持High profile                                                                 //

    HI_U32 PicWidth;           /* done for 16 aligned @ venc */
    HI_U32 PicHeight;          /* done for 16 aligned @ venc */
    HI_U32 YuvStoreType;
    HI_U32 YuvSampleType;
    HI_U32 RotationAngle;
    //HI_U32 FixedQP;
    HI_U16 SlcSplitEn;
    HI_U8  SlcSplitMod;       /* 0 or 1, byte or mb line ,just select 1*/
    HI_U32 SplitSize;
#if 0
    HI_U32  MaskIntEn;          /* 0: interrupt mode, 1: query mode */                         //
    HI_U32  RegConfigMode;                                                                     //32->8
    HI_U32  RegConfigAddr;
    HI_U8  *pDdrBase;                                                                          //??
#endif
    HI_U8  Priority;
    HI_BOOL QuickEncode;

    /* frame buffer parameter */
    HI_U32 SStrideY;
    HI_U32 SStrideC;
    HI_U32  StoreFmt;           /* 0, semiplannar; 1, package; 2,planer */
    HI_U32 PackageSel;
    HI_U32 SrcYAddr;
    HI_U32 SrcCAddr;
    HI_U32 SrcVAddr;                  //just for input of planner
    HI_U32 RcnYAddr[2];
    HI_U32 RcnCAddr[2];
    HI_U32 RcnIdx;             /* 0 or 1, idx for rcn, !idx for ref */
#if 1

    HI_U32  RStrideY;        
    HI_U32  RStrideC;
#endif
    /* stream buffer parameter */
    HI_U32          StrmBufAddr;
    HI_U32          StrmBufRpAddr; /* phy addr for hardware */
    HI_U32          StrmBufWpAddr;
    HI_U32          StrmBufSize;
    HI_U32          Vir2BusOffset; /* offset = vir - bus, @ stream buffer */
    VALG_CRCL_BUF_S stCycBuf;
    HI_VOID *       pStrmBufLock; /* lock stream buffer */

    /* header parameter */
    HI_U8  SpsStream[16];
    HI_U8  PpsStream[12];
    HI_U32 SpsBits;
    HI_U32 PpsBits;
    HI_U32 SlcHdrStream [4];
    HI_U32 ReorderStream[2];
    HI_U32 MarkingStream[2];
    HI_U32 SlcHdrBits;         /* 8bit_0 | mark | reorder | slchdr */
    HI_U32 H263SrcFmt;         /* 1 ~ 6 */
    HI_U32 H264FrmNum;

    /* other parameter */
    HI_U32 *pRegBase;
    HI_U32  WaitingIsr;         /* to block STOP channel */
    HI_U32  PTS0;
    HI_U32  PTS1;
    HI_BOOL bNeverEnc;
    
    /* read back reg */
    HI_U32 PicBits;
    HI_U32 TotalMbhBits;
    HI_U32 TotalTxtBits;
    HI_U32 TotalTxtCost0;
    HI_U32 TotalTxtCost1;
    HI_U32 MeanQP;
    HI_U32 VencEndOfPic;
    HI_U32 VencBufFull;
    HI_U32 VencPbitOverflow;
    HI_U32  Timer;
	HI_U32  IdleTimer;
#ifdef __VENC_S40V200_CONFIG__
    HI_U32  CrefldReqNum;
    HI_U32  CrefldDatNum;
#endif
	HI_U32  CfgErrIndex;
	HI_U32  IntraCloseCnt;
	HI_U32  FracCloseCnt;
	HI_U32  IntpSearchCnt;
    /* sequence rate control in */
    HI_U32 Gop;
    HI_U32 BitRate;
    HI_U32 ViFrmRate;
    HI_U32 VoFrmRate;
    HI_U32 VBRflag;    /* fixed now...todo */
    HI_U32 PicLevel;
    HI_U32  SkipFrameEn;                                                                      //

    /* frame rate control out */
    HI_U32 IntraPic;
    HI_U32 TargetBits;         /* targetBits of each frame */
    HI_U32 StartQp;
    HI_U32 MinQp;
    HI_U32 MaxQp;

    /* rate control mid */
    HI_U32      RcStart;
    HI_U32      BitsFifo[60];
    HI_U32      MeanBit;
    HI_U32      TrCount;
    HI_U32      LastTR;
    HI_U32      InterFrmCnt;
    VALG_FIFO_S stBitsFifo;
    HI_S32  bMorePPS;                                                                     
    HI_S32  ChrQpOffset;
    HI_S32  ConstIntra;
    HI_S32  CabacInitIdc;
    HI_S32  CabacStuffEn;
    
    HI_S32  DblkIdc;
    HI_S32  DblkAlpha;
    HI_S32  DblkBeta;
    
    HI_S32  IPCMPredEn;
    HI_S32  Intra4x4PredEn;
    HI_S32  Intra8x8PredEn;
    HI_S32  Intra16x16PredEn;
    HI_S32  I4ReducedModeEn;
    
    HI_S32  Inter8x8PredEn;
    HI_S32  Inter8x16PredEn;
    HI_S32  Inter16x8PredEn;
    HI_S32  Inter16x16PredEn;
    HI_S32  PskipEn;
    HI_S32  ExtedgeEn;
    HI_S32  TransMode;
    HI_S32  NumRefIndex;

	HI_S32  PixClipEn;
	HI_S32  LumaClipMin;
	HI_S32  LumaClipMax;
	HI_S32  ChrClipMin;
	HI_S32  ChrClipMax;
	
    
    HI_S32  HSWSize;
    HI_S32  VSWSize;
    HI_S32  RectMod   [6];
    HI_S32  RectWidth [6];
    HI_S32  RectHeight[6];
    HI_S32  RectHstep [6];
    HI_S32  RectVstep [6];
    HI_S32  StartThr1;
    HI_S32  StartThr2;
    HI_S32  IntraThr;
    HI_S32  LmdaOff16;
    HI_S32  LmdaOff1608;
    HI_S32  LmdaOff0816;
    HI_S32  LmdaOff8;
    HI_S32  CrefldBur8En;
    HI_S32  MdDelta;
	
#ifdef __VENC_S40V200_CONFIG__  
    HI_S32  MctfStrength0; /* NA */
    HI_S32  MctfStrength1; /* NA */

    HI_S32  MctfStillEn;
    HI_S32  MctfMovEn;
    HI_S32  mctfStillMvThr;
    HI_S32  mctfRealMvThr;
    HI_S32  MctfStillCostThr;
    HI_S32  MctfLog2mctf;
    HI_S32  MctfLumaDiffThr;
    HI_S32  MctfChrDiffThr;
    HI_S32  MctfChrDeltaThr;
    HI_S32  MctfStiMadiThr1;
    HI_S32  MctfStiMadiThr2;
    HI_S32  MctfMovMadiThr;
    HI_S32  MctfMovMad1_m;
    HI_S32  MctfMovMad1_n;
    HI_S32  MctfMovMad2_m;
    HI_S32  MctfMovMad2_n;
    HI_S32  MctfStiMad1_m;
    HI_S32  MctfStiMad1_n;
    HI_S32  MctfStiMad2_m;
    HI_S32  MctfStiMad2_n;
	
#elif defined(__VENC_3716CV200_CONFIG__)

    HI_S32 MctfStillEn            ;
    HI_S32 MctfSmlmovEn           ;
    HI_S32 MctfBigmovEn           ;
    HI_S32 mctfStillMvThr         ;
    HI_S32 mctfRealMvThr          ;
    HI_S32 MctfStillCostThr       ;
    HI_S32 MctfStiLumaOrialpha    ;
    HI_S32 MctfSmlmovLumaOrialpha ;
    HI_S32 MctfBigmovLumaOrialpha ;
    HI_S32 MctfChrOrialpha        ;
    HI_S32 mctfStiLumaDiffThr0    ;
    HI_S32 mctfStiLumaDiffThr1    ;
    HI_S32 mctfStiLumaDiffThr2    ;
    HI_S32 mctfSmlmovLumaDiffThr0 ;
    HI_S32 mctfSmlmovLumaDiffThr1 ;
    HI_S32 mctfSmlmovLumaDiffThr2 ;
    HI_S32 mctfBigmovLumaDiffThr0 ;
    HI_S32 mctfBigmovLumaDiffThr1 ;
    HI_S32 mctfBigmovLumaDiffThr2 ;
    HI_S32 mctfStiLumaDiffK0      ;
    HI_S32 mctfStiLumaDiffK1      ;
    HI_S32 mctfStiLumaDiffK2      ;
    HI_S32 mctfSmlmovLumaDiffK0   ;
    HI_S32 mctfSmlmovLumaDiffK1   ;
    HI_S32 mctfSmlmovLumaDiffK2   ;
    HI_S32 mctfBigmovLumaDiffK0   ;
    HI_S32 mctfBigmovLumaDiffK1   ;
    HI_S32 mctfBigmovLumaDiffK2   ;
    HI_S32 mctfChrDiffThr0        ;
    HI_S32 mctfChrDiffThr1        ;
    HI_S32 mctfChrDiffThr2        ;
    HI_S32 mctfChrDiffK0          ;
    HI_S32 mctfChrDiffK1          ;
    HI_S32 mctfChrDiffK2          ;
    HI_S32 mctfOriRatio           ;
    HI_S32 mctfStiMaxRatio        ;
    HI_S32 mctfSmlmovMaxRatio     ;
    HI_S32 mctfBigmovMaxRatio     ;
    HI_S32 mctfStiMadiThr0        ;
    HI_S32 mctfStiMadiThr1        ;
    HI_S32 mctfStiMadiThr2        ;
    HI_S32 mctfSmlmovMadiThr0     ;
    HI_S32 mctfSmlmovMadiThr1     ;
    HI_S32 mctfSmlmovMadiThr2     ;
    HI_S32 mctfBigmovMadiThr0     ;
    HI_S32 mctfBigmovMadiThr1     ;
    HI_S32 mctfBigmovMadiThr2     ;
    HI_S32 mctfStiMadiK0          ;
    HI_S32 mctfStiMadiK1          ;
    HI_S32 mctfStiMadiK2          ;
    HI_S32 mctfSmlmovMadiK0       ;
    HI_S32 mctfSmlmovMadiK1       ;
    HI_S32 mctfSmlmovMadiK2       ;
    HI_S32 mctfBigmovMadiK0       ;
    HI_S32 mctfBigmovMadiK1       ;
    HI_S32 mctfBigmovMadiK2       ;  
#endif

    HI_S32  StartQpType;
    HI_S32  RcQpDelta;
    HI_S32  RcMadpDelta;
    HI_S32  RcQpDeltaThr[12];
    HI_S32  ModLambda[40];
    HI_S32  Scale8x8[128];

	HI_S32  IntraLowpowEn;   /* register of low power work mode */ 
	HI_S32  fracLowpowEn;
	HI_S32  intpLowpowEn;
    HI_S32  fracRealMvThr;

	HI_S32  LowDlyMod;
    HI_U32  tunlReadIntvl;
    HI_U32  tunlcellAddr;
    HI_S32  RegLockEn;
    HI_S32  ClkGateEn;
    HI_S32  MemClkGateEn;
    HI_S32  TimeOutEn;
    HI_S32  TimeOut;
    HI_S32  PtBitsEn;
    HI_S32  PtBits;
    HI_S32  OutStdNum;
    
    HI_S32  IMbNum;
    HI_S32  StartMb;
    VeduEfl_RoiAttr_S RoiCfg;
    VeduEfl_OsdAttr_S OsdCfg;

    /* attach vi or vo ,backup for vpss*/
    VeduEfl_SrcInfo_S     stSrcInfo;
    VeduEfl_SrcInfo_S     stSrcInfo_toVPSS;
    HI_DRV_VIDEO_FRAME_S  stImage;

    /* statistic */
    VeduEfl_StatInfo_S stStat;

    VALG_CRCL_QUE_BUF_S stCycQueBuf;      //add

    
} VeduEfl_EncPara_S;

typedef struct
{
    HI_U32 BusViY;      /* 16-byte aligned  */
    HI_U32 BusViC;      /* 16-byte aligned  */
    HI_U32 BusViV;
    HI_U32 ViYStride;   /* 16-byte aligned  */
    HI_U32 ViCStride;   /* 16-byte aligned  */

    HI_U32 PTS0;
    HI_U32 PTS1;
  //ADD  
    HI_U32   RStrideY;      /* 16-byte aligned  */
    HI_U32   RStrideC;      /* 16-byte aligned  */
    HI_U32   TunlCellAddr;
} VeduEfl_EncIn_S;

typedef struct
{
    HI_U32 EncHandle;
} VeduEfl_ChnCtx_S;

typedef struct
{
    HI_U32   IpFree;       /* for channel control */
    HI_U32   CurrHandle;   /* used in ISR */
    HI_U32  *pRegBase;
    HI_VOID *pChnLock;     /* lock ChnCtx[MAX_CHN] */
    HI_VOID *pTask;
    HI_U32   StopTask;
    HI_U32   TaskRunning;  /* to block Close IP */
} VeduEfl_IpCtx_S;

HI_S32	VENC_DRV_EflOpenVedu( HI_VOID );
HI_S32	VENC_DRV_EflCloseVedu( HI_VOID );
HI_S32	VENC_DRV_EflCreateVenc( HI_U32 *pEncHandle, VeduEfl_EncCfg_S *pEncCfg );
HI_S32	VENC_DRV_EflDestroyVenc( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflAttachInput( HI_U32 EncHandle, VeduEfl_SrcInfo_S *pSrcInfo );
HI_S32	VENC_DRV_EflDetachInput( HI_U32 EncHandle, VeduEfl_SrcInfo_S *pSrcInfo );
HI_S32	VENC_DRV_EflStartVenc( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflStopVenc( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflRcGetAttr( HI_U32 EncHandle, VeduEfl_RcAttr_S *pRcAttr );
HI_S32	VENC_DRV_EflRcSetAttr( HI_U32 EncHandle, VeduEfl_RcAttr_S *pRcAttr );
HI_S32	VENC_DRV_EflRcFrmRateCtrl( HI_U32 EncHandle, HI_U32 TR );
HI_S32	VENC_DRV_EflRcOpenOneFrm( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflRcCloseOneFrm( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflStartOneFrameVenc( HI_U32 EncHandle, VeduEfl_EncIn_S *pEncIn );
HI_S32	VENC_DRV_EflEndOneFrameVenc( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflRequestIframe( HI_U32 EncHandle );
HI_S32	VENC_DRV_EflSetResolution( HI_U32 EncHandle, HI_U32 FrameWidth, HI_U32 FrameHeight );
HI_S32	VENC_DRV_EflResumeVedu(HI_VOID);
HI_S32	VENC_DRV_EflGetBitStream( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu );
HI_S32	VENC_DRV_EflSkpBitStream( HI_U32 EncHandle, VeduEfl_NALU_S *pNalu );
HI_S32	VENC_DRV_EflQueryStatInfo( HI_U32 EncHandle, VeduEfl_StatInfo_S *pStatInfo );
HI_S32	VENC_DRV_EflEncodeFrame(HI_VOID);
HI_U32	VENC_DRV_EflGetStreamLen( HI_U32 EncHandle );
HI_S32  VENC_DRV_EflQueueFrame( HI_U32 EncHandle, HI_DRV_VIDEO_FRAME_S  *pstFrame);
HI_S32  VENC_DRV_EflDequeueFrame( HI_U32 EncHandle, HI_DRV_VIDEO_FRAME_S  *pstFrame);
HI_S32  VENC_DRV_EflGetImage(HI_S32 EncHandle, HI_DRV_VIDEO_FRAME_S  *pstFrame);
HI_S32  VENC_DRV_EflPutImage(HI_S32 EncHandle, HI_DRV_VIDEO_FRAME_S  *pstFrame);

HI_S32  VENC_DRV_EflCfgRegVenc( HI_U32 EncHandle );
HI_VOID VENC_DRV_EflSortPriority(HI_VOID);

HI_BOOL VENC_DRV_EflJudgeVPSS( HI_HANDLE EncHandle, HI_DRV_VIDEO_FRAME_S *pstFrameInfo ,HI_BOOL bActiveMode);   //调用在获得帧信息后
HI_VOID VENC_DRV_EflWakeUpThread(HI_VOID);
/*************************************************************************************/
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__DRV_VENC_EFL_H__
