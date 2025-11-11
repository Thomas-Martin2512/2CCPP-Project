#ifndef DISPLAY_BOARD_HPP_INCLUDED
#define DISPLAY_BOARD_HPP_INCLUDED
#include "../Board/Board.hpp"
#include <iostream>

class Display_Board {
    public :
        explicit Display_Board(const Board& board);

        void display() const;

    private:
        const Board& board;
        void printTopBoarder(int cols) const;
        void printBottomBoarder(int cols) const;
};


#endif // DISPLAY_BOARD_HPP_INCLUDED