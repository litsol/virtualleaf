# $Id$

QMAKE = qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: tutorial1C

tutorial1C: Makefile.tutorial1C
	$(MAKE) -f Makefile.tutorial1C

Makefile.tutorial1C: tutorial1C.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	$(MAKE) -f Makefile.tutorial1C clean
ifeq ($(MAKE),make)
	touch tutorial1C.pro
else
	copy /b tutorial1C.pro +,,
endif

# finis
