// ****************************************************************************
//  load_thread.cpp                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//    Implementation of the LoadThread class
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "load_thread.h"
#include <GLC_Factory>
#include <GLC_FileLoader>
#include <GLC_World>
#include <GLC_Exception>


void LoadThread::run()
// ----------------------------------------------------------------------------
//   Load the file
// ----------------------------------------------------------------------------
{
    try
    {
        GLC_FileLoader * loader = GLC_Factory::instance()->createFileLoader();
        connect(loader, SIGNAL(currentQuantum(int)),
                this,   SIGNAL(percentComplete(int)));
        QFile file(path);
        world = loader->createWorldFromFile(file);
        delete loader;
    }
    catch (GLC_Exception e)
    {
        error = e.what();
        emit failed();
    }
}
