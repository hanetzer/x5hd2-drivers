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

CFLAGS  += -D_GNU_SOURCE -D_XOPEN_SOURCE=600 
CFLAGS  += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)         \
            -I$(COMMON_API_INCLUDE)         \
            -I$(COMMON_DRV_INCLUDE)         \
            -I$(MSP_UNF_INCLUDE)            \
            -I$(MSP_API_INCLUDE)            \
            -I$(MSP_DRV_INCLUDE)            \
            -I$(MSP_DIR)/api/pvr/include

LIBS := libhi_pvr

OBJS := unf_pvr.o           \
        hi_pvr_fifo.o       \
        hi_pvr_index.o      \
        hi_pvr_intf.o       \
        hi_pvr_play_ctrl.o  \
        hi_pvr_rec_ctrl.o   \
        pvr_bitstream.o     \
        pvr_index.o         \
        pvr_scd.o

HEADER_FILES := $(MSP_DIR)/include/hi_unf_pvr.h 

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all: $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS).a
	$(AT)rm -rf $(LIBS).so

install: all
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

