#pragma once
#include <raylib.h>
#include "qtreebounds.hpp"
class BoundsSelect {
private:
    bool isPanning {false};
    bool isReady {false};
    float left {0.f};
    float top {0.f};
    Vector2 startPos;
    Vector2 endPos;
    Rectangle current_rec;
public:
    BoundsSelect();
    ~BoundsSelect();
    void update(const Camera2D &camera);
    void draw();
    bool selectionReady();
    QTreeBounds getBounds() const;
};