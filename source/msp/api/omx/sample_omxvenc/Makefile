#===============================================================================
# export variable
#===============================================================================

include $(CURDIR)/../../../../../cfg.mak
include $(CURDIR)/../../../../../sample/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(CURDIR)/../inc/
CFLAGS  +=  -I$(CURDIR)/../../../../common/include/

LIBPATH = -L${CURDIR}/../omx_core
LIBPATH += -L$(CURDIR)/..//omx_venc
LIBPATH += -L${CURDIR}/../../../../../pub/lib/share


LIB = -Wl,--start-group -lpthread -ldl -lOMX_Core -lOMX.hisi.video.encoder -lhi_common -lhi_otp -Wl,--end-group

		  
APP = sample_omxvenc           
OBJS = omx_venc_test.o queue.o

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all:$(APP)

%.o: %.c
	$(AT)$(CC) $(CFLAGS) -c -o $@ $^

$(APP): ${OBJS}
	$(AT)$(CC) -o $@ $^ ${LIBPATH} ${LIB}


clean:
	-$(AT)rm -f ${OBJS}
	-$(AT)rm -f $(APP)

	
install:

uninstall:
