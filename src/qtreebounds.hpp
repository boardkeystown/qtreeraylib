#pragma once
#include <raylib.h>
#include <string>
class QTreeBounds {
private:
    Rectangle rec {0.f,0.f,0.f,0.f};
    Rectangle draw_rec {0.f,0.f,0.f,0.f};
    float left {0.f};
    float right {0.f};
    float top {0.f};
    float bottom {0.f};
public:
    QTreeBounds();
    QTreeBounds(const Rectangle &rec);
    QTreeBounds(const QTreeBounds &o);
    QTreeBounds & operator=(const QTreeBounds &o);
    ~QTreeBounds();
    void setBounds(const Rectangle &rec);
    const Rectangle &getDrawRec();
    void draw();
    bool contains(const Vector2 &point) const;
    bool intersects(const QTreeBounds &bounds) const;
    QTreeBounds northeast() const;
    QTreeBounds northwest() const;
    QTreeBounds southeast() const;
    QTreeBounds southwest() const;
    std::string toString() const;
};