// ****************************************************************************
//  preferences_dialog.h                                           Tao project
// ****************************************************************************
//
//   File Description:
//
//    Display a message dialog to allow user to configure the module
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
//  (C) 2011 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>
#include <QString>

// QSettings group name to read/write preferences
#define OBJLOADER_GROUP "ObjectLoader"

class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog() {}

public slots:
    void           useVBOToggled(bool use);

public:
    static bool    useVBOs();
    static bool    useVBOsDefault();
};


#endif // PREFERENCES_DIALOG_H
