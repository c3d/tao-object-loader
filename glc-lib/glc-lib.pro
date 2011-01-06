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
    DEFINES += GL_NV_geometry_program4
}

# GLC_lib.pro may force a build mode (for instance: CONFIG += release).
# We don't want that, because if Tao is in a different mode than GLC_Lib,
# both the debug and release Qt DLLs would be loaded. This is bad (see
# bug#651).
!isEmpty(NO_RELEASE):CONFIG -= release
!isEmpty(NO_DEBUG):CONFIG -= debug
