// *****************************************************************************
// glc_userinput.cpp                                               Tao3D project
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
// (C) 2011,2019, Christophe de Dinechin <christophe@dinechin.org>
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

//! \file glc_mover.cpp Implementation of the GLC_Mover class.

#include "glc_userinput.h"

GLC_UserInput::GLC_UserInput(int x, int y)
: m_X(x)
, m_Y(y)
, m_NormalyzeX(0.0)
, m_NormalyzeY(0.0)
, m_Translation()
, m_Rotation(0.0)
, m_ScaleFactor(1.0)
, m_TransformationIsSet(false)
{

}

GLC_UserInput::~GLC_UserInput()
{

}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
void GLC_UserInput::setPosition(int x, int y)
{
	m_X= x;
	m_Y= y;
}

void GLC_UserInput::setNormalyzeTouchCenterPosition(double x, double y)
{
	m_NormalyzeX= x;
	m_NormalyzeY= y;
}

void GLC_UserInput::setTransformation(const GLC_Vector2d& translation, double rotation, double scaleFactor)
{
	m_Translation= translation;
	m_Rotation= rotation;
	m_ScaleFactor= scaleFactor;

	m_TransformationIsSet= true;
}
