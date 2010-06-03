#
#  $Id$
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

CONFIG += release
CONFIG -= debug
CONFIG += staticlib

QMAKE_CXXFLAGS += -fexceptions
QMAKE_CXXFLAGS_DEBUG += -g3
QMAKE_CXXFLAGS_DEBUG += -DQDEBUG
QMAKE_CXXFLAGS_DEBUG -= -finstrument-functions

DEFINES = QTGRAPHICS # VLEAFPLUGIN
DESTDIR = build_models/lib 
PERLDIR = ./perl
PARTMPL = VirtualLeafpar.tmpl
QT += qt3support
TARGET = vleaf
TEMPLATE = lib

HEADERS = \
 cellbase.h \
 matrix.h \
 output.h \
 parameter.h \
 parse.h \
 random.h \
 simplugin.h \
 UniqueMessage.h \
 vector.h \
 wallbase.h \
 warning.h

SOURCES = \
 cellbase.cpp \
 matrix.cpp \
 output.cpp \
 parameter.cpp \
 parse.cpp \
 random.cpp \
 simplugin.cpp \
 UniqueMessage.cpp \
 vector.cpp \
 wallbase.cpp \
 warning.cpp

unix {
 system(if `which perl > /dev/null 2>&1` ; then rm -f parameter.cpp parameter.h pardialog.cpp pardialog.h; fi)
 system(if `which perl > /dev/null 2>&1` ; then perl $$PERLDIR/make_parameter_source.pl $$PARTMPL; fi)
 system(if `which perl > /dev/null 2>&1` ; then perl $$PERLDIR/make_pardialog_source.pl $$PARTMPL; fi)
#system(if `which perl > /dev/null 2>&1` ; then perl $$PERLDIR/make_xmlwritecode.pl -h $$REACTIONS; fi)

 QMAKE_CXXFLAGS += -fPIC -I/usr/include/libxml2
 QMAKE_LFLAGS += -fPIC
 LIBS += -lxml2 -lz -lm 
}

win32 {
 system(WHERE /Q perl && IF errorlevel 0 (DEL parameter.cpp parameter.h pardialog.cpp pardialog.h)
 system(WHERE /Q perl && IF errorlevel 0 (perl $$PERLDIR\make_parameter_source.pl $$PARTMPL)
 system(WHERE /Q perl && IF errorlevel 0 (perl $$PERLDIR\make_pardialog_source.pl $$PARTMPL)
#system(WHERE /Q perl && IF errorlevel 0 (perl $$PERLDIR\make_xmlwritecode.pl -h $$REACTIONS)

 LIBXML2DIR = C:\libxml2
 LIBICONVDIR = C:\libiconv
 LIBZDIR = C:\libz
 QMAKE_CXXFLAGS += -DLIBXML_STATIC
 QMAKE_CXXFLAGS += -I$${LIBXML2DIR}\include -I$${LIBICONVDIR}\include -I$${LIBZDIR}\include
}

#
