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


#include "../glc-lib/glc_ext.h"
#if !defined(Q_OS_MAC)
#ifdef __linux__
#  if __GNUC__ >= 4
#    define DLL_PRIVATE __attribute__ ((visibility ("hidden")))
#  else
#    define DLL_PRIVATE
#  endif
#else
#  define DLL_PRIVATE
#endif
DLL_PRIVATE PFNGLUSEPROGRAMPROC glUseProgram;
#endif

#include "object3d.h"
#include "load_thread.h"
#include "preferences_dialog.h"
#include <QString>
#include <QFileInfo>
#include <QEvent>
#include <GLC_Factory>

using namespace Tao;


const ModuleApi *  Object3D::tao = NULL;
QImage             Object3D::progress[NPROGRESS];
const QGLContext * Object3D::context = NULL;
Object3D::file_map Object3D::loaded;


void Object3D::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Object3D *)arg)->Draw();
}


Object3D::Object3D(kstring name, bool colored)
// ----------------------------------------------------------------------------
//   Initialize an object. If a name is given, load the file
// ----------------------------------------------------------------------------
    : glcWorld(), loadThread(NULL), complete(0), hasTexture(false),
      status(NotStarted), colored(colored)
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
    {
        status = LoadSuccess;
        // Search if object contains a texture
        for(int i = 0; i < glcWorld.listOfMaterials().size(); i++)
            hasTexture |= glcWorld.listOfMaterials()[i]->hasTexture();
    }
    delete loadThread;
    loadThread = NULL;
}


void Object3D::loadFailed()
// ----------------------------------------------------------------------------
//   Show load error
// ----------------------------------------------------------------------------
{
    errorStr = loadThread->error;
    IFTRACE(objloader)
        debug() << "Load error: " << errorStr.toStdString() << "\n";
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


void Object3D::Identify()
// ----------------------------------------------------------------------------
//   Identify object under cursor
// ----------------------------------------------------------------------------
{
    switch (status)
    {
    case NotStarted:
    case InProgress:
    case LoadFailed:
        break;

    case LoadSuccess:
        IdentifyObject();       break;

    default:                    break;
    }
}


void Object3D::DrawObject()
// ----------------------------------------------------------------------------
//   Draw the 3D object
// ----------------------------------------------------------------------------
{
    static bool licensed, tested = false;
    if (!tested)
    {
        licensed = tao->checkImpressOrLicense("ObjectLoader 1.02");
        tested = true;
    }

    checkCurrentContext();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glEnable(GL_NORMALIZE);

    // If colored then use Tao materials
    // otherwise use object materials.
    GLC_3DViewCollection *items = glcWorld.collection();
    if(colored)
    {
         // If color on lines is non transparent, then draw
         // wireframe object
        if(Object3D::tao->SetLineColor())
        {
            // Set wireframe mode
            items->setPolygonModeForAll(GL_FRONT_AND_BACK, GL_LINE);

            glcWorld.render(0, glc::GeometryOnlyRenderFlag);

            // Reset polygon mode
            items->setPolygonModeForAll(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Classic draw with color
        if(Object3D::tao->SetFillColor())
        {
            GLfloat color[4];
            glGetFloatv(GL_CURRENT_COLOR, color);
            if (color[3] != 1)
                glDepthMask(GL_FALSE);
            glcWorld.render(0, glc::GeometryOnlyRenderFlag);
            if (color[3] != 1)
                glDepthMask(GL_TRUE);
        }
    }
    else
    {
        // Classic draw
        if(Object3D::tao->SetFillColor())
        {
            glcWorld.render(0, glc::ShadingFlag);
            glcWorld.render(0, glc::TransparentRenderFlag);
        }
    }
}


void Object3D::IdentifyObject()
// ----------------------------------------------------------------------------
//   Identify the 3D object
// ----------------------------------------------------------------------------
{
    checkCurrentContext();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

    glDisable(GL_NORMALIZE);

    // Classic draw
    if(Object3D::tao->SetFillColor())
    {
        glUseProgram(0);
        glcWorld.render(0, glc::ShadingFlag);
        glUseProgram(0);
        glcWorld.render(0, glc::TransparentRenderFlag);
    }
}


void Object3D::DrawPlaceHolder()
// ----------------------------------------------------------------------------
//   Draw a placeholder object
// ----------------------------------------------------------------------------
{
    // Request refresh on next time interval
    tao->refreshOn(QEvent::Timer, -1.0);
    if (loadTime.elapsed() < 2000)
        return;
    if (!progress[0].isNull())
    {
        GLint prog = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
        if (prog)
            glUseProgram(0);

        int idx = (NPROGRESS - 1) * complete / 100;
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


Object3D *Object3D::Object(text name, bool colored)
// ----------------------------------------------------------------------------
//   Maintain a list of object files currently in use
// ----------------------------------------------------------------------------
{
    checkCurrentContext();

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
                Object3D *object = new Object3D(path.c_str(), colored);
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


void Object3D::checkCurrentContext()
// ----------------------------------------------------------------------------
//   Do what is needed if GL context has changed
// ----------------------------------------------------------------------------
{
    if (context)
    {
        if (context == QGLContext::currentContext())
            return;
        IFTRACE(objloader)
            debug() << "GL context has changed\n";
        // Force reload, because textures used by cached objects would be
        // invalid.
        // REVISIT: it would be far more efficient to tell GLC_Lib to reload
        // its textures.
        IFTRACE(objloader)
            debug() << "Clearing cache\n";
        loaded.clear();
    }

    IFTRACE(objloader)
        debug() << "Initializing GLC_Lib\n";
    initGLC();
}


static text supp(bool b)
// ----------------------------------------------------------------------------
//   Debug helper
// ----------------------------------------------------------------------------
{
    if (!b)
        return "not ";
    return "";
}


void Object3D::initGLC()
// ----------------------------------------------------------------------------
//   Initialize the GLC library
// ----------------------------------------------------------------------------
{
    GLC_State::init();
    bool useVBOs = PreferencesDialog::useVBOs();
    GLC_State::setVboUsage(useVBOs);
    context = QGLContext::currentContext();

    IFTRACE(objloader)
    {
        debug() << "GLC_Lib has detected the following:\n";
        debug() << "  GL version: " << toText(GLC_State::version()) << "\n";
        debug() << "  Vendor: " << toText(GLC_State::vendor()) << "\n";
        debug() << "  Renderer: " << toText(GLC_State::renderer()) << "\n";
        debug() << "  VBOs " << supp(GLC_State::vboSupported())
                << "supported\n";
        debug() << "  VBOs " << supp(GLC_State::vboUsed())
                << "used\n";
    }

#if !defined(Q_OS_MACX)
    glUseProgram = (PFNGLUSEPROGRAMPROC)context->getProcAddress("glUseProgram");
    Q_ASSERT(glUseProgram);
#endif
}


std::ostream& Object3D::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[ObjLoader] ";
    return std::cerr;
}


text Object3D::toText(QString s)
// ----------------------------------------------------------------------------
//   Convert QString to UTF-8 encoded std::string (text)
// ----------------------------------------------------------------------------
{
    return std::string(s.toUtf8().constData());
}
