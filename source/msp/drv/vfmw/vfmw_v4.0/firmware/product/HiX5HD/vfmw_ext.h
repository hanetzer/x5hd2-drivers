///////////////  软解  /////////////////
/*#include "syn_cmn.h"
#include "mem_manage.h"
#include "syntax.h"*/
///////////////  软解  /////////////////


typedef HI_VOID  (*FN_VFMW_OpenModule)(HI_VOID);
typedef HI_VOID  (*FN_VFMW_ExitModule)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Init)(HI_S32 (*VdecCallback)(HI_S32, HI_S32, HI_VOID*));
typedef HI_S32   (*FN_VFMW_InitWithOperation)(VDEC_OPERATION_S *);
typedef HI_S32   (*FN_VFMW_Control)(HI_S32, VDEC_CID_E, HI_VOID*);
typedef HI_S32   (*FN_VFMW_Exit)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Suspend)(HI_VOID);
typedef HI_S32   (*FN_VFMW_Resume)(HI_VOID);
typedef HI_S32   (*FN_VFMW_SetDbgOption)(HI_U32, HI_U8*);
typedef HI_VOID  (*FN_VFMW_VdmIntServProc)(HI_S32);
 

/* 软解码ko要使用如下函数 */
/*typedef HI_VOID  (*FN_VFMW_KlibFlushCache)(HI_VOID*, HI_U32, HI_U32);
typedef HI_VOID  (*FN_VFMW_KernelFlushCache)(HI_VOID*, HI_U32, HI_U32);
typedef HI_S32   (*FN_VFMW_MemMapRegisterAddr)(HI_S32, HI_S32, MEM_RECORD_S*);
typedef HI_S32   (*FN_VFMW_InsertImgToVoQueue)(HI_S32, VID_STD_E, HI_VOID*, IMAGE_VO_QUEUE*, IMAGE*);
typedef HI_S32   (*FN_VFMW_Dprint)(HI_U32, const HI_S8*, ...);
typedef HI_S32   (*FN_VFMW_VctrlGetChanIDByCtx)(HI_VOID*);
typedef HI_S32   (*FN_VFMW_MemVir2Phy)(HI_S8*);
typedef HI_VOID  (*FN_VFMW_ResetVoQueue)(IMAGE_VO_QUEUE *);*/



typedef struct
{
    FN_VFMW_OpenModule          pfnVfmwOpenModule;
    FN_VFMW_ExitModule          pfnVfmwExitModule;
    FN_VFMW_Init                pfnVfmwInit;
    FN_VFMW_InitWithOperation   pfnVfmwInitWithOperation;
    FN_VFMW_Control             pfnVfmwControl;
    FN_VFMW_Exit                pfnVfmwExit;
    FN_VFMW_Suspend             pfnVfmwSuspend;
    FN_VFMW_Resume              pfnVfmwResume;
    FN_VFMW_SetDbgOption        pfnVfmwSetDbgOption;
    FN_VFMW_VdmIntServProc      pfnVfmwVdmIntServProc;

    /* 软解码ko要使用如下函数 */
    /*FN_VFMW_KlibFlushCache        pfnVfmwKlibFlushCache;
    FN_VFMW_KernelFlushCache      pfnVfmwKernelFlushCache;
    FN_VFMW_MemMapRegisterAddr    pfnVfmwMemMapRegisterAddr;
    FN_VFMW_InsertImgToVoQueue    pfnVfmwInsertImgToVoQueue;
    FN_VFMW_Dprint                pfnVfmwDprint;
    FN_VFMW_VctrlGetChanIDByCtx   pfnVfmwVctrlGetChanIDByCtx;
    FN_VFMW_MemVir2Phy            pfnVfmwMemVir2Phy;
    FN_VFMW_ResetVoQueue          pfnVfmwResetVoQueue;*/

}   VFMW_EXPORT_FUNC_S;


