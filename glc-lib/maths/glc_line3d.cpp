// *****************************************************************************
// glc_line3d.cpp                                                  Tao3D project
// *****************************************************************************
//
// File description:
//
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011, Soulisse Baptiste <soulisse@polytech.unice.fr>
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
//! \file glc_line3d.cpp Implementation of the GLC_Line3d class.

#include "glc_line3d.h"

GLC_Line3d::GLC_Line3d()
: m_Point()
, m_Vector()
{

}

GLC_Line3d::GLC_Line3d(const GLC_Point3d& point, const GLC_Vector3d& vector)
: m_Point(point)
, m_Vector(vector)
{

}


GLC_Line3d::GLC_Line3d(const GLC_Line3d& line)
: m_Point(line.m_Point)
, m_Vector(line.m_Vector)
{

}


GLC_Line3d::~GLC_Line3d()
{

}
