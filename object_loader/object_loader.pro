# ******************************************************************************
#  object_loader.pro                                                Tao project
# ******************************************************************************
# File Description:
# Qt build file for the 3D object loader module
# ******************************************************************************
# This software is property of Taodyne SAS - Confidential
# Ce logiciel est la propriété de Taodyne SAS - Confidentiel
# (C) 2010 Jerome Forissier <jerome@taodyne.com>
# (C) 2010 Taodyne SAS
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
win32 {
  DEFINES     += GLEW_STATIC
  SOURCES     += $${TAOTOPSRC}/tao/include/tao/GL/glew.c
}
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

LICENSE_FILES = object_loader.taokey.notsigned
exists(../../licenses.pri):include(../../licenses.pri)
