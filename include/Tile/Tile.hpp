#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <string>
#include <vector>

class Tile {
    protected:
        std::string type;
        std::string color;
        std::vector<int> x, y;

};

#endif // TILE_HPP_INCLUDED