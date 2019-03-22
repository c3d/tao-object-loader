// *****************************************************************************
// glc_log.cpp                                                     Tao3D project
// *****************************************************************************
//
// File description:
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
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011, Jérôme Forissier <jerome@taodyne.com>
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
/*

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

//! \file glc_log.h implementation of the GLC_Log class.

#include "glc_log.h"
#include <QtDebug>
#include <QTime>

GLC_Log::GLC_Log(const QString& baseLogFileName)
: m_pFile(new QTemporaryFile(baseLogFileName))
, m_TextStream()
{
	Q_CHECK_PTR(m_pFile);
	m_pFile->open();
	m_pFile->setAutoRemove(false);
	m_TextStream.setDevice(m_pFile);
}

GLC_Log::~GLC_Log()
{
	m_TextStream.flush();
	delete m_pFile;
}

QString GLC_Log::fullFileName() const
{
	Q_ASSERT(NULL != m_pFile);
	return m_pFile->fileName();
}

void GLC_Log::add(const QString& line)
{
	Q_ASSERT(NULL != m_pFile);
	qWarning() << line;
	m_TextStream << line << '\n';
	m_TextStream.flush();
}

void GLC_Log::addSeparator()
{
	Q_ASSERT(NULL != m_pFile);
	const QString separator("---------------------------------------------------------------------");
	qWarning() << separator;
	m_TextStream << separator << '\n';
	m_TextStream.flush();
}

void GLC_Log::addCurrentTime()
{
	Q_ASSERT(NULL != m_pFile);
	m_TextStream << QTime::currentTime().toString() << '\n';
	m_TextStream.flush();
}
