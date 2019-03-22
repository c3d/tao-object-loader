// *****************************************************************************
// glc_zoommover.h                                                 Tao3D project
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

#ifndef GLC_ZOOMMOVER_H_
#define GLC_ZOOMMOVER_H_

#include "glc_mover.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_ZoomMover
/*! \brief GLC_ZoomMover : Zoom interactive manipulation */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_ZoomMover : public GLC_Mover
{
public:
	//! Default constructor
	GLC_ZoomMover(GLC_Viewport*, const QList<GLC_RepMover*>& repsList= QList<GLC_RepMover*>());

	//! Copy constructor
	GLC_ZoomMover(const GLC_ZoomMover&);

	//! Destructor
	virtual ~GLC_ZoomMover();


//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return the maximum zoom factor
	inline double maxZoomFactor() const
	{return m_MaxZoomFactor;}

	//! Return a clone of the mover
	virtual GLC_Mover* clone() const;

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Initialized the mover
	virtual void init(const GLC_UserInput& userInput);

	//! Move the camera
	virtual bool move(const GLC_UserInput& userInput);

	//! Set the maximum zoom factor
	inline void setMaxZoomFactor(const double factor)
	{m_MaxZoomFactor= factor;}

//@}
//////////////////////////////////////////////////////////////////////
// private Members
//////////////////////////////////////////////////////////////////////
private:
	//! The maximum zoom factor
	double m_MaxZoomFactor;

};

#endif /* GLC_ZOOMMOVER_H_ */
