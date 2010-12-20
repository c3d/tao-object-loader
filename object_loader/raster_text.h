#ifndef RASTER_TEXT_H
#define RASTER_TEXT_H
// ****************************************************************************
//  raster_text.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//    Show a 2D text overlay using a simple bitmap font. Text position is
//    defined in 3D.
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


#include <QtOpenGL>


struct RasterText
// ----------------------------------------------------------------------------
//   Display bitmap text in the GL view
// ----------------------------------------------------------------------------
{
    // Print text at position (0, 0, 0)
    static int printf(const char* format, ...);

private:
    RasterText();
    ~RasterText();

    static void makeRasterFont();

private:
    static struct Cleanup
    // ------------------------------------------------------------------------
    //   Automatically delete the RasterText instance
    // ------------------------------------------------------------------------
    {
        ~Cleanup()
        {
            if (RasterText::instance)
                delete RasterText::instance;
        }
    } cleanup;

private:
    static RasterText * instance;
    static GLuint       fontOffset;
};

#endif // RASTER_TEXT_H
