// *****************************************************************************
// object3d_drawing.cpp                                            Tao3D project
// *****************************************************************************
//
// File description:
//
//    Object3DDrawing implementation
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011,2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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

#include "tao/graphic_state.h"
#include "object3d_drawing.h"

using namespace Tao;


DLL_PUBLIC Tao::GraphicState * graphic_state = NULL;


void Object3DDrawing::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Object3DDrawing *)arg)->Draw();
}

void Object3DDrawing::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: call the identify function for the object
// ----------------------------------------------------------------------------
{
     ((Object3DDrawing *)arg)->Identify();
}

void Object3DDrawing::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Object3DDrawing *)arg;
}


void Object3DDrawing::Draw()
// ----------------------------------------------------------------------------
//   Draw object, centered at (x, y, z) and scaled to fit (w, h, d) size
// ----------------------------------------------------------------------------
{
    // Get current active texture
    GLuint unit = GL.ActiveTextureUnitIndex();

    // Active texture unit 0 to apply correctly
    // object's texture
    GL.ActiveTexture(GL_TEXTURE0);
    GL.Sync();

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

    Transform();

    object->colored = colored;
    object->Draw();

    glPopAttrib();
    glPopMatrix();

    // Restore texture unit
    GL.ActiveTexture(unit);
}


void Object3DDrawing::Identify()
// ----------------------------------------------------------------------------
//   Identify object under cursor
// ----------------------------------------------------------------------------
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

    Transform();

    object->Identify();

    glPopAttrib();
    glPopMatrix();
}

void Object3DDrawing::Transform()
// ----------------------------------------------------------------------------
//   Transform correctly the object
// ----------------------------------------------------------------------------
{
    Box3 bounds = object->Bounds();
    if (bounds.IsEmpty())
    {
        // When object load is in progress, bound are not yet knwon. We just
        // need to translate to display the % complete at the right place
        glTranslatef(x, y, z);
    }
    else if (w > 0 || h > 0 || d > 0)
    {
        // Compute scaling factor if bbox dimensions are specified
        scale sx = 1.0, sy = 1.0, sz = 1.0, s;
        if (w > 0)
            sx = w / bounds.Width();
        if (h > 0)
            sy = h / bounds.Height();
        if (d > 0)
            sz = d / bounds.Depth();
        s = sx;
        if (sy < s)
            s = sy;
        if (sz < s)
            s = sz;

        // Compute translation to center object
        coord ox = bounds.Center().x;
        coord oy = bounds.Center().y;
        coord oz = bounds.Center().z;

        GLdouble matrix[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
        glLoadIdentity();
        glTranslated(ox, oy, oz);
        glMultMatrixd(matrix);
        glTranslated(x, y, z);
        glScalef(s, s, s);
    }
}
