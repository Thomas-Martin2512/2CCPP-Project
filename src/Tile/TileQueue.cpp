#include "../../include/Tile/TileQueue.hpp"
#include <algorithm>
#include <stdexcept>

TileQueue::TileQueue(bool reshuffleOnExhaustion)
  : reshuffleOnExhaustion_(reshuffleOnExhaustion), rng_(std::random_device{}()) {}

bool TileQueue::initFrom(const InitTiles& src, bool shuffle, std::uint32_t seed) {
    const auto& v = src.all();
    if (v.empty()) return false;
    all_ = v;
    rng_.seed(seed);
    std::vector<Tile> tmp = all_;
    if (shuffle) std::shuffle(tmp.begin(), tmp.end(), rng_);
    deck_.assign(tmp.begin(), tmp.end());
    return true;
}

Tile TileQueue::draw() {
    if (deck_.empty()) {
        if (reshuffleOnExhaustion_) reshuffle_();
        else throw std::runtime_error("draw on empty deck");
    }
    Tile t = deck_.front();
    deck_.pop_front();
    if (deck_.empty() && reshuffleOnExhaustion_) reshuffle_();
    return t;
}

std::vector<Tile> TileQueue::peek(std::size_t count) const {
    std::vector<Tile> out;
    count = std::min<std::size_t>(count, deck_.size());
    out.reserve(count);
    for (std::size_t i = 0; i < count; ++i) out.push_back(deck_[i]);
    return out;
}

void TileQueue::reshuffle_() {
    std::vector<Tile> tmp = all_;
    std::shuffle(tmp.begin(), tmp.end(), rng_);
    deck_.assign(tmp.begin(), tmp.end());
}

std::optional<Tile> TileQueue::findInAll_(const std::string& id) const {
    for (const auto& t : all_) if (t.getId() == id) return t;
    return std::nullopt;
}

std::vector<std::string> TileQueue::nextTileIds(std::size_t count) const {
    auto nxt = peek(count);
    std::vector<std::string> ids;
    ids.reserve(nxt.size());
    for (auto& t : nxt) ids.push_back(t.getId());
    return ids;
}

std::optional<Tile> TileQueue::exchangeWithWindow(std::size_t index, const std::string& currentId) {
    if (index >= 5 || index >= deck_.size()) return std::nullopt;

    auto curOpt = findInAll_(currentId);
    if (!curOpt) return std::nullopt;

    Tile newCurrent = deck_[index];
    deck_[index] = *curOpt;
    return newCurrent;
}

static std::pair<int,int> boxWH(const std::vector<std::pair<int,int>>& cells) {
    if (cells.empty()) return {0,0};
    int maxX = 0, maxY = 0;
    for (auto [x,y] : cells) { maxX = std::max(maxX, x); maxY = std::max(maxY, y); }
    return {maxX+1, maxY+1};
}

std::vector<std::string> TileQueue::asciiTile_(const Tile& t, char fill) {
    const auto& cells = t.getCells();
    auto [w,h] = boxWH(cells);
    if (w==0 || h==0) return {"(vide)"};
    std::vector<std::string> g(h, std::string(w, ' '));
    for (auto [x,y] : cells) if (x>=0 && y>=0 && x<w && y<h) g[y][x] = fill;
    return g;
}

std::vector<std::string> TileQueue::hstack_(const std::vector<std::vector<std::string>>& blocks, int gap) {
    std::size_t H = 0;
    for (auto& b : blocks) H = std::max(H, b.size());
    std::vector<std::string> out(H, "");
    std::string sep(gap, ' ');
    for (std::size_t i=0;i<blocks.size();++i) {
        const auto& b = blocks[i];
        std::size_t w = b.empty()?0:b[0].size();
        for (std::size_t r=0;r<H;++r) {
            if (r < b.size()) out[r] += b[r];
            else out[r] += std::string(w, ' ');
            if (i+1<blocks.size()) out[r] += sep;
        }
    }
    return out;
}

void TileQueue::display(std::ostream& os, const Tile& current, std::size_t nextCount) const {
    os << "Current Tile :\n\n";
    auto cur = asciiTile_(current, '\xDB'); // █
    for (auto& line : cur)
        os << "    " << line << "\n";

    os << "\nNext Tiles :\n\n";

    auto nxt = peek(nextCount);
    std::vector<std::vector<std::string>> blocks;
    blocks.reserve(nxt.size());
    for (auto& t : nxt)
        blocks.push_back(asciiTile_(t, '\xDB'));

    auto lines = hstack_(blocks, 14);
    for (auto& line : lines)
        os << "    " << line << "\n";

    os << "\n";
}