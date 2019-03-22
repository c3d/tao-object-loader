#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H
// *****************************************************************************
// object_loader.h                                                 Tao3D project
// *****************************************************************************
//
// File description:
//
//    Prototype of the function used by object_loader.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011,2013, Jérôme Forissier <jerome@taodyne.com>
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


#include "tree.h"

XL::Tree_p object(XL::Tree_p self,
                  XL::Real_p x, XL::Real_p y, XL::Real_p z,
                  XL::Real_p w, XL::Real_p h, XL::Real_p d,
                  XL::Text_p name, bool colored);

XL::Tree_p object(XL::Tree_p self, XL::Text_p name, bool colored);

#endif // OBJECT_LOADER_H
