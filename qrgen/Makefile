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

CCCFLAGS = -DLINUX -fPIC
OUTPUT = .lib
LDFLAGS = -Bdynamic -shared -lstdc++

SO = $(OUTPUT)/libqrgen.so.1
LIB = $(OUTPUT)/libqrgen.a

SRC = \
	QR-Code-generator/c/qrcodegen.c \
	qrgen.cpp

OBJ = $(OUTPUT)/qrgen.o $(OUTPUT)/qrcodegen.o

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
	gcc $(CCCFLAGS) -c qrgen.cpp  -o $(OUTPUT)/qrgen.o
	gcc $(CCCFLAGS) -c QR-Code-generator/c/qrcodegen.c -o $(OUTPUT)/qrcodegen.o


