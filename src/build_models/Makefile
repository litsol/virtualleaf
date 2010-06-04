# $Id: Makefile,v 3f0977faba37 2010/06/03 15:54:37 michael $

QMAKE = qmake

all: plugin_auxingrowth plugin_leaf plugin_meinhardt plugin_test

plugin_auxingrowth: Makefile.plugin_auxingrowth
	make -f Makefile.plugin_auxingrowth

Makefile.plugin_auxingrowth: plugin_auxingrowth.pro
	$(QMAKE) -makefile -o $@ $< 

plugin_leaf: Makefile.plugin_leaf
	make -f Makefile.plugin_leaf

Makefile.plugin_leaf: plugin_leaf.pro
	$(QMAKE) -makefile -o $@ $< 

plugin_meinhardt: Makefile.plugin_meinhardt
	make -f Makefile.plugin_meinhardt

Makefile.plugin_meinhardt: plugin_meinhardt.pro
	$(QMAKE) -makefile -o $@ $< 

plugin_test: Makefile.plugin_test
	make -f Makefile.plugin_test

Makefile.plugin_test: plugin_test.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	make -f Makefile.plugin_auxingrowth clean
	make -f Makefile.plugin_leaf clean
	make -f Makefile.plugin_meinhardt clean
	make -f Makefile.plugin_test clean

#finis
