#ifndef FILE_TO_WORLD_H
#define FILE_TO_WORLD_H
// ****************************************************************************
//  file_to_world.h                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//    A class that unifies the various file loaders of GLC_Lib. Useful to load
//    several models simultaneously and monitor % complete information.
//    The current version (2.0.1) of GLC_Factory has a currentQuantum(int)
//    signal, but since it is attached to the factory it is not suitable for
//    use when multiple loads are active. On the other hand, individual file
//    loaders (GLC_ObjToWorld, etc.) are private and have different interfaces.
//    This implementation presents file loaders with a unified interface.
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include <GLC_World>
#include <QObject>


QT_BEGIN_NAMESPACE
class QGlContext;
QT_END_NAMESPACE


class FileToWorld : public QObject
// ----------------------------------------------------------------------------
//   Load a 3D model file and create a GLC_World object
// ----------------------------------------------------------------------------
{
public:
    FileToWorld(const QGLContext * context = NULL) : m_pQGLContext(context) {}
    virtual ~FileToWorld() {}

public:
    GLC_World           createWorldFromFile(QFile & file,
                                            QStringList * attached = NULL);

signals:
    void                currentQuantum(int);

protected:
    const QGLContext *  m_pQGLContext;

    Q_OBJECT
};

#endif // FILE_TO_WORLD_H
