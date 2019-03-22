// *****************************************************************************
// glc_reptrackballmover.h                                         Tao3D project
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
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
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
//! \file glc_reptrackballmover.h Interface for the GLC_RepTrackBallMover class.

#ifndef GLC_REPTRACKBALLMOVER_H_
#define GLC_REPTRACKBALLMOVER_H_

#include "glc_repmover.h"
#include "../geometry/glc_circle.h"
#include "../sceneGraph/glc_3dviewinstance.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_RepTrackBallMover
/*! \brief GLC_RepTrackBallMover : Track Ball representation*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_RepTrackBallMover : public GLC_RepMover
{
public:
	//! Default constructor
	GLC_RepTrackBallMover(GLC_Viewport*);

	//! Copy constructor
	GLC_RepTrackBallMover(const GLC_RepTrackBallMover&);

	// Destructor
	virtual ~GLC_RepTrackBallMover();


//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
	//! Return a clone of the repmover
	virtual GLC_RepMover* clone() const;
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:

	//! Set Arcs orientation and position in concordance with mouse position
	virtual void init();

	//! Set Arcs position in concordance with mouse position
	virtual void update();

	//! Set representation main color
	virtual void setMainColor(const QColor& color);

	//! Set representation screen ration
	inline void setRatio(double ratio)
	{m_Ratio= ratio;}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Virtual interface for OpenGL Geometry set up.
	virtual void glDraw();

//@}

//////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////
private:
	//! Compute trackball radius
	void computeRadius();

//////////////////////////////////////////////////////////////////////
// Private Members
//////////////////////////////////////////////////////////////////////
private:
	//! trackball radius
	double m_Radius;
	//! Main Circle
	GLC_Circle m_MainCircle;
	//! Arc 1 showing orbit sphere orientation
	GLC_3DViewInstance m_Arc1;
	//! Arc 1 positionning Matrix
	GLC_Matrix4x4 m_MatArc1;
	//! Arc 2 showing orbit sphere orientation
	GLC_3DViewInstance m_Arc2;
	//! Arc 2 positionning Matrix
	GLC_Matrix4x4 m_MatArc2;
	//! The ratio of the trackball size
	double m_Ratio;

};

#endif /* GLC_REPTRACKBALLMOVER_H_ */
