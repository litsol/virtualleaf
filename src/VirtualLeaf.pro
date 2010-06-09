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
CONFIG += qt

QMAKE_CXXFLAGS += -fexceptions
QMAKE_CXXFLAGS_DEBUG += -g3
QMAKE_CXXFLAGS_DEBUG += -DQDEBUG

#REACTIONS = reactions_auxin_growth.h 
#REACTIONS = reactions_meinhardt.h
#REACTIONS = reactions_pce_growth.h
DEFINES += QTGRAPHICS
DEFINES += REACTIONS_HEADER=$${REACTIONS}
DEFINES += REACTIONS_HEADER_STRING=\"$${REACTIONS}\"
DEFINES += FLEMING

PERLDIR = ./perl
BINDIR = ../bin
DESTDIR = $$BINDIR
TARGET = VirtualLeaf
TEMPLATE = app
PARTMPL = $${TARGET}par.tmpl
MAINSRC = $${TARGET}.cpp
QT -= network sql xml
QT += qt3support

!win32 {
 GRAPHICS = qt #qwt
}

win32 {
 CONFIG += console
 LIBXML2DIR = C:\libxml2
 LIBICONVDIR = C:\libiconv
 LIBZDIR = C:\libz
 system(DEL parameter.cpp parameter.h) 
 GRAPHICS = qt 
 RC_FILE = VirtualLeaf.rc
 QMAKE_CXXFLAGS += -DLIBXML_STATIC
 QMAKE_CXXFLAGS += -I$${LIBXML2DIR}\include -I$${LIBICONVDIR}\include -I$${LIBZDIR}\include
 QMAKE_POST_LINK = "\
  C:\Bin\cp release\VirtualLeaf.exe \
  C:\Qt\4.5.3\bin\Qt3Support4.dll \
  C:\Qt\4.5.3\bin\QtGui4.dll \
  C:\Qt\4.5.3\bin\QtSql4.dll \
  C:\Qt\4.5.3\bin\QtXml4.dll \
  C:\Qt\4.5.3\bin\QtCore4.dll \
  C:\Qt\4.5.3\bin\QtNetwork4.dll \
  C:\Qt\4.5.3\bin\QtSvg4.dll \
  C:\bin\iconv.dll \
  C:\bin\libxml2.dll \
  C:\bin\zlib1.dll \
  C:\MinGW\bin\mingwm10.dll \
  $${DESTDIR}"
 LIBS += -L$${LIBXML2DIR}\lib -lxml2 -L$${LIBICONVDIR}\lib -L$${LIBZDIR}\lib  -lz -lm -lwsock32 -liconv
}

# Application icons
macx {
 ICON = leaficon.icns
 # make sure that the executable can find the libqwt share library
 QMAKE_POST_LINK = "mkdir $${DESTDIR}/$${TARGET}.app/Contents/Frameworks; \
  cp /usr/local/qwt/lib/libqwt.dylib $${DESTDIR}/$${TARGET}.app/Contents/Frameworks/.; \
  #install_name_tool -change libqwt.5.dylib $$QWTDIR/lib/libqwt.dylib $${DESTDIR}/$${TARGET}.app/Contents/MacOS/$${TARGET}; \
  install_name_tool -id @executable_path/../Frameworks/libqwt.dylib $${DESTDIR}/$${TARGET}.app/Contents/Frameworks/libqwt.dylib; \
  install_name_tool -change libqwt.5.dylib @executable_path/../Frameworks/libqwt.dylib $${DESTDIR}/$${TARGET}.app/Contents/MacOS/$${TARGET};\
  cp leaficon.icns $${DESTDIR}/$${TARGET}.app; "
}

macx:release {
 LIBS+= -dead_strip
}

unix {
 system(rm -f parameter.cpp parameter.h) 
 CC = /usr/bin/gcc 
 QWTDIR = /ufs/guravage/opt/qwt-5.2.1-svn
 QMAKE_LIBDIR += $$QWTDIR/lib 
 QMAKE_CXXFLAGS += -fPIC -I/usr/include/libxml2
 QMAKE_LFLAGS += -fPIC
 LIBS += -lxml2 -lz -lm 
}

system(perl $$PERLDIR/make_parameter_source.pl $$PARTMPL)
system(perl $$PERLDIR/make_pardialog_source.pl $$PARTMPL)
#system(perl $$PERLDIR/make_xmlwritecode.pl -h $$REACTIONS)

# Input
HEADERS += \
 apoplastitem.h \
 canvas.h \
 cellbase.h \
 cell.h \
 cellitem.h \
# forwardeuler.h \
 infobar.h \
 mainbase.h \
 mainbase.h \
 matrix.h \
 mesh.h \
 miscq.h \
 modelcatalogue.h \
 Neighbor.h \
 node.h \
 nodeitem.h \
 nodeset.h \
 OptionFileDialog.h \
 output.h \
 parameter.h \
 pardialog.h \
 parse.h \
 pi.h \
 qcanvasarrow.h \
 random.h \
 rungekutta.h \
 simitembase.h \
 simplugin.h \
 sqr.h \
 tiny.h \
 transporterdialog.h \
 UniqueMessage.h \
 vector.h \
 wallbase.h \
 wall.h \
 wallitem.h \
 warning.h \
 xmlwrite.h \
 flux_function.h \
 vleafmodel.h \
 $${PARTMPL}

SOURCES += \
 apoplastitem.cpp \
 canvas.cpp \
 cellbase.cpp \
 cell.cpp \
 cellitem.cpp \
# forwardeuler.cpp \
 mainbase.cpp \
 matrix.cpp \
 mesh.cpp \
 miscq.cpp \
 modelcatalogue.cpp \
 Neighbor.cpp \
 node.cpp \
 nodeitem.cpp \
 nodeset.cpp \
 OptionFileDialog.cpp \
 output.cpp \
 parameter.cpp \
 pardialog.cpp \
 parse.cpp \
 random.cpp \
 rungekutta.cpp \
 simitembase.cpp \
 transporterdialog.cpp \
 UniqueMessage.cpp \
 vector.cpp \
 wallbase.cpp \
 wall.cpp \
 wallitem.cpp \
 warning.cpp \
 xmlwritecode.cpp \
 xmlwrite.cpp \
 $$MAINSRC

contains( TARGET, leaf_fleming ) {
 DEFINES += FLEMING	
}

contains(GRAPHICS, qwt) {
 #macx:LIBS += -L$$QWTDIR/lib -lqwt
 #win32:LIBS += -L$$QWTDIR/lib -lqwt5
 #LIBS += -L$$QWTDIR/lib -lqwt
 INCLUDEPATH += $$QWTDIR/include
 DEFINES += HAVE_QWT
 HEADERS += data_plot.h
 SOURCES += data_plot.cpp
}

contains( GRAPHICS, qt ) {
 message( "Building Qt executable" )
 QMAKE_CXXFLAGS += -DQTGRAPHICS # -fpermissive
}

contains( GRAPHICS, xfig ) {
 message("Building Xfig executable (background runnable).")
 QMAKE_CXXFLAGS += -DXFIGGRAPHICS
}

contains( GRAPHICS, x11 ) {
 !unix {
  error("X11 graphics only available on Unix systems.")
 }
 message("Building X11 executable")
 SOURCES += x11graph.cpp
 HEADERS += x11graph.h
 QMAKE_CXXFLAGS += -DX11GRAPHICS
 CONFIG -= qt
 CONFIG += x11
 unix:LIBS += -lpng
}

# finis
