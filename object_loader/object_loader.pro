# ******************************************************************************
#  object_loader.pro                                                Tao project
# ******************************************************************************
# File Description:
# Qt build file for the 3D object loader module
# ******************************************************************************
# This document is released under the GNU General Public License.
# See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
# (C) 2010 Jerome Forissier <jerome@taodyne.com>
# (C) 2010 Taodyne SAS
# ******************************************************************************

include(../object_loader.pri)

TAO_SDK = ../../..

include($${TAO_SDK}/modules/modules.pri)

HEADERS     = object_loader.h \
              object3d.h \
              load_thread.h \
              raster_text.h \
              file_to_world.h
SOURCES     = object_loader.cpp \
              object3d.cpp \
              load_thread.cpp \
              raster_text.cpp \
              file_to_world.cpp
TBL_SOURCES = object_loader.tbl
OTHER_FILES = module.xl object_loader.tbl traces.tbl
OTHER_FILES = module.xl object_loader.tbl

INCLUDEPATH += ../glc-lib/include \
               ../glc-lib
DEPENDPATH  += ../glc-lib/include \
               ../glc-lib
QT          += core \
               gui \
               opengl
win32 {
    LIBS += -L../glc-lib/release -L../glc-lib/debug -lGLC_lib2
}
unix {
    LIBS += -L../glc-lib -lGLC_lib
}
QMAKE_CXXFLAGS -= -Werror  # REVISIT: Avoid errors from GL redefinitions

INSTALLS    += thismod_icon
