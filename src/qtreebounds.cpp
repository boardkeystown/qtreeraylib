#include "qtreebounds.hpp"
#include <sstream>
QTreeBounds::QTreeBounds() {
    this->setBounds(Rectangle{0.f,0.f,1.f,1.f});
};
QTreeBounds::QTreeBounds(const Rectangle &rec) {
    this->setBounds(rec);
}
QTreeBounds::~QTreeBounds() {}

QTreeBounds::QTreeBounds(const QTreeBounds &o) {*this = o;}
QTreeBounds& QTreeBounds::operator=(const QTreeBounds &o) {
    this->rec = o.rec;
    this->draw_rec = o.draw_rec;
    this->left = o.left;
    this->right = o.right;
    this->top = o.top;
    this->bottom = o.bottom;
    return *this;
}

void QTreeBounds::setBounds(const Rectangle &rec) {
    this->rec = rec;
    // NOTE: the origin you set in REC is the center of 
    // of the BOX!!!!!
    this->left = rec.x - (rec.width / 2.f);
    this->right = rec.x + (rec.width / 2.f);
    this->top = rec.y - (rec.height / 2.f);
    this->bottom = rec.y + (rec.height / 2.f);
    // NOTE: To draw it you need to offset the origin in raylib
    this->draw_rec = this->rec;
    this->draw_rec.x = this->left;
    this->draw_rec.y = this->top;
}

const Rectangle &QTreeBounds::getDrawRec() {
    return this->draw_rec;
}

void QTreeBounds::draw() {
    DrawRectangleLinesEx(this->draw_rec,1,BLUE);
}
bool QTreeBounds::contains(const Vector2 &point) const {
    return point.x >= this->left && point.x <= this->right &&
           point.y >= this->top && point.y <= this->bottom;
}

bool QTreeBounds::intersects(const QTreeBounds &bounds) const {
    return !(
        this->right < bounds.left ||  // This rectangle is to the left of the other
        bounds.right < this->left ||  // This rectangle is to the right of the other
        this->bottom < bounds.top ||  // This rectangle is above the other
        bounds.bottom < this->top     // This rectangle is below the other
    );
}
QTreeBounds QTreeBounds::northeast() const {
    return QTreeBounds(Rectangle {
        this->rec.x + (this->rec.width / 4.f),
        this->rec.y - (this->rec.height / 4.f),
        this->rec.width / 2.f,
        this->rec.height / 2.f
    });
}
QTreeBounds QTreeBounds::northwest() const {
    return QTreeBounds(Rectangle {
        this->rec.x - (this->rec.width / 4.f),
        this->rec.y - (this->rec.height / 4.f),
        this->rec.width / 2.f,
        this->rec.height / 2.f
    });
}
QTreeBounds QTreeBounds::southeast() const {
    return QTreeBounds(Rectangle {
        this->rec.x + (this->rec.width / 4.f),
        this->rec.y + (this->rec.height / 4.f),
        this->rec.width / 2.f,
        this->rec.height / 2.f
    });
}
QTreeBounds QTreeBounds::southwest() const {
    return QTreeBounds(Rectangle {
        this->rec.x - (this->rec.width / 4.f),
        this->rec.y + (this->rec.height / 4.f),
        this->rec.width / 2.f,
        this->rec.height / 2.f
    });
}
std::string QTreeBounds::toString() const {
    std::stringstream ss;
    ss 
    << "QTreeBounds(Rec("
    << this->rec.x
    << ", "
    << this->rec.y
    << ", "
    << this->rec.width
    << ", "
    << this->rec.height
    << ") "
    << this->left
    << ", "
    << this->right
    << ", "
    << this->top
    << ", "
    << this->bottom
    << ")";
    return ss.str();

}