# $Id$

QMAKE = $(HOME)/Trolltech/Qt-4.4.1/bin/qmake

# Set $(MAKE) to the name of the make command on your machine, e.g. 'make' on Linux and 'mingw32-make' on windows.
MAKE =  make # mingw32-make

all: VirtualLeaf libplugin plugins

VirtualLeaf: Makefile.VirtualLeaf
	$(MAKE) -f Makefile.VirtualLeaf

Makefile.VirtualLeaf: VirtualLeaf.pro
	$(QMAKE) -makefile -o $@ $< 

libplugin: Makefile.libplugin
	$(MAKE) -f Makefile.libplugin

Makefile.libplugin: libplugin.pro
	$(QMAKE) -makefile -o $@ $< 

plugins:
	$(MAKE) -C build_models

clean:
	$(MAKE) -f Makefile.libplugin clean
	$(MAKE) -f Makefile.VirtualLeaf clean
	$(MAKE) -C build_models clean
ifeq ($(MAKE),make)
	touch VirtualLeaf.pro
	touch libplugin.pro
endif

#finis
