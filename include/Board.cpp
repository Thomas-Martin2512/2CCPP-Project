#include "Board.h"
#include "string"
#include "iostream"

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