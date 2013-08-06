
#ifndef __HI_DRV_AO_EXT_H__
#define __HI_DRV_AO_EXT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef HI_S32 (*FN_AIAO_todofunc)(HI_VOID);

typedef struct 
{       
    FN_AIAO_todofunc pfnAiaotodofunc;         
} AIAO_EXPORT_FUNC_S;

HI_S32 AIAO_DRV_ModInit(HI_VOID);
HI_VOID AIAO_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif//__HI_DRV_AO_EXT_H__