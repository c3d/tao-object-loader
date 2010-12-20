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
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "load_thread.h"
#include <GLC_Factory>
#include <GLC_World>
#include <GLC_Exception>


LoadThread::LoadThread(QString path)
// ----------------------------------------------------------------------------
//   Prepare thread to load one file
// ----------------------------------------------------------------------------
    : path(path), factory(GLC_Factory::instance())
{
    connect(factory, SIGNAL(currentQuantum(int)),
            this,    SIGNAL(percentComplete(int)), Qt::QueuedConnection);
}


LoadThread::~LoadThread()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void LoadThread::run()
// ----------------------------------------------------------------------------
//   Load the file
// ----------------------------------------------------------------------------
{
    try
    {
        QFile file(path);
        world = factory->createWorldFromFile(file);
    }
    catch (GLC_Exception e)
    {
        error = e.what();
        emit failed();
    }
}
