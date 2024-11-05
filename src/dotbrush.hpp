#pragma once
#include <raylib.h>
#include <vector>
#include "qtreebounds.hpp"
#include "qtree.hpp"
#include "boundsselect.hpp"
class DotBrush {
private:
    bool isDrawing {false};
    bool isDrawListing{false};
    bool isSelectListing{false};
    Vector2 last_pos;
    QTreeBounds bounds;
    std::vector<Vector2> points;
    std::vector<Vector2> selected_points;
    BoundsSelect boundsSelect;
    QTree qtree;
public:
    DotBrush();
    ~DotBrush();
    void update(const Camera2D &camera);
    void draw_points();
    void draw_status();
    void setListenDraw(const bool onoff);
    bool getListenDraw() const;
    void setSelectListing(const bool onoff);
    bool getSelectListing() const;
    void setBounds(const float width, const float height);
    void unloadPoints(const std::string &file);
    void loadPoints(const std::string &file);
    void clear();
};