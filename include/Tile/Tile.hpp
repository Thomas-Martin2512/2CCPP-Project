#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>



class Tile {
    protected:
        std::string id_;
        std::string color_;
        std::vector<std::pair<int, int>> cells_;

    public:
        Tile() = default;
        Tile(std::string id, std::vector<std::pair<int, int>> cells, std::string color = "green");
        virtual ~Tile() = default;

        virtual const std::string& getId() const;
        virtual const std::string& getColor() const;
        virtual const std::vector<std::pair<int, int>>& getCells() const;

        virtual void setId(std::string& newId);
        virtual void setColor(std::string& newColor);

        virtual void rotate();
        virtual void flip();
        virtual void normalizeToOrigin();

        virtual std::pair<int, int> boxLimit() const;
        virtual std::vector<std::pair<int, int>>
        footprint(int originX, int originY, int rotations = 0, bool flipped = false) const;

        virtual bool shapeEquals(const Tile& other) const;

    private:
        static void normalize(std::vector<std::pair<int, int>>& pts);
};

#endif // TILE_HPP_INCLUDED