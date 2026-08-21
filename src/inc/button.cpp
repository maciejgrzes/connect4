#include "button.hpp"

#include <raylib.h>
#include <string>

void Button::Draw(std::string name) {
    DrawRectangleRounded(rec, 0.1, 15, hovering ? colorHovering : colorNormal);
    int textWidth = MeasureText(name.c_str(), 20);
    float x = rec.x + (rec.width - textWidth) / 2.0f;
    float y = rec.y + (rec.height - 20) / 2.0f;
    DrawText(name.c_str(), (int)x, (int)y, 20, BLACK);
}

bool Button::Pressed(Vector2 pos) {
    hovering = (pos.x > rec.x && pos.x < rec.x + rec.width) &&
               (pos.y > rec.y && pos.y < rec.y + rec.height);
    return hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
