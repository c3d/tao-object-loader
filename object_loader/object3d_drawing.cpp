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
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "object3d_drawing.h"

using namespace Tao;



void Object3DDrawing::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Object3DDrawing *)arg)->Draw();
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
    Box3 bounds = object->Bounds();

    glPushMatrix();
    if (bounds.IsEmpty())
    {
        // When object load is in progress, bound are not yet knwon. We just
        // need to translate to display the % complete at the right place
        glTranslatef(x, y, z);
    }
    else
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
        coord ox = x - bounds.Center().x;
        coord oy = y - bounds.Center().y;
        coord oz = z - bounds.Center().z;

        // Translate and scale object
        glScalef(s, s, s);
        glTranslatef(ox, oy, oz);
    }

    object->Draw();
    glPopMatrix();
}
