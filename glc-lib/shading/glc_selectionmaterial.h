// *****************************************************************************
// glc_selectionmaterial.h                                         Tao3D project
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
// (C) 2011-2012, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2012,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2012, Jérôme Forissier <jerome@taodyne.com>
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
//! \file glc_selectionmaterial.h interface for the GLC_SelectionMaterial class.

#ifndef GLC_SELECTIONMATERIAL_H_
#define GLC_SELECTIONMATERIAL_H_

#include <QColor>
#include "../glc_qtopengl.h"
#include <QHash>

#include "../glc_ext.h"
#include "glc_shader.h"

#include "../glc_config.h"

class QGLContext;
class GLC_Material;

//////////////////////////////////////////////////////////////////////
//! \class GLC_SelectionMaterial
/*! \brief GLC_SelectionMaterial : Material used for selection feedback*/

//////////////////////////////////////////////////////////////////////

class GLC_LIB_EXPORT GLC_SelectionMaterial
{
private:
	GLC_SelectionMaterial();


//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Use the given material as selection material
	static void useMaterial(GLC_Material* pMaterial);

	//! Use the default selection color
	/*! if a selection material is used, unused it*/
	static void useDefautSelectionColor();

//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Execute OpenGL Material
	static void glExecute();
	//! Init shader
	static void initShader(const QGLContext* pContext);
	//! delete shader
	static void deleteShader(const QGLContext* pContext);
	//! Set shader
	static void setShaders(QFile& vertex, QFile& fragment, const QGLContext* pContext);
	//! Use shader
	static void useShader();
	//! Unused shader
	static void unUseShader();

//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////

private:
		//! Selection Shader
		static QHash<const QGLContext*, GLC_Shader*> m_SelectionShaderHash;

		//! Selection material id
		static GLC_uint m_SelectionMaterialId;

		//! Material of this selection material
		static GLC_Material* m_pMaterial;

};

#endif /*GLC_SELECTIONMATERIAL_H_*/
