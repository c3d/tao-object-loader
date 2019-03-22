// *****************************************************************************
// glc_bsreptoworld.h                                              Tao3D project
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
//! \file glc_bsreptoworld.h interface for the GLC_BSRepToWorld class.

#ifndef GLC_BSREPTOWORLD_H_
#define GLC_BSREPTOWORLD_H_

#include <QFile>
#include "../glc_config.h"

class GLC_World;

//////////////////////////////////////////////////////////////////////
//! \class GLC_BSRepToWorld
/*! \brief GLC_BSRepToWorld : Create an GLC_World from BSRep file */

/*! An GLC_BSRepToWorld extract the only mesh from an .BSRep file*/
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_BSRepToWorld
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	GLC_BSRepToWorld();
	virtual ~GLC_BSRepToWorld();
//@}

//////////////////////////////////////////////////////////////////////
/*! @name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Create and return an GLC_World* from an input BSRep File
	GLC_World* CreateWorldFromBSRep(QFile &file);
//@}

};

#endif /* GLC_BSREPTOWORLD_H_ */
