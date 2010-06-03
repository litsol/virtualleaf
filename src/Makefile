# $Id$

all: VirtualLeaf libplugin plugins

VirtualLeaf: Makefile.VirtualLeaf
	make -f Makefile.VirtualLeaf

Makefile.VirtualLeaf: VirtualLeaf.pro
	qmake -makefile -o $@ $< 

libplugin: Makefile.libplugin
	make -f Makefile.libplugin

Makefile.libplugin: libplugin.pro
	qmake -makefile -o $@ $< 

plugins:
	make -C build_models

clean:
	make -f Makefile.libplugin clean
	make -f Makefile.VirtualLeaf clean
	make -C build_models clean

#finis
