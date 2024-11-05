#include "dotbrush.hpp"
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <sstream>

DotBrush::DotBrush() {
    this->isDrawing  = false;
    this->isDrawListing = false;
    this->isSelectListing = false;

    this->last_pos = Vector2 {0.f,0.f};
    this->setBounds(500.f,500.f);
    this->points = {};
}
DotBrush::~DotBrush() {}
void DotBrush::update(const Camera2D &camera) {
    
    if (this->isSelectListing)
        this->boundsSelect.update(camera);

    if (this->isDrawListing)
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mp = GetMousePosition();
            Vector2 smp = GetScreenToWorld2D(mp,camera);
            // do not add same point over and over
            std::cout << this->bounds.toString() << "\n";
            std::cout << "\t" << smp.x << "," << smp.y << "\n";
            if (!Vector2Equals(smp,last_pos) && this->bounds.contains(smp)) {
                this->points.emplace_back(smp);
                // TODO: OH GOD REMEMBER I NEED TO SET BOUNDS B4
                // I USE THIS
                (void)this->qtree.insert(smp);
                last_pos = smp;
            }
            std::cout << "points# " << this->points.size() << "\n";
            std::cout << "qtree # " << this->qtree.size() << "\n";
        }
    if (this->boundsSelect.selectionReady()) {
        std::cout << "selection made\n";
        this->selected_points.clear();
        qtree.query(this->boundsSelect.getBounds(),this->selected_points);
        std::cout << "number of points selected " << this->selected_points.size() << "\n";
    }
}

void DotBrush::draw_status() {
    if (this->isDrawListing) {
        DrawText("Drawing Active - (Ctrl+d)", 10,45,20.f,YELLOW);
    } else {
        DrawText("Drawing Locked - (Ctrl+d)", 10,45,20.f,YELLOW);
    }

    if (this->isSelectListing) {
        DrawText("Selection Active - (Ctrl+s)", 10,65,20.f,YELLOW);
    } else {
        DrawText("Selection Locked - (Ctrl+s)", 10,65,20.f,YELLOW);
    }
    DrawText("Clear Points - (Ctrl+q)", 10,85,20.f,YELLOW);

}

void DotBrush::draw_points() {
    for (const auto & point : this->points ) {
        DrawCircleV(point, 5, YELLOW);
    }
    DrawRectangleLinesEx(this->bounds.getDrawRec(),1,RED);
    this->boundsSelect.draw();
    this->qtree.draw();

    for (const auto & point : this->selected_points ) {
        DrawCircleV(point, 5, BLUE);
    }
}

void DotBrush::setListenDraw(const bool onoff) {
    this->isDrawListing = onoff;
    if (onoff) this->isSelectListing = false;
}
bool DotBrush::getListenDraw() const {return this->isDrawListing;}

void DotBrush::setSelectListing(const bool onoff) {
    this->isSelectListing = onoff;
    if (onoff) this->isDrawListing = false;
}
bool DotBrush::getSelectListing() const {return this->isSelectListing;}

void DotBrush::setBounds(const float width, const float height) {
    this->bounds.setBounds(Rectangle {0,0, width, height});
}
void DotBrush::unloadPoints(const std::string &file) {
    std::ofstream w_file;
    if (this->points.size()) {
        w_file.open(file.c_str());
    }
    for (auto const &point : this->points) {
        w_file << point.x  << ","  << point.y << "\n";
    }
    if (w_file.is_open()) {
        w_file.close();
    }
}

void DotBrush::clear() {
    this->qtree.clear();
    this->points.clear();
    this->selected_points.clear();
}

void DotBrush::loadPoints(const std::string &file) {
    auto Tokenize = [](const std::string &line) {
        std::vector<std::string> vec {};
        std::string token {""};
        std::istringstream tokens(line);
        while (std::getline(tokens, token, ',')) {
            vec.emplace_back(token);
        }
        return vec;
    };
    this->points = {};
    std::ifstream r_file;
    r_file.open(file.c_str());
    std::string line;
    while (!std::getline(r_file,line).eof()) {
        auto tokens = Tokenize(line);
        this->points.emplace_back(Vector2 {
            std::stof(tokens.at(0)),
            std::stof(tokens.at(1))
        });
    }
    qtree.setBounds(this->bounds);
    for (const auto &point: this->points) {
        (void)qtree.insert(point);
    }
    std::cout << "qtree.size() " << qtree.size() << "\n";
    std::cout << "this->points.size() " << this->points.size() << "\n";
}