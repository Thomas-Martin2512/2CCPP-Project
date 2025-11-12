#ifndef CURSORCONTROLLER_HPP_INCLUDED
#define CURSORCONTROLLER_HPP_INCLUDED

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



#endif // CURSORCONTROLLER_HPP_INCLUDED