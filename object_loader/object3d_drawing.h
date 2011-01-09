#ifndef OBJECT3D_DRAWING_H
#define OBJECT3D_DRAWING_H
// ****************************************************************************
//  object3d_drawing.h                                             Tao project
// ****************************************************************************
//
//   File Description:
//
//    Draw a 3D model (Object3D) at a specific location and with a specific
//    size.
//      x, y, z: where you want the center of the object to be
//      w, h, d: the desired object dimensions, 0 to set them automatically
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
#include "tree.h"
#include "object3d.h"


struct Object3DDrawing
// ----------------------------------------------------------------------------
//   We draw a 3D object by reference
// ----------------------------------------------------------------------------
{
    Object3DDrawing (Object3D *obj,
                     XL::Real_p x, XL::Real_p y, XL::Real_p z,
                     XL::Real_p w, XL::Real_p h, XL::Real_p d)
        : object(obj), x(x), y(y), z(z), w(w), h(h), d(d) {}
    ~Object3DDrawing() {}

    void                Draw();

    // Call ((Object3DDrawing *)arg)->Draw()
    static void         render_callback(void *arg);
    // Call delete (Object3DDrawing *)arg
    static void         delete_callback(void *arg);

public:
    Object3D *          object;
    XL::Real_p          x, y, z;
    XL::Real_p          w, h, d;
};

#endif // OBJECT3D_DRAWING_H
