// *****************************************************************************
// glc_turntablemover.cpp                                          Tao3D project
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

#include "glc_turntablemover.h"
#include "glc_viewport.h"

// Default constructor
GLC_TurnTableMover::GLC_TurnTableMover(GLC_Viewport* pViewport, const QList<GLC_RepMover*>& repsList)
: GLC_Mover(pViewport, repsList)
, m_Sign(1.0)
{

}


// Copy constructor
GLC_TurnTableMover::GLC_TurnTableMover(const GLC_TurnTableMover& mover)
: GLC_Mover(mover)
, m_Sign(mover.m_Sign)
{
}


GLC_TurnTableMover::~GLC_TurnTableMover()
{
}


//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return a clone of the mover
GLC_Mover* GLC_TurnTableMover::clone() const
{
	return new GLC_TurnTableMover(*this);
}


//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Initialized the mover
void GLC_TurnTableMover::init(const GLC_UserInput& userInput)
{
	GLC_Mover::m_PreviousVector.setVect(static_cast<double>(userInput.x()), static_cast<double>(userInput.y()),0.0);
	GLC_Camera* pCamera= GLC_Mover::m_pViewport->cameraHandle();
	// Calculate angle sign
	m_Sign= pCamera->defaultUpVector() * pCamera->upVector();
	if (m_Sign == 0)
	{
		m_Sign= 1;
	}
	else
	{
		m_Sign= m_Sign / fabs(m_Sign);
	}

	pCamera->setUpCam(pCamera->defaultUpVector() * m_Sign);
}


bool GLC_TurnTableMover::move(const GLC_UserInput& userInput)
{
	GLC_Camera* pCamera= GLC_Mover::m_pViewport->cameraHandle();
	// Turn table rotation
	const double rotSpeed= 2.3;
	const double width= static_cast<double> ( GLC_Mover::m_pViewport->viewVSize() );
	const double height= static_cast<double> ( GLC_Mover::m_pViewport->viewHSize() );

	const double alpha = -((static_cast<double>(userInput.x()) - GLC_Mover::m_PreviousVector.x()) / width) * rotSpeed;
	const double beta = ((static_cast<double>(userInput.y()) - GLC_Mover::m_PreviousVector.y()) / height) * rotSpeed;

	// Rotation around the screen vertical axis
	pCamera->rotateAroundTarget(pCamera->defaultUpVector(), alpha * m_Sign);

	// Rotation around the screen horizontal axis
	GLC_Vector3d incidentVector= -pCamera->forward();
	GLC_Vector3d rightVector= incidentVector ^ pCamera->upVector();
	if (!rightVector.isNull())
	{
		pCamera->rotateAroundTarget(rightVector, beta);
	}

	m_PreviousVector.setVect(static_cast<double>(userInput.x()), static_cast<double>(userInput.y()), 0.0);

	return true;
}
