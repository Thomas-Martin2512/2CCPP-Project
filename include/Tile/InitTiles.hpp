#ifndef INITTILES_HPP_INCLUDED
#define INITTILES_HPP_INCLUDED

#include "Tile.hpp"
#include <string>
#include <vector>
#include <optional>


/**
 * @class InitTiles
 * @brief Gère l’initialisation et la gestion d’un ensemble de tuiles.
 *
 * Cette classe permet de :
 * - Charger des tuiles à partir d’un fichier JSON ;
 * - Accéder à l’ensemble complet ou à une tuile spécifique par ID ;
 * - Supprimer les doublons basés sur la forme des tuiles.
 */
class InitTiles {
    private:
        /** @brief Conteneur des tuiles chargées depuis le fichier JSON. */
        std::vector<Tile> tiles_;

    public:
        /** @brief Constructeur par défaut. */
        InitTiles() = default;

        /**
        * @brief Construit l’objet et charge immédiatement les tuiles depuis un fichier JSON.
        * @param jsonPath Chemin du fichier JSON contenant les définitions de tuiles.
        */
        explicit InitTiles(const std::string& jsonPath);

        /**
        * @brief Charge les tuiles depuis un fichier JSON.
        *
        * Le fichier doit avoir une structure similaire à :
        * @code{.json}
        * {
        *   "tiles": [
        *     { "id": "A", "cells": [[0,0], [1,0], [1,1]] },
        *     { "id": "B", "cells": [[0,0], [0,1]] }
        *   ]
        * }
        * @endcode
        *
        * @param jsonPath Chemin vers le fichier JSON à lire.
        * @return true si le chargement a réussi, false sinon.
        */
        bool loadFromFile(const std::string& jsonPath);

        /**
        * @brief Retourne la liste complète des tuiles chargées.
        * @return Référence constante vers le vecteur de tuiles.
        */
        const std::vector<Tile>& all() const;

        /**
        * @brief Recherche une tuile par identifiant.
        * @param id Identifiant de la tuile recherchée.
        * @return Une tuile si trouvée (`std::optional<Tile>`), sinon `std::nullopt`.
        */
        std::optional<Tile> byId(const std::string& id) const;


        /**
        * @brief Supprime les tuiles en double basées sur leur forme.
        *
        * Compare chaque tuile avec les autres via `Tile::shapeEquals()`
        * et conserve uniquement une occurrence de chaque forme unique.
        */
        void deduplicateByShape();
};

#endif // INITTILES_HPP_INCLUDED