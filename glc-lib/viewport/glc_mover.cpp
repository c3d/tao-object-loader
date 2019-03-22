// *****************************************************************************
// glc_mover.cpp                                                   Tao3D project
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

//! \file glc_mover.cpp Implementation of the GLC_Mover class.

#include "glc_mover.h"
#include "glc_viewport.h"

GLC_Mover::GLC_Mover(GLC_Viewport* pViewport, const QList<GLC_RepMover*>& repsList)
: QObject()
, m_RepMoverList(repsList)
, m_PreviousVector()
, m_pViewport(pViewport)
, m_MoverInfo()
{
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList[i]->setRepMoverInfo(&m_MoverInfo);
	}
}

// Copy constructor
GLC_Mover::GLC_Mover(const GLC_Mover& mover)
: QObject()
, m_RepMoverList()
, m_PreviousVector(mover.m_PreviousVector)
, m_pViewport(mover.m_pViewport)
, m_MoverInfo(mover.m_MoverInfo)
{
	const int size= mover.m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList.append(mover.m_RepMoverList.at(i)->clone());
		m_RepMoverList.last()->setRepMoverInfo(&m_MoverInfo);
	}
}

GLC_Mover::~GLC_Mover()
{
	clearMoverRepresentation();
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Set the mover representation list
void GLC_Mover::setRepresentationsList(const QList<GLC_RepMover*>& listOfRep)
{
	qDebug() << "GLC_Mover::setRepresentationsList";
	clearMoverRepresentation();
	m_RepMoverList= listOfRep;
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList[i]->setRepMoverInfo(&m_MoverInfo);
	}
}

// Update representation
void GLC_Mover::initRepresentation()
{
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList[i]->init();
	}
}

// Update representation
void GLC_Mover::updateRepresentation()
{
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList[i]->update();
	}

}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

// Mover representations list display
void GLC_Mover::renderRepresentation()
{
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		m_RepMoverList[i]->render();
	}
}

//////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////

// Clear mover representation
void GLC_Mover::clearMoverRepresentation()
{
	// Delete mover representations
	const int size= m_RepMoverList.size();
	for (int i= 0; i < size; ++i)
	{
		delete m_RepMoverList.at(i);
	}
}


