// *****************************************************************************
// glc_utils_maths.h                                               Tao3D project
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
//! \file glc_utils_maths.h Mathematic constants.
/*! \brief Definition of usefull constants*/

#ifndef GLC_UTILS_MATHS_H_
#define GLC_UTILS_MATHS_H_

// Standard C math library
#include <math.h>
namespace glc
{
	/*! \def EPSILON
	 *  \brief Define precison of comparaison*/

	const double EPSILON= 1e-10;

	/*! \def PI
	 * \brief Define the magic number PI */
	const double PI= acos(-1.0);

	//! Convert the given degre angle in radian
	inline double toRadian(double angle)
	{return PI * angle / 180.0;}
};

#endif /*GLC_UTILS_MATHS_H_*/
