#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED
#include <vector>
#include <map>
#include <memory>
#include "Bonus/Bonus.hpp"


class Board {

    private:
        int rows;
        int cols;
        std::vector<std::vector<char>> grid;
        std::map<std::pair<int,int>, std::shared_ptr<Bonus>> bonuses;
        std::vector<std::vector<int>> ownerGrid;

    public:
        Board();
        Board(int numberOfPlayers);

        void initGrid(int numberOfPlayers);
        void placeBonus(int numberOfPlayers);
        void displayGrid() const;
        void placeTile(int x, int y, int playerId);
        void checkBonusCapture(int x, int y, int playerId);


        int getRows() const;
        int getCols() const;
        const std::vector<std::vector<char>>& getGrid() const;
        const std::vector<std::vector<int>>& getOwnerGrid() const { return ownerGrid; };
        const std::map<std::pair<int,int>, std::shared_ptr<Bonus>>& getBonus() const;
        bool canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) const;
        bool placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId);
};


#endif // BOARD_HPP_INCLUDED