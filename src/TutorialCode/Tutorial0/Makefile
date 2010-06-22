# $Id$

QMAKE = qmake

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: tutorial0

tutorial0: Makefile.tutorial0
	$(MAKE) -f Makefile.tutorial0

Makefile.tutorial0: tutorial0.pro
	$(QMAKE) -makefile -o $@ $< 

clean:
	$(MAKE) -f Makefile.tutorial0 clean
ifeq ($(MAKE),make)
	touch tutorial0.pro
else
	copy /b tutorial0.pro +,,
endif

# finis
