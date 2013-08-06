/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: had-dma.c
 * Description: aiao alsa dma interface
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    HISI Audio Team
 ********************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <asm/io.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/wait.h>
#ifdef CONFIG_PM
 #include <linux/pm.h>
#endif
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/control.h>
#include <sound/initval.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "drv_ao_func.h"
#include "alsa_aiao_proc_func.h"
#include "alsa_aiao_comm.h"
#include "hi_drv_ao.h"

#define DSP0TOA9_IRQ_NUM  51
#define AO_BUFFER_SIZE    1024*8
#define INITIAL_VALUE  0xffffffff
#define AIAO_DF_PeriodBufSize 2048

//#define AIAO_ALSA_DEBUG
#ifdef AIAO_ALSA_DEBUG
#define ATRP()    printk(KERN_ALERT"\nfunc:%s line:%d ", __func__, __LINE__)
#define ATRC    printk
#else
#define ATRP() 
#define ATRC(fmt, ...) 
#endif

//#define ALSA_TIME_DEBUG
#ifdef  ALSA_TIME_DEBUG
struct timespec curtime;
#endif

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
#define ALSA_PROC_NAME "hi_aiao_data"
#endif

static const struct snd_pcm_hardware aiao_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED |
				  SNDRV_PCM_INFO_BLOCK_TRANSFER |
				  SNDRV_PCM_INFO_PAUSE |
				  SNDRV_PCM_INFO_RESUME,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S24_LE,
	.channels_min		= 2,
	.channels_max	= 2,
	.period_bytes_min	= 0x3c0,     
       .period_bytes_max	= 0xf00,    
	.periods_min		= 4,
	.periods_max		= 16,
	.buffer_bytes_max	= 0xf000,
};

extern HI_U32 aiao_isr_num;
static irqreturn_t dsp0Isr(int irq, void * dev_id)
{
    struct snd_pcm_substream *substream = dev_id;
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    HI_U32 isr_cnt;
    HI_U32 hw_readpos = 0;
    
    //ATRP();
    had->isr_total_cnt++;

    clr_dsp_int();
    if (unlikely(!substream))
    {
    	pr_err("%s: null substream\n", __func__);
    	return IRQ_HANDLED;
    }
    
    if(1 == had->first_irq)
    {
        ATRP();
        had->local_isr_num = aiao_isr_num;
        isr_cnt = 1;
    }
    else
        isr_cnt = aiao_isr_num  - had->local_isr_num - had->isr_discard_cnt + 1;
    
    if((isr_cnt%had->sthwparam.periods))
        hw_readpos = (isr_cnt  * AIAO_DF_PeriodBufSize) % (AIAO_DF_PeriodBufSize * had->sthwparam.periods);
    else
        hw_readpos = had->sthwparam.periods * AIAO_DF_PeriodBufSize;

    had->aiao_isr_num = aiao_isr_num;
    had->first_irq = 0;

#if 1
    aoe_get_AipReadPos(had->dma_index, &had->aoe_readpos);
    //ATRC("\naoe_get_AipReadPos 0x%x", had->aoe_readpos);
/*************************
                     | hw_write
       --------------------
           | aoe_read
*************************/
    if(had->runtime_appl_offset > had->aoe_readpos)
    {
        if((hw_readpos > had->aoe_readpos)&(hw_readpos <= had->runtime_appl_offset))
        {
            had->isr_discard_cnt++;
            return IRQ_HANDLED;
        }
    }
 /*************************
             | hw_write
       --------------------
                       | aoe_read
*************************/
    if(had->runtime_appl_offset < had->aoe_readpos)
    {
        if((hw_readpos > had->aoe_readpos)||(hw_readpos <= had->runtime_appl_offset))
        {
            had->isr_discard_cnt++;
            return IRQ_HANDLED;
        }
    }
#endif

    had->hw_readpos = hw_readpos;
    snd_pcm_period_elapsed(substream);
    
    return IRQ_HANDLED;
}

static int hi_dma_prepare(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);

    ATRP();

    had->local_isr_num = 0;
    had->isr_total_cnt = 0;
    had->isr_discard_cnt = 0;
    had->first_irq = 1;

    had->ack_cnt = 0;
    had->last_pos = 0;
    had->runtime_appl_ptr = 0;
    had->aoe_write_ptr = 0;
    had->aoe_write_offset = 0;   
    had->runtime_appl_offset = 0;   
    had->aoe_updatewptr_offset = 0;

    had->pointer_frame_offset = 0;
    //To Test 
    had->hw_readpos = 0;
    //TO DO
    aiao_isr_num = 0;       //reset state to create state
    aoe_dma_flushbuf(had->dma_index);   //prepare to restart dma buf
    return aoe_dma_prepare(had->dma_index, NULL);
}

static int hi_dma_trigger(struct snd_pcm_substream *substream, int cmd)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret = 0;

    ATRP();
    
#ifdef  ALSA_TIME_DEBUG
	getnstimeofday(&curtime);
	ATRC("\n----trigger--:%d . %u\n", tmp.tv_sec, tmp.tv_nsec);
#endif

    switch(cmd) 
    {
    case SNDRV_PCM_TRIGGER_START:
    case SNDRV_PCM_TRIGGER_RESUME:
    case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
        if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
        {
            had->local_isr_num = aiao_isr_num;   //when pause  to start in time without prepare
            ret = aoe_dma_start(had->dma_index, NULL);
            if(ret)
            {   
                ATRC("AIAO ALSA start dma Fail \n");
            }
        }
        if(substream->stream == SNDRV_PCM_STREAM_CAPTURE)
        {
            //TODO CAPTURE
        }
        break;
    case SNDRV_PCM_TRIGGER_STOP:
    case SNDRV_PCM_TRIGGER_SUSPEND:
    case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
        if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
        {
            ret = aoe_dma_stop(had->dma_index, NULL);
            if(ret)
            {
                ATRC("AIAO ALSA stop dma Fail \n");
            }
        }
        if(substream->stream == SNDRV_PCM_STREAM_CAPTURE)
        {
            //TODO CAPTURE
        }
        break;
        
    default:
        ret = -EINVAL;
        break;
    }
    return 0;
}

static int hi_dma_mmap(struct snd_pcm_substream *substream,
	struct vm_area_struct *vma)
{
    //TODO 
#if 0
    //struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    //struct snd_soc_platform *platform = soc_rtd->platform;
    //struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    
    struct snd_pcm_runtime *runtime = substream->runtime;
    //hw base addr
    //unsigned long hw_base;
    int ret;
    unsigned int size;
    ATRP();

    vma->vm_flags |= VM_IO | VM_RESERVED;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    size = vma->vm_end - vma->vm_start;

    //here  just for kernel ddr linear area
    ret = io_remap_pfn_range(vma,
                vma->vm_start,
                runtime->dma_addr >> PAGE_SHIFT,
                size, 
                vma->vm_page_prot);

    if (ret)
        return -EAGAIN;
#endif
	return 0;
}

static snd_pcm_uframes_t hi_dma_pointer(struct snd_pcm_substream *substream)
{
    struct snd_pcm_runtime *runtime = substream->runtime;
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    snd_pcm_uframes_t frame_offset = 0;
    unsigned int bytes_offset = 0;

    //ATRP();
#ifdef  ALSA_TIME_DEBUG
	getnstimeofday(&curtime);
	ATRC("\n----pointer--:%d . %u\n", tmp.tv_sec, tmp.tv_nsec);
#endif

    bytes_offset = had->hw_readpos;
    if(bytes_offset >= snd_pcm_lib_buffer_bytes(substream))
 		bytes_offset = 0;
     
    frame_offset = bytes_to_frames(runtime, bytes_offset);
    //ATRC("\npointer return frame :0x%x   hw_readpos : 0x%x\n", frame_offset, bytes_offset);
    
    had->pointer_frame_offset = frame_offset;

    return frame_offset;
}

static int hi_dma_hwparams(struct snd_pcm_substream *substream,
    struct snd_pcm_hw_params *params)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_pcm_runtime *runtime = substream->runtime;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    //int stream = substream->stream;
    int buffer_bytes = params_buffer_bytes(params);
    //int period_bytes = params_period_bytes(params);
    int dma_index;
    AO_BUF_ATTR_S stDmaAttr;
    HI_UNF_SND_ATTR_S stAttr;
    int ret = 0;
    
    ATRP();

    dma_index = INITIAL_VALUE;
    if(snd_pcm_lib_malloc_pages(substream, buffer_bytes) < 0)
        return -ENOMEM;

    //STEP 1 open snd device 
    if(!had->open_cnt)
    {
        ret = aoe_dma_getopendefparam(&stAttr);
        if(HI_SUCCESS != ret)
        {
            ret = -EINVAL;
            goto err_AllocateDma;
        }
#ifdef AIAO_ALSA_DEBUG
        ATRC("\nbuffer_bytes : 0x%x \n", buffer_bytes);
        //ATRC("\nperiod_bytes : 0x%x \n", period_bytes);
        ATRC("\nstAttr.enSampleRate : 0x%x \n", (int)stAttr.enSampleRate);
        ATRC("\nstAttr.u32PortNum : 0x%x \n", stAttr.u32PortNum);
 #endif      
        ret = aoe_dma_open(&stAttr, &had->file);
        if(HI_SUCCESS != ret)
        {
            ret =  -EINVAL;             //TODO  different sample rete  
            goto err_AllocateDma;
        }
      had->open_cnt++;
    }
       
    if(INITIAL_VALUE == had->dma_index)
    {
        stDmaAttr.u32BufPhyAddr = runtime->dma_addr;    //to check addr attr
        stDmaAttr.u32BufVirAddr = (int)runtime->dma_area;
        stDmaAttr.u32BufSize = runtime->dma_bytes;
       ret = aoe_dma_requestchan(&dma_index, &had->file, &stDmaAttr);
        if(ret)
        {
            ATRC("AOE request dma fail \n");
            goto err_OpenDma;
        }
        //ATRC("\nAOE request new track dma %d \n", dma_index);
    }
   
    had->dma_index = dma_index;
    had->sthwparam.channels = params_channels(params);
    had->sthwparam.rate = params_rate(params);
    had->sthwparam.format = params_format(params);
    had->sthwparam.periods = params_periods(params);
    had->sthwparam.period_size = params_period_size(params);
    had->sthwparam.buffer_size = params_buffer_size(params);
    had->sthwparam.buffer_bytes = params_buffer_bytes(params);
    //had->sthwparam.frame_size = frames_to_bytes(runtime, 1);

    switch (had->sthwparam.format) {
    case SNDRV_PCM_FORMAT_S16_LE:
    	had->sthwparam.frame_size = 2 * had->sthwparam.channels;
    	break;
    case SNDRV_PCM_FMTBIT_S24_LE:
    	had->sthwparam.frame_size = 3 * had->sthwparam.channels;
    	break;
    default:
        break;
    }
    
#ifdef AIAO_ALSA_DEBUG
    printk("\nhad->sthwparam.channels : 0x%x", had->sthwparam.channels);
    printk("\nhad->sthwparam.rate : 0x%x", had->sthwparam.rate);
    printk("\nhad->sthwparam.periods : 0x%x", had->sthwparam.periods);
    printk("\nhad->sthwparam.period_size : 0x%x", (int)had->sthwparam.period_size);
    printk("\nhad->sthwparam.buffer_size : 0x%x", (int)had->sthwparam.buffer_size);
    printk("\nhad->sthwparam.frame_size : 0x%x", had->sthwparam.frame_size);
#endif

#ifdef USE_DSP_ISR    
    if(had->irq_num == INITIAL_VALUE)
    {
        if (request_irq(DSP0TOA9_IRQ_NUM, dsp0Isr, IRQF_DISABLED, "DSP0TOA9", substream) != 0)  
        {
            HI_FATAL_AO("DSP0TOA9 request_irq failed irq num =%d!\n", DSP0TOA9_IRQ_NUM);
            //goto err_RequestChan;
        }
        had->irq_num = DSP0TOA9_IRQ_NUM;
    }
#endif

    return ret;
    //ret = aoe_dma_releasechan(dma_index);
err_OpenDma:
   ret = aoe_dma_close(&had->file);
err_AllocateDma:
   ret = snd_pcm_lib_free_pages(substream);

    return HI_FAILURE;
}

static int hi_dma_hwfree(struct snd_pcm_substream *substream)
{
    int ret = HI_SUCCESS;
    return ret;
}
static int hi_dma_open(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret = 0;
 
    ATRP();
    ret = snd_soc_set_runtime_hwparams(substream, &aiao_hardware);
    if(ret)
        return ret;

    //interrupt by step
    ret = snd_pcm_hw_constraint_integer(substream->runtime, SNDRV_PCM_HW_PARAM_PERIODS);

    had->irq_num = INITIAL_VALUE;
    had->dma_index= INITIAL_VALUE;
    had->open_cnt = 0;
    had->first_irq = 1;
    had->hw_readpos = 0;
    had->aoe_readpos = 0;
    had->aoe_freesize = INITIAL_VALUE;
    //isr
    had->aiao_isr_num = 0;
    had->local_isr_num = 0;
    had->isr_total_cnt= 0;
    had->isr_discard_cnt= 0;

    had->ack_cnt = 0;
    had->last_pos = 0;   
    had->runtime_appl_ptr = 0;
    had->aoe_write_ptr = 0;
    had->aoe_write_offset = 0;   
    had->runtime_appl_offset = 0;   
    had->aoe_updatewptr_offset = 0;
    
#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
   had->pointer_frame_offset = 0;
#endif
    
    return ret;
}

static int hi_dma_close(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret;

    ATRP();

    if(INITIAL_VALUE != had->dma_index)
    {
        ret = aoe_dma_releasechan(had->dma_index);
        if(ret)
            ATRC("aoe_dma_releasechan dma  num =%d fail !\n", had->dma_index);
        else 
            had->dma_index = INITIAL_VALUE;
    }
    if(had->open_cnt)
    {
        ret = aoe_dma_close(&had->file);
        if(ret)
            ATRC("\naoe_dma_close dma  fail !\n");
        else
             had->open_cnt--;
    }

#ifdef USE_DSP_ISR    
    if(had->irq_num != INITIAL_VALUE)
    {
        free_irq(DSP0TOA9_IRQ_NUM, substream);
        had->irq_num = INITIAL_VALUE;
    }
    had->isr_total_cnt = 0;
    had->isr_discard_cnt = 0;
#endif

    snd_pcm_lib_free_pages(substream);

    return HI_SUCCESS;
}

static int hi_dma_ack(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_pcm_runtime *runtime = substream->runtime;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int real_size;
    int current_pos = 0;

    //ATRP();
    
    current_pos = runtime->control->appl_ptr;
    real_size = (current_pos - had->last_pos)*had->sthwparam.frame_size;


#ifdef  ALSA_TIME_DEBUG
	getnstimeofday(&curtime);
	ATRC("\n----ack--:%d . %u\n", curtime.tv_sec, curtime.tv_nsec);
#endif

#ifdef AIAO_ALSA_DEBUG
    if(had->ack_cnt < 8)
        ATRC("\n func:hi_dma_ack real_size:0x%x ", real_size);
#endif
    had->ack_cnt++;

    had->runtime_appl_ptr = runtime->control->appl_ptr * had->sthwparam.frame_size;
    had->aoe_write_ptr += real_size;   
    had->runtime_appl_offset = had->runtime_appl_ptr % had->sthwparam.buffer_bytes;
    had->aoe_write_offset = had->aoe_write_ptr % had->sthwparam.buffer_bytes;   
    had->aoe_updatewptr_offset= real_size;
#if 0
    aoe_get_AipReadPos(had->dma_index, &had->aoe_readpos);
    aoe_get_freesize(had->dma_index, &had->aoe_freesize);
    if(real_size > had->aoe_freesize)
    {
        had->aoe_overwrite_cnt++;
    }
#endif    
    aoe_update_writeptr(had->dma_index, &(real_size));

    had->last_pos = current_pos;
    return 0;
}

static struct snd_pcm_ops hi_dma_ops = {
    .open = hi_dma_open,
    .close = hi_dma_close,
    .ioctl  = snd_pcm_lib_ioctl,
    .hw_params = hi_dma_hwparams,
    .hw_free = hi_dma_hwfree,
    .prepare = hi_dma_prepare,
    .trigger = hi_dma_trigger,
    .pointer = hi_dma_pointer,
    .mmap = hi_dma_mmap,
    .ack = hi_dma_ack,
};

static int hi_dma_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
    struct snd_pcm *pcm = rtd->pcm;
    int ret = 0;
    if(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream || 
        pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream)
    {
        ret = snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,NULL,
        AO_BUFFER_SIZE, AO_BUFFER_SIZE / 2);
        if(ret)
        {
            ATRC("dma buffer allocation fail \n");
            return ret;
        }
    }
    return ret;
}

static void hi_dma_pcm_free(struct snd_pcm *pcm)
{
    snd_pcm_lib_preallocate_free_for_all(pcm);
}

static int hi_dma_probe(struct snd_soc_platform *soc_platform)
{
    int ret = 0;
    
#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    struct hiaudio_data *had = dev_get_drvdata(soc_platform->dev);
    ret = hiaudio_proc_init(soc_platform->card->snd_card, ALSA_PROC_NAME, had);
    if(ret < 0)
    {
        //ATRC("had_init_debugfs fail %d", ret);
    }
#endif
    
    return ret;
}

static struct snd_soc_platform_driver aiao_soc_platform_drv =
{
    .ops         = &hi_dma_ops,
    .pcm_new   = hi_dma_pcm_new,
    .pcm_free   = hi_dma_pcm_free,
    .probe      = hi_dma_probe,
};


static int __devinit soc_snd_platform_probe(struct platform_device *pdev)
{
    struct hiaudio_data *had;
    int ret = -EINVAL;

    had = kzalloc(sizeof(struct hiaudio_data), GFP_KERNEL);
    if(had == NULL)
        return -ENOMEM;

    dev_set_drvdata(&pdev->dev, had);

    //init had data struct
    mutex_init(&had->mutex);
    had->irq_num = INITIAL_VALUE;
    had->first_irq = 1;
    had->open_cnt = 0;
    had->dma_index = INITIAL_VALUE;
    had->hw_readpos = 0;
    had->aoe_readpos = 0;
    had->aoe_freesize = INITIAL_VALUE;
    had->aoe_overwrite_cnt = 0;
    had->local_isr_num = 0;
    had->aiao_isr_num = 0;

    had->isr_total_cnt = 0;
    had->isr_discard_cnt = 0;

    had->ack_cnt = 0;
    had->last_pos = 0;
    had->runtime_appl_ptr = 0;
    had->aoe_write_ptr = 0;
    had->aoe_updatewptr_offset = 0;
    had->pointer_frame_offset = 0;

    memset(&had->sthwparam, 0, sizeof(struct hisi_aiao_hwparams));

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    had->entry = NULL;
#endif

    ret = snd_soc_register_platform(&pdev->dev, &aiao_soc_platform_drv);
    if (ret < 0)
        goto err;

    return ret;
err:
    kfree(had);
    return ret;
}

static int __devexit soc_snd_platform_remove(struct platform_device *pdev)
{
    struct hiaudio_data *had = dev_get_drvdata(&pdev->dev);

    if(had)
        kfree(had);

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    hiaudio_proc_cleanup();
#endif

    snd_soc_unregister_platform(&pdev->dev);

    return 0;
}

static struct platform_driver hiaudio_dma_driver = {
    .driver = {
        .name = "hisi-audio",
        .owner = THIS_MODULE,
    },
    .probe = soc_snd_platform_probe,
    .remove = __devexit_p(soc_snd_platform_remove),
};

static struct platform_device *hiaudio_dma_device;

int hiaudio_dma_init(void)
{
    int ret =0;
    
    hiaudio_dma_device = platform_device_alloc("hisi-audio", -1);
    if (!hiaudio_dma_device) {
    	HI_FATAL_AO(KERN_ERR "hiaudio-dma SoC platform device: Unable to register\n");
    	return -ENOMEM;
    }
    
    ret = platform_device_add(hiaudio_dma_device);
    if (ret) {
    	HI_FATAL_AO(KERN_ERR "hiaudio-dma SoC platform device: Unable to add\n");
    	platform_device_put(hiaudio_dma_device);
       return ret;
    }

    return platform_driver_register(&hiaudio_dma_driver);
}

void hiaudio_dma_deinit(void)
{
    platform_device_unregister(hiaudio_dma_device);
    platform_driver_unregister(&hiaudio_dma_driver);
}
