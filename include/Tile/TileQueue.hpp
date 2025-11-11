#ifndef TILEQUEUE_HPP_INCLUDED
#define TILEQUEUE_HPP_INCLUDED

#include <deque>
#include <random>
#include <vector>
#include <iostream>
#include "Tile/Tile.hpp"
#include "Tile/InitTiles.hpp"

class TileQueue {
public:
    explicit TileQueue(bool reshuffleOnExhaustion = true);

    bool initFrom(const InitTiles& src, bool shuffle = true, std::uint32_t seed = std::random_device{}());

    std::vector<Tile> peek(std::size_t count = 5) const;

    Tile draw();

    std::optional<Tile> exchangeWithWindow(std::size_t index, const std::string& currentId);

    void display(std::ostream& os, const Tile& current, std::size_t nextCount = 5) const;

    std::size_t size() const { return deck_.size(); }

    bool empty() const { return deck_.empty(); }

    std::vector<std::string> nextTileIds(std::size_t count = 5) const;

private:
    bool reshuffleOnExhaustion_;
    std::vector<Tile> all_;
    std::deque<Tile> deck_;
    mutable std::mt19937 rng_;

    void reshuffle_();
    static std::vector<std::string> asciiTile_(const Tile& t, char fill = '\xDB');
    static std::vector<std::string> hstack_(const std::vector<std::vector<std::string>>& blocks, int gap = 14);

    std::optional<Tile> findInAll_(const std::string& id) const;
};

#endif // TILEQUEUE_HPP_INCLUDED