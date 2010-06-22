# $Id$

QMAKE = $(HOME)/Trolltech/Qt-4.4.1/bin/qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: plugin_auxingrowth plugin_meinhardt plugin_test

plugin_auxingrowth: Makefile.plugin_auxingrowth
	$(MAKE) -f Makefile.plugin_auxingrowth

Makefile.plugin_auxingrowth: plugin_auxingrowth.pro
	$(QMAKE) -makefile -o $@ $< 

plugin_meinhardt: Makefile.plugin_meinhardt
	$(MAKE) -f Makefile.plugin_meinhardt

Makefile.plugin_meinhardt: plugin_meinhardt.pro
	$(QMAKE) -makefile -o $@ $< 

plugin_test: Makefile.plugin_test
	$(MAKE) -f Makefile.plugin_test

Makefile.plugin_test: plugin_test.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	$(MAKE) -f Makefile.plugin_auxingrowth clean
	$(MAKE) -f Makefile.plugin_meinhardt clean
	$(MAKE) -f Makefile.plugin_test clean
ifeq ($(MAKE),make)
	touch plugin_auxingrowth.pro
	touch plugin_meinhardt.pro
	touch plugin_test.pro
else
	copy /b plugin_auxingrowth.pro +,,
	copy /b plugin_meinhardt.pro +,,
	copy /b plugin_test.pro +,,

endif

#finis
