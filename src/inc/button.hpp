#pragma once

#include <raylib.h>
#include <string>

struct Button {
    Rectangle rec = {};
    Color colorNormal = { 235, 235, 238, 255 };
    Color colorHovering = GRAY;
    bool hovering = false;

    void Draw(std::string name);

    bool Pressed(Vector2 pos);
};
