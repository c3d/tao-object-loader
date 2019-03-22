# ******************************************************************************
# object_loader.pro                                                Tao3D project
# ******************************************************************************
#
# File description:
# Qt build file for the 3D object loader module
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
# (C) 2011-2013,2015,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2010-2013, Jérôme Forissier <jerome@taodyne.com>
# (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
# ******************************************************************************

include(../object_loader.pri)

TAO_SDK = ../../..

include($${TAO_SDK}/modules/modules.pri)

HEADERS     = object_loader.h \
              object3d.h \
              object3d_drawing.h \
              load_thread.h \
              preferences_dialog.h
SOURCES     = object_loader.cpp \
              object3d.cpp \
              object3d_drawing.cpp \
              load_thread.cpp \
              preferences_dialog.cpp
TBL_SOURCES = object_loader.tbl
OTHER_FILES = object_loader.xl object_loader.tbl traces.tbl

INCLUDEPATH += ../glc-lib/include \
               ../glc-lib \
               $${TAOTOPSRC}/tao/include/tao
DEPENDPATH  += ../glc-lib/include \
               ../glc-lib \
               $${TAOTOPSRC}/tao/include/tao
QT          += core \
               gui \
               opengl
win32:LIBS += -L../glc-lib/release -L../glc-lib/debug -lGLC_lib2
unix:LIBS += -L../glc-lib -lGLC_lib

QMAKE_CXXFLAGS -= -Werror  # REVISIT: Avoid errors from GL redefinitions

progress_img.path  = $$MODINSTPATH
progress_img.files = progress.png

INSTALLS    += thismod_icon progress_img

TRANSLATIONS = object_loader_fr.ts
include($${TAO_SDK}/translations.pri)
translations.path = $$MODINSTPATH
translations.files = *.qm
INSTALLS += translations
