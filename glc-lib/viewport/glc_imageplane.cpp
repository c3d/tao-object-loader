// *****************************************************************************
// glc_imageplane.cpp                                              Tao3D project
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

//! \file glc_imagePlane.cpp implementation of the GLC_ImagePlane class.
#include "glc_imageplane.h"
#include "glc_viewport.h"
#include "../glc_openglexception.h"
#include "../glc_factory.h"
#include <QtDebug>

//////////////////////////////////////////////////////////////////////
// Constructor Destructor
//////////////////////////////////////////////////////////////////////

GLC_ImagePlane::GLC_ImagePlane(const QString& ImageName)
: m_Material()
{
	GLC_Texture* pImgTexture= GLC_Factory::instance()->createTexture(ImageName);
	pImgTexture->setMaxTextureSize(pImgTexture->imageOfTexture().size());
	m_Material.setTexture(pImgTexture);
}

GLC_ImagePlane::GLC_ImagePlane(const QImage& image)
: m_Material()
{
	GLC_Texture* pImgTexture= GLC_Factory::instance()->createTexture(image);
	pImgTexture->setMaxTextureSize(image.size());
	m_Material.setTexture(pImgTexture);
}

GLC_ImagePlane::~GLC_ImagePlane()
{

}

//////////////////////////////////////////////////////////////////////
// OpenGL Functions
//////////////////////////////////////////////////////////////////////

void GLC_ImagePlane::render()
{
	m_Material.glExecute();
	// Display info area
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);

		glNormal3d(0.0, 0.0, 1.0);	// Z
		glTexCoord2f(0.0f, 0.0f); glVertex3d(-1.0, -1.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3d(1.0, -1.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3d(1.0, 1.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3d(-1.0, 1.0, 0.0);

	glEnd();
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
