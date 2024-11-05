#include "boundsselect.hpp"
#include <raymath.h>
#include <iostream>
BoundsSelect::BoundsSelect() {}
BoundsSelect::~BoundsSelect() {}
void BoundsSelect::update(const Camera2D &camera) {
    if (!this->isPanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mp = GetMousePosition();
        this->startPos = GetScreenToWorld2D(mp,camera);
        this->isPanning = true;
        this->isReady = false;
    }
    if (this->isPanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mp = GetMousePosition();
        this->endPos = GetScreenToWorld2D(mp, camera);
        // Calculate the rectangle for selection based on the mouse drag direction
        if (this->startPos.x < this->endPos.x && this->startPos.y < this->endPos.y) {
            // Lower right
            this->current_rec = Rectangle {
                this->startPos.x,
                this->startPos.y,
                std::abs(this->startPos.x - this->endPos.x),
                std::abs(this->startPos.y - this->endPos.y)
            };
        } else if (this->startPos.x > this->endPos.x && this->startPos.y < this->endPos.y) {
            // Lower left
            this->current_rec = Rectangle {
                this->endPos.x,
                this->startPos.y,
                std::abs(this->startPos.x - this->endPos.x),
                std::abs(this->startPos.y - this->endPos.y)
            };
        } else if (this->startPos.x < this->endPos.x && this->startPos.y > this->endPos.y) {
            // Upper right
            this->current_rec = Rectangle {
                this->startPos.x,
                this->endPos.y,
                std::abs(this->startPos.x - this->endPos.x),
                std::abs(this->startPos.y - this->endPos.y)
            };
        } else if (this->startPos.x > this->endPos.x && this->startPos.y > this->endPos.y) {
            // Upper left
            this->current_rec = Rectangle {
                this->endPos.x,
                this->endPos.y,
                std::abs(this->startPos.x - this->endPos.x),
                std::abs(this->startPos.y - this->endPos.y)
            };
        }
        this->left = this->current_rec.x - (this->current_rec.width / 2.f);
        this->top = this->current_rec.y - (this->current_rec.height / 2.f);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        this->isPanning = false;
        this->isReady = true;
    }
}
void BoundsSelect::draw() {
    if (!isPanning) return;
    const Color transparentBlue = { 0, 0, 255, 128 };
    DrawRectangleRec(this->current_rec, transparentBlue);
    DrawCircleV(this->startPos,10,transparentBlue);
    DrawCircleV(this->endPos,10,transparentBlue);
    // DrawRectangleLinesEx(QTreeBounds(Rectangle {
    //     this->current_rec.x + (this->current_rec.width / 2.f),
    //     this->current_rec.y + (this->current_rec.height / 2.f),
    //     this->current_rec.width,
    //     this->current_rec.height
    // }).getDrawRec(),5,RED);
}
bool BoundsSelect::selectionReady() {
    if (this->isReady) {
        this->isReady = false;
        return true;
    }
    return false;
}
QTreeBounds BoundsSelect::getBounds() const {
    // RECALL THAT QTree bounds start at center not origin top left.
    // so selection should me adjusted to be center ?
    // return QTreeBounds(this->current_rec);
    return QTreeBounds(Rectangle {
        this->current_rec.x + (this->current_rec.width / 2.f),
        this->current_rec.y + (this->current_rec.height / 2.f),
        this->current_rec.width,
        this->current_rec.height
    });
}