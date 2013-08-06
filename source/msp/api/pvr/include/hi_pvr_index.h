/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_index.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   :
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_INDEX_H__
#define __HI_PVR_INDEX_H__

#include "hi_pvr_priv.h"
#include "hi_pvr_fifo.h"

#include "pvr_index.h"
#include "hi_unf_demux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PVR_TPLAY_MIN_DISTANCE       25  /* the min interval of trickmode for sending frame, the max value, decode 40 frame per second with 1920*1080 definition */

#define PVR_TPLAY_MIN_FRAME_RATE	 40	 /* ms, per frame */

#define PVR_TPLAY_FRAME_SHOW_TIME    40UL  /* ms, no need to play too fast at TrickMode  */ 

#define PVR_INDEX_REC                0
#define PVR_INDEX_PLAY               1


#if 0
#define PVR_INDEX_LOCK(p_mutex)        HI_INFO_PVR("==>\n");(void)pthread_mutex_lock(p_mutex);HI_INFO_PVR("==|\n")
#define PVR_INDEX_UNLOCK(p_mutex)      HI_INFO_PVR("<==\n");(void)pthread_mutex_unlock(p_mutex);HI_INFO_PVR("==|\n")
#else
#define PVR_INDEX_LOCK(p_mutex)       (void)pthread_mutex_lock(p_mutex)
#define PVR_INDEX_UNLOCK(p_mutex)     (void)pthread_mutex_unlock(p_mutex)
#endif


/* frame type definition                                                    */
    /*
    001	intra-coded (I)
    010	predictive-coded (P)
    011	bidirectionally-predictive-coded (B)
    100	shall not be used
    (dc intra-coded (D) in ISO/IEC11172-2)
    */
#define PVR_INDEX_FRAME_I            0x01
#define PVR_INDEX_FRAME_P            0x02
#define PVR_INDEX_FRAME_B            0x03

/* start code type definition(data from SCD buffer) */
#define PVR_INDEX_SC_TYPE_TS         0x1      /* ts packet header */
#define PVR_INDEX_SC_TYPE_PTS        0x2      /* pes packet header */
#define PVR_INDEX_SC_TYPE_PAUSE      0x3      /* pause flag */
#define PVR_INDEX_SC_TYPE_PIC        0x4      /* the start 00 00 01 of frame data */
#define PVR_INDEX_SC_TYPE_PIC_SHORT  0x5      /* the short head 00 01 of frame data */
#define PVR_INDEX_SC_TYPE_PES_ERR    0xf      /* the header of PES syntax error */


#define PVR_INDEX_HEADER_CODE        0x5A5A5A5A
#define PVR_DFT_RESERVED_REC_SIZE    (1024*1024)

#define PVR_MIN_CYC_SIZE (50 * 1024 * 1024LLU)
#define PVR_MIN_CYC_TIMEMS (60 *1000)

#define PVR_MIN_CYC_DIFF (4LLU * 1024LLU * 1024LLU)

#define PVR_INDEX_PAUSE_INVALID_OFFSET      ((HI_U32)(-1))
#define PVR_INDEX_STEPBACK_INVALID_OFFSET   ((HI_U32)(-1))
#define PVR_INDEX_INVALID_PTSMS             ((HI_U32)(-1))
#define PVR_INDEX_INVALID_SEQHEAD_OFFSET    ((HI_U64)(-1))
#define PVR_INDEX_INVALID_I_FRAME_OFFSET    (0x3fffU)
#define PVR_INDEX_PAUSE_SEQHEAD_OFFSET      ((HI_U64)(-2))

/* rewind record or not */
#define PVR_INDEX_IS_REWIND(handle)         ((handle)->stCycMgr.bIsRewind)

/* record or not */
#define PVR_INDEX_IS_REC(handle)         ((handle)->stCycMgr.bIsRec)

/* play or not */
#define PVR_INDEX_IS_PLAY(handle)         ((handle)->stCycMgr.bIsPlay)

/*whether index type is audio or not */
#define PVR_INDEX_IS_TYPE_AUDIO(handle)     ((handle)->enIndexType == HI_UNF_PVR_REC_INDEX_TYPE_AUDIO)

/* the max size of ts file */
#define PVR_INDEX_MAX_FILE_SIZE(handle)     ((handle)->stCycMgr.u64MaxCycSize)



/* get frame type macro                                                      */
#define PVR_INDEX_get_frameType(pEntry) \
(((pEntry)->u16FrameTypeAndGop >> 14) & 0x3UL)

/* get offset from previous I frame macro                                   */
#define PVR_INDEX_get_preIoffset(pEntry) \
((pEntry)->u16FrameTypeAndGop & 0x3fffUL)


/* judgement of frame type                                                  */
#define PVR_INDEX_is_Iframe(pEntry) \
((((pEntry)->u16FrameTypeAndGop >> 14) & 0x3UL) == PVR_INDEX_FRAME_I)

#define PVR_INDEX_is_Bframe(pEntry) \
((((pEntry)->u16FrameTypeAndGop >> 14) & 0x3UL) == PVR_INDEX_FRAME_B)

#define PVR_INDEX_is_Pframe(pEntry) \
((((pEntry)->u16FrameTypeAndGop >> 14) & 0x3UL) == PVR_INDEX_FRAME_P)

#define PVR_INDEX_OverFlow(pEntry)\
((((pEntry)->u16UpFlowFlag) & 0x1UL) == 0x1UL)

#define PVR_INDEX_Masked(pEntry) \
((((pEntry)->u16UpFlowFlag >> 1) & 0x1UL) == 0x1UL)


#define PVR_IS_SPEED_SEND_ALL(speed) \
    (HI_UNF_PVR_PLAY_SPEED_NORMAL == (speed) \
     || HI_UNF_PVR_PLAY_SPEED_2X_FAST_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_4X_FAST_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_2X_SLOW_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_4X_SLOW_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_8X_SLOW_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_16X_SLOW_FORWARD == (speed)\
     || HI_UNF_PVR_PLAY_SPEED_32X_SLOW_FORWARD == (speed))

/* pvr index user list                                                      */
typedef enum hiPVR_INDEX_USER_E
{
    PVR_INDEX_USER_FREE = 0x00,                   /* no one use it */
    PVR_INDEX_USER_REC  = 0x01,                   /* used by record */
    PVR_INDEX_USER_PLAY = 0x02,                   /* used by play */
    PVR_INDEX_USER_BOTH = 0x03,                   /* used by record and play meantime */
    PVR_INDEX_USER_BUTT
} PVR_INDEX_USER_E;

/* rewind record control info */
typedef struct hiPVR_CYC_MGR_S
{
    HI_BOOL bIsRewind;          /* rewind record or not */
    HI_U32  u32StartFrame;      /* the first valid frame number in index on cycle playing */
    HI_U32  u32EndFrame;        /* the last valid frame number in index on cycle playing */
    HI_U32  u32LastFrame;       /* the last number of frame cycle end */

    HI_S32  s32CycTimes;        /* the times for cycle record */
    HI_U32  u32Reserve;         /* u64 aligned */
    HI_U64  u64MaxCycSize;      /* max file size of cycle record */
    HI_U64  u64MaxCycTimeInMs;  /* max time length of cycle record */
}PVR_CYC_MGR_S;


/* pvr index handle descriptor                                              */
typedef struct hiPVR_INDEXER_S
{
    HI_BOOL              bIsRec;                 /* record or not */
    HI_BOOL              bIsPlay;                /* play or not */

    PVR_FILE             s32ReadFd;              /* read descriptor for index file */
    PVR_FILE             s32SeekFd;              /* seek descriptor for index file */
    PVR_FILE             s32WriteFd;             /* write descriptor for index file */
    PVR_FILE             s32HeaderFd;            /* write index header for index file */

    HI_UNF_PVR_REC_INDEX_TYPE_E enIndexType;     /* the type of index, in common about record and play. assigned init index */
    PVR_CYC_MGR_S               stCycMgr;        /* control rewind record, and save the frame position, regardless of rewind record */

    HI_U64               u64GlobalOffset;        /* last write frame offset, the total data size from start record to current play or record, included the rewind data */
    HI_U64               u64FileSizeGlobal;      /* the actual file saved size, for debug only  */
    
    HI_U32               u32IdxStartOffsetLen;   /* the length of file header, included header info and user info. in common between record and play*/

    HI_U32               u32LastDavBufOffset;    /* last DAV buffer offset, on recording, save the offset of dav buffer */
    HI_U32               u32DavBufSize;          /* demux dav buffer size, on recording, save the size of dav buffer */

    HI_U32               u32PauseFrame;          /* mark a pause flag for recording file, included pause the record file on live and pause the playing timeshif */
    HI_U64               u64PauseOffset;         /* the offset from the record start to pause flag. used for checking the pause position rewriten or not by rewind record. */

    HI_U16               u16RecLastIframe;       /* on recording, save the previous I frame position */
    HI_U16               u16RecUpFlowFlag;       /* on recording, dav up flow flag */
    HI_U32               u32RecLastValidPtsMs;   /* on recording, save the previous valid PTS */
    HI_U32               u32RecPicParser;        /* on recording, FIDX ID */
    HI_U32               u32RecFirstFrmTimeMs;   /* on recording, save the system time at the first frame incoming */

    HI_U32               u32WriteFrame;          /* the write pointer, frame number of index file on recording */
    HI_U32               u32ReadFrame;           /* the read porinter, frame number of index file on playing */
    PVR_INDEX_ENTRY_S    stCurRecFrame;          /* the current frame info of recording */
    PVR_INDEX_ENTRY_S    stCurPlayFrame;         /* the current frame info of outputing */
    HI_BOOL 			 bIsFristIframe;		 /* flag the first I frame or not on ff and rw trick mode */
    HI_U32 				 u32FrameDistance;		 /* the frame number of trick mode between I frame */   
    HI_U32               u32RecReachPlay;        /* record catchs up the play or not, catched and reset it */

	HI_U32 				 u32FflushTime;			 /* fresh time pointer flag */
    HI_U32               u32DmxClkTimeMs;

    HI_UNF_PVR_FILE_ATTR_S    stIndexFileAttr;   /* for pure play, the file attribute of the exist index file, and just only assigned on creating play channel */

    HI_CHAR              szIdxFileName[PVR_MAX_FILENAME_LEN+4];
    pthread_mutex_t      stMutex;
}PVR_INDEX_S, *PVR_INDEX_HANDLE;


HI_U32 PVRIndexGetCurTimeMs(HI_VOID);



/* init index module, and create index handle, and destroy   */
HI_S32 PVR_Index_Init(HI_VOID);
PVR_INDEX_HANDLE PVR_Index_CreatPlay(HI_U32 chnID,
                                const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr,
                                HI_BOOL *pIsNoIdx);
PVR_INDEX_HANDLE PVR_Index_CreatRec(HI_U32 chnID,
                                HI_UNF_PVR_REC_ATTR_S *pstRecAttr);

HI_S32 PVR_Index_Destroy(PVR_INDEX_HANDLE handle, HI_U32 u32PlayOrRec);

/* attr opration */
HI_VOID PVR_Index_ResetRecAttr(PVR_INDEX_HANDLE handle);
HI_VOID PVR_Index_ResetPlayAttr(PVR_INDEX_HANDLE handle);
HI_S32 PVR_Index_ChangePlayMode(PVR_INDEX_HANDLE handle);

/***** save frame *****/
HI_S32 PVR_Index_SaveFramePosition(HI_U32 InstIdx, FRAME_POS_S *pstScInfo);


/* get frame opration    */
HI_S32 PVR_Index_GetNextFrame(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_S *pstFrame);
HI_S32 PVR_Index_GetNextIFrame(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_S *pstFrame);
HI_S32 PVR_Index_GetPreIFrame(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_S *pstFrame);
HI_S32 PVR_Index_GetCurrentFrame(const PVR_INDEX_HANDLE handle,  PVR_INDEX_ENTRY_S *pEntry);
HI_S32 PVR_Index_QueryFrameByPTS(const PVR_INDEX_HANDLE handle, HI_U32 u32SearchPTS, PVR_INDEX_ENTRY_S *pEntry, HI_U32 *pu32Pos, HI_U32 IsForword);
HI_S32 PVR_Index_GetFrameByNum(const PVR_INDEX_HANDLE handle,  PVR_INDEX_ENTRY_S *pEntry, HI_U32 num);


/* seek opration */
HI_S32 PVR_Index_SeekToPTS(PVR_INDEX_HANDLE handle, HI_U32 u32PtsMs, HI_U32 IsForword, HI_U32 IsNextForword);
HI_S32 PVR_Index_SeekToStart(PVR_INDEX_HANDLE handle);
HI_S32 PVR_Index_SeekToEnd(PVR_INDEX_HANDLE handle);
HI_S32 PVR_Index_SeekToPauseOrStart(PVR_INDEX_HANDLE handle);
HI_S32 PVR_Index_SeekByTime(PVR_INDEX_HANDLE handle, HI_S64 offset, HI_S32 whence);
HI_S32 PVR_Index_SeekByFrame2I(PVR_INDEX_HANDLE handle, HI_S32 offset, HI_S32 whence);


/* for timeshift pause  */
HI_S32 PVR_Index_MarkPausePos(PVR_INDEX_HANDLE handle);

/*file opration*/
HI_S32 PVR_Index_PlayGetFileAttrByFileName(const HI_CHAR *pFileName, HI_UNF_PVR_FILE_ATTR_S *pAttr);
HI_VOID PVR_Index_GetIdxFileName(HI_CHAR* pIdxFileName, HI_CHAR* pSrcFileName);
HI_S32 PVR_Index_PrepareHeaderInfo(PVR_INDEX_HANDLE handle, HI_U32 u32UsrDataLen);
HI_S32 PVR_Index_GetUsrDataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32BuffSize);
HI_S32 PVR_Index_SetUsrDataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32UsrDataLen);
HI_S32 PVR_Index_GetCADataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32BuffSize);
HI_S32 PVR_Index_SetCADataInfo(HI_S32 s32Fd, HI_U8* pBuff, HI_U32 u32CADataLen);
HI_BOOL PVR_Index_CheckSetRecReachPlay(PVR_INDEX_HANDLE handle);
HI_BOOL PVR_Index_QureyClearRecReachPlay(PVR_INDEX_HANDLE handle);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifdef __HI_PVR_INDEX_H__ */

