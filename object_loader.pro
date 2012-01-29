TEMPLATE = subdirs
SUBDIRS  = glc-lib object_loader
object_loader.depends = glc-lib

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(object_loader.pri)
include(../modules_doc.pri)
