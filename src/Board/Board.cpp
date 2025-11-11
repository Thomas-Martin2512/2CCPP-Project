#include "../../include/Board/Board.hpp"
#include "string"
#include "iostream"
#include "../../include/Bonus/ExchangeBonus.hpp"
#include "../../include/Bonus/StoneBonus.hpp"
#include "../../include/Bonus/SteelBonus.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>


Board::Board() : rows(0), cols(0) {}

Board::Board(int numberOfPlayers) {
    initGrid(numberOfPlayers);
}

void Board::initGrid(int numberOfPlayers) {
    if (numberOfPlayers >= 2 && numberOfPlayers <= 4) {
        rows = 20;
        cols = 20;
    } else if (numberOfPlayers >= 5 && numberOfPlayers <= 9) {
        rows = 30;
        cols = 30;
    } else {
        std::cerr << "Nombre de joueurs invalide !" << std::endl;
        rows = cols = 0;
    }

    grid = std::vector<std::vector<char>>(rows, std::vector<char>(cols, '.'));
}


void Board::placeBonus(int numberOfPlayers) {
    if (rows == 0 || cols == 0) return;

    srand(static_cast<unsigned>(time(nullptr)));

    int nbExchange = std::ceil(1.5 * numberOfPlayers);
    int nbStone = std::ceil(0.5 * numberOfPlayers);
    int nbSteal = numberOfPlayers;

    auto place = [&](std::shared_ptr<Bonus> bonus) {
        int x, y;
        bool valid = false;
        while (!valid) {
            x = rand() % (rows - 2) + 1;
            y = rand() % (cols - 2) + 1;
            valid = (grid[x][y] == '.') && (bonuses.find({x,y}) == bonuses.end());
            if (valid) {
                for (int dx = -1; dx <= 1; ++dx)
                    for (int dy = -1; dy <= 1; ++dy)
                        if (bonuses.find({x+dx, y+dy}) != bonuses.end())
                            valid = false;
            }
        }
        bonuses[{x,y}] = bonus;
    };

    for (int i = 0; i < nbExchange; ++i) place(std::make_shared<ExchangeBonus>());
    for (int i = 0; i < nbStone; ++i)   place(std::make_shared<StoneBonus>());
    for (int i = 0; i < nbSteal; ++i)   place(std::make_shared<StealBonus>());
}

const std::map<std::pair<int,int>, std::shared_ptr<Bonus>>& Board::getBonus() const {
    return bonuses;
}


void Board::displayGrid() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

int Board::getRows() const { return rows; }
int Board::getCols() const { return cols; }
const std::vector<std::vector<char>>& Board::getGrid() const { return grid; }