###########################################################################
#                                                                         #
#  Makefile.quirc_api                                                     #
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

CCCFLAGS = -DLINUX -fPIC -fpermissive
OUTPUT = .lib
LDFLAGS = -Bdynamic -shared -Lquirc -lquirc -lstdc++

SO = $(OUTPUT)/libqrgen.so.1
LIB = $(OUTPUT)/libqrgen.a

SRC = \
	quirc_api.cpp

OBJ = $(OUTPUT)/quirc_api.o

all:	$(LIB) $(SO)

clean:
	rm -rf $(OUTPUT)
	mkdir -p $(OUTPUT)

$(SO):	$(OBJ) quirc
	gcc -o $(SO) $(OBJ) $(LDFLAGS)
	cp $(SO) .

$(LIB):	$(OBJ) quirc
	ar -qcv $(LIB) $(OBJ)
	cp $(LIB) .
quirc:	quirc/libquirc.a
	cd quirc && make

$(OBJ):	$(SRC)
	gcc $(CCCFLAGS) -c quirc_api.cpp  -o $(OUTPUT)/quirc_api.o


