#include "../../include/Display_Board/Display_Board.hpp"
#include "../../include/Game/Game.hpp"
#include <iostream>
#include <iomanip>

Display_Board::Display_Board(const Board &board) : board(board) {}

void Display_Board::printTopBoarder(int cols) const {
    std::cout << "  +";
    for (int j = 0; j < cols; ++j)
        std::cout << "--";
    std::cout << "+" << std::endl;
}


void Display_Board::printBottomBoarder(int cols) const {
    printTopBoarder(cols);
}

void Display_Board::display(const Game& game) const {
    int rows = board.getRows();
    int cols = board.getCols();
    const auto& grid = board.getGrid();
    const auto& bonuses = board.getBonus();
    const auto& ownerGrid = board.getOwnerGrid();

    if (rows == 0 || cols == 0) {
        std::cerr << "Error : empty grid or non-initialized." << std::endl;
        return;
    }

    std::cout << std::endl;
    std::cout << "   === Game board ===" << std::endl << std::endl;

    printTopBoarder(cols);

    for (int i = 0; i < rows; ++i) {
        std::cout << std::setw(2) << std::setfill(' ') << '|' << " ";

        for (int j = 0; j < cols; ++j) {
            auto it = bonuses.find({i, j});

            if (it != bonuses.end()) {
                std::cout << it->second->getSymbol() << ' ';
            } else {
                char cell = grid[i][j];

                if (cell == '#') {
                    int ownerId = ownerGrid[i][j];
                    if (ownerId > 0 && ownerId <= game.getPlayers().size()) {
                        std::string color = game.getPlayers()[ownerId - 1].getColor();
                        std::string ansi = game.getAnsiColor(color);
                        std::cout << ansi << "#" << "\033[0m" << ' ';
                    } else {
                        std::cout << "#" << ' ';
                    }
                } else {
                    std::cout << cell << ' ';
                }
            }
        }

        std::cout << '|' << std::endl;

        std::cout << "  |";
        for (int j = 0; j < cols; ++j)
            std::cout << "  ";
        std::cout << '|' << std::endl;
    }

    printBottomBoarder(cols);
    std::cout << std::endl;
}
