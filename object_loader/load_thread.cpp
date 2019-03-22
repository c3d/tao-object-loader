// *****************************************************************************
// load_thread.cpp                                                 Tao3D project
// *****************************************************************************
//
// File description:
//
//    Implementation of the LoadThread class
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
// (C) 2011,2014,2019, Christophe de Dinechin <christophe@dinechin.org>
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
