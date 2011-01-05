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
