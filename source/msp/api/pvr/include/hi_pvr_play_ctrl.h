/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_pvr_play_ctrl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/14
  Description   :
  History       :
  1.Date        : 2008/04/14
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_PLAY_CTRL_H__
#define __HI_PVR_PLAY_CTRL_H__

#include "hi_type.h"
#include "hi_pvr_fifo.h"
#include "hi_pvr_index.h"

#include "hi_drv_pvr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#define PVR_PLAY_DMX_GET_BUF_TIME_OUT 5000  /* ms */


/*whether move the short pakcet less than 188 byte to the end of TS or not.
    0:not move, and after that fill it with 0xff
    1:move it, and before that fill it with 0xff 
*/
#define PVR_TS_MOVE_TO_END   1


#define PVR_PLAY_DO_NOT_MARK_DISPLAY 0xffU

#define PVR_PLAY_STEP_WATI_TIME   1000UL  /* ms */

#define PVR_PLAY_MAX_FRAME_SIZE  (1024*1024*10)   /* the size of max frame */

#define PVR_PLAY_PICTURE_HEADER_LEN  4			/* the length of picture header ID, in byte */


#define WHENCE_STRING(whence)   ((0 == (whence)) ? "SEEK_SET" : ((1 == (whence)) ? "SEEK_CUR" : "SEEK_END"))

/* check channel validity                                                   */
#define PVR_PLAY_CHECK_CHN(u32Chn)\
    do {\
        if (u32Chn >= PVR_PLAY_MAX_CHN_NUM )\
        {\
            HI_ERR_PVR("play chn(%u) id invalid!\n", u32Chn);\
            return HI_ERR_PVR_INVALID_CHNID;\
        }\
    }while(0)

/* check play module initialized                                            */
#define PVR_PLAY_CHECK_INIT(pCommAttr)\
    do {\
        if (HI_FALSE == (pCommAttr)->bInit)\
        {\
            HI_ERR_PVR("play not inti yet!\n");\
            return HI_ERR_PVR_NOT_INIT;\
        }\
    }while(0)

#define PVR_PLAY_CHECK_CHN_INIT(enState)\
            do\
            {\
                if (HI_UNF_PVR_PLAY_STATE_INVALID ==  enState )\
                {\
                    return HI_ERR_PVR_CHN_NOT_INIT;\
                }\
            } while (0)

#define PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr)\
                    do\
                    {\
                        if (HI_UNF_PVR_PLAY_STATE_INVALID ==  pChnAttr->enState )\
                        {\
                            PVR_UNLOCK(&(pChnAttr->stMutex));\
                            return HI_ERR_PVR_CHN_NOT_INIT;\
                        }\
                    } while (0)

/* PVR ts file read.
return pointer offset forward on success.
otherwise, return the file header.*/
#define  PVR_PLAY_READ_FILE(pu8Addr, offset, size, pChnAttr) \
            do \
            {\
                ssize_t  n;\
                if ((n = PVR_PREAD64(pu8Addr, (size), \
                            pChnAttr->s32DataFile, (offset))) == -1)\
                {\
                    if (EINTR == errno)\
                    {\
                        continue;\
                    }\
                    else if (errno)\
                    { \
                        perror("read ts error: ");\
                        return HI_ERR_PVR_FILE_CANT_READ;\
                    }\
                    else\
                    {\
                        HI_ERR_PVR("read err1,  want:%u, off:%llu \n", (size), offset);\
                        return HI_ERR_PVR_FILE_TILL_END;\
                    }\
                }\
                if ((0 == n) && (0 != (size)))\
                {\
                    HI_WARN_PVR("read 0,  want:%u, off:%llu \n", (size), offset);\
                    return HI_ERR_PVR_FILE_TILL_END;\
                }\
           }while(0)


/* common information for play module                                      */
typedef struct hiPVR_PLAY_COMM_S
{
    HI_BOOL bInit;
    HI_S32  s32Reserved;
} PVR_PLAY_COMM_S;

/* the info struction for calculating trick mode rate */
typedef struct hiPVR_TPLAY_SPEED_CTRL_S
{
    HI_U32               u32RefFrmPtsMs;         /* the PTS of reference frame, usually, the first frame PTS*/
    HI_U32               u32RefFrmSysTimeMs;     /* the system time of reference frame output */

}PVR_TPLAY_SPEED_CTRL_S;

/* attributes of play channel                                               */
typedef struct hiPVR_PLAY_CHN_S
{
    HI_U32           u32chnID;

    HI_HANDLE        hAvplay;                 /* avplay handle */
    HI_HANDLE        hTsBuffer;               /* TS buffer handle */
    HI_HANDLE        hCipher;                 /* cipher handle */
    PVR_INDEX_HANDLE IndexHandle;             /* index handle */

    HI_UNF_PVR_PLAY_ATTR_S  stUserCfg;               /* play attributes for user configure */

    HI_U64           u64LastSeqHeadOffset;    /* last sequence offset */

    HI_UNF_PVR_PLAY_STATE_E enState;                 /* play state */
    HI_UNF_PVR_PLAY_STATE_E enLastState;                 /* last play state */
    HI_UNF_PVR_PLAY_SPEED_E enSpeed;

    PVR_FILE64       s32DataFile;             /* descriptor of play file */
    HI_U64           u64CurReadPos;           /* current data file read position */
    PVR_PHY_BUF_S    stCipherBuf;             /* cipher buffer for data decrypt */
    HI_BOOL          bCAStreamHeadSent;

    pthread_t        PlayStreamThread;        /* play thread id   */
    HI_BOOL          bQuickUpdateStatus;      /* new play status incoming */
    HI_BOOL          bPlayMainThreadStop;
    HI_BOOL          bEndOfFile;             /* playing to EOF */
    HI_BOOL          bTillStartOfFile;       /* TRUE: reach to the start of file, FALSE: reach to the end of the file,  used together with bEndOfFile */
    HI_BOOL          bTsBufReset;

    HI_BOOL          bPlayingTsNoIdx;
    HI_U64           u64TsFileSize;          /* the size of ts file, to control the end for playing without index file */

    HI_U32           u32LastEsBufSize;
    HI_U32           u32LastPtsMs;
    PVR_TPLAY_SPEED_CTRL_S stTplayCtlInfo;      /* control info for trick mode */
    HI_UNF_PVR_PLAY_STATUS_S stLastStatus;     /* the last play status, when failure to get current play status, return this */
    ExtraCallBack     readCallBack;
    pthread_mutex_t  stMutex;

} PVR_PLAY_CHN_S;


HI_S32 HI_PVR_PlayRegisterReadCallBack(HI_U32 u32Chn, ExtraCallBack readCallBack);

HI_S32 HI_PVR_PlayUnRegisterReadCallBack(HI_U32 u32Chn);

HI_BOOL PVR_Play_IsFilePlayingSlowPauseBack(const HI_CHAR *pFileName);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifdef __HI_PVR_PLAY_CTRL_H__ */

