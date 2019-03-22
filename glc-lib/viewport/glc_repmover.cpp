// *****************************************************************************
// glc_repmover.cpp                                                Tao3D project
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

//! \file glc_repmover.cpp Implementation of the GLC_RepMover class.

#include "glc_repmover.h"
#include "glc_viewport.h"


GLC_RepMover::GLC_RepMover(GLC_Viewport* pViewport)
: m_pViewport(pViewport)
, m_MainColor(Qt::black)
, m_Thickness(1.0)
, m_RenderProperties()
, m_pRepMoverInfo(NULL)
{

}
// Copy constructor
GLC_RepMover::GLC_RepMover(const GLC_RepMover& repMover)
: m_pViewport(repMover.m_pViewport)
, m_MainColor(repMover.m_MainColor)
, m_Thickness(repMover.m_Thickness)
, m_RenderProperties(repMover.m_RenderProperties)
, m_pRepMoverInfo(repMover.m_pRepMoverInfo)
{

}


GLC_RepMover::~GLC_RepMover()
{

}

void GLC_RepMover::setRepMoverInfo(RepMoverInfo* pRepMoverInfo)
{
	m_pRepMoverInfo= pRepMoverInfo;
}

void GLC_RepMover::setMainColor(const QColor& color)
{
	m_MainColor= color;
}

void GLC_RepMover::setThickness(double thickness)
{
	m_Thickness= thickness;
}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Representation OpenGL Execution
void GLC_RepMover::render()
{
	// Call virtual draw function
	glDraw();
}
