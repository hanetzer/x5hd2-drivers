
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../../../../..

include $(SDK_DIR)/base.mak
endif

TARGET := jpeg6b
#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  $(CFG_HI_CFLAGS)
CFLAGS  +=  -DCFG_HI_JPEG6B_STREAMBUFFER_SIZE=$(CFG_HI_JPEG6B_STREAMBUFFER_SIZE)

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)                 \
            -I$(COMMON_API_INCLUDE)                 \
            -I$(COMMON_DRV_INCLUDE)                 \
            -I$(MSP_UNF_INCLUDE)                    \
            -I$(MSP_API_INCLUDE)                    \
            -I$(MSP_DRV_INCLUDE)                    \
            -I$(MSP_DIR)/api/jpeg/$(TARGET)/include      \
            -I$(MSP_DIR)/drv/jpeg/include 	    \
            -I$(MSP_DIR)/api/tde/include 	    \
	    -I$(CURDIR)

srcdir = .

prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin

libdir = $(exec_prefix)/lib

includedir = $(prefix)/include



binprefix =
manprefix =
manext = 1
mandir = $(prefix)/man/man$(manext)

.o:.c
	$(CC) -c $< -fPIC -o $@ $(CFLAGS)


ANSI2KNRFLAGS= 

LDFLAGS= 

LIBTOOL = 

O = o
A = a

JPEG_LIB_VERSION = 62


SYSDEPMEM= jmemnobs.$(O)

SHELL= /bin/sh

LN= $(CC)

RM= rm -f

MKDIR= mkdir

AR= ar rc

AR2= ranlib
INSTALL= /usr/bin/install -c
INSTALL_PROGRAM= ${INSTALL}
INSTALL_LIB= ${INSTALL} -m 644
INSTALL_DATA= ${INSTALL} -m 644


LIBSOURCES=jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
        jcinit.c jcmainct.c jcmarker.c jcmaster.c jcparam.c \
        jcphuff.c jcprepct.c jcsample.c jctrans.c \
        hijpeg_decode_hw.c hi_jpeg_api.c \
        jcomapi.c jdapimin.c jdapistd.c \
        jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
        jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
        jdpostct.c  jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
        jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c
 

SYSDEPSOURCES=jmemnobs.c


SOURCES= $(LIBSOURCES) $(SYSDEPSOURCES) $(APPSOURCES)


INCLUDES=jdhuff.h jdct.h jerror.h jinclude.h jmemsys.h jmorecfg.h \
        jpegint.h jpeglib.h jversion.h \
        hi_jpeg_api.h hi_tde_api.h hi_tde_errcode.h hi_tde_type.h  hi_type.h hijpeg_api.h hijpeg_decode_hw.h \
        hijpeg_type.h

COMOBJECTS= jcomapi.$(O) jutils.$(O) jerror.$(O) jmemmgr.$(O) $(SYSDEPMEM)

CLIBOBJECTS= jcapimin.$(O) jcapistd.$(O) jctrans.$(O) jcparam.$(O) \
        jdatadst.$(O) jcinit.$(O) jcmaster.$(O) jcmarker.$(O) jcmainct.$(O) \
        jcprepct.$(O) jccoefct.$(O) jccolor.$(O) jcsample.$(O) jchuff.$(O) \
        jcphuff.$(O) jcdctmgr.$(O) jfdctfst.$(O) jfdctflt.$(O) \
        jfdctint.$(O)
               
DLIBOBJECTS= jdapimin.$(O) jdapistd.$(O) jdtrans.$(O) jdatasrc.$(O) \
        jdmaster.$(O) jdinput.$(O) jdmarker.$(O) jdhuff.$(O) jdphuff.$(O) \
        jdmainct.$(O) jdcoefct.$(O) jdpostct.$(O) jddctmgr.$(O) \
        jidctfst.$(O) jidctflt.$(O) jidctint.$(O) jidctred.$(O) \
        jdsample.$(O) jdcolor.$(O) jquant1.$(O) jquant2.$(O) jdmerge.$(O) \
        hijpeg_decode_hw.$(O) hi_jpeg_api.$(O) 
        
LIBOBJECTS=$(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)

all:  libjpeg6b.$(A) libjpeg6b.so 

.SUFFIXES: .lo .la

ansi2knr: ansi2knr.c
	$(CC) $(CFLAGS) $(ANSI2KNRFLAGS) -o ansi2knr $(srcdir)/ansi2knr.c
	$(MKDIR) knr

libjpeg6b.a:  $(LIBOBJECTS)
	$(RM)  libjpeg6b.a
	$(AR)  libjpeg6b.a  $(LIBOBJECTS)
	$(AR2) libjpeg6b.a
	ln -s  libjpeg6b.a       libjpeg.a
libjpeg6b.so: $(LIBOBJECTS)
	$(CC) -o libjpeg6b.so -shared $(LIBOBJECTS)
	ln -s  libjpeg6b.so      libjpeg.so
#	$(CC) -o libjpeg.so.6 -shared $(LIBOBJECTS)
#	$(CC) -o libjpeg.so.6.0.1 -shared $(LIBOBJECTS)

libjpeg6b.la:  $(LIBOBJECTS)
	$(LIBTOOL) --mode=link $(CC) -o libjpeg6b.la $(LIBOBJECTS) \
		-rpath $(libdir) -version-info $(JPEG_LIB_VERSION)


install-lib: libjpeg6b.$(A) install-headers
	$(INSTALL_LIB) libjpeg6b.$(A) $(libdir)/$(binprefix)libjpeg6b.$(A)

install-headers: jconfig.h
	$(INSTALL_DATA) jconfig.h $(includedir)/jconfig.h
	$(INSTALL_DATA) $(srcdir)/jpeglib.h $(includedir)/jpeglib.h
	$(INSTALL_DATA) $(srcdir)/jmorecfg.h $(includedir)/jmorecfg.h
	$(INSTALL_DATA) $(srcdir)/jerror.h $(includedir)/jerror.h

clean:
	$(RM) *.o *.lo libjpeg6b.a libjpeg6b.la libjpeg6b.so
	$(RM) cjpeg djpeg jpegtran rdjpgcom wrjpgcom
	$(RM) ansi2knr core testout* config.log config.status
	$(RM) -r knr .libs _libs

distclean: clean
	$(RM) Makefile jconfig.h libtool config.cache

.PHONY: all install install-lib install-headers clean distclean 



