#include "grid.hpp"

Grid::Grid() {
    centerLines.emplace_back(
        std::make_tuple<Vector2, Vector2>(Vector2{0.f,0.f},Vector2{100.f,0.f})
    );
    centerLines.emplace_back(
        std::make_tuple<Vector2, Vector2>(Vector2{0.f,0.f},Vector2{-100.f,0.f})
    );
    centerLines.emplace_back(
        std::make_tuple<Vector2, Vector2>(Vector2{0.f,0.f},Vector2{0.f,100.f})
    );
    centerLines.emplace_back(
        std::make_tuple<Vector2, Vector2>(Vector2{0.f,0.f},Vector2{0.f,-100.f})
    );
}
Grid::~Grid() {}
void Grid::draw() {
    DrawCircle(0,0,10,GRAY);
    for (const auto & [start,end] : this->centerLines)
        DrawLineEx(start,end,4,GRAY);
}

