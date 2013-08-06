/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpeg_proc.c
Version		    : 
Author		    : y00181162
Created		    : 2012/10/31
Description	    : 
Function List 	: 
			    : 

History       	:
Date				Author        		Modification
2012/10/31		    y00181162		    Created file      	
******************************************************************************/




/*********************************add include here******************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

/**
 ** use common proc to deal with
 **/
#include "drv_proc_ext.h"

#include "hijpeg_proc.h"
#include "hi_jpeg_config.h"




/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/

JPEG_PROC_INFO_S s_stJpeg6bProcInfo = {0};

static HI_BOOL gs_bProcOn = HI_FALSE;




/******************************* API forward declarations *******************/




/******************************* API realization *****************************/




/*****************************************************************************
* Function     : JPEG_Read_Proc
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/

HI_S32 JPEG_Read_Proc(struct seq_file *p, HI_VOID *v)
{

	if(HI_TRUE==gs_bProcOn)
    {
            DRV_PROC_ITEM_S *item;
            JPEG_PROC_INFO_S *procinfo;
            
            char fmtname[10];
            char DecodeState[15];
            char DecodeType[10];
            char BufImformation[15];
            char OutputColorspace[10];

            item = (DRV_PROC_ITEM_S *)(p->private);
            procinfo = (JPEG_PROC_INFO_S *)(item->data);

            /** the output image format **/
            switch(procinfo->eImageFormat)
            {
            
                    case JPEG_YUV_400:
                        strcpy(fmtname, "YUV400");
                        break;
                        
                    case JPEG_YUV_420:
                        strcpy(fmtname, "YUV420");
                        break;
                        
                    case JPEG_YUV_422_21:
                        strcpy(fmtname, "YUV422_21");
                        break;
                        
                    case JPEG_YUV_422_12:
                        strcpy(fmtname, "YUV422_12");
                        break;
                        
                    case JPEG_YUV_444:
                        strcpy(fmtname, "YUV444");
                        break;
                        
                    default:
                        strcpy(fmtname, "Unknown");
                        break;
                        
            }

            switch(procinfo->eDecodeState)
            {

                 case JPEG_STATE_NOSTART:
                        strcpy(DecodeState, "NoStartDec");
                        break;
                        
                  case JPEG_STATE_DECING:
                        strcpy(DecodeState, "decoding");
                        break;

                  case JPEG_STATE_FINISH:
                        strcpy(DecodeState, "DecFinish");
                        break;
                        
                  case JPEG_STATE_ERR:
                        strcpy(DecodeState, "DecErr");
                        break;
                        
                  default:
                       strcpy(DecodeState, "UnDecode");
                       break;
                        
            }

            
            /** the decode type **/
            switch(procinfo->eDecodeType)
            {
            
                    case JPEG_DECODETYPE_HW:
                        strcpy(DecodeType, "HardDec");
                        break;
                        
                    case JPEG_DECODETYPE_SW:
                        strcpy(DecodeType, "SoftDec");
                        break;
                        
                    default:
                        strcpy(DecodeType, "UnDecode");
                        break;
                        
            }


            /** the buffer imformation **/
            if(HI_TRUE == procinfo->OutPutPhyBuf)
            {
            
               strcpy(BufImformation, "PhyBufOutput");
               
            }
            else
            {
               strcpy(BufImformation, "VirtBufOutput");

            }
            

            /** the buffer imformation **/
            switch(procinfo->eOutputColorspace)
            {

                    case 0:
                        strcpy(OutputColorspace, "Unknow");
                        break;
                        
                    case 1:
                        strcpy(OutputColorspace, "gray");
                        break;
                        
                    case 2:
                        strcpy(OutputColorspace, "RGB");
                        break;

                     case 3:
                        strcpy(OutputColorspace, "YCbCr");
                        break;
                        
                      case 4:
                        strcpy(OutputColorspace, "CMYK");
                        break;

                     case 5:
                        strcpy(OutputColorspace, "YCCK");
                        break;
                        
                     case 6:
                        strcpy(OutputColorspace, "RGB8888");
                        break;

                     case 7:
                        strcpy(OutputColorspace, "RGB565");
                        break;
                        
                    default:
                        strcpy(OutputColorspace, "UnKnow");
                        break;
                        
            }

            
            seq_printf(p, "image_height\t\t:%u\n", procinfo->u32ImageWidth);
            seq_printf(p, "image_height\t\t:%u\n", procinfo->u32ImageHeight);
            seq_printf(p, "output_width\t\t:%u\n", procinfo->u32OutputWidth);
            seq_printf(p, "output_height\t\t:%u\n", procinfo->u32OutputHeight);

            seq_printf(p, "image format\t\t:%s\n", fmtname);

            seq_printf(p, "output buffer address\t:0x%x\n", procinfo->u32OutputBufAddr);
            seq_printf(p, "output buffer information \t\t:%s\n", BufImformation);
            seq_printf(p, "stream buffer address\t:0x%x\n", procinfo->u32OutputStreamBufAddr);

            seq_printf(p, "decode state\t\t:%s\n", DecodeState);
            seq_printf(p, "decode type\t\t:%s\n", DecodeType);
            
            seq_printf(p, "the image scale\t\t:%u\n", procinfo->s32ImageScale);


            seq_printf(p, "the output components \t\t:%u\n", procinfo->s32OutPutComponents);
            
            seq_printf(p, "the output color space\t\t:%s\n", OutputColorspace);

    }
		
    return HI_SUCCESS;
		
}


/*****************************************************************************
* Function     : JPEG_Write_Proc
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/

HI_S32 JPEG_Write_Proc(struct file * file,
    const char __user * pBuf, size_t count, loff_t *ppos) 
{

	    HI_CHAR buf[128];
	    
	    if (count > sizeof(buf))
	    {
	        return 0;
	    }

	    memset(buf, 0, sizeof(buf));

	    if (copy_from_user(buf, pBuf, count))
	    {
	        return 0;
	    }
	    
	    if (strstr(buf, "proc on"))
	    {
	        gs_bProcOn = HI_TRUE;
	        //seq_printf(seq, "jpeg proc on!\n");
	    }
	    else if (strstr(buf, "proc off"))
	    {
	        gs_bProcOn = HI_FALSE;
	        //seq_printf(seq, "jpeg proc off!\n");
	    }

	    return count;
	
}


/*****************************************************************************
* Function     : JPEG_Proc_GetStruct
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/

HI_VOID JPEG_Proc_GetStruct(JPEG_PROC_INFO_S **ppstProcInfo)
{

    *ppstProcInfo = &s_stJpeg6bProcInfo;
    
    return;
    
}



/********* the followed three fun we should call at our routine ************/


/*****************************************************************************
* Function     : JPEG_Proc_init
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/
HI_VOID JPEG_Proc_init(HI_VOID)
{


        #ifndef HIJPEG_GAO_AN_VERSION
		
	    DRV_PROC_EX_S  stProc;

	    stProc.fnRead     = JPEG_Read_Proc;
	    stProc.fnWrite    = JPEG_Write_Proc;
	    stProc.fnIoctl    = NULL;

	    HI_DRV_PROC_AddModuleEx("jpeg", &stProc, (HI_VOID *)&s_stJpeg6bProcInfo);
	    
		#endif
	
}
/*****************************************************************************
* Function     : JPEG_Proc_Cleanup
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/

HI_VOID JPEG_Proc_Cleanup(HI_VOID)
{
     #ifndef HIJPEG_GAO_AN_VERSION
     HI_DRV_PROC_RemoveModule("jpeg");
	 #endif
}


/*****************************************************************************
* Function     : JPEG_Proc_IsOpen
* Description  : 
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/
HI_BOOL JPEG_Proc_IsOpen(HI_VOID)
{

    return gs_bProcOn;
    
}
/*****************************************************************************
* Function     : JPEG_Get_Proc_Status
* Description  : get the proc status
* param[in]    :
* param[in]    :
* Output       :
* retval       :
* retval       :
* others:	   :nothing
*****************************************************************************/

HI_VOID JPEG_Get_Proc_Status(HI_BOOL* pbProcStatus)
{
     #ifndef HIJPEG_GAO_AN_VERSION
     *pbProcStatus = gs_bProcOn;
	 #endif
}
