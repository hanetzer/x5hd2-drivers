#include "vpss_instance.h"
#include "vpss_alg.h"
#include "vpss_common.h"
#include "drv_stat_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_U32 VPSS_INST_AddEmptyImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstUndoImage)
{
    VPSS_IMAGELIST_INFO_S *pstImageList;

    pstImageList = &(pstInstance->stSrcImagesList);

    VPSS_OSAL_DownLock(&(pstImageList->stEmptyListLock));
    list_add_tail(&(pstUndoImage->node), &(pstImageList->stEmptyImageList));
    VPSS_OSAL_UpLock(&(pstImageList->stEmptyListLock));

    return HI_SUCCESS;
}
HI_S32 VPSS_INST_RelImageBuf(VPSS_INSTANCE_S *pstInstance,VPSS_IMAGE_NODE_S *pstDoneImageNode)
{
    HI_S32 s32Ret;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    
    pstImageList = &(pstInstance->stSrcImagesList);
    
    if(pstDoneImageNode->stSrcImage.bProgressive == HI_FALSE)
    {
        if(pstDoneImageNode->stSrcImage.enFieldMode == HI_DRV_FIELD_TOP
            && pstDoneImageNode->stSrcImage.bTopFieldFirst == HI_FALSE)
        {
            pstImageList->u32RelUsrTotal ++;
            s32Ret = pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE(pstInstance->ID,
                                        &(pstDoneImageNode->stSrcImage));
            if (s32Ret == HI_FAILURE)
            {
                pstImageList->u32RelUsrFailed++;
            }
        }

        if(pstDoneImageNode->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM
            && pstDoneImageNode->stSrcImage.bTopFieldFirst == HI_TRUE)
        {
            pstImageList->u32RelUsrTotal ++;
            if(pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE == HI_NULL)
            {
                VPSS_FATAL("\n VPSS_REL_SRCIMAGE HI_NULL \n");
            }
            s32Ret = pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE(pstInstance->ID,
                                        &(pstDoneImageNode->stSrcImage));
            if (s32Ret == HI_FAILURE)
            {
                pstImageList->u32RelUsrFailed++;
            }
        }
    }
    else
    {
        pstImageList->u32RelUsrTotal ++;
        s32Ret = pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE(pstInstance->ID,
                                        &(pstDoneImageNode->stSrcImage));
        if (s32Ret == HI_FAILURE)
        {
            pstImageList->u32RelUsrFailed++;
        }
        
    }
    
    VPSS_INST_AddEmptyImage(pstInstance,pstDoneImageNode);

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_RelDoneImage(VPSS_INSTANCE_S *pstInstance)
{
    /*
        根据pstInstance处理需要的IMG，释放掉不用的IMG
    */
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstDoneImageNode;
    HI_BOOL bProgressive;
    LIST *pos, *n, *head;
    
    pstImageList = &(pstInstance->stSrcImagesList);
    pstDoneImageNode = HI_NULL;
    bProgressive = HI_FALSE;

    #if 0
    {
        LIST *pos, *n;
        VPSS_IMAGE_NODE_S *pstCurFieldNode;
        printk("\n-----------------RelDone Start------------------\n");
        list_for_each_safe(pos, n, &(pstImageList->stFulImageList))
        {
            
            pstCurFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);

            printk("\nIn ID %d BT %d TopFirst %x",pstCurFieldNode->stSrcImage.u32FrameIndex,
                                pstCurFieldNode->stSrcImage.enFieldMode,
                                pstCurFieldNode->stSrcImage.bTopFieldFirst);
            
            if(pos == pstImageList->pstTarget_1)
            {
                printk("-->tar_1");
            }
        }
        printk("\n-----------------RelDone Down------------------\n");
    }
    #endif
    /*
        获取已处理场的逐隔行信息
        逐行:未依赖历史场信息，可以释放target_1场
        隔行:依赖了历史场信息，可以释放target_1->pre场
        逐隔行切换
     */
    if(pstImageList->pstTarget_1->next != &(pstImageList->stFulImageList))
    {
        pstDoneImageNode = list_entry((pstImageList->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
        bProgressive = pstDoneImageNode->stSrcImage.bProgressive;

    }
    else
    {
        VPSS_FATAL("\n RelDoneImage Error\n");
        return HI_FAILURE;
    }

    /*
       未打开DEI 每次处理时释放上一IMG
       需要考虑场序修正和逐隔行修正
     */
    pstDoneImageNode = HI_NULL;
    if (bProgressive == HI_TRUE)
    {
        head = &(pstImageList->stFulImageList);
            
        /*释放该帧前所有帧存*/
        for (pos = (head)->next, n = pos->next; pos != (pstImageList->pstTarget_1)->next; 
	        pos = n, n = pos->next)
	    {
            pstDoneImageNode = list_entry(pos,
                            VPSS_IMAGE_NODE_S, node);
            if (pos == pstImageList->pstTarget_1)
            {
                pstImageList->pstTarget_1 = pstImageList->pstTarget_1->prev;
            }
            list_del_init(&(pstDoneImageNode->node));
            VPSS_INST_RelImageBuf(pstInstance, pstDoneImageNode);
	    }
	    
        /*当前处理为逐行，则该帧前所有帧存可释放*/
        if(pstImageList->pstTarget_1->next != &(pstImageList->stFulImageList))
        {
		    /*释放刚刚处理帧*/
            pstDoneImageNode = list_entry((pstImageList->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
            
            list_del_init(&(pstDoneImageNode->node));
            VPSS_INST_RelImageBuf(pstInstance, pstDoneImageNode);
        }
        else
        {

        }
    }
    else
    {
        switch(pstInstance->stProcCtrl.eDEI)
        {
            /*
                    5场模式 一帧分为1,2两场
               */
            case HI_DRV_VPSS_DIE_5FIELD:
            case HI_DRV_VPSS_DIE_4FIELD:
            case HI_DRV_VPSS_DIE_3FIELD:
                /*
                    释放target_1前一场
                    */
                if(pstImageList->pstTarget_1 != &(pstImageList->stFulImageList))
                {
                    pstDoneImageNode = list_entry((pstImageList->pstTarget_1),
                                VPSS_IMAGE_NODE_S, node);
                    pstImageList->pstTarget_1 = pstImageList->pstTarget_1->prev;
                    list_del_init(&(pstDoneImageNode->node));
                    VPSS_INST_RelImageBuf(pstInstance, pstDoneImageNode);
                }
                else
                {
                    
                }
                break;
            default://4场模式
                
                break;
        }
    }
   
    return HI_SUCCESS;
}

/*
用户主动释放IMG的接口
*/
HI_S32 VPSS_INST_DelDoneImage(VPSS_INSTANCE_S *pstInstance,
                                HI_DRV_VIDEO_FRAME_S *pstImage)
{
    /*
        根据pstInstance处理需要的IMG，释放掉不用的IMG
    */
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstDoneImageNode;
    
    pstImageList = &(pstInstance->stSrcImagesList);

    /*
        目前简化策略是每次处理时释放最早处理完的帧
    */
    VPSS_OSAL_DownLock(&(pstImageList->stFulListLock));

    if((pstImageList->stFulImageList.next != pstImageList->pstTarget_1->next))
    {
        pstDoneImageNode = list_entry((pstImageList->stFulImageList.next),
                            VPSS_IMAGE_NODE_S, node);
        
        if(&(pstDoneImageNode->node) == pstImageList->pstTarget_1)
        {
            pstImageList->pstTarget_1 = pstImageList->pstTarget_1->prev;
        }
                    
        list_del_init(&(pstDoneImageNode->node));
        
        VPSS_OSAL_UpLock(&(pstImageList->stFulListLock));

        memcpy(pstImage,&(pstDoneImageNode->stSrcImage),sizeof(HI_DRV_VIDEO_FRAME_S));
        
        VPSS_INST_AddEmptyImage(pstInstance,pstDoneImageNode);
    }
    else
    {
        VPSS_OSAL_UpLock(&(pstImageList->stFulListLock));
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_INST_DelFulImage(VPSS_INSTANCE_S *pstInstance,
                            HI_DRV_VIDEO_FRAME_S *pstImage)
{
    VPSS_IMAGE_NODE_S *pstTarget;
    LIST *pos, *n, *head;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    HI_DRV_VIDEO_FRAME_S *pstFulImage;
    pstImageList = &(pstInstance->stSrcImagesList);

    head = &(pstImageList->stFulImageList);
    for (pos = (head)->next, n = pos->next; pos != (pstImageList->pstTarget_1)->next; 
		pos = n, n = pos->next)
    {
        pstTarget = list_entry(pos, VPSS_IMAGE_NODE_S, node);
        pstFulImage = &(pstTarget->stSrcImage);
        if (pstFulImage->stBufAddr[0].u32PhyAddr_Y
            == pstImage->stBufAddr[0].u32PhyAddr_Y)
        {
            if(pos != (pstImageList->pstTarget_1))
            {
                
            }
            else
            {
                pstImageList->pstTarget_1 = (pstImageList->pstTarget_1)->prev;
            }
            list_del_init(pos);
            VPSS_INST_AddEmptyImage(pstInstance,pstTarget);
            break;
        }
    }

    if (pos != (pstImageList->pstTarget_1)->next)
    {
        return HI_SUCCESS;
    }
    else
    {
        VPSS_FATAL("\n RelImg doesn't exit------------->func %s line %d \r\n",
                    __func__, __LINE__);
        return HI_FAILURE;
    }
}
/*生产者:USRCALLBACK*/
VPSS_IMAGE_NODE_S* VPSS_INST_GetEmptyImage(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_IMAGE_NODE_S *pstTarget;
    LIST *pos, *n;
    VPSS_IMAGELIST_INFO_S *pstImageList;

    pstImageList = &(pstInstance->stSrcImagesList);
    
    pstTarget = HI_NULL;

    VPSS_OSAL_DownLock(&(pstImageList->stEmptyListLock));
    list_for_each_safe(pos, n, &(pstImageList->stEmptyImageList))
    {
        pstTarget = list_entry(pos, VPSS_IMAGE_NODE_S, node);
        list_del_init(pos);
        break;
    }
    VPSS_OSAL_UpLock(&(pstImageList->stEmptyListLock));

    if(pstTarget)
    {
        memset(&(pstTarget->stSrcImage), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));
        return pstTarget;
    }
    else
    {
        return HI_NULL;
    }
}
HI_S32 VPSS_INST_AddFulImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstFulImage)
{
    VPSS_IMAGELIST_INFO_S *pstImageList;

    pstImageList = &(pstInstance->stSrcImagesList);

    VPSS_OSAL_DownLock(&(pstImageList->stFulListLock));
    list_add_tail(&(pstFulImage->node), &(pstImageList->stFulImageList));
    VPSS_OSAL_UpLock(&(pstImageList->stFulListLock));

    return HI_SUCCESS;
}


HI_S32 VPSS_INST_GetUserImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstSrcImage)
{
    HI_S32 s32Ret;
    HI_S32 hDst;
    PFN_VPSS_SRC_FUNC  pfUsrCallBack;
    HI_VOID *pstArgs;

    pfUsrCallBack = pstInstance->stSrcFuncs.VPSS_GET_SRCIMAGE;
    
    if(!pfUsrCallBack)
    {
        VPSS_FATAL("\n VPSS_GET_SRCIMAGE doesn't Exit------------->func %s line %d \r\n",
                    __func__, __LINE__);
        return HI_FAILURE;
    }

    hDst = pstInstance->hDst;
    
    pstArgs = &(pstSrcImage->stSrcImage);
    
    s32Ret = pfUsrCallBack(pstInstance->ID,pstArgs);

    return s32Ret;
}

VPSS_PORT_S *VPSS_INST_GetPort(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort)
{
    
    HI_U32 u32PortID;
    VPSS_PORT_S *pstPort;
    u32PortID = PORTHANDLE_TO_PORTID(hPort);
    pstPort = &(pstInstance->stPort[u32PortID]);

    if (pstPort->s32PortId == VPSS_INVALID_HANDLE)
    {
        VPSS_FATAL("\nPort doesn't Exit------------->func %s line %d \r\n",
                    __func__, __LINE__);
        
        pstPort = HI_NULL;
    }
    
    return pstPort;
}

HI_BOOL VPSS_INST_CheckImageList(VPSS_INSTANCE_S *pstInstance)
{
    HI_BOOL bReVal;
    VPSS_IMAGELIST_INFO_S* pstImgInfo;
    VPSS_IMAGE_NODE_S *pstImgNode;
    LIST *pre;
    LIST *cur;
    LIST *next1;
    LIST *next2;
    LIST *next3;
    pstImgInfo = &(pstInstance->stSrcImagesList);
    
    VPSS_OSAL_DownLock(&(pstImgInfo->stFulListLock));
    //有未处理IMAGE
    if ((pstImgInfo->pstTarget_1)->next != &(pstImgInfo->stFulImageList))
    {
        pstImgNode = list_entry((pstImgInfo->pstTarget_1->next),
                                VPSS_IMAGE_NODE_S, node);
        /*待处理帧为逐行*/
        if(pstInstance->stProcCtrl.eDEI == HI_DRV_VPSS_DIE_DISABLE
            || pstImgNode->stSrcImage.bProgressive == HI_TRUE)
        {
                bReVal = HI_TRUE;
        }
        /*待处理帧为隔行*/
        else
        {
            switch(pstInstance->stProcCtrl.eDEI)
            {
                case HI_DRV_VPSS_DIE_5FIELD:
                case HI_DRV_VPSS_DIE_4FIELD:
                case HI_DRV_VPSS_DIE_3FIELD:
                    pre = pstImgInfo->pstTarget_1;
                    if(pre == &(pstImgInfo->stFulImageList))
                    {
                            
                        bReVal = HI_FALSE;
                        if (pre->next != &(pstImgInfo->stFulImageList))
                        {
                            pstImgInfo->pstTarget_1 = pstImgInfo->pstTarget_1->next;
                            pre = pstImgInfo->pstTarget_1;
                        }
                        break;
                    }
                    cur = pre->next;
                    if(cur == &(pstImgInfo->stFulImageList))
                    {
                            bReVal = HI_FALSE;
                            break;
                    }
                    next1 = cur->next;
                    if(next1 == &(pstImgInfo->stFulImageList))
                    {
                            bReVal = HI_FALSE;
                            break;
                    }
                    next2 = next1->next;
                    if(next2 == &(pstImgInfo->stFulImageList))
                    {
                            bReVal = HI_FALSE;
                            break;
                    }
                    next3 = next2->next;
                    if(next3 == &(pstImgInfo->stFulImageList))
                    {
                             bReVal = HI_FALSE;
                            break;
                    }
                    
                    bReVal = HI_TRUE;
                    break;
                default:
                    bReVal = HI_FALSE;
                    break;
            }
        }
    }
    else
    {
            bReVal = HI_FALSE;
    }
    

    VPSS_OSAL_UpLock(&(pstImgInfo->stFulListLock));
    return bReVal;
}

HI_U32 u32Pts = 0;
HI_U32 u32PrePts = 0;
HI_S32 VPSS_INST_AddUndoImage(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_IMAGELIST_INFO_S* pstImgInfo;
    VPSS_IMAGE_NODE_S* pstEmpty1stImage;
    VPSS_IMAGE_NODE_S* pstEmpty2ndImage;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_S32 s32Ret;
    
    pstImgInfo = &(pstInstance->stSrcImagesList);
    if(pstInstance->eSrcImgMode == VPSS_SOURCE_MODE_VPSSACTIVE)
    {
        /*不管是逐行，隔行的待处理帧，获取两个帧存*/
        pstEmpty1stImage = VPSS_INST_GetEmptyImage(pstInstance);
        pstImgInfo->u32GetUsrTotal++;
        
        if(!pstEmpty1stImage)
        {
            VPSS_INFO("\n------%d-----NO EmptyImage",pstInstance->ID);
            
            return HI_FALSE;
        }
        pstEmpty2ndImage = VPSS_INST_GetEmptyImage(pstInstance);

        if(!pstEmpty2ndImage)
        {
            VPSS_INFO("\n------%d-----NO EmptyImage",pstInstance->ID);
            VPSS_INST_AddEmptyImage(pstInstance,pstEmpty1stImage);
            return HI_FALSE;
        }

        s32Ret = VPSS_INST_GetUserImage(pstInstance,pstEmpty1stImage);

        
        if (s32Ret != HI_SUCCESS)
        {
            VPSS_INFO("\n-------%d------NO UsrImage",pstInstance->ID);
            
            pstImgInfo->u32GetUsrFailed++;
            VPSS_INST_AddEmptyImage(pstInstance,pstEmpty1stImage);
            VPSS_INST_AddEmptyImage(pstInstance,pstEmpty2ndImage);
            return HI_FALSE;
        }

        if (pstInstance->stDbgCtrl.stInstDbg.bWriteYUV == HI_TRUE)
        {
            VPSS_OSAL_WRITEYUV(&(pstEmpty1stImage->stSrcImage), 
                                pstInstance->stDbgCtrl.stInstDbg.chFile);
            pstInstance->stDbgCtrl.stInstDbg.bWriteYUV = HI_FALSE;                  
        }
        //printk("Before InPro %d \n",pstEmpty1stImage->stSrcImage.bProgressive);

        if (pstEmpty1stImage->stSrcImage.bIsFirstIFrame)
        {
            HI_DRV_STAT_Event(STAT_EVENT_VPSSGETFRM, 0);
        }

        /*Revise image height to 4X*/
        pstEmpty1stImage->stSrcImage.u32Height = 
                pstEmpty1stImage->stSrcImage.u32Height & 0xfffffffc;       
		 /*3D TEST*/
		#if 0
        pstEmpty1stImage->stSrcImage.eFrmType = HI_DRV_FT_SBS;
        pstEmpty1stImage->stSrcImage.bProgressive = HI_FALSE;
        pstEmpty1stImage->stSrcImage.bTopFieldFirst = HI_TRUE;
        pstEmpty1stImage->stSrcImage.enFieldMode = HI_DRV_FIELD_ALL;
        /**************************************************/
        #endif
        
        /*
         * SBS TAB 处理一致 MVC透传
         * 
         */
        if(pstEmpty1stImage->stSrcImage.eFrmType == HI_DRV_FT_SBS)
        {
            HI_DRV_VIDEO_FRAME_S *pstImg;
            pstImg = &(pstEmpty1stImage->stSrcImage);
            memcpy(&(pstImg->stBufAddr[1]),&(pstImg->stBufAddr[0]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));
                        
            pstImg->stBufAddr[HI_DRV_BUF_ADDR_RIGHT].u32PhyAddr_Y = 
                    pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32PhyAddr_Y + pstImg->u32Width/2;
            pstImg->stBufAddr[HI_DRV_BUF_ADDR_RIGHT].u32PhyAddr_C = 
                    pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32PhyAddr_C + pstImg->u32Width/2;
        }

        if(pstEmpty1stImage->stSrcImage.eFrmType == HI_DRV_FT_TAB)
        {
            HI_DRV_VIDEO_FRAME_S *pstImg;
            pstImg = &(pstEmpty1stImage->stSrcImage);
            memcpy(&(pstImg->stBufAddr[1]),&(pstImg->stBufAddr[0]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));
                        
            pstImg->stBufAddr[HI_DRV_BUF_ADDR_RIGHT].u32PhyAddr_Y = 
                    pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32PhyAddr_Y 
                    + pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32Stride_Y * pstImg->u32Height/2;
            pstImg->stBufAddr[HI_DRV_BUF_ADDR_RIGHT].u32PhyAddr_C = 
                    pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32PhyAddr_C
                    + pstImg->stBufAddr[HI_DRV_BUF_ADDR_LEFT].u32Stride_C * pstImg->u32Height/4;
        }
        /**************************************************/
        
		VPSS_INST_CheckNeedRstDei(pstInstance,&(pstEmpty1stImage->stSrcImage));
		
        VPSS_INST_CorrectProgInfo(pstInstance,&(pstEmpty1stImage->stSrcImage));

        if (pstEmpty1stImage->stSrcImage.bIsFirstIFrame)
        {
            pstEmpty1stImage->stSrcImage.bProgressive = HI_TRUE;
        }
		
        VPSS_INFO("\n-----%d------get Usr img",pstInstance->ID);
        
        /*首次收帧时，相信码流自带顶底场优先信息*/
        if(pstInstance->u32RealTopFirst == 0xffffffff)
        {
            if(pstEmpty1stImage->stSrcImage.bProgressive == HI_TRUE)
            {
                pstInstance->u32RealTopFirst = 0xfffffffe;
            }
            else
            {
                pstInstance->u32RealTopFirst = 
                            pstEmpty1stImage->stSrcImage.bTopFieldFirst;
            }
        }
        else
        {
            /*
               *检出场序信息为逐行
               */
            if (pstInstance->u32RealTopFirst == 0xfffffffe)
            {
                if(pstEmpty1stImage->stSrcImage.bProgressive == HI_TRUE)
                {
                    
                }
                else
                {
                    pstInstance->u32RealTopFirst = 
                        pstEmpty1stImage->stSrcImage.bTopFieldFirst;
                }
            }
            /*
               *检出场序信息为隔行
               */
            else
            {
                if(pstEmpty1stImage->stSrcImage.bProgressive == HI_TRUE)
                {
                    pstInstance->u32RealTopFirst = 
                        pstEmpty1stImage->stSrcImage.bTopFieldFirst;
                }
                else
                {
                    pstEmpty1stImage->stSrcImage.bTopFieldFirst = 
                        pstInstance->u32RealTopFirst;
                }
            }
            
        }

        

        if (pstEmpty1stImage->stSrcImage.bProgressive == HI_FALSE
            && pstEmpty1stImage->stSrcImage.enFieldMode == HI_DRV_FIELD_ALL)
        {
            HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
            HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
            /*一个帧存存储的顶底两场信息，做帧信息拆分*/
            memcpy(&(pstEmpty2ndImage->stSrcImage),&(pstEmpty1stImage->stSrcImage),
                    sizeof(HI_DRV_VIDEO_FRAME_S));
            if(pstInstance->u32RealTopFirst == HI_TRUE)
            //if(pstEmpty1stImage->stSrcImage.bTopFieldFirst == HI_TRUE)
            {
                pstEmpty1stImage->stSrcImage.enFieldMode = HI_DRV_FIELD_TOP;
                
                pstEmpty2ndImage->stSrcImage.enFieldMode = HI_DRV_FIELD_BOTTOM;
            }
            else
            {
                pstEmpty1stImage->stSrcImage.enFieldMode = HI_DRV_FIELD_BOTTOM;
                
                pstEmpty2ndImage->stSrcImage.enFieldMode = HI_DRV_FIELD_TOP;
            }
            pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
            pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);

            pstEmpty2ndImage->stSrcImage.u32Pts = pstEmpty1stImage->stSrcImage.u32Pts
                                                 + pstVdecPriv->s32InterPtsDelta;
            pstEmpty1stImage->stSrcImage.u32FrameRate = 
                        pstEmpty1stImage->stSrcImage.u32FrameRate*2;
            pstEmpty2ndImage->stSrcImage.u32FrameRate = 
                        pstEmpty2ndImage->stSrcImage.u32FrameRate*2;

            VPSS_INST_ChangeInRate(pstInstance,pstEmpty1stImage->stSrcImage.u32FrameRate);
            /*
            printk("\n 1st PTS %d 2nd PTS %d  Rate %d\n",
                    pstEmpty1stImage->stSrcImage.u32Pts,
                    pstEmpty2ndImage->stSrcImage.u32Pts,
                    pstEmpty1stImage->stSrcImage.u32FrameRate);
                */
            /*****************VFMW 最后一帧适配方案*****************************/
            /*
                *当读取的帧信息带有最后一帧标记时候
                *逐行源:无影响
                *隔行源:改变倒数第二帧第二场的逐隔行信息，强送出去
                *    影响:送出三场隔行图,但传送给后级的是逐行信息
                */
            /**********************************************/
            pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
            
            if (pstPriv->u32LastFlag
                    == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
            {
                /*1.改变前一帧的第二场 逐隔行信息为逐行*/
                VPSS_IMAGE_NODE_S* pstPreNode;
                if(pstImgInfo->stFulImageList.prev
                    != &(pstImgInfo->stFulImageList))
                {
                    pstPreNode = list_entry((pstImgInfo->stFulImageList.prev),
                            VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;
                }
                else
                {
                    VPSS_FATAL("\nLast Field Error\n");
                }

                /*2.改变最后一帧两场的 逐隔行信息为逐行*/
                pstEmpty1stImage->stSrcImage.bProgressive = HI_TRUE;
                pstEmpty2ndImage->stSrcImage.bProgressive = HI_TRUE;
                /*3.将最后一帧标记打在第2场*/
                pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty1stImage->stSrcImage.u32Priv[0]);
                pstPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;

                pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstEmpty2ndImage->stSrcImage.u32Priv[0]);
                pstPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;

                s32Ret = VPSS_INST_AddFulImage(pstInstance,pstEmpty1stImage);  
            
                s32Ret = VPSS_INST_AddFulImage(pstInstance,pstEmpty2ndImage);  
            }
            else if(pstPriv->u32LastFlag
                    == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
            {
                VPSS_IMAGE_NODE_S* pstPreNode;
                LIST * pstPre;
                
                HI_BOOL bPreProg;
                pstPre = pstImgInfo->stFulImageList.prev;
                bPreProg = HI_FALSE;
				/*最后ERROR假帧默认给隔行，需要判定当前源是否在做隔行处理*/
                if (pstPre != &(pstImgInfo->stFulImageList))
                {
                    pstPreNode = list_entry(pstPre,
                            VPSS_IMAGE_NODE_S, node);
                    bPreProg = pstPreNode->stSrcImage.bProgressive;
                }

                if (bPreProg == HI_TRUE || pstPre == &(pstImgInfo->stFulImageList))
                {
                    
                }
                else
                {
                    if (pstPre == &(pstImgInfo->stFulImageList) 
                    || pstPre->prev == &(pstImgInfo->stFulImageList)
                    || pstPre->prev->prev == &(pstImgInfo->stFulImageList))
                    {
                        VPSS_FATAL("\nCan't Get pre three field\n");
                    }

                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;

                    pstPre = pstPre->prev;

                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;

                    pstPre = pstPre->prev;

                    pstPreNode = list_entry(pstPre,
                                VPSS_IMAGE_NODE_S, node);
                    pstPreNode->stSrcImage.bProgressive = HI_TRUE;
                }
                

                s32Ret = VPSS_INST_AddEmptyImage(pstInstance,pstEmpty1stImage);  
            
                s32Ret = VPSS_INST_AddEmptyImage(pstInstance,pstEmpty2ndImage);  
            }
            else
            {
                s32Ret = VPSS_INST_AddFulImage(pstInstance,pstEmpty1stImage);  
                
                s32Ret = VPSS_INST_AddFulImage(pstInstance,pstEmpty2ndImage);  
            }
            
        }
        else
        {
            s32Ret = VPSS_INST_AddFulImage(pstInstance,pstEmpty1stImage);
        
            VPSS_INST_AddEmptyImage(pstInstance,pstEmpty2ndImage);
        }
    }
    #if 0
    {
        LIST *pos, *n;
        VPSS_IMAGE_NODE_S *pstCurFieldNode;
        printk("\n-----------------List Start------------------\n");
        list_for_each_safe(pos, n, &(pstImgInfo->stFulImageList))
        {
            
            pstCurFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);

            printk("\nIn ID %d BT %d TopFirst %x",pstCurFieldNode->stSrcImage.u32FrameIndex,
                                pstCurFieldNode->stSrcImage.enFieldMode,
                                pstCurFieldNode->stSrcImage.bTopFieldFirst);
            
            if(pos == pstImgInfo->pstTarget_1)
            {
                printk("-->tar_1");
            }
        }
        printk("\n-----------------List Down------------------\n");
    }
    #endif
    return HI_SUCCESS;
}
HI_BOOL VPSS_INST_CheckUndoImage(VPSS_INSTANCE_S *pstInstance)
{
    //有未处理IMAGE
    HI_S32 s32Ret;
    if (VPSS_INST_CheckImageList(pstInstance) == HI_TRUE)
    {
        VPSS_INFO("\n-----%d------has undo img",pstInstance->ID);
        return HI_TRUE;
    }
    else
    {
        VPSS_INST_AddUndoImage(pstInstance);

        s32Ret = VPSS_INST_CheckImageList(pstInstance);
        return s32Ret;
    }
}


HI_S32 VPSS_INST_GetPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VPSS_PORT_S *pstPort;
    VPSS_FB_INFO_S *pstFrameList;
    HI_U32 u32PortID;
    HI_S32 s32Ret;
    HI_CHAR *pchFile = HI_NULL;
    
    u32PortID = PORTHANDLE_TO_PORTID(hPort);

    pstPort = HI_NULL;
    pstPort = VPSS_INST_GetPort(pstInstance,hPort); 
    
    if(!pstPort)
    {   
        return HI_FAILURE;
    }

    
    pstFrameList = &(pstPort->stFrmInfo);
    
    if(pstInstance->stDbgCtrl.stPortDbg[u32PortID].bWriteYUV == HI_TRUE)
    {   
        pchFile = pstInstance->stDbgCtrl.stPortDbg[u32PortID].chFile;
        pstInstance->stDbgCtrl.stPortDbg[u32PortID].bWriteYUV = HI_FALSE;
    }
    
    s32Ret = VPSS_FB_GetFulFrmBuf(pstFrameList,pstFrame,pchFile);
    
    return s32Ret;
}

HI_S32 VPSS_INST_RelPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VPSS_PORT_S *pstPort;
    VPSS_FB_INFO_S *pstFrameList;
    HI_S32 s32Ret;

    pstPort = HI_NULL;
    pstPort = VPSS_INST_GetPort(pstInstance,hPort);

    if(!pstPort)
    {
        return HI_FAILURE;
    }
    pstFrameList = &(pstPort->stFrmInfo);

    if (pstFrameList->stBufListCfg.eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE
        || pstFrameList->stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE)
    {
        
    }
    else
    {
        VPSS_FATAL("\n Buffer type don't support RelPortFrame");
        return HI_FAILURE;
    }
    s32Ret = VPSS_FB_RelFulFrmBuf(pstFrameList,pstFrame);

    return s32Ret;
}


HI_S32 VPSS_INST_GetDefInstCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    HI_DRV_VPSS_PROCESS_S *pstProcCtrl;
    
    pstVpssCfg->s32Priority = 0;

    pstVpssCfg->bAlwaysFlushSrc = HI_FALSE;

    pstProcCtrl = &(pstVpssCfg->stProcCtrl);

    pstProcCtrl->eACC = HI_DRV_VPSS_ACC_DISABLE;
    pstProcCtrl->eACM = HI_DRV_VPSS_ACM_DISABLE;
    pstProcCtrl->eDR = HI_DRV_VPSS_DR_AUTO;
    pstProcCtrl->eDB = HI_DRV_VPSS_DB_AUTO;
    pstProcCtrl->eHFlip = HI_DRV_VPSS_HFLIP_DISABLE;
    pstProcCtrl->eVFlip = HI_DRV_VPSS_VFLIP_DISABLE;
    pstProcCtrl->eCC = HI_DRV_VPSS_CC_AUTO;
    pstProcCtrl->eDEI = HI_DRV_VPSS_DIE_5FIELD;
    pstProcCtrl->eRotation = HI_DRV_VPSS_ROTATION_DISABLE;
    pstProcCtrl->eSharpness = HI_DRV_VPSS_SHARPNESS_AUTO;
    pstProcCtrl->eStereo = HI_DRV_VPSS_STEREO_DISABLE;
    
    pstProcCtrl->stInRect.s32X  = 0;
    pstProcCtrl->stInRect.s32Y  = 0;
    pstProcCtrl->stInRect.s32Height = 0;
    pstProcCtrl->stInRect.s32Width = 0;

    
    pstProcCtrl->bUseCropRect = HI_FALSE;
    pstProcCtrl->stCropRect.u32LeftOffset = 0;
    pstProcCtrl->stCropRect.u32RightOffset= 0;
    pstProcCtrl->stCropRect.u32TopOffset  = 0;
    pstProcCtrl->stCropRect.u32BottomOffset = 0;
    
    return HI_SUCCESS;
}
HI_S32 VPSS_INST_Init(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    HI_U32 u32Count;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstImageNode;
    HI_DRV_VPSS_CFG_S stTmpCfg;
    HI_S32 s32Ret;
    /*ID由VPSS_CTRL分配*/
    pstInstance->ID = 0;
    
    pstInstance->hDst = 0;
    pstInstance->pfUserCallBack = HI_NULL;

    pstInstance->eSrcImgMode = VPSS_SOURCE_MODE_BUTT;
    pstInstance->stSrcFuncs.VPSS_GET_SRCIMAGE = HI_NULL;
    pstInstance->stSrcFuncs.VPSS_REL_SRCIMAGE = HI_NULL;

    pstInstance->u32RealTopFirst = 0xffffffff;
    
    pstInstance->u32NeedRstDei = HI_FALSE;

	pstInstance->u32IsNewImage = HI_TRUE;
	
	pstInstance->u32Rwzb = 0;
	pstInstance->u32InRate = 0;
    pstImageList = &(pstInstance->stSrcImagesList);

    VPSS_OSAL_InitLOCK(&(pstImageList->stEmptyListLock), 1);
    VPSS_OSAL_InitLOCK(&(pstImageList->stFulListLock), 1);
    
    INIT_LIST_HEAD(&(pstImageList->stEmptyImageList));
    INIT_LIST_HEAD(&(pstImageList->stFulImageList));
    
    pstImageList->u32GetUsrTotal = 0;
    pstImageList->u32GetUsrFailed = 0;
    pstImageList->u32RelUsrTotal = 0;
    pstImageList->u32RelUsrFailed = 0;
    u32Count = 0;
    while(u32Count < VPSS_SOURCE_MAX_NUMB)
    {
        
        pstImageNode = (VPSS_IMAGE_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_IMAGE_NODE_S));
        
        memset(&(pstImageNode->stSrcImage), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));
        
        list_add_tail(&(pstImageNode->node), 
                        &(pstImageList->stEmptyImageList));
        u32Count++;
    }

    pstImageList->pstTarget_1 = &(pstImageList->stFulImageList);
    
    memset(&(pstInstance->stPort), 0, 
                sizeof(VPSS_PORT_S)*DEF_HI_DRV_VPSS_PORT_MAX_NUMBER);
    u32Count = 0;
    while(u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER)
    {
        pstInstance->stPort[u32Count].s32PortId = VPSS_INVALID_HANDLE;
        pstInstance->stPort[u32Count].bEnble = HI_FALSE;
        u32Count++;
    }
    
    
    VPSS_ALG_InitAuInfo((HI_U32)&(pstInstance->stAuInfo[0]));
    
    if (HI_NULL == pstVpssCfg)
    {
        VPSS_INST_GetDefInstCfg(&stTmpCfg);
        VPSS_INST_SetInstCfg(pstInstance,&stTmpCfg);
    }
    else
    {
        VPSS_INST_SetInstCfg(pstInstance,pstVpssCfg);
    }
    s32Ret = VPSS_INST_SyncUsrCfg(pstInstance);

    if(s32Ret == HI_FAILURE)
    {
        VPSS_FATAL("\n------------------Init Sync VPSS CFG Fail\n");
    }
    else
    {
    
    }
    VPSS_DBG_DbgInit(&(pstInstance->stDbgCtrl));
    return HI_SUCCESS;
}

HI_S32 VPSS_INST_DelInit(VPSS_INSTANCE_S *pstInstance)
{

    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_IMAGELIST_INFO_S*  pstImgList;
    LIST *pos, *n;
    VPSS_IMAGE_NODE_S* pstImgNode;
    /*
    销毁SrcImage占用的空间
    */
    pstImgList = &(pstInstance->stSrcImagesList);

    u32Count = 0;
    list_for_each_safe(pos, n, &(pstImgList->stEmptyImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_VFREE(pstImgNode);
        u32Count++;
    }
    
    list_for_each_safe(pos, n, &(pstImgList->stFulImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_VFREE(pstImgNode);
        u32Count++;
    }

    if(u32Count != VPSS_SOURCE_MAX_NUMB)
    {
        VPSS_FATAL("\n##############Inst %d free SrcImage Error %d",
                        pstInstance->ID,
                        u32Count);
    }
    /*
    销毁各PORT开辟的空间
    */
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId != VPSS_INVALID_HANDLE)
        {
            VPSS_INST_DestoryPort(pstInstance,pstPort->s32PortId);

            pstPort->s32PortId = VPSS_INVALID_HANDLE;
        }
    }
    
    VPSS_ALG_DeInitAuInfo((HI_U32)&(pstInstance->stAuInfo));
    
    return HI_SUCCESS;
}


HI_S32 VPSS_INST_SetInstCfg(VPSS_INSTANCE_S *pstInstance,
                                HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    //HI_RECT_S *pstInstInCrop;
    //HI_RECT_S *pstCfgInCrop;
    //HI_DRV_CROP_RECT_S *pstInstUsrCrop;
    //HI_DRV_CROP_RECT_S *pstCfgUsrCrop;
    HI_DRV_VPSS_CFG_S *pstInstUsrcCfg;
    
    pstInstUsrcCfg = &(pstInstance->stUsrInstCfg);

    
    VPSS_OSAL_DownSpin(&(pstInstance->stUsrSetSpin));
    
    pstInstance->u32IsNewCfg = HI_TRUE;
    pstInstUsrcCfg->bAlwaysFlushSrc = pstVpssCfg->bAlwaysFlushSrc;
    pstInstUsrcCfg->s32Priority = pstVpssCfg->s32Priority;
    pstInstUsrcCfg->stProcCtrl = pstVpssCfg->stProcCtrl;
    VPSS_OSAL_UpSpin(&(pstInstance->stUsrSetSpin));
    
    return HI_SUCCESS;
}


HI_U32 VPSS_INST_GetInstCfg(VPSS_INSTANCE_S *pstInstance,
                                HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    pstVpssCfg->s32Priority = pstInstance->s32Priority;
    pstVpssCfg->bAlwaysFlushSrc = pstInstance->bAlwaysFlushSrc;

    memcpy(&(pstVpssCfg->stProcCtrl),&(pstInstance->stProcCtrl),
                                        sizeof(HI_DRV_VPSS_PROCESS_S));

    return HI_SUCCESS;
}

HI_U32 VPSS_INST_GetDefPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstPortCfg)
{
    memset(pstPortCfg,0,sizeof(HI_DRV_VPSS_PORT_CFG_S));
    
    pstPortCfg->bTunnelEnable = HI_FALSE;
    pstPortCfg->s32SafeThr = 100;
    
    //pstPortCfg->s32OutputWidth = 720;
    //pstPortCfg->s32OutputHeight = 576;

    pstPortCfg->s32OutputWidth = 0;
    pstPortCfg->s32OutputHeight = 0;

    pstPortCfg->stScreen.s32Height = 576;
    pstPortCfg->stScreen.s32Width = 720;
    pstPortCfg->stScreen.s32X = 0;
    pstPortCfg->stScreen.s32Y = 0;
    pstPortCfg->stDispPixAR.u8ARh = 0;
    pstPortCfg->stDispPixAR.u8ARw = 0;
    pstPortCfg->stCustmAR.u8ARh = 0;
    pstPortCfg->stCustmAR.u8ARw = 0;
    
    pstPortCfg->eDstCS = HI_DRV_CS_BUTT;
    pstPortCfg->eAspMode  = HI_DRV_ASP_RAT_MODE_BUTT;
    pstPortCfg->stProcCtrl.eCSC = HI_DRV_VPSS_CSC_AUTO;
    pstPortCfg->stProcCtrl.eFidelity = HI_DRV_VPSS_FIDELITY_DISABLE;

    pstPortCfg->eFormat = HI_DRV_PIX_FMT_NV21;
    pstPortCfg->u32MaxFrameRate = 60;

    pstPortCfg->stBufListCfg.eBufType = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
    pstPortCfg->stBufListCfg.u32BufNumber = 10;

    //pstPortCfg->stBufListCfg.u32BufSize = 720*576*2;
    //pstPortCfg->stBufListCfg.u32BufStride = 720;

    pstPortCfg->stBufListCfg.u32BufSize = 1920*1080*2;
    pstPortCfg->stBufListCfg.u32BufStride = 1920;

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_CreatePort(VPSS_INSTANCE_S *pstInstance,
                                HI_DRV_VPSS_PORT_CFG_S *pstPortCfg,
                                VPSS_HANDLE *phPort)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    HI_S32 s32Ret;
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId == VPSS_INVALID_HANDLE)
        {
            break;
        }
    }
    if (u32Count == DEF_HI_DRV_VPSS_PORT_MAX_NUMBER)
    {
        VPSS_FATAL("\nPort Number is MAX------------->func %s line %d \r\n",
                    __func__, __LINE__);

        *phPort = 0;
        return HI_FAILURE;
    }
    else
    {
        HI_DRV_VPSS_PORT_CFG_S stPortDefCfg;
        HI_DRV_VPSS_PORT_CFG_S *pstPortSetCfg;
        memset(pstPort,0,sizeof(VPSS_PORT_S));
        
        if(pstPortCfg == HI_NULL)
        {
            VPSS_INST_GetDefPortCfg(&(stPortDefCfg));
            pstPortSetCfg = &(stPortDefCfg);
        }
        else
        {
            pstPortSetCfg = pstPortCfg;
        }
        pstPort->bEnble = HI_FALSE;
        pstPort->s32PortId = (pstInstance->ID * 256) + u32Count;
        
        pstPort->u32OutCount = 0;
        s32Ret = VPSS_FB_Init(&(pstPort->stFrmInfo),&(pstPortSetCfg->stBufListCfg));
        if (s32Ret == HI_FAILURE)
        {
            return HI_FAILURE;
        }
        VPSS_INST_SetPortCfg(pstInstance,pstPort->s32PortId,pstPortSetCfg);

        s32Ret = VPSS_INST_SyncUsrCfg(pstInstance);
        if (s32Ret == HI_FAILURE)
        {
            VPSS_FATAL("Create Port SyncCfg Fail.\n");
            return HI_FAILURE;
        }
        *phPort = pstPort->s32PortId;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_DestoryPort(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort)
{
    VPSS_PORT_S *pstPort;
    HI_U32 u32PortID;

    u32PortID = PORTHANDLE_TO_PORTID(hPort);

    pstPort = HI_NULL;

    pstPort = VPSS_INST_GetPort(pstInstance,hPort);
    if(!pstPort)
    {
        return HI_FAILURE;
    }
    
    VPSS_FB_DelInit(&(pstPort->stFrmInfo));

    memset(pstPort,0,sizeof(VPSS_PORT_S));

    pstPort->s32PortId = VPSS_INVALID_HANDLE;
    
    return HI_SUCCESS;

}
HI_S32 VPSS_INST_GetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, 
                                HI_DRV_VPSS_PORT_CFG_S *pstPortCfg)
{
    VPSS_PORT_S *pstPort;

    pstPort = HI_NULL;
    pstPort = VPSS_INST_GetPort(pstInstance,hPort);

    if(!pstPort)
    {
        return HI_FAILURE;
    }

    pstPortCfg->eFormat = pstPort->eFormat; 
    pstPortCfg->s32OutputWidth = pstPort->s32OutputWidth;
    pstPortCfg->s32OutputHeight = pstPort->s32OutputHeight;
    pstPortCfg->eDstCS = pstPort->eDstCS;
    pstPortCfg->stDispPixAR = pstPort->stDispPixAR;
    pstPortCfg->eAspMode = pstPort->eAspMode;
    pstPortCfg->bTunnelEnable = pstPort->bTunnelEnable;
    pstPortCfg->s32SafeThr = pstPort->s32SafeThr;   
    pstPortCfg->u32MaxFrameRate = pstPort->u32MaxFrameRate; 

    memcpy(&(pstPortCfg->stProcCtrl),&(pstPort->stProcCtrl),
                                        sizeof(HI_DRV_VPSS_PORT_PROCESS_S));
    
    memcpy(&(pstPortCfg->stBufListCfg),&((pstPort->stFrmInfo).stBufListCfg),
                                        sizeof(HI_DRV_VPSS_BUFLIST_CFG_S));

    return HI_SUCCESS;
}
HI_S32 VPSS_INST_SetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, 
                                HI_DRV_VPSS_PORT_CFG_S *pstPortCfg)
{
    /*
        Set操作分为两步
        UsrSet
        SyncCfg
     */
     
    VPSS_PORT_S *pstPort;
    HI_DRV_VPSS_PORT_CFG_S *pstUsrPortCfg;
    HI_U32 u32PortID;
    pstPort = HI_NULL;

    pstPort = VPSS_INST_GetPort(pstInstance,hPort);
    if(!pstPort)
    {
        return HI_FAILURE;
    }

    VPSS_OSAL_DownSpin(&(pstInstance->stUsrSetSpin));
    
    u32PortID = PORTHANDLE_TO_PORTID(hPort);
    
    pstInstance->u32IsNewCfg = HI_TRUE;

    pstUsrPortCfg = &(pstInstance->stUsrPortCfg[u32PortID]);
    pstUsrPortCfg->bInterlaced = pstPortCfg->bInterlaced;
    
    pstUsrPortCfg->bTunnelEnable = pstPortCfg->bTunnelEnable;
    
    pstUsrPortCfg->eAspMode = pstPortCfg->eAspMode;
    
    pstUsrPortCfg->bInterlaced = pstPortCfg->bInterlaced;
    pstUsrPortCfg->stScreen = pstPortCfg->stScreen;
    pstUsrPortCfg->stDispPixAR = pstPortCfg->stDispPixAR;
    pstUsrPortCfg->stCustmAR = pstPortCfg->stCustmAR;
    
    pstUsrPortCfg->eDstCS = pstPortCfg->eDstCS;
    
    pstUsrPortCfg->s32OutputWidth = pstPortCfg->s32OutputWidth;
    pstUsrPortCfg->s32OutputHeight = pstPortCfg->s32OutputHeight;
    pstUsrPortCfg->eFormat = pstPortCfg->eFormat;
    pstUsrPortCfg->u32MaxFrameRate = pstPortCfg->u32MaxFrameRate;
    pstUsrPortCfg->bTunnelEnable = pstPortCfg->bTunnelEnable;
    pstUsrPortCfg->s32SafeThr = pstPortCfg->s32SafeThr;
    pstUsrPortCfg->stBufListCfg = pstPortCfg->stBufListCfg;
    pstUsrPortCfg->stProcCtrl = pstPortCfg->stProcCtrl;
    
    VPSS_OSAL_UpSpin(&(pstInstance->stUsrSetSpin));
    #if 0
    pstPort->eFormat = pstPortCfg->eFormat; 
    pstPort->s32OutputWidth = pstPortCfg->s32OutputWidth;
    pstPort->s32OutputHeight = pstPortCfg->s32OutputHeight;
    pstPort->eDstCS = pstPortCfg->eDstCS;
    pstPort->stDispPixAR = pstPortCfg->stDispPixAR;
    pstPort->eAspMode = pstPortCfg->eAspMode;
    pstPort->bTunnelEnable = pstPortCfg->bTunnelEnable;
    pstPort->s32SafeThr = pstPortCfg->s32SafeThr;   
    pstPort->u32MaxFrameRate = pstPortCfg->u32MaxFrameRate; 

    memcpy(&(pstPort->stProcCtrl),&(pstPortCfg->stProcCtrl),
                                        sizeof(HI_DRV_VPSS_PORT_PROCESS_S));
    #endif                                    
    /*输出PORT的BUFFER配置需要创建时指定，不可改变*/
    /*
    memcpy(&((pstPort->stFrmInfo).stBufListCfg),&(pstPortCfg->stBufListCfg),
                                        sizeof(HI_DRV_VPSS_BUFLIST_CFG_S));
    */
    return HI_SUCCESS;
}
HI_S32 VPSS_INST_EnablePort(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,HI_BOOL bEnPort)
{
    VPSS_PORT_S *pstPort;

    pstPort = HI_NULL;

    pstPort = VPSS_INST_GetPort(pstInstance,hPort);
    
    if(!pstPort)
    {
        return HI_FAILURE;
    }
    /*
        使能的条件在此加入
    */
    pstPort->bEnble = bEnPort;
     
    return HI_SUCCESS;
}

HI_S32 VPSS_INST_CheckPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,
                                HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg)
{
    VPSS_PORT_S *pstPort;
    HI_DRV_VPSS_PORT_PROCESS_S *pstProcCtrl;
    pstPort = HI_NULL;

    pstPort = VPSS_INST_GetPort(pstInstance,hPort);
    
    if(!pstPort)
    {
        return HI_FAILURE;
    }
	
    pstProcCtrl = &(pstVpssPortCfg->stProcCtrl);

    if(pstVpssPortCfg->eFormat == HI_DRV_PIX_FMT_NV12 
		|| pstVpssPortCfg->eFormat == HI_DRV_PIX_FMT_NV16
		|| pstVpssPortCfg->eFormat == HI_DRV_PIX_FMT_NV21
		|| pstVpssPortCfg->eFormat == HI_DRV_PIX_FMT_NV61)
    {
        
    }
	else
	{
		return HI_FAILURE;
	}
    
    return HI_SUCCESS;
}
HI_S32 VPSS_INST_SetCallBack(VPSS_INSTANCE_S *pstInstance,
                            HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback)
{
    pstInstance->hDst = hDst;
    if(pfVpssCallback != HI_NULL)
    {   
        pstInstance->pfUserCallBack = pfVpssCallback;

        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
    
}

HI_S32 VPSS_INST_ReplyUserCommand(VPSS_INSTANCE_S * pstInstance,
                                    HI_DRV_VPSS_USER_COMMAND_E eCommand,
                                    HI_VOID *pArgs)
{
    HI_BOOL *pbAllDone;
    switch ( eCommand )
    {
        case HI_DRV_VPSS_USER_COMMAND_RESET:
            VPSS_INST_Reset(pstInstance);
            break;
        case HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE:
            pbAllDone = (HI_BOOL *)pArgs;
            *pbAllDone = VPSS_INST_CheckAllDone(pstInstance);
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_ResetPort(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort)
{
    VPSS_PORT_S *pstPort;
    HI_U32 u32PortID;

    u32PortID = PORTHANDLE_TO_PORTID(hPort);

    pstPort = HI_NULL;

    pstPort = VPSS_INST_GetPort(pstInstance,hPort);
    if(!pstPort)
    {
        return HI_FAILURE;
    }
    pstPort->u32OutCount = 0;
    VPSS_FB_Reset(&(pstPort->stFrmInfo));
    
    return HI_SUCCESS;
    
}
HI_S32 VPSS_INST_Reset(VPSS_INSTANCE_S *pstInstance)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_IMAGELIST_INFO_S*  pstImgList;
    LIST *pos, *n;
    VPSS_IMAGE_NODE_S* pstImgNode;

    VPSS_OSAL_DownLock(&(pstInstance->stInstLock));

    /*重置输出image队列*/
    pstImgList = &(pstInstance->stSrcImagesList);
    
    list_for_each_safe(pos, n, &(pstImgList->stFulImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);

        list_del_init(pos);

        VPSS_INST_AddEmptyImage(pstInstance,pstImgNode);
    }

    pstImgList->pstTarget_1 = &(pstImgList->stFulImageList);

    /*重置port*/
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId != VPSS_INVALID_HANDLE)
        {
            VPSS_INST_ResetPort(pstInstance,pstPort->s32PortId);
        }
    }
    VPSS_OSAL_UpLock(&(pstInstance->stInstLock));

	return HI_SUCCESS;
}
HI_BOOL VPSS_INST_CheckIsAvailable(VPSS_INSTANCE_S *pstInstance)
{
    HI_U32 u32Count;
    HI_U32 u32BufIsEnough;
    PFN_VPSS_CALLBACK pfUserCallBack;
    HI_S32 hDst;
    HI_DRV_VPSS_BUFFUL_STRATAGY_E eBufStratagy;
    VPSS_FB_INFO_S * pstFrameList;
    HI_U32 u32HasEnablePort;

    /*
     检测三路PORT情况
     */
    u32HasEnablePort = 0;
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
            u32Count ++)
    {
        if (pstInstance->stPort[u32Count].s32PortId != VPSS_INVALID_HANDLE
            && pstInstance->stPort[u32Count].bEnble == HI_TRUE)
        {
            u32HasEnablePort = 1;
        }
    }
    if (u32HasEnablePort == 0)
    {
        return HI_FALSE;
    }
    
    /*
     获取待处理image
     */
    if(!VPSS_INST_CheckUndoImage(pstInstance))
    {
        return HI_FALSE;
    }

    u32BufIsEnough = 0;
    u32HasEnablePort = 0;
    /*
    保证打开的通道有足够BUF
    */
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
            u32Count ++)
    {       
        if (pstInstance->stPort[u32Count].s32PortId != VPSS_INVALID_HANDLE
            && pstInstance->stPort[u32Count].bEnble == HI_TRUE)
        {
            u32HasEnablePort = 1;
            u32BufIsEnough = u32BufIsEnough - 1;
            pstFrameList = &((pstInstance->stPort[u32Count]).stFrmInfo);
            if(VPSS_FB_CheckIsAvailable(pstFrameList))
            {
                u32BufIsEnough = u32BufIsEnough + 1;
            }
            else
            {
                
            }
        }
        
    }

    if(u32HasEnablePort == 0)
    {
        return HI_FALSE;
    }
    if (u32BufIsEnough != 0)
    {
        if(pstInstance->pfUserCallBack == HI_NULL)
        {
            VPSS_FATAL("\n------%d------UserCallBack is NULL------",pstInstance->ID);
            return HI_FALSE;
        }
        pfUserCallBack = pstInstance->pfUserCallBack;
        hDst = pstInstance->hDst;
        eBufStratagy = HI_DRV_VPSS_BUFFUL_BUTT;
        pfUserCallBack(hDst, VPSS_EVENT_BUFLIST_FULL, &eBufStratagy);
        if(eBufStratagy == HI_DRV_VPSS_BUFFUL_PAUSE 
            || eBufStratagy == HI_DRV_VPSS_BUFFUL_BUTT)
        {
            VPSS_INFO("\n------%d------OUT Buf Is FULL------",pstInstance->ID);
            return HI_FALSE;
        }
    }
    return HI_TRUE;
    
}


HI_DRV_VIDEO_FRAME_S *VPSS_INST_GetUndoImage(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_IMAGE_NODE_S *pstImgNode;
    VPSS_IMAGELIST_INFO_S *pstImgListInfo;
    //HI_DRV_VIDEO_FRAME_S *pstImage;
    pstImgListInfo = &(pstInstance->stSrcImagesList);
    

    VPSS_OSAL_DownLock(&(pstImgListInfo->stFulListLock));
    if((pstImgListInfo->pstTarget_1)->next != &(pstImgListInfo->stFulImageList))
    {
        pstImgNode = list_entry((pstImgListInfo->pstTarget_1)->next, VPSS_IMAGE_NODE_S, node);

        VPSS_OSAL_UpLock(&(pstImgListInfo->stFulListLock));
        
                    
        return &(pstImgNode->stSrcImage);
    }
    else
    {
        VPSS_FATAL("\t\nWrong VPSS_INST_GetUndoImage");
        VPSS_OSAL_UpLock(&(pstImgListInfo->stFulListLock));
        return HI_NULL;
    }
       
}

HI_U32 u32LastPts;
HI_U32 u32Tmp;
HI_S32 VPSS_INST_CompleteUndoImage(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_IMAGELIST_INFO_S* pstImgInfo;
    
    pstImgInfo = &(pstInstance->stSrcImagesList);

    //有未处理IMAGE
    VPSS_OSAL_DownLock(&(pstImgInfo->stFulListLock));

    if ((pstImgInfo->pstTarget_1)->next != &(pstImgInfo->stFulImageList))
    {
        pstImgInfo->pstTarget_1 = (pstImgInfo->pstTarget_1)->next;
    }
    else
    {
        //VPSS_FATAL("\t\n CompleteUndoImage VPSS_INST_CompleteUndoImage Wrong");
    }
    
    VPSS_OSAL_UpLock(&(pstImgInfo->stFulListLock));
    return HI_SUCCESS;
}

HI_S32 VPSS_INST_GetSrcListState(VPSS_INSTANCE_S* pstInstance,VPSS_IMAGELIST_STATE_S *pstListState)
{
    HI_U32 u32Count;
    HI_U32 u32Total;
    HI_U32 u32DoneFlag;
    VPSS_IMAGELIST_INFO_S *pstImgList;
    VPSS_IMAGE_NODE_S *pstImgNode;
    LIST *pos, *n;
    if(pstInstance == HI_NULL)
    {
        VPSS_FATAL("\n pstInstance is NULL");
        return HI_FAILURE;
    }
    pstImgList = &(pstInstance->stSrcImagesList);
    
    pstListState->u32Target = (HI_U32)list_entry(pstImgList->pstTarget_1, VPSS_IMAGE_NODE_S, node);
    if (pstImgList->pstTarget_1 == &(pstImgList->stFulImageList))
    {
        u32DoneFlag = 2;
    }
    else
    {
        u32DoneFlag = 1;
    }
    u32Count = 0;
    u32Total = 0;
    list_for_each_safe(pos, n, &(pstImgList->stFulImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);
        
        pstListState->u32FulList[u32Count] = (HI_U32)pstImgNode;
        pstListState->u32List[u32Total][0] = pstImgNode->stSrcImage.u32FrameIndex;
        pstListState->u32List[u32Total][1] = u32DoneFlag;

        if (pstListState->u32Target == (HI_U32)pstImgNode)
        {
            u32DoneFlag = 2;
        }
        u32Count++;
        u32Total++;
        
    }
    list_for_each_safe(pos, n, &(pstImgList->stEmptyImageList))
    {
        pstImgNode = list_entry(pos, VPSS_IMAGE_NODE_S, node);
        pstListState->u32EmptyList[u32Count] = (HI_U32)pstImgNode;

        pstListState->u32List[u32Total][0] = -1;
        pstListState->u32List[u32Total][1] = 0;
        u32Count++;
        u32Total++;
    }
    pstListState->u32EmptyListNumb = u32Count;
    
    u32Count = 0;
    
    
    if (u32Total != VPSS_SOURCE_MAX_NUMB)
    {
        VPSS_FATAL("SrcList Proc Error.\n");
    }
    pstListState->u32FulListNumb = u32Count;

    pstListState->u32TotalNumb = pstListState->u32EmptyListNumb
                                + pstListState->u32FulListNumb;
    
    pstListState->u32GetUsrTotal = pstImgList->u32GetUsrTotal;
    pstListState->u32GetUsrFailed = pstImgList->u32GetUsrFailed;
    pstListState->u32RelUsrTotal = pstImgList->u32RelUsrTotal;
    pstListState->u32RelUsrFailed = pstImgList->u32RelUsrFailed;
    return HI_SUCCESS;                            
}

HI_S32 VPSS_INST_GetPortListState(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstListState)
{
    VPSS_PORT_S *pstPort;
    VPSS_FB_INFO_S *pstFrameList;
    VPSS_FB_STATE_S stFbState;
    HI_S32 s32Ret;

    pstPort = HI_NULL;
    pstPort = VPSS_INST_GetPort(pstInstance,hPort); 
    
    if(!pstPort)
    {   
        return HI_FAILURE;
    }
    pstFrameList = &(pstPort->stFrmInfo);

    s32Ret = VPSS_FB_GetState(pstFrameList, &(stFbState));

    if(HI_SUCCESS == s32Ret)
    {
        pstListState->u32TotalBufNumber = stFbState.u32TotalNumb;
        pstListState->u32FulBufNumber = stFbState.u32FulListNumb;
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_INST_GetFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,
                    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg,VPSS_BUFFER_S *pstBuffer,
                    HI_U32 u32StoreH,HI_U32 u32StoreW)
{
    MMZ_BUFFER_S *pstMMZBuf;
    HI_S32 s32Ret;
    HI_DRV_VPSS_BUFFER_TYPE_E eBufferType;
    HI_DRV_VPSS_FRMBUF_S stFrmBuf;
    HI_U32 u32BufSize;   

    if(pstInstance == HI_NULL)
    {
        VPSS_FATAL("\n pstInstance is NULL");
        return HI_FAILURE;
    }
    eBufferType = pstBufCfg->eBufType;
    u32BufSize = pstBufCfg->u32BufSize;
    
    pstMMZBuf = &(pstBuffer->stMMZBuf);
    if(pstInstance->pfUserCallBack == HI_NULL)
    {
        VPSS_FATAL("\n pfUserCallBack is NULL");
        return HI_FAILURE;
    }
    stFrmBuf.hPort = hPort;
    stFrmBuf.u32Size = pstBufCfg->u32BufSize;
    stFrmBuf.u32Stride = pstBufCfg->u32BufStride;
    stFrmBuf.u32FrmH = u32StoreH;
    stFrmBuf.u32FrmW = u32StoreW;
    s32Ret = pstInstance->pfUserCallBack(pstInstance->hDst,VPSS_EVENT_GET_FRMBUFFER,&stFrmBuf);

    if(s32Ret == HI_SUCCESS)
    {
        pstMMZBuf->u32Size = stFrmBuf.u32Size;
        pstMMZBuf->u32StartPhyAddr = stFrmBuf.u32StartPhyAddr;
        pstMMZBuf->u32StartVirAddr= stFrmBuf.u32StartVirAddr;
        pstBuffer->u32Stride = stFrmBuf.u32Stride;
    }
    
    return s32Ret;
}

HI_S32 VPSS_INST_RelFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE  hPort,
                                HI_DRV_VPSS_BUFLIST_CFG_S   *pstBufCfg,
                                MMZ_BUFFER_S *pstMMZBuf)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_FRMBUF_S stFrmBuf;

    if(pstInstance == HI_NULL)
    {
        VPSS_FATAL("\n pstInstance is NULL");
        return HI_FAILURE;
    }

    if(pstInstance->pfUserCallBack == HI_NULL)
    {
        VPSS_FATAL("\n pfUserCallBack is NULL");
        return HI_FAILURE;
    }
    stFrmBuf.hPort = hPort;
    stFrmBuf.u32Size = pstBufCfg->u32BufSize;
    stFrmBuf.u32Stride = pstBufCfg->u32BufStride;

    stFrmBuf.u32Size = pstMMZBuf->u32Size;
    stFrmBuf.u32StartPhyAddr = pstMMZBuf->u32StartPhyAddr;
    stFrmBuf.u32StartVirAddr = pstMMZBuf->u32StartVirAddr;
    
    s32Ret = pstInstance->pfUserCallBack(pstInstance->hDst,VPSS_EVENT_REL_FRMBUFFER,&stFrmBuf);

    if(s32Ret == HI_SUCCESS)
    {
        memset(pstMMZBuf,0,sizeof(MMZ_BUFFER_S));
    }
    
    return s32Ret;
}

HI_S32 VPSS_INST_ReportNewFrm(VPSS_INSTANCE_S* pstInstance,
                                VPSS_HANDLE  hPort,HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_FRMINFO_S stFrmInfo;

    if(pstInstance == HI_NULL)
    {
        VPSS_FATAL("\n pstInstance is NULL");
        return HI_FAILURE;
    }

    if(pstInstance->pfUserCallBack == HI_NULL)
    {
        VPSS_FATAL("\n pfUserCallBack is NULL");
        return HI_FAILURE;
    }
    stFrmInfo.hPort = hPort;
    memcpy(&(stFrmInfo.stFrame),pstFrm,sizeof(HI_DRV_VIDEO_FRAME_S));
				
    s32Ret = pstInstance->pfUserCallBack(pstInstance->hDst,VPSS_EVENT_NEW_FRAME,&stFrmInfo);
    
    return s32Ret;
}

HI_S32 VPSS_INST_GetFieldAddr(LIST *pNode,
                                HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                                HI_DRV_BUF_ADDR_E eLReye,
                                HI_RECT_S *pstInRect)
{
    
    VPSS_IMAGE_NODE_S *pstTarget;
    HI_DRV_VIDEO_FRAME_S *pstImg;
    HI_U32 u32InHeight;
    HI_U32 u32InWidth;
    
    pstTarget = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
    pstImg = &(pstTarget->stSrcImage);
    memcpy(pstFieldAddr,&(pstImg->stBufAddr[eLReye]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));

    
    
    #if 1
    if (pstInRect->s32X == 0 && pstInRect->s32Y == 0 
        && pstInRect->s32Height == 0 && pstInRect->s32Width == 0)
    {
        memcpy(pstFieldAddr,&(pstImg->stBufAddr[eLReye]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));
    }        
    else
    {
        u32InHeight = (pstInRect->s32Height + 7) / 8 * 8;
        u32InWidth  = pstInRect->s32Width;
        memcpy(pstFieldAddr,&(pstImg->stBufAddr[eLReye]),
                        sizeof(HI_DRV_VID_FRAME_ADDR_S));
        if (pstImg->ePixFormat == HI_DRV_PIX_FMT_NV12 
            || pstImg->ePixFormat == HI_DRV_PIX_FMT_NV21)
        {
            pstFieldAddr->u32PhyAddr_Y = 
                pstImg->stBufAddr[eLReye].u32PhyAddr_Y 
                + pstImg->stBufAddr[eLReye].u32Stride_Y * pstInRect->s32Y + pstInRect->s32X;
            pstFieldAddr->u32PhyAddr_C = 
                pstImg->stBufAddr[eLReye].u32PhyAddr_C 
                + pstImg->stBufAddr[eLReye].u32Stride_C * pstInRect->s32Y / 2 + pstInRect->s32X;
            
        }
        else if(pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV16 
                || pstTarget->stSrcImage.ePixFormat == HI_DRV_PIX_FMT_NV61)
        {
            pstFieldAddr->u32PhyAddr_Y = 
                pstImg->stBufAddr[eLReye].u32PhyAddr_Y 
                + pstImg->stBufAddr[eLReye].u32Stride_Y * pstInRect->s32Y + pstInRect->s32X;
            pstFieldAddr->u32PhyAddr_C = 
                pstImg->stBufAddr[eLReye].u32PhyAddr_C 
                + pstImg->stBufAddr[eLReye].u32Stride_C * pstInRect->s32Y + pstInRect->s32X;
        }
        else
        {
            VPSS_FATAL("\n InCropError \n");
        }
    }
    #endif

    if(pstTarget->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstFieldAddr->u32PhyAddr_Y = pstFieldAddr->u32PhyAddr_Y + pstFieldAddr->u32Stride_Y;
        pstFieldAddr->u32PhyAddr_C= pstFieldAddr->u32PhyAddr_C + pstFieldAddr->u32Stride_C;
    }

    if(pstTarget->stSrcImage.bProgressive == HI_FALSE)
    {
        
        pstFieldAddr->u32Stride_Y = 2* pstFieldAddr->u32Stride_Y;
        pstFieldAddr->u32Stride_C = 2* pstFieldAddr->u32Stride_C;
        
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_INST_GetDeiAddr(VPSS_INSTANCE_S* pstInstance,
                            HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                            HI_DRV_VPSS_DIE_MODE_E eDeiMode,
                            HI_DRV_BUF_ADDR_E eLReye)
{
    LIST *pNode;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    HI_RECT_S stInRect;
    VPSS_IMAGE_NODE_S *pstTarget;
    HI_DRV_VIDEO_FRAME_S *pstImg;
    HI_DRV_VID_FRAME_ADDR_S stAddr[6];
    pstImageList = &(pstInstance->stSrcImagesList);

    memset(stAddr,0,sizeof(HI_DRV_VID_FRAME_ADDR_S)*6);

    pNode = pstImageList->pstTarget_1;
    if (pNode != &(pstImageList->stFulImageList))
    {
        pstTarget = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        pstImg = &(pstTarget->stSrcImage);
    }
    else
    {
        VPSS_FATAL("\nGet Dei Addr Error\n");
        return HI_FAILURE;
    }
    
    if(pstInstance->stProcCtrl.bUseCropRect == HI_FALSE)
    {
        stInRect.s32Height = pstInstance->stProcCtrl.stInRect.s32Height / 2 * 2;
        stInRect.s32Width  = pstInstance->stProcCtrl.stInRect.s32Width;
        stInRect.s32X      = pstInstance->stProcCtrl.stInRect.s32X;
        stInRect.s32Y      = pstInstance->stProcCtrl.stInRect.s32Y;
    }
    else
    {
        stInRect.s32Height = (pstImg->u32Height
                             - pstInstance->stProcCtrl.stCropRect.u32TopOffset
                             - pstInstance->stProcCtrl.stCropRect.u32BottomOffset)/ 2 * 2;
        stInRect.s32Width  = pstImg->u32Width 
                             - pstInstance->stProcCtrl.stCropRect.u32LeftOffset
                             - pstInstance->stProcCtrl.stCropRect.u32RightOffset;
        stInRect.s32X      = pstInstance->stProcCtrl.stCropRect.u32LeftOffset;
        stInRect.s32Y      = pstInstance->stProcCtrl.stCropRect.u32TopOffset;
        
    }
    
    if ((stInRect.s32Width +stInRect.s32X) > pstImg->u32Width
            || (stInRect.s32Height + stInRect.s32Y) > pstImg->u32Height
            || stInRect.s32X < 0
            || stInRect.s32Y < 0)
    {
        stInRect.s32Width = 0;
        stInRect.s32Height = 0;
        stInRect.s32X = 0;
        stInRect.s32Y = 0;
    }
    
    switch(eDeiMode)
    {
        case HI_DRV_VPSS_DIE_DISABLE:
            break;
        case HI_DRV_VPSS_DIE_3FIELD:
        case HI_DRV_VPSS_DIE_4FIELD:
        case HI_DRV_VPSS_DIE_5FIELD:
            pNode = pstImageList->pstTarget_1;
            VPSS_INST_GetFieldAddr(pNode,&(stAddr[0]),eLReye,&stInRect);
                        
            pNode = pNode->next->next;
            if(pNode == &(pstImageList->stFulImageList))
            {
                VPSS_FATAL("\n GetDeiAddr Error 3\n");
            }
            VPSS_INST_GetFieldAddr(pNode,&(stAddr[3]),eLReye,&stInRect);
            
            pNode = pNode->next;
            VPSS_INST_GetFieldAddr(pNode,&(stAddr[4]),eLReye,&stInRect);   
                        
            pNode = pNode->next;
            VPSS_INST_GetFieldAddr(pNode,&(stAddr[5]),eLReye,&stInRect);   
            break;
        default:
            VPSS_FATAL("\n Dei Mode isn't supported");
            return HI_FAILURE;
            break;
    }
    memcpy(pstFieldAddr,stAddr,sizeof(HI_DRV_VID_FRAME_ADDR_S)*6);
    return HI_SUCCESS;
}


HI_S32 VPSS_INST_GetImgVc1Info(LIST *pNode,VPSS_ALG_VC1INFO_S *pstVc1Info)
{
    VPSS_IMAGE_NODE_S *pstTarget;
    HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
    HI_VDEC_VC1_RANGE_INFO_S *pstVdecVcInfo;
    
    pstTarget = list_entry(pNode,VPSS_IMAGE_NODE_S, node);

    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstTarget->stSrcImage.u32Priv[0]);
    pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
    pstVdecVcInfo = &(pstVdecPriv->stVC1RangeInfo);

    pstVc1Info->u8PicStructure = pstVdecVcInfo->u8PicStructure;     /**< 0: frame, 1: top, 2: bottom, 3: mbaff, 4: field pair */
    pstVc1Info->u8PicQPEnable = pstVdecVcInfo->u8PicQPEnable;
    pstVc1Info->u8ChromaFormatIdc = pstVdecVcInfo->u8ChromaFormatIdc;  /**< 0: yuv400, 1: yuv420 */
    pstVc1Info->u8VC1Profile = pstVdecVcInfo->u8VC1Profile;
    
    pstVc1Info->s32QPY = pstVdecVcInfo->s32QPY;
    pstVc1Info->s32QPU = pstVdecVcInfo->s32QPU;
    pstVc1Info->s32QPV = pstVdecVcInfo->s32QPV;
    pstVc1Info->s32RangedFrm = pstVdecVcInfo->s32RangedFrm;
    
    pstVc1Info->u8RangeMapYFlag = pstVdecVcInfo->u8RangeMapYFlag;
    pstVc1Info->u8RangeMapY = pstVdecVcInfo->u8RangeMapY;
    pstVc1Info->u8RangeMapUVFlag = pstVdecVcInfo->u8RangeMapUVFlag;
    pstVc1Info->u8RangeMapUV = pstVdecVcInfo->u8RangeMapUV;
    pstVc1Info->u8BtmRangeMapYFlag = pstVdecVcInfo->u8BtmRangeMapYFlag;
    pstVc1Info->u8BtmRangeMapY = pstVdecVcInfo->u8BtmRangeMapY;
    pstVc1Info->u8BtmRangeMapUVFlag = pstVdecVcInfo->u8BtmRangeMapUVFlag;
    pstVc1Info->u8BtmRangeMapUV = pstVdecVcInfo->u8BtmRangeMapUV;

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_GetVc1Info(VPSS_INSTANCE_S* pstInstance,VPSS_ALG_VC1INFO_S *pstVc1Info,HI_DRV_VPSS_DIE_MODE_E eDeiMode)
{
    LIST *pNode;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_ALG_VC1INFO_S stInfo[3];
    VPSS_IMAGE_NODE_S *pstPreFieldNode;
    VPSS_IMAGE_NODE_S *pstCurFieldNode;
    pstImageList = &(pstInstance->stSrcImagesList);

    memset(stInfo,0,sizeof(VPSS_ALG_VC1INFO_S)*3);
    
    switch(eDeiMode)
    {
        case HI_DRV_VPSS_DIE_DISABLE:
            pNode = pstImageList->pstTarget_1->next;
            
            pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);

            VPSS_INST_GetImgVc1Info(pNode,&(stInfo[1]));
            break;
        case HI_DRV_VPSS_DIE_5FIELD:
            pNode = pstImageList->pstTarget_1;
            
            pstPreFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
            
            pNode = pNode->next;
            pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);

            /*
               * 1.处理一帧的第一场，需要前一帧，当前帧，后一帧信息
               *     T B T B T B
               *         |
               *  2.处理一帧的第二场，需要当前帧，后一帧，后两帧信息
               *     T B T B T B
               *       |
               */
            pNode = pstImageList->pstTarget_1;
            
            /*处理的是一帧的第一场*/
            if (pstPreFieldNode->stSrcImage.u32FrameIndex
                != pstCurFieldNode->stSrcImage.u32FrameIndex)
            {
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[0]));
                pNode = pNode->next->next;
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[1]));
                pNode = pNode->next;
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[2]));   
            }
            else
            {
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[0]));
                pNode = pNode->next->next;
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[1]));
                pNode = pNode->next->next;
                VPSS_INST_GetImgVc1Info(pNode,&(stInfo[2]));
            }
            break;
        default:
            VPSS_FATAL("\n Dei Mode isn't supported");
            return HI_FAILURE;
            break;
    }
    memcpy(pstVc1Info,stInfo,sizeof(VPSS_ALG_VC1INFO_S)*3);
    return HI_SUCCESS;
}



HI_S32 VPSS_INST_CorrectImgListOrder(VPSS_INSTANCE_S* pstInstance,HI_U32 u32RealTopFirst)
{
    LIST *pNode;
    VPSS_IMAGELIST_INFO_S *pstImageList;
    VPSS_IMAGE_NODE_S *pstPreFieldNode;
    VPSS_IMAGE_NODE_S *pstCurFieldNode;
    LIST *pPreNode;
    LIST *pNextNode;
    LIST *pos, *n;
    
    pstImageList = &(pstInstance->stSrcImagesList);

    #if 0
    list_for_each_safe(pos, n, &(pstImageList->stFulImageList))
    {
        
        pstCurFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);

        printk("\nIn ID %d BT %d TopFirst %d",pstCurFieldNode->stSrcImage.u32FrameIndex,
                            pstCurFieldNode->stSrcImage.enFieldMode,
                            pstCurFieldNode->stSrcImage.bTopFieldFirst);
        
        if(pos == pstImageList->pstTarget_1)
        {
            printk("-->tar_1");
        }
    }
    #endif
    if(u32RealTopFirst == pstInstance->u32RealTopFirst )
    {
        return HI_SUCCESS;
    }

    /*
     1.检出顶底场顺序保存在实例属性中
     2.根据该属性，拆分隔行源
     3.算法检出->获取DEI地址后调用修正
     4.目前可见的情况:
        (1)逐行源第一帧进入
            0xffffffff -> 0xfffffffe
        (2)隔行源第一帧进入,根据帧信息，做第一次赋值
            0xffffffff -> TRUE，然后做帧信息拆分
        (3)传入帧信息逐隔行变化
            逐行->隔行
                问题1:
                   修正逐行->假隔行
                   真实逐行->真隔行  存在吗?暂不考虑
            隔行->逐行
                问题2:
                   修正隔行->假逐行
                   真实隔行->真逐行  存在吗?暂不考虑
     */

     
    /*初始值->有效值*/
    if(pstInstance->u32RealTopFirst == 0xffffffff)
    {
        pstInstance->u32RealTopFirst = u32RealTopFirst;
        return HI_SUCCESS;
    }

    

    
    /*底场优先 -> 顶场优先 切换*/
    /*
	 *   B T  B T B T
	 *     | 
	 *   T B (T B T B)
	 *
      */
    if(u32RealTopFirst == HI_TRUE)
    {
        pNode = pstImageList->pstTarget_1->next;
            
        pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        if(pstCurFieldNode->stSrcImage.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
   

        
    }
    /*顶场优先->底场优先 切换*/
    else
    {
        pNode = pstImageList->pstTarget_1->next;
            
        pstCurFieldNode = list_entry(pNode,VPSS_IMAGE_NODE_S, node);
        if(pstCurFieldNode->stSrcImage.enFieldMode == HI_DRV_FIELD_TOP)
        {
            VPSS_FATAL("Field Order Detect Error\n");
            return HI_FAILURE;
        }
    }

    for (pos = (pstImageList->stFulImageList).next, n = pos->next; 
            pos != &(pstImageList->stFulImageList);
            pos = pos->next, n = pos->next)
    {
        pPreNode = pos->prev;
        pNextNode = n->next;
        
        pstCurFieldNode = list_entry(n,VPSS_IMAGE_NODE_S, node);
        pstPreFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);
        if(pstCurFieldNode->stSrcImage.u32FrameIndex
           != pstCurFieldNode->stSrcImage.u32FrameIndex)
        {
            VPSS_FATAL("Field Order Detect Error\n");
        }
        
        if(pstImageList->pstTarget_1 == n)
        {
            VPSS_FATAL("Field Order Detect Error\n");
        }
        
        if(pstImageList->pstTarget_1 == pos)
        {
            pstImageList->pstTarget_1 = pos->next;
        }
        
        
        
        list_del_init(pos);
        list_del_init(n);

        /*顶底场调换*/
        n->next = pos;
        n->prev = pPreNode;
        pos->next = pNextNode;
        pos->prev = n;

        pPreNode->next = n;
        pNextNode->prev = pos;
    }
    pstInstance->u32RealTopFirst = u32RealTopFirst;
    #if 0 
    list_for_each_safe(pos, n, &(pstImageList->stFulImageList))
    {
        
        pstCurFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);
        
        printk("\nOut ID %d BT %d TopFirst %d",pstCurFieldNode->stSrcImage.u32FrameIndex,
                            pstCurFieldNode->stSrcImage.enFieldMode,
                            pstCurFieldNode->stSrcImage.bTopFieldFirst);
    }

    printk("\n");
    #endif
    #if 0
    printk("\nOut ###################\n");
    list_for_each_safe(pos, n, &(pstImageList->stFulImageList))
    {
        
        pstCurFieldNode = list_entry(pos,VPSS_IMAGE_NODE_S, node);

        printk("\nIn ID %d BT %d TopFirst %d",pstCurFieldNode->stSrcImage.u32FrameIndex,
                            pstCurFieldNode->stSrcImage.enFieldMode,
                            pstCurFieldNode->stSrcImage.bTopFieldFirst);
        
        if(pos == pstImageList->pstTarget_1)
        {
            printk("-->tar_1");
        }
    }
    #endif
    return HI_SUCCESS;
}

HI_S32 VPSS_INST_CheckNeedRstDei(VPSS_INSTANCE_S *pstInstance,HI_DRV_VIDEO_FRAME_S *pCurImage)
{
    VPSS_IMAGE_NODE_S *pstPreImgNode;
    HI_DRV_VIDEO_FRAME_S *pPreImage;
    
    HI_DRV_VIDEO_PRIVATE_S *pstCurPriv;
    HI_VDEC_PRIV_FRAMEINFO_S *pstCurVdecPriv;
    
    HI_DRV_VIDEO_PRIVATE_S *pstPrePriv;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPreVdecPriv;
    
    
    VPSS_IMAGELIST_INFO_S *pstSrcImagesList;

    pstCurPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pCurImage->u32Priv[0]);
    pstCurVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstCurPriv->u32Reserve[0]);


    if (pstCurPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
        return HI_SUCCESS;
    }
    if (pCurImage->bProgressive == HI_TRUE)
    {
        return HI_SUCCESS;
    }
    pstSrcImagesList = &(pstInstance->stSrcImagesList);

    if ((pstSrcImagesList->stFulImageList.prev)
        != &(pstSrcImagesList->stFulImageList))
    {
        pstPreImgNode = 
            list_entry(pstSrcImagesList->stFulImageList.prev, VPSS_IMAGE_NODE_S, node);
        pPreImage = &(pstPreImgNode->stSrcImage);

        pstPrePriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pPreImage->u32Priv[0]);
        pstPreVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstPrePriv->u32Reserve[0]);

        if ((pPreImage->u32Width != pCurImage->u32Width)
            || (pPreImage->u32Height != pCurImage->u32Height)
            || (pPreImage->ePixFormat != pCurImage->ePixFormat)
            /*
            || (pstPreVdecPriv->u32YStride != pCrtVdecFrame->u32YStride)
            || (pstPreVdecPriv->enSampleType != pCrtVdecFrame->enSampleType)
            
            || (pstPreVdecPriv->enFieldMode != pCrtVdecFrame->enFieldMode)
            || (pstPreVdecPriv->bTopFieldFirst != pCrtVdecFrame->bTopFieldFirst)
                */)
        {
            pstInstance->u32NeedRstDei = HI_TRUE;
            pstInstance->u32RealTopFirst = 0xffffffff;
            
        }
        
    }
    else
    {
        pstInstance->u32NeedRstDei = HI_TRUE;
        pstInstance->u32RealTopFirst = 0xffffffff;
        
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_INST_CorrectProgInfo(VPSS_INSTANCE_S *pstInstance,HI_DRV_VIDEO_FRAME_S *pImage)
{
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
    HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
    
    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pImage->u32Priv[0]);
    pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
	
	
    /*
    // belive VI, VI will give the right info  
    if (FRAME_SOURCE_VI == pFrameOptm->enFrameSrc) 
    {
        return ;
    }
    */

    if ( 0x2 == (pstVdecPriv->u8Marker &= 0x2)) 
    {
        pImage->bProgressive = HI_TRUE;
        return  HI_SUCCESS;
    }

    if ( pImage->u32Height > 1080 && pImage->u32Width > 1920) 
    {
        pImage->bProgressive = HI_TRUE;
        return  HI_SUCCESS;
    }
    

    if (pImage->eFrmType == HI_DRV_FT_FPK)
    {
        pImage->bProgressive = HI_TRUE;
        return HI_SUCCESS;
    }
    
    if (pstFrmPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG
        || pstFrmPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
        return HI_SUCCESS;
    }
    
    if (  (HI_UNF_VCODEC_TYPE_REAL8 == pstVdecPriv->entype)
            ||(HI_UNF_VCODEC_TYPE_REAL9 == pstVdecPriv->entype)
            ||(HI_UNF_VCODEC_TYPE_MPEG4 == pstVdecPriv->entype))
    {
        return HI_SUCCESS;
    }

    if(pstInstance->u32Rwzb > 0)
    {
        /* special HD bit-stream, according to discussion of algorithm (20110115),
         * employ way of trusting output system information.
         * Currently, it is used in HD output.
         */

        /* for 576i/p 480i/p CVBS output, employ interlaced read */
        if( (pstInstance->u32Rwzb == PAT_CCITT033)
                || (pstInstance->u32Rwzb == PAT_CCITT18)
                || (pstInstance->u32Rwzb == PAT_CBAR576_75)
                || (pstInstance->u32Rwzb == PAT_CCIR3311)
                || (pstInstance->u32Rwzb == PAT_MATRIX625)
                || ((pstInstance->u32Rwzb >= PAT_CBAR576_75_B) && (pstInstance->u32Rwzb <= PAT_M576I_BOWTIE))
          )
        {
            pImage->bProgressive = HI_FALSE;
        }
        else if (pstInstance->u32Rwzb == PAT_720P50 || pstInstance->u32Rwzb == PAT_720P59)
        {
			pImage->bProgressive = HI_TRUE;
            /*
            if (1 == OPTM_M_GetDispProgressive(HAL_DISP_CHANNEL_DHD))
            {
                pFrame->enSampleType = VIDEO_SAMPLE_TYPE_PROGRESSIVE;
            }
            else
            {
                pFrame->enSampleType = VIDEO_SAMPLE_TYPE_INTERLACE;
            }
            */
        }
        else
        {

        }

        /* special SD bit-stream, trust bit-stream information  */
    }    
    else if(pImage->u32Height == 720)
    {
        pImage->bProgressive = HI_TRUE;
    }
    /* un-trust bit-stream information  */
    else if(pImage->u32Height <= 576)
    {
        if ( (240 >= pImage->u32Height) && (320 >= pImage->u32Width) )
        {
            pImage->bProgressive = HI_TRUE;
        }
        else if (pImage->u32Height <= (pImage->u32Width * 9 / 14 ) ) 
        {
            // Rule: wide aspect ratio stream is normal progressive, we think that progressive info is correct.
        }
        else
        {
            pImage->bProgressive = HI_FALSE;
        }
    }
    else
    {

    }

    /*对于隔行非顶底场间插的帧数据，强制拆分*/
    if (pImage->enFieldMode != HI_DRV_FIELD_ALL)
    {
        VPSS_INFO("\n InImgFieldMode %d  OriPro %d  TopFirst %d CorrectProg Error \n",
                    pImage->enFieldMode,
                    pImage->bProgressive,
                    pImage->bTopFieldFirst);
        pImage->bProgressive = HI_FALSE;
        pImage->enFieldMode = HI_DRV_FIELD_ALL;
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_INST_ChangeInRate(VPSS_INSTANCE_S *pstInstance,HI_U32 u32InRate)
{
    HI_U32 u32HzRate; /*0 -- 100*/
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;

    
    u32HzRate = u32InRate / 1000;
    /**/
    if(u32HzRate < 10)
    {
        u32HzRate = 1;
    }
    else if(u32HzRate < 20)
    {
        u32HzRate = 10;
    }
    else if(u32HzRate < 30)
    {
        u32HzRate = 25;
    }
    else if(u32HzRate < 40)
    {
        u32HzRate = u32HzRate / 10 * 10;
    }
    else if(u32HzRate < 60)
    {
        u32HzRate = 50;
    }
    else
    {
        u32HzRate = u32HzRate / 10 * 10;
    }
    
    if( u32HzRate == pstInstance->u32InRate)
    {
        return HI_SUCCESS;
    }
    else
    {
        pstInstance->u32InRate = u32HzRate;
    }
    
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId != VPSS_INVALID_HANDLE)
        {
            pstPort->u32OutCount = 0;
            
        }
    }
    return HI_SUCCESS;
}


HI_BOOL VPSS_INST_CheckIsDropped(VPSS_INSTANCE_S *pstInstance,HI_U32 u32OutRate,HI_U32 u32OutCount)
{
    HI_U32 u32Multiple;
    HI_U32 u32Quote;
    HI_BOOL bDropped;

    bDropped = HI_FALSE;
    
    if(pstInstance->u32InRate < u32OutRate || u32OutRate == 0)
    {
         bDropped = HI_FALSE;
    }
    else
    {
        u32Multiple = pstInstance->u32InRate*10 / u32OutRate;

        u32Quote = (u32Multiple + 5)/10;

        if(u32OutCount % u32Quote == 1)
        {
            bDropped = HI_TRUE;
        }
        else
        {
            bDropped = HI_FALSE;
        }
    }

    return bDropped;
}


HI_BOOL VPSS_INST_CheckAllDone(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_IMAGELIST_INFO_S *pstImgListInfo;
    VPSS_PORT_S *pstPort;
    VPSS_FB_INFO_S *pstFrmListInfo;
    HI_U32 u32Count;
    HI_BOOL bDone;

    pstImgListInfo = &(pstInstance->stSrcImagesList);
    bDone = HI_TRUE;
    
    /*检查前级输入的image是否都处理完*/
    VPSS_OSAL_DownLock(&(pstImgListInfo->stFulListLock));
    if ( pstImgListInfo->pstTarget_1 != pstImgListInfo->stFulImageList.prev)
    {
        bDone = HI_FALSE;
    }
    else
    {
        bDone = HI_TRUE;
    }
    VPSS_OSAL_UpLock(&(pstImgListInfo->stFulListLock));

    /*检查后级输出的frame是否都取走*/
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count ++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if(pstPort->s32PortId != VPSS_INVALID_HANDLE)
        {
            pstFrmListInfo = &(pstPort->stFrmInfo);
            //VPSS_OSAL_DownLock(&(pstFrmListInfo->stFulBufLock));
            VPSS_OSAL_DownSpin(&(pstFrmListInfo->stFulBufSpin));
            if (pstFrmListInfo->pstTarget_1 != pstFrmListInfo->stFulFrmList.prev)
            {
                bDone = bDone & HI_FALSE;
            }
            else
            {
                bDone = bDone & HI_TRUE;
            }
            VPSS_OSAL_UpSpin(&(pstFrmListInfo->stFulBufSpin));
            //VPSS_OSAL_UpLock(&(pstFrmListInfo->stFulBufLock));
        }
    }

    return bDone;
}


HI_S32 VPSS_INST_SyncUsrCfg(VPSS_INSTANCE_S * pstInstance)
{
    HI_RECT_S *pstInstInCrop;
    HI_RECT_S *pstCfgInCrop;
    HI_DRV_CROP_RECT_S *pstInstUsrCrop;
    HI_DRV_CROP_RECT_S *pstCfgUsrCrop;
    HI_DRV_VPSS_CFG_S *pstInstUsrcCfg;
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    HI_DRV_VPSS_PORT_CFG_S *pstPortCfg;
    HI_S32 s32Ret;

    s32Ret = VPSS_OSAL_TryLockSpin(&(pstInstance->stUsrSetSpin));

    if(s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    
    if(pstInstance->u32IsNewCfg)
    {
        pstInstUsrcCfg = &(pstInstance->stUsrInstCfg);
    
    //VPSS_OSAL_DownLock(&(pstInstance->stInstLock));
    
        pstInstance->s32Priority = pstInstUsrcCfg->s32Priority;  
        pstInstance->bAlwaysFlushSrc = pstInstUsrcCfg->bAlwaysFlushSrc;

        pstInstInCrop = &(pstInstance->stProcCtrl.stInRect);
        pstCfgInCrop= &(pstInstUsrcCfg->stProcCtrl.stInRect);

       
        if (pstInstUsrcCfg->stProcCtrl.bUseCropRect == HI_FALSE
            &&(pstInstInCrop->s32Height != pstCfgInCrop->s32Height
            || pstInstInCrop->s32Width != pstCfgInCrop->s32Width
            || pstInstInCrop->s32X != pstCfgInCrop->s32X
            || pstInstInCrop->s32Y != pstCfgInCrop->s32Y))
        {
            pstInstance->u32NeedRstDei = HI_TRUE;
            pstInstance->u32RealTopFirst = 0xffffffff;      
        }
        
        pstInstUsrCrop = &(pstInstance->stProcCtrl.stCropRect);
        pstCfgUsrCrop = &(pstInstUsrcCfg->stProcCtrl.stCropRect);
        
        if(pstInstUsrcCfg->stProcCtrl.bUseCropRect == HI_TRUE
            &&(pstInstUsrCrop->u32BottomOffset != pstCfgUsrCrop->u32BottomOffset
            || pstInstUsrCrop->u32TopOffset != pstCfgUsrCrop->u32TopOffset
            || pstInstUsrCrop->u32LeftOffset != pstCfgUsrCrop->u32LeftOffset
            || pstInstUsrCrop->u32RightOffset != pstCfgUsrCrop->u32RightOffset))
        {
            pstInstance->u32NeedRstDei = HI_TRUE;
            pstInstance->u32RealTopFirst = 0xffffffff;     
        }

        pstInstance->stProcCtrl = pstInstUsrcCfg->stProcCtrl;


       
        for(u32Count = 0; 
            u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;
            u32Count ++)
        {
            pstPort = &(pstInstance->stPort[u32Count]);
            
            if(pstPort->s32PortId != VPSS_INVALID_HANDLE)
            {
                pstPortCfg = &(pstInstance->stUsrPortCfg[u32Count]);

                pstPort->eFormat = pstPortCfg->eFormat; 
                pstPort->s32OutputWidth = pstPortCfg->s32OutputWidth;
                pstPort->s32OutputHeight = pstPortCfg->s32OutputHeight;
                pstPort->eDstCS = pstPortCfg->eDstCS;
                pstPort->stDispPixAR = pstPortCfg->stDispPixAR;
                pstPort->eAspMode = pstPortCfg->eAspMode;
                pstPort->stCustmAR = pstPortCfg->stCustmAR;
                pstPort->stScreen = pstPortCfg->stScreen;
                pstPort->bInterlaced = pstPortCfg->bInterlaced;

                pstPort->bTunnelEnable = pstPortCfg->bTunnelEnable;
                pstPort->s32SafeThr = pstPortCfg->s32SafeThr;   
                pstPort->u32MaxFrameRate = pstPortCfg->u32MaxFrameRate; 

                pstPort->stProcCtrl = pstPortCfg->stProcCtrl;
            }
        }
        
        pstInstance->u32IsNewCfg = HI_FALSE;

    }
    else
    {

    }
    
    VPSS_OSAL_UpSpin(&(pstInstance->stUsrSetSpin));

    return HI_SUCCESS;
}


HI_S32 VPSS_INST_GetPortPrc(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,VPSS_PORT_PRC_S *pstPortPrc)
{
    VPSS_PORT_S *pstPort;
    pstPort = HI_NULL;

    if (hPort == VPSS_INVALID_HANDLE)
    {
        memset(pstPortPrc,0,sizeof(VPSS_PORT_PRC_S));
        pstPortPrc->s32PortId = VPSS_INVALID_HANDLE;
    }
    else
    {
        pstPort = VPSS_INST_GetPort(pstInstance,hPort);
        if (pstPort == HI_NULL)
        {
            VPSS_FATAL("Get Port Proc Error.\n");
            return HI_FAILURE;
        }
        pstPortPrc->s32PortId = pstPort->s32PortId ;
        pstPortPrc->bEnble = pstPort->bEnble ;
        pstPortPrc->eFormat = pstPort->eFormat ;
        pstPortPrc->s32OutputWidth = pstPort->s32OutputWidth ;
        pstPortPrc->s32OutputHeight = pstPort->s32OutputHeight ;
        pstPortPrc->eDstCS = pstPort->eDstCS ;
        pstPortPrc->stDispPixAR = pstPort->stDispPixAR ;
        pstPortPrc->eAspMode = pstPort->eAspMode ;
        pstPortPrc->stCustmAR = pstPort->stCustmAR ;
        pstPortPrc->bInterlaced = pstPort->bInterlaced ;
        pstPortPrc->stScreen = pstPort->stScreen ;
        pstPortPrc->u32MaxFrameRate = pstPort->u32MaxFrameRate ;
        pstPortPrc->u32OutCount = pstPort->u32OutCount ;
        pstPortPrc->stProcCtrl = pstPort->stProcCtrl ;
        pstPortPrc->bTunnelEnable = pstPort->bTunnelEnable ;
        pstPortPrc->s32SafeThr = pstPort->s32SafeThr ;
        
        pstPortPrc->stBufListCfg.eBufType = pstPort->stFrmInfo.stBufListCfg.eBufType;
        pstPortPrc->stBufListCfg.u32BufNumber = pstPort->stFrmInfo.stBufListCfg.u32BufNumber;
        pstPortPrc->stBufListCfg.u32BufSize = pstPort->stFrmInfo.stBufListCfg.u32BufSize;
        pstPortPrc->stBufListCfg.u32BufStride = pstPort->stFrmInfo.stBufListCfg.u32BufStride;

        VPSS_FB_GetState(&(pstPort->stFrmInfo),&(pstPortPrc->stFbPrc));
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_INST_StoreDeiData(VPSS_INSTANCE_S *pstInstance,ALG_FMD_RTL_STATPARA_S *pstDeiData)
{
    VPSS_ALG_StoreDeiData((HI_U32)pstInstance->stAuInfo,pstDeiData);

    return HI_SUCCESS;
}

HI_S32 VPSS_INST_GetDeiData(VPSS_INSTANCE_S *pstInstance,ALG_FMD_RTL_STATPARA_S *pstDeiData)
{
    VPSS_ALG_GetDeiData((HI_U32)pstInstance->stAuInfo,pstDeiData);

    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
