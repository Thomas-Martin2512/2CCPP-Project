#ifndef STEELBONUS_HPP_INCLUDED
#define STEELBONUS_HPP_INCLUDED

#include "Bonus.hpp"



/**
 * @class StealBonus
 * @brief Bonus offrant un effet furtif/vol selon les règles de ton jeu.
 *
 * Symbole : 'S'
 * Effet : dépend de ton implémentation (furtivité, vol, action spéciale…)
 *
 * Note :
 * Ce bonus est détecté lorsque le joueur entoure la case bonus.
 */
class StealBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "S"; }
        std::string getName() const override { return "Stealth"; }
};


#endif // STEELBONUS_HPP_INCLUDED