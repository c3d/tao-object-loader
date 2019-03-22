// *****************************************************************************
// glc_attributes.cpp                                              Tao3D project
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
//! \file glc_attributes.cpp implementation of the GLC_Attributes class.

#include "glc_attributes.h"

// Default constructor
GLC_Attributes::GLC_Attributes()
: m_AttributesHash()
, m_AttributesList()
{

}

// Copy Constructor
GLC_Attributes::GLC_Attributes(const GLC_Attributes& attr)
: m_AttributesHash(attr.m_AttributesHash)
, m_AttributesList(attr.m_AttributesList)
{

}

// Overload "=" operator
GLC_Attributes& GLC_Attributes::operator=(const GLC_Attributes& attr)
{
	if (this != &attr)
	{
		m_AttributesHash= attr.m_AttributesHash;
		m_AttributesList= attr.m_AttributesList;
	}
	return *this;
}

// Destructor
GLC_Attributes::~GLC_Attributes()
{

}
