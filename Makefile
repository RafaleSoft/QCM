###########################################################################
#                                                                         #
#  Makefile.pyscan                                                         #
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

OUTPUT = QCM/venv/libs64

all:	qcm

qcm: pyscan
	(cd QCM; python3 setup.py build)
	(cd QCM; sudo python3 setup.py install)

pyscan:	qrgen quirc scan
	(cd QCM/pyscan ; python3 setup.py build)
	(cd QCM/pyscan ; sudo python3 setup.py install)

clean:
	rm -f $(OUTPUT)/*
	make -C qrgen clean
	make -C QUIRC/quirc clean
	make -C QUIRC clean
	make -C scan clean
	rm -rf QCM/pyscan/build

qrgen:	$(OUTPUT)/libqrgen.so

quirc:	$(OUTPUT)/libquirc.so

scan:	$(OUTPUT)/libscan.so


$(OUTPUT)/libqrgen.so:
	make -C qrgen
	cp qrgen/libqrgen.* $(OUTPUT)
	ln -s $(OUTPUT)/libqrgen.so.1 $(OUTPUT)/libqrgen.so
	
$(OUTPUT)/libquirc.so:
	make -C QUIRC/quirc libquirc.a
	make -C QUIRC/quirc libquirc.so
	make -C QUIRC
	cp QUIRC/libquirc.* $(OUTPUT)
	ln -s $(OUTPUT)/libquirc.so.1 $(OUTPUT)/libquirc.so

$(OUTPUT)/libscan.so:
	make -C scan
	cp scan/libscan.* $(OUTPUT)
	ln -s $(OUTPUT)/libscan.so.1 $(OUTPUT)/libscan.so
