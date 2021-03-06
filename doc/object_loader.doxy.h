// *****************************************************************************
// object_loader.doxy.h                                            Tao3D project
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
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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
/**
 * @~english
 * @taomoduledescription{ObjectLoader, Object Loader}
 * <tt>import ObjectLoader</tt> - Loads 3D models from files.@n
 *
 * With this module, you can import 3D models exported from many popular
 * 3D software packages.
 * Supported formats include:
 *
 * @~french
 * @taomoduledescription{ObjectLoader, Lecteur de modèles 3D}
 * <tt>import ObjectLoader</tt> - Charge des fichiers 3D.@n
 *
 * Ce module vous permet d'afficher des modèles 3D exportés par de nombreux
 * logiciels. Les formats supportés sont :
 *
 * @~
 * @li OBJ (*.obj)
 * @li 3DS (*.3ds)
 * @li Collada V1.4 (*.dae)
 * @li 3DXML ASCII V3 and V4 (*.3dxml)
 * @li STL (ASCII and binary) (*.stl)
 * @li OFF and COFF (*.off)
 *
 * @~english
 * For instance, the following Tao document will load a 3D model from
 * file @c conestoga.3ds, and show it rotating around the z axis.
 * @~french
 * Par exemple, le document qui suit charge le fichier @c conestoga.3ds
 * et le fait tourner autour de l'axe Z.
 * @~
@code
import ObjectLoader

the_light X, Y, Z ->
    light 0
    light_ambient 0.4, 0.4, 0.4, 1.0
    light_diffuse 0.4, 0.4, 0.4, 1.0
    light_specular 0.8, 0.8, 0.8, 1.0
    light_attenuation 1.0, 0.0, 0.0
    light_position X, Y, Z

rotating_obj F ->
    locally
        rotatex -70
        rotatez 8 * time
        object 0, 0, 0, 400, 400, 400, F

the_light 300, 300, 300
rotating_obj "conestoga.3ds"
@endcode
 *
 * @~english
 * Here is a screen capture:
 * @image html conestoga.png "(3D model by Helmut Schaub &mdash; http://www.3dxtra.net, http://www.3d-xtra.bravepages.com)"
 * @endtaomoduledescription{ObjectLoader}
 *
 * @~french
 * Voici une capture d'écran :
 * @image html conestoga.png "(Modèle 3D par Helmut Schaub &mdash; http://www.3dxtra.net, http://www.3d-xtra.bravepages.com)"
 * @endtaomoduledescription{ObjectLoader}
 *
 * @~
 * @{
 */

/**
 * @~english
 * Loads a 3D object from a file.
 * @p file is a relative or absolute path to the model file.
 * No scaling or translation occurs: the model is drawn as defined in the file.
 * @~french
 * Charge un fichier 3D.
 * @p file est un chemin relatif ou absolu vers le fichier.
 * Aucune translation ou changement d'échelle n'est fait. Le modèle 3D est
 * affiché tel que défini dans le fichier.
 */
object(text file);

/**
 * @~english
 * Loads a 3D object from a file and shows it with a user-define color
 * @p file is a relative or absolute path to the model file.
 * No scaling or translation occurs: the model is drawn as defined in the file.
 * @~french
 * Charge un fichier 3D avec une couleur définie par l'utilisateur
 * @p file est un chemin relatif ou absolu vers le fichier.
 * Aucune translation ou changement d'échelle n'est fait. Le modèle 3D est
 * affiché tel que défini dans le fichier.
 */
colored_object(text file);

/**
 * @~english
 * Loads and translates a 3D object.
 * @p file is a relative or absolute path to the model file.
 * The object is shown centered at (@a x, @a y, @a z).
 * No scaling occurs: the object dimensions are as defined in the model file.
 * @~french
 * Charge un fichier 3D et le positionne.
 * @p file est un chemin relatif ou absolu vers le fichier.
 * Le modèle est centré en (@a x, @a y, @a z).
 * Aucune translation ou changement d'échelle n'est fait. Le modèle 3D est
 * affiché tel que défini dans le fichier.
 */
object(real x, real y, real z, text file);


/**
 * @~english
 * Loads, scales and translates a 3D object.
 * @p file is a relative or absolute path to the model file.
 * The object is shown centered at (@a x, @a y, @a z).
 * The size of the object is specified by @a width, @a height and @a depth:
 * the model is scaled proportionally so that it fits in a box of the
 * specified size.
 * @~french
 * Charge un fichier 3D, le redimensionne et le positionne.
 * @p file is a relative or absolute path to the model file.
 * L'objet est centré en (@a x, @a y, @a z).
 * La taille de l'objet est définie par @a width, @a height and @a depth :
 * le modèle est agrandi ou réduit proportionnellement de sorte qu'il ne
 * dépasse pas des dimensions spécifiées.
 */
object(real x, real y, real z, real width, real height, real depth, text file);

/**
 * @~english
 * Loads, translates and paints a 3D object with user-defined colors.
 * Forces the object to take the current color or material properties
 * even if the model file defines colors, materials or textures.
 * @~french
 * Charge et positionne un fichier 3D avec une couleur définie par l'utilisateur
 * Force l'utilisation des couleurs et textures activées avant l'utilisation
 * de cette primitive, même si le fichier modèle définit ses propres couleurs
 * ou textures.
 * @~
 * @copydetails object(real x, real y, real z, text file)
 * @since 1.01
 */
colored_object(real x, real y, real z, text file);


/**
 * @~english
 * Loads, scales, translates and paints a 3D object with a user-defined color
 * Forces the object to
 * take the current color or material properties even if the model file defines
 * colors, materials or textures.
 * @~french
 * Charge et affiche un fichier 3D dont la couleur est définie par l'utilisateur
 * Force l'utilisation des couleurs et textures activées avant l'utilisation
 * de cette primitive, même si le fichier modèle définit ses propres couleurs
 * ou textures.
 * @~
 * @copydetails object(real x, real y, real z, real width, real height, real depth, text file)
 * @since 1.01
 */
colored_object(real x, real y, real z, real width, real height, real depth, text file);


/**
 * @}
 */
