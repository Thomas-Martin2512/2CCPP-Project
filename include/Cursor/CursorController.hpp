#ifndef INC_2CCPP_PROJECT_CURSORCONTROLLER_H
#define INC_2CCPP_PROJECT_CURSORCONTROLLER_H

#include "Board/Board.hpp"
#include "Game/Game.hpp"

class CursorController {
    private:
        Board& board;
        Game& game;
        int cursorX;
        int cursorY;

    public:
        CursorController(Board& b, Game& g);

        void moveAndPlaceTile(int playerId);

    private:
        void drawBoard(int playerId) const;
        int getArrowKey() const;
    };



#endif