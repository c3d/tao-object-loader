// ****************************************************************************
//  object3d.cpp                                                   Tao project
// ****************************************************************************
//
//   File Description:
//
//    Object3D implementation
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
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "object3d.h"
#include "load_thread.h"
#include <QString>
#include <QFileInfo>
#include <GLC_Factory>

using namespace Tao;


const ModuleApi *Object3D::tao = NULL;
QImage           Object3D::progress[NPROGRESS];


void Object3D::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Object3D *)arg)->Draw();
}


Object3D::Object3D(kstring name)
// ----------------------------------------------------------------------------
//   Initialize an object. If a name is given, load the file
// ----------------------------------------------------------------------------
    : glcWorld(), loadThread(NULL), status(NotStarted), complete(0)
{
    if (name)
        Load(name);
}


Object3D::~Object3D()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void Object3D::Load(kstring name)
// ----------------------------------------------------------------------------
//    Load a 3D object file from disk
// ----------------------------------------------------------------------------
{
    IFTRACE(objloader)
        debug() << "Start loading " << name << "\n";

    if (!loadThread)
    {
        status = InProgress;
        loadTime.start();
        loadThread = new LoadThread(name);
        connect(loadThread, SIGNAL(percentComplete(int)),
                this,       SLOT(percentComplete(int)));
        connect(loadThread, SIGNAL(finished()),
                this,       SLOT(loadFinished()));
        connect(loadThread, SIGNAL(failed()),
                this,       SLOT(loadFailed()));
        loadThread->start(QThread::LowPriority);
    }
}


void Object3D::percentComplete(int p)
// ----------------------------------------------------------------------------
//   Show loading progress
// ----------------------------------------------------------------------------
{
    IFTRACE(objloader)
        std::cerr << p << "%...";
    complete = p;
}


void Object3D::loadFinished()
// ----------------------------------------------------------------------------
//   Activate display of object
// ----------------------------------------------------------------------------
{
    IFTRACE(objloader)
        std::cerr << "done!\n";
    glcWorld = loadThread->world;
    if (status != LoadFailed && !glcWorld.isEmpty())
        status = LoadSuccess;
    delete loadThread;
    loadThread = NULL;
}


void Object3D::loadFailed()
// ----------------------------------------------------------------------------
//   Show load error
// ----------------------------------------------------------------------------
{
    IFTRACE(objloader)
        debug() << "Load error: " << loadThread->error.toStdString() << "\n";
    status = LoadFailed;
}


void Object3D::Draw()
// ----------------------------------------------------------------------------
//   Draw the 3D object or a placeholder
// ----------------------------------------------------------------------------
{
    switch (status)
    {
    case NotStarted:
    case InProgress:
        DrawPlaceHolder();      break;

    case LoadFailed:
        DrawErrorPlaceHolder(); break;

    case LoadSuccess:
        DrawObject();           break;

    default:                    break;
    }
}


void Object3D::DrawObject()
// ----------------------------------------------------------------------------
//   Draw the 3D object
// ----------------------------------------------------------------------------
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TRANSFORM_BIT);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glEnable(GL_NORMALIZE);

    glcWorld.render(0, glc::ShadingFlag);
    glcWorld.render(0, glc::TransparentRenderFlag);

    glPopAttrib();
}


void Object3D::DrawPlaceHolder()
// ----------------------------------------------------------------------------
//   Draw a placeholder object
// ----------------------------------------------------------------------------
{
    // Request refresh on next time interval
    tao->refreshOn(QEvent::Timer);
    if (loadTime.elapsed() < 2000)
        return;
    if (!progress[0].isNull())
    {
        GLint prog = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
        if (prog)
            glUseProgram(0);

        int idx = NPROGRESS * complete / 100;
        QImage img = progress[idx];
        glRasterPos3d(0, 0, 0);
        glDrawPixels(img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                     img.bits());

        if (prog)
            glUseProgram(prog);
    }
}


void Object3D::DrawErrorPlaceHolder()
// ----------------------------------------------------------------------------
//   Draw a placeholder object
// ----------------------------------------------------------------------------
{
}


Box3 Object3D::Bounds()
// ----------------------------------------------------------------------------
//   Return object's bounding box. IsEmpty() is true when bounds are unknown.
// ----------------------------------------------------------------------------
{
    if (status != LoadSuccess)
        return Box3();

    GLC_BoundingBox b = glcWorld.boundingBox();
    GLC_Point3d     l = b.lowerCorner();
    return Box3(l.x(), l.y(), l.z(), b.xLength(), b.yLength(), b.zLength());
}


Object3D *Object3D::Object(text name)
// ----------------------------------------------------------------------------
//   Maintain a list of object files currently in use
// ----------------------------------------------------------------------------
{
    typedef std::map<text, Object3D *> file_map;
    static file_map loaded;
    static int use_vbo = -1;
    static const QGLContext *context = NULL;

    if (QGLContext::currentContext() != context)
    {
        // GL context has changed. Force reload, because textures used by
        // cached objects would be invalid.
        // REVISIT: it would be far more efficient to tell GLC_Lib to reload
        // its textures.
        IFTRACE(objloader)
            debug() << "GL context changed: clearing cache\n";
        loaded.clear();
        context = QGLContext::currentContext();
        use_vbo = -1;
    }
    if (use_vbo == -1)
    {
        // Enable Vertex Buffer Objects only if they're supported, otherwise
        // GLC may crash
        use_vbo = GLC_State::vboSupported();
        GLC_State::setVboUsage(use_vbo);
        IFTRACE(objloader)
        {
            const char * nt = use_vbo ? "" : "not ";
            debug() << " Info: VBOs " << nt << "supported\n";
        }
    }

    file_map::iterator found = loaded.find(name);
    if (found == loaded.end())
    {
        QString qname(name.c_str());
        QFileInfo file(qname);
        if (!file.isReadable())
        {
            QFile qualified ("object:" + qname);
            file = QFileInfo(qualified);
        }
        text path = file.canonicalFilePath().toStdString();
        file.setFile(QString(path.c_str()));
        if (file.isReadable())
        {
            found = loaded.find(path);
            if (found == loaded.end())
            {
                Object3D *object = new Object3D(path.c_str());
                loaded[name] = object;
                loaded[path] = object;
            }
        }
        else
        {
            // Record that we don't know how to load this guy
            loaded[name] = NULL;
        }

        // We should have a valid entry now
        found = loaded.find(name);
    }

    return (*found).second;
}


std::ostream& Object3D::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[ObjLoader] ";
    return std::cerr;
}
