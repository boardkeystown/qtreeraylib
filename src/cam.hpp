#pragma once
#include <raylib.h>
#include <string>
class Cam  {
private:
    bool isPanning {false};
    bool isListenPan {false};
    Vector2 panStartWorldPos;
public:
    Camera2D camera;
private:
    std::string mk_mouse_screen_pos(const Vector2 &vec2);
public:
    Cam(const Vector2 &world_size, const Vector2 &screen_size);
    ~Cam();
    void update();
    void draw();
    void setListenPan(const bool onoff);
    bool getListenPan() const;
};