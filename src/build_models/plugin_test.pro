#
# $Id$
#
#  This file is part of the Virtual Leaf.
#
#  The Virtual Leaf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  The Virtual Leaf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the Virtual Leaf.  If not, see <http://www.gnu.org/licenses/>.
#
#  Copyright 2010 Roeland Merks.
#


BINDIR = ../../bin
CONFIG += plugin debug #release
DEFINES = QTGRAPHICS # VLEAFPLUGIN
DESTDIR = $${BINDIR}/models
TARGET = test
HEADERS = ../simplugin.h $${TARGET}plugin.h  
QMAKE_CXXFLAGS += -I..
QT += qt3support
SOURCES = $${TARGET}plugin.cpp
TEMPLATE = lib 

unix {
 LIBS += -Llib -lvleaf
 QMAKE_CXXFLAGS += -fexceptions -I/usr/include/libxml2
 QMAKE_LFLAGS += -fPIC
}

win32 {
 LIBXML2DIR = C:\libxml2
 LIBICONVDIR = C:\libiconv
 LIBS += -Llib -lvleaf
 QMAKE_CXXFLAGS += -DLIBXML_STATIC
 QMAKE_CXXFLAGS += -fexceptions -I$${LIBXML2DIR}\include  -I$${LIBICONVDIR}\include

}

# finis
