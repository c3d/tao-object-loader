#ifndef OBJECT3D_DRAWING_H
#define OBJECT3D_DRAWING_H
// *****************************************************************************
// object3d_drawing.h                                              Tao3D project
// *****************************************************************************
//
// File description:
//
//    Draw a 3D model (Object3D) at a specific location and with a specific
//    size.
//      x, y, z: where you want the center of the object to be
//      w, h, d: the desired object dimensions, 0 to set them automatically
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2012,2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011, Soulisse Baptiste <soulisse@polytech.unice.fr>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************


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
