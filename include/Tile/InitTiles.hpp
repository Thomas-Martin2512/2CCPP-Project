#ifndef INITTILES_HPP_INCLUDED
#define INITTILES_HPP_INCLUDED

#include "Tile.hpp"
#include <string>
#include <vector>
#include <optional>



class InitTiles {
private:
    std::vector<Tile> tiles_;

public:
    InitTiles() = default;
    explicit InitTiles(const std::string& jsonPath);

    bool loadFromFile(const std::string& jsonPath);

    const std::vector<Tile>& all() const;
    std::optional<Tile> byId(const std::string& id) const;

    void deduplicateByShape();
};

#endif // INITTILES_HPP_INCLUDED