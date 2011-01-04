#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H
// ****************************************************************************
//  object_loader.h                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//    Prototype of the function used by object_loader.tbl to
//    implement the new XL primitive
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


#include "tree.h"

XL::Tree_p object(XL::Tree_p self,
                  XL::Real_p x, XL::Real_p y, XL::Real_p z,
                  XL::Real_p w, XL::Real_p h, XL::Real_p d,
                  XL::Text_p name);

XL::Tree_p object(XL::Tree_p self, XL::Text_p name);

#endif // OBJECT_LOADER_H
