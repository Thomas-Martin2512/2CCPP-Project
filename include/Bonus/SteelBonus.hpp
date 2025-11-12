#ifndef STEELBONUS_HPP_INCLUDED
#define STEELBONUS_HPP_INCLUDED

#include "Bonus.hpp"

class StealBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "S"; }
        std::string getName() const override { return "Stealth"; }
};


#endif // STEELBONUS_HPP_INCLUDED