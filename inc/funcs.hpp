#pragma once

#include "structs.hpp"
#include <raylib.h>

Rectangle GetColumnRect(int col);

Vector2 CellToScreen(int row, int col);

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color);

void DrawHoles();

void DrawPreview(Vector2 pos, Game game);

void DrawChips(Game game);
