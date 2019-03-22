// *****************************************************************************
// glc_objmtlloader.h                                              Tao3D project
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
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011, Baptiste Soulisse <baptiste@geonosis.local>
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

//! \file glc_objmtlloader.h interface for the GLC_ObjMtlLoader class.

#ifndef GLC_OBJMTLLOADER_H_
#define GLC_OBJMTLLOADER_H_

#include <QString>
#include <QHash>
#include <QTextStream>
#include "../shading/glc_material.h"

#include "../glc_config.h"

class QGLContext;
//////////////////////////////////////////////////////////////////////
//! \class GLC_ObjMtlLoader
/*! \brief GLC_ObjMtlLoader : Load the mtl file associated to a OBJ File */

/*! An GLC_ObjMtlLoader create GLC_Material from the .mtl file \n
  */
//////////////////////////////////////////////////////////////////////
class GLC_LIB_EXPORT GLC_ObjMtlLoader
{

//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////
public:
	GLC_ObjMtlLoader(const QString&);

	virtual ~GLC_ObjMtlLoader();
//@}
//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Return true if the material name is found
	inline bool contains(const QString &name) const
	{return m_Materials.contains(name);}

	//! Get a material from is name
	GLC_Material* material(const QString&);

	//! Get the list of attached files
	inline QStringList listOfAttachedFileName() const
	{return m_ListOfAttachedFileName.toList();}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Load the materials
	bool loadMaterials();

//@}
//////////////////////////////////////////////////////////////////////
/*! \name Private services functions*/
//@{
//////////////////////////////////////////////////////////////////////
private:
	//! Extract the material name
	bool extractMaterialName(QString &);

	//! Extract the texture file name
	void extractTextureFileName(QString &);

	//! Extract RGB value
	bool extractRGBValue(QString &);

	//! Extract One value
	bool extractOneValue(QString &);

	//! Get texture file name without parameters
	QString getTextureName(QTextStream &, const QString &);

	//! Process Maya specific obj
	void processMayaSpecific();


//@}

//////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////
private:
	//! The mtl file name
	QString m_FileName;

	//! Current material
	GLC_Material* m_pCurrentMaterial;

	//! The GLC_Material Hash Table
	QHash<QString, GLC_Material*> m_Materials;

	//! the Load status
	QString m_LoadStatus;

	//! The list of attached file name
	QSet<QString> m_ListOfAttachedFileName;


};

#endif /*GLC_OBJMTLLOADER_H_*/
