#ifndef DISPLAY_BOARD_HPP_INCLUDED
#define DISPLAY_BOARD_HPP_INCLUDED
#include "../Board/Board.hpp"

class Display_Board {
    public :
        Display_Board(const Board& board);

        void display() const;

    private:
        const Board& board;
};


#endif // DISPLAY_BOARD_HPP_INCLUDED