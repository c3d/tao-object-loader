// *****************************************************************************
// glc_state.cpp                                                   Tao3D project
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
// (C) 2011,2019, Christophe de Dinechin <christophe@dinechin.org>
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
/*

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

//! \file glc_state.cpp implementation of the GLC_State class.

#include "glc_state.h"
#include "glc_ext.h"
#include "sceneGraph/glc_octree.h"

#include <QGLFramebufferObject>

bool GLC_State::m_VboSupported= false;
bool GLC_State::m_UseVbo= true;
bool GLC_State::m_GlslSupported= false;
bool GLC_State::m_PointSpriteSupported= false;
bool GLC_State::m_UseShader= true;
bool GLC_State::m_UseSelectionShader= false;
bool GLC_State::m_IsInSelectionMode= false;
bool GLC_State::m_IsPixelCullingActivated= true;
bool GLC_State::m_IsFrameBufferSupported= false;

QString GLC_State::m_Version;
QString GLC_State::m_Vendor;
QString GLC_State::m_Renderer;

bool GLC_State::m_UseCache= false;

GLC_CacheManager GLC_State::m_CacheManager;

bool GLC_State::m_IsSpacePartitionningActivated= false;
bool GLC_State::m_IsFrustumCullingActivated= false;

GLC_State::~GLC_State()
{
}

bool GLC_State::vboSupported()
{
	return m_VboSupported;
}

bool GLC_State::vboUsed()
{
	return m_UseVbo;
}

bool GLC_State::glslSupported()
{
	return m_GlslSupported;
}

bool GLC_State::frameBufferSupported()
{
	return m_IsFrameBufferSupported;
}

bool GLC_State::glslUsed()
{
	return m_UseShader && m_GlslSupported;
}

bool GLC_State::pointSpriteSupported()
{
	return m_PointSpriteSupported;
}

bool GLC_State::selectionShaderUsed()
{
	return m_UseSelectionShader;
}

bool GLC_State::isInSelectionMode()
{
	return m_IsInSelectionMode;
}

QString GLC_State::version()
{
	return m_Version;
}

QString GLC_State::vendor()
{
	return m_Vendor;
}

QString GLC_State::renderer()
{
	return m_Renderer;
}

bool GLC_State::vendorIsNvidia()
{
	return m_Vendor.contains("NVIDIA");
}

bool GLC_State::isPixelCullingActivated()
{
	return m_IsPixelCullingActivated;
}

bool GLC_State::cacheIsUsed()
{
	return m_UseCache;
}

GLC_CacheManager& GLC_State::currentCacheManager()
{
	return m_CacheManager;
}

bool GLC_State::isSpacePartitionningActivated()
{
	return m_IsSpacePartitionningActivated;
}

int GLC_State::defaultOctreeDepth()
{
	return GLC_Octree::defaultDepth();
}

bool GLC_State::isFrustumCullingActivated()
{
	return m_IsFrustumCullingActivated;
}

void GLC_State::init()
{
	setVboSupport();
	setGlslSupport();
	setPointSpriteSupport();
	setFrameBufferSupport();
	m_Version= (char *) glGetString(GL_VERSION);
	m_Vendor= (char *) glGetString(GL_VENDOR);
	m_Renderer= (char *) glGetString(GL_RENDERER);
}

void GLC_State::setVboSupport()
{
	m_VboSupported= glc::extensionIsSupported("ARB_vertex_buffer_object") && glc::loadVboExtension();
	setVboUsage(m_UseVbo);
}

void GLC_State::setVboUsage(const bool vboUsed)
{
	m_UseVbo= m_VboSupported && vboUsed;
}

void GLC_State::setGlslSupport()
{
	m_GlslSupported= glc::extensionIsSupported("GL_ARB_shading_language_100") && glc::loadGlSlExtension();
	setGlslUsage(m_UseShader);
}

void GLC_State::setPointSpriteSupport()
{
	m_PointSpriteSupported= glc::extensionIsSupported("GL_ARB_point_parameters") && glc::loadPointSpriteExtension();
}

void GLC_State::setFrameBufferSupport()
{
	m_IsFrameBufferSupported= QGLFramebufferObject::hasOpenGLFramebufferObjects();
}

void GLC_State::setGlslUsage(const bool glslUsage)
{
	m_UseShader= m_GlslSupported && glslUsage;
}

void GLC_State::setSelectionShaderUsage(const bool shaderUsed)
{
	m_UseSelectionShader= shaderUsed && m_GlslSupported;
}

void GLC_State::setSelectionMode(const bool mode)
{
	m_IsInSelectionMode= mode;
}

void GLC_State::setPixelCullingUsage(const bool activation)
{
	m_IsPixelCullingActivated= activation;
}

void GLC_State::setCacheUsage(const bool cacheUsage)
{
	m_UseCache= cacheUsage;
}

void GLC_State::setCurrentCacheManager(const GLC_CacheManager& cacheManager)
{
	m_CacheManager= cacheManager;
}

void GLC_State::setSpacePartionningUsage(const bool usage)
{
	m_IsSpacePartitionningActivated= usage;
}

void GLC_State::setDefaultOctreeDepth(int depth)
{
	GLC_Octree::setDefaultDepth(depth);
}

void GLC_State::setFrustumCullingUsage(bool usage)
{
	m_IsFrustumCullingActivated= usage;
}
