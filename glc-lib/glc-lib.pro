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
