#ifndef INC_2CCPP_PROJECT_STEELBONUS_H
#define INC_2CCPP_PROJECT_STEELBONUS_H

#include "Bonus.hpp"

class StealBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "S"; }
        std::string getName() const override { return "Stealth"; }
};


#endif