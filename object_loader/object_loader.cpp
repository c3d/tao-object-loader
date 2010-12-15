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


using namespace XL;


XL_DEFINE_TRACES

Tree_p object(Tree_p self,
              Real_p x, Real_p y, Real_p z,
              Real_p w, Real_p h, Real_p d,
              Text_p name)
// ----------------------------------------------------------------------------
//   Load a 3D object
// ----------------------------------------------------------------------------
{
    (void*)self;
    (void*)x; (void*)y; (void*)z;
    (void*)w; (void*)h; (void*)d;
    (void*)name;

    IFTRACE(objloader)
        std::cerr << "Load " << name->value << "\n";

    return XL::xl_false;
}
