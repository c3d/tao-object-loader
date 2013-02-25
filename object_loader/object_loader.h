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
                  XL::Real_p x, XL::Real_p y, XL::Real_p z,
                  XL::Real_p w, XL::Real_p h, XL::Real_p d,
                  XL::Text_p name, bool colored);

XL::Tree_p object(XL::Tree_p self, XL::Text_p name, bool colored);

#endif // OBJECT_LOADER_H
