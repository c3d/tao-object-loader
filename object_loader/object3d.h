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
#include <vector>
#include <GLC_World>


struct Object3D
// ----------------------------------------------------------------------------
//   Representation of a complete 3D object
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Object3D(kstring name = NULL);
    ~Object3D();

    // Loading an objet file
    void Load(kstring name);

    // Draw interface (called from Object3DDrawing)
    void Draw();

    static Object3D *Object(text name);

    // Call ((Object3D *)arg)->Draw()
    static void render_callback(void *arg);

public:
    // Representation of an object
    GLC_World   glcWorld;
};

#endif // OBJECT3D_H
