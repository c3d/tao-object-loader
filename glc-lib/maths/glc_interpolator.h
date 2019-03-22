// *****************************************************************************
// glc_interpolator.h                                              Tao3D project
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
// (C) 2010-2011, JÃ©rÃ´me Forissier <jerome@taodyne.com>
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

//! \file glc_interpolator.h interface for the GLC_Interpolator class.

#ifndef GLC_INTERPOLATOR_H_
#define GLC_INTERPOLATOR_H_

#include "glc_vector3d.h"
#include "glc_matrix4x4.h"

#include "../glc_config.h"

// Types d'interpolation
enum INTERPOL_TYPE
{
	INTERPOL_LINEAIRE,
	INTERPOL_ANGULAIRE,
	INTERPOL_HOMOTETIE
};

//////////////////////////////////////////////////////////////////////
//! \class GLC_Interpolator
/*! \brief GLC_Interpolator : Matrix interpolation class*/

/*! An GLC_Interpolator is a class used to interpolate 2 4D matrix*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_Interpolator
{

//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////
public:
	// Contructeur par défaut Interpolation Linéaire
	GLC_Interpolator();

//////////////////////////////////////////////////////////////////////
// Fonctions Set
//////////////////////////////////////////////////////////////////////
public:
	// Défini la matrice d'interpolation
	void SetInterpolMat(int NbrPas, const GLC_Vector3d &VectDepart, const GLC_Vector3d &VectArrive
		, INTERPOL_TYPE Interpolation = INTERPOL_LINEAIRE);
	// Type d'interpolation
	void SetType(INTERPOL_TYPE Interpolation);
	// Nombre de pas
	void SetNbrPas(int NbrPas);
	// Vecteur d'arrivée et de depart
	void SetVecteurs(const GLC_Vector3d &VectDepart, const GLC_Vector3d &VectArrive);

//////////////////////////////////////////////////////////////////////
// Fonctions Get
//////////////////////////////////////////////////////////////////////
public:
	// Retourne la matrice d'interpolation
	GLC_Matrix4x4 GetInterpolMat(void) const
	{
		return m_InterpolMat;
	}

//////////////////////////////////////////////////////////////////////
// Fonctions de Service privée
//////////////////////////////////////////////////////////////////////
private:
	// Calcul La matrice d'interpolation
	bool CalcInterpolMat(void);

	// Calcul la matrice d'interpolation linéaire
	bool CalcInterpolLineaireMat(void);

	// Calcul la matrice d'interpolation angulaire
	bool CalcInterpolAngulaireMat(void);

//////////////////////////////////////////////////////////////////////
// Membres privés
//////////////////////////////////////////////////////////////////////
private:
	// Vecteur de départ
	GLC_Vector3d m_VectDepart;
	// Vecteur d'arriver
	GLC_Vector3d m_VectArrive;

	// Type d'interpolation courante
	INTERPOL_TYPE m_InterpolType;

	// Nombre de pas d'interpolation
	int m_nNbrPas;

	// Matrice d'interpolation
	GLC_Matrix4x4 m_InterpolMat;
};

#endif /*GLC_INTERPOLATOR_H_*/
