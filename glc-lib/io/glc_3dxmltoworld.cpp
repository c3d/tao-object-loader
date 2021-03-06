// *****************************************************************************
// glc_3dxmltoworld.cpp                                            Tao3D project
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

//! \file glc_3dxmltoworld.cpp implementation of the GLC_3dxmlToWorld class.

#include "glc_3dxmltoworld.h"
#include "../sceneGraph/glc_world.h"
#include "../glc_fileformatexception.h"
#include "../geometry/glc_mesh.h"
#include "../geometry/glc_3drep.h"
#include "glc_xmlutil.h"

// Quazip library
#include "../3rdparty/quazip/quazip.h"
#include "../3rdparty/quazip/quazipfile.h"

#include <QString>
#include <QGLContext>
#include <QFileInfo>
#include <QSet>
#include <QMutexLocker>

//using namespace glcXmlUtil;

QMutex GLC_3dxmlToWorld::m_ZipMutex;

static qint64 chunckSize= 10000000;

GLC_3dxmlToWorld::GLC_3dxmlToWorld()
: QObject()
, m_pStreamReader(NULL)
, m_FileName()
, m_p3dxmlArchive(NULL)
, m_pCurrentFile(NULL)
, m_RootName()
, m_pWorld(NULL)
, m_ReferenceHash()
, m_AssyLinkList()
, m_InstanceOf()
, m_SetOfExtRef()
, m_InstanceOfExtRefHash()
, m_ExternalReferenceHash()
, m_MaterialHash()
, m_IsInArchive(false)
, m_ReferenceRepHash()
, m_LocalRepLinkList()
, m_ExternRepLinkList()
, m_SetOfExtRep()
, m_pCurrentMaterial(NULL)
, m_TextureImagesHash()
, m_LoadStructureOnly(false)
, m_SetOfAttachedFileName()
, m_CurrentFileName()
, m_CurrentDateTime()
, m_OccurenceAttrib()
, m_GetExternalRef3DName(false)
, m_ByteArrayList()
{

}

GLC_3dxmlToWorld::~GLC_3dxmlToWorld()
{
	delete m_pStreamReader;
	m_pStreamReader= NULL;

	delete m_pCurrentFile;
	delete m_p3dxmlArchive;

	clearMaterialHash();

	// Clear specific attributes hash table
	QHash<unsigned int, OccurenceAttrib*>::iterator iAttrib= m_OccurenceAttrib.begin();
	while (m_OccurenceAttrib.constEnd() != iAttrib)
	{
		delete iAttrib.value();
		++iAttrib;
	}
	m_OccurenceAttrib.clear();
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////

// Create an GLC_World from an input 3DXML File
GLC_World* GLC_3dxmlToWorld::createWorldFrom3dxml(QFile &file, bool structureOnly, bool getExternalRef)
{
	clear();
	m_pWorld= new GLC_World();
	m_GetExternalRef3DName= getExternalRef;
	m_LoadStructureOnly= structureOnly;
	m_FileName= file.fileName();

	// Create the 3dxml Zip archive
	m_p3dxmlArchive= new QuaZip(m_FileName);
	// Trying to load archive
	if(!m_p3dxmlArchive->open(QuaZip::mdUnzip))
	{
	  // In this case, the 3dxml is not compressed or is not valid
	  m_RootName= m_FileName;
	  delete m_p3dxmlArchive;
	  m_p3dxmlArchive= NULL;
	}
	else
	{
		// Get the 3DXML time stamp
		m_CurrentDateTime= QFileInfo(m_FileName).lastModified();

		m_IsInArchive= true;
		// Set the file Name Codec
		//m_p3dxmlArchive->setFileNameCodec("IBM866");

		// Load the manifest
		loadManifest();
	}

	if (!m_LoadStructureOnly)
	{
		// Trying to Load CATRepImage file
		loadCatRepImage();

		// Trying to Load CATRefMaterial File
		loadCatMaterialRef();
	}

	// Load the product structure
	loadProductStructure();


	emit currentQuantum(100);
	return m_pWorld;
}

// Create 3DRep from an 3DXML rep
GLC_3DRep GLC_3dxmlToWorld::create3DrepFrom3dxmlRep(const QString& fileName)
{
	GLC_3DRep resultRep;
	if (glc::isArchiveString(fileName))
	{
		m_FileName= glc::archiveFileName(fileName);

		// Create the 3dxml Zip archive
		m_ZipMutex.lock();
		m_p3dxmlArchive= new QuaZip(m_FileName);
		// Trying to load archive
		if(!m_p3dxmlArchive->open(QuaZip::mdUnzip))
		{
		  delete m_p3dxmlArchive;
		  return GLC_3DRep();
		}
		else
		{
			m_IsInArchive= true;
			// Set the file Name Codec
			//m_p3dxmlArchive->setFileNameCodec("IBM866");
		}
		m_ZipMutex.unlock();
		m_CurrentFileName= glc::archiveEntryFileName(fileName);

		// Get the 3DXML time stamp
		m_CurrentDateTime= QFileInfo(QFileInfo(m_FileName).absolutePath() + QDir::separator() + QFileInfo(fileName).fileName()).lastModified();
	}
	else if (glc::isFileString(fileName))
	{
		m_FileName= glc::archiveFileName(fileName);
		m_CurrentFileName= glc::archiveEntryFileName(fileName);

		// Get the rep time stamp
		m_CurrentDateTime= QFileInfo(m_CurrentFileName).lastModified();

		// Keep only the file name
		QDir structureDir(QFileInfo(m_FileName).absolutePath() + QDir::separator());
		m_CurrentFileName= structureDir.relativeFilePath(m_CurrentFileName);

	}
	else
	{
		return resultRep;
	}


	setRepresentationFileName(&resultRep);

	if (QFileInfo(m_CurrentFileName).suffix().toLower() == "3dxml")
	{
		if (GLC_State::cacheIsUsed() && GLC_State::currentCacheManager().isUsable(m_CurrentDateTime, QFileInfo(m_FileName).baseName(), QFileInfo(m_CurrentFileName).fileName()))
		{
			GLC_CacheManager cacheManager = GLC_State::currentCacheManager();

			GLC_BSRep binaryRep = cacheManager.binary3DRep(QFileInfo(m_FileName).baseName(), QFileInfo(m_CurrentFileName).fileName());
			resultRep = binaryRep.loadRep();
		}
		else
		{
			if (setStreamReaderToFile(m_CurrentFileName, true))
			{
				GLC_StructReference* pStructRef = createReferenceRep(QString(), NULL);
				GLC_3DRep* pRep = NULL;
				if ((NULL != pStructRef) && pStructRef->hasRepresentation())
				{
					pRep= dynamic_cast<GLC_3DRep*> (pStructRef->representationHandle());

					if (NULL != pRep)
					{
						resultRep = GLC_3DRep(*pRep);
						resultRep.setName(pStructRef->name());
					}
				}
				delete pStructRef;
			}
		}
	}
	else if (QFileInfo(m_CurrentFileName).suffix().toLower() == "3drep")
	{
        if (GLC_State::cacheIsUsed() && GLC_State::currentCacheManager().isUsable(m_CurrentDateTime, QFileInfo(m_FileName).baseName(), QFileInfo(m_CurrentFileName).fileName()))
		{
			GLC_CacheManager cacheManager = GLC_State::currentCacheManager();
			GLC_BSRep binaryRep = cacheManager.binary3DRep(QFileInfo(m_FileName).baseName(), QFileInfo(m_CurrentFileName).fileName());
			resultRep = binaryRep.loadRep();
		}
		else
		{
			if (setStreamReaderToFile(m_CurrentFileName, true))
			{
				resultRep = loadCurrentExtRep();
			}
		}
	}
	resultRep.clean();

	return resultRep;
}

//////////////////////////////////////////////////////////////////////
// Private services functions
//////////////////////////////////////////////////////////////////////
// Load the 3dxml's manifest
void GLC_3dxmlToWorld::loadManifest()
{
	setStreamReaderToFile("Manifest.xml");
	m_RootName= getContent(m_pStreamReader, "Root");

	if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadManifest Manifest file ") + m_FileName + " doesn't contains Root Element");
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

	delete m_pStreamReader;
	m_pStreamReader= NULL;
}

//! Close all files and clear memmory
void GLC_3dxmlToWorld::clear()
{
	delete m_pWorld;
	m_pWorld= NULL;

	delete m_pStreamReader;
	m_pStreamReader= NULL;

	m_ByteArrayList.clear();
	// Clear current file
	if (NULL != m_pCurrentFile)
	{
		m_pCurrentFile->close();
		delete m_pCurrentFile;
		m_pCurrentFile= NULL;
	}

	// Clear the 3dxml archive
	if (NULL != m_p3dxmlArchive)
	{
		m_p3dxmlArchive->close();
		delete m_p3dxmlArchive;
		m_p3dxmlArchive= NULL;
	}

	m_SetOfAttachedFileName.clear();

	clearMaterialHash();
}

// Go to a Rep of a xml
void GLC_3dxmlToWorld::goToRepId(const QString& id)
{
	while(!m_pStreamReader->atEnd() && !((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "Representation")
			&& (m_pStreamReader->attributes().value("id").toString() == id)))
	{
		readNext();;
	}

}

// Go to Polygonal Rep Type
void GLC_3dxmlToWorld::gotToPolygonalRepType()
{
	while(endElementNotReached(m_pStreamReader, "Representation") && !m_pStreamReader->atEnd() && !((QXmlStreamReader::StartElement == m_pStreamReader->tokenType())
			&& ((m_pStreamReader->name() == "Rep") || (m_pStreamReader->name() == "Root"))
			&& (m_pStreamReader->attributes().value("xsi:type").toString() == "PolygonalRepType")))
	{
		//qDebug() << m_pStreamReader->name();
		//qDebug() << m_pStreamReader->attributes().value("xsi:type").toString();
		readNext();;
	}

}

// Read the specified attribute
QString GLC_3dxmlToWorld::readAttribute(const QString& name, bool required)
{
	QString attributeValue;
	if (required && !m_pStreamReader->attributes().hasAttribute(name))
	{
		QString message(QString("required attribute ") + name + QString(" Not found"));
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}
	else
	{
		attributeValue= m_pStreamReader->attributes().value(name).toString();
	}
	return attributeValue;
}

// Load the product structure
void GLC_3dxmlToWorld::loadProductStructure()
{
	setStreamReaderToFile(m_RootName);
	goToElement(m_pStreamReader, "ProductStructure");
	if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadProductStructure Element ProctStructure Not found in ") + m_FileName);
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

	// Load the structure
	while(endElementNotReached(m_pStreamReader, "ProductStructure"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType())
				&& ((m_pStreamReader->name() == "Reference3D") || (m_pStreamReader->name() == "Instance3D")
						|| (m_pStreamReader->name() == "ReferenceRep") || (m_pStreamReader->name() == "InstanceRep")))
		{
			if (m_pStreamReader->name() == "Reference3D") loadReference3D();
			else if (m_pStreamReader->name() == "Instance3D") loadInstance3D();
			else if (m_pStreamReader->name() == "ReferenceRep") loadReferenceRep();
			else loadInstanceRep();
		}

		readNext();;
	}

	// Load Default view properties
	while(endElementNotReached(m_pStreamReader, "Model_3dxml"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType())
				&& ((m_pStreamReader->name() == "DefaultView") || (m_pStreamReader->name() == "GeometricRepresentationSet")))
		{
			if (m_pStreamReader->name() == "DefaultView") loadGraphicsProperties();
			else if (m_pStreamReader->name() == "GeometricRepresentationSet") loadLocalRepresentations();

		}
		readNext();;
	}

	// Check if an error Occur
	if (m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadProductStructure An error occur in ") + m_FileName);
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

	// Load external ref (3DXML V3)
	loadExternalRef3D();

	// Load extern representations (3DXML V4)
	loadExternRepresentations();

	{ // Link locals instance with reference
		InstanceOfHash::iterator iInstance= m_InstanceOf.begin();
		while (iInstance != m_InstanceOf.constEnd())
		{
			GLC_StructInstance* pInstance= iInstance.key();
			GLC_StructReference* pRef= m_ReferenceHash.value(iInstance.value());
			if (NULL == pRef)
			{
				QString message(QString("GLC_3dxmlToWorld::loadProductStructure a instance reference a non existing reference"));
				message.append(" Instance name " + pInstance->name());
				GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
				clear();
				throw(fileFormatException);
			}
			pInstance->setReference(pRef);

			++iInstance;
		}
		m_InstanceOf.clear();

	}
	//qDebug() << "Local instance linked with reference";

	{ // Link external instance with reference

		InstanceOfExtRefHash::iterator iInstance= m_InstanceOfExtRefHash.begin();
		while (iInstance != m_InstanceOfExtRefHash.constEnd())
		{
			GLC_StructInstance* pInstance= iInstance.key();
			GLC_StructReference* pRef;
			if (m_ExternalReferenceHash.contains(iInstance.value()))
			{
				pRef= m_ExternalReferenceHash.value(iInstance.value());
			}
			else
			{
				QString referenceName= pInstance->name();
				referenceName= referenceName.left(pInstance->name().lastIndexOf('.'));
				QStringList stringList(m_FileName);
				stringList.append("Reference not found : " + referenceName);
				GLC_ErrorLog::addError(stringList);
				pRef= new GLC_StructReference(referenceName);
			}

			pInstance->setReference(pRef);

			++iInstance;
		}

		// Check usage of reference in the external reference hash
		ExternalReferenceHash::const_iterator iRef= m_ExternalReferenceHash.constBegin();
		while (m_ExternalReferenceHash.constEnd() != iRef)
		{
			GLC_StructReference* pRef= iRef.value();
			if (! pRef->hasStructInstance())
			{
				QStringList stringList(m_FileName);
				stringList.append("Orphan reference : " + pRef->name());
				GLC_ErrorLog::addError(stringList);
				delete pRef;
			}
			++iRef;
		}
		m_ExternalReferenceHash.clear();

	}
	//qDebug() << "external instance linked with reference";

	// Create the unfolded tree
	createUnfoldedTree();

	// Update occurence number
	m_pWorld->rootOccurence()->updateOccurenceNumber(1);

	// Change occurence attributes
	if (! m_OccurenceAttrib.isEmpty())
	{
		//qDebug() << "Not visible occurence= " << m_OccurenceAttrib.size();
		QList<GLC_StructOccurence*> occurenceList= m_pWorld->listOfOccurence();
		const int size= occurenceList.size();
		for (int i= 0; i < size; ++i)
		{
			if (m_OccurenceAttrib.contains(occurenceList.at(i)->occurenceNumber()))
			{
				OccurenceAttrib* pOccurenceAttrib= m_OccurenceAttrib.value(occurenceList.at(i)->occurenceNumber());
				occurenceList.at(i)->setVisibility(pOccurenceAttrib->m_IsVisible);
				if (NULL != pOccurenceAttrib->m_pRenderProperties)
				{
					occurenceList.at(i)->setRenderProperties(*(pOccurenceAttrib->m_pRenderProperties));
				}
			}
		}
	}
	// Check usage of Instance
	InstanceOfExtRefHash::const_iterator iInstance= m_InstanceOfExtRefHash.constBegin();
	while (m_InstanceOfExtRefHash.constEnd() != iInstance)
	{
		GLC_StructInstance* pInstance= iInstance.key();
		if (!pInstance->hasStructOccurence())
		{
			QStringList stringList(m_FileName);
			stringList.append("Orphan Instance : " + pInstance->name());
			GLC_ErrorLog::addError(stringList);
			delete pInstance;
		}
		else
		{
			QList<GLC_StructOccurence*> occurences= pInstance->listOfStructOccurences();
			const int size= occurences.size();
			for (int i= 0; i < size; ++i)
			{
				const GLC_StructOccurence* pOccurence= occurences.at(i);
				if (pOccurence->isOrphan())
				{
					QStringList stringList(m_FileName);
					stringList.append("Orphan occurence : " + pOccurence->name());
					GLC_ErrorLog::addError(stringList);
					delete pOccurence;
				}
			}
		}
		++iInstance;
	}

	m_InstanceOfExtRefHash.clear();


	//qDebug() << "Unfolded tree created";

}

// Load a Reference3D
void GLC_3dxmlToWorld::loadReference3D()
{
	const unsigned int id= readAttribute("id", true).toUInt();
	const QString refName(readAttribute("name", true));
	GLC_StructReference* pStructReference;

	if (id == 1) // This is the root reference.
	{
		m_pWorld->setRootName(refName);
		pStructReference= m_pWorld->rootOccurence()->structInstance()->structReference();
		pStructReference->setName(refName);
	}
	else
	{
		pStructReference= new GLC_StructReference(refName);
	}
	// Try to find extension
	GLC_Attributes userAttributes;
	while (endElementNotReached(m_pStreamReader, "Reference3D"))
	{
		if (m_pStreamReader->isStartElement() && (m_pStreamReader->name() == "Reference3DExtensionType"))
		{
			while (endElementNotReached(m_pStreamReader, "Reference3DExtensionType"))
			{
				if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "Attribute"))
				{
					QString name= readAttribute("name", true);
					QString value= readAttribute("value", true);
					if (name == "FILEPATH" && QDir(value).isRelative())
					{
						value= QFileInfo(m_FileName).absolutePath() + QDir::separator() + value;
					}
					userAttributes.insert(name, value);
				}
				readNext();;
			}
		}
		readNext();;
	}
	if (!userAttributes.isEmpty())
	{
		pStructReference->setAttributes(userAttributes);
	}

	m_ReferenceHash.insert(id, pStructReference);
}

// Load a Instance3D
void GLC_3dxmlToWorld::loadInstance3D()
{
	const QString local= "urn:3DXML:Reference:loc:";
	const QString externRef= "urn:3DXML:Reference:ext:";

	const unsigned int instanceId= readAttribute("id", true).toUInt();
	const QString instName(readAttribute("name", false));
	const unsigned int aggregatedById= getContent(m_pStreamReader, "IsAggregatedBy").toUInt();
	QString instanceOf= getContent(m_pStreamReader, "IsInstanceOf");
	const QString matrixString= getContent(m_pStreamReader, "RelativeMatrix");
	GLC_Matrix4x4 instanceMatrix= loadMatrix(matrixString);


	GLC_StructInstance* pStructInstance= new GLC_StructInstance(instName);
	pStructInstance->move(instanceMatrix);

	// Try to find extension
	GLC_Attributes userAttributes;
	while (endElementNotReached(m_pStreamReader, "Instance3D"))
	{
		if (m_pStreamReader->isStartElement() && (m_pStreamReader->name() == "Instance3DExtensionType"))
		{
			while (endElementNotReached(m_pStreamReader, "Instance3DExtensionType"))
			{
				if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "Attribute"))
				{
					QString name= readAttribute("name", true);
					QString value= readAttribute("value", true);
					userAttributes.insert(name, value);
				}
				readNext();;
			}
		}
		readNext();;
	}
	if (!userAttributes.isEmpty())
	{
		pStructInstance->setAttributes(userAttributes);
	}
	if (instanceOf.contains(externRef))
	{

		const QString extRefId= instanceOf.remove(externRef).remove("#1");
		m_SetOfExtRef << extRefId;
		m_InstanceOfExtRefHash.insert(pStructInstance, extRefId);
	}
	else if (instanceOf.contains(local))
	{
		const unsigned int refId= instanceOf.remove(local).toUInt();
		m_InstanceOf.insert(pStructInstance, refId);
	}
	else
	{
		// 3dvia 3dxml
		const unsigned int refId= instanceOf.toUInt();
		m_InstanceOf.insert(pStructInstance, refId);
	}

	AssyLink assyLink;
	assyLink.m_ParentRefId= aggregatedById;
	assyLink.m_pChildInstance= pStructInstance;
	assyLink.m_InstanceId= instanceId;
	m_AssyLinkList.append(assyLink);
}

// Load a Reference representation
void GLC_3dxmlToWorld::loadReferenceRep()
{
	const QString local= "urn:3DXML:Representation:loc:";
	const QString externName= "urn:3DXML:";

	const unsigned int id= readAttribute("id", true).toUInt();
	//const QString refName(readAttribute("name", true));
	const QString format= readAttribute("format", false);
	QString associatedFile(readAttribute("associatedFile", true));
	if (format == "TESSELLATED")
	{
		if (associatedFile.contains(local))
		{
			const QString repId= associatedFile.remove(local);
			m_ReferenceRepHash.insert(id, repId);
		}
		else if (associatedFile.contains(externName))
		{
			const QString repId= associatedFile.remove(externName);
			m_ReferenceRepHash.insert(id, repId);
		}
	}
	else if (format == "UVR")
	{
		QString message(QString("GLC_3dxmlToWorld::loadReferenceRep in file ") + m_CurrentFileName + " format " + format + " not supported");
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::FileNotSupported);
		clear();
		throw(fileFormatException);

	}
}

// Load a Instance representation
void GLC_3dxmlToWorld::loadInstanceRep()
{
	const QString local= "urn:3DXML:Reference:loc:";

	//const QString instName(readAttribute("name", true));
	const unsigned int aggregatedById= getContent(m_pStreamReader, "IsAggregatedBy").toUInt();
	QString instanceOf= getContent(m_pStreamReader, "IsInstanceOf");

	if (instanceOf.contains(local))
	{
		// The 3dxml is a 3dxml rep from CATIA V5
		const unsigned int refId= instanceOf.remove(local).toUInt();

		RepLink repLink;
		repLink.m_ReferenceId= aggregatedById;
		repLink.m_RepId= refId;

		m_LocalRepLinkList.append(repLink);
	}
	else
	{
		// The 3dxml is a 3dvia 3dxml
		const unsigned int refId= instanceOf.toUInt();
		RepLink repLink;
		repLink.m_ReferenceId= aggregatedById;
		repLink.m_RepId= refId;

		m_ExternRepLinkList.append(repLink);
	}
}

// Load External Ref
void GLC_3dxmlToWorld::loadExternalRef3D()
{
	if (m_SetOfExtRef.isEmpty()) return;

	const int size= m_SetOfExtRef.size();
	int previousQuantumValue= 0;
	int currentQuantumValue= 0;
	int currentFileIndex= 0;
	emit currentQuantum(currentQuantumValue);

	SetOfExtRef::iterator iExtRef= m_SetOfExtRef.begin();
	while (iExtRef != m_SetOfExtRef.constEnd())
	{

		m_CurrentFileName= (*iExtRef);

		if (! m_IsInArchive)
		{
			// Get the representation time stamp
			m_CurrentDateTime= QFileInfo(QFileInfo(m_FileName).absolutePath() + QDir::separator() + QFileInfo(m_CurrentFileName).fileName()).lastModified();
		}

		if (!m_LoadStructureOnly && GLC_State::cacheIsUsed() && GLC_State::currentCacheManager().isUsable(m_CurrentDateTime, QFileInfo(m_FileName).baseName(), m_CurrentFileName))
		{
			GLC_CacheManager cacheManager= GLC_State::currentCacheManager();

			GLC_BSRep binaryRep= cacheManager.binary3DRep(QFileInfo(m_FileName).baseName(), m_CurrentFileName);
			GLC_3DRep* pRep= new GLC_3DRep(binaryRep.loadRep());

			setRepresentationFileName(pRep);
			factorizeMaterial(pRep);

			GLC_StructReference* pCurrentRef= new GLC_StructReference(pRep);
			pCurrentRef->setName(QFileInfo(m_CurrentFileName).baseName());
			m_ExternalReferenceHash.insert(m_CurrentFileName, pCurrentRef);

		}
		else if (!m_LoadStructureOnly && setStreamReaderToFile(m_CurrentFileName))
		{

			// Avoid recursive call off createReferenceRep
			const QString localFileName= m_CurrentFileName;

			GLC_StructReference* pCurrentRef= createReferenceRep(QString(), NULL);
			if (NULL != pCurrentRef)
			{
				m_ExternalReferenceHash.insert(localFileName, pCurrentRef);
			}
			else
			{
				QStringList stringList(m_FileName);
				stringList.append("GLC_3dxmlToWorld::loadExternalRef3D");
				stringList.append("Failed to load " + m_CurrentFileName);
				GLC_ErrorLog::addError(stringList);
			}
		}
		else if(m_LoadStructureOnly)
		{
			GLC_3DRep* pRep= new GLC_3DRep();
			if (m_IsInArchive)
			{
				pRep->setFileName(glc::builtArchiveString(m_FileName, m_CurrentFileName));
			}
			else
			{
				const QString repFileName= glc::builtFileString(m_FileName, m_CurrentFileName);
				pRep->setFileName(repFileName);
				m_SetOfAttachedFileName << glc::archiveEntryFileName(repFileName);
			}

			if (m_GetExternalRef3DName && setStreamReaderToFile(m_CurrentFileName))
			{
				const QString localFileName= m_CurrentFileName;
				GLC_StructReference* pCurrentRef= createReferenceRep(QString(), pRep);
				m_ExternalReferenceHash.insert(localFileName, pCurrentRef);
			}
			else
			{
				GLC_StructReference* pCurrentRef= new GLC_StructReference(pRep);
				pCurrentRef->setName(QFileInfo(m_CurrentFileName).baseName());
				m_ExternalReferenceHash.insert(m_CurrentFileName, pCurrentRef);
			}
		}
		else
		{
			qDebug() << "GLC_3dxmlToWorld::loadExternalRef3D No File Found";
		}

		++currentFileIndex;
		// Progrees bar indicator
		currentQuantumValue = static_cast<int>((static_cast<double>(currentFileIndex) / size) * 100);
		if (currentQuantumValue > previousQuantumValue)
		{
			emit currentQuantum(currentQuantumValue);
		}
		previousQuantumValue= currentQuantumValue;

		++iExtRef;
	}
	m_SetOfExtRef.clear();

}

// Create Instance from 3DXML Rep
GLC_StructReference* GLC_3dxmlToWorld::createReferenceRep(QString repId, GLC_3DRep* pRep)
{
	//qDebug() << "GLC_3dxmlToWorld::createReferenceRep :" << repId;

	QString refName;

	if (repId.isEmpty())
	{
		goToElement(m_pStreamReader, "ProductStructure");
		checkForXmlError("Element ProductStructure not found");

		goToElement(m_pStreamReader, "Reference3D");
		checkForXmlError("Element Reference3D not found");
		refName= readAttribute("name", true);

		if (pRep != NULL)
		{
			pRep->setName(refName);
			return new GLC_StructReference(pRep);
		}

		goToElement(m_pStreamReader, "ReferenceRep");
		if (m_pStreamReader->atEnd())
		{
			QStringList stringList(m_FileName);
			stringList.append("Element ReferenceRep not found in  file " + m_CurrentFileName);
			GLC_ErrorLog::addError(stringList);
			return NULL;
		}
		checkForXmlError("Element ReferenceRep contains error");

		const QString format(readAttribute("format", true));
		if (format != "TESSELLATED")
		{
			QString message(QString("GLC_3dxmlToWorld::addExtenalRef 3D rep format ") + format + " Not Supported");
			GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::FileNotSupported);
			clear();
			throw(fileFormatException);
		}

		repId= readAttribute("associatedFile");

		const QString local= "urn:3DXML:Representation:loc:";
		const QString ext= "urn:3DXML:Representation:ext:";
		if (repId.contains(ext))
		{
			repId.remove(ext);
			repId.resize(repId.size() - 2);
			if (setStreamReaderToFile(repId))
			{
				return createReferenceRep(QString(), NULL);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			repId.remove(local);
		}

		checkForXmlError("attribute associatedFile not found");
		goToRepId(repId);
		checkForXmlError("repId not found");
	}

	GLC_Mesh* pMesh= new GLC_Mesh();
	pMesh->setName(refName);
	GLC_3DRep currentMesh3DRep(pMesh);
	// Add time Stamp and file name to the 3D rep

	if (repId.contains("3DXML_Local_"))
	{
		QString saveCurrentFileName= m_CurrentFileName;
		m_CurrentFileName= repId;
		setRepresentationFileName(&currentMesh3DRep);
		m_CurrentFileName= saveCurrentFileName;
	}
	else
	{
		setRepresentationFileName(&currentMesh3DRep);
	}

	currentMesh3DRep.setLastModified(m_CurrentDateTime);

	int numberOfMesh= 1;
	while (endElementNotReached(m_pStreamReader, "Representation"))
	{
		gotToPolygonalRepType();
		if (!endElementNotReached(m_pStreamReader, "Representation") || m_pStreamReader->atEnd() || m_pStreamReader->hasError())
		{
			if (m_pStreamReader->hasError() || m_pStreamReader->atEnd())
			{
				QString message(QString("An element have not been found in file ") + m_FileName);
				GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
				clear();
				throw(fileFormatException);
			}
			else
			{
				pMesh->finish();
				currentMesh3DRep.clean();
				if (!currentMesh3DRep.isEmpty())
				{
					if (GLC_State::cacheIsUsed())
					{
						GLC_CacheManager currentManager= GLC_State::currentCacheManager();
						if (!currentManager.addToCache(QFileInfo(m_FileName).baseName(), currentMesh3DRep))
						{
							QStringList stringList("GLC_3dxmlToWorld::createReferenceRep");
							stringList.append(m_FileName);
							stringList.append("File " + currentMesh3DRep.fileName() + " Not Added to cache");
							GLC_ErrorLog::addError(stringList);
						}
					}

					return new GLC_StructReference(new GLC_3DRep(currentMesh3DRep));
				}
				else
				{
					return new GLC_StructReference("Empty Rep");
				}
			}
		}
		if (numberOfMesh > 1)
		{
			pMesh->finish();
			pMesh = new GLC_Mesh();
			pMesh->setName(refName);
			currentMesh3DRep.addGeom(pMesh);
		}

		// Get the master lod accuracy
		double masterLodAccuracy= readAttribute("accuracy", false).toDouble();

		loadLOD(pMesh);
		if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
		{
			QStringList stringList(m_FileName);
			stringList.append(m_CurrentFileName);
			stringList.append("Master LOD not found");
			GLC_ErrorLog::addError(stringList);
			return new GLC_StructReference("Empty Rep");
		}

		// Load Faces index data
		while (endElementNotReached(m_pStreamReader, "Faces"))
		{
			readNext();;
			if ( m_pStreamReader->name() == "Face")
			{
				loadFace(pMesh, 0, masterLodAccuracy);
			}
		}
		checkForXmlError("End of Faces not found");

		while (startElementNotReached(m_pStreamReader, "Edges") && startElementNotReached(m_pStreamReader, "VertexBuffer"))
		{
			readNext();;
		}

		checkForXmlError("Element VertexBuffer not found");
		if (m_pStreamReader->name() == "Edges")
		{
			while (endElementNotReached(m_pStreamReader, "Edges"))
			{
				readNext();;
				if ( m_pStreamReader->name() == "Polyline")
				{
					loadPolyline(pMesh);
					readNext();;
				}
			}
		}


		{
			QString verticePosition= getContent(m_pStreamReader, "Positions").replace(',', ' ');
			//qDebug() << "Position " << verticePosition;
			checkForXmlError("Error while retrieving Position ContentVertexBuffer");
			// Load Vertice position
			QTextStream verticeStream(&verticePosition);
			QList<GLfloat> verticeValues;
			QString buff;
			while ((!verticeStream.atEnd()))
			{
				verticeStream >> buff;
				verticeValues.append(buff.toFloat());
			}
			pMesh->addVertice(verticeValues.toVector());

		}

		{
			QString normals= getContent(m_pStreamReader, "Normals").replace(',', ' ');
			//qDebug() << "Normals " << normals;
			checkForXmlError("Error while retrieving Normals values");
			// Load Vertice Normals
			QTextStream normalsStream(&normals);
			QList<GLfloat> normalValues;
			QString buff;
			while ((!normalsStream.atEnd()))
			{
				normalsStream >> buff;
				normalValues.append(buff.toFloat());
			}
			pMesh->addNormals(normalValues.toVector());
		}

		// Try to find texture coordinate
		while (endElementNotReached(m_pStreamReader, "VertexBuffer"))
		{
			//qDebug() << "Try to find texture coordinate " << m_pStreamReader->name() << " " << m_pStreamReader->lineNumber();
			if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "TextureCoordinates"))
			{
				QString texels= getContent(m_pStreamReader, "TextureCoordinates").replace(',', ' ');
				checkForXmlError("Error while retrieving Texture coordinates");
				QTextStream texelStream(&texels);
				QList<GLfloat> texelValues;
				QString buff;
				while ((!texelStream.atEnd()))
				{
					texelStream >> buff;
					texelValues.append(buff.toFloat());
				}
				pMesh->addTexels(texelValues.toVector());
			}
			readNext();;
		}

		++numberOfMesh;
	}

	pMesh->finish();

	currentMesh3DRep.clean();
	if (!currentMesh3DRep.isEmpty())
	{
		if (GLC_State::cacheIsUsed())
		{
			GLC_CacheManager currentManager= GLC_State::currentCacheManager();
			currentManager.addToCache(QFileInfo(m_FileName).baseName(), currentMesh3DRep);
		}

		return new GLC_StructReference(new GLC_3DRep(currentMesh3DRep));
	}
	else
	{
		return new GLC_StructReference("Empty Rep");
	}
}

// Load Matrix
GLC_Matrix4x4 GLC_3dxmlToWorld::loadMatrix(const QString& stringMatrix)
{

	QStringList stringValues(stringMatrix.split(" "));

	if (stringValues.size() != 12) return GLC_Matrix4x4();

	QVector<double> values(16);
	// Rotation
	values[0]= stringValues[0].toDouble();
	values[1]= stringValues[1].toDouble();
	values[2]= stringValues[2].toDouble();
	values[3]= 0.0;
	values[4]= stringValues[3].toDouble();
	values[5]= stringValues[4].toDouble();
	values[6]= stringValues[5].toDouble();
	values[7]= 0.0;
	values[8]= stringValues[6].toDouble();
	values[9]= stringValues[7].toDouble();
	values[10]= stringValues[8].toDouble();
	values[11]= 0.0;
	// Translation
	values[12]= stringValues[9].toDouble();
	values[13]= stringValues[10].toDouble();
	values[14]= stringValues[11].toDouble();
	values[15]= 1.0;

	GLC_Matrix4x4 resultMatrix(values.data());
	resultMatrix.optimise();

	return resultMatrix;
}

// Create the unfolded  tree
void GLC_3dxmlToWorld::createUnfoldedTree()
{
	//qDebug() << "createUnfoldedTree";
	// Run throw all link in the list of link

	qSort(m_AssyLinkList.begin(), m_AssyLinkList.end());
	AssyLinkList::iterator iLink= m_AssyLinkList.begin();
	while(iLink != m_AssyLinkList.constEnd())
	{
		GLC_StructInstance* pChildInstance= (*iLink).m_pChildInstance;
		if (pChildInstance->structReference() == NULL)
		{
			QStringList stringList(m_FileName);
			stringList.append("Instance without reference");
			GLC_ErrorLog::addError(stringList);
			pChildInstance->setReference(new GLC_StructReference("Part"));
		}
		Q_ASSERT(m_ReferenceHash.contains((*iLink).m_ParentRefId));
		GLC_StructReference* pRef= m_ReferenceHash.value((*iLink).m_ParentRefId);
		// Attach pInstance at all Occurence of pRef
		QList<GLC_StructInstance*> instanceList= pRef->listOfStructInstances();
		const int instanceNumber= instanceList.size();
		for (int i= 0; i < instanceNumber; ++i)
		{
			if (instanceList.at(i)->hasStructOccurence())
			{
				QList<GLC_StructOccurence*> occurenceList= instanceList.at(i)->listOfStructOccurences();
				const int occurenceNumber= occurenceList.size();
				for (int j= 0; j < occurenceNumber; ++j)
				{
					if (pChildInstance->hasStructOccurence() && pChildInstance->firstOccurenceHandle()->isOrphan())
					{
						Q_ASSERT(pChildInstance->listOfStructOccurences().size() == 1);
						occurenceList.at(j)->addChild(pChildInstance->firstOccurenceHandle());
					}
					else
					{
						occurenceList.at(j)->addChild(pChildInstance);
					}
				}
			}
			else
			{
				GLC_StructOccurence* pOccurence= new GLC_StructOccurence(instanceList.at(i), m_pWorld->worldHandle());
				if (pChildInstance->hasStructOccurence() && pChildInstance->firstOccurenceHandle()->isOrphan())
				{
					Q_ASSERT(pChildInstance->listOfStructOccurences().size() == 1);
					pOccurence->addChild(pChildInstance->firstOccurenceHandle());
				}
				else
				{
					pOccurence->addChild(pChildInstance);
				}
			}
		}

		++iLink;
	}

	m_AssyLinkList.clear();

	// Check the assembly structure occurence
	ReferenceHash::const_iterator iRef= m_ReferenceHash.constBegin();
	while (m_ReferenceHash.constEnd() != iRef)
	{
		if (iRef.key() != 1)
		{
			GLC_StructReference* pReference= iRef.value();
			if (!pReference->hasStructInstance())
			{
				QStringList stringList(m_FileName);
				stringList.append("GLC_3dxmlToWorld::createUnfoldedTree() : Orphan reference: " + pReference->name());
				GLC_ErrorLog::addError(stringList);
				delete pReference;
			}
			else
			{
				QList<GLC_StructInstance*> instances= pReference->listOfStructInstances();
				const int size= instances.size();
				for (int i= 0; i < size; ++i)
				{
					GLC_StructInstance* pInstance= instances.at(i);
					if (!pInstance->hasStructOccurence())
					{
						QStringList stringList(m_FileName);
						stringList.append("GLC_3dxmlToWorld::createUnfoldedTree() : Orphan Instance: " + pInstance->name());
						GLC_ErrorLog::addError(stringList);
						delete pInstance;
					}
					else
					{
						QList<GLC_StructOccurence*> occurences= pInstance->listOfStructOccurences();
						const int occurencesSize= occurences.size();
						for (int j= 0; j < occurencesSize; ++j)
						{
							GLC_StructOccurence* pOcc= occurences.at(j);
							if (pOcc->isOrphan())
							{
								QStringList stringList(m_FileName);
								stringList.append("GLC_3dxmlToWorld::createUnfoldedTree(): Orphan occurence: " + pOcc->name());
								GLC_ErrorLog::addError(stringList);
								delete pOcc;
							}
						}
					}
				}
			}
		}
		++iRef;
	}
	m_ReferenceHash.clear();

	// Update position
	m_pWorld->rootOccurence()->updateChildrenAbsoluteMatrix();

}
// Check for XML error
void GLC_3dxmlToWorld::checkForXmlError(const QString& info)
{
	if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
	{
		QString message(QString("An element have not been found in file ") + m_CurrentFileName);

		QStringList stringList(info + " " + m_pStreamReader->errorString());
		stringList.append(message);
		GLC_ErrorLog::addError(stringList);

		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}
}
// Go to the master LOD
void GLC_3dxmlToWorld::loadLOD(GLC_Mesh* pMesh)
{
	int lodIndex= 1;
	while(!m_pStreamReader->atEnd() && !((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "Faces")))
	{
		readNext();;
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "SurfaceAttributes"))
		{
			m_pCurrentMaterial= loadSurfaceAttributes();
		}
		else if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "PolygonalLOD"))
		{
			double accuracy= readAttribute("accuracy", true).toDouble();
			// Load Faces index data
			while (endElementNotReached(m_pStreamReader, "Faces"))
			{
				readNext();;
				if ( m_pStreamReader->name() == "Face")
				{
					loadFace(pMesh, lodIndex, accuracy);
				}
			}
			checkForXmlError("End of Faces not found");
			++lodIndex;
		}
	}
}
// Load a face
void GLC_3dxmlToWorld::loadFace(GLC_Mesh* pMesh, const int lod, double accuracy)
{
	//qDebug() << "GLC_3dxmlToWorld::loadFace" << m_pStreamReader->name();
	// List of index declaration
	QString triangles= readAttribute("triangles", false).trimmed();
	QString strips= readAttribute("strips", false).trimmed();
	QString fans= readAttribute("fans", false).trimmed();

	if (triangles.isEmpty() && strips.isEmpty() && fans.isEmpty())
	{
		QStringList stringList(m_CurrentFileName);
		stringList.append("GLC_3dxmlToWorld::loadFace : Empty face found");
		GLC_ErrorLog::addError(stringList);
		return;
	}

	GLC_Material* pCurrentMaterial= NULL;

	while(endElementNotReached(m_pStreamReader, "Face"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "SurfaceAttributes"))
		{
			pCurrentMaterial= loadSurfaceAttributes();
		}
		readNext();;
	}
	if (NULL == pCurrentMaterial)
	{
		pCurrentMaterial= m_pCurrentMaterial;
	}

	// Trying to find triangles
	if (!triangles.isEmpty())
	{
		// For 3dvia mesh
		if (triangles.contains(','))
		{
			triangles.remove(',');
		}
		QTextStream trianglesStream(&triangles);
		IndexList trianglesIndex;
		QString buff;
		while ((!trianglesStream.atEnd()))
		{
			trianglesStream >> buff;
			trianglesIndex.append(buff.toUInt());
		}
		pMesh->addTriangles(pCurrentMaterial, trianglesIndex, lod, accuracy);
	}
	// Trying to find trips
	if (!strips.isEmpty())
	{

		QStringList stripsList(strips.split(","));
		const int size= stripsList.size();
		for (int i= 0; i < size; ++i)
		{
			//qDebug() << "Strip " << stripsList.at(i);
			QTextStream stripsStream(&stripsList[i]);
			IndexList stripsIndex;
			QString buff;
			while ((!stripsStream.atEnd()))
			{
				stripsStream >> buff;
				stripsIndex.append(buff.toUInt());
			}
			pMesh->addTrianglesStrip(pCurrentMaterial, stripsIndex, lod, accuracy);
		}
	}
	// Trying to find fans
	if (!fans.isEmpty())
	{
		QStringList fansList(fans.split(","));
		const int size= fansList.size();
		for (int i= 0; i < size; ++i)
		{
			QTextStream fansStream(&fansList[i]);
			IndexList fansIndex;
			QString buff;
			while ((!fansStream.atEnd()))
			{
				fansStream >> buff;
				fansIndex.append(buff.toUInt());
			}
			pMesh->addTrianglesFan(pCurrentMaterial, fansIndex, lod, accuracy);
		}
	}

}

// Load polyline
void GLC_3dxmlToWorld::loadPolyline(GLC_Mesh* pMesh)
{
	QString data= readAttribute("vertices", true);

	data.replace(',', ' ');
	QTextStream dataStream(&data);
	GLfloatVector dataVector;
	QString buff;
	while ((!dataStream.atEnd()))
	{
		dataStream >> buff;
		dataVector.append(buff.toFloat());
	}
	pMesh->addVerticeGroup(dataVector);
}

// Clear material hash
void GLC_3dxmlToWorld::clearMaterialHash()
{
	MaterialHash::iterator iMaterial= m_MaterialHash.begin();
	while (m_MaterialHash.constEnd() != iMaterial)
	{
		if (iMaterial.value()->isUnused())
		{
			delete iMaterial.value();
		}
		++iMaterial;
	}

	m_MaterialHash.clear();
}

GLC_Material* GLC_3dxmlToWorld::loadSurfaceAttributes()
{
	GLC_Material* pMaterial= NULL;
	while(endElementNotReached(m_pStreamReader, "SurfaceAttributes"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "Color"))
		{
			pMaterial= getMaterial();
		}
		else if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "MaterialApplication"))
		{
			while (endElementNotReached(m_pStreamReader, "MaterialApplication"))
			{
				readNext();;
				if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "MaterialId"))
				{
					checkForXmlError("Material ID not found");
					QString materialId= readAttribute("id", true).remove("urn:3DXML:CATMaterialRef.3dxml#");
					pMaterial= m_MaterialHash.value(materialId);
				}
			}

		}
		readNext();;
	}

	return pMaterial;
}

GLC_Material* GLC_3dxmlToWorld::getMaterial()
{
	GLC_Material* pMaterial= NULL;
	const QString red(readAttribute("red", true));
	const QString green(readAttribute("green", true));
	const QString blue(readAttribute("blue", true));
	const QString alpha(readAttribute("alpha", true));

	qreal redReal= red.toDouble();
	qreal greenReal= green.toDouble();
	qreal blueReal= blue.toDouble();
	qreal alphaReal= alpha.toDouble();
	QColor diffuse;
	diffuse.setRgbF(redReal, greenReal, blueReal);
	pMaterial= new GLC_Material(diffuse);
	pMaterial->setName("Material_" + QString::number(m_MaterialHash.size()));
	pMaterial->setAmbientColor(QColor(50, 50, 50));
	pMaterial->setSpecularColor(QColor(70, 70, 70));
	pMaterial->setShininess(35.0);
	pMaterial->setOpacity(alphaReal);

	const QString matKey= QString::number(pMaterial->hashCode());
	if (m_MaterialHash.contains(matKey))
	{
		delete pMaterial;
		pMaterial= m_MaterialHash.value(matKey);
	}
	else
	{
		m_MaterialHash.insert(matKey, pMaterial);
	}

	return pMaterial;
}

// Set the stream reader to the specified file
bool GLC_3dxmlToWorld::setStreamReaderToFile(QString fileName, bool test)
{
	m_CurrentFileName= fileName;
	if (m_IsInArchive)
	{
		QMutexLocker locker(&m_ZipMutex);
		m_ByteArrayList.clear();
		// Create QuaZip File
		QuaZipFile* p3dxmlFile= new QuaZipFile(m_p3dxmlArchive);

		// Get the file of the 3dxml
		if (!m_p3dxmlArchive->setCurrentFile(fileName, QuaZip::csInsensitive))
		{
			if (!test)
			{
				QString message(QString("GLC_3dxmlToWorld::setStreamReaderToFile File ") + m_FileName + " doesn't contains " + fileName);
				GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
				clear();
				throw(fileFormatException);
			}
			else return false;
		}

		// Open the file of the 3dxml
		if(!p3dxmlFile->open(QIODevice::ReadOnly))
	    {
			QString message(QString("GLC_3dxmlToWorld::setStreamReaderToFile Unable to Open ") + fileName);
			GLC_FileFormatException fileFormatException(message, fileName, GLC_FileFormatException::FileNotSupported);
			clear();
			throw(fileFormatException);
	    }

		// Test if the file is a binary
		checkFileValidity(p3dxmlFile);
		{
			delete p3dxmlFile;
			p3dxmlFile= new QuaZipFile(m_p3dxmlArchive);
			m_p3dxmlArchive->setCurrentFile(fileName, QuaZip::csInsensitive);
			p3dxmlFile->open(QIODevice::ReadOnly);
		}

		// Set the stream reader
		delete m_pStreamReader;

		QByteArray currentByteArray;
		while (!currentByteArray.isEmpty() || m_ByteArrayList.isEmpty())
		{
			currentByteArray= p3dxmlFile->read(chunckSize);
			m_ByteArrayList.append(currentByteArray);
		}
		m_pStreamReader= new QXmlStreamReader(m_ByteArrayList.takeFirst());
		delete p3dxmlFile;
	}
	else
	{
		delete m_pCurrentFile;
		// Create the file to load
		if (fileName != m_FileName && !m_FileName.isEmpty())
		{
			fileName= QFileInfo(m_FileName).absolutePath() + QDir::separator() + fileName;
		}
		// Get the 3DXML time stamp
		m_CurrentDateTime= QFileInfo(fileName).lastModified();

		m_pCurrentFile= new QFile(fileName);
		if (!m_pCurrentFile->open(QIODevice::ReadOnly))
		{
			QString message(QString("GLC_3dxmlToWorld::setStreamReaderToFile File ") + fileName + QString(" not found"));
			QStringList stringList(message);
			GLC_ErrorLog::addError(stringList);
			return false;
		}
		else if (m_FileName != fileName)
		{
			m_SetOfAttachedFileName << fileName;
		}

		// Test if the file is a binary
		checkFileValidity(m_pCurrentFile);

		// Set the stream reader
		delete m_pStreamReader;
		m_pStreamReader= new QXmlStreamReader(m_pCurrentFile);
	}
	return true;
}

// Load the local representation
void GLC_3dxmlToWorld::loadLocalRepresentations()
{
	qDebug() << "GLC_3dxmlToWorld::loadLocalRepresentations()";

	if (m_LocalRepLinkList.isEmpty()) return;
	QHash<const QString, GLC_3DRep> repHash;

	// Load all local ref
	goToElement(m_pStreamReader, "GeometricRepresentationSet");
	while (endElementNotReached(m_pStreamReader, "GeometricRepresentationSet"))
	{
		if (m_pStreamReader->name() == "Representation")
		{
			QString id= readAttribute("id", true);
			GLC_StructReference* pRef= createReferenceRep("3DXML_Local_" + id, NULL);
			if (pRef->hasRepresentation())
			{
				GLC_3DRep representation(*(dynamic_cast<GLC_3DRep*>(pRef->representationHandle())));
				repHash.insert(id, representation);
			}
			delete pRef;
		}
		readNext();;
	}
	//qDebug() << "Local rep loaded";

	// Attach the ref to the structure reference
	RepLinkList::iterator iLocalRep= m_LocalRepLinkList.begin();
	while (iLocalRep != m_LocalRepLinkList.constEnd())
	{
		unsigned int referenceId= (*iLocalRep).m_ReferenceId;
		unsigned int refId= (*iLocalRep).m_RepId;

		GLC_StructReference* pReference= m_ReferenceHash.value(referenceId);
		const QString representationID= m_ReferenceRepHash.value(refId);
		pReference->setRepresentation(repHash.value(representationID));
		pReference->setRepresentationName(pReference->name());

		++iLocalRep;
	}
}

void GLC_3dxmlToWorld::loadGraphicsProperties()
{
	if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadGraphicsProperties Element DefaultView Not found in ") + m_FileName);
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

	// Load the graphics properties
	while(endElementNotReached(m_pStreamReader, "DefaultView"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "DefaultViewProperty"))
		{
			loadDefaultViewProperty();
		}

		readNext();;
	}

	// Check if an error Occur
	if (m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadGraphicsProperties An error occur in ") + m_FileName);
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

}

void GLC_3dxmlToWorld::loadDefaultViewProperty()
{
	goToElement(m_pStreamReader, "OccurenceId");
	unsigned int occurenceId= getContent(m_pStreamReader, "OccurenceId").toUInt();

	// Load the graphics properties
	while(endElementNotReached(m_pStreamReader, "DefaultViewProperty"))
	{
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "GraphicProperties"))
		{
			while(endElementNotReached(m_pStreamReader, "GraphicProperties"))
			{
				if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "GeneralAttributes"))
				{
					QString visibleString= readAttribute("visible", true);
					if (visibleString != "true")
					{
						if (!m_OccurenceAttrib.contains(occurenceId))
						{
							OccurenceAttrib* pOccurenceAttrib= new OccurenceAttrib();
							pOccurenceAttrib->m_IsVisible= false;
							m_OccurenceAttrib.insert(occurenceId, pOccurenceAttrib);
						}
						else m_OccurenceAttrib.value(occurenceId)->m_IsVisible= false;
					}
				}
				else if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "SurfaceAttributes"))
				{
					goToElement(m_pStreamReader, "Color");
					const double red= readAttribute("red", true).toDouble();
					const double green= readAttribute("green", true).toDouble();
					const double blue= readAttribute("blue", true).toDouble();
					double alpha= 1.0;
					QString alphaString= readAttribute("alpha", false);
					if (!alphaString.isEmpty()) alpha= alphaString.toDouble();

					GLC_RenderProperties* pRenderProperties= new GLC_RenderProperties();
					if (red != -1.0f)
					{
						QColor diffuseColor;
						diffuseColor.setRgbF(red, green, blue, alpha);
						GLC_Material* pMaterial= new GLC_Material();
						pMaterial->setDiffuseColor(diffuseColor);
						pRenderProperties->setOverwriteMaterial(pMaterial);
						pRenderProperties->setRenderingMode(glc::OverwriteMaterial);
					}
					else if (alpha < 1.0f)
					{
						pRenderProperties->setOverwriteTransparency(static_cast<float>(alpha));
						pRenderProperties->setRenderingMode(glc::OverwriteTransparency);
					}
					if (!m_OccurenceAttrib.contains(occurenceId))
					{
						OccurenceAttrib* pOccurenceAttrib= new OccurenceAttrib();
						pOccurenceAttrib->m_pRenderProperties= pRenderProperties;
						m_OccurenceAttrib.insert(occurenceId, pOccurenceAttrib);
					}
					else m_OccurenceAttrib.value(occurenceId)->m_pRenderProperties= pRenderProperties;
				}

				readNext();;
			}

		}

		readNext();;
	}

	// Check if an error Occur
	if (m_pStreamReader->hasError())
	{
		QString message(QString("GLC_3dxmlToWorld::loadDefaultViewProperty An error occur in ") + m_FileName);
		GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
		clear();
		throw(fileFormatException);
	}

}
// Load the extern representation
void GLC_3dxmlToWorld::loadExternRepresentations()
{

	if (m_ExternRepLinkList.isEmpty()) return;

	QHash<const unsigned int, GLC_3DRep> repHash;

	// Progress bar variables
	const int size= m_ReferenceRepHash.size();
	int previousQuantumValue= 0;
	int currentQuantumValue= 0;
	int currentFileIndex= 0;
	emit currentQuantum(currentQuantumValue);

	// Load all external rep
	ReferenceRepHash::iterator iRefRep= m_ReferenceRepHash.begin();
	while (iRefRep != m_ReferenceRepHash.constEnd())
	{
		m_CurrentFileName= iRefRep.value();
		const unsigned int id= iRefRep.key();

		if (!m_IsInArchive)
		{
			// Get the 3DXML time stamp
			m_CurrentDateTime= QFileInfo(QFileInfo(m_FileName).absolutePath() + QDir::separator() + QFileInfo(m_CurrentFileName).fileName()).lastModified();
		}


		if (!m_LoadStructureOnly && setStreamReaderToFile(m_CurrentFileName))
		{
			GLC_3DRep representation;
			if (GLC_State::cacheIsUsed() && GLC_State::currentCacheManager().isUsable(m_CurrentDateTime, QFileInfo(m_FileName).baseName(), m_CurrentFileName))
			{
				GLC_CacheManager cacheManager= GLC_State::currentCacheManager();
				GLC_BSRep binaryRep= cacheManager.binary3DRep(QFileInfo(m_FileName).baseName(), m_CurrentFileName);
				representation= binaryRep.loadRep();
				setRepresentationFileName(&representation);
				factorizeMaterial(&representation);
			}
			else
			{
				representation= loadCurrentExtRep();
				representation.clean();
			}
			if (!representation.isEmpty())
			{
				repHash.insert(id, representation);
			}
		}
		else if (m_LoadStructureOnly)
		{
			GLC_3DRep representation;
			if (m_IsInArchive)
			{
				representation.setFileName(glc::builtArchiveString(m_FileName, m_CurrentFileName));
			}
			else
			{
				const QString repFileName= glc::builtFileString(m_FileName, m_CurrentFileName);
				representation.setFileName(repFileName);
				m_SetOfAttachedFileName << glc::archiveEntryFileName(repFileName);
			}

			repHash.insert(id, representation);
		}

		// Progrees bar indicator
		++currentFileIndex;
		currentQuantumValue = static_cast<int>((static_cast<double>(currentFileIndex) / size) * 100);
		if (currentQuantumValue > previousQuantumValue)
		{
			emit currentQuantum(currentQuantumValue);
		}
		previousQuantumValue= currentQuantumValue;

		++iRefRep;
	}

	// Attach the ref to the structure reference
	RepLinkList::iterator iExtRep= m_ExternRepLinkList.begin();
	while (iExtRep != m_ExternRepLinkList.constEnd())
	{
		unsigned int referenceId= (*iExtRep).m_ReferenceId;
		unsigned int refId= (*iExtRep).m_RepId;

		GLC_StructReference* pReference= m_ReferenceHash.value(referenceId);
		if (pReference->hasRepresentation())
		{
			GLC_3DRep* pRep= dynamic_cast<GLC_3DRep*>(pReference->representationHandle());
			if (NULL != pRep)
			{
				GLC_3DRep newRep(repHash.value(refId));
				pRep->take(&newRep);
			}
		}
		else
		{
			pReference->setRepresentation(repHash.value(refId));
		}
		// If representation hasn't a name. Set his name to reference name
		if (pReference->representationName().isEmpty())
		{
			pReference->setRepresentationName(pReference->name());
		}

		++iExtRep;
	}

}

// Return the instance of the current extern representation
GLC_3DRep GLC_3dxmlToWorld::loadCurrentExtRep()
{
	GLC_Mesh* pMesh= new GLC_Mesh();
	GLC_3DRep currentMeshRep(pMesh);
	currentMeshRep.setName(QString());
	// Set rep file name and time stamp
	setRepresentationFileName(&currentMeshRep);

	currentMeshRep.setLastModified(m_CurrentDateTime);

	int numberOfMesh= 1;
	while (!m_pStreamReader->atEnd())
	{
		m_pCurrentMaterial= NULL;
		gotToPolygonalRepType();
		if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
		{
			if (m_pStreamReader->hasError())
			{
				QString message(QString("An element have not been found in file ") + m_FileName);
				GLC_FileFormatException fileFormatException(message, m_FileName, GLC_FileFormatException::WrongFileFormat);
				clear();
				throw(fileFormatException);
			}
			else
			{
				pMesh->finish();
				currentMeshRep.clean();

				if (GLC_State::cacheIsUsed())
				{
					GLC_CacheManager currentManager= GLC_State::currentCacheManager();
					currentManager.addToCache(QFileInfo(m_FileName).baseName(), currentMeshRep);
				}

				return currentMeshRep;
			}
		}
		if (numberOfMesh > 1)
		{
			pMesh->finish();
			pMesh = new GLC_Mesh();
			currentMeshRep.addGeom(pMesh);
		}

		// Get the master lod accuracy
		double masteLodAccuracy= readAttribute("accuracy", false).toDouble();

		loadLOD(pMesh);
		if (m_pStreamReader->atEnd() || m_pStreamReader->hasError())
		{
			QStringList stringList(m_FileName);
			stringList.append(m_CurrentFileName);
			stringList.append("Master LOD not found");
			GLC_ErrorLog::addError(stringList);

			pMesh->finish();
			currentMeshRep.clean();

			if (GLC_State::cacheIsUsed())
			{
				GLC_CacheManager currentManager= GLC_State::currentCacheManager();
				currentManager.addToCache(QFileInfo(m_FileName).baseName(), currentMeshRep);
			}

			return currentMeshRep;
		}

		// Load Faces index data
		while (endElementNotReached(m_pStreamReader, "Faces"))
		{
			readNext();;
			if ( m_pStreamReader->name() == "Face")
			{
				loadFace(pMesh, 0, masteLodAccuracy);
			}
		}
		checkForXmlError("End of Faces not found");

		while (startElementNotReached(m_pStreamReader, "Edges") && startElementNotReached(m_pStreamReader, "VertexBuffer"))
		{
			readNext();;
		}

		checkForXmlError("Element VertexBuffer not found");
		if (m_pStreamReader->name() == "Edges")
		{
			while (endElementNotReached(m_pStreamReader, "Edges"))
			{
				readNext();;
				if ( m_pStreamReader->name() == "Polyline")
				{
					loadPolyline(pMesh);
					readNext();;
				}
			}
		}

		{
			QString verticePosition= getContent(m_pStreamReader, "Positions").replace(',', ' ');
			//qDebug() << "Position " << verticePosition;
			checkForXmlError("Error while retrieving Position ContentVertexBuffer");
			// Load Vertice position
			QTextStream verticeStream(&verticePosition);
			QList<GLfloat> verticeValues;
			QString buff;
			while ((!verticeStream.atEnd()))
			{
				verticeStream >> buff;
				verticeValues.append(buff.toFloat());
			}
			pMesh->addVertice(verticeValues.toVector());

		}

		{
			QString normals= getContent(m_pStreamReader, "Normals").replace(',', ' ');
			//qDebug() << "Normals " << normals;
			checkForXmlError("Error while retrieving Normals values");
			// Load Vertice Normals
			QTextStream normalsStream(&normals);
			QList<GLfloat> normalValues;
			QString buff;
			while ((!normalsStream.atEnd()))
			{
				normalsStream >> buff;
				normalValues.append(buff.toFloat());
			}
			pMesh->addNormals(normalValues.toVector());
		}
		// Try to find texture coordinate
		while (endElementNotReached(m_pStreamReader, "VertexBuffer"))
		{
			if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && (m_pStreamReader->name() == "TextureCoordinates"))
			{
				QString texels= getContent(m_pStreamReader, "TextureCoordinates").replace(',', ' ');
				checkForXmlError("Error while retrieving Texture coordinates");
				QTextStream texelStream(&texels);
				QList<GLfloat> texelValues;
				QString buff;
				while ((!texelStream.atEnd()))
				{
					texelStream >> buff;
					texelValues.append(buff.toFloat());
				}
				pMesh->addTexels(texelValues.toVector());
			}
			readNext();;
		}
		++numberOfMesh;
	}

	pMesh->finish();
	currentMeshRep.clean();

	if (GLC_State::cacheIsUsed())
	{
		GLC_CacheManager currentManager= GLC_State::currentCacheManager();
		currentManager.addToCache(QFileInfo(m_FileName).baseName(), currentMeshRep);
	}

	return currentMeshRep;
}
// Load CatMaterial Ref if present
void GLC_3dxmlToWorld::loadCatMaterialRef()
{

	QList<MaterialRef> materialRefList;

	// Load material Name, Id and associated File
	if (setStreamReaderToFile("CATMaterialRef.3dxml", true))
	{
		// Load the material file
		//qDebug() << "CATMaterialRef.3dxml found and current";
		goToElement(m_pStreamReader, "CATMaterialRef");
		checkForXmlError("Element CATMaterialRef not found in CATMaterialRef.3dxml");
		while (endElementNotReached(m_pStreamReader, "CATMaterialRef"))
		{
			if (QXmlStreamReader::StartElement == m_pStreamReader->tokenType())
			{
				const QStringRef currentElementName= m_pStreamReader->name();
				if (currentElementName == "CATMatReference")
				{
					MaterialRef currentMaterial;
					currentMaterial.m_Id= readAttribute("id", true);
					currentMaterial.m_Name= readAttribute("name", true);
					goToElement(m_pStreamReader, "MaterialDomain");
					checkForXmlError("Element MaterialDomain not found after CATMatReference Element");
					currentMaterial.m_AssociatedFile= readAttribute("associatedFile", true).remove("urn:3DXML:");
					materialRefList.append(currentMaterial);
					//qDebug() << "Material " << currentMaterial.m_Name << " Added";
				}
			}
			readNext();;
		}
	}
	// Load material files
	const int size= materialRefList.size();
	for (int i= 0; i < size; ++i)
	{
		if (setStreamReaderToFile(materialRefList.at(i).m_AssociatedFile, true))
		{
			//qDebug() << "Load MaterialDef : " << materialRefList.at(i).m_AssociatedFile;
			loadMaterialDef(materialRefList.at(i));
		}
	}
}

// Create material from material def file
void GLC_3dxmlToWorld::loadMaterialDef(const MaterialRef& materialRef)
{
	GLC_Material* pMaterial= new GLC_Material();
	goToElement(m_pStreamReader, "Osm");
	checkForXmlError(QString("Element Osm not found in file : ") + materialRef.m_AssociatedFile);
	while (endElementNotReached(m_pStreamReader, "Osm"))
	{
		readNext();;
		if ((QXmlStreamReader::StartElement == m_pStreamReader->tokenType()) && m_pStreamReader->name() == "Attr")
		{
			const QString currentName= readAttribute("Name", true);
			if (currentName == "DiffuseColor")
			{
				QString color= m_pStreamReader->attributes().value("Value").toString();
				color.remove('[');
				color.remove(']');
				QStringList colors(color.split(","));
				QColor diffuseColor;
				diffuseColor.setRedF(colors.at(0).toDouble());
				diffuseColor.setGreenF(colors.at(1).toDouble());
				diffuseColor.setBlueF(colors.at(2).toDouble());
				pMaterial->setDiffuseColor(diffuseColor);
			}
			else if (currentName == "Transparency")
			{
				double opacity= readAttribute("Value", true).toDouble();
				opacity= 1.0 - opacity;
				pMaterial->setOpacity(opacity);
			}
			else if (currentName == "SpecularExponent")
			{
				double SpecularExponent= readAttribute("Value", true).toDouble() * 128.0;
				pMaterial->setShininess(SpecularExponent);
			}
			else if (currentName == "TextureImage")
			{
				//qDebug() << "TextureImage";
				QString imageId= readAttribute("Value", true).remove("urn:3DXML:CATRepImage.3dxml#");
				if (m_TextureImagesHash.contains(imageId))
				{
					QString imageName= m_TextureImagesHash.value(imageId);
					GLC_Texture* pTexture= loadTexture(imageName);
					if (NULL != pTexture)
					{
						pMaterial->setTexture(pTexture);
					}
				}
			}
			else if (currentName == "EmissiveCoef")
			{

			}
			else if (currentName == "SpecularColor")
			{
				QString color= readAttribute("Value", true);
				color.remove('[');
				color.remove(']');
				QStringList colors(color.split(","));
				QColor specularColor;
				specularColor.setRedF(colors.at(0).toDouble());
				specularColor.setGreenF(colors.at(1).toDouble());
				specularColor.setBlueF(colors.at(2).toDouble());
				pMaterial->setSpecularColor(specularColor);
			}
			else if (currentName == "AmbientColor")
			{
				QString color= readAttribute("Value", true);
				color.remove('[');
				color.remove(']');
				QStringList colors(color.split(","));
				QColor ambientColor;
				ambientColor.setRedF(colors.at(0).toDouble());
				ambientColor.setGreenF(colors.at(1).toDouble());
				ambientColor.setBlueF(colors.at(2).toDouble());
				pMaterial->setAmbientColor(ambientColor);
			}

		}
	}
	pMaterial->setName(materialRef.m_Name);
	m_MaterialHash.insert(materialRef.m_Id, pMaterial);
}

// Load CATRepIage if present
void GLC_3dxmlToWorld::loadCatRepImage()
{
	// Load texture image name
	if (setStreamReaderToFile("CATRepImage.3dxml", true))
	{
		//qDebug() << "CATRepImage.3dxml Found";
		goToElement(m_pStreamReader, "CATRepImage");
		checkForXmlError("Element CATRepImage not found in CATRepImage.3dxml");
		while (endElementNotReached(m_pStreamReader, "CATRepImage"))
		{
			if (QXmlStreamReader::StartElement == m_pStreamReader->tokenType())
			{
				const QStringRef currentElementName= m_pStreamReader->name();
				if (currentElementName == "CATRepresentationImage")
				{
					QString id= readAttribute("id", true);
					QString associatedFile= readAttribute("associatedFile", true).remove("urn:3DXML:");
					m_TextureImagesHash.insert(id,associatedFile);
				}
			}
			readNext();;
		}
		//qDebug() << "CATRepImage.3dxml Load";
	}
}

// try to load the specified image
GLC_Texture* GLC_3dxmlToWorld::loadTexture(QString fileName)
{
	QString format= QFileInfo(fileName).suffix().toUpper();
	QImage resultImage;
	QString resultImageFileName;
	if (m_IsInArchive)
	{
		// Create QuaZip File
		QuaZipFile* p3dxmlFile= new QuaZipFile(m_p3dxmlArchive);

		// Get the file of the 3dxml
		if (!m_p3dxmlArchive->setCurrentFile(fileName, QuaZip::csInsensitive))
		{
			return NULL;
		}

		// Open the file of the 3dxml
		if(!p3dxmlFile->open(QIODevice::ReadOnly))
	    {
			delete p3dxmlFile;
			QString message(QString("GLC_3dxmlToWorld::loadImage Unable to Open ") + fileName);
			GLC_FileFormatException fileFormatException(message, fileName, GLC_FileFormatException::FileNotSupported);
			clear();
			throw(fileFormatException);
	    }
		resultImage.load(p3dxmlFile, format.toLocal8Bit());
		p3dxmlFile->close();
		delete p3dxmlFile;
		resultImageFileName= glc::builtArchiveString(m_FileName, fileName);
	}
	else
	{
		// Create the file to load
		if (fileName != m_FileName)
		{
			resultImageFileName= QFileInfo(m_FileName).absolutePath() + QDir::separator() + fileName;
		}
		QFile* pCurrentFile= new QFile(resultImageFileName);
		if (!pCurrentFile->open(QIODevice::ReadOnly))
		{
			delete pCurrentFile;
			QString message(QString("GLC_3dxmlToWorld::loadImage File ") + resultImageFileName + QString(" not found"));
			QStringList stringList(m_CurrentFileName);
			stringList.append(message);
			GLC_ErrorLog::addError(stringList);
			return NULL;
		}
		else
		{
			m_SetOfAttachedFileName << resultImageFileName;
		}
		resultImage.load(pCurrentFile, format.toLocal8Bit());
		pCurrentFile->close();
		delete pCurrentFile;
	}

	GLC_Texture* pTexture= NULL;
	if (!resultImage.isNull())
	{
		pTexture= new GLC_Texture(resultImage, resultImageFileName);
	}
	else
	{
		QStringList stringList(m_CurrentFileName);
		stringList.append("Unable to load " + resultImageFileName);
		GLC_ErrorLog::addError(stringList);
	}

	return pTexture;
}

// Factorize material use
void GLC_3dxmlToWorld::factorizeMaterial(GLC_3DRep* pRep)
{
	//qDebug() << "GLC_3dxmlToWorld::factorizeMaterial";
	// Get the Set of materials of the rep
	QSet<GLC_Material*> repMaterialSet= pRep->materialSet();
	//! The hash table of rep material
	QHash<GLC_uint, GLC_Material*> repMaterialHash;
	// Construct the map of material String Hash and Id
	QHash<QString, GLC_uint> materialMap;

	{ // Fill the map of material
		QSet<GLC_Material*>::const_iterator iMat= repMaterialSet.constBegin();
		while(repMaterialSet.constEnd() != iMat)
		{
			GLC_Material* pCurrentMat= *iMat;
			materialMap.insert(QString::number(pCurrentMat->hashCode()), pCurrentMat->id());
			repMaterialHash.insert(pCurrentMat->id(), pCurrentMat);
			++iMat;
		}
	}

	// Make the factorization
	QHash<QString, GLC_uint>::iterator iMat= materialMap.begin();
	while (materialMap.constEnd() != iMat)
	{
		if (m_MaterialHash.contains(iMat.key()))
		{
			//qDebug() << "Replace Mat :" << iMat.key() << " " << iMat.value();
			pRep->replaceMaterial(iMat.value(), m_MaterialHash.value(iMat.key()));
		}
		else
		{
			//qDebug() << "Indert mat " << iMat.key() << " " << iMat.value();
			m_MaterialHash.insert(iMat.key(), repMaterialHash.value(iMat.value()));
		}
		++iMat;
	}

}

void GLC_3dxmlToWorld::setRepresentationFileName(GLC_3DRep* pRep)
{
	if (m_IsInArchive)
	{
		pRep->setFileName(glc::builtArchiveString(m_FileName, m_CurrentFileName));
	}
	else
	{
		pRep->setFileName(QFileInfo(m_FileName).absolutePath() + QDir::separator() + m_CurrentFileName);
	}
}

void GLC_3dxmlToWorld::checkFileValidity(QIODevice* pIODevice)
{
	QByteArray begining= pIODevice->read(2);
	if (begining == "V5")
	{
		QString message(QString("GLC_3dxmlToWorld::setStreamReaderToFile : File ") + m_CurrentFileName + " is binary");
		GLC_FileFormatException fileFormatException(message, m_CurrentFileName, GLC_FileFormatException::FileNotSupported);
		clear();
		throw(fileFormatException);
	}
	else
	{
		pIODevice->seek(0);
	}
}
