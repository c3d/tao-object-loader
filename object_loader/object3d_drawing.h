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
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
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
                     Tao::coord x, Tao::coord y, Tao::coord z,
                     Tao::scale w, Tao::scale h, Tao::scale d,
                     bool colored = false)
        : object(obj), x(x), y(y), z(z), w(w), h(h), d(d), colored(colored) {}
    ~Object3DDrawing() {}

    void                Draw();
    void                Identify();
    void                Transform();

    // Call ((Object3DDrawing *)arg)->Draw()
    static void         render_callback(void *arg);
    // Call ((Object3DDrawing *)arg)->Draw()
    static void         identify_callback(void *arg);
    // Call delete (Object3DDrawing *)arg
    static void         delete_callback(void *arg);

public:
    Object3D *          object;
    Tao::coord          x, y, z;
    Tao::scale          w, h, d;
    bool                colored;
};

#endif // OBJECT3D_DRAWING_H
