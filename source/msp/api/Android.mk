LOCAL_PATH := $(call my-dir)

API_MODULE := hdmi gpio vo pdm cipher

ifeq ($(CFG_HI_LOADER_APPLOADER),y)
API_MODULE += i2c frontend ir demux tde jpeg otp higo
endif

ifneq ($(CFG_HI_LOADER_APPLOADER),y)
 ifneq ($(CFG_HI_LOADER_RECOVERY),y)
 API_MODULE += i2c frontend ir demux tde jpeg otp higo \
           pm avplay pvr sync ao adec vdec wdg png omx mce 3d jpge
 endif
endif

ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
API_MODULE += keyled
endif

ifeq ($(CFG_HI_AENC_SUPPORT),y)
API_MODULE += aenc
endif

ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
API_MODULE += ci
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP),y)
API_MODULE += hdcp
endif

ifeq ($(CFG_HI_SCI_SUPPORT),y)
API_MODULE += sci
endif

ifeq ($(CFG_HI_VI_SUPPORT),y)
API_MODULE += vi 
endif

ifeq ($(CFG_HI_VENC_SUPPORT),y)
API_MODULE += venc
endif


include $(call all-named-subdir-makefiles,$(API_MODULE))
