// *****************************************************************************
// glc_repcrossmover.h                                             Tao3D project
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

#ifndef GLC_REPCROSSMOVER_H_
#define GLC_REPCROSSMOVER_H_

#include "glc_repmover.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_RepCrossMover
/*! \brief GLC_RepCrossMover : Cross representation*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_RepCrossMover : public GLC_RepMover
{
public:
	//! Default constructor
	GLC_RepCrossMover(GLC_Viewport*);

	//! Copy constructor
	GLC_RepCrossMover(const GLC_RepCrossMover&);

	//! Destructor
	virtual ~GLC_RepCrossMover();

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
	//! Return a clone of the repmover
	virtual GLC_RepMover* clone() const;
//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Virtual interface for OpenGL Geometry set up.
	virtual void glDraw();

//@}

};

#endif /* GLC_REPCROSSMOVER_H_ */
