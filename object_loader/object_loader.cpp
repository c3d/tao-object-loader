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
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao_gl.h"
#include "object_loader.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "object3d.h"
#include "object3d_drawing.h"
#include "tao/module_api.h"
#include <GLC_Exception>
#include <QImage>
#include <QGLWidget>


using namespace XL;

XL_DEFINE_TRACES

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
        Object3D *obj = Object3D::Object(name);
        if (!obj)
        {
            Ooops("File not found or unreadable: $1", self);
            return XL::xl_false;
        }

        Object3DDrawing *drawing = new Object3DDrawing(obj, x, y, z, w, h, d,
                                                       colored);
        Object3D::tao->addToLayout(Object3DDrawing::render_callback, drawing,
                                   Object3DDrawing::delete_callback);
        Object3D::tao->addControlBox(x, y, z, w, h, d);
    }
    catch (GLC_Exception e)
    {
        return XL::Ooops(e.what(), self);
    }

    return XL::xl_true;
}


Tree_p object(Tree_p self, Text_p name)
// ----------------------------------------------------------------------------
//   Load a 3D object and show it as-is (not coordinate manipulation)
// ----------------------------------------------------------------------------
{
    if (!Object3D::tao)
        return XL::xl_false;

    try
    {
        Object3D *obj = Object3D::Object(name);
        if (!obj)
        {
            Ooops("File not found or unreadable: $1", self);
            return XL::xl_false;
        }

        Object3D::tao->scheduleRender(Object3D::render_callback, obj);
    }
    catch (GLC_Exception e)
    {
        return XL::Ooops(e.what(), self);
    }

    return XL::xl_true;
}


int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *mod)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    glewInit();
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
