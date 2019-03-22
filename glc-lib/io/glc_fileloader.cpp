// *****************************************************************************
// glc_fileloader.cpp                                              Tao3D project
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
// (C) 2011, Jérôme Forissier <jerome@taodyne.com>
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

//! \file glc_filetoworld.cpp implementation of the GLC_FileToWorld class.


#include "glc_fileloader.h"

#include "glc_objtoworld.h"
#include "glc_stltoworld.h"
#include "glc_offtoworld.h"
#include "glc_3dstoworld.h"
#include "glc_3dxmltoworld.h"
#include "glc_colladatoworld.h"
#include "glc_bsreptoworld.h"

#include "../sceneGraph/glc_world.h"
#include "../glc_fileformatexception.h"
#include "../glc_factory.h"
#include "glc_worldreaderplugin.h"

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////
GLC_FileLoader::GLC_FileLoader()
{
}

GLC_FileLoader::~GLC_FileLoader()
{
}

/////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Create an GLC_World from an input File
GLC_World GLC_FileLoader::createWorldFromFile(QFile &file, QStringList* pAttachedFileName)
{
#if defined(Q_OS_WIN)
	// We need to force the connection type on Windows, not sure why:
	// Qt::DirectConnection should be selected automatically since
	// source and destination are in the same thread...
#define connect(snd, sig, rcv, memb) \
        connect(snd, sig, rcv, memb, Qt::DirectConnection)
#endif

	const QString suffix= QFileInfo(file).suffix();
	if (GLC_Factory::canBeLoaded(suffix))
	{
		GLC_WorldReaderHandler* pReaderHandler= GLC_Factory::loadingHandler(file.fileName());
		if (NULL != pReaderHandler)
		{
			qDebug() << "Use STL plugin";
			QObject* pObject= dynamic_cast<QObject*>(pReaderHandler);
			Q_ASSERT(NULL != pObject);
			connect(pObject, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
			GLC_World resultWorld= pReaderHandler->read(&file);
			if (NULL != pAttachedFileName)
			{
				(*pAttachedFileName)= pReaderHandler->listOfAttachedFileName();
			}

			delete pReaderHandler;
			return resultWorld;
		}
	}
	GLC_World* pWorld= NULL;
	if (QFileInfo(file).suffix().toLower() == "obj")
	{
		GLC_ObjToWorld objToWorld;
		connect(&objToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= objToWorld.CreateWorldFromObj(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= objToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "stl")
	{
		GLC_StlToWorld stlToWorld;
		connect(&stlToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= stlToWorld.CreateWorldFromStl(file);
	}
	else if (QFileInfo(file).suffix().toLower() == "off")
	{
		GLC_OffToWorld offToWorld;
		connect(&offToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= offToWorld.CreateWorldFromOff(file);
	}
	else if (QFileInfo(file).suffix().toLower() == "3ds")
	{
		GLC_3dsToWorld studioToWorld;
		connect(&studioToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= studioToWorld.CreateWorldFrom3ds(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= studioToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "3dxml")
	{
		GLC_3dxmlToWorld d3dxmlToWorld;
		connect(&d3dxmlToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= d3dxmlToWorld.createWorldFrom3dxml(file, false);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= d3dxmlToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "dae")
	{
		GLC_ColladaToWorld colladaToWorld;
		connect(&colladaToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
		pWorld= colladaToWorld.CreateWorldFromCollada(file);
		if (NULL != pAttachedFileName)
		{
			(*pAttachedFileName)= colladaToWorld.listOfAttachedFileName();
		}
	}
	else if (QFileInfo(file).suffix().toLower() == "bsrep")
	{
		GLC_BSRepToWorld bsRepToWorld;
		pWorld= bsRepToWorld.CreateWorldFromBSRep(file);
		emit currentQuantum(100);
	}

	if (NULL == pWorld)
	{
		// File extension not recognize or file not loaded
		QString message(QString("GLC_Factory::createWorldFromFile File ") + file.fileName() + QString(" not loaded"));
		GLC_FileFormatException fileFormatException(message, file.fileName(), GLC_FileFormatException::FileNotSupported);
		throw(fileFormatException);
	}
	GLC_World resulWorld(*pWorld);
	delete pWorld;

	return resulWorld;
}
