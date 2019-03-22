// *****************************************************************************
// glc_interpolator.cpp                                            Tao3D project
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
// (C) 2010-2011, J√©r√¥me Forissier <jerome@taodyne.com>
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

//! \file glc_interpolator.cpp implementation of the GLC_Interpolator class.

#include "glc_interpolator.h"

using namespace glc;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Contructeur par dÈfaut Interpolation LinÈaire
GLC_Interpolator::GLC_Interpolator()
: m_InterpolType(INTERPOL_LINEAIRE)
, m_nNbrPas(1)
{

}

//////////////////////////////////////////////////////////////////////
// Fonction Set
//////////////////////////////////////////////////////////////////////
// DÈfini la matrice d'interpolation
void GLC_Interpolator::SetInterpolMat(int NbrPas, const GLC_Vector3d &VectDepart, const GLC_Vector3d &VectArrive
								   , INTERPOL_TYPE Interpolation)
{
	// Mise ‡ jour des donnÈes membre
	m_InterpolType= Interpolation;
	if (!NbrPas)
	{
		//TRACE("GLC_Interpolator::SetInterpolMat -> NbrPas == 0 \n");
	}
	else m_nNbrPas= NbrPas;

	m_VectDepart= VectDepart;
	m_VectArrive= VectArrive;
	// Calcul de la matrice d'interpolation
	CalcInterpolMat();
}
// Type d'interpolation
void GLC_Interpolator::SetType(INTERPOL_TYPE Interpolation)
{
	if (m_InterpolType != Interpolation)
	{
		m_InterpolType= Interpolation;
		// Calcul de la matrice d'interpolation
		CalcInterpolMat();
	}
}
// Nombre de pas
void GLC_Interpolator::SetNbrPas(int NbrPas)
{
	if (!NbrPas)
	{
		//TRACE("GLC_Interpolator::SetNbrPas -> NbrPas == 0 \n");
		return;
	}

	if (m_nNbrPas != NbrPas)
	{
		m_nNbrPas= NbrPas;
		// Calcul de la matrice d'interpolation
		CalcInterpolMat();
	}
}
// Vecteur d'arrivÈe et de depart
void GLC_Interpolator::SetVecteurs(const GLC_Vector3d &VectDepart, const GLC_Vector3d &VectArrive)
{
	m_VectDepart= VectDepart;
	m_VectArrive= VectArrive;

	// Calcul de la matrice d'interpolation
	CalcInterpolMat();

}

//////////////////////////////////////////////////////////////////////
// Fonction Get
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Fonctions de Service privÈe
//////////////////////////////////////////////////////////////////////
// Calcul La matrice d'interpolation
bool GLC_Interpolator::CalcInterpolMat(void)
{
	// Verifie que les vecteur d'arrivÈ et de dÈpart sont diffÈrent
	if (m_VectDepart == m_VectArrive)
	{
		//TRACE("GLC_Interpolator::CalcInterpolMat : Depart == Arrive\n");
		return false;
	}

	switch (m_InterpolType)
	{
	case INTERPOL_LINEAIRE:
		return CalcInterpolLineaireMat();
		break;

	case INTERPOL_ANGULAIRE:
		return CalcInterpolAngulaireMat();
		break;

	case INTERPOL_HOMOTETIE:
		return false;
		break;

	default:
		//TRACE("GLC_Interpolator::CalcInterpolMat : Type d'interpolation non valide\n");
		return false;
	}
}

// Calcul la matrice d'interpolation linÈaire
bool GLC_Interpolator::CalcInterpolLineaireMat(void)
{

	// Calcul la matrice de translation
	const GLC_Vector3d VectTrans= (m_VectArrive - m_VectDepart) * (1.0 / m_nNbrPas);
	if(VectTrans.isNull())
	{
		//TRACE("GLC_Interpolator::CalcInterpolLineaireMat -> Translation NULL\n");
		m_InterpolMat.setToIdentity();
		return false;
	}
	else
	{
		m_InterpolMat.setMatTranslate(VectTrans);
		return true;
	}
}

// Calcul la matrice d'interpolation angulaire
bool GLC_Interpolator::CalcInterpolAngulaireMat(void)
{
	// Calcul de l'axe de rotation
	const GLC_Vector3d AxeRot(m_VectDepart ^ m_VectArrive);
	// Calcul de l'angle entre les vecteurs
	const double Angle= m_VectArrive.angleWithVect(m_VectDepart) / m_nNbrPas;
	// Calcul de la matrice de rotation
	if (qFuzzyCompare(Angle, 0.0))
	{
		//TRACE("GLC_Interpolator::CalcInterpolAngulaireMat -> Rotation NULL\n");
		m_InterpolMat.setToIdentity();
		return false;
	}
	else
	{
		m_InterpolMat.setMatRot( AxeRot, Angle);
		return true;
	}
}



