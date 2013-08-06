/*
 * jdapistd.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the decompression half
 * of the JPEG library.  These are the "standard" API routines that are
 * used in the normal full-decompression case.  They are not used by a
 * transcoding-only application.  Note that if an application links in
 * jpeg_start_decompress, it will end up linking in the entire decompressor.
 * We thus must separate this file from jdapimin.c to avoid linking the
 * whole decompression library into a transcoder.
 */

#define JPEG_INTERNALS
#include <sys/ioctl.h>

#include "jinclude.h"
#include "jpeglib.h"
#include "jdatasrc.h"

/********************* add some file at here *********************/
#include  "hijpeg_decode_hw.h"

#include  "hijpeg_type.h"
#include  "hi_jpg_ioctl.h"

/* Forward declarations */
LOCAL(boolean) output_pass_setup JPP((j_decompress_ptr cinfo));
LOCAL(void) JPEG_GetProcMessage(j_decompress_ptr cinfo);

/*
 * Decompression initialization.
 * jpeg_read_header must be completed before calling this.
 *
 * If a multipass operating mode was selected, this will do all but the
 * last pass, and thus may take a great deal of time.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */
LOCAL(boolean) start_decompress(j_decompress_ptr cinfo,JPEG_MESSAGE_S  *pstMessagePrivate)
{

  if (cinfo->global_state == DSTATE_READY) {
      
        /* First call: initialize master control, select active modules */
        jinit_master_decompress(cinfo);
        if(TRUE == pstMessagePrivate->ErrMsg){
            return FALSE;
        }
        if (cinfo->buffered_image) {
              /* No more work here; expecting jpeg_start_output next */
              cinfo->global_state = DSTATE_BUFIMAGE;
              return TRUE;
        }
        cinfo->global_state = DSTATE_PRELOAD;
  }
  
  if (cinfo->global_state == DSTATE_PRELOAD) {
          /* If file has multiple scans, absorb them all into the coef buffer */
         if (cinfo->inputctl->has_multiple_scans) {
	         #ifdef D_MULTISCAN_FILES_SUPPORTED
	         for (;;) {
		         int retcode;
		         /* Call progress monitor hook if present */
		         if (cinfo->progress != NULL)
		              (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
		         /* Absorb some more input */
		         retcode = (*cinfo->inputctl->consume_input) (cinfo);
				 if(TRUE == pstMessagePrivate->ErrMsg){
		                return FALSE;
  	             }
		         if (retcode == JPEG_SUSPENDED)
		                return FALSE;
		         if (retcode == JPEG_REACHED_EOI)
		                break;
		         /* Advance progress counter if appropriate */
		         if (cinfo->progress != NULL &&
		              (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS))
		              {
		                   if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
		                       /* jdmaster underestimated number of scans; ratchet up one scan */
		                      cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
		                    }
	                  }
            }
           #else
              ERREXIT(cinfo, JERR_NOT_COMPILED);
              return FALSE;
          #endif /* D_MULTISCAN_FILES_SUPPORTED */
       }
       cinfo->output_scan_number = cinfo->input_scan_number;

   } else if (cinfo->global_state != DSTATE_PRESCAN){
           ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
           return FALSE;
    }
  /* Perform any dummy output passes, and set up for the final pass */


    #if 1
   /** if have call setmode and soft decode, we should save all data **/
    if(FALSE == JPEG_Api_CheckAddStructMessage((j_common_ptr)cinfo,pstMessagePrivate))
    {
        return FALSE;
    }
	/*** when have error happen, followed can not operation ****/
    if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode){
		    HI_S32 s32RowStride = 0;
			switch(cinfo->out_color_space)
			{
			    case JCS_RGB:
			    case JCS_BGR:
					s32RowStride = (((cinfo->output_width*3)+15)&(~15));
					break;
				case JCS_RGB_565:
				case JCS_BGR_565:
					s32RowStride = (((cinfo->output_width*2)+15)&(~15));
					break;
			    case JCS_RGBA_8888:
				case JCS_BGRA_8888:
		 			s32RowStride = (((cinfo->output_width*4)+15)&(~15));
					break;
				case JCS_YCbCr:
				case JCS_CrCbY:
				    s32RowStride = (((cinfo->output_width*3)+15)&(~15));
					break;
				default:
                    #ifdef JPEG6B_DEBUG
					HI_JPEG_TRACE("the color space can not support\n");
                    #endif
					break;
			}
			pstMessagePrivate->SetModeOutBuffer = (*cinfo->mem->alloc_sarray)
						((j_common_ptr) cinfo, JPOOL_IMAGE, s32RowStride, 1);
     }

  #endif
			
  return output_pass_setup(cinfo);


}

/*****************************************************************************
* func        : duplicate_stream_info
* description : 备份码流信息
* param[in]   : 
* param[in]   :
* param[in]   :
* others:	  : nothing
*****************************************************************************/
static HI_VOID duplicate_stream_info(JPEG_MESSAGE_S  *pstMessagePrivate, \
                                               j_decompress_ptr cinfo)
{

   	   my_src_ptr src = (my_src_ptr) cinfo->src;
	   
       /** 进入解码前备份码流信息,这样软解的时候可以少一次拷贝 **/
   	   if(STREAM_TYPE_INTER_FILE==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType)
	   {
		   /** 目前只对读文件方式做了处理,读码流方式还没有方案 **/
		   /** 进入硬解之前获取当前文件所在的位置 **/
		   pstMessagePrivate->sJpegHtoSInfo.s32FilePos = ftell(src->infile);
		   memcpy(pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf,      \
		   	      (char*)cinfo->src->next_input_byte,              \
		   	      cinfo->src->bytes_in_buffer);
		   	/** buffer中剩余的码流 **/
	       pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte = cinfo->src->bytes_in_buffer;
			
	   }
	   else if(STREAM_TYPE_INTER_STREAM==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType)
	   {
            pstMessagePrivate->sJpegHtoSInfo.s32StreamPos = src->stBufStream.pos;
			#ifndef STREAM_DO_NOT_NEED_MEMCPY
			memcpy(pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf,     \
		   	      (char*)cinfo->src->next_input_byte,              \
		   	      cinfo->src->bytes_in_buffer);
			#endif
		    /** buffer中剩余的码流 **/
	        pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte = cinfo->src->bytes_in_buffer;
			
	   }
	   /** 2,3==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType则由外部去处理 **/

				   
}
/*****************************************************************************
* func        : resume_stream
* description : 硬解失败恢复码流为硬解之前的状态
* param[in]   : 
* param[in]   :
* param[in]   :
* others:	  : nothing
*****************************************************************************/
static HI_VOID resume_stream(JPEG_MESSAGE_S  *pstMessagePrivate, \
                                          j_decompress_ptr cinfo)
{

	   
	   
       HI_S32 s32Ret;
   	   my_src_ptr src = (my_src_ptr) cinfo->src;

	   if(STREAM_TYPE_INTER_FILE==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType)
	   {
		   s32Ret = fseek(src->infile,pstMessagePrivate->sJpegHtoSInfo.s32FilePos,SEEK_SET);
		   if(HI_SUCCESS!=s32Ret)
		   {
				#ifdef JPEG6B_DEBUG
	            HI_JPEG_TRACE("硬解失败后文件返回失败!\n");
	            #endif
		   }
		   /** 硬解之前剩余的码流拷回 **/
		   memcpy((char*)cinfo->src->next_input_byte,             \
		   	      pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf,     \
		   	      pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte);

		   cinfo->src->bytes_in_buffer = pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte;
		   
	   }
	   else if(STREAM_TYPE_INTER_STREAM==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType) 
	   {
	        
            src->stBufStream.pos = pstMessagePrivate->sJpegHtoSInfo.s32StreamPos;
			if(0==src->stBufStream.pos)
			{
				#ifdef JPEG6B_DEBUG
	            HI_JPEG_TRACE("回退码流失败!\n");
	            #endif
			}
			#ifdef STREAM_DO_NOT_NEED_MEMCPY
			/** 如果是指针方式就要回退指针位置,不然会释放内存错误 **/
			cinfo->src->next_input_byte = (unsigned char*)((src->stBufStream.img_buffer)     \
			                      +(pstMessagePrivate->sJpegHtoSInfo.s32StreamPos)  \
			                      -(pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte));
			#else
		    memcpy((char*)cinfo->src->next_input_byte, \
		   	        pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf,\
		   	        pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte);
			#endif
			
			cinfo->src->bytes_in_buffer = pstMessagePrivate->sJpegHtoSInfo.s32LeaveByte;
			
	   }
       /** 2,3==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType则由外部去处理 **/
	   
}

GLOBAL(boolean)
jpeg_start_decompress (j_decompress_ptr cinfo)
{
			
       JPEG_MESSAGE_S  *pstMessagePrivate;
       j_common_ptr pCinfo = (j_common_ptr)cinfo;
       pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
       if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
       {
          return FALSE;
       }
       pstMessagePrivate->s32BeforeStartState = cinfo->global_state;
	   //my_src_ptr src = (my_src_ptr) cinfo->src;

	   if(HI_TRUE == pstMessagePrivate->bCannotSupport)
       {/**
         ** 这个是针对应用层读码流硬解失败重新调用该接口时要是颜色
         ** 空间不支持做的判断，其他情况不应走这里 
         **/
            fprintf(stderr,">>>>>>>> %s : %d \n",__FUNCTION__,__LINE__);
            return FALSE;
       }
	              
       /** hard decode at here, because the read_scanlines is called with loops,
        ** so the hard decode can called alse with loops, it is not appropriate
        **/
       if(HI_TRUE==pstMessagePrivate->sJpegHtoSInfo.bHDECSuc)
       {/**
         ** 第一次走该通道,重新调用一次就不走该通道了,主要是针对外部自己读码流方式的
         ** 外部通过调用hi_get_dec_state获取硬件解码状态,要是解失败则再调用该接口走
         ** 软解
         **/
	       if(cinfo->global_state == DSTATE_READY)
		   {
	          if( (TRUE == JPEG_Decode_IsHWSupport(cinfo)) && (JCS_YCbCr!=cinfo->out_color_space))
			  {
	            if(TRUE == JPEG_Decode_OpenDev(pCinfo))
				{
	               if(TRUE == JPEG_Decode_SetParameter(cinfo))
				   {
				       /** 备份码流信息 **/
				       /** 不要用malloc分配,不然不好释放 **/
				       if(STREAM_TYPE_INTER_FILE==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType \
						   	       || STREAM_TYPE_INTER_STREAM==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType)
				       {
				            duplicate_stream_info(pstMessagePrivate,cinfo);
				       }
	                   if(TRUE == JPEG_Decode_SendStream(cinfo))
					   {
		                    pstMessagePrivate->HaveCallStartDec = FALSE;
		                    JPEG_GetProcMessage(cinfo);
		                    return TRUE;
	                   }
	                   else
					   {
					       cinfo->global_state = pstMessagePrivate->s32BeforeStartState;
						   if(HI_TRUE == pstMessagePrivate->bCannotSupport)
						   {/** 输出为YCbCr且源是灰度图,则解码失败就不再解了,因为解不了**/
						        return FALSE;
						   }
						   else if(STREAM_TYPE_INTER_FILE==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType \
						   	       || STREAM_TYPE_INTER_STREAM==pstMessagePrivate->sJpegHtoSInfo.eReadStreamType)
						   {
						        /**
						         ** 内部读码流及读文件方式就由内部来处理,
						         ** 软件解失败就进入硬件解
						         **/
						        resume_stream(pstMessagePrivate,cinfo);
						        goto SOFT_DEC;
						   }
						   else
						   {/** 都在应用层处理,外部处理读取码流,所以就在外部做处理,重新调用 **/
	                           return FALSE;
						   }
						   #if 0
						   else
						   {/**
						     ** 记住应用层必须做处理,使用hi_jpeg_api.h文件的一些信息
						     ** 其实这里也进不来,因为假如硬解失败应用层会重新调用该函数
						     ** 但不走硬解通道了
						     **/
	                            goto SOFT_DEC;
						   }
						   #endif
						   
	                       /** else get the proc message at output_pass_setup **/
							
	                   }
					   
	               }
	            }
	         }
	       }
		   
       }
       /** if hard decode failure, to beging soft decode and the golbal_state 
        ** should return the state that before hard decode,because the start_decompress
        ** should use the golbal state.
        **/
       SOFT_DEC:
	       pstMessagePrivate->HaveCallStartDec = TRUE;
	       return start_decompress(cinfo,pstMessagePrivate);

}

/*
 * Set up for an output pass, and perform any dummy pass(es) needed.
 * Common subroutine for jpeg_start_decompress and jpeg_start_output.
 * Entry: global_state = DSTATE_PRESCAN only if previously suspended.
 * Exit: If done, returns TRUE and sets global_state for proper output mode.
 *       If suspended, returns FALSE and sets global_state = DSTATE_PRESCAN.
 */

LOCAL(boolean)
output_pass_setup (j_decompress_ptr cinfo)
{

  JPEG_MESSAGE_S  *pstMessagePrivate;
  j_common_ptr pCinfo = (j_common_ptr)cinfo;
  pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
      
  if (cinfo->global_state != DSTATE_PRESCAN) {
    /* First call: do pass setup */
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    if(TRUE == pstMessagePrivate->ErrMsg){
        return FALSE;
    }
    cinfo->output_scanline = 0;
    cinfo->global_state = DSTATE_PRESCAN;
  }
  /* Loop over any required dummy passes */
  while (cinfo->master->is_dummy_pass) {
#ifdef QUANT_2PASS_SUPPORTED
    /* Crank through the dummy pass */
    while (cinfo->output_scanline < cinfo->output_height) {
      JDIMENSION last_scanline;
      /* Call progress monitor hook if present */
      if (cinfo->progress != NULL) {
	cinfo->progress->pass_counter = (long) cinfo->output_scanline;
	cinfo->progress->pass_limit = (long) cinfo->output_height;
	(*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      }
      /* Process some data */
      last_scanline = cinfo->output_scanline;
      (*cinfo->main->process_data) (cinfo, (JSAMPARRAY) NULL,
				    &cinfo->output_scanline, (JDIMENSION) 0);
      if (cinfo->output_scanline == last_scanline)
	return FALSE;		/* No progress made, must suspend */
    }
    /* Finish up dummy pass, and set up for another one */
    (*cinfo->master->finish_output_pass) (cinfo);
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    if(TRUE == pstMessagePrivate->ErrMsg){
        return FALSE;
    }
    cinfo->output_scanline = 0;
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
    return FALSE;
#endif /* QUANT_2PASS_SUPPORTED */
  }
  /* Ready for application to drive output pass through
   * jpeg_read_scanlines or jpeg_read_raw_data.
   */
  cinfo->global_state = cinfo->raw_data_out ? DSTATE_RAW_OK : DSTATE_SCANNING;

  /** get the proc message at here, do not at read_scanlines, because it is called loops **/
  JPEG_GetProcMessage(cinfo);
  
  return TRUE;
  
}


LOCAL(void) JPEG_GetProcMessage(j_decompress_ptr cinfo)
{
    JPEG_PROC_INFO_S stUserProcInfo;
    
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

     stUserProcInfo.u32ImageWidth = cinfo->image_width;
     stUserProcInfo.u32ImageHeight = cinfo->image_height;
     stUserProcInfo.u32OutputWidth = cinfo->output_width;
     stUserProcInfo.u32OutputHeight = cinfo->output_width;

     stUserProcInfo.eImageFormat = JPEG_Api_GetImagFormat(cinfo);
     
     if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate)){
         return;
     }

	 if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode){
         stUserProcInfo.u32OutputBufAddr =  pstMessagePrivate->stPub.u32OutPhyAddr[0];
	 }
	 if(HI_TRUE == pstMessagePrivate->hardware_support){
	    stUserProcInfo.u32OutputStreamBufAddr = (HI_U32)pstMessagePrivate->mmz_PhyAddr;
	 }
     if(TRUE== pstMessagePrivate->IfHaveCallSetMemMode){
            stUserProcInfo.OutPutPhyBuf = TRUE;
     }
     else{
            stUserProcInfo.OutPutPhyBuf = FALSE;
     }

     stUserProcInfo.eDecodeState = JPEG_STATE_DECING;
     
     if(TRUE == pstMessagePrivate->hardware_support)
     {
        stUserProcInfo.eDecodeType = JPEG_DECODETYPE_HW;
     }

     else
     {
       stUserProcInfo.eDecodeType = JPEG_DECODETYPE_SW;
     }
     stUserProcInfo.s32ImageScale = (cinfo->scale_denom)/(cinfo->scale_num);   
	 stUserProcInfo.s32OutPutComponents = cinfo->output_components;

	 J_COLOR_SPACE eOutputColorspace;
	 eOutputColorspace   = cinfo->out_color_space;
     stUserProcInfo.eOutputColorspace = (HI_U32)eOutputColorspace;  
     
     ioctl(pstMessagePrivate->jpg, CMD_JPG_READPROC, &stUserProcInfo);
     
}

/*
 * Read some scanlines of data from the JPEG decompressor.
 *
 * The return value will be the number of lines actually read.
 * This may be less than the number requested in several cases,
 * including bottom of image, data source suspension, and operating
 * modes that emit multiple scanlines at a time.
 *
 * Note: we warn about excess calls to jpeg_read_scanlines() since
 * this likely signals an application programmer error.  However,
 * an oversize buffer (max_lines > scanlines remaining) is not an error.
 */

GLOBAL(JDIMENSION)
jpeg_read_scanlines (j_decompress_ptr cinfo, JSAMPARRAY scanlines,
		     JDIMENSION max_lines)
{

      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
    {
        return -1;
    }
    JDIMENSION row_ctr;

   if(TRUE == pstMessagePrivate->HaveDoneSentStream){
       row_ctr = JPEG_Decode_TDEColorConvert(cinfo,scanlines,max_lines);  
       if(row_ctr>0){
          return row_ctr;
       }
       else{
           /** return before call start_decompress() data **/
           cinfo->global_state = pstMessagePrivate->s32BeforeStartState;
       } 
   }
   /**************************************************************/
  
   /***************************************************************/

   /** soft decode **/
   #if 1
   if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode){
       scanlines =  pstMessagePrivate->SetModeOutBuffer; 
   }
   #endif
   
   if(FALSE  == pstMessagePrivate->HaveCallStartDec){
         start_decompress(cinfo,pstMessagePrivate);
   }
  if (cinfo->global_state != DSTATE_SCANNING){
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
    return -1;
    }
  if (cinfo->output_scanline >= cinfo->output_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->output_scanline;
    cinfo->progress->pass_limit = (long) cinfo->output_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Process some data */
  row_ctr = 0;
  (*cinfo->main->process_data) (cinfo, scanlines, &row_ctr, max_lines);
  cinfo->output_scanline += row_ctr;


  #if 1
  if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode){
  	/** physical buffer output, so you should save the line data to 
  	 ** map buffer one by one
  	 **/
  	 HI_S32 s32RowNum;
	 HI_UCHAR* retBuf = (HI_UCHAR* )pstMessagePrivate->stPub.pu32OutVirAddr[0];
  	 s32RowNum = (pstMessagePrivate->stPub.s32OutStride[0]) * (cinfo->output_scanline-1);
 	 memcpy(&retBuf[s32RowNum],scanlines[0], \
	 	                    pstMessagePrivate->stPub.s32OutStride[0]);
  }
  #endif

   /*** The proc message ***/
   if(TRUE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate)){
        JPEG_PROC_INFO_S stUserProcInfo;
        stUserProcInfo.u32OutputBufAddr = (HI_U32)scanlines[0];
        ioctl(pstMessagePrivate->jpg, CMD_JPG_READPROC, &stUserProcInfo);
   }
   /************************************/
   
  return row_ctr;

  
}


/*
 * Alternate entry point to read raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */

GLOBAL(JDIMENSION)
jpeg_read_raw_data (j_decompress_ptr cinfo, JSAMPIMAGE data,
		    JDIMENSION max_lines)
{
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
    {
        return -1;
    }
  JDIMENSION lines_per_iMCU_row;

  if (cinfo->global_state != DSTATE_RAW_OK){
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
    return -1;
    }
  if (cinfo->output_scanline >= cinfo->output_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->output_scanline;
    cinfo->progress->pass_limit = (long) cinfo->output_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Verify that at least one iMCU row can be returned. */
  lines_per_iMCU_row = cinfo->max_v_samp_factor * cinfo->min_DCT_scaled_size;
  if (max_lines < lines_per_iMCU_row){
    ERREXIT(cinfo, JERR_BUFFER_SIZE);
    return -1;
    }

  /* Decompress directly into user's buffer. */
  if (! (*cinfo->coef->decompress_data) (cinfo, data))
    return 0;			/* suspension forced, can do nothing more */

  /* OK, we processed one iMCU row. */
  cinfo->output_scanline += lines_per_iMCU_row;
  return lines_per_iMCU_row;
}


/* Additional entry points for buffered-image mode. */

#ifdef D_MULTISCAN_FILES_SUPPORTED

/*
 * Initialize for an output pass in buffered-image mode.
 */

GLOBAL(boolean)
jpeg_start_output (j_decompress_ptr cinfo, int scan_number)
{
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
    {
        return FALSE;
    }
  if (cinfo->global_state != DSTATE_BUFIMAGE &&
      cinfo->global_state != DSTATE_PRESCAN){
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
    return FALSE;
    }
  /* Limit scan number to valid range */
  if (scan_number <= 0)
    scan_number = 1;
  if (cinfo->inputctl->eoi_reached &&
      scan_number > cinfo->input_scan_number)
    scan_number = cinfo->input_scan_number;
  cinfo->output_scan_number = scan_number;
  /* Perform any dummy output passes, and set up for the real pass */
  return output_pass_setup(cinfo);
}


/*
 * Finish up after an output pass in buffered-image mode.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

GLOBAL(boolean)
jpeg_finish_output (j_decompress_ptr cinfo)
{
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
    {
        return FALSE;
    }
  if ((cinfo->global_state == DSTATE_SCANNING ||
       cinfo->global_state == DSTATE_RAW_OK) && cinfo->buffered_image) {
    /* Terminate this pass. */
    /* We do not require the whole pass to have been completed. */
    (*cinfo->master->finish_output_pass) (cinfo);
    cinfo->global_state = DSTATE_BUFPOST;
  } else if (cinfo->global_state != DSTATE_BUFPOST) {
    /* BUFPOST = repeat call after a suspension, anything else is error */
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
    return FALSE;
  }
  /* Read markers looking for SOS or EOI */
  while (cinfo->input_scan_number <= cinfo->output_scan_number &&
	 ! cinfo->inputctl->eoi_reached) {

	if(TRUE == pstMessagePrivate->ErrMsg){
		return FALSE;
  	}
    if ((*cinfo->inputctl->consume_input) (cinfo) == JPEG_SUSPENDED)
      return FALSE;		/* Suspend, come back later */
  }
  cinfo->global_state = DSTATE_BUFIMAGE;
  return TRUE;
  
}

#endif /* D_MULTISCAN_FILES_SUPPORTED */
