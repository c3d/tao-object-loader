// *****************************************************************************
// glc_selectionmaterial.cpp                                       Tao3D project
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
//! \file glc_selectionmaterial.cpp implementation of the GLC_SelectionMaterial class.

#include "../glc_qtopengl.h"

#include "glc_selectionmaterial.h"
#include "glc_material.h"


QHash<const QGLContext*, GLC_Shader*> GLC_SelectionMaterial::m_SelectionShaderHash;
GLC_uint GLC_SelectionMaterial::m_SelectionMaterialId= 0;
GLC_Material* GLC_SelectionMaterial::m_pMaterial= NULL;

GLC_SelectionMaterial::GLC_SelectionMaterial()
{

}

void GLC_SelectionMaterial::useMaterial(GLC_Material* pMaterial)
{
	if (0 == m_SelectionMaterialId)
	{
		m_SelectionMaterialId= glc::GLC_GenUserID();
	}
	Q_ASSERT(NULL != pMaterial);
	if (NULL != m_pMaterial)
	{
		m_pMaterial->delUsage(m_SelectionMaterialId);
		if (m_pMaterial->isUnused())
		{
			delete m_pMaterial;
		}
	}
		m_pMaterial= pMaterial;
		m_pMaterial->addUsage(m_SelectionMaterialId);
}

void GLC_SelectionMaterial::useDefautSelectionColor()
{
	if (NULL != m_pMaterial)
	{
		m_pMaterial->delUsage(m_SelectionMaterialId);
		if (m_pMaterial->isUnused())
		{
			delete m_pMaterial;
		}
		m_pMaterial= NULL;
	}
}


// Execute OpenGL Material
void GLC_SelectionMaterial::glExecute()
{
	if (NULL != m_pMaterial)
	{
		m_pMaterial->glExecute();
	}
	else
	{
		// Use default selection color
		static GLfloat pAmbientColor[4]= {1.0f, 0.376f, 0.223f, 1.0f};

		static GLfloat pDiffuseColor[4]= {1.0f, 0.376f, 0.223f, 1.0f};

		static GLfloat pSpecularColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};

		static GLfloat pLightEmission[4]= {0.0f, 0.0f, 0.0f, 1.0f};

		static float shininess= 50.0f;

		glColor4fv(pAmbientColor);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pAmbientColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pDiffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pSpecularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pLightEmission);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
	}
}

void GLC_SelectionMaterial::initShader(const QGLContext* pContext)
{
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));
	m_SelectionShaderHash.value(pContext)->createAndCompileProgrammShader();
}

void GLC_SelectionMaterial::setShaders(QFile& vertex, QFile& fragment, const QGLContext* pContext)
{
	if (m_SelectionShaderHash.contains(pContext))
	{
		deleteShader(pContext);
	}
	GLC_Shader* pShader= new GLC_Shader;

	pShader->setVertexAndFragmentShader(vertex, fragment);
	m_SelectionShaderHash.insert(pContext, pShader);
}


void GLC_SelectionMaterial::useShader()
{
	const QGLContext* pContext= QGLContext::currentContext();
	Q_ASSERT(NULL != pContext);
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));
	m_SelectionShaderHash.value(pContext)->use();
}

void GLC_SelectionMaterial::unUseShader()
{
	const QGLContext* pContext= QGLContext::currentContext();
	Q_ASSERT(NULL != pContext);
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));

	m_SelectionShaderHash.value(pContext)->unuse();
}

//! delete shader
void GLC_SelectionMaterial::deleteShader(const QGLContext* pContext)
{
	Q_ASSERT(m_SelectionShaderHash.contains(pContext));
	GLC_Shader* pShader= m_SelectionShaderHash.value(pContext);
	pShader->deleteShader();
	delete pShader;
	m_SelectionShaderHash.remove(pContext);
}
