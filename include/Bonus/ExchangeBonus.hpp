#ifndef INC_2CCPP_PROJECT_EXCHANGEBONUS_H
#define INC_2CCPP_PROJECT_EXCHANGEBONUS_H

#include <string>
#include "Bonus.hpp"

class ExchangeBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "E"; }
        std::string getName() const override { return "Echange de tuiles"; }
};


#endif