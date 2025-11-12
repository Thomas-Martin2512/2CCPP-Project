#ifndef DISPLAY_BOARD_HPP_INCLUDED
#define DISPLAY_BOARD_HPP_INCLUDED
#include "../Board/Board.hpp"
#include <iostream>
#include "Game/Game.hpp"

class Display_Board {
    public :
        explicit Display_Board(const Board& board);

        void display(const Game& game) const;

    private:
        const Board& board;
        void printTopBoarder(int cols) const;
        void printBottomBoarder(int cols) const;
};


#endif // DISPLAY_BOARD_HPP_INCLUDED