/**
* @file InitTiles.cpp
 * @brief Implémentation de la classe InitTiles — chargement et gestion d’un ensemble de tuiles.
 */

#include "../../include/Tile/InitTiles.hpp"
#include <fstream>
#include <unordered_set>
#include <nlohmann/json.hpp>

using nlohmann::json;

/**
 * @brief Constructeur qui charge immédiatement les tuiles depuis un fichier JSON.
 * @param jsonPath Chemin vers le fichier JSON contenant les tuiles.
 */
InitTiles::InitTiles(const std::string& jsonPath) {
    loadFromFile(jsonPath);
}

/**
 * @brief Charge les tuiles depuis un fichier JSON.
 *
 * Vérifie la présence d’un tableau `"tiles"` et crée un objet `Tile`
 * pour chaque entrée valide.
 *
 * Exemple de structure JSON attendue :
 * @code{.json}
 * {
 *   "tiles": [
 *     { "id": "T1", "cells": [[0,0],[1,0],[0,1]] },
 *     { "id": "T2", "cells": [[0,0],[1,0]] }
 *   ]
 * }
 * @endcode
 *
 * @param jsonPath Chemin du fichier JSON à ouvrir.
 * @return true si le chargement s’est effectué avec succès, false sinon.
 */
bool InitTiles::loadFromFile(const std::string& jsonPath) {
    std::ifstream f(jsonPath);
    if (!f) return false;
    json j;
    f >> j;
    tiles_.clear();
    if (!j.contains("tiles") || !j["tiles"].is_array()) return false;
    for (const auto& tile : j["tiles"]) {
        if (!tile.contains("id") || !tile.contains("cells")) continue;
        std::string id = tile["id"].get<std::string>();
        std::vector<std::pair<int, int>> cells;
        for (const auto& c : tile["cells"]) {
            if (!c.is_array() || c.size() != 2) continue;
            int x = c[0].get<int>();
            int y = c[1].get<int>();
            cells.emplace_back(x,y);
        }
        tiles_.emplace_back(id, std::move(cells));
    }
    return true;
}


/**
 * @brief Retourne la liste complète des tuiles chargées.
 * @return Référence constante vers le vecteur interne `tiles_`.
 */
const std::vector<Tile>& InitTiles::all() const {
    return tiles_;
}

/**
 * @brief Recherche une tuile spécifique à partir de son identifiant.
 * @param id Identifiant de la tuile recherchée.
 * @return Une tuile optionnelle (`std::optional<Tile>`). Vide si l’ID n’existe pas.
 */
std::optional<Tile> InitTiles::byId(const std::string& id) const {
    for (const auto& tile : tiles_) {
        if (tile.getId() == id) return tile;
    }
    return std::nullopt;
}

/**
 * @brief Supprime les tuiles ayant la même forme géométrique.
 *
 * Compare chaque tuile avec les précédentes à l’aide de `Tile::shapeEquals()`.
 * Si une forme identique est trouvée, la nouvelle tuile est ignorée.
 */
void InitTiles::deduplicateByShape() {
    std::vector<Tile> unique;
    for (const auto& t : tiles_) {
        bool already = false;
        for (const auto& u : unique) {
            if (t.shapeEquals(u)) { already = true; break; }
        }
        if (!already) unique.push_back(t);
    }
    tiles_.swap(unique);
}
