# $Id$

QMAKE = qmake

all: plugin_auxingrowth plugin_meinhardt plugin_test

plugin_auxingrowth: Makefile.plugin_auxingrowth
	make -f Makefile.plugin_auxingrowth

Makefile.plugin_auxingrowth: plugin_auxingrowth.pro
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
	make -f Makefile.plugin_meinhardt clean
	make -f Makefile.plugin_test clean
	touch plugin_auxingrowth.pro
	touch plugin_meinhardt.pro
	touch plugin_test.pro

#finis
