#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED
#include <vector>

class Board {

    private:
        int rows;
        int cols;
        std::vector<std::vector<char>> grid;

    public:
        Board();
        Board(int numberOfPlayers);

        void initGrid(int numberOfPlayers);
        void displayGrid() const;

        int getRows() const;
        int getCols() const;
        const std::vector<std::vector<char>>& getGrid() const;

};


#endif // BOARD_HPP_INCLUDED