###########################################################################
#                                                                         #
#  Makefile.qrgen                                                         #
#                                                                         #
#    Raptor OpenGL & Vulkan realtime 3D Engine SDK.                       #
#                                                                         #
#  Copyright 1998-2019 by                                                 #
#  Fabrice FERRAND.                                                       #
#                                                                         #
#  This file is part of the Raptor project, and may only be used,         #
#  modified, and distributed under the terms of the Raptor project        #
#  license, LICENSE.  By continuing to use, modify, or distribute         #
#  this file you indicate that you have read the license and              #
#  understand and accept it fully.                                        #
#                                                                         #
###########################################################################


export

# Mandatory compiler flags
CFLAGS += -std=c99
# Diagnostics. Adding '-fsanitize=address' is helpful for most versions of Clang and newer versions of GCC.
CFLAGS += -Wall -fsanitize=undefined
# Optimization level
CFLAGS += -O1

CCCFLAGS = -DLINUX -fPIC -fpermissive -fno-rtti
OUTPUT = .lib
LDFLAGS = -Wno-undef -Bdynamic -shared -lstdc++

SO = $(OUTPUT)/libscan.so.1
LIB = $(OUTPUT)/libscan.a

SRC = \
	scan.cpp \
	scan_pdf.cpp \
	scan_jpg.cpp \
	scan_png.cpp \
	Color.cpp \
	Image.cpp \
	JPGImaging.cpp \
	PNGImaging.cpp \
	TGAImaging.cpp

OBJ = \
	$(OUTPUT)/scan.o \
	$(OUTPUT)/scan_pdf.o \
	$(OUTPUT)/scan_jpg.o \
	$(OUTPUT)/scan_png.o \
	$(OUTPUT)/Color.o \
	$(OUTPUT)/Image.o \
	$(OUTPUT)/JPGImaging.o \
	$(OUTPUT)/PNGImaging.o \
	$(OUTPUT)/TGAImaging.o

all:	$(LIB) $(SO)

clean:
	rm -rf $(OUTPUT)
	mkdir -p $(OUTPUT)

$(SO):	$(OBJ)
	gcc -o $(SO) $(OBJ) $(LDFLAGS)
	cp $(SO) .

$(LIB):	$(OBJ) 
	ar -qcv $(LIB) $(OBJ)
	cp $(LIB) .

$(OBJ):	$(SRC)
	gcc $(CCCFLAGS) -c scan.cpp  -o $(OUTPUT)/scan.o
	gcc $(CCCFLAGS) -c scan_jpg.cpp  -o $(OUTPUT)/scan_jpg.o
	gcc $(CCCFLAGS) -c scan_pdf.cpp  -o $(OUTPUT)/scan_pdf.o
	gcc $(CCCFLAGS) -c scan_png.cpp  -o $(OUTPUT)/scan_png.o
	gcc $(CCCFLAGS) -c Color.cpp  -o $(OUTPUT)/Color.o
	gcc $(CCCFLAGS) -c Image.cpp  -o $(OUTPUT)/Image.o
	gcc $(CCCFLAGS) -c JPGImaging.cpp  -o $(OUTPUT)/JPGImaging.o
	gcc $(CCCFLAGS) -c PNGImaging.cpp  -o $(OUTPUT)/PNGImaging.o
	gcc $(CCCFLAGS) -c TGAImaging.cpp  -o $(OUTPUT)/TGAImaging.o


