# $Id$

QMAKE = $(HOME)/Trolltech/Qt-4.4.1/bin/qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: VirtualLeaf libplugin plugins tutorials

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

tutorials:
	$(MAKE) -C build_models
	$(MAKE) -C TutorialCode

clean:
	$(MAKE) -f Makefile.libplugin clean
	$(MAKE) -f Makefile.VirtualLeaf clean
	$(MAKE) -C build_models clean
	$(MAKE) -C TutorialCode clean
ifeq ($(MAKE),make)
	touch VirtualLeaf.pro
	touch libplugin.pro
else
	copy /b VirtualLeaf.pro +,,
	copy /b libplugin.pro +,,
endif

#finis
