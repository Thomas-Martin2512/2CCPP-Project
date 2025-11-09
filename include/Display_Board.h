#ifndef INC_2CCPP_PROJECT_DISPLAY_BOARD_H
#define INC_2CCPP_PROJECT_DISPLAY_BOARD_H
#include "Board.h"

class Display_Board {
    public :
        Display_Board(const Board& board);

        void display() const;

    private:
        const Board& board;
};


#endif