LOCAL_PATH := $(call my-dir)


#================ PVR smooth static lib ================
include $(CLEAR_VARS)

include $(SDK_DIR)/Android.def

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libhi_pvrsmooth
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := $(CFG_HI_CFLAGS)
LOCAL_CFLAGS += -DLOG_TAG=\"$(LOCAL_MODULE)\"

LOCAL_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600
LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

#LOCAL_SRC_FILES := $(sort $(call all-c-files-under, ./))
LOCAL_SRC_FILES := hi_pvr_smooth_ctrl.c


LOCAL_C_INCLUDES := $(COMMON_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_API_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_API_INCLUDE)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpge/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpegfmw/include
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vfmw/vfmw_release
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vdec
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include

LOCAL_SHARED_LIBRARIES := libcutils libutils libhi_common libhi_demux libhi_avplay
LOCAL_SHARED_LIBRARIES += libhi_cipher

include $(BUILD_STATIC_LIBRARY)

#================ PVR smooth share lib ================
#include $(CLEAR_VARS)
#
#include $(SDK_DIR)/Android.def
#
#LOCAL_PRELINK_MODULE := false
#
#LOCAL_MODULE := libhi_pvrsmooth
#ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)
#
#LOCAL_MODULE_TAGS := optional
#
#LOCAL_CFLAGS := $(CFG_HI_CFLAGS)
#LOCAL_CFLAGS += -DLOG_TAG=\"$(LOCAL_MODULE)\"
#
#LOCAL_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600
#LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
#
##LOCAL_SRC_FILES := $(sort $(call all-c-files-under, ./))
#LOCAL_SRC_FILES := hi_pvr_smooth_ctrl.c
#
#
#LOCAL_C_INCLUDES := $(COMMON_UNF_INCLUDE)
#LOCAL_C_INCLUDES += $(COMMON_DRV_INCLUDE)
#LOCAL_C_INCLUDES += $(COMMON_API_INCLUDE)
#LOCAL_C_INCLUDES += $(MSP_UNF_INCLUDE)
#LOCAL_C_INCLUDES += $(MSP_DRV_INCLUDE)
#LOCAL_C_INCLUDES += $(MSP_API_INCLUDE)
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
#LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include
#LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpge/include
#LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpegfmw/include
#LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vfmw/vfmw_release
#LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vdec
#LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include
#
#LOCAL_SHARED_LIBRARIES := libcutils libutils libhi_common libhi_demux libhi_avplay
#LOCAL_SHARED_LIBRARIES += libhi_cipher
#
#include $(BUILD_SHARED_LIBRARY)

#================ PVR share lib ================
include $(CLEAR_VARS)

include $(SDK_DIR)/Android.def

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libhi_pvr
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := $(CFG_HI_CFLAGS)
LOCAL_CFLAGS += -DLOG_TAG=\"$(LOCAL_MODULE)\"

LOCAL_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600
LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

#LOCAL_SRC_FILES := $(sort $(call all-c-files-under, ./))
LOCAL_SRC_FILES := unf_pvr.c           \
                   hi_pvr_fifo.c       \
                   hi_pvr_index.c      \
                   hi_pvr_intf.c       \
                   hi_pvr_play_ctrl.c  \
                   hi_pvr_rec_ctrl.c   \
                   pvr_bitstream.c     \
                   pvr_index.c         \
                   pvr_scd.c

LOCAL_C_INCLUDES := $(COMMON_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_API_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_API_INCLUDE)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpge/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpegfmw/include
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vfmw/vfmw_release
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vdec
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include

LOCAL_SHARED_LIBRARIES := libcutils libutils libhi_common libhi_demux libhi_avplay 
LOCAL_SHARED_LIBRARIES += libhi_cipher
LOCAL_STATIC_LIBRARIES := libhi_pvrsmooth

include $(BUILD_SHARED_LIBRARY)

#================ PVR static lib ================
include $(CLEAR_VARS)

include $(SDK_DIR)/Android.def

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libhi_pvr
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := $(CFG_HI_CFLAGS)
LOCAL_CFLAGS += -DLOG_TAG=\"$(LOCAL_MODULE)\"

LOCAL_CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600
LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

#LOCAL_SRC_FILES := $(sort $(call all-c-files-under, ./))
LOCAL_SRC_FILES := unf_pvr.c           \
                   hi_pvr_fifo.c       \
                   hi_pvr_index.c      \
                   hi_pvr_intf.c       \
                   hi_pvr_play_ctrl.c  \
                   hi_pvr_rec_ctrl.c   \
                   pvr_bitstream.c     \
                   pvr_index.c         \
                   pvr_scd.c

LOCAL_C_INCLUDES := $(COMMON_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_API_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_API_INCLUDE)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpge/include
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpegfmw/include
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vfmw/vfmw_release
LOCAL_C_INCLUDES += $(MSP_DIR)/drv/vdec
LOCAL_C_INCLUDES += $(MSP_DIR)/api/jpeg/include

LOCAL_SHARED_LIBRARIES := libcutils libutils libhi_common libhi_demux libhi_avplay 
LOCAL_SHARED_LIBRARIES += libhi_cipher
LOCAL_STATIC_LIBRARIES := libhi_pvrsmooth

include $(BUILD_STATIC_LIBRARY)
