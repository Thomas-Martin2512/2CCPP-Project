#include "Render/Renderer.hpp"
#include <vector>
#include <algorithm>
#include <sstream>

namespace render {

    static std::pair<int,int> box(const std::vector<std::pair<int,int>>& cells) {
        if (cells.empty()) return {0,0};
        int maxX = 0, maxY = 0;
        for (auto [x,y] : cells) { maxX = std::max(maxX, x); maxY = std::max(maxY, y); }
        return {maxX+1, maxY+1};
    }

    std::string drawTile(const Tile& t, char fill) {
        const auto& cells = t.getCells();
        auto [w,h] = box(cells);
        if (w==0 || h==0) return "(vide)\n";

        std::vector<std::string> grid(h, std::string(w, '.'));
        for (auto [x,y] : cells) {
            if (y>=0 && y<h && x>=0 && x<w) grid[y][x] = fill;
        }
        std::ostringstream out;
        out << t.getId() << " (" << w << "x" << h << ")\n";
        for (int row=0; row<h; ++row) out << grid[row] << "\n";
        return out.str();
    }

    std::string drawFootprint(const Tile& t,
                              int originX, int originY,
                              int rotations, bool flipped,
                              int width, int height,
                              char fill) {

        std::vector<std::string> grid(height, std::string(width, '.'));
        auto pts = t.footprint(originX, originY, rotations, flipped);

        for (auto [x,y] : pts) {
            if (y>=0 && y<height && x>=0 && x<width) grid[y][x] = fill;
        }
        std::ostringstream out;
        out << t.getId()
            << " @(" << originX << "," << originY << ") rot=" << (rotations%4)
            << (flipped ? " flip=1" : " flip=0") << "\n";
        for (int row=0; row<height; ++row) out << grid[row] << "\n";
        return out.str();
    }

}