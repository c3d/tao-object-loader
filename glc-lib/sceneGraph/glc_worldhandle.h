// *****************************************************************************
// glc_worldhandle.h                                               Tao3D project
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

#ifndef GLC_WORLDHANDLE_H_
#define GLC_WORLDHANDLE_H_

#include "glc_3dviewcollection.h"
#include "glc_structoccurence.h"
#include "glc_selectionset.h"

#include <QHash>

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_WorldHandle
/*! \brief GLC_WorldHandle : Handle of shared GLC_World*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_WorldHandle
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! The default constructor
	GLC_WorldHandle();

	//! The default destructor
	~GLC_WorldHandle();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the collection
	inline GLC_3DViewCollection* collection()
	{return &m_Collection;}

	//! Return the number of world associated with this handle
	inline int numberOfWorld() const
	{return m_NumberOfWorld;}

	//! Return true if there is only one world associated with this handle
	inline bool isOrphan() const
	{return m_NumberOfWorld == 0;}

	//! Return true if the specified occurence id is in this world
	inline bool containsOccurence(GLC_uint id) const
	{return m_OccurenceHash.contains(id);}

	//! Return the occurence specified by an id
	/*! Id must be a valid identifier*/
	inline GLC_StructOccurence* getOccurence(GLC_uint id) const
	{
		Q_ASSERT(m_OccurenceHash.contains(id));
		return m_OccurenceHash.value(id);
	}

	//! Return the list off occurences
	inline QList<GLC_StructOccurence*> occurences() const
	{return m_OccurenceHash.values();}

	//! Return the number of occurence
	inline int numberOfOccurence() const
	{return m_OccurenceHash.size();}

	//! Return the list of instance
	QList<GLC_StructInstance*> instances() const;

	//! Return the list of Reference
	QList<GLC_StructReference*> references() const;

	//! Return the number of body
	int numberOfBody() const;

	//! Return the number of representation
	int representationCount() const;

	//! Return the world Up vector
	inline GLC_Vector3d upVector() const
	{return m_UpVector;}

	//! Return an handle to the selection set
	inline GLC_SelectionSet* selectionSetHandle()
	{return &m_SelectionSet;}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Increment the number of world
	inline void increment()
	{++m_NumberOfWorld;}

	//! Decrement the number of world
	inline void decrement()
	{--m_NumberOfWorld;}

	//! An Occurence has been added
	void addOccurence(GLC_StructOccurence* pOccurence, bool isSelected= false, GLuint shaderId= 0);

	//! An Occurence has been removed
	void removeOccurence(GLC_StructOccurence* pOccurence);

	//! All Occurence has been removed
	inline void removeAllOccurences()
	{
		m_OccurenceHash.clear();
	}

	//! Set the world Up Vector
	inline void setUpVector(const GLC_Vector3d& vect)
	{m_UpVector= vect;}

	//! Set the attached viewport of this world handle
	inline void setAttachedViewport(GLC_Viewport* pViewport)
	{m_Collection.setAttachedViewport(pViewport);}

	//! Select the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void select(GLC_uint occurenceId, bool propagate= true);

	//! Unselect the given occurence id
	/*! The given occurence id must belong to this worldhandle*/
	void unselect(GLC_uint occurenceId, bool propagate= true);

	//! Select all occurence of this world handle
	void selectAllWith3DViewInstance(bool allShowState);

	//! Unselecte all occurence of this world handle
	void unselectAll();

	//! Show / Hide selected 3DViewInstance
	void showHideSelected3DViewInstance();

	//! Set selected 3DViewInstance visibility
	void setSelected3DViewInstanceVisibility(bool isVisible);


//@}

//////////////////////////////////////////////////////////////////////
// private members
//////////////////////////////////////////////////////////////////////
private:
	//! The Collection
	GLC_3DViewCollection m_Collection;

	//! Number of this world
	int m_NumberOfWorld;

	//! The hash table containing struct occurence
	QHash<GLC_uint, GLC_StructOccurence*> m_OccurenceHash;

	//! This world Up Vector
	GLC_Vector3d m_UpVector;

	//! This world selectionSet
	GLC_SelectionSet m_SelectionSet;
};

#endif /* GLC_WORLDHANDLE_H_ */
