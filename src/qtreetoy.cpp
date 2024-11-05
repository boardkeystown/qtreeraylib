#include "qtreetoy.hpp"
#include <raylib.h>

#include "grid.hpp"
#include "cam.hpp"
#include "dotbrush.hpp"

QTreeToy::QTreeToy() {}
QTreeToy::~QTreeToy() {
    if (this->started)
        CloseWindow();
}
void QTreeToy::run() {
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800 , 600, "qtreetoy");
    SetTargetFPS(120);
    this->started = true;
    Grid grid;
    Cam cam(Vector2 {800,600}, Vector2 {800,600});
    DotBrush dotBrush;
    dotBrush.setBounds(300,300);
    dotBrush.loadPoints("points.log");
    while (!WindowShouldClose()) {
        // input
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P)) {
            cam.setListenPan(!cam.getListenPan());
            if (cam.getListenPan()) {
                dotBrush.setListenDraw(false);
                dotBrush.setSelectListing(false);
            }
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_D)) {
            dotBrush.setListenDraw(!dotBrush.getListenDraw());
            if (dotBrush.getListenDraw()) cam.setListenPan(false);
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            dotBrush.setSelectListing(!dotBrush.getSelectListing());
            if (dotBrush.getSelectListing()) cam.setListenPan(false);
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
            dotBrush.clear();
        }

        // update
        cam.update();
        dotBrush.update(cam.camera);
        // draw
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(cam.camera);
            grid.draw();
            dotBrush.draw_points();
        EndMode2D();
        cam.draw();
        dotBrush.draw_status();
        DrawFPS(GetScreenWidth()-100,10);
        EndDrawing();
    }
    // dotBrush.unloadPoints("points.log");
}
