/*
 * Copyright (c) (2011 - ...) digital media project platform development dept,
 * Hisilicon. All rights reserved.
 *
 * File: vdec.c
 *
 * Purpose: vdec omx adaptor layer
 *
 * Author: sunny. liucan 180788
 *
 * Date: 26, Dec, 2011
 *
 */

#ifndef __VDEC_H__
#define __VDEC_H__

#include "hi_type.h"


#define MAX_OPEN_COUNT	16 
#define MAX_CHAN_NUM	       MAX_OPEN_COUNT   
#define MAX_VPSS_NUM           MAX_OPEN_COUNT

enum vdec_state {
	VDEC_STATE_INVALID = -1,
	VDEC_STATE_IDLE,
	VDEC_STATE_WORK
};

struct vdec_entry {
	enum vdec_state    state;
	HI_U32                   open_count;
	spinlock_t               lock;

	spinlock_t               channel_lock;
	HI_U32                   total_chan_num;
	unsigned long          chan_bitmap;
	struct list_head      chan_list;

	struct cdev cdev;
	struct device *device;
};

HI_U32 get_channel_num(struct vdec_entry *vdec);
HI_VOID release_channel_num(struct vdec_entry *vdec, HI_S32 chan_num);
struct chan_ctx_s *find_match_channel(struct vdec_entry *vdec, HI_S32 chan_num);
struct chan_ctx_s *find_match_channel_by_vpssid(struct vdec_entry *vdec, HI_S32 VpssId);

#endif
