// ****************************************************************************
//  object_loader.cpp                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//     Implementation of the XL primitives for the "object loader" module.
//
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
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "object_loader.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "object3d.h"
#include "object3d_drawing.h"
#include "tao/module_api.h"
#include "preferences_dialog.h"
#include <GLC_Exception>
#include <QImage>
#include <QGLWidget>

using namespace XL;

XL_DEFINE_TRACES

static XL::Name_p logLoadError(Object3D *obj, Tree_p self)
// ----------------------------------------------------------------------------
//   Helper function: forward GLC load error message to Tao
// ----------------------------------------------------------------------------
{
    XL_ASSERT(obj->status == Object3D::LoadFailed);
    Ooops("ObjectLoader: Could not load 3D object in $1, $2", self)
        .Arg(obj->errorStr.toStdString());
    return XL::xl_false;
}


Tree_p object(Tree_p self,
              Real_p x, Real_p y, Real_p z,
              Real_p w, Real_p h, Real_p d,
              Text_p name, bool colored)
// ----------------------------------------------------------------------------
//   Primitive to load a 3D object and show it centered at (x, y, z)
// ----------------------------------------------------------------------------
{
    if (!Object3D::tao)
        return XL::xl_false;

    try
    {
        Object3D *obj = Object3D::Object(name, colored);
        if (!obj)
        {
            Ooops("ObjectLoader: Unable to load $2 in $1",
                  self, name);
            return XL::xl_false;
        }
        if (obj->status == Object3D::LoadFailed)
            return logLoadError(obj, self);

        Object3DDrawing *drawing = new Object3DDrawing(obj, x, y, z, w, h, d,
                                                       colored);
        Object3D::tao->AddToLayout2(Object3DDrawing::render_callback,
                                    Object3DDrawing::identify_callback,
                                    drawing,
                                    Object3DDrawing::delete_callback);
        Object3D::tao->addControlBox(x, y, z, w, h, d);
    }
    catch (GLC_Exception e)
    {
        XL::Ooops("ObjectLoader: Error loading $2 in $1: $3", self, name)
            .Arg(e.what());
        return XL::xl_false;
    }

    return XL::xl_true;
}


Tree_p object(Tree_p self, Text_p name, bool colored)
// ----------------------------------------------------------------------------
//   Load a 3D object and show it as-is (not coordinate manipulation)
// ----------------------------------------------------------------------------
{
    if (!Object3D::tao)
        return XL::xl_false;

    try
    {
        Object3D *obj = Object3D::Object(name, colored);
        if (!obj)
        {
            Ooops("ObjectLoader: File $2 unreadable in $1", self)
                .Arg((Tree *) name);
            return XL::xl_false;
        }
        if (obj->status == Object3D::LoadFailed)
            return logLoadError(obj, self);

        Object3D::tao->scheduleRender(Object3D::render_callback, obj);
    }
    catch (GLC_Exception e)
    {
        XL::Ooops("ObjectLoader: Error loading $2 in $1: $3", self)
            .Arg((Tree *) name).Arg(e.what());
        return XL::xl_false;
    }

    return XL::xl_true;
}


int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *mod)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    Object3D::tao = api;

    // Pre-load progress images
    QString path = QString("%1/progress.png").arg(mod->path.c_str());
    QImage progress(path);
    if (!progress.isNull())
    {
        int w = progress.width()/NPROGRESS;
        int h = progress.height();
        int x = 0, y = 0;
        for (int i = 0; i < NPROGRESS; i++)
        {
            QImage img = progress.copy(x, y, w, h);
            Object3D::progress[i] = QGLWidget::convertToGLFormat(img);
            x += w;
        }
    }
    else
    {
        Object3D::debug() << QObject::tr("%1 not found\n").arg(path)
                                                          .toStdString();
    }
    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    return 0;
}


int show_preferences()
// ----------------------------------------------------------------------------
//   Show the module preference dialog
// ----------------------------------------------------------------------------
{
    PreferencesDialog().exec();
    return 0;
}
