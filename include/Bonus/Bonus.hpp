#ifndef BONUS_HPP_INCLUDED
#define BONUS_HPP_INCLUDED

#include <string>

/**
 * @class Bonus
 * @brief Classe abstraite représentant un bonus placé sur le plateau.
 *
 * Chaque bonus possède :
 * - un symbole (caractère lisible dans la grille : 'E', 'R', 'S', etc.)
 * - un nom descriptif (ex : "Exchange a tile", "Rock", ...)
 *
 * Cette classe sert uniquement d’interface. Les bonus concrets héritent de cette classe
 * et fournissent leur symbole et leur nom.
 */
class Bonus {
    public:
        virtual ~Bonus() = default;

        /**
         * @brief Retourne le symbole du bonus dans la grille.
         *
         * Ce symbole est utilisé lors du rendu du plateau et pour identifier
         * le type de bonus lors d'une capture.
         *
         * @return Symbole (ex. "E", "R", "S").
         */
        virtual std::string getSymbol() const = 0;

        /**
         * @brief Nom complet du bonus, destiné aux messages du jeu.
         *
         * @return Nom descriptif du bonus.
         */
        virtual std::string getName() const = 0;
};

#endif // BONUS_HPP_INCLUDED