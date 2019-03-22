// *****************************************************************************
// glc_panmover.cpp                                                Tao3D project
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
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
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

//! \file glc_panmover.cpp Implementation of the GLC_PanMover class.

#include "glc_panmover.h"
#include "glc_viewport.h"
#include "QMouseEvent"

// Default constructor
GLC_PanMover::GLC_PanMover(GLC_Viewport* pViewport, const QList<GLC_RepMover*>& repsList)
: GLC_Mover(pViewport, repsList)
{

}

// Copy constructor
GLC_PanMover::GLC_PanMover(const GLC_PanMover& panMover)
: GLC_Mover(panMover)
{

}


GLC_PanMover::~GLC_PanMover()
{

}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return a clone of the mover
GLC_Mover* GLC_PanMover::clone() const
{
	return new GLC_PanMover(*this);
}


//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Initialized the mover
void GLC_PanMover::init(const GLC_UserInput& userInput)
{
	m_PreviousVector= m_pViewport->mapPosMouse(static_cast<double>(userInput.x()), static_cast<double>(userInput.y()));
}

// Move the camera
bool GLC_PanMover::move(const GLC_UserInput& userInput)
{
	const GLC_Vector3d VectCur(m_pViewport->mapPosMouse(static_cast<double>(userInput.x()), static_cast<double>(userInput.y())));
	const GLC_Vector3d VectPan= VectCur - m_PreviousVector;	// moving Vector

	// Pan the camera
	m_pViewport->cameraHandle()->pan(-VectPan);
	m_PreviousVector= VectCur;
	return true;
}

