
#ifndef __DRV_AI_IOCTL_H__
 #define __DRV_AI_IOCTL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_unf_ai.h"



typedef struct hiAI_Create_Param_S
{
 HI_UNF_AI_E              enAiPort;
 HI_UNF_AI_ATTR_S         stAttr;
 HI_HANDLE                hAi;
} AI_Create_Param_S, *AI_Create_Param_S_PTR;

 
/*AI Device command code*/
#define CMD_AI_GetDefaultAttr _IOWR  (HI_ID_AI, 0x00, HI_UNF_AI_ATTR_S)
#define CMD_AI_Create _IOWR  (HI_ID_AI, 0x01, AI_Create_Param_S)
#define CMD_AI_Destory _IOW  (HI_ID_AI, 0x02, HI_UNF_AI_ATTR_S)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif 
 
