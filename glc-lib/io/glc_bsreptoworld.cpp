// *****************************************************************************
// glc_bsreptoworld.cpp                                            Tao3D project
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
//! \file glc_bsreptoworld.cpp implementation of the GLC_BSRepToWorld class.

#include "glc_bsreptoworld.h"
#include "../sceneGraph/glc_world.h"
#include "../glc_fileformatexception.h"

GLC_BSRepToWorld::GLC_BSRepToWorld()
{

}

GLC_BSRepToWorld::~GLC_BSRepToWorld()
{

}

GLC_World* GLC_BSRepToWorld::CreateWorldFromBSRep(QFile &file)
{
	//////////////////////////////////////////////////////////////////
	// Test if the file exist and can be opened
	//////////////////////////////////////////////////////////////////
	if (!file.open(QIODevice::ReadOnly))
	{
		QString fileName(file.fileName());
		QString message(QString("GLC_BSRepToWorld::CreateWorldFromBSRep File ") + fileName + QString(" doesn't exist"));
		GLC_FileFormatException fileFormatException(message, fileName, GLC_FileFormatException::FileNotFound);
		throw(fileFormatException);
	}
	file.close();
	GLC_BSRep bsRep(file.fileName());
	GLC_3DRep rep(bsRep.loadRep());
	GLC_World* pWorld= new GLC_World();
	pWorld->rootOccurence()->addChild(new GLC_StructOccurence(new GLC_3DRep(rep)));
	return pWorld;
}
