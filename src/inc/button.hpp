#pragma once

#include <raylib.h>

struct Button {
    Rectangle rec = {};
    Color colorNormal = { 235, 235, 238, 255 };
    Color colorHovering = GRAY;
    bool hovering = false;

    void Draw();

    bool Pressed(Vector2 pos);
};
