// *****************************************************************************
// glc_vector2df.h                                                 Tao3D project
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

//! \file glc_vector2df.h interface for the GLC_Vector2df class.

#ifndef GLC_VECTOR2DF_H_
#define GLC_VECTOR2DF_H_

#include "glc_utils_maths.h"

#include "../glc_config.h"

//////////////////////////////////////////////////////////////////////
// definition global
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! \class GLC_Vector2df
/*! \brief GLC_Vector2df is a 2 dimensions Vector*/

/*! GLC_Vector2df is used to represent 2D position and vectors.
 * */
//////////////////////////////////////////////////////////////////////

class GLC_LIB_EXPORT GLC_Vector2df
{
	friend class GLC_Vector2d;
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	/*! Default constructor
	*  Value is set to
	* \n X = 0.0
	* \n Y =  0.0
	*/
	inline GLC_Vector2df()
	{
		vector[0]= 0.0f;
		vector[1]= 0.0f;
	}

	//! Standard constructor With x, y = 0.0
	inline GLC_Vector2df(const float &dX, const float &dY)
	{
		vector[0]= dX;
		vector[1]= dY;
	}

	/*! Copy constructor
	 * Sample use
	 * \code
	 * NewVect = new GLC_Vector2d(OldVect);
	 * \endcode
	 */
	inline GLC_Vector2df(const GLC_Vector2df &Vect)
	{
		vector[0]= Vect.vector[0];
		vector[1]= Vect.vector[1];
	}
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! X Compound
	inline GLC_Vector2df& setX(const float &dX)
	{
		vector[0]= dX;
		return *this;
	}

	//! Y Compound
	inline GLC_Vector2df& setY(const float &dY)
	{
		vector[1]= dY;
		return *this;
	}

	//! All Compound
	inline GLC_Vector2df& setVect(const float &dX, const float &dY)
	{
		vector[0]= dX;
		vector[1]= dY;
		return *this;
	}

	//! From another Vector
	inline GLC_Vector2df& setVect(const GLC_Vector2df &Vect)
	{
		vector[0]= Vect.vector[0];
		vector[1]= Vect.vector[1];
		return *this;
	}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return X Compound
	inline float X(void) const
	{
		return vector[0];
	}
	//! Return Y Compound
	inline float Y(void) const
	{
		return vector[1];
	}
	//! Return a pointer to vector data
	inline const float *return_dVect(void) const
	{
		return vector;
	}
	//! Return true if the vector is null
	inline bool isNull(void) const
	{
		return (qFuzzyCompare(vector[0], 0.0f) && qFuzzyCompare(vector[1], 0.0f));
	}

//@}

//////////////////////////////////////////////////////////////////////
//name Private attributes
//////////////////////////////////////////////////////////////////////
private:
	/*! Vector array definition \n
	*	vector[0]	X \n
	*	vector[1]	Y \n
	*/
	float vector[2];

}; //class GLC_Vector2df

//! Define GLC_Point2D
typedef GLC_Vector2df GLC_Point2df;

#endif /*GLC_VECTOR2DF_H_*/
