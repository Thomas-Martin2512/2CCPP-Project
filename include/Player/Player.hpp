#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
#include <string>

class Player {
    private:
        static int nextID;
        int id;
        std::string name;
        std::string color;
        int exchangeCoupons = 1;

    public:
        Player();
        Player(const std::string& playerName, const std::string& playerColor);

        void inputPlayerInfo(int playerNumber);
        void displayInfo() const;

        int getID() const;
        std::string getName() const;
        std::string getColor() const;
        void setName(const std::string& newName);
        void setColor(const std::string& newColor);

        int getExchangeCoupons() const { return exchangeCoupons; }
        void addExchangeCoupon() { exchangeCoupons++; }
        void useExchangeCoupon() { if (exchangeCoupons > 0) exchangeCoupons--; }
    };

#endif // PLAYER_HPP_INCLUDED
