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
#include "tao/module_api.h"
#include <GLC_Exception>


using namespace XL;

XL_DEFINE_TRACES

static const Tao::ModuleApi *tao = NULL;

Tree_p object(Tree_p self,
              Real_p /*x*/, Real_p /*y*/, Real_p /*z*/,
              Real_p /*w*/, Real_p /*h*/, Real_p /*d*/,
              Text_p name)
// ----------------------------------------------------------------------------
//   Primitive to load a 3D object
// ----------------------------------------------------------------------------
{
    if (!tao)
        return XL::xl_false;

    // Try to load the 3D object in memory and graphic card
    try
    {
        Object3D *obj = Object3D::Object(name);
        if (!obj)
            return XL::xl_false;

        tao->scheduleRender(Object3D::render_callback, obj);
    }
    catch (GLC_Exception e)
    {
        return XL::Ooops(e.what(), self);
    }

    return XL::xl_true;
}


int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module: instantiate all CanIViz modules
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    tao = api;
    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module: free all CanIViz modules
// ----------------------------------------------------------------------------
{
    return 0;
}
