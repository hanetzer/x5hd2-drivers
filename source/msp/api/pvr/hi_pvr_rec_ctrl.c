/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_rec_ctrl.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/10
  Description   : RECORD module
  History       :
  1.Date        : 2008/04/10
    Author      : q46153
    Modification: Created file

******************************************************************************/

#include <errno.h>
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "drv_struct_ext.h"

#include "pvr_debug.h"
#include "hi_pvr_rec_ctrl.h"
#include "hi_pvr_play_ctrl.h"
#include "hi_pvr_intf.h"
#include "hi_pvr_index.h"
#include "hi_mpi_demux.h"
#include "hi_drv_pvr.h"
#include "pvr_scd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 g_s32PvrFd = -1;      /*PVR file description */
char api_pathname_pvr[] = "/dev/" UMAP_DEVNAME_PVR;

/* init flag of record module                                               */
STATIC PVR_REC_COMM_S g_stRecInit;

/* all information of record channel                                        */
STATIC PVR_REC_CHN_S g_stPvrRecChns[PVR_REC_MAX_CHN_NUM];

#ifdef PVR_PROC_SUPPORT
static PVR_REC_CHN_PROC_S *paRecProcInfo[PVR_REC_MAX_CHN_NUM];  /* PROC info pointer of recording channel */
#endif

#define PVR_GET_RECPTR_BY_CHNID(chnId) (&g_stPvrRecChns[chnId - PVR_REC_START_NUM])
#define PVR_REC_IS_REWIND(pstRecAttr) ((pstRecAttr)->bRewind)
#define PVR_REC_IS_FIXSIZE(pstRecAttr) ((((pstRecAttr)->u64MaxFileSize > 0) || ((pstRecAttr)->u64MaxTimeInMs > 0)) && !((pstRecAttr)->bRewind))

//extern HI_S32 HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

STATIC INLINE HI_S32 PVRRecDevInit(HI_VOID)
{
    int fd;

#ifdef PVR_PROC_SUPPORT
    HI_S32 ret;
    HI_U32 PhyAddr;
    HI_U32 u32BufSize;
    HI_U32 u32VirAddr;
    HI_U32 i;
#endif

    if (g_s32PvrFd == -1)
    {
        fd = open (api_pathname_pvr, O_RDWR, 0);

        if (fd <= 0)
        {
            HI_FATAL_PVR("Cannot open '%s'\n", api_pathname_pvr);
            return HI_FAILURE;
        }

        g_s32PvrFd = fd;
    }
#ifdef PVR_PROC_SUPPORT
    ret = ioctl(g_s32PvrFd, CMD_PVR_INIT_REC, (HI_S32)&PhyAddr);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_PVR("pvr rec init error\n");
        return HI_FAILURE;
    }

    u32BufSize = sizeof(PVR_REC_CHN_PROC_S) * PVR_REC_MAX_CHN_NUM;
    u32BufSize = (u32BufSize % 4096 == 0) ? u32BufSize : (u32BufSize / 4096 + 1) * 4096;

    u32VirAddr = (HI_U32)HI_MMAP(PhyAddr, u32BufSize);
    if (0 == u32VirAddr)
    {
        HI_FATAL_PVR("rec proc buffer mmap error\n");
        return HI_FAILURE;
    }

    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        paRecProcInfo[i] = (PVR_REC_CHN_PROC_S*)(u32VirAddr + sizeof(PVR_REC_CHN_PROC_S) * i);
    }
#endif
    return HI_SUCCESS;
}

STATIC INLINE PVR_REC_CHN_S * PVRRecFindFreeChn(HI_VOID)
{
    PVR_REC_CHN_S * pChnAttr = NULL;

#if 0 /* not support multi-process */
    HI_U32 i;

    /* find a free play channel */
    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        if (g_stPvrRecChns[i].enState == HI_UNF_PVR_REC_STATE_INVALID)
        {
            pChnAttr = &g_stPvrRecChns[i];
            pChnAttr->enState = HI_UNF_PVR_REC_STATE_INIT;
            break;
        }
    }

#else /* support multi-process by kernel manage resources */
    HI_U32 ChanId;
    if (HI_SUCCESS != ioctl(g_s32PvrFd, CMD_PVR_CREATE_REC_CHN, (HI_S32)&ChanId))
    {
        HI_FATAL_PVR("pvr rec creat channel error\n");
        return HI_NULL;
    }

    HI_ASSERT(g_stPvrRecChns[ChanId].enState == HI_UNF_PVR_REC_STATE_INVALID);
    pChnAttr = &g_stPvrRecChns[ChanId];
    pChnAttr->enState = HI_UNF_PVR_REC_STATE_INIT;
#endif


    return pChnAttr;
}

/*
STATIC INLINE HI_VOID PVRRecCheckExistFile(HI_CHAR* pTsFileName)
{
    if (PVR_CHECK_FILE_EXIST(pTsFileName))
    {
        HI_CHAR szIdxFileName[PVR_MAX_FILENAME_LEN + 4] = {0};
        sprintf(szIdxFileName, "%s.idx", pTsFileName);
        truncate(pTsFileName, 0);
        truncate(szIdxFileName, 0);
    }

    return;
}
*/
STATIC INLINE HI_S32 PVRRecCheckUserCfg(const HI_UNF_PVR_REC_ATTR_S *pUserCfg)
{
    HI_U32 i;

    CHECK_REC_DEMUX_ID(pUserCfg->u32DemuxID);

    if ((HI_UNF_PVR_STREAM_TYPE_TS != pUserCfg->enStreamType)
        && (HI_UNF_PVR_STREAM_TYPE_ALL_TS != pUserCfg->enStreamType))
    {
        HI_ERR_PVR("enStreamType error, not support this stream type:(%d)\n", pUserCfg->enStreamType);
        return HI_ERR_PVR_INVALID_PARA;
    }

    /* all ts record, just only used for analysing, not supported cipher and rewind */
    if (HI_UNF_PVR_STREAM_TYPE_ALL_TS == pUserCfg->enStreamType)
    {
        if (pUserCfg->bRewind || pUserCfg->stEncryptCfg.bDoCipher)
        {
            HI_ERR_PVR("All Ts record can't support rewind or ciphter\n");
            return HI_ERR_PVR_INVALID_PARA;
        }
    }

    if (pUserCfg->enIndexType >= HI_UNF_PVR_REC_INDEX_TYPE_BUTT)
    {
        HI_ERR_PVR("pUserCfg->enIndexType(%d) >= HI_UNF_PVR_REC_INDEX_TYPE_BUTT\n", pUserCfg->enIndexType);
        return HI_ERR_PVR_INVALID_PARA;
    }

    if (HI_UNF_PVR_REC_INDEX_TYPE_VIDEO == pUserCfg->enIndexType)
    {
        if (pUserCfg->enIndexVidType >= HI_UNF_VCODEC_TYPE_BUTT)
        {
            HI_ERR_PVR("pUserCfg->enIndexVidType(%d) >= HI_UNF_VCODEC_TYPE_BUTT\n", pUserCfg->enIndexVidType);
            return HI_ERR_PVR_INVALID_PARA;
        }
    }

    if ((pUserCfg->u32DavBufSize % PVR_FIFO_WRITE_BLOCK_SIZE)
        || (!((pUserCfg->u32DavBufSize >= PVR_REC_MIN_DAV_BUF)
              && (pUserCfg->u32DavBufSize <= PVR_REC_MAX_DAV_BUF))))
    {
        HI_ERR_PVR("invalid dav buf size:%u\n", pUserCfg->u32DavBufSize);
        return HI_ERR_PVR_INVALID_PARA;
    }

    if ((pUserCfg->u32ScdBufSize % 28)
        || (!((pUserCfg->u32ScdBufSize >= PVR_REC_MIN_SC_BUF)
              && (pUserCfg->u32ScdBufSize <= PVR_REC_MAX_SC_BUF))))
    {
        HI_ERR_PVR("invalid scd buf size:%u\n", pUserCfg->u32ScdBufSize);
        return HI_ERR_PVR_INVALID_PARA;
    }

    PVR_CHECK_CIPHER_CFG(&pUserCfg->stEncryptCfg);

    /*  if record file name ok */
    if (((strlen(pUserCfg->szFileName)) >= PVR_MAX_FILENAME_LEN)
        || (strlen(pUserCfg->szFileName) != pUserCfg->u32FileNameLen))
    {
        HI_ERR_PVR("Invalid file name, file name len=%d!\n", pUserCfg->u32FileNameLen);
        return HI_ERR_PVR_FILE_INVALID_FNAME;
    }

    if (pUserCfg->u32UsrDataInfoSize > PVR_MAX_USERDATA_LEN)
    {
        HI_ERR_PVR("u32UsrDataInfoSize(%u) too larger\n", pUserCfg->u32UsrDataInfoSize);
        return HI_ERR_PVR_REC_INVALID_UDSIZE;
    }

    /* check for cycle record. for cycle record, the length should more than PVR_MIN_CYC_SIZE, and it MUST not be zero */
    if (PVR_REC_IS_REWIND(pUserCfg))
    {
        if ((pUserCfg->u64MaxFileSize < PVR_MIN_CYC_SIZE)
            &&(pUserCfg->u64MaxTimeInMs < PVR_MIN_CYC_TIMEMS))
        {
            HI_ERR_PVR("record file rewind, but file size:%llu(time:%llu) less than %llu(%llu).\n",
                   pUserCfg->u64MaxFileSize, pUserCfg->u64MaxTimeInMs, PVR_MIN_CYC_SIZE, PVR_MIN_CYC_TIMEMS);
            return HI_ERR_PVR_REC_INVALID_FSIZE;
        }
    }
    else
    {
        /* the length too less and not equal zero. zero means no limited */
        if (((pUserCfg->u64MaxFileSize > 0)&&(pUserCfg->u64MaxFileSize < PVR_MIN_CYC_SIZE))
            ||((pUserCfg->u64MaxTimeInMs > 0)&&(pUserCfg->u64MaxTimeInMs < PVR_MIN_CYC_TIMEMS)))
        {
            HI_ERR_PVR("record file not rewind, but file size:%llu(time:%llu) less than %llu(%llu) and not 0.\n",
                   pUserCfg->u64MaxFileSize, pUserCfg->u64MaxTimeInMs, PVR_MIN_CYC_SIZE, PVR_MIN_CYC_TIMEMS);
            return HI_ERR_PVR_REC_INVALID_FSIZE;
        }
    }

    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        /* check whether the demux id used or not */
        if (HI_UNF_PVR_REC_STATE_INVALID != g_stPvrRecChns[i].enState)
        {
            if (g_stPvrRecChns[i].stUserCfg.u32DemuxID == pUserCfg->u32DemuxID)
            {
                HI_ERR_PVR("demux %d already has been used to record.\n", pUserCfg->u32DemuxID);
                return HI_ERR_PVR_ALREADY;
            }

            /* recording for the same file name or not*/
            if (0 == strcmp(g_stPvrRecChns[i].stUserCfg.szFileName, pUserCfg->szFileName))
            {
                HI_ERR_PVR("file %s was exist to be recording.\n", pUserCfg->szFileName);
                return HI_ERR_PVR_FILE_EXIST;
            }
        }
    }

    /* check if stream file exist!                                          */
    HI_PVR_RemoveFile(pUserCfg->szFileName);

    //PVRRecCheckExistFile((HI_U8*)pUserCfg->szFileName);

    return HI_SUCCESS;
}

/* on recording, in sequence write some data into ts file */
//STATIC HI_S32 PVRRecWriteStreamDirect(PVR_REC_CHN_S *pRecChn, HI_U8 *pBuf, HI_U32 len, HI_U64 u64OffsetInFile)
STATIC HI_S32 PVRRecWriteStreamDirect(PVR_REC_CHN_S *pRecChn, DMX_DATA_S TsData, HI_U32 len, HI_U64 u64OffsetInFile)
{
    ssize_t sizeWrite, sizeWriten = 0;
    HI_UNF_PVR_DATA_ATTR_S stDataAttr;
    HI_U32            u32StartFrm;
    HI_U32            u32EndFrm;
    PVR_INDEX_ENTRY_S stStartFrame;
    PVR_INDEX_ENTRY_S stEndFrame;
    HI_U64            u64LenAdp = 0;
    HI_U64            u64OffsetAdp = 0;
    HI_S32            ret = 0;

    if(NULL != pRecChn->writeCallBack)
    {
        u32StartFrm = pRecChn->IndexHandle->stCycMgr.u32StartFrame;

        ret = PVR_Index_GetFrameByNum(pRecChn->IndexHandle, &stStartFrame, u32StartFrm);
        if(HI_SUCCESS != ret)
        {
            HI_WARN_PVR("Get Start Frame failed,ret=%d\n", ret);
            stStartFrame.u64Offset = 0;
        }

        if (pRecChn->IndexHandle->stCycMgr.u32EndFrame > 0)
        {
            u32EndFrm = pRecChn->IndexHandle->stCycMgr.u32EndFrame - 1;
        }
        else
        {
            u32EndFrm = pRecChn->IndexHandle->stCycMgr.u32LastFrame - 1;
        }

        ret = PVR_Index_GetFrameByNum(pRecChn->IndexHandle, &stEndFrame, u32EndFrm);
        if(HI_SUCCESS != ret)
        {
            HI_WARN_PVR("Get End Frame failed,ret=%d\n",ret);
            stEndFrame.u64Offset = 0;
        }

        stDataAttr.u32ChnID = pRecChn->u32ChnID;
        stDataAttr.u64FileStartPos = stStartFrame.u64Offset;
        stDataAttr.u64FileEndPos = stEndFrame.u64Offset;
        stDataAttr.u64GlobalOffset = u64OffsetInFile;
        memset(stDataAttr.CurFileName, 0, sizeof(stDataAttr.CurFileName));
        PVRFileGetOffsetFName(pRecChn->dataFile, u64OffsetInFile, stDataAttr.CurFileName);
        PVR_Index_GetIdxFileName(stDataAttr.IdxFileName, pRecChn->stUserCfg.szFileName);

        u64LenAdp = len;
        u64OffsetAdp = u64OffsetInFile;

        PVRFileGetRealOffset(pRecChn->dataFile, &u64OffsetAdp, &u64LenAdp);
        if(u64LenAdp < len)
        {
            ret = pRecChn->writeCallBack(&stDataAttr, TsData.pAddr, TsData.u32PhyAddr, (HI_U32)u64OffsetAdp, (HI_U32)u64LenAdp);
            if (ret != HI_SUCCESS)
            {
                HI_ERR_PVR("write call back error:%x\n", ret);
            }

            u64OffsetAdp = u64OffsetInFile + u64LenAdp;
            PVRFileGetRealOffset(pRecChn->dataFile, &u64OffsetAdp, NULL);
            PVRFileGetOffsetFName(pRecChn->dataFile, u64OffsetInFile + len, stDataAttr.CurFileName);
            TsData.pAddr += u64LenAdp;
            ret = pRecChn->writeCallBack(&stDataAttr, TsData.pAddr, TsData.u32PhyAddr, (HI_U32)u64OffsetAdp, (HI_U32)(len - u64LenAdp));
            if (ret != HI_SUCCESS)
            {
                HI_ERR_PVR("write call back error:%x\n", ret);
            }
            TsData.pAddr -= u64LenAdp;
        }
        else
        {
            ret = pRecChn->writeCallBack(&stDataAttr, TsData.pAddr, TsData.u32PhyAddr, (HI_U32)u64OffsetAdp, (HI_U32)u64LenAdp);
            if (ret != HI_SUCCESS)
            {
                HI_ERR_PVR("write call back error:%x\n", ret);
            }
        }
    }

    /* try to cycle write*/
    do
    {
         sizeWrite = PVR_PWRITE64(&((const char *)TsData.pAddr)[sizeWriten],
                                 len - (HI_U32)sizeWriten,
                                 pRecChn->dataFile,
                                 u64OffsetInFile + sizeWriten);
        if ((-1) == sizeWrite)
        {
            if (EINTR == errno)
            {
                perror("can's write ts error number is EINTR: ");
                HI_WARN_PVR("EINTR can't write ts. try:%u, addr:%p, fd:%d\n", len, TsData.pAddr, pRecChn->dataFile);
                continue;
            }
            else if (ENOSPC == errno)
            {
                return HI_ERR_PVR_FILE_DISC_FULL;
            }
            else
            {
                perror("can's write ts: ");
                HI_ERR_PVR("can't write ts. try:%u, addr:%p, fd:%d\n", len, TsData.pAddr, pRecChn->dataFile);
                return HI_ERR_PVR_FILE_CANT_WRITE;
            }
        }

        sizeWriten += sizeWrite;
    } while ((HI_U32)sizeWriten < len);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVRRecCycWriteStream
 Description     : write stream to file
 Input           : pBuf      **
                   len       **
                   dataFile  **
                   chnID     **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/29
    Author       : q46153
    Modification : Created function

*****************************************************************************/
//STATIC HI_S32 PVRRecCycWriteStream(HI_U8 *pBuf, HI_U32 len, PVR_REC_CHN_S *pRecChn)
STATIC HI_S32 PVRRecCycWriteStream(DMX_DATA_S TsData, PVR_REC_CHN_S *pRecChn)
{
    HI_U32 len = 0;
    HI_U32 len1 = 0;
    HI_U32 len2 = 0;
    HI_U64 before_pos = pRecChn->u64CurFileSize;
    HI_S32 ret;
    HI_U64 before_globalPos = before_pos;
    static HI_BOOL s_bTimeRewindFlg = HI_FALSE;
    HI_U64 u64MaxSize = 0;

    //HI_ERR_PVR("==%d,%llu\n", pRecChn->stUserCfg.bRewind, pRecChn->stUserCfg.u64MaxFileSize);

    len = TsData.u32Len;
    len1 = len;

    /* record fixed file length,  reach to the length, stop record */
    if (HI_UNF_PVR_STREAM_TYPE_ALL_TS == pRecChn->stUserCfg.enStreamType)
    {
        u64MaxSize =  pRecChn->stUserCfg.u64MaxFileSize;
    }
    else
    {
        u64MaxSize =  pRecChn->IndexHandle->stCycMgr.u64MaxCycSize;
    }

    len1 = len;
    if (PVR_REC_IS_FIXSIZE(&pRecChn->stUserCfg))
    {
        if (u64MaxSize > 0)
        {
            if ((pRecChn->u64CurFileSize + len) > u64MaxSize)
            {
               HI_ERR_PVR("cur size will over fix size, cur size:%llu, fix size:%llu\n",
                         pRecChn->u64CurFileSize, u64MaxSize);
               return HI_ERR_PVR_FILE_TILL_END;
            }
        }
    }
    else if (PVR_REC_IS_REWIND(&pRecChn->stUserCfg))  /* case rewind record */
    {
        if (u64MaxSize > 0)
        {
            before_pos = pRecChn->u64CurFileSize % u64MaxSize;

            if ((before_pos + (HI_U64)len) >= u64MaxSize) /* stride the rewind */
            {
                pRecChn->s32OverFixTimes++;
                len1 = (HI_U32)(u64MaxSize - before_pos);
                len2 = len - len1;
            }

            /* In fix-time rewind case,when the first rewind time u64CurFileSize may bigger than u64MaxCycSize.*/
            if ((s_bTimeRewindFlg)
                &&((pRecChn->u64CurFileSize +(HI_U64)len) > u64MaxSize ))
            {
                pRecChn->s32OverFixTimes++;
                s_bTimeRewindFlg = HI_FALSE;
            }
        }
        else
        {
            s_bTimeRewindFlg = HI_TRUE;
        }
        //printf("%llu,--%u,%u\n", before_pos, len1, len2);
        //usleep(1);
    }
    else
    {
        ;
    }

    if (TsData.pAddr)
    {
        //HI_INFO_PVR("Write before_pos=%llu, len=%d.\n", before_pos ,len1);
       // ret = PVRRecWriteStreamDirect(pRecChn, pBuf, len1, before_pos);
        ret = PVRRecWriteStreamDirect(pRecChn, TsData, len1, before_pos);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("write pvr file error\n");
            return ret;
        }

        pRecChn->u64CurFileSize += len1;
        pRecChn->u32Flashlen += len1;
        if (pRecChn->u32Flashlen >= 1024 * 1024)
        {
            //PVR_FSYNC64(pRecChn->dataFile);
            pRecChn->u32Flashlen = 0;
        }

        //HI_ERR_PVR("S=%llu, Max=%llu.\n", pRecChn->u64CurFileSize ,pRecChn->stUserCfg.u64MaxFileSize);
        //printf("r + %u = %llx.\n", len1, pRecChn->u64CurFileSize);
    }

    if (len2 > 0)
    {
        /* for two direction, after writing the first direction, it just write to the end of file*/
        if(u64MaxSize > 0)
        {
            HI_ASSERT(0LLU == (pRecChn->u64CurFileSize % u64MaxSize));
        }

        //ret = PVRRecWriteStreamDirect(pRecChn, pBuf + len1, len2, 0);
        TsData.pAddr += len1;
        ret = PVRRecWriteStreamDirect(pRecChn, TsData, len2, 0);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("write pvr file error\n");
            return ret;
        }

        pRecChn->u64CurFileSize += len2;
        pRecChn->u32Flashlen += len2;
        if (pRecChn->u32Flashlen >= 1024 * 1024)
        {
            //PVR_FSYNC64(pRecChn->dataFile);
            pRecChn->u32Flashlen = 0;
        }

        //HI_INFO_PVR("r2 + %u = %llx.\n", len1, pRecChn->u64CurFileSize);
    }
    else
    {
        HI_ASSERT(len1 == len);
    }

    if (HI_UNF_PVR_STREAM_TYPE_ALL_TS != pRecChn->stUserCfg.enStreamType)
    {
        pRecChn->IndexHandle->u64FileSizeGlobal = pRecChn->u64CurFileSize;
        if (before_globalPos < pRecChn->IndexHandle->stCurPlayFrame.u64GlobalOffset)
        {
            if (pRecChn->u64CurFileSize > pRecChn->IndexHandle->stCurPlayFrame.u64GlobalOffset)
            {
                HI_ERR_PVR("cur size will over readPos, %llu-->%llu, ReadPos:%llu\n",
                           before_globalPos, pRecChn->u64CurFileSize,
                           pRecChn->IndexHandle->stCurPlayFrame.u64GlobalOffset);
            }
        }
    }

    return HI_SUCCESS;
}

STATIC INLINE HI_VOID PVRRecCheckError(const PVR_REC_CHN_S  *pChnAttr, HI_S32 ret)
{
    if (HI_SUCCESS == ret)
    {
        return;
    }

    if (HI_ERR_DMX_NOAVAILABLE_DATA == ret)
    {
        return;
    }

    if (HI_ERR_PVR_FILE_DISC_FULL == ret)
    {
        PVR_Intf_DoEventCallback(pChnAttr->u32ChnID, HI_UNF_PVR_EVENT_REC_DISKFULL, 0);
    }
    else if (HI_ERR_PVR_FILE_TILL_END == ret)
    {
        PVR_Intf_DoEventCallback(pChnAttr->u32ChnID, HI_UNF_PVR_EVENT_REC_OVER_FIX, 0);
    }
    else
    {
        PVR_Intf_DoEventCallback(pChnAttr->u32ChnID, HI_UNF_PVR_EVENT_REC_ERROR, ret);
    }

    return;
}

STATIC INLINE HI_VOID PVRRecCheckRecPosition(PVR_REC_CHN_S  *pChnAttr)
{
    HI_S32 times;

    if (PVR_REC_IS_REWIND(&pChnAttr->stUserCfg))
    {
        if (pChnAttr->s32OverFixTimes > 0)
        {
            times = pChnAttr->s32OverFixTimes;
            pChnAttr->s32OverFixTimes = 0;
            PVR_Intf_DoEventCallback(pChnAttr->u32ChnID, HI_UNF_PVR_EVENT_REC_OVER_FIX, times);
        }
    }
}

/*****************************************************************************
 Prototype       : PVRRecSaveIndex
 Description     : save index to fs
 Input           : pRecChn  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/11
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC HI_S32 PVRRecSaveIndex(FILE *fpDmxIdx, PVR_REC_CHN_S *pRecChn)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i;
    HI_U32 scCnt;
    HI_U32 DemuxID = pRecChn->stUserCfg.u32DemuxID;
    DMX_DATA_S ScData;
    DMX_IDX_DATA_S dmxScData;
    FINDEX_SCD_S scdToIdx;
    FRAME_POS_S stFrmInfo;
    static HI_U32 u32NextScdInvalidFlag = 0;

    /* loop to record index                                                    */
    ret = HI_MPI_DMX_AcquireRecScdBuf(DemuxID, &ScData, PVR_REC_DMX_GET_SC_TIME_OUT);
    if (HI_SUCCESS == ret)
    {
        scCnt = ScData.u32Len / sizeof(DMX_IDX_DATA_S);

        //HI_INFO_PVR("chan:%d=====scCnt=%d===.\n", pRecChn->u32ChnID, scCnt);

        if (fpDmxIdx)
        {
            fwrite(ScData.pAddr, 1, ScData.u32Len, fpDmxIdx);
        }

        /*deal with all the scd from hardware */
        for (i = 0; i < scCnt; i++)
        {
            memcpy(&dmxScData, ScData.pAddr + i * sizeof(DMX_IDX_DATA_S),
                   sizeof(DMX_IDX_DATA_S));

            if ((HI_UNF_PVR_REC_STATE_PAUSE == pRecChn->enState) || (1 == u32NextScdInvalidFlag))
            {
                dmxScData.u32Chn_Ovflag_IdxType_Flags &= (~(1 << 28)); //mark the upflow flag
                u32NextScdInvalidFlag = 1;
            }

            if (HI_UNF_PVR_REC_INDEX_TYPE_VIDEO == pRecChn->stUserCfg.enIndexType)
            {
                /* convert scd into the index struct of FIDX*/
                if (HI_SUCCESS == PVR_SCD_Scd2Idx(pRecChn->IndexHandle, &dmxScData, &scdToIdx))
                {
                    /* wait for TS, wait until the TS data has write to disk */
                    while (pRecChn->IndexHandle->u64GlobalOffset > pRecChn->u64CurFileSize)
                    {
                        if (HI_UNF_PVR_REC_STATE_STOP != pRecChn->enState)
                        {
                            usleep(10000);
                        }
                        else
                        {
                            goto scd_release;
                        }
                    }

                    pRecChn->IndexHandle->u32DmxClkTimeMs = dmxScData.u32SrcClk / 90; /* u32SrcClk in unit 90kHz, convert to millisecond */

                    /* deal with index for FIDX, and write index into file */
                    FIDX_FeedStartCode((HI_S32)pRecChn->IndexHandle->u32RecPicParser, &scdToIdx);
                }
            }
            else
            {
                if (HI_SUCCESS == PVR_SCD_Scd2AudioFrm(pRecChn->IndexHandle, &dmxScData, &stFrmInfo))
                {
                    /* wait for TS, wait until the TS data has write to disk */
                    while (pRecChn->IndexHandle->u64GlobalOffset > pRecChn->u64CurFileSize)
                    {
                        if (HI_UNF_PVR_REC_STATE_STOP != pRecChn->enState)
                        {
                            HI_INFO_PVR("wait stream to write ok, stream offset:0x%llx, index offset:0x%llx.\n",
                                        pRecChn->u64CurFileSize, pRecChn->IndexHandle->u64GlobalOffset);
                            usleep(10000);
                        }
                        else
                        {
                            goto scd_release;
                        }
                    }

                    pRecChn->IndexHandle->u32DmxClkTimeMs = dmxScData.u32SrcClk / 90; /* u32SrcClk in unit 90kHz, convert to millisecond */

                    PVR_Index_SaveFramePosition(pRecChn->IndexHandle->u32RecPicParser, &stFrmInfo);
                }
            }

            if (PVR_Play_IsFilePlayingSlowPauseBack(pRecChn->stUserCfg.szFileName))
            {
                if (PVR_Index_CheckSetRecReachPlay(pRecChn->IndexHandle)) /* rec reach to play */
                {
                    PVR_Index_SeekToStart(pRecChn->IndexHandle); /* force play to move forward */
                    PVR_Intf_DoEventCallback(pRecChn->u32ChnID, HI_UNF_PVR_EVENT_REC_REACH_PLAY, 0);
                }
            }
#ifdef PVR_PROC_SUPPORT
            /*save proc info */
            memcpy((HI_VOID*)&(paRecProcInfo[pRecChn->u32ChnID - PVR_REC_START_NUM]->stLastRecFrm),
                   (HI_VOID*)&pRecChn->IndexHandle->stCurRecFrame, sizeof(PVR_INDEX_ENTRY_S));
            paRecProcInfo[pRecChn->u32ChnID - PVR_REC_START_NUM]->u32WriteFrame = pRecChn->IndexHandle->u32WriteFrame;
#endif
        }

scd_release:
        if (HI_UNF_PVR_REC_STATE_PAUSE != pRecChn->enState)
        {
            u32NextScdInvalidFlag = 0;
        }

        ScData.u32Len = scCnt * sizeof(DMX_IDX_DATA_S);
        ret = HI_MPI_DMX_ReleaseRecScdBuf(DemuxID, &ScData);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Release SCD data failed:%#x\n", ret);
        }
    }
    else
    {
        if ((HI_ERR_DMX_NOAVAILABLE_DATA != ret) && (HI_ERR_DMX_TIMEOUT != ret))
        {
            HI_ERR_PVR("Acquire SCD data failed:%#x\n", ret);
        }

        usleep(10*1000);
    }

    return ret;
}

STATIC HI_VOID * PVRRecSaveIndexRoutine(HI_VOID *args)
{
    HI_S32 ret = HI_SUCCESS;

    PVR_REC_CHN_S *pRecChn = (PVR_REC_CHN_S *)args;

    FILE *fpDmxSc = NULL;

    /* +4: reserved for .sc AI7D02958 */
    HI_CHAR szScFileName[PVR_MAX_FILENAME_LEN + 4] = {0};

    HI_INFO_PVR("start %d.\n", pRecChn->u32ChnID);

    snprintf(szScFileName, PVR_MAX_FILENAME_LEN + 4, "%s.sc", pRecChn->stUserCfg.szFileName);

    //fpDmxSc = fopen(szScFileName, "wb");

    while (HI_UNF_PVR_REC_STATE_STOP != pRecChn->enState)
    {
        ret = PVRRecSaveIndex(fpDmxSc, pRecChn);
        if (!((HI_SUCCESS == ret) || (HI_ERR_DMX_NOAVAILABLE_DATA == ret) || (HI_ERR_DMX_TIMEOUT == ret)))
        {
            break;
        }
    }

    //if (fpDmxSc)
    //{
        //fclose(fpDmxSc);
    //}

    HI_INFO_PVR("<==PVRRecSaveIdxRoutine ret=%#x.\n", ret);
    return NULL;
}
/*****************************************************************************
 Prototype       : PVRRecSaveStream
 Description     : save stream to fs
 Input           : pRecChn  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/11
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC HI_VOID* PVRRecSaveStreamRoutine(HI_VOID *args)
{
    HI_S32 ret = HI_SUCCESS;
    DMX_DATA_S TsData = {0};

    HI_U32 u32OverflowTimes = 0;
    HI_MPI_DMX_BUF_STATUS_S stStatus;


    PVR_REC_CHN_S *pRecChn = (PVR_REC_CHN_S *)args;
    HI_U32 DemuxID = pRecChn->stUserCfg.u32DemuxID;

    HI_INFO_PVR("chan:%d, start.\n", pRecChn->u32ChnID);

    while (HI_UNF_PVR_REC_STATE_STOP != pRecChn->enState)
    {
        if (HI_UNF_PVR_REC_STATE_PAUSE == pRecChn->enState)
        {
            usleep(1000 * 10);
            continue;
        }

        ret = HI_MPI_DMX_AcquireRecTsBuf(DemuxID, &TsData,
                                         HI_FALSE, PVR_REC_DMX_GET_STREAM_TIME_OUT);
        if ((HI_SUCCESS == ret) && (TsData.u32Len > 0))
        {
            if (TsData.u32Len % PVR_TS_LEN)
            {
                HI_FATAL_PVR("rec size:%u != 188*N, offset:0x%llx.\n", TsData.u32Len, pRecChn->u64CurFileSize);
            }

            //HI_INFO_PVR("chan:%d, rec acquire size:%u.\n", pRecChn->u32ChnID, TsData.u32Len);
            HI_ASSERT(((TsData.u32Len % 512) == 0));
            
            /* if cipher, get and save the cipher data */
            if (pRecChn->stUserCfg.stEncryptCfg.bDoCipher)
            {
                ret = HI_UNF_CIPHER_Encrypt(pRecChn->hCipher, TsData.u32PhyAddr, TsData.u32PhyAddr, TsData.u32Len);
                if (ret != HI_SUCCESS)
                {
                    HI_ERR_PVR("HI_UNF_CIPHER_Encrypt failed:%#x!\n", ret);
                    HI_MPI_DMX_ReleaseRecTsBuf(DemuxID, &TsData);
                    continue;
                }
            }

           // ret = PVRRecCycWriteStream(TsData.pAddr, TsData.u32Len, pRecChn);
            ret = PVRRecCycWriteStream(TsData, pRecChn);
            if (HI_SUCCESS != ret)
            {
                HI_WARN_PVR("size:%u, addr:%p, ret:%#x\n", TsData.u32Len, TsData.pAddr, ret);

                //perror("PVR Write stream failed: ");
                (HI_VOID)HI_MPI_DMX_ReleaseRecTsBuf(DemuxID, &TsData);
                break;
            }

            //HI_INFO_PVR("chan:%d, release size:%u.\n", pRecChn->u32ChnID, TsData.u32Len);
            (HI_VOID)HI_MPI_DMX_ReleaseRecTsBuf(DemuxID, &TsData);

            /* check the buffer status */

            //get status
            ret = HI_MPI_DMX_GetRECBufferStatus(DemuxID, &stStatus);
            if (HI_SUCCESS == ret)
            {
                /* used > size*80% overflow */
                if (stStatus.u32UsedSize > stStatus.u32BufSize * 90 / 100)
                {
                    u32OverflowTimes++;
                    if (u32OverflowTimes > 5)
                    {
                        u32OverflowTimes = 0;
                        PVR_Intf_DoEventCallback(pRecChn->u32ChnID, HI_UNF_PVR_EVENT_REC_DISK_SLOW, 0);
                    }
                }
            }
        }
        else if ((HI_ERR_DMX_NOAVAILABLE_DATA == ret) || (HI_ERR_DMX_TIMEOUT == ret))
        {
            usleep(10*1000);
            continue;
        }
        else
        {
            HI_ERR_PVR("receive rec stream error:%#x\n", ret);
            break;
        }

        PVRRecCheckRecPosition(pRecChn);
    } /* end while */

    PVRRecCheckError(pRecChn, ret);

    if (HI_UNF_PVR_REC_STATE_STOP != pRecChn->enState)
    {
        HI_INFO_PVR("-----PVRRecSaveStreamRoutine exiting with error:%#x...\n", ret);
    }

    pRecChn->bSavingData = HI_FALSE;
    HI_INFO_PVR("<==PVRRecSaveStreamRoutine,FileLen:0x%llx.\n", pRecChn->u64CurFileSize);

    return NULL;
}

STATIC INLINE HI_S32 PVRRecPrepareCipher(PVR_REC_CHN_S *pChnAttr)
{
    HI_S32 ret;
    HI_UNF_PVR_CIPHER_S *pCipherCfg;
    HI_UNF_CIPHER_CTRL_S ctrl;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    pCipherCfg = &(pChnAttr->stUserCfg.stEncryptCfg);
    if (!pCipherCfg->bDoCipher)
    {
        return HI_SUCCESS;
    }

    /* get cipher handle */
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&pChnAttr->hCipher, &stCipherAttr);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_CIPHER_CreateHandle failed:%#x\n", ret);
        return ret;
    }

    ctrl.enAlg = pCipherCfg->enType;
    ctrl.bKeyByCA = HI_FALSE;
    memcpy(ctrl.u32Key, pCipherCfg->au8Key, sizeof(ctrl.u32Key));
    memset(ctrl.u32IV, 0, sizeof(ctrl.u32IV));
    if (HI_UNF_CIPHER_ALG_AES == pCipherCfg->enType)
    {
        ctrl.enBitWidth = PVR_CIPHER_AES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_AES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_AES_KEY_LENGTH;
    }
    else if (HI_UNF_CIPHER_ALG_DES == pCipherCfg->enType)
    {
        ctrl.enBitWidth = PVR_CIPHER_DES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_DES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_DES_KEY_LENGTH;
    }
    else
    {
        ctrl.enBitWidth = PVR_CIPHER_3DES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_3DES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_3DES_KEY_LENGTH;
    }

    ret = HI_UNF_CIPHER_ConfigHandle(pChnAttr->hCipher, &ctrl);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_CIPHER_ConfigHandle failed:%#x\n", ret);
        (HI_VOID)HI_UNF_CIPHER_DestroyHandle(pChnAttr->hCipher);
        return ret;
    }

    return HI_SUCCESS;
}

STATIC INLINE HI_S32 PVRRecReleaseCipher(PVR_REC_CHN_S  *pChnAttr)
{
    HI_S32 ret = HI_SUCCESS;

    /* free cipher handle */
    if (pChnAttr->hCipher)
    {
        ret = HI_UNF_CIPHER_DestroyHandle(pChnAttr->hCipher);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("release Cipher handle failed! erro:%#x\n", ret);
        }

        pChnAttr->hCipher = 0;
    }

    return ret;
}

/* return TRUE just only start record*/
HI_BOOL PVR_Rec_IsFileSaving(const HI_CHAR *pFileName)
{
    HI_U32 i;

    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        if (g_stPvrRecChns[i].bSavingData)
        {
            if (!strcmp((const char *)g_stPvrRecChns[i].stUserCfg.szFileName, (const char *)pFileName))
            {
                return HI_TRUE;
            }
        }
    }

    return HI_FALSE;
}

/*****************************************************************************
 Prototype       : PVR_Rec_IsChnRecording
 Description     : to check if record channel is recording
 Input           : u32ChnID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/30
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_BOOL PVR_Rec_IsChnRecording(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S  *pRecChn = HI_NULL;

    if ((u32ChnID < PVR_REC_START_NUM) || (u32ChnID >= PVR_REC_MAX_CHN_NUM + PVR_REC_START_NUM))
    {
        return HI_FALSE;
    }

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);

    if (HI_UNF_PVR_REC_STATE_RUNNING == pRecChn->enState)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

/*****************************************************************************
 Prototype       : PVR_Rec_MarkPausePos
 Description     : mark a flag for timeshift, and save the current record position
                        if start timeshift, playing from this position
 Input           : u32ChnID
 Output          : None
 Return Value    :
  History
  1.Date         : 2010/06/02
    Author       : j40671
    Modification : Created function

*****************************************************************************/
HI_S32 PVR_Rec_MarkPausePos(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S  *pRecChn = HI_NULL;

    CHECK_REC_CHNID(u32ChnID);
    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);

    return PVR_Index_MarkPausePos(pRecChn->IndexHandle);
}

/*****************************************************************************
 Prototype       : HI_PVR_RecInit
 Description     : init record module
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecInit(HI_VOID)
{
    HI_U32 i;
    HI_S32 ret;

    if (HI_TRUE == g_stRecInit.bInit)
    {
        HI_WARN_PVR("Record Module has been Initialized!\n");
        return HI_SUCCESS;
    }
    else
    {
        /* initialize all the index */
        PVR_Index_Init();

        ret = PVRRecDevInit();
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        FIDX_Init(PVR_Index_SaveFramePosition);
        ret = PVRIntfInitEvent();
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        /* set all record channel as INVALID status                            */
        for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
        {
            g_stPvrRecChns[i].enState  = HI_UNF_PVR_REC_STATE_INVALID;
            g_stPvrRecChns[i].u32ChnID = i + PVR_REC_START_NUM;
            g_stPvrRecChns[i].hCipher = 0;
            g_stPvrRecChns[i].writeCallBack = NULL;

            if (-1 == pthread_mutex_init(&(g_stPvrRecChns[i].stMutex), NULL))
            {
                PVRIntfDeInitEvent();

                /* TODO: destroy mutex **/
                HI_ERR_PVR("init mutex lock for PVR rec chn%d failed \n", i);
                return HI_FAILURE;
            }
        }

        g_stRecInit.bInit = HI_TRUE;

        return HI_SUCCESS;
    }
}

/*****************************************************************************
 Prototype       : HI_PVR_RecDeInit
 Description     : deinit record module
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecDeInit(HI_VOID)
{
    HI_U32 i;

    if (HI_FALSE == g_stRecInit.bInit)
    {
        HI_WARN_PVR("Record Module is not Initialized!\n");
        return HI_SUCCESS;
    }
    else
    {
        /* set all record channel as INVALID status                            */
        for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
        {
            if (g_stPvrRecChns[i].enState != HI_UNF_PVR_REC_STATE_INVALID)
            {
                HI_ERR_PVR("rec chn%d is in use, can NOT deInit REC!\n", i);
                return HI_ERR_PVR_BUSY;
            }

            (HI_VOID)pthread_mutex_destroy(&(g_stPvrRecChns[i].stMutex));
        }

        PVRIntfDeInitEvent();
        g_stRecInit.bInit = HI_FALSE;
        return HI_SUCCESS;
    }
}

/*****************************************************************************
 Prototype       : HI_PVR_RecCreateChn
 Description     : apply a new reocrd channel
 Input           : pstRecAttr  **the attr user config
 Output          : pu32ChnID   **the chn id we get
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecCreateChn(HI_U32 *pu32ChnID, const HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    HI_S32 ret;
    HI_U64 fileSizeReal;
    HI_UNF_PVR_REC_ATTR_S stRecAttrLocal;
    PVR_REC_CHN_S *pChnAttr = NULL;
    HI_U32 u32RecIdInteral;

    PVR_CHECK_POINTER(pu32ChnID);
    PVR_CHECK_POINTER(pstRecAttr);

    CHECK_REC_INIT(&g_stRecInit);

    memcpy(&stRecAttrLocal, pstRecAttr, sizeof(HI_UNF_PVR_REC_ATTR_S));
    ret = PVRRecCheckUserCfg(&stRecAttrLocal);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    fileSizeReal = (stRecAttrLocal.u64MaxFileSize / PVR_FIFO_WRITE_BLOCK_SIZE) * PVR_FIFO_WRITE_BLOCK_SIZE;
    stRecAttrLocal.u64MaxFileSize = fileSizeReal;

    pChnAttr = PVRRecFindFreeChn();
    if (NULL == pChnAttr)
    {
        HI_ERR_PVR("Not enough channel to be used!\n");
        return HI_ERR_PVR_NO_CHN_LEFT;
    }

    PVR_LOCK(&(pChnAttr->stMutex));
    u32RecIdInteral = pChnAttr->u32ChnID - PVR_REC_START_NUM;

    /* create an data file and open it                                         */
    pChnAttr->dataFile = PVR_OPEN64(stRecAttrLocal.szFileName, PVR_FOPEN_MODE_DATA_WRITE);
    if (PVR_FILE_INVALID_FILE == pChnAttr->dataFile)
    {
        HI_ERR_PVR("create stream file error!\n");
        PVR_REMOVE_FILE64(stRecAttrLocal.szFileName);
        pChnAttr->enState = HI_UNF_PVR_REC_STATE_INVALID;
        ioctl(g_s32PvrFd, CMD_PVR_DESTROY_REC_CHN, (HI_S32)&(u32RecIdInteral));
        PVR_UNLOCK(&(pChnAttr->stMutex));
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    PVR_SET_MAXFILE_SIZE(pChnAttr->dataFile, fileSizeReal);

    /* save chn user-config attr */
    memcpy(&pChnAttr->stUserCfg, &stRecAttrLocal, sizeof(HI_UNF_PVR_REC_ATTR_S));
    pChnAttr->stUserCfg.u64MaxFileSize = fileSizeReal;
    pChnAttr->u64CurFileSize = 0;
    pChnAttr->u32Flashlen = 0;
    if (HI_UNF_PVR_STREAM_TYPE_ALL_TS != pstRecAttr->enStreamType)
    {
        /* get a new index handle                                                  */
        pChnAttr->IndexHandle = PVR_Index_CreatRec(pChnAttr->u32ChnID, &stRecAttrLocal);
        if (HI_NULL_PTR == pChnAttr->IndexHandle)
        {
            PVR_CLOSE64(pChnAttr->dataFile);
            PVR_REMOVE_FILE64(stRecAttrLocal.szFileName);
            pChnAttr->enState = HI_UNF_PVR_REC_STATE_INVALID;
            ioctl(g_s32PvrFd, CMD_PVR_DESTROY_REC_CHN, (HI_S32)&(u32RecIdInteral));
            PVR_UNLOCK(&(pChnAttr->stMutex));
            return HI_ERR_PVR_INDEX_CANT_MKIDX;
        }

#ifdef PVR_PROC_SUPPORT
        paRecProcInfo[pChnAttr->u32ChnID - PVR_REC_START_NUM]->enIndexType = pChnAttr->IndexHandle->enIndexType;
#endif

        ret = PVRRecPrepareCipher(pChnAttr);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Pvr recorde prepare cipher error!\n");
            (HI_VOID)PVR_Index_Destroy(pChnAttr->IndexHandle, PVR_INDEX_REC);
            PVR_CLOSE64(pChnAttr->dataFile);
            PVR_REMOVE_FILE64(stRecAttrLocal.szFileName);
            pChnAttr->enState = HI_UNF_PVR_REC_STATE_INVALID;
            ioctl(g_s32PvrFd, CMD_PVR_DESTROY_REC_CHN, (HI_S32)&(u32RecIdInteral));
            PVR_UNLOCK(&(pChnAttr->stMutex));
            return ret;
        }
    }
    else
    {
        pChnAttr->IndexHandle = HI_NULL;
    }

#ifdef PVR_PROC_SUPPORT
    memcpy(&(paRecProcInfo[pChnAttr->u32ChnID - PVR_REC_START_NUM]->stUserCfg), &(pChnAttr->stUserCfg),
           sizeof(HI_UNF_PVR_REC_ATTR_S));
#endif

    HI_INFO_PVR("file size adjust to :%lld.\n", fileSizeReal);

    /* here we get record channel successfully                              */
    *pu32ChnID = pChnAttr->u32ChnID;
    HI_INFO_PVR("record creat ok\n");
    PVR_UNLOCK(&(pChnAttr->stMutex));

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecDestroyChn
 Description     : free record channel
 Input           : u32ChnID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecDestroyChn(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S *pRecChn = NULL;
    HI_U32 u32RecIdInteral;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    /* to affirm record channel stopped                                        */
    if ((HI_UNF_PVR_REC_STATE_RUNNING == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_PAUSE == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_STOPPING == pRecChn->enState))
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        HI_ERR_PVR(" can't free rec chn%d : chn still runing\n", u32ChnID);
        return HI_ERR_PVR_BUSY;
    }

    /* we don't care about whether it is timeshifting!                           */

    /* close index handle                                                      */
    if (HI_NULL != pRecChn->IndexHandle)
    {
        (HI_VOID)PVR_Index_Destroy(pRecChn->IndexHandle, PVR_INDEX_REC);
        pRecChn->IndexHandle = NULL;
    }
    (HI_VOID)PVRRecReleaseCipher(pRecChn);

    /* close data file                                                         */
    (HI_VOID)PVR_CLOSE64(pRecChn->dataFile);

    /* set channel state to invalid                                         */
    pRecChn->enState = HI_UNF_PVR_REC_STATE_INVALID;
    u32RecIdInteral = u32ChnID - PVR_REC_START_NUM;

    if (HI_SUCCESS != ioctl(g_s32PvrFd, CMD_PVR_DESTROY_REC_CHN, (HI_S32)&u32RecIdInteral))
    {
        HI_FATAL_PVR("pvr rec destroy channel error.\n");
        PVR_UNLOCK(&(pRecChn->stMutex));
        return HI_FAILURE;
    }

    PVR_UNLOCK(&(pRecChn->stMutex));

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecSetChn
 Description     : set record channel attributes
 Input           : u32ChnID  **
                   pRecAttr  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecSetChn(HI_U32 u32ChnID, const HI_UNF_PVR_REC_ATTR_S * pstRecAttr)
{
    PVR_REC_CHN_S *pRecChn = NULL;

    CHECK_REC_CHNID(u32ChnID);
    PVR_CHECK_POINTER(pstRecAttr);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    CHECK_REC_CHN_INIT(pRecChn->enState);

    /* currently, we can't set record channel dynamically. */

    return HI_ERR_PVR_NOT_SUPPORT;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecGetChn
 Description     : get record channel attributes
 Input           : u32ChnID  **
                   pRecAttr  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecGetChn(HI_U32 u32ChnID, HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    PVR_REC_CHN_S *pRecChn = NULL;

    CHECK_REC_CHNID(u32ChnID);

    PVR_CHECK_POINTER(pstRecAttr);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    CHECK_REC_CHN_INIT(pRecChn->enState);

    memcpy(pstRecAttr, &(pRecChn->stUserCfg), sizeof(HI_UNF_PVR_REC_ATTR_S));

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecStartChn
 Description     : start record channel
 Input           : u32ChnID, the record channel ID
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecStartChn(HI_U32 u32ChnID)
{
    HI_S32 ret;
    PVR_REC_CHN_S *pRecChn = NULL;
    HI_UNF_PVR_REC_ATTR_S *pUserCfg;
    HI_MPI_DMX_RECORD_TYPE_E    enRecType;
    HI_MPI_DMX_REC_INDEX_TYPE_E enIndexType;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    if ((HI_UNF_PVR_REC_STATE_RUNNING == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_PAUSE == pRecChn->enState))
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        return HI_ERR_PVR_ALREADY;
    }
    else
    {
        pRecChn->enState = HI_UNF_PVR_REC_STATE_RUNNING;
    }

#ifdef PVR_PROC_SUPPORT
    paRecProcInfo[pRecChn->u32ChnID - PVR_REC_START_NUM]->enState = pRecChn->enState;
#endif

    pUserCfg = &(pRecChn->stUserCfg);

    if (HI_UNF_PVR_STREAM_TYPE_ALL_TS == pUserCfg->enStreamType)
    {
        enRecType = HI_MPI_DMX_RECORD_ALL_TS;
    }
    else
    {
        if (HI_FALSE == pUserCfg->bIsClearStream)
        {
            enRecType = HI_MPI_DMX_RECORD_SCRAM_TS;
        }
        else
        {
            enRecType = HI_MPI_DMX_RECORD_DESCRAM_TS;
        }
    }

    switch (pUserCfg->enIndexType)
    {
        case HI_UNF_PVR_REC_INDEX_TYPE_VIDEO :
            enIndexType = HI_MPI_DMX_REC_INDEX_TYPE_VIDEO;
            break;

        case HI_UNF_PVR_REC_INDEX_TYPE_AUDIO :
            enIndexType = HI_MPI_DMX_REC_INDEX_TYPE_AUDIO;
            break;

        default :
            enIndexType = HI_MPI_DMX_REC_INDEX_TYPE_NONE;
    }

    //PVRRecCheckExistFile(pUserCfg->szFileName);

    /* create record thread to receive index from the channel                 */
    ret = HI_MPI_DMX_StartRecord(pUserCfg->u32DemuxID, enIndexType, pUserCfg->u32IndexPid,
                                 pUserCfg->enIndexVidType,
                                 enRecType, pUserCfg->u32ScdBufSize, pUserCfg->u32DavBufSize);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("start demux record channel failure!\n");
        pRecChn->enState = HI_UNF_PVR_REC_STATE_INIT;
        PVR_UNLOCK(&(pRecChn->stMutex));
        return ret;
    }
    else
    {
        HI_INFO_PVR("start demux OK, indexTYpe:%d!\n", pUserCfg->enIndexType);
    }

    pRecChn->u64CurFileSize = 0;
    pRecChn->u32Flashlen = 0;
    pRecChn->bSavingData = HI_TRUE;
    pRecChn->s32OverFixTimes = 0;
    gettimeofday(&(pRecChn->tv_start), NULL);

    if (HI_NULL != pRecChn->IndexHandle)
    {
        PVR_Index_ResetRecAttr(pRecChn->IndexHandle);

        /* failure to write user data, but still, continue to record. just only print the error info */
        if (PVR_Index_PrepareHeaderInfo(pRecChn->IndexHandle, pRecChn->stUserCfg.u32UsrDataInfoSize))
        {
            HI_ERR_PVR("PVR_Index_PrepareHeaderInfo fail\n");
        }

        /* create record thread to receive index from the channel                 */
        ret = pthread_create(&pRecChn->RecordIndexThread, NULL, PVRRecSaveIndexRoutine, pRecChn);
        if (ret != HI_SUCCESS)
        {
            pRecChn->enState = HI_UNF_PVR_REC_STATE_STOP;
            HI_ERR_PVR("create record INDEX thread failure!\n");
            (HI_VOID)HI_MPI_DMX_StopRecord(pUserCfg->u32DemuxID);
            PVR_UNLOCK(&(pRecChn->stMutex));
            return HI_FAILURE;
        }
    }

    /* create record thread to receive stream from the channel                 */
    ret = pthread_create(&pRecChn->RecordStreamThread, NULL, PVRRecSaveStreamRoutine, pRecChn);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("create record STREAM thread failure!\n");
        (HI_VOID)HI_MPI_DMX_StopRecord(pUserCfg->u32DemuxID);
        pRecChn->enState = HI_UNF_PVR_REC_STATE_INIT;
        PVR_UNLOCK(&(pRecChn->stMutex));
        return HI_FAILURE;
    }

    HI_INFO_PVR("channel %d start ok.\n", u32ChnID);

    PVR_UNLOCK(&(pRecChn->stMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecStopChn
 Description     : stop the pointed record channel
 Input           : u32ChnId, channle id
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecStopChn(HI_U32 u32ChnID)
{
    HI_S32 ret;

    //HI_UNF_PVR_FILE_ATTR_S  fileAttr;

    PVR_REC_CHN_S  *pRecChn;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    /* to confirm the record channel running                           */
    if ((HI_UNF_PVR_REC_STATE_RUNNING != pRecChn->enState)
        && (HI_UNF_PVR_REC_STATE_PAUSE != pRecChn->enState))
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        HI_ERR_PVR("Channel has already stopped!\n");
        return HI_ERR_PVR_ALREADY;
    }

    /* state: stoping -> stop. make sure the index thread exit first   */
    pRecChn->enState = HI_UNF_PVR_REC_STATE_STOPPING;

    //(HI_VOID)HI_PthreadJoin(pRecChn->RecordIndexThread, NULL);
    //HI_ASSERT(HI_UNF_PVR_REC_STATE_STOP == pRecChn->enState);

#if 0
    ret = PVR_Index_RecGetFileAttr(pRecChn->IndexHandle, &fileAttr);
    if (HI_SUCCESS == ret)
    {
        indexedSize = fileAttr.u64ValidSizeInByte;
    }
    else
    {
        indexedSize = pRecChn->u64CurFileSize;
    }

    HI_ERR_PVR("file size:%llu, index size:%llu\n", pRecChn->u64CurFileSize,
               fileAttr.u64ValidSizeInByte);

    while ((pRecChn->u64CurFileSize < indexedSize)
           && (waitTimes < 30)
           && pRecChn->bSavingData)    /*If returned already by error,go ahead*/ /*CNcomment:如果已经出错退出，可以不用继续等待 */
    {
        usleep(1000 * 40);
        waitTimes++;
        HI_ERR_PVR("wait%u, file size:%llu, index size:%llu\n", waitTimes,
                   pRecChn->u64CurFileSize,
                   fileAttr.u64ValidSizeInByte);
    }
#endif


    pRecChn->enState = HI_UNF_PVR_REC_STATE_STOP;
    (HI_VOID)pthread_join(pRecChn->RecordStreamThread, NULL);

    if (HI_NULL != pRecChn->IndexHandle)
    {
        (HI_VOID)pthread_join(pRecChn->RecordIndexThread, NULL);
    }

    ret = HI_MPI_DMX_StopRecord(pRecChn->stUserCfg.u32DemuxID);
    if (HI_SUCCESS != ret)
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        HI_ERR_PVR("demux stop error:%#x\n", ret);
        return ret;
    }
    else
    {
        HI_INFO_PVR("stop demux%d ok\n", pRecChn->stUserCfg.u32DemuxID);
    }

#ifdef PVR_PROC_SUPPORT
    paRecProcInfo[pRecChn->u32ChnID - PVR_REC_START_NUM]->enState = pRecChn->enState;
#endif

    PVR_UNLOCK(&(pRecChn->stMutex));
    return HI_SUCCESS;
}

HI_S32 HI_PVR_RecPauseChn(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S  *pRecChn;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    /* to confirm the record channel running  */
    if ((HI_UNF_PVR_REC_STATE_RUNNING != pRecChn->enState)
        && (HI_UNF_PVR_REC_STATE_PAUSE != pRecChn->enState))
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        HI_ERR_PVR("Channel not started!\n");
        return HI_ERR_PVR_REC_INVALID_STATE;
    }

    pRecChn->enState = HI_UNF_PVR_REC_STATE_PAUSE;
    PVR_UNLOCK(&(pRecChn->stMutex));
    return HI_SUCCESS;
}

HI_S32 HI_PVR_RecResumeChn(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S  *pRecChn;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    /* to confirm the record channel running  */
    if ((HI_UNF_PVR_REC_STATE_RUNNING != pRecChn->enState)
        && (HI_UNF_PVR_REC_STATE_PAUSE != pRecChn->enState))
    {
        PVR_UNLOCK(&(pRecChn->stMutex));
        HI_ERR_PVR("Channel not started!\n");
        return HI_ERR_PVR_REC_INVALID_STATE;
    }

    pRecChn->enState = HI_UNF_PVR_REC_STATE_RUNNING;
    PVR_UNLOCK(&(pRecChn->stMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_RecGetStatus
 Description     : get record status and recorded file size
 Input           : u32ChnID    **
                   pRecStatus  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_RecGetStatus(HI_U32 u32ChnID, HI_UNF_PVR_REC_STATUS_S *pstRecStatus)
{
    HI_S32 ret;
    PVR_REC_CHN_S   *pRecChn;
    HI_UNF_PVR_FILE_ATTR_S fileAttr;
    HI_MPI_DMX_BUF_STATUS_S stStatus;

    CHECK_REC_CHNID(u32ChnID);
    PVR_CHECK_POINTER(pstRecStatus);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);

    if ((HI_UNF_PVR_REC_STATE_INIT == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_INVALID == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_STOPPING == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_STOP == pRecChn->enState)
        || (HI_UNF_PVR_REC_STATE_BUTT == pRecChn->enState)) /* not running, just return state */
    {
        memset(pstRecStatus, 0, sizeof(HI_UNF_PVR_REC_STATUS_S));
        pstRecStatus->enState = pRecChn->enState;
        PVR_UNLOCK(&(pRecChn->stMutex));
        return HI_SUCCESS;
    }

    /* get record state                                                        */
    pstRecStatus->enState = pRecChn->enState;

    ret = HI_MPI_DMX_GetRECBufferStatus(pRecChn->stUserCfg.u32DemuxID, &stStatus);
    if (HI_SUCCESS == ret)
    {
        pstRecStatus->stRecBufStatus.u32BufSize  = stStatus.u32BufSize;
        pstRecStatus->stRecBufStatus.u32UsedSize = stStatus.u32UsedSize;
    }
    else
    {
        pstRecStatus->stRecBufStatus.u32BufSize  = 0;
        pstRecStatus->stRecBufStatus.u32UsedSize = 0;
    }

    if (HI_NULL == pRecChn->IndexHandle)
    {
        pstRecStatus->u32CurTimeInMs   = 0;
        pstRecStatus->u32CurWriteFrame = 0;
        pstRecStatus->u64CurWritePos   = pRecChn->u64CurFileSize;
        pstRecStatus->u32StartTimeInMs = 0;
        pstRecStatus->u32EndTimeInMs = 0;
        PVR_UNLOCK(&(pRecChn->stMutex));

        return HI_SUCCESS;
    }

    /* get recorded file size                                                  */

    //pstRecStatus->u64CurWritePos = pRecChn->u64CurFileSize;

    ret = PVR_Index_PlayGetFileAttrByFileName(pRecChn->stUserCfg.szFileName, &fileAttr);
    if (HI_SUCCESS == ret)
    {
        pstRecStatus->u32CurTimeInMs   = fileAttr.u32EndTimeInMs - fileAttr.u32StartTimeInMs;
        pstRecStatus->u32CurWriteFrame = fileAttr.u32FrameNum;
        pstRecStatus->u64CurWritePos   = fileAttr.u64ValidSizeInByte;
        pstRecStatus->u32StartTimeInMs = fileAttr.u32StartTimeInMs;
        pstRecStatus->u32EndTimeInMs = fileAttr.u32EndTimeInMs;
    }

    PVR_UNLOCK(&(pRecChn->stMutex));

    return ret;
}

HI_S32 HI_PVR_RecRegisterWriteCallBack(HI_U32 u32ChnID, ExtraCallBack writeCallBack)
{
    PVR_REC_CHN_S   *pRecChn;

    CHECK_REC_CHNID(u32ChnID);
    PVR_CHECK_POINTER(writeCallBack);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);
    pRecChn->writeCallBack = writeCallBack;
    PVR_UNLOCK(&(pRecChn->stMutex));

    return HI_SUCCESS;
}

HI_S32 HI_PVR_RecUnRegisterWriteCallBack(HI_U32 u32ChnID)
{
    PVR_REC_CHN_S   *pRecChn;

    CHECK_REC_CHNID(u32ChnID);

    pRecChn = PVR_GET_RECPTR_BY_CHNID(u32ChnID);
    PVR_LOCK(&(pRecChn->stMutex));
    CHECK_REC_CHN_INIT_UNLOCK(pRecChn);
    pRecChn->writeCallBack = NULL;
    PVR_UNLOCK(&(pRecChn->stMutex));

    return HI_SUCCESS;
}

/*
 * suggesting, the user should set/get the user data by TS file name. as extend, also used by *.idx
 */
HI_S32 HI_PVR_SetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32UsrDataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Fd;
    HI_CHAR strIdxFileName[PVR_MAX_FILENAME_LEN + 5] = {0};

    PVR_CHECK_POINTER(pFileName);
    PVR_CHECK_POINTER(pInfo);

    if (0 == u32UsrDataLen)
    {
        return HI_SUCCESS;
    }

    PVR_Index_GetIdxFileName(strIdxFileName, (HI_CHAR*)pFileName);

    if (HI_FALSE == PVR_CHECK_FILE_EXIST(strIdxFileName))
    {
        HI_ERR_PVR("file:%s not exist.\n", strIdxFileName);
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    s32Fd = PVR_OPEN(strIdxFileName, PVR_FOPEN_MODE_INDEX_BOTH);
    if (s32Fd < 0)
    {
        HI_ERR_PVR("open file:%s fail:0x%x\n", strIdxFileName, s32Fd);
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    s32Ret = PVR_Index_SetUsrDataInfo(s32Fd, pInfo, u32UsrDataLen);
    if (s32Ret > 0)
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        HI_ERR_PVR("PVR_Index_SetUsrDataInfo fail\n");
    }

    PVR_CLOSE(s32Fd);
    return s32Ret;
}

HI_S32 HI_PVR_GetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* pUsrDataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Fd;
    HI_CHAR strIdxFileName[PVR_MAX_FILENAME_LEN + 5] = {0};

    PVR_CHECK_POINTER(pFileName);
    PVR_CHECK_POINTER(pInfo);
    PVR_CHECK_POINTER(pUsrDataLen);

    if (0 == u32BufLen)
    {
        return HI_SUCCESS;
    }

    PVR_Index_GetIdxFileName(strIdxFileName, (HI_CHAR*)pFileName);

    s32Fd = PVR_OPEN(strIdxFileName, PVR_FOPEN_MODE_INDEX_READ);
    if (s32Fd < 0)
    {
        HI_ERR_PVR("open file:%s fail:0x%x\n", strIdxFileName, s32Fd);
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    s32Ret = PVR_Index_GetUsrDataInfo(s32Fd, pInfo, u32BufLen);
    if (s32Ret > 0)
    {
        *pUsrDataLen = s32Ret;
        s32Ret = HI_SUCCESS;
    }
    else
    {
        *pUsrDataLen = 0;
        HI_ERR_PVR("PVR_Index_GetUsrDataInfo fail\n");
    }

    PVR_CLOSE(s32Fd);
    return s32Ret;
}

HI_S32 HI_PVR_SetCAData(const HI_CHAR *pIdxFileName, HI_U8 *pInfo, HI_U32 u32CADataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Fd;

    PVR_CHECK_POINTER(pIdxFileName);
    PVR_CHECK_POINTER(pInfo);

    if ((0 == u32CADataLen )||(PVR_MAX_CADATA_LEN < u32CADataLen))
    {
        HI_ERR_PVR("u32CADataLen (%d) invalid!\n",u32CADataLen);
        return HI_FAILURE;
    }

   //PVR_Index_GetIdxFileName(strIdxFileName, (HI_CHAR*)pFileName);

    s32Fd = PVR_OPEN(pIdxFileName, PVR_FOPEN_MODE_INDEX_BOTH);
    if (s32Fd < 0)
    {
        HI_ERR_PVR("open file:%s fail:0x%x\n", pIdxFileName, s32Fd);
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    s32Ret = PVR_Index_SetCADataInfo(s32Fd, pInfo, u32CADataLen);
    if (s32Ret > 0)
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        HI_ERR_PVR("PVR_Index_SetCADataInfo fail\n");
    }

    PVR_CLOSE(s32Fd);
    return s32Ret;
}

HI_S32 HI_PVR_GetCAData(const HI_CHAR *pIdxFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* u32CADataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32Fd;

    PVR_CHECK_POINTER(pIdxFileName);
    PVR_CHECK_POINTER(pInfo);
    PVR_CHECK_POINTER(u32CADataLen);

    if (!u32BufLen)
    {
        return HI_SUCCESS;
    }

    //PVR_Index_GetIdxFileName(strIdxFileName, (HI_CHAR*)pFileName);

    s32Fd = PVR_OPEN(pIdxFileName, PVR_FOPEN_MODE_INDEX_READ);
    if (s32Fd < 0)
    {
         HI_ERR_PVR("open file:%s fail:0x%x\n", pIdxFileName, s32Fd);
         return HI_ERR_PVR_FILE_CANT_OPEN;
    }

     s32Ret = PVR_Index_GetCADataInfo(s32Fd, pInfo, u32BufLen);
     if (s32Ret > 0)
     {
        *u32CADataLen = (HI_U32)s32Ret;
         s32Ret = HI_SUCCESS;
     }
     else
     {
         *u32CADataLen = 0;
         HI_ERR_PVR("PVR_Index_GetCADataInfo fail\n");
     }

     PVR_CLOSE(s32Fd);
     return s32Ret;
}
/*
 virtual a index file
 */
HI_S32 HI_PVR_CreateIdxFile2(const HI_CHAR* pstTsFileName, HI_CHAR* pstIdxFileName, HI_UNF_PVR_GEN_IDX_ATTR_S* pAttr)
{
#define FRAME_SIZE (188 * 1024)
    HI_U32 i;
    HI_U32 idxNum;
    HI_U64 tsSize, offset = 0;
    PVR_INDEX_ENTRY_S entry;
    HI_U32 dispSeq = 0;
    HI_U32 clk = 0;

    PVR_FILE fdIdxFile = 0;

    PVR_CHECK_POINTER(pstTsFileName);
    PVR_CHECK_POINTER(pstIdxFileName);
    if (pAttr)
    {
        ;
    }

    tsSize = PVR_FILE_GetFileSize(pstTsFileName);
    if (tsSize < 188)
    {
        HI_ERR_PVR("the ts file '%s' size is too small:%llu.\n", pstTsFileName, tsSize);
        return HI_ERR_PVR_INVALID_PARA;
    }

    /* open file, note the right */
    fdIdxFile = open(pstIdxFileName, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (-1 == fdIdxFile)
    {
        HI_ERR_PVR("can not open file '%s' for write.\n", pstIdxFileName);
        perror("can not create file:");
        return HI_ERR_PVR_FILE_CANT_OPEN;
    }

    idxNum = (HI_U32)tsSize / FRAME_SIZE;

    entry.u161stFrameOfTT = 0;
    entry.u16FrameTypeAndGop = 1;
    entry.u16IndexType  = HI_UNF_PVR_REC_INDEX_TYPE_VIDEO;
    entry.u16UpFlowFlag = 0;
    entry.u32FrameSize = FRAME_SIZE;
    entry.s32CycTimes = 0;
    entry.u64GlobalOffset = PVR_INDEX_INVALID_SEQHEAD_OFFSET;

    for (i = 0; i < idxNum; i++)
    {
        entry.u32DisplayTimeMs = dispSeq++;
        entry.u32PtsMs  = clk;
        entry.u64Offset = offset;

        clk    += 500;
        offset += FRAME_SIZE;

        write(fdIdxFile, &entry, sizeof(entry));
    }

    if (offset < tsSize)
    {
        entry.u32DisplayTimeMs = dispSeq++;
        entry.u32PtsMs  = clk;
        entry.u64Offset = offset;
        entry.u32FrameSize =(HI_U32)(tsSize - offset);

        write(fdIdxFile, &entry, sizeof(entry));
    }

    close(fdIdxFile);
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

