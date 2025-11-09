#include "../../include/Tile/InitTiles.hpp"
#include <fstream>
#include <unordered_set>
#include <nlohmann/json.hpp>

using nlohmann::json;

InitTiles::InitTiles(const std::string& jsonPath) {
    loadFromFile(jsonPath);
}

bool InitTiles::loadFromFile(const std::string& jsonPath) {
    std::ifstream f(jsonPath);
    if (!f) return false;
    json j;
    f >> j;
    tiles_.clear();
    if (!j.contains("tiles") || !j["tiles"].is_array()) return false;
    for (const auto& tile : j["tiles"]) {
        if (!tile.contains("id") || !tile.contains("cells")) continue;
        std::string id = tile["id"].get<std::string>();
        std::vector<std::pair<int, int>> cells;
        for (const auto& c : tile["cells"]) {
            if (!c.is_array() || c.size() != 2) continue;
            int x = c[0].get<int>();
            int y = c[1].get<int>();
            cells.emplace_back(x,y);
        }
        tiles_.emplace_back(id, std::move(cells));
    }
    return true;
}

const std::vector<Tile>& InitTiles::all() const {
    return tiles_;
}

std::optional<Tile> InitTiles::byId(const std::string& id) const {
    for (const auto& tile : tiles_) {
        if (tile.getId() == id) return tile;
    }
    return std::nullopt;
}

void InitTiles::deduplicateByShape() {
    std::vector<Tile> unique;
    for (const auto& t : tiles_) {
        bool already = false;
        for (const auto& u : unique) {
            if (t.shapeEquals(u)) { already = true; break; }
        }
        if (!already) unique.push_back(t);
    }
    tiles_.swap(unique);
}




























