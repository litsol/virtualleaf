.. $Id$

.. |date| date::
.. |time| date:: %H:%M

.. header::
  ###Title###  -  |date|

.. footer::
  ###Page### 


Installation Instructions
=========================

Requirements
------------

The Virtual Leaf is written with the QT Cross platform application and
UI framework, and can run on Linux, Apple Macintosh and Windows
machines. To compile the Virtual Leaf you will need to install the QT
software development kit (SDK) which you can download from the `QT download
site <http://qt.nokia.com/downloads/downloads#lgpl/>`_. Whether
installed globally by a system administrator our locally in your own
user space, make sure the QT bin directory containing qmake is in your
execution path.


All Platforms
-------------

Compilation is effected with make, either the native ``make`` on UNIX
and MAC systems or the ``mingw32-make`` distributed with the windows
version of QT. In the Virtual Leaf ``src`` directory you will find a
``Makefile``, the root of a hierarchy of makefiles, that will guide
the compilation and installation of the VirtualLeaf executable, its
default plugins and the tutorial examples.  To facilitate cross
platform compatibility, the makefiles expect an environment variable
named ``MAKE`` to name the make utility to use. The fallback default
is 'make.' Platform specific instructions follow.


Linux
-----

Prepend the QT bin directory to your path, for example::

 > PATH=/opt/QT/qt/bin:$PATH 

If you wish to use some other make utility than make, instantiate an
environment variable named ``MAKE``, for example::

 > export MAKE=gmake.

Go to the ``src`` directory and invoke make, for example::

 > cd /home/michael/VirtualLeaf/v1.0/src
 > make

When complete, you will find the ``VirtualLeaf`` binary in
``v1.0/bin`` and the plugins in ``v1.0/bin/models``.


Windows
-------

For convenience sake the libiconv, libxml2 and libz header files and
libraries are distributed with the Virtual Leaf code, and Virtual Leaf
will compile correctly with them. If, however, you wish to compile
with other versions of these libraries, you will need to reassign the
``LIBZML2DIR``, ``LIBICONVDIR`` and ``LIBZDIR`` variables in all the
project files.

After installing QT you should be able to invoke a QT command window
from the start menu. This shell automatically includes the necessary
QT folder in your execution PATH. Within this command window, go to the
Virtual Leaf's src directory, 

In the start menu, right click on My Computer and choose properties
from the drop down list. In the advanced tab click on environment
variables. Append the QT


MacOS
-----



