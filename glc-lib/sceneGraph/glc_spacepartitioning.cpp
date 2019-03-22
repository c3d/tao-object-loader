// *****************************************************************************
// glc_spacepartitioning.cpp                                       Tao3D project
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
//! \file glc_spacepartitioning.cpp implementation for the GLC_SpacePartitioning class.

#include "glc_spacepartitioning.h"
#include "glc_3dviewcollection.h"

#include <QtGlobal>

// Default constructor
GLC_SpacePartitioning::GLC_SpacePartitioning(GLC_3DViewCollection* pCollection)
: m_pCollection(pCollection)
{
	Q_ASSERT(m_pCollection != NULL);
}

// Copy constructor
GLC_SpacePartitioning::GLC_SpacePartitioning(const GLC_SpacePartitioning& spacePartitionning)
: m_pCollection(spacePartitionning.m_pCollection)
{

}

GLC_SpacePartitioning::~GLC_SpacePartitioning()
{

}
