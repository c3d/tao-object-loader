// *****************************************************************************
// glc_worldreaderhandler.h                                        Tao3D project
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

//! \file glc_worldreaderplugin.h interface for reading world from 3D model

#ifndef GLC_WORLDREADERHANDLER_H_
#define GLC_WORLDREADERHANDLER_H_

class QFile;
#include <QStringList>

#include "../sceneGraph/glc_world.h"

class GLC_WorldReaderHandler
{
public:
	virtual ~GLC_WorldReaderHandler(){}

	//! Read a world from the given file
	virtual GLC_World read(QFile* pFile)= 0;

	//! Get the list of attached files
	virtual QStringList listOfAttachedFileName() const{return QStringList();}

};

#endif /* GLC_WORLDREADERHANDLER_H_ */
