/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:
* Description: driver aiao common header
*
* History:
* Version   Date         Author         DefectNum    Description
* main\1       AudioGroup     NULL         Create this file.
***********************************************************************************/
#ifndef __DRV_AI_COMMON_H__
#define __DRV_AI_COMMON_H__


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

//AI BUF ATTR
typedef struct hiAI_BUF_ATTR_S
{
    HI_U32      u32Start;
    HI_U32      u32Read;
    HI_U32      u32Write;
    HI_U32      u32End;
    /* user space virtual address */
    HI_U32 u32UserVirBaseAddr;
    /* kernel space virtual address */
    HI_U32 u32KernelVirBaseAddr;
    //TO DO
    //MMZ Handle
    
} AI_BUF_ATTR_S;

//AI
typedef struct hiAI_RESOURCE_S
{
    HI_U32                                   u32AIPortID;                     //AI Port ID
    //HI_UNF_AI_INPUTTYPE_E              enAIType;                         //AI Type
    AI_BUF_ATTR_S                      *pstAOPBufAttr;

} AI_RESOURCE_S;

//AI GLOABL RESOURCE 
typedef struct hiAI_GLOBAL_RESOURCE_S
{ 
    HI_U32                      u32BitFlag_AI;                              //resource usage such as  (1 << I2S | 1  << HDMI RX | 1 <<  ...)
    AI_RESOURCE_S       *pstAI_ATTR_S[AI_MAX_TOTAL_NUM];
    //to do
    
}AI_GLOBAL_RESOURCE_S;


/* private dev state Save AI Resource opened */
typedef struct hiAI_AOESTATE_S
{
    //ai
    HI_U32 *RecordId[AI_MAX_TOTAL_NUM];
    //todo
    
} AI_STATE_S;





#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif 
