// *****************************************************************************
// glc_structinstance.cpp                                          Tao3D project
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

//! \file glc_structinstance.cpp implementation of the GLC_StructInstance class.

#include "glc_structinstance.h"
#include "glc_structreference.h"
#include "glc_structoccurence.h"

// Default constructor
GLC_StructInstance::GLC_StructInstance(GLC_StructReference* pStructReference)
: m_pNumberOfInstance(NULL)
, m_pStructReference(pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name()
, m_pAttributes(NULL)
{
	if (NULL == m_pStructReference)
	{
		m_pStructReference= new GLC_StructReference();
	}
	m_Name= m_pStructReference->name();

	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
	//qDebug() << "GLC_StructInstance::GLC_StructInstance : " << (*m_pNumberOfInstance) << " " << m_pNumberOfInstance;
}

// Create instance with a rep
GLC_StructInstance::GLC_StructInstance(GLC_Rep* pRep)
: m_pNumberOfInstance(NULL)
, m_pStructReference(new GLC_StructReference(pRep))
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(m_pStructReference->name())
, m_pAttributes(NULL)
{
	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Copy constructor
GLC_StructInstance::GLC_StructInstance(const GLC_StructInstance& structInstance)
: m_pNumberOfInstance(structInstance.m_pNumberOfInstance)
, m_pStructReference(structInstance.m_pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix(structInstance.m_RelativeMatrix)
, m_Name(structInstance.name())
, m_pAttributes(NULL)
{
	//qDebug() << "Instance Copy constructor";
	Q_ASSERT(NULL != m_pStructReference);
	// Copy attributes if necessary
	if (NULL != structInstance.m_pAttributes)
	{
		m_pAttributes= new GLC_Attributes(*(structInstance.m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Copy constructor
GLC_StructInstance::GLC_StructInstance(GLC_StructInstance* pStructInstance)
: m_pNumberOfInstance(pStructInstance->m_pNumberOfInstance)
, m_pStructReference(pStructInstance->m_pStructReference)
, m_ListOfOccurences()
, m_RelativeMatrix(pStructInstance->m_RelativeMatrix)
, m_Name(pStructInstance->name())
, m_pAttributes(NULL)
{
	//qDebug() << "Instance Copy constructor";
	Q_ASSERT(NULL != m_pStructReference);
	// Copy attributes if necessary
	if (NULL != pStructInstance->m_pAttributes)
	{
		m_pAttributes= new GLC_Attributes(*(pStructInstance->m_pAttributes));
	}

	++(*m_pNumberOfInstance);

	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);
}

// Create empty instance
GLC_StructInstance::GLC_StructInstance(const QString& name)
: m_pNumberOfInstance(NULL)
, m_pStructReference(NULL)
, m_ListOfOccurences()
, m_RelativeMatrix()
, m_Name(name)
, m_pAttributes(NULL)
{
}

// Set the reference of an empty instance
void GLC_StructInstance::setReference(GLC_StructReference* pStructReference)
{
	Q_ASSERT(NULL == m_pStructReference);
	Q_ASSERT(NULL == m_pNumberOfInstance);
	m_pStructReference= pStructReference;
	if (m_pStructReference->hasStructInstance())
	{
		m_pNumberOfInstance= m_pStructReference->firstInstanceHandle()->m_pNumberOfInstance;
		++(*m_pNumberOfInstance);
	}
	else
	{
		m_pNumberOfInstance= new int(1);
	}
	// Inform reference that an instance has been created
	m_pStructReference->structInstanceCreated(this);

	if (m_Name.isEmpty())
	{
		m_Name= pStructReference->name();
	}
}

// Destructor
GLC_StructInstance::~GLC_StructInstance()
{
	if(m_pNumberOfInstance != NULL)
	{
		// Update number of instance
		if ((--(*m_pNumberOfInstance)) == 0)
		{
			delete m_pStructReference;
			delete m_pNumberOfInstance;
		}
		else
		{
			// Inform reference that an instance has been deleted
			m_pStructReference->structInstanceDeleted(this);
			Q_ASSERT(m_pStructReference->hasStructInstance());
		}
		delete m_pAttributes;
	}
	else qDebug() << "GLC_StructInstance::~GLC_StructInstance() of empty instance";

}
