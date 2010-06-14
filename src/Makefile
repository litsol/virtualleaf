# $Id$

QMAKE = qmake

all: VirtualLeaf libplugin plugins

VirtualLeaf: Makefile.VirtualLeaf
	make -f Makefile.VirtualLeaf

Makefile.VirtualLeaf: VirtualLeaf.pro
	$(QMAKE) -makefile -o $@ $< 

libplugin: Makefile.libplugin
	make -f Makefile.libplugin

Makefile.libplugin: libplugin.pro
	$(QMAKE) -makefile -o $@ $< 

plugins:
	make -C build_models

clean:
	make -f Makefile.libplugin clean
	make -f Makefile.VirtualLeaf clean
	make -C build_models clean
	touch VirtualLeaf.pro
	touch libplugin.pro

#finis
