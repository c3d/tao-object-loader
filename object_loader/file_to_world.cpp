// ****************************************************************************
//  file_to_world.cpp                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//    Implementation of the FileToWorld class
//
//
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "file_to_world.h"

#define USE_GLC_PRIVATE

#if defined(USE_GLC_PRIVATE)

// Private GLC headers
#include "io/glc_objtoworld.h"
#include "io/glc_stltoworld.h"
#include "io/glc_offtoworld.h"
#include "io/glc_3dstoworld.h"
#include "io/glc_3dxmltoworld.h"
#include "io/glc_colladatoworld.h"
#include "io/glc_bsreptoworld.h"

#else

#include <GLC_Factory>

#endif

#include <GLC_FileFormatException>
#include <QFileInfo>


GLC_World FileToWorld::createWorldFromFile(QFile & file,
                                           QStringList * pAttachedFileName)
// ----------------------------------------------------------------------------
//  Create GLC_World from model file. NB: Copied verbatim from glc_factory.cpp
// ----------------------------------------------------------------------------
{
#if defined(USE_GLC_PRIVATE)

    //  NB: This code is copied verbatim from GLC_Factory::createWorldFromFile

    GLC_World* pWorld= NULL;
    if (QFileInfo(file).suffix().toLower() == "obj")
    {
        GLC_ObjToWorld objToWorld(m_pQGLContext);
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
        GLC_3dsToWorld studioToWorld(m_pQGLContext);
        connect(&studioToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
        pWorld= studioToWorld.CreateWorldFrom3ds(file);
        if (NULL != pAttachedFileName)
        {
            (*pAttachedFileName)= studioToWorld.listOfAttachedFileName();
        }
    }
    else if (QFileInfo(file).suffix().toLower() == "3dxml")
    {
        GLC_3dxmlToWorld d3dxmlToWorld(m_pQGLContext);
        connect(&d3dxmlToWorld, SIGNAL(currentQuantum(int)), this, SIGNAL(currentQuantum(int)));
        pWorld= d3dxmlToWorld.createWorldFrom3dxml(file, false);
        if (NULL != pAttachedFileName)
        {
            (*pAttachedFileName)= d3dxmlToWorld.listOfAttachedFileName();
        }
    }
    else if (QFileInfo(file).suffix().toLower() == "dae")
    {
        GLC_ColladaToWorld colladaToWorld(m_pQGLContext);
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

#else

    // Note: with this implementation, currentQuantum() reports will be bogus
    // if several files are loaded simultaneously
    GLC_Factory *factory = GLC_Factory::instance(m_pQGLContext);
    connect(factory, SIGNAL(currentQuantum(int)),
            this,    SIGNAL(currentQuantum(int)));
    return factory->createWorldFromFile(file, pAttachedFileName);

#endif
}
