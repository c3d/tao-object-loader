// ****************************************************************************
//  object3d_drawing.cpp                                           Tao project
// ****************************************************************************
//
//   File Description:
//
//    Object3DDrawing implementation
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao/graphic_state.h"
#include "object3d_drawing.h"

using namespace Tao;


DLL_PUBLIC Tao::GraphicState * graphic_state = NULL;
#define GL (*graphic_state)


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
