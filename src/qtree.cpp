#include "qtree.hpp"

// private
void QTree::subdivide() {
    this->northeast = std::make_unique<QTree>(this->bounds.northeast());
    this->northwest = std::make_unique<QTree>(this->bounds.northwest());
    this->southeast = std::make_unique<QTree>(this->bounds.southeast());
    this->southwest = std::make_unique<QTree>(this->bounds.southwest());
    this->divided = true;

    // // TODO: move points down to kids to keep bubbling down
    // move points down
    // for (const auto &point : this->points) {
    //     bool inserted = 
    //      this->northeast->insert(point) ||
    //      this->northwest->insert(point) ||
    //      this->southeast->insert(point) ||
    //      this->southwest->insert(point);
    // }
    // this->points.clear();
}

// public 
QTree::QTree(/* args */) {}
QTree::QTree(const QTreeBounds &bounds) { this->bounds = bounds; }
QTree::~QTree() {}
void QTree::setBounds(const QTreeBounds &bounds) { this->bounds = bounds; }
void QTree::draw() {
    for (const auto &point : this->points) {
        DrawCircleV(point, 1, RED);
    }
    this->bounds.draw();
    if (this->divided) {
        this->northeast->draw();
        this->northwest->draw();
        this->southeast->draw();
        this->southwest->draw();
    }
}
bool QTree::insert(const Vector2 &point) {
    if (!this->bounds.contains(point)) {
        return false;
    }
    if (!this->divided) {
        // TODO: what to do about depth???
        if (this->points.size() < this->capacity) {
            this->points.emplace_back(point);
            return true;
        }
        this->subdivide();
    }
    return
        this->northeast->insert(point) ||
        this->northwest->insert(point) ||
        this->southeast->insert(point) ||
        this->southwest->insert(point)
    ;

}
void QTree::query(const QTreeBounds &bounds, std::vector<Vector2> &found) {
    if (!bounds.intersects(this->bounds)) {
        return;
    }
    if (this->divided) {
        this->northeast->query(bounds,found);
        this->northwest->query(bounds,found);
        this->southeast->query(bounds,found);
        this->southwest->query(bounds,found);
    }
    for (const auto &point : this->points) {
        if (bounds.contains(point)) {
            found.emplace_back(point);
        }
    }
}


std::size_t QTree::size() {
    std::size_t size = this->points.size();
    if (this->divided) {
        size+= this->northeast->size() +
                this->northwest->size() +
                this->southeast->size() +
                this->southwest->size()
        ;
    }
    return size;
}


void QTree::clear() {
    if (this->divided) {
        this->northeast->clear();
        this->northwest->clear();
        this->southeast->clear();
        this->southwest->clear();

        this->northeast.reset();
        this->northwest.reset();
        this->southeast.reset();
        this->southwest.reset();
    }
    this->divided = false;
    this->points = {};

}