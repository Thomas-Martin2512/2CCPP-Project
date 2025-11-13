#include "../../include/Board/Board.hpp"
#include "string"
#include "iostream"
#include "../../include/Bonus/ExchangeBonus.hpp"
#include "../../include/Bonus/StoneBonus.hpp"
#include "../../include/Bonus/SteelBonus.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "../../include/Player/Player.hpp"
#include "../../include/Bonus/Bonus.hpp"
#include "../../include/Game/Game.hpp"

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
    ownerGrid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
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



void Board::placeTile(int x, int y, int playerId) {
    if (x >= 0 && x < cols && y >= 0 && y < rows && grid[y][x] == '.') {
        grid[y][x] = '#';
        ownerGrid[y][x] = playerId;
    }
}

int Board::letterToCol(const std::string& letter) {
    if (letter.empty()) return -1;
    char c = toupper(letter[0]);
    if (c < 'A' || c > 'Z') return -1;
    return c - 'A';
}

void Board::checkBonusCapture(int playerId, Game& game) {
    static const std::vector<std::pair<int,int>> directions = {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    std::vector<std::pair<int,int>> capturedKeys;

    for (auto& [pos, bonusPtr] : bonuses) {
        int bx = pos.first;
        int by = pos.second;

        bool surrounded = true;
        for (auto [dx, dy] : directions) {
            int nx = bx + dx;
            int ny = by + dy;

            if (nx < 0 || ny < 0 || nx >= cols || ny >= rows || ownerGrid[ny][nx] != playerId) {
                surrounded = false;
                break;
            }
        }

        if (surrounded) {
            ownerGrid[by][bx] = playerId;
            std::cout << "Bonus captured by player " << playerId
                      << " : " << bonusPtr->getName() << std::endl;

            Player& player = game.getPlayerById(playerId);

            if (bonusPtr->getSymbol() == "E") {
                player.addExchangeCoupon();
                std::cout << "Ticket d'échange +1. Total : "
                          << player.getExchangeCoupons() << "\n";
            }

            if (bonusPtr->getSymbol() == "R") {
                player.setRockBonusAvailable(true);
                std::cout << "Rock bonus : available\n";

                std::cout << "Player " << playerId
                          << " can now place a stone (1x1 'X') on an empty cell.\n";

                int px, py;
                bool valid = false;
                while (!valid) {
                    std::string col;
                    std::cout << "Enter stone position (column letter, row number) : ";
                    std::cin >> col >> py;
                    px = letterToCol(col);
                    py--;

                    if (px >= 0 && px < cols && py >= 0 && py < rows && grid[py][px] == '.') {
                        grid[py][px] = 'X';
                        ownerGrid[py][px] = 0;
                        valid = true;
                        std::cout << "Stone placed at " << col << py+1 << "\n";
                    } else {
                        std::cout << "Invalid or occupied cell, try again.\n";
                    }
                }

                player.setRockBonusAvailable(false);
            }

            if (bonusPtr->getSymbol() == "S") {
                player.setStealthBonusAvailable(true);
                std::cout << "Stealth bonus : available.\n";
            }

            capturedKeys.push_back(pos);
        }
    }

    for (auto& key : capturedKeys) {
        bonuses.erase(key);
    }
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

bool Board::canPlaceFootprint(const std::vector<std::pair<int,int>>& pts, int /*playerId*/) const {
    for (auto [x,y] : pts) {
        if (x < 0 || x >= cols || y < 0 || y >= rows) return false;
        if (grid[y][x] != '.') return false;  // occupé
    }
    return true;
}

bool Board::placeFootprint(const std::vector<std::pair<int,int>>& pts, int playerId) {
    if (!canPlaceFootprint(pts, playerId)) return false;
    for (auto [x,y] : pts) {
        grid[y][x] = '#';
        ownerGrid[y][x] = playerId;
    }
    return true;
}