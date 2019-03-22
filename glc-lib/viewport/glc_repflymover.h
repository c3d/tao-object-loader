// *****************************************************************************
// glc_repflymover.h                                               Tao3D project
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
//! \file glc_repflymover.h Interface for the GLC_RepFlyMover class.

#ifndef GLC_REPFLYMOVER_H_
#define GLC_REPFLYMOVER_H_

#include "glc_repmover.h"
#include "../geometry/glc_circle.h"
#include "../sceneGraph/glc_3dviewinstance.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_RepFlyMover
/*! \brief GLC_RepFlyMover : Fly representation*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_RepFlyMover : public GLC_RepMover
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	GLC_RepFlyMover(GLC_Viewport* pViewport);

	//! Copy constructor
	GLC_RepFlyMover(const GLC_RepFlyMover& repFlyMover);

	//! Destructor
	virtual ~GLC_RepFlyMover();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
	//! Return a clone of the flymover
	virtual GLC_RepMover* clone() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
	//! Update the representation
	virtual void update();

	//! Set representation main color
	virtual void setMainColor(const QColor& color);

	//! Set representation wire thickness
	virtual void setThickness(double thickness);

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Virtual interface for OpenGL Geometry set up.
	virtual void glDraw();

//@}


/////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////
private:
	//! Create the plane representation
	void createRepresentation();

//////////////////////////////////////////////////////////////////////
// Private Members
//////////////////////////////////////////////////////////////////////
private:
	//! Center Circle radius
	double m_Radius;

	//! Center Circle
	GLC_3DViewInstance m_CenterCircle;

	//! Plane
	GLC_3DViewInstance m_Plane;

	//! HUD
	GLC_3DViewInstance m_Hud;

	//! HUD offset
	GLC_Vector2d m_HudOffset;
};

#endif /* GLC_REPFLYMOVER_H_ */
