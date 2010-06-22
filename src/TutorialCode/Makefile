# $Id$

# Set an environment varible $(MAKE) to the name of the make command
# on your machine, e.g. 'make' on UNIX and 'mingw32-make' on windows.
# Fallback default is UNIX make.

ifeq ($(MAKE),)
	MAKE=make
endif

all: tutorial_0 tutorial_1A tutorial_1B tutorial_1C tutorial_1D tutorial_2 tutorial_3 tutorial_4 tutorial_5

tutorial_0:
	$(MAKE) -C Tutorial0

tutorial_1A:
	$(MAKE) -C Tutorial1A

tutorial_1B:
	$(MAKE) -C Tutorial1B

tutorial_1C:
	$(MAKE) -C Tutorial1C

tutorial_1D:
	$(MAKE) -C Tutorial1D

tutorial_2:
	$(MAKE) -C Tutorial2

tutorial_3:
	$(MAKE) -C Tutorial3

tutorial_4:
	$(MAKE) -C Tutorial4

tutorial_5:
	$(MAKE) -C Tutorial5

clean:
	$(MAKE) -C Tutorial0  clean
	$(MAKE) -C Tutorial1A clean
	$(MAKE) -C Tutorial1B clean
	$(MAKE) -C Tutorial1C clean
	$(MAKE) -C Tutorial1D clean
	$(MAKE) -C Tutorial2  clean
	$(MAKE) -C Tutorial3  clean
	$(MAKE) -C Tutorial4  clean
	$(MAKE) -C Tutorial5  clean

# finis
