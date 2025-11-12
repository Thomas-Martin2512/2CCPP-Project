#ifndef STONEBONUS_HPP_INCLUDED
#define STONEBONUS_HPP_INCLUDED

#include "Bonus.hpp"

class StoneBonus : public Bonus {
    public:
        std::string getSymbol() const override { return "R"; }
        std::string getName() const override { return "Rock"; }
};



#endif // STONEBONUS_HPP_INCLUDED