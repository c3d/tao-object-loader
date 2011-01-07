#ifndef OBJECT3D_H
#define OBJECT3D_H
// ****************************************************************************
//  object3d.h                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//    Load a 3D model file and render it using GLC_Lib.
//
//
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "tao/coords3d.h"
#include "tao/module_api.h"
#include <QObject>
#include <QTime>
#include <vector>
#include <iostream>
#include <GLC_World>


class LoadThread;

struct Object3D : public QObject
// ----------------------------------------------------------------------------
//   Representation of a complete 3D object
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Object3D(kstring name = NULL);
    ~Object3D();

    // Loading an objet file
    void              Load(kstring name);

    // Draw interface
    void              Draw();

    // Object bounding box
    Tao::Box3         Bounds();

    // Object3D cache
    static Object3D * Object(text name);

    // Call ((Object3D *)arg)->Draw()
    static void       render_callback(void *arg);

public slots:
    void              percentComplete(int p);
    void              loadFinished();
    void              loadFailed();

public:
    // Debug trace
    static std::ostream& debug();

private:
    enum Status {
        NotStarted = 0,
        InProgress,
        LoadFailed,
        LoadSuccess,
    };

private:
    void              DrawObject();
    void              DrawPlaceHolder();
    void              DrawErrorPlaceHolder();

private:
    // Representation of an object
    GLC_World     glcWorld;
    // Thread to load file asynchronously
    LoadThread *  loadThread;
    // File load status
    Status        status;
    // During load, percent complete
    int           complete;
    // Load start time
    QTime         loadTime;

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;

    Q_OBJECT
};

#endif // OBJECT3D_H
