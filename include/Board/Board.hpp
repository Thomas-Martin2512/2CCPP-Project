#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED
#include <vector>
#include <map>
#include <memory>
#include "Bonus/Bonus.hpp"
#include <iostream>
#include "../include/Display_Board/Display_Board.hpp"


class Board {

    private:
        int rows;
        int cols;
        std::vector<std::vector<char>> grid;
        std::vector<std::vector<int>> ownerGrid;
        std::map<std::pair<int,int>, std::shared_ptr<Bonus>> bonuses;

    public:
        Board();
        Board(int numberOfPlayers);

        void initGrid(int numberOfPlayers);
        void placeBonus(int numberOfPlayers);
        void displayGrid() const;
        void placeTile(int x, int y, int playerId);

        int getRows() const;
        int getCols() const;
        const std::vector<std::vector<char>>& getGrid() const;
        const std::vector<std::vector<int>>& getOwnerGrid() const;
        const std::map<std::pair<int,int>, std::shared_ptr<Bonus>>& getBonus() const;
};


#endif // BOARD_HPP_INCLUDED