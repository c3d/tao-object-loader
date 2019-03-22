// *****************************************************************************
// glc_stltoworld.h                                                Tao3D project
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

//! \file glc_stltoworld.h interface for the GLC_StlToWorld class.

#ifndef GLC_STLTOWORLD_H_
#define GLC_STLTOWORLD_H_

#include <QString>
#include <QObject>
#include <QFile>
#include <QTextStream>

#include "../geometry/glc_mesh.h"
#include "../maths/glc_vector3df.h"

#include "../glc_config.h"

class GLC_World;
class QGLContext;

//////////////////////////////////////////////////////////////////////
//! \class GLC_StlToWorld
/*! \brief GLC_StlToWorld : Create an GLC_World from stl file */

/*! An GLC_StlToWorld extract the only mesh from an .stl file \n
 * 	List of elements extracted from the STL
 * 		- Vertex
 * 		- Face
 * 		- Normal coordinate
  */
//////////////////////////////////////////////////////////////////////

class GLC_LIB_EXPORT GLC_StlToWorld : public QObject
{
	Q_OBJECT
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	GLC_StlToWorld();
	virtual ~GLC_StlToWorld();
//@}
//////////////////////////////////////////////////////////////////////
/*! @name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Create and return an GLC_World* from an input STL File
	GLC_World* CreateWorldFromStl(QFile &file);
//@}

//////////////////////////////////////////////////////////////////////
/*! @name Private services functions */
//@{
//////////////////////////////////////////////////////////////////////
private:
	//! clear stlToWorld allocate memmory
	void clear();
	//! Scan a line previously extracted from STL file
	void scanFacet();
	//! Extract a 3D Vector from a string
	GLC_Vector3df extract3dVect(QString &);
	//! Load Binarie STL File
	void LoadBinariStl(QFile &);



//@}

//////////////////////////////////////////////////////////////////////
// Qt Signals
//////////////////////////////////////////////////////////////////////
	signals:
	void currentQuantum(int);

//////////////////////////////////////////////////////////////////////
	/* Private members */
//////////////////////////////////////////////////////////////////////
private:
	//! pointer to a GLC_World
	GLC_World* m_pWorld;

	//! The Stl File name
	QString m_FileName;

	//! The current line number
	int m_CurrentLineNumber;

	//! The Text Stream
	QTextStream m_StlStream;

	//! The current mesh
	GLC_Mesh* m_pCurrentMesh;

	//! Current face index
	IndexList m_CurrentFace;

	//! Vertex Bulk data
	QList<float> m_VertexBulk;

	//! Normal Bulk data
	QList<float> m_NormalBulk;

	//! The current index
	GLuint m_CurrentIndex;
};

#endif /*GLC_STLTOWORLD_H_*/
