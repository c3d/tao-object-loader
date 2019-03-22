#ifndef LOAD_THREAD_H
#define LOAD_THREAD_H
// *****************************************************************************
// load_thread.h                                                   Tao3D project
// *****************************************************************************
//
// File description:
//
//    Load a 3D model file asynchronously using GLC_Lib
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2012, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2012,2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2012, Jérôme Forissier <jerome@taodyne.com>
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


#include <QFile>
#include <QThread>
#include <GLC_Factory>
#include <GLC_World>


class LoadThread : public QThread
// ----------------------------------------------------------------------------
//   Load a 3D object file in a new thread
// ----------------------------------------------------------------------------
{
public:
    LoadThread(QString path) : path(path) {}
    ~LoadThread() {}

    // Load the file
    virtual void   run();

signals:
    void           percentComplete(int percent);
    void           failed();

public:
    QString        path;
    GLC_World      world;
    QString        error;

    Q_OBJECT
};

#endif // LOAD_THREAD_H
