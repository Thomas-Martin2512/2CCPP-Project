#ifndef STONEBONUS_HPP_INCLUDED
#define STONEBONUS_HPP_INCLUDED

#include "Bonus.hpp"



/**
 * @class StoneBonus
 * @brief Bonus permettant de placer une pierre (blocage 1x1) sur le plateau.
 *
 * Symbole : 'R'
 * Effet :
 *  - Le joueur gagne un Rock bonus.
 *  - Au début de son prochain tour, il DOIT placer une pierre ('X')
 *    sur n’importe quelle case vide du plateau.
 *
 * Une pierre :
 *  - bloque définitivement la case,
 *  - ne compte pour aucun joueur (ownerGrid = 0),
 *  - peut empêcher un adversaire de se développer.
 */
class StoneBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "R"; }
        std::string getName() const override { return "Rock"; }
};



#endif // STONEBONUS_HPP_INCLUDED