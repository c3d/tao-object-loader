// *****************************************************************************
// glc_object.cpp                                                  Tao3D project
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
/*

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

//! \file GLC_Object.cpp Implementation of the GLC_Object class.

#include "glc_object.h"

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

GLC_Object::GLC_Object(const QString& name)
: m_Uid(glc::GLC_GenID())	// Object ID
, m_Name(name)			// Object Name
{

}

GLC_Object::GLC_Object(const GLC_Object& sourceObject)
: m_Uid(sourceObject.m_Uid)
, m_Name(sourceObject.m_Name)
{
}

GLC_Object::~GLC_Object()
{

}


//////////////////////////////////////////////////////////////////////
// Set function
//////////////////////////////////////////////////////////////////////

void GLC_Object::setId(const GLC_uint id)
{
	QMutexLocker mutexLocker(&m_Mutex);
	m_Uid= id;
}

void GLC_Object::setName(const QString& name)
{
	QMutexLocker mutexLocker(&m_Mutex);
	m_Name= name;
}


GLC_Object& GLC_Object::operator=(const GLC_Object& object)
{
	QMutexLocker mutexLocker(&m_Mutex);
	m_Uid= object.m_Uid;
	m_Name= object.m_Name;
	return *this;
}
