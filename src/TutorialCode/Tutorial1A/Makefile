# $Id$

QMAKE = qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: tutorial1A

tutorial1A: Makefile.tutorial1A
	$(MAKE) -f Makefile.tutorial1A

Makefile.tutorial1A: tutorial1A.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	$(MAKE) -f Makefile.tutorial1A clean
ifeq ($(MAKE),make)
	touch tutorial1A.pro
else
	copy /b tutorial1A.pro +,,
endif

# finis
