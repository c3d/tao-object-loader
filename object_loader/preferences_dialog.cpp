// ****************************************************************************
//  preferences_dialog.cpp                                         Tao project
// ****************************************************************************
//
//   File Description:
//
//    Preferences dialog implementation
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
//  (C) 2011 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSettings>
#include <QVariant>
#include "preferences_dialog.h"


PreferencesDialog::PreferencesDialog(QWidget *parent)
// ----------------------------------------------------------------------------
//   Construct dialog
// ----------------------------------------------------------------------------
    : QDialog(parent)
{
    setWindowTitle(tr("3D Object Loader Preferences"));

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel(tr("Note: you need to restart for changes to "
                                  "take effect."));
    layout->addWidget(label);
    QCheckBox *vbo = new QCheckBox(tr("Use VBOs if available"));
    vbo->setChecked(useVBOs());
    connect(vbo, SIGNAL(toggled(bool)), this, SLOT(useVBOToggled(bool)));
    layout->addWidget(vbo);

    setLayout(layout);
}


void PreferencesDialog::useVBOToggled(bool use)
// ----------------------------------------------------------------------------
//   Save setting
// ----------------------------------------------------------------------------
{
    QSettings settings;
    settings.beginGroup(OBJLOADER_GROUP);
    if (use == useVBOsDefault())
        settings.remove("UseVBOs");
    else
        settings.setValue("UseVBOs", QVariant(use));
}


bool PreferencesDialog::useVBOs()
// ----------------------------------------------------------------------------
//   Read "use VBOs" setting from preferences
// ----------------------------------------------------------------------------
{
    bool useDflt = useVBOsDefault();
    QSettings settings;
    settings.beginGroup(OBJLOADER_GROUP);
    bool useVBOs = settings.value("UseVBOs", QVariant(useDflt)).toBool();
    return useVBOs;
}


bool PreferencesDialog::useVBOsDefault()
// ----------------------------------------------------------------------------
//   Should we allow use of VBOs if there is nothing in settings?
// ----------------------------------------------------------------------------
{
    return true;
}
