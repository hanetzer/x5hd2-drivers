#include "vpss_fb.h"
#include "drv_stat_ext.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
HI_S32 VPSS_FB_Init(VPSS_FB_INFO_S *pstFrameList,
                HI_DRV_VPSS_BUFLIST_CFG_S *pstBufListCfg)
{
    HI_U32 u32Count;
    HI_S32 s32Ret;
    VPSS_FB_NODE_S* pstNode;
    VPSS_BUFFER_S* pstBuf;
    MMZ_BUFFER_S* pstMMZBuf;
    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg;

    pstBufCfg = &( pstFrameList->stBufListCfg );

    memcpy( pstBufCfg, pstBufListCfg,
            sizeof( HI_DRV_VPSS_BUFLIST_CFG_S ) );

    INIT_LIST_HEAD( &( pstFrameList->stEmptyFrmList ) );
    INIT_LIST_HEAD( &( pstFrameList->stFulFrmList ) );


    VPSS_OSAL_InitSpin(&( pstFrameList->stFulBufSpin ), 1);
    //VPSS_OSAL_InitLOCK(&( pstFrameList->stFulBufLock ), 1);
    //VPSS_OSAL_InitLOCK(&( pstFrameList->stEmptyBufLock ), 1);
    VPSS_OSAL_InitSpin(&( pstFrameList->stEmptyBufSpin ), 1);
    pstFrameList->u32GetTotal = 0;
    pstFrameList->u32GetFail= 0;
    pstFrameList->u32RelTotal = 0;
    pstFrameList->u32RelFail= 0;
    pstFrameList->u32ListFul = 0;
    pstFrameList->ulStart = jiffies;
    pstFrameList->u32GetHZ = 0;
    pstFrameList->u32GetLast = 0;
    
    for ( u32Count = 0; u32Count < pstBufListCfg->u32BufNumber; u32Count ++ )
    {
        pstNode = ( VPSS_FB_NODE_S* )VPSS_VMALLOC( sizeof( VPSS_FB_NODE_S ));
        memset( &( pstNode->stOutFrame ), 0, sizeof( HI_DRV_VIDEO_FRAME_S ) );
        pstBuf = &( pstNode->stBuffer );

        pstBuf->u32Stride = pstBufCfg->u32BufStride;

        pstMMZBuf = &( pstBuf->stMMZBuf );

        switch(pstBufCfg->eBufType)
        {
            case HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE:
                s32Ret = HI_DRV_MMZ_AllocAndMap( "FB", "VPSS", pstBufListCfg->u32BufSize, 0, pstMMZBuf );
                break;
            case HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE:
                pstMMZBuf->u32StartPhyAddr = 0;
                pstMMZBuf->u32StartVirAddr = 0;
                pstMMZBuf->u32Size = pstBufListCfg->u32BufSize;
                break;
            case HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE:
                pstMMZBuf->u32StartPhyAddr = pstBufListCfg->u32BufPhyAddr[u32Count];
                pstMMZBuf->u32StartVirAddr = 0;
                pstMMZBuf->u32Size = pstBufListCfg->u32BufSize;
                break;
            default:
                break;
        }
        if ( HI_SUCCESS != s32Ret )
        {
            VPSS_FATAL( "\n VPSS Alloc Buffer fail----------\n");
            return HI_FAILURE;
        }                   

        list_add_tail( &( pstNode->node ), &( pstFrameList->stEmptyFrmList ) );

    }

    pstFrameList->pstTarget_1 = &( pstFrameList->stFulFrmList );
    return HI_SUCCESS;
}
HI_S32 VPSS_FB_DelInit(VPSS_FB_INFO_S *pstFrameList)
{
    HI_DRV_VPSS_BUFLIST_CFG_S *pstBufCfg;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    HI_U32 u32DelCount;
    
    pstBufCfg = &(pstFrameList->stBufListCfg);

    u32DelCount = 0;

    if (pstFrameList->stBufListCfg.eBufType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
    {
        list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
        {
            pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->stBuffer.stMMZBuf));

            list_del_init(pos);

            VPSS_VFREE(pstTarget);
            u32DelCount++;
        }

        list_for_each_safe(pos, n, &(pstFrameList->stFulFrmList))
        {
            pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->stBuffer.stMMZBuf));

            list_del_init(pos);

            VPSS_VFREE(pstTarget);
            u32DelCount++;
        }

        if (u32DelCount != pstBufCfg->u32BufNumber)
        {
            VPSS_FATAL("\n#################FB_Del ERROR %d",u32DelCount);
        }
    }
    else
    {

    }
    return HI_SUCCESS;
}
//消费者INSTANCE
HI_S32 VPSS_FB_GetFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile)
{
    VPSS_FB_NODE_S *pstFrmNode;
    HI_DRV_VIDEO_FRAME_S *pstFrm;
    LIST *pstNextNode;
    VPSS_FB_NODE_S *pstLeftNode;
    VPSS_FB_NODE_S *pstRightNode;
    HI_S32 s32Ret = HI_FAILURE;

    
    pstFrameList->u32GetTotal++;
    
    if(jiffies - pstFrameList->ulStart >= HZ)
    {
        pstFrameList->ulStart = jiffies;
        pstFrameList->u32GetHZ = pstFrameList->u32GetTotal - pstFrameList->u32GetLast;
        pstFrameList->u32GetLast = pstFrameList->u32GetTotal;
    }
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    
    pstNextNode = (pstFrameList->pstTarget_1)->next;
    
    if(pstNextNode != &(pstFrameList->stFulFrmList))
    {
        pstFrmNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);

        pstFrm = &(pstFrmNode->stOutFrame);
        
        if(pstFrm->eFrmType == HI_DRV_FT_NOT_STEREO)
        {
            memcpy(pstFrame,pstFrm,sizeof(HI_DRV_VIDEO_FRAME_S));
            
            pstFrameList->pstTarget_1 =  pstNextNode;
			s32Ret = HI_SUCCESS;
        }
        else
        {
			if(pstNextNode->next != &(pstFrameList->stFulFrmList))
            {
            	pstLeftNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);
            	pstNextNode = pstNextNode->next;
            	pstRightNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);
            	pstFrmNode = list_entry(pstNextNode, VPSS_FB_NODE_S, node);

            	pstFrm = &(pstFrmNode->stOutFrame);
            
            	if (pstLeftNode->stOutFrame.u32FrameIndex 
                	!= pstRightNode->stOutFrame.u32FrameIndex )
            	{
                	VPSS_FATAL("GetFulFrmBuf 3D Error.\n");
            	}
            
            	memcpy(pstFrame,pstFrm,sizeof(HI_DRV_VIDEO_FRAME_S));
        
            	pstFrameList->pstTarget_1 =  pstNextNode;
				s32Ret = HI_SUCCESS;
            }
            else
            {
                s32Ret = HI_FAILURE;
            }
        }

        
        if (pchFile != HI_NULL)
        {
            printk("\nIndex %d\n",pstFrm->u32FrameIndex);
            VPSS_FB_WRITEYUV(pstFrmNode,pchFile);
        }
        
    }
    else
    {
        pstFrameList->u32GetFail++;
        s32Ret = HI_FAILURE;
    }
    
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
    return s32Ret;
}

HI_U32 u32Debug = 0;
HI_S32 VPSS_FB_RelFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    VPSS_FB_NODE_S *pstTarget;
    VPSS_BUFFER_S *pstBuf;
    LIST *pos, *n;
    HI_S32 s32GetFrm = HI_FAILURE;
    HI_U32 u32Count = 0;
    pstTarget = HI_NULL;
    
    pstFrameList->u32RelTotal++;
    //VPSS_OSAL_DownLock(&(pstFrameList->stFulBufLock));
    //printk("\n Rel %d\n",pstFrame->u32FrameIndex);

    #if 0
    
    if (u32Debug == 1)
    {
        
        printk("\n----------------------------Rel  %d----------------\n",pstFrame->u32FrameIndex);
        
            VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
        for (pos = (pstFrameList->stFulFrmList.next), n = pos->next; 
        pos != &(pstFrameList->stFulFrmList);
		pos = n, n = pos->next)
        {
            u32Debug = 1;
            pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
            printk("\nRel before %d\n",pstTarget->stOutFrame.u32FrameIndex);
        }
            VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
        printk("\n----------------------------Rel %d----------------\n",pstFrame->u32FrameIndex);
    }
    pstTarget = HI_NULL;
    #endif
    
    for (pos = (pstFrameList->stFulFrmList).next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList) && pos != (pstFrameList->pstTarget_1)->next;
		pos = n, n = pos->next)
    {
        if ( u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_WARN("\n RelFulFrmBuf Error\n");
        }
        u32Count ++;
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);

        pstBuf = &(pstTarget->stBuffer);
       
        if(pstBuf->stMMZBuf.u32StartPhyAddr 
           == pstFrame->stBufAddr[0].u32PhyAddr_Y
            /*FOR 3D 璋冭瘯*/
           || pstBuf->stMMZBuf.u32StartPhyAddr 
           == pstFrame->stBufAddr[1].u32PhyAddr_Y)
        {
            if(pstTarget->stOutFrame.u32FrameIndex != pstFrame->u32FrameIndex)
            {
               VPSS_WARN("\nRel Error BufferId %d FrmId %d\n",
                    pstTarget->stOutFrame.u32FrameIndex,
                    pstFrame->u32FrameIndex);
               return HI_FAILURE;
            }
            
            VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
            if(pstFrameList->pstTarget_1 != pos)
            {

            }
            else
            {
                pstFrameList->pstTarget_1 = (pstFrameList->pstTarget_1)->prev;
            }
            
                
            list_del_init(pos);
            
            VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
            
            
            VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstTarget);
            s32GetFrm = HI_SUCCESS;
        }
    }
    
    if(s32GetFrm == HI_FAILURE)
    {
        pstFrameList->u32RelFail++;
        VPSS_WARN("Can't Get RelFrm %d\n",pstFrame->u32FrameIndex);
    }
    return s32GetFrm;
}

//生产者TASK
VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList)
{
    //HI_S32 s32Ret;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg;
    HI_U32 u32Count = 0;
    pstBufCfg = &( pstFrameList->stBufListCfg );

    pstTarget = HI_NULL;
    
    //VPSS_OSAL_DownLock(&(pstFrameList->stEmptyBufLock));
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        if (u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("\n Get GetEmptyFrmBuf Error\n");
        }
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        list_del_init(pos);
        u32Count++;
        break;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin));
    if (pstTarget)
    { 
        memset(&(pstTarget->stOutFrame), 0, 
                sizeof(HI_DRV_VIDEO_FRAME_S));
        //VPSS_OSAL_UpLock(&(pstFrameList->stEmptyBufLock));
        return pstTarget;
    }
    else
    {
        //VPSS_OSAL_UpLock(&(pstFrameList->stEmptyBufLock));
        /*
        VPSS_FATAL("\n there is no empty FrmBuf------------->func %s line %d \r\n",
                    __func__, __LINE__);
        */
        return HI_NULL;
    }
}

HI_S32 VPSS_FB_AddFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode)
{
    //VPSS_OSAL_DownLock(&(pstFrameList->stFulBufLock));
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    list_add_tail(&(pstFBNode->node), &(pstFrameList->stFulFrmList));

    if (pstFBNode->stOutFrame.bIsFirstIFrame)
    {
        HI_DRV_STAT_Event(STAT_EVENT_VPSSOUTFRM, 0);
    }
    
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
    //VPSS_OSAL_UpLock(&(pstFrameList->stFulBufLock));
    return HI_SUCCESS;
}
HI_S32 VPSS_FB_AddEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode)
{
    
    //VPSS_OSAL_DownLock(&(pstFrameList->stEmptyBufLock));
    list_add_tail(&(pstFBNode->node), &(pstFrameList->stEmptyFrmList));
    //VPSS_OSAL_UpLock(&(pstFrameList->stEmptyBufLock));
    
    return HI_SUCCESS;
}

HI_BOOL VPSS_FB_CheckIsAvailable(VPSS_FB_INFO_S *pstFrameList)
{
    LIST* pstEmptyList;
    
    pstEmptyList = &(pstFrameList->stEmptyFrmList);
    //VPSS_OSAL_DownLock(&(pstFrameList->stEmptyBufLock));
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin));
    if(pstEmptyList->next != pstEmptyList)
    {
        //VPSS_OSAL_UpLock(&(pstFrameList->stEmptyBufLock));
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin));
        return HI_TRUE;
    }
    else
    {   
        pstFrameList->u32ListFul++;
        VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin));
        //VPSS_OSAL_UpLock(&(pstFrameList->stEmptyBufLock));
        return HI_FALSE;
    }
}

HI_S32 VPSS_FB_Reset(VPSS_FB_INFO_S *pstFrameList)
{
    HI_DRV_VPSS_BUFLIST_CFG_S *pstBufCfg;
    VPSS_FB_NODE_S *pstTarget;
    LIST *pos, *n;
    HI_U32 u32Count;
    pstBufCfg = &(pstFrameList->stBufListCfg);
    
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    u32Count = 0;
    for (pos = (pstFrameList->pstTarget_1)->next, n = pos->next; 
        pos != &(pstFrameList->stFulFrmList);
		pos = n, n = pos->next)
    {
        if ( u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("\n Reset Error\n");
        }
        
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        list_del_init(pos);
        
        u32Count ++;
        VPSS_FB_AddEmptyFrmBuf(pstFrameList,pstTarget);
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));

    
    #if 0
    printk("\n----------------------------Reset End XX----------------\n");
    
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    for (pos = (pstFrameList->stFulFrmList.next), n = pos->next; 
        pos != &(pstFrameList->stFulFrmList);
		pos = n, n = pos->next)
    {
        u32Debug = 1;
        pstTarget = list_entry(pos, VPSS_FB_NODE_S, node);
        printk("\nReset  After %d\n",pstTarget->stOutFrame.u32FrameIndex);
    }
    
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
    printk("\n----------------------------Reset End YY----------------\n");
    #endif
    return HI_SUCCESS;
}
HI_S32 VPSS_FB_GetState(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_STATE_S *pstFbState)
{
    HI_U32 u32Count;
    VPSS_FB_NODE_S *pstFbNode;
    LIST *pos, *n;
    HI_U32 u32Total = 0;
    HI_U32 u32DoneFlag;

    
    if (pstFrameList->pstTarget_1 != &(pstFrameList->stFulFrmList))
        pstFbState->u32Target_1 = (HI_U32)list_entry(pstFrameList->pstTarget_1, VPSS_FB_NODE_S, node);
    else
        pstFbState->u32Target_1 = (HI_U32)&(pstFrameList->stFulFrmList);
        
    if (pstFrameList->pstTarget_1 == &(pstFrameList->stFulFrmList))
    {
        u32DoneFlag = 2;
    }
    else
    {
        u32DoneFlag = 1;
    }    
    
    
    u32Count = 0;
    VPSS_OSAL_DownSpin(&(pstFrameList->stFulBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stFulFrmList))
    {
        if (u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("\n Get FbList Error\n");
        }
        pstFbNode = list_entry(pos, VPSS_FB_NODE_S, node);
        
        pstFbState->u32FulList[u32Count] = (HI_U32)pstFbNode;
        
        pstFbState->u32List[u32Total][0] = pstFbNode->stOutFrame.u32FrameIndex;
        pstFbState->u32List[u32Total][1] = u32DoneFlag;

        if (pstFbState->u32Target_1 == (HI_U32)pstFbNode)
        {
            u32DoneFlag = 2;
        }
        u32Count++;
        u32Total++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stFulBufSpin));
    pstFbState->u32FulListNumb = u32Count;

    u32Count = 0;
    VPSS_OSAL_DownSpin(&(pstFrameList->stEmptyBufSpin));
    list_for_each_safe(pos, n, &(pstFrameList->stEmptyFrmList))
    {
        if (u32Count >= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
        {
            VPSS_FATAL("\n Get FbList Error\n");
        }
        pstFbNode = list_entry(pos, VPSS_FB_NODE_S, node);
        pstFbState->u32EmptyList[u32Count] = (HI_U32)pstFbNode;

        pstFbState->u32List[u32Total][0] = -1;
        pstFbState->u32List[u32Total][1] = 0;
        u32Count++;
        u32Total++;
    }
    VPSS_OSAL_UpSpin(&(pstFrameList->stEmptyBufSpin));
    pstFbState->u32EmptyListNumb = u32Count;

    while(u32Total < DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER)
    {
        pstFbState->u32List[u32Total][0] = -1;
        pstFbState->u32List[u32Total][1] = 3;
        u32Total++;
    }
    
    
    pstFbState->u32TotalNumb = pstFrameList->stBufListCfg.u32BufNumber;

    pstFbState->u32GetHZ = pstFrameList->u32GetHZ;
    pstFbState->u32GetTotal = pstFrameList->u32GetTotal;
    pstFbState->u32GetFail = pstFrameList->u32GetFail;
    
    pstFbState->u32RelTotal = pstFrameList->u32RelTotal;
    pstFbState->u32RelFail = pstFrameList->u32RelFail;
    
    pstFbState->u32ListFul = pstFrameList->u32ListFul;
    
    return HI_SUCCESS;                            
}

HI_S32 VPSS_FB_WRITEYUV(VPSS_FB_NODE_S  *pstFbNode,HI_CHAR* pchFile)
{
	char str[50] = {0};
	unsigned char *ptr;
	FILE *fp;
    MMZ_BUFFER_S *pstMMZ;
    HI_DRV_VIDEO_FRAME_S *pstFrame;
    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_S8  s_VpssSavePath[64] = {'/','m','n','t',0};
    HI_U32 i,j;
    pstMMZ = &(pstFbNode->stBuffer.stMMZBuf);
    pstFrame = &(pstFbNode->stOutFrame);
    
    ptr = (unsigned char *)pstMMZ->u32StartVirAddr;

    pu8Udata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
    pu8Vdata = VPSS_VMALLOC(pstFrame->u32Width * pstFrame->u32Height / 2 /2);
    pu8Ydata = VPSS_VMALLOC(pstFrame->stBufAddr[0].u32Stride_Y);
    
	if (!ptr)
	{
        VPSS_FATAL("address is not valid!\n");
	}
	else
	{   
	    sprintf(str, "%s/%s", s_VpssSavePath,pchFile);

        fp = VPSS_OSAL_fopen(str, O_RDWR | O_CREAT|O_APPEND, 0);

        if (fp == HI_NULL)
        {
            VPSS_FATAL("open file '%s' fail!\n", str);
            return HI_FAILURE;
        }

        /*写 Y 数据*/
        for (i=0; i<pstFrame->u32Height; i++)
        {
            memcpy(pu8Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);

            /*
                    HI_S32 VPSS_FB_WRITELOGO(HI_U8 *pu8Ydata,
                            HI_U32 u32ImgH,HI_U32 u32ImgW,HI_U32 u32line)
                    if(i>100 && i < 110 )
                    {   
                        for(j = 0; j < pstFrame->u32Width;j++)
                        {
                            pu8Ydata[j] = 200;
                        }
                    }
                */
            
      	    //if(pstFrame->u32Width != klib_fwrite(ptr,pstFrame->u32Width, fp))
            if(pstFrame->u32Width != VPSS_OSAL_fwrite(pu8Ydata,pstFrame->u32Width, fp))
      	    {
                VPSS_FATAL("line %d: fwrite fail!\n",__LINE__);
            }
            ptr += pstFrame->stBufAddr[0].u32Stride_Y;
        }
        /*
        ptr = pstMMZ->u32StartVirAddr 
            + (pstFrame->stBufAddr[0].u32PhyAddr_C 
            - pstFrame->stBufAddr[0].u32PhyAddr_Y);
            */

        /* U V 数据 转存*/
        for (i=0; i<pstFrame->u32Height/2; i++)
        {
            for (j=0; j<pstFrame->u32Width/2; j++)
            {
                if(pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
                {
                    pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                    pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                }
                else
                {
                    pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                    pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                }
            }
            ptr += pstFrame->stBufAddr[0].u32Stride_C;
        }
        /*写 U */
        VPSS_OSAL_fwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);

        /*写 V */
        VPSS_OSAL_fwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);
        

        VPSS_OSAL_fclose(fp);
        VPSS_FATAL("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                    str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);

        VPSS_VFREE(pu8Udata);
        VPSS_VFREE(pu8Vdata);
        VPSS_VFREE(pu8Ydata);
	}
	
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
