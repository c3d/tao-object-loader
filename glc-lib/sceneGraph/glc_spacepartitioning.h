// *****************************************************************************
// glc_spacepartitioning.h                                         Tao3D project
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
//! \file glc_spacepartitioning.h interface for the GLC_SpacePartitioning class.

#ifndef GLC_SPACEPARTITIONING_H_
#define GLC_SPACEPARTITIONING_H_

#include "../glc_config.h"
#include "../glc_boundingbox.h"
#include "../viewport/glc_frustum.h"

class GLC_3DViewCollection;
class GLC_3DViewInstance;

//////////////////////////////////////////////////////////////////////
//! \class GLC_SpacePartitioning
/*! \brief GLC_SpacePartitioning : Abstract class for space partitionning */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_SpacePartitioning
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	GLC_SpacePartitioning(GLC_3DViewCollection*);

	//! Copy constructor
	GLC_SpacePartitioning(const GLC_SpacePartitioning&);

	//! Destructor
	virtual ~GLC_SpacePartitioning();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the 3DViewCollection of the space partitioning
	inline GLC_3DViewCollection* collectionHandle()
	{return m_pCollection;}

	//! Return the list off instances inside or intersect the given bounding box
	virtual QList<GLC_3DViewInstance*> listOfIntersectedInstances(const GLC_BoundingBox&)= 0;


//@}
//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Update visible GLC_3DViewInstance
	virtual void updateViewableInstances(const GLC_Frustum&)= 0;

	//! Update the space partionning
	virtual void updateSpacePartitioning()= 0;

	//! Clear the space partionning
	virtual void clear()= 0;

//@}

//////////////////////////////////////////////////////////////////////
// Protected members
//////////////////////////////////////////////////////////////////////
protected:
	//! The Collection containing 3dview Instances
	GLC_3DViewCollection* m_pCollection;

};

#endif /* GLC_SPACEPARTITIONING_H_ */
