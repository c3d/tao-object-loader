// *****************************************************************************
// glc_flymover.h                                                  Tao3D project
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
//! \file glc_flymover.h Interface for the GLC_FlyMover class.

#ifndef GLC_FLYMOVER_H_
#define GLC_FLYMOVER_H_
#include "glc_mover.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_FlyMover
/*! \brief GLC_FlyMover : Fly Mode interactive manipulation */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_FlyMover : public GLC_Mover
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Default constructor
	GLC_FlyMover(GLC_Viewport*, const QList<GLC_RepMover*>& repsList= QList<GLC_RepMover*>());

	//! Copy constructor
	GLC_FlyMover(const GLC_FlyMover& flyMover);

	//! Destructor
	virtual ~GLC_FlyMover();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return a clone of the mover
	virtual GLC_Mover* clone() const;

	//! Return the turning rate in degres
	inline double turningRate() const
	{return m_TurnRate / glc::PI * 180.0;}

	//! Return the flying velocity
	inline double flyingVelocity() const
	{return m_Velocity;}
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

	//! Ends this mover
	virtual void ends();

	//! Set the maximum turning rate in degre
	inline void setMaximumTurnRate(double turnRate)
	{m_TurnRate= turnRate;}

	//! Set the flying velocity
	void setFlyingVelocity(double velocity);

	//! increase the flying velocity
	void increaseVelocity(double factor);

//@}

protected:
    void timerEvent(QTimerEvent*);

/////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////
private:
	//! Map the position of mouse for the fly mode
	GLC_Vector3d mapForFlying(double x, double y);

	//! Fly
	void fly();

//////////////////////////////////////////////////////////////////////
// Private Members
//////////////////////////////////////////////////////////////////////
private:
	//! THe turning rate
	double m_TurnRate;

	//! The timer id
	int m_TimerId;

	//! the timer interval
	int m_TimerInterval;

	//! fly velocity
	double m_Velocity;

};

#endif /* GLC_FLYMOVER_H_ */
