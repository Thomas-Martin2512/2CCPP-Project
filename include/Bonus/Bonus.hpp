#ifndef BONUS_HPP_INCLUDED
#define BONUS_HPP_INCLUDED

#include <string>

class Bonus {
    public:
        virtual ~Bonus() = default;
        virtual std::string getSymbol() const = 0;
        virtual std::string getName() const = 0;
};

#endif // BONUS_HPP_INCLUDED