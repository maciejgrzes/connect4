#pragma once

#include "game.hpp"
#include <raylib.h>
#include <string>

bool IsValidIPv4(std::string ip);

Rectangle GetColumnRect(int col);

Vector2 CellToScreen(int row, int col);

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color);

void DrawHoles();

void DrawPreview(Vector2 pos, Game game);

void DrawChips(Game game);
