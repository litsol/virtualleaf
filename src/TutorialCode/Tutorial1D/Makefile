# $Id$

QMAKE = qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: tutorial1D

tutorial1D: Makefile.tutorial1D
	$(MAKE) -f Makefile.tutorial1D

Makefile.tutorial1D: tutorial1D.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	$(MAKE) -f Makefile.tutorial1D clean
ifeq ($(MAKE),make)
	touch tutorial1D.pro
else
	copy /b tutorial1D.pro +,,
endif

# finis
