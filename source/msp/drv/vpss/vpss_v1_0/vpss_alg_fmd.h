#ifndef __VPSS_ALG_FMD_H__
#define __VPSS_ALG_FMD_H__

#include"vpss_common.h"
typedef enum
{
    ALG_DEI_MODE_5FLD = 0,
    ALG_DEI_MODE_4FLD,
    ALG_DEI_MODE_3FLD,
    ALG_DEI_MODE_BUTT
}ALG_DEI_MODE_E;



typedef enum
{
    FMD_FALSE = 0,
    FMD_TRUE
} FMD_BOOL_EN;


typedef enum
{
    PB_VIDEO = 0,
    PB_32PD,
    PB_22PD
} PB_STATE_EN;

typedef struct
{
    HI_S32 IsProgressiveSeq;
    HI_S32 IsProgressiveFrm;
    HI_S32 RealFrmRate;
}ALG_VDEC_INFO_S;

typedef struct
{
    HI_BOOL   Pd32Det;    
    HI_BOOL   WithOvlpTitle;
    HI_S8      PdState;
    HI_S8      RptFlag;
    HI_S8      PdSubState; /*10 sub state to set pd para. meaningless when pd32det is false*/
} PD32_INFO_S;

typedef struct
{
    HI_S32 Count;
} PD32_PHASE_INFO_S;

/*------------------------structs' definitions---------------------------*/
typedef struct
{
    HI_S32 histFrmITDiff[5]; /* save five successive history FrmITDiff values for calculating mean value. */
    HI_S32 lstFrmITDiffMean; /* mean value of last five history FrmITDiff values. */
    HI_S32 curFrmITDiffMean; /* mean value of current five history FrmITDiff values. */
    HI_S32 meanFrmITDiffAcc; /* Accumulation value of history FrmITDiff mean diffrence. */
    HI_S32 lstRepfMotion;    /* Repeate field motion of last time. */
    HI_S32 anomalCnt;        /* abnormal situation counter of progressive state. */

    HI_S32 lstPcc_Crss;	 /* save the current PCC_CRSS for next use,add by b00182026 for 4 field IP detect at 2011-11-16 */
    HI_S32 lstnonmatch_weave;	 /* save the current PCC_CRSS for next use,add by b00182026 for 4 field IP detect at 2011-11-16 */

    HI_S32 field_match_weave;
    HI_S32 field_nonmatch_weave;
    HI_S32 field_pcc_crss;
    HI_S32 field_match_tkr;
    HI_S32 field_nonmatch_tkr;

    HI_S32 frame_match_weave;
    HI_S32 frame_nonmatch_weave;
    HI_S32 frame_pcc_crss;
    HI_S32 frame_match_tkr;
    HI_S32 frame_nonmatch_tkr;
    HI_S8 phaseCnt[2];/* 22pulldown phase counter */
    HI_BOOL pld22Lock;  /* lock state:0-unlock; 1-lock */
} PLD22_CNTXT_S;

/* still blocks? */
typedef struct
{
    HI_S32 STILLBLK_THR;    /* 8bit [0:255] */
} STILLBLK_THD_S;     /* still block */


/* pulldown detect input */
typedef struct
{
    HI_S8 THR[4]; /* 8bit (0:255] */
} REG_HISTOGRAM_INPUT_S;

typedef struct
{
    HI_S8 THR[3];/* 8bit (0:255] */
} REG_UM_INPUT_S; /* unexpected motion */

typedef struct
{
    HI_S8 CORING_NORM;  /* 8bit [0:255] */
    HI_S8 CORING_TKR;   /* 8bit [0:255] */
    HI_S8 CORING_BLK;   /* 8bit [0:255] */
    HI_S8 H_THR;        /* 8bit (0:255] */
    HI_S8 V_THR[4];     /* 8bit (0:255] */
    HI_S8 MOV_CORING_NORM;  /* 8bit (0:255] */
    HI_S8 MOV_CORING_TKR;  /* 8bit (0:255] */
    HI_S8 MOV_CORING_BLK;  /* 8bit (0:255] */
} REG_PCC_INPUT_S;       /* polarity change counter */

typedef struct
{
    HI_S8 THR[4];    /* 8bit (0:255] */
} REG_ITDIFF_INPUT_S; /*  inverse telecine difference */

typedef struct
{
    HI_S8 THR;       /* 8bit (0:255] */
    HI_S8 EDGE_THR;  /* 8bit (0:255] */
    HI_S8 LASI_MOV_THR; /*8bit (0:255]  the threshold for new method(median method) for Lasi */
} REG_LASI_INPUT_S;

typedef struct
{
    /*useless*/
    HI_S16 ROW0; /*  16bit (0:pic width) */
    HI_S16 ROW1; /*  16bit (ROW0:pic width) */
    HI_S16 COL0; /*  16bit (0:pic_height) */
    HI_S16 COL1; /*  16bit (COL0:pic_height) */
} REG_BLOCKINFO_INPUT_S;

typedef struct
{
    HI_S32 BITSMOV2R; /*8bit (0:255] */
} REG_SCENCHG_INPUT_S;



/* pulldown detect output */
typedef struct
{
    HI_S32 lasiCnt14;
    HI_S32 lasiCnt32;
    HI_S32 lasiCnt34;
} REG_LASI_STAT_S;

typedef struct
{
    HI_S32 PCC_FFWD;
    HI_S32 PCC_FWD;
    HI_S32 PCC_BWD;
    HI_S32 PCC_CRSS;
    HI_S32 pixel_weave;
    HI_S32 PCC_FWD_TKR;
    HI_S32 PCC_BWD_TKR;
    HI_S32 PCCBLK_FWD[9];
    HI_S32 PCCBLK_BWD[9];
} REG_PCC_STAT_S;

typedef struct
{
    HI_S32 HISTOGRAM_BIN_1;
    HI_S32 HISTOGRAM_BIN_2;
    HI_S32 HISTOGRAM_BIN_3;
    HI_S32 HISTOGRAM_BIN_4;
} REG_HISTOGRAM_STAT_S;

typedef struct
{
    HI_S32 match_UM;
    HI_S32 nonmatch_UM;
    HI_S32 match_UM2;
    HI_S32 nonmatch_UM2;
} REG_UM_STAT_S;

typedef struct
{
    HI_S32 StillBlkCnt;
    HI_S32 BlkSad[16];
} REG_STILLBLK_STAT_S;

typedef struct
{
    HI_S32 iCHD;
}REG_SCENCHG_STAT_S;

typedef struct
{
    HI_S8 EdgeSmoothRatio;    
    HI_S32 DIFF_MOVBLK_THR; /* 11bit [0:2047] */
    REG_HISTOGRAM_INPUT_S HistogramCtrl;
    REG_UM_INPUT_S        UmCtrl;
    REG_PCC_INPUT_S       PccCtrl;
    REG_ITDIFF_INPUT_S    ItdiffCtrl;
    REG_LASI_INPUT_S      LasiCtrl;
//    REG_BLOCKINFO_INPUT_S BlockInfo; 
    REG_SCENCHG_INPUT_S   ScenChgCtrl;
} ALG_FMD_RTL_INITPARA_S;

typedef struct
{
    HI_S32               frmITDiff;
    REG_UM_STAT_S        frmUm;
    REG_PCC_STAT_S       frmPcc;
    REG_HISTOGRAM_STAT_S frmHstBin;
    REG_LASI_STAT_S      lasiStat;
    REG_STILLBLK_STAT_S  StillBlkInfo;
    REG_SCENCHG_STAT_S	 SceneChangeInfo;
}ALG_FMD_RTL_STATPARA_S;

/* submission information struct for hardware (exterior interface) */
typedef struct
{
    HI_BOOL  DirMch;
    HI_BOOL  DieOutSelLum;
    HI_BOOL  DieOutSelChr;
    HI_BOOL  EdgeSmoothEn; /*����2:2��Ӱģʽ�ұ�Եƽ��ʹ�������Ϊ1*/  
    
    HI_BOOL  SceneChange;/* �����л���Ϣ */
    HI_S32 s32FieldOrder;/* ���س��� */
    HI_S32 s32FilmType;/* ��Ӱģʽ */
}ALG_FMD_RTL_OUTPARA_S;

typedef struct
{
    HI_S32 lst_repeat;
    HI_S32 cur_repeat;
    HI_S32 nxt_repeat;
}REPEAT_S;

typedef struct
{
    HI_S32 lst_drop;
    HI_S32 cur_drop;
    HI_S32 nxt_drop;
}DROP_S;

typedef struct  
{
    HI_U8 g_lstreffld;    /* reference field for the frame befor the current frame */
    HI_U8 g_curreffld;    /* reference field for the current frame */
    HI_U8 g_nxtreffld;    /* reference field for the coming frame */
}REF_FLD_S;


typedef struct
{
    HI_U8 g_lstbtmode;
    HI_U8 g_curbtmode;
    HI_U8 g_nxtbtmode;	
}BTMODE_S;


/*structure of pulldown software result */
typedef struct
{
    HI_BOOL Is2ndFld;             /*��ʾ����ͳ����Ϣʱ���ο����Ƿ�Ϊ�ڶ���*/
    HI_BOOL NxtIs2ndFld;          /*��ʾ������һ�����ø��߼��Ĳο����Ƿ�Ϊ�ڶ���*/
    HI_BOOL BtMode;               /*��ʾ����ͳ����Ϣʱ�����ø��߼��ĳ���*/
    HI_BOOL RefFld;               /*��ʾ����ͳ����Ϣʱ�����ø��߼��Ĳο���,�㷨��Ա�����Ĳο���Ҳ���߼���Ա�ĵ�ǰ��*/
    HI_S32 s32PbDelay;            /*delayed fields*/
    HI_S32 SadBuf[16], SadCnt, *pSadRd, *pSadWt;
    PD32_INFO_S Pld32InfoBuf[5], PdInfo;
    PD32_PHASE_INFO_S Phases32[5];
    HI_S8 Last32Phase;
    HI_S32 SADDiffAcc;
    HI_S32 FieldOrder;
    HI_S8 SceneChange[2];
    HI_S32 ScSadBuf[6];
    HI_BOOL Pld22LockState[3]; /* lock state of last 3 fields */
    PLD22_CNTXT_S Pld22Ctx;
    HI_U16 sadReg[16][3];
    HI_S8 MainState;
    HI_S8 PrevMainState;
    HI_S8 init_symbol;

    REPEAT_S stRepeatHist;
    DROP_S stDropHist;
} ALG_FMD_CTX_S;


typedef struct
{  
    HI_BOOL bDeiEnLum;
    HI_BOOL bDeiEnChr;
    HI_S32  s32DeiMdLum;   /*0-5 field; 1-4 filed; 2-3 field; 3-reserved*/
    HI_S32  s32DeiMdChr;
    HI_BOOL bDeiRst;       /*0-reset invalid; 1-reset valid,don't read history motion infomation;*/
    HI_BOOL FodEnable;         /* field order detect enable on-off: 1-enable; 0-disable*/
    HI_BOOL Pld22Enable;      /* Interleaved/Progressive detect enable on-off: 1-enable; 0-disable*/
    HI_BOOL Pld32Enable;      /* pulldown detect enable on-off: 1-enable; 0-disable*/    
    HI_BOOL EdgeSmoothEn;  /* edge smooth enable on-off: 1-disable; 0-enable*/

    HI_S32 s32FrmHeight;          /* height of source*/
    HI_S32 s32FrmWidth;           /* width of source*/    
    //HI_S32 s32PbDelay;            /*delayed fields*/
    HI_BOOL bOfflineMode;            /*system mode: 1-offline mode; 0-online mode*/

    /*the following may be different for different frames*/
    HI_S32 s32Repeat;    /*if s32Repeat>0, it indicates this frame is repeated. */
    HI_S32 s32Drop;       /*if s32Repeat>0, it indicates this frame is dropped. */
    HI_BOOL  BtMode;        /*  Ϊ��ͳ����Ϣʱ����һ���ĳ��� 0 topFirst*/    
    HI_BOOL  RefFld;       /* ref field  Ϊ��ͳ����Ϣʱ����һ�������� ��0 ��ǰ��Ϊ�׳� ��1*/
    HI_BOOL  bPreInfo;    /*DEI�߼�����timeout���Ծɴ�����һ��*/ 
        
    ALG_VDEC_INFO_S stVdecInfo;
    ALG_FMD_RTL_STATPARA_S stFmdRtlStatPara; /*��Ҫ���߼�����ͳ����Ϣ*/
}ALG_DEI_DRV_PARA_S;


typedef struct
{
     HI_U32  u32InitFlag;
     REF_FLD_S stRefFld;    
     BTMODE_S stBtMode;    
     ALG_FMD_CTX_S stFmdCtx;          
     STILLBLK_THD_S  StillBlkCtrl;
     ALG_FMD_RTL_OUTPARA_S stRtlOutParaBak;/*��DEI�߼�Timeoutʱ��ʹ����һ�εļ�����*/
}ALG_FMD_SOFTINFO_S;

HI_S32 ALG_FmdInit(ALG_FMD_SOFTINFO_S *pstFmdSoftInfo);

HI_VOID FmdThdParaInit(ALG_FMD_RTL_INITPARA_S *pstFmdRtlInPara);

HI_S32 ALG_FmdDeInit(ALG_FMD_SOFTINFO_S *pstFmdSoftInfo);

HI_S32 ALG_FmdReset(ALG_FMD_SOFTINFO_S *pstFmdSoftInfo,ALG_DEI_DRV_PARA_S* pstDeiDrvPara);

HI_S32 ALG_FmdSet(ALG_FMD_SOFTINFO_S *pstFmdSoftInfo,ALG_DEI_DRV_PARA_S* pstDeiDrvPara,ALG_FMD_RTL_OUTPARA_S*pstFmdRtlOutPara);

#endif
