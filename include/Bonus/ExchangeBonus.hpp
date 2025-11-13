#ifndef EXCHANGEBONUS_HPP_INCLUDED
#define EXCHANGEBONUS_HPP_INCLUDED

#include <string>
#include "Bonus.hpp"



/**
 * @class ExchangeBonus
 * @brief Bonus permettant au joueur de gagner un ticket d’échange.
 *
 * Symbole : 'E'
 * Effet : le joueur reçoit +1 ticket d’échange, qu’il peut utiliser pour
 *         remplacer la tuile piochée avec une tuile aperçue dans la fenêtre.
 */
class ExchangeBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "E"; }
        std::string getName() const override { return "Exchange a tile"; }
};


#endif // EXCHANGEBONUS_HPP_INCLUDED