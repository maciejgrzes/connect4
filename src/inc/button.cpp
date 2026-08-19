#include "button.hpp"

#include <raylib.h>

void Button::Draw() {
    DrawRectangleRounded(rec, 0.1, 15, hovering ? colorHovering : colorNormal);
}

bool Button::Pressed(Vector2 pos) {
    hovering = (pos.x > rec.x && pos.x < rec.x + rec.width) &&
               (pos.y > rec.y && pos.y < rec.y + rec.height);
    return hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
