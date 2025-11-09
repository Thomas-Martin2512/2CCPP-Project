#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <string>
#include "Tile/Tile.hpp"

namespace render {

    std::string drawTile(const Tile& t, char fill = '#');

    std::string drawFootprint(const Tile& t,
                              int originX, int originY,
                              int rotations = 0, bool flipped = false,
                              int width = 10, int height = 10,
                              char fill = '#');

}

#endif // RENDERER_HPP_INCLUDED