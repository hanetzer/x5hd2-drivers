/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_disp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

*******************************************************************************/


#ifndef __MPI_AIAO_H__
#define __MPI_AIAO_H__

#include "hi_unf_sio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

HI_S32 HI_MPI_AI_SetPubAttr(HI_MPI_AUDIO_DEV AudioDevId, const HI_MPI_AIO_ATTR_S *pstAttr);
HI_S32 HI_MPI_AI_GetPubAttr(HI_MPI_AUDIO_DEV AudioDevId, HI_MPI_AIO_ATTR_S *pstAttr);
HI_S32 HI_MPI_AI_Enable(HI_MPI_AUDIO_DEV AiDevId);
HI_S32 HI_MPI_AI_Disable(HI_MPI_AUDIO_DEV AiDevId);
HI_S32 HI_MPI_AI_EnableChn(HI_MPI_AUDIO_DEV AiDevId, HI_MPI_AI_CHN AiChn);
HI_S32 HI_MPI_AI_DisableChn(HI_MPI_AUDIO_DEV AiDevId, HI_MPI_AI_CHN AiChn);
HI_S32 HI_MPI_AI_GetFrame(HI_MPI_AUDIO_DEV AiDevId, HI_MPI_AI_CHN AiChn, HI_MPI_AUDIO_FRAME_S *pstData, HI_MPI_AUDIO_FRAME_S *pstAecData, HI_U32 u32Block);
HI_S32 HI_MPI_AI_EnableAec(HI_MPI_AUDIO_DEV AiDevId, HI_MPI_AI_CHN AiChn, HI_MPI_AUDIO_DEV AoDevId, HI_MPI_AO_CHN AoChn);
HI_S32 HI_MPI_AI_DisableAec(HI_MPI_AUDIO_DEV AiDevId, HI_MPI_AI_CHN AiChn);

HI_S32 HI_MPI_AO_SetPubAttr(HI_MPI_AUDIO_DEV AudioDevId, const HI_MPI_AIO_ATTR_S *pstAttr);
HI_S32 HI_MPI_AO_GetPubAttr(HI_MPI_AUDIO_DEV AudioDevId ,HI_MPI_AIO_ATTR_S *pstAttr);
HI_S32 HI_MPI_AO_Enable(HI_MPI_AUDIO_DEV AoDevId);
HI_S32 HI_MPI_AO_Disable(HI_MPI_AUDIO_DEV AoDevId);
HI_S32 HI_MPI_AO_EnableChn(HI_MPI_AUDIO_DEV AoDevId, HI_MPI_AO_CHN AoChn);
HI_S32 HI_MPI_AO_DisableChn(HI_MPI_AUDIO_DEV AoDevId, HI_MPI_AO_CHN AoChn);
HI_S32 HI_MPI_AO_SendFrame(HI_MPI_AUDIO_DEV AoDevId , HI_MPI_AO_CHN AoChn, const HI_MPI_AUDIO_FRAME_S *pstData, HI_U32 u32Block);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

