// *****************************************************************************
// preferences_dialog.cpp                                          Tao3D project
// *****************************************************************************
//
// File description:
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
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2012, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2012,2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2012, Jérôme Forissier <jerome@taodyne.com>
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
