// *****************************************************************************
// glc_world.cpp                                                   Tao3D project
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

//! \file glc_world.cpp implementation of the GLC_World class.

#include "glc_world.h"
#include "glc_structinstance.h"
#include "glc_structreference.h"

// Default constructor
GLC_World::GLC_World()
: m_pWorldHandle(new GLC_WorldHandle())
, m_pRoot(new GLC_StructOccurence())
{
	m_pRoot->setWorldHandle(m_pWorldHandle);
	//qDebug() << "GLC_World::GLC_World() : " << (*m_pNumberOfWorld) << " " << this;
}

// Copy constructor
GLC_World::GLC_World(const GLC_World& world)
: m_pWorldHandle(world.m_pWorldHandle)
, m_pRoot(world.m_pRoot)
{
	//qDebug() << "GLC_World::GLC_World() : " << (*m_pNumberOfWorld) << " " << this;
	// Increment the number of world
	m_pWorldHandle->increment();
}

GLC_World::~GLC_World()
{

	// Decrement the number of world
	m_pWorldHandle->decrement();
	//qDebug() << "GLC_World::GLC_World() : " << (*m_pNumberOfWorld) << " " << this;
	if (m_pWorldHandle->isOrphan())
	{
		// this is the last World, delete the root product and collection
		//m_pWorldHandle->collection()->clear(); // Clear collection first (performance)
		delete m_pRoot;
		delete m_pWorldHandle;
	}
}

// Merge this world with another world
void GLC_World::mergeWithAnotherWorld(GLC_World& anotherWorld)
{
	qDebug() << "GLC_World::mergeWithAnotherWorld";
	GLC_StructOccurence* pAnotherRoot= anotherWorld.rootOccurence();
	if (pAnotherRoot->childCount() > 0)
	{
		QList<GLC_StructOccurence*> childs= pAnotherRoot->children();
		const int size= childs.size();
		for (int i= 0; i < size; ++i)
		{
			m_pRoot->addChild(childs.at(i)->clone(m_pWorldHandle, false));
		}
		m_pRoot->updateChildrenAbsoluteMatrix();
	}
	else
	{
		m_pRoot->addChild(anotherWorld.rootOccurence()->clone(m_pWorldHandle, false));
	}
}

// Assignment operator
GLC_World& GLC_World::operator=(const GLC_World& world)
{
	if (this != &world)
	{
		// Decrement the number of world
		m_pWorldHandle->decrement();
		if (m_pWorldHandle->isOrphan())
		{
			// this is the last World, delete the root product and collection
			//m_pWorldHandle->collection()->clear(); // Clear collection first (performance)
			delete m_pRoot;
			delete m_pWorldHandle;
		}
		m_pRoot= world.m_pRoot;
		m_pWorldHandle= world.m_pWorldHandle;
		m_pWorldHandle->increment();
	}
	return *this;
}
