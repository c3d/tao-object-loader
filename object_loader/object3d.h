#ifndef OBJECT3D_H
#define OBJECT3D_H
// *****************************************************************************
// object3d.h                                                      Tao3D project
// *****************************************************************************
//
// File description:
//
//    Load a 3D model file and render it using GLC_Lib.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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


#include "tao/coords3d.h"
#include "tao/module_api.h"
#include <QObject>
#include <QTime>
#include <QImage>
#include <vector>
#include <iostream>
#include <GLC_World>

class LoadThread;

struct Object3D : public QObject
// ----------------------------------------------------------------------------
//   Representation of a complete 3D object
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Object3D(kstring name = NULL, bool colored = false);
    ~Object3D();

    // Loading an objet file
    void              Load(kstring name);

    // Draw interface
    void              Draw();
    void              Identify();

    // Object bounding box
    Tao::Box3         Bounds();

    // Object3D cache
    static Object3D * Object(text name, bool colored);

    // Call ((Object3D *)arg)->Draw()
    static void       render_callback(void *arg);

public slots:
    void              percentComplete(int p);
    void              loadFinished();
    void              loadFailed();

public:
    // Debug trace
    static std::ostream& debug();

public:
    enum Status {
        NotStarted = 0,
        InProgress,
        LoadFailed,
        LoadSuccess,
    };

private:
    void              DrawObject();
    void              IdentifyObject();
    void              DrawPlaceHolder();
    void              DrawErrorPlaceHolder();

private:
    static text       toText(QString s);
    static void       initGLC();
    static void       checkCurrentContext();

private:
    typedef std::map<text, Object3D *> file_map;
    static file_map           loaded;
    static const QGLContext * context;

private:
    // Representation of an object
    GLC_World     glcWorld;
    // Thread to load file asynchronously
    LoadThread *  loadThread;
    // During load, percent complete
    int           complete;
    // Load start time
    QTime         loadTime;
    // Textured object?
    bool          hasTexture;

public:
    // File load status
    Status        status;
    // GLC_Lib error message in case status == LoadError
    QString       errorStr;
    // True if Draw should force use of current color/material
    bool          colored;

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;
    // Collection of images to show load progress
#   define         NPROGRESS 21
    static QImage  progress[NPROGRESS];

    Q_OBJECT
};

#endif // OBJECT3D_H
