#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../../..

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  $(CFG_HI_CFLAGS)

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)                 \
            -I$(COMMON_API_INCLUDE)                 \
            -I$(COMMON_DRV_INCLUDE)                 \
            -I$(MSP_UNF_INCLUDE)                    \
            -I$(MSP_API_INCLUDE)                    \
            -I$(MSP_DRV_INCLUDE)                    \
            -I$(CURDIR)                             \
            -I$(CURDIR)/../adec                     

ifeq ($(CFG_HI_CHIP_TYPE),hi3716cv200es)
CFLAGS  +=  -I$(CURDIR)/../hiao/aiaov2
else
CFLAGS  +=  -I$(CURDIR)/../hiao/hiao
endif            

CFLAGS  += -DCFG_HI_ADEC_MAX_INSTANCE=$(CFG_HI_ADEC_MAX_INSTANCE)
CFLAGS  += -DCFG_HI_ADEC_OUTPUTBUF_FRAMENUM=$(CFG_HI_ADEC_OUTPUTBUF_FRAMENUM)

LIBS := libhi_aenc

OBJS := unf_aenc.o  mpi_aenc.o  mpi_aenc_core.o mpi_aenc_lsrc.o
HEADER_FILES := $(MSP_DIR)/include/hi_unf_aenc.h
#===============================================================================
# rules
#===============================================================================
.PHONY : all clean install uninstall

all: $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS).a
	$(AT)rm -rf $(LIBS).so

install:all
	$(AT)cp -f  $(LIBS).a           $(STATIC_LIB_DIR)/
	$(AT)cp -f  $(LIBS).so          $(SHARED_LIB_DIR)/
	$(AT)cp -f  $(HEADER_FILES)     $(INCLUDE_DIR)/

uninstall:
	$(AT)rm -rf $(STATIC_LIB_DIR)/$(LIBS).a
	$(AT)rm -rf $(SHARED_LIB_DIR)/$(LIBS).so
	$(foreach file, $(HEADER_FILES), $(AT)rm -rf $(INCLUDE_DIR)/$(notdir $(file)))

$(LIBS) : $(OBJS)
	$(AT)$(AR) -rsv $@.a $^
	$(AT)$(CC) -shared -o $@.so $^
