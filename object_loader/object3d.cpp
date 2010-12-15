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
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include "object3d.h"
#include <QString>
#include <QFileInfo>
#include <GLC_Factory>


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
    : glcWorld()
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
    QFile file(name);
    glcWorld = GLC_Factory::instance()->createWorldFromFile(file);
}


void Object3D::Draw()
// ----------------------------------------------------------------------------
//   Draw the 3D object
// ----------------------------------------------------------------------------
{
    glcWorld.render(0, glc::ShadingFlag);
}


Object3D *Object3D::Object(text name)
// ----------------------------------------------------------------------------
//   Maintain a list of object files currently in use
// ----------------------------------------------------------------------------
{
    typedef std::map<text, Object3D *> file_map;
    static file_map loaded;

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
