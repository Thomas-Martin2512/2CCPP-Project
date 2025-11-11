#include <iostream>
#include <string>
#include "Tile/InitTiles.hpp"
#include "Render/Renderer.hpp"
#include "Tile/TileQueue.hpp"

int main() {
    InitTiles tiles;

    if (!tiles.loadFromFile("Shapes.json")) {
        std::cerr << "Impossible de charger Shapes.json\n";
        return 1;
    }

    std::cout << "Tiles chargees : " << tiles.all().size() << "\n\n";

    for (size_t i = 0; i < std::min<size_t>(tiles.all().size(), 3); ++i) {
        std::cout << render::drawTile(tiles.all()[i]) << "\n";
    }

    if (!tiles.all().empty()) {
        const Tile& t = tiles.all().front();
        std::cout << "== Footprint sur 10x10 ==\n";
        for (int rot = 0; rot < 4; ++rot) {
            std::cout << render::drawFootprint(t, 3, 3, rot, false, 10, 10, '#') << "\n";
        }
        std::cout << "-- avec flip --\n";
        std::cout << render::drawFootprint(t, 3, 3, 0, true, 10, 10, '#') << "\n";
    }

    InitTiles loader;
    if (!loader.loadFromFile("Shapes.json")) {
        std::cerr << "Impossible de charger Shapes.json\n";
        return 1;
    }

    TileQueue q(true);
    q.initFrom(loader, true);

    Tile current = q.draw();

    while (true) {
        q.display(std::cout, current, 5);
        std::cout << "\n";
        std::cout << "Choose the tile to exchange with : ";
        int idx;
        std::cin >> idx;
        if (idx < 0) break;

        auto newCurrent = q.exchangeWithWindow(idx, current.getId());
        current = *newCurrent;
        }

    return 0;
}