JPEG_MODULE := jpeg6b

ifeq ($(CFG_HI_VDEC_REG_CODEC_SUPPORT),y)
JPEG_MODULE += jpegfmw
endif

include $(call all-named-subdir-makefiles,$(JPEG_MODULE))
