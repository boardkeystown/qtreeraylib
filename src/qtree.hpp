#pragma once
#include <vector>
#include <memory>
#include <raylib.h>
#include "qtreebounds.hpp"
constexpr const std::size_t QTREE_MAX_CAP {4};
class QTree {
private:
    bool divided {false};
    std::size_t capacity {QTREE_MAX_CAP};
    QTreeBounds bounds;
    std::vector<Vector2> points;
    std::unique_ptr<QTree> northeast;
    std::unique_ptr<QTree> northwest;
    std::unique_ptr<QTree> southeast;
    std::unique_ptr<QTree> southwest;
private:
    void subdivide();
public:
    QTree(/* args */);
    QTree(const QTreeBounds &bounds);
    ~QTree();
    void setBounds(const QTreeBounds &bounds);
    void draw();
    bool insert(const Vector2 &point);
    void query(const QTreeBounds &bounds, std::vector<Vector2> &found);
    std::size_t size();
    void clear();
};
