/**
* @file Tile.cpp
 * @brief Implémentation de la classe Tile (tuile de jeu) : construction,
 *        accès aux propriétés, transformations (rotation/miroir), normalisation
 *        et calcul d’empreinte sur le plateau.
 */

#include "../../include/Tile/Tile.hpp"
#include <algorithm>
#include <unordered_set>


/**
 * @brief Construit une tuile et la normalise immédiatement à l’origine.
 * @param id Identifiant unique.
 * @param cells Coordonnées relatives des cases formant la tuile.
 * @param color Couleur de la tuile.
 */
Tile::Tile(std::string id, std::vector<std::pair<int,int>> cells, std::string color)
    : id_(std::move(id)), color_(std::move(color)), cells_(std::move(cells))
{
    normalizeToOrigin();
}

/**
 * @brief Retourne l’identifiant unique de la tuile.
 * @return Référence constante vers l’identifiant.
 */
const std::string& Tile::getId() const {
    return id_;
}

/**
 * @brief Retourne la couleur de la tuile.
 * @return Référence constante vers la couleur.
 */
const std::string &Tile::getColor() const {
    return color_;
}

/**
 * @brief Retourne la liste des cases (coordonnées relatives) qui composent la tuile.
 * @return Référence constante vers le vecteur de paires (x, y).
 */
const std::vector<std::pair<int,int>>& Tile::getCells() const {
    return cells_;
}

/**
 * @brief Modifie l’identifiant de la tuile.
 * @param newId Nouvel identifiant.
 */
void Tile::setId(std::string &newId) {
    id_ = newId;
}

/**
 * @brief Modifie la couleur de la tuile.
 * @param newColor Nouvelle couleur.
 */
void Tile::setColor(std::string &newColor) {
    color_ = newColor;
}


/**
 * @brief Normalise un ensemble de points par translation vers (0, 0) puis dédoublonnage.
 *
 * Recherche le minimum de x et y, translate tous les points pour que le coin
 * haut-gauche soit à l’origine, trie puis supprime les doublons.
 *
 * @param pts Vecteur de points à normaliser (modifié en place).
 */
void Tile::normalize(std::vector<std::pair<int, int>>& pts) {
    if (pts.empty()) return;
    int minX = pts[0].first, minY = pts[0].second;
    for (auto& p : pts) {
        if (p.first < minX) minX = p.first;
        if (p.second < minY) minY = p.second;
    }
    for (auto& p : pts) {
        p.first -= minX;
        p.second -= minY;
    }
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
}

/**
 * @brief Normalise la forme interne de la tuile pour l’aligner sur l’origine.
 */
void Tile::normalizeToOrigin() {
    normalize(cells_);
}

/**
 * @brief Calcule les dimensions englobantes (boîte) de la tuile normalisée.
 * @return (largeur, hauteur) = (maxX + 1, maxY + 1). Retourne (0, 0) si vide.
 */
std::pair<int, int> Tile::boxLimit() const {
    if (cells_.empty()) return {0, 0};
    int maxX = cells_[0].first, maxY = cells_[0].second;
    for (auto& p : cells_) {
        if (p.first > maxX) maxX = p.first;
        if (p.second > maxY) maxY = p.second;
    }
    return {maxX + 1, maxY + 1};
}

/**
 * @brief Effectue une rotation antihoraire de 90° sur la tuile.
 *
 * La transformation utilisée est (x, y) -> (y, -x), suivie d’une normalisation.
 * Modifie la forme interne (cells_).
 */
void Tile::rotate() {
    std::vector<std::pair<int,int>> out;
    out.reserve(cells_.size());
    for (auto [x, y] : cells_) {
        out.emplace_back(y, -x);
    }
    normalize(out);
    cells_.swap(out);
}

/**
 * @brief Applique une symétrie miroir horizontale (miroir vertical visuel).
 *
 * La transformation utilisée est (x, y) -> (-x, y), suivie d’une normalisation.
 * Modifie la forme interne (cells_).
 */
void Tile::flip() {
    std::vector<std::pair<int,int>> out;
    out.reserve(cells_.size());
    for (auto [x, y] : cells_) {
        out.emplace_back(-x, y);
    }
    normalize(out);
    cells_.swap(out);
}

/**
 * @brief Calcule l’empreinte de la tuile sur le plateau à partir d’une origine.
 *
 * Applique optionnellement une symétrie, puis 0–3 rotations de 90° (antihoraire),
 * normalise après chaque transformation pour conserver un ancrage cohérent, et
 * translate finalement tous les points de (originX, originY).
 *
 * @param originX Abscisse d’origine (translation finale).
 * @param originY Ordonnée d’origine (translation finale).
 * @param rotations Nombre de rotations de 90° à appliquer (peut être négatif, ramené à [0..3]).
 * @param flipped Si true, applique d’abord un miroir horizontal.
 * @return Vecteur de coordonnées absolues (x, y) après transformations.
 */
std::vector<std::pair<int, int> > Tile::footprint(int originX, int originY, int rotations, bool flipped) const {
    std::vector<std::pair<int,int>> pts = cells_;
    if (flipped) {
        for (auto& p : pts) {
            p.first = -p.first;
        }
        normalize(pts);
    }
    rotations = ((rotations % 4) + 4) % 4;
    for (int k = 0; k < rotations; ++k) {
        for (auto& p : pts) {
            int x = p.first, y = p.second;
            p = { y, -x };
        }
        normalize(pts);
    }
    for (auto& p : pts) {
        p.first  += originX;
        p.second += originY;
    }
    return pts;
}

/**
 * @brief Teste l’égalité de forme entre deux tuiles.
 *
 * @param other Tuile à comparer.
 * @return true si les deux tuiles ont exactement les mêmes points (même taille
 *         et mêmes coordonnées, supposées normalisées), false sinon.
 */
bool Tile::shapeEquals(const Tile& other) const {
    if (cells_.size() != other.cells_.size()) return false;
    return std::equal(cells_.begin(), cells_.end(), other.cells_.begin());
}
