#ifndef INC_2CCPP_PROJECT_STONEBONUS_H
#define INC_2CCPP_PROJECT_STONEBONUS_H

#include "Bonus.hpp"

class StoneBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "R"; }
        std::string getName() const override { return "Rock"; }
};



#endif