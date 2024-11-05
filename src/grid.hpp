#pragma once
#include <raylib.h>
#include <vector>
#include <tuple>
class Grid {
private:
    std::vector<std::tuple<Vector2,Vector2> > centerLines;
public:
    Grid();
    ~Grid();
    void draw();
};