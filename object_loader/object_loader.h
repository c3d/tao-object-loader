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
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "tree.h"

XL::Tree_p object(XL::Tree_p self,
                  XL::Real &x, XL::Real &y, XL::Real &z,
                  XL::Real &w, XL::Real &h, XL::Real &d,
                  XL::Text &name);

XL::Tree_p object(XL::Tree_p self, XL::Text &name);

#endif // OBJECT_LOADER_H
