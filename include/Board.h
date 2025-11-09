#ifndef INC_2CCPP_PROJECT_BOARD_H
#define INC_2CCPP_PROJECT_BOARD_H
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


#endif