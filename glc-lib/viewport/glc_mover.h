// *****************************************************************************
// glc_mover.h                                                     Tao3D project
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

//! \file glc_mover.h Interface for the GLC_Mover class.

#ifndef GLC_MOVER_H_
#define GLC_MOVER_H_

#include "glc_repmover.h"
#include "../maths/glc_vector3d.h"
#include "glc_userinput.h"

#include <QObject>
#include <QList>

#include "../glc_config.h"

class GLC_Viewport;

//////////////////////////////////////////////////////////////////////
//! \class GLC_Mover
/*! \brief GLC_Mover : Base class for all interactive manipulation */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_Mover : public QObject
{
	Q_OBJECT

public:
	//! Default constructor
	GLC_Mover(GLC_Viewport*, const QList<GLC_RepMover*>&);

	//! Copy constructor
	GLC_Mover(const GLC_Mover&);

	//! Destructor
	virtual ~GLC_Mover();


//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return a clone of the mover
	virtual GLC_Mover* clone() const= 0;
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Initialized the mover
	virtual void init(const GLC_UserInput& userInput)= 0;

	//! Move the camera
	virtual bool move(const GLC_UserInput& userInput)= 0;

	//! Ends this mover
	virtual void ends(){}

	//! Set the mover representation list
	void setRepresentationsList(const QList<GLC_RepMover*>&);

	//! Init representation
	void initRepresentation();

	//! Update representation
	void updateRepresentation();


//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Mover representations list display
	void renderRepresentation();

//@}

signals:
	//! Signal sent if the view as been updated
	void updated();

//////////////////////////////////////////////////////////////////////
// Private services Functions
//////////////////////////////////////////////////////////////////////
private:
	//! Clear mover representation
	void clearMoverRepresentation();

//////////////////////////////////////////////////////////////////////
// Protected Members
//////////////////////////////////////////////////////////////////////
protected:
	//! The mover representations list
	QList<GLC_RepMover*> m_RepMoverList;

	//! The previous mover value
	GLC_Vector3d m_PreviousVector;

	//! The Viewport
	GLC_Viewport* m_pViewport;

	//! The mover info (passed to the rep)
	GLC_RepMover::RepMoverInfo m_MoverInfo;
};

#endif /* GLC_MOVER_H_ */
