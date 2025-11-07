#ifndef INC_2CCPP_PROJECT_PLAYER_H
#define INC_2CCPP_PROJECT_PLAYER_H
#include "string"


class Player {
    private:
        std :: string name;
        std :: string color;

    public:
        Player();
        Player(const std::string& playerName, const std::string& playerColor);


        void inputPlayerInfo(int playerNumber);
        void displayInfo() const;

        std::string getName() const;
        std::string getColor() const;
        void setName(const std::string& newName);
        void setColor(const std::string& newColor);
};

#endif