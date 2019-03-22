# ******************************************************************************
# glc-lib.pro                                                      Tao3D project
# ******************************************************************************
#
# File description:
#
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
# (C) 2011,2013-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2010-2013, Jérôme Forissier <jerome@taodyne.com>
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
!CONFIG(debug,   debug|release):NO_DEBUG=1
!CONFIG(release, debug|release):NO_RELEASE=1

include(GLC_lib.pro)

include(../object_loader.pri)
include(../../modules_defs.pri)

target.path = $${MODINSTPATH}/lib
INSTALLS = target

win32 {
    QMAKE_MKDIR = mkdir -p
    # Quick fix for in redefinition warnings in glext.h
    DEFINES += GL_EXT_geometry_shader4

    # Remove the .a file after installation
    postinstall.path     = $${MODINSTPATH}/lib
    postinstall.depends  = install_target
    postinstall.commands = rm $${MODINSTPATH}/lib/lib$${TARGET}?.a
    INSTALLS += postinstall
}

# GLC_lib.pro may force a build mode (for instance: CONFIG += release).
# We don't want that, because if Tao is in a different mode than GLC_Lib,
# both the debug and release Qt DLLs would be loaded. This is bad (see
# bug#651).
!isEmpty(NO_RELEASE):CONFIG -= release
!isEmpty(NO_DEBUG):CONFIG -= debug

# Silence qWarning/qDebug in release mode
CONFIG(release, debug|release):DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

macx {
    # Make sure libGLC references the Qt libraries bundled with the application
    # and not the ones that may be installed on the target system, otherwise
    # they may clash
    FIX_QT_REFS = $${TAOTOPSRC}/modules/fix_qt_refs
    QMAKE_POST_LINK = $$FIX_QT_REFS "$(TARGET)" \"$$[QT_INSTALL_LIBS]\"
}
