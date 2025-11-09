#include "../../include/Tile/Tile.hpp"
#include <algorithm>
#include <unordered_set>



Tile::Tile(std::string id, std::vector<std::pair<int,int>> cells, std::string color)
    : id_(std::move(id)), color_(std::move(color)), cells_(std::move(cells))
{
    normalizeToOrigin();
}

const std::string& Tile::getId() const {
    return id_;
}
const std::string &Tile::getColor() const {
    return color_;
}
const std::vector<std::pair<int,int>>& Tile::getCells() const {
    return cells_;
}

void Tile::setId(std::string &newId) {
    id_ = newId;
}
void Tile::setColor(std::string &newColor) {
    color_ = newColor;
}



void Tile::normalize(std::vector<std::pair<int, int>>& pts) {
    if (pts.empty()) return;
    int minX = pts[0].first, minY = pts[0].second;
    for (auto& p : pts) {
        if (p.first < minX) minX = p.first;
        if (p.second < minY) minY = p.second;
    }
    for (auto& p : pts) {
        p.first -= minX;
        p.second -= minY;
    }
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
}

void Tile::normalizeToOrigin() {
    normalize(cells_);
}

std::pair<int, int> Tile::boxLimit() const {
    if (cells_.empty()) return {0, 0};
    int maxX = cells_[0].first, maxY = cells_[0].second;
    for (auto& p : cells_) {
        if (p.first > maxX) maxX = p.first;
        if (p.second > maxY) maxY = p.second;
    }
    return {maxX + 1, maxY + 1};
}

void Tile::rotate() {
    std::vector<std::pair<int,int>> out;
    out.reserve(cells_.size());
    for (auto [x, y] : cells_) {
        out.emplace_back(y, -x);
    }
    normalize(out);
    cells_.swap(out);
}

void Tile::flip() {
    std::vector<std::pair<int,int>> out;
    out.reserve(cells_.size());
    for (auto [x, y] : cells_) {
        out.emplace_back(-x, y);
    }
    normalize(out);
    cells_.swap(out);
}

std::vector<std::pair<int, int> > Tile::footprint(int originX, int originY, int rotations, bool flipped) const {
    std::vector<std::pair<int,int>> pts = cells_;
    if (flipped) {
        for (auto& p : pts) {
            p.first = -p.first;
        }
        normalize(pts);
    }
    rotations = ((rotations % 4) + 4) % 4;
    for (int k = 0; k < rotations; ++k) {
        for (auto& p : pts) {
            int x = p.first, y = p.second;
            p = { y, -x };
        }
        normalize(pts);
    }
    for (auto& p : pts) {
        p.first  += originX;
        p.second += originY;
    }
    return pts;
}

bool Tile::shapeEquals(const Tile& other) const {
    if (cells_.size() != other.cells_.size()) return false;
    return std::equal(cells_.begin(), cells_.end(), other.cells_.begin());
}
