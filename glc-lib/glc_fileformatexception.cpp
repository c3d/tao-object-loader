// *****************************************************************************
// glc_fileformatexception.cpp                                     Tao3D project
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
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011,2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011,2013-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011,2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011,2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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

//! \file glc_fileformatexception.cpp implementation of the GLC_FileFormatException class.

#include "glc_fileformatexception.h"

GLC_FileFormatException::GLC_FileFormatException(const QString &message, const QString &fileName, ExceptionType exceptionType)
: GLC_Exception(message)
, m_FileName(fileName)
, m_ExceptionType(exceptionType)
{

}

GLC_FileFormatException::~GLC_FileFormatException() throw()
{
}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return exception description
const char* GLC_FileFormatException::what() const throw()
{
	QString exceptionmsg("GLC_FileFormatException : ");
	exceptionmsg.append(m_ErrorDescription);
	exceptionmsg.append(" in file : ");
	exceptionmsg.append(m_FileName);
	return exceptionmsg.toUtf8().data();
}
