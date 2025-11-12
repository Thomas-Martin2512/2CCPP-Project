#ifndef EXCHANGEBONUS_HPP_INCLUDED
#define EXCHANGEBONUS_HPP_INCLUDED

#include <string>
#include "Bonus.hpp"

class ExchangeBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "E"; }
        std::string getName() const override { return "Exchange a tile"; }
};


#endif // EXCHANGEBONUS_HPP_INCLUDED