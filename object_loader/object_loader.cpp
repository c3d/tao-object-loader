// ****************************************************************************
//  object_loader.cpp                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//     Implementation of the XL primitives for the "object loader" module.
//
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
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "object_loader.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "object3d.h"
#include "object3d_drawing.h"
#include "tao/module_api.h"
#include <GLC_Exception>


using namespace XL;

XL_DEFINE_TRACES

Tree_p object(Tree_p self,
              Real_p x, Real_p y, Real_p z,
              Real_p w, Real_p h, Real_p d,
              Text_p name)
// ----------------------------------------------------------------------------
//   Primitive to load a 3D object and show it centered at (x, y, z)
// ----------------------------------------------------------------------------
{
    if (!Object3D::tao)
        return XL::xl_false;

    try
    {
        Object3D *obj = Object3D::Object(name);
        if (!obj)
            return XL::xl_false;

        Object3DDrawing *drawing = new Object3DDrawing(obj, x, y, z, w, h, d);
        Object3D::tao->addToLayout(Object3DDrawing::render_callback, drawing,
                                   Object3DDrawing::delete_callback);
    }
    catch (GLC_Exception e)
    {
        return XL::Ooops(e.what(), self);
    }

    return XL::xl_true;
}


Tree_p object(Tree_p self, Text_p name)
// ----------------------------------------------------------------------------
//   Load a 3D object and show it as-is (not coordinate manipulation)
// ----------------------------------------------------------------------------
{
    if (!Object3D::tao)
        return XL::xl_false;

    try
    {
        Object3D *obj = Object3D::Object(name);
        if (!obj)
            return XL::xl_false;

        Object3D::tao->scheduleRender(Object3D::render_callback, obj);
    }
    catch (GLC_Exception e)
    {
        return XL::Ooops(e.what(), self);
    }

    return XL::xl_true;
}


int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    Object3D::tao = api;
    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    return 0;
}
