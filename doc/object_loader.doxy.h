/**
 * @~english
 * @taomoduledescription{ObjectLoader, Object Loader}
 * Loads 3D models from files.
 *
 * With this module, you can import 3D models exported from many popular
 * 3D software packages.
 * Supported formats include:
 *
 * @~french
 * @taomoduledescription{ObjectLoader, Lecteur de modèles 3D}
 * Charge des fichiers 3D.
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
 * @code
import ObjectLoader 1.01

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
 * @endcode
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
 * Loads, translates and paints a 3D object.
 * Forces the object to
 * take the current color or material properties even if the model file defines
 * colors, materials or textures.
 * @~french
 * Charge un fichier 3D, le positionne, permet la couleur.
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
 * Loads, scales, translates and paints a 3D object.
 * Forces the object to
 * take the current color or material properties even if the model file defines
 * colors, materials or textures.
 * @~french
 * Charge un fichier 3D, le positionne, permet la couleur.
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
