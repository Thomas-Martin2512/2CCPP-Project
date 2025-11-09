#include "Display_Board.h"
#include <iostream>

Display_Board::Display_Board(const Board &board) : board(board) {}

void Display_Board::display() const {
    int rows = board.getRows();
    int cols = board.getCols();
    const std::vector<std::vector<char>>& grid = board.getGrid();

    if (rows == 0 || cols == 0) {
        std::cerr << "Erreur : la grille est vide ou non initialisée." << std::endl;
        return;
    }
    std::cout << '+';
    for (int j = 0; j < cols; ++j)
        std::cout << '-';
    std::cout << '+' << std::endl;

    for (int i = 0; i < rows; ++i) {
        std::cout << '|';
        for (int j = 0; j < cols; ++j) {
            char cell = grid[i][j];
            if (cell == '.')
                std::cout << '.';
            else
                std::cout << '#';
        }
        std::cout << '|' << std::endl;
    }


    std::cout << '+';
    for (int j = 0; j < cols; ++j)
        std::cout << '-';
    std::cout << '+' << std::endl;
}

