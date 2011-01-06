CONFIG(debug, debug|release):FORCE_NO_RELEASE=1

include(GLC_lib.pro)

include(../object_loader.pri)
include(../../modules_defs.pri)

target.path = $${MODINSTPATH}/lib
INSTALLS = target

win32 {
    QMAKE_MKDIR = mkdir -p
    # Quick fix for in redefinition warnings in glext.h
    DEFINES += GL_NV_geometry_program4
}

# GLC_lib.pro forces a release build (CONFIG += release).
# We don't want that, because when Tao is built in debug mode, both the debug
# AND the release versions of the Qt DLLs would be loaded. This is bad (see
# bug#651).
!isEmpty(FORCE_NO_RELEASE):CONFIG -= release
