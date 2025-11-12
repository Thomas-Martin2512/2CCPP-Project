#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "Board/Board.hpp"
#include "Display_Board/Display_Board.hpp"
#include "Player/Player.hpp"
#include <vector>
#include <random>

class Game {
private:
    std::vector<Player> players;
    Board board;
    Display_Board* display;
    int currentRound;

public:
    Game();
    void start();

private:
    void setupPlayers();
    void setupBoard();
    void placeStartingTiles();
    void displayBoard() const;
    void shufflePlayerOrder();
    std::string getAnsiColor(const std::string& colorName) const;
};

#endif // GAME_HPP_INCLUDED