#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "Board/Board.hpp"
#include "Display_Board/Display_Board.hpp"
#include "Player/Player.hpp"
#include "Tile/InitTiles.hpp"
#include "Tile/TileQueue.hpp"
#include <vector>
#include <random>
#include <string>

class Display_Board;

class Game {
private:
    std::vector<Player> players;
    Board board;
    Display_Board* display;
    InitTiles tileSet;
    TileQueue queue;
    int currentRound;
    int currentPlayerIndex;
    bool gameOver;

public:
    Game();
    ~Game();

    void start();
    void placeStartingTiles();

    const std::vector<Player>& getPlayers() const { return players; }
    std::string getAnsiColor(const std::string& colorName) const;

private:
    void setupPlayers();
    void setupBoard();
    void setupTiles();

    void runRounds(int maxRounds = 9);
    void playTurn(Player& player);
    void endRound();
    bool isGameOver() const;

    void showQueueWithCurrent(const Tile& current) const;
    void promptTransform(Tile& current) const;
    bool promptExchange(Tile& current) ;
    bool promptPlace(Tile& current, int playerId);

    static int readIntInRange(const std::string& prompt, int minVal, int maxVal);
    static bool readYesNo(const std::string& prompt);
    static char readChoice(const std::string& prompt, const std::string& allowed); // ex "pefrq"

    bool canPlaceFootprint(const std::vector<std::pair<int,int>>& pts) const;
    void placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId);

    void displayBoard() const;
    void announceOrder() const;
};



#endif // GAME_HPP_INCLUDED