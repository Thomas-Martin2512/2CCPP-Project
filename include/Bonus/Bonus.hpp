#ifndef INC_2CCPP_PROJECT_BONUS_H
#define INC_2CCPP_PROJECT_BONUS_H

#include <string>

class Bonus {
    public:
        virtual ~Bonus() = default;
        virtual std::string getSymbol() const = 0;
        virtual std::string getName() const = 0;
};

#endif