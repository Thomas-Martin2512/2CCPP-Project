#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <string>
#include "Tile/Tile.hpp"

/**
 * @namespace render
 * @brief Espace de noms regroupant les fonctions d’affichage textuel des tuiles.
 *
 * Ce module permet :
 *  - d'afficher une tuile brute sous forme d’un petit schéma en ASCII ;
 *  - d'afficher une empreinte (footprint) positionnée sur une grille fictive,
 *    selon une origine, une rotation et un flip ;
 *
 * Il s'agit d'un outil de debug important pour visualiser facilement
 * la géométrie d'une tuile.
 */
namespace render {

    /**
 * @brief Génère une représentation ASCII compacte d’une tuile.
 *
 * Exemple :
 * ```
 * T1 (3x2)
 * ##.
 * .##
 * ```
 *
 * La taille affichée correspond à la bounding-box minimale qui contient la tuile.
 *
 * @param t La tuile à afficher.
 * @param fill Le caractère utilisé pour représenter une cellule de la tuile.
 * @return Une chaîne textuelle contenant la représentation ASCII.
 */
    std::string drawTile(const Tile& t, char fill = '#');

    /**
 * @brief Affiche la projection (footprint) d’une tuile sur une grille virtuelle.
 *
 * Cette fonction permet d’afficher comment la tuile apparaîtrait :
 *  - positionnée à (originX, originY),
 *  - après application éventuelle de rotations et flip,
 *  - dans une grille de taille `width` x `height`.
 *
 * Très utile pour tester manuellement les transformations géométriques.
 *
 * @param t La tuile.
 * @param originX Position X sur la grille simulée.
 * @param originY Position Y sur la grille simulée.
 * @param rotations Nombre de rotations (0 à 3).
 * @param flipped true si la tuile est retournée horizontalement.
 * @param width Largeur de la grille virtuelle.
 * @param height Hauteur de la grille virtuelle.
 * @param fill Caractère utilisé pour dessiner la tuile.
 * @return Une représentation textuelle de la tuile appliquée sur la grille.
 */
    std::string drawFootprint(const Tile& t,
                              int originX, int originY,
                              int rotations = 0, bool flipped = false,
                              int width = 10, int height = 10,
                              char fill = '#');

}

#endif // RENDERER_HPP_INCLUDED