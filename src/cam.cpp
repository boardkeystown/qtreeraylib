#include "cam.hpp"
#include <raymath.h>
#include <sstream>

// private

std::string Cam::mk_mouse_screen_pos(const Vector2 &vec2) {
    std::stringstream ss;
    ss
    << vec2.x
    << ","
    << vec2.y
    ;
    return ss.str();
}

// public 

Cam::Cam(
        const Vector2 &world_size,
        const Vector2 &screen_size) {
    this->isPanning = false;
    this->isListenPan = false;
    this->panStartWorldPos = Vector2 {0.f,0.f};
    this->camera.target = Vector2 {
        world_size.x / 2.f,
        world_size.y / 2.f
    };
    this->camera.offset = Vector2 {
        screen_size.x / 2.f,
        screen_size.y / 2.f
    };
    this->camera.rotation = 0.f;
    this->camera.zoom = 1.f;

    this->camera.target.x = 0.f;
    this->camera.target.y = 0.f;
    
}
Cam::~Cam() {}
void Cam::update() {
    if (!this->isListenPan) return;
    // panning
    if (!this->isPanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mp = GetMousePosition();
        this->panStartWorldPos = GetScreenToWorld2D(mp,this->camera);
        this->isPanning = true;
    }
    if (this->isPanning && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mp = GetMousePosition();
        Vector2 currWorldPos = GetScreenToWorld2D(mp, this->camera);
        Vector2 panDelta = Vector2Subtract(
            this->panStartWorldPos,
            currWorldPos
        );
        this->camera.target = Vector2Add(this->camera.target,panDelta);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        this->isPanning = false;
    }
}
void Cam::draw() {
    Vector2 mp = GetMousePosition();
    Vector2 mpworldpos = GetScreenToWorld2D(mp, this->camera);
    std::string ss = this->mk_mouse_screen_pos(mpworldpos);
    DrawText(ss.c_str(), 10,10,20.f,PURPLE);
    if (this->isListenPan) {
        DrawText("camPan Active - (Ctrl+p)", 10,25,20.f,PURPLE);
    } else {
        DrawText("camPan Paused - (Ctrl+p)", 10,25,20.f,PURPLE);
    }
}
void Cam::setListenPan(const bool onoff) { this->isListenPan = onoff; }
bool Cam::getListenPan() const { return this->isListenPan; }