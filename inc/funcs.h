#pragma once

#include "vars.h"
#include "structs.h"
#include <raylib.h>
#include <vector>

Rectangle GetColumnRect(int col);

Vector2 CellToScreen(int row, int col);

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color);

bool CheckWin(int board[ROWS][COLS], int player);

bool FindWinningLine(int board[ROWS][COLS], int player, std::vector<Cell> (&winningCells));

void DrawHoles();

void DrawPreview(Vector2 pos, int heights[7], int turn);

void DrawChips(int board[ROWS][COLS]);

void EndGame(bool (&game_over), int (&board)[ROWS][COLS], int (&heights)[COLS], std::vector<Cell> (&winningCells));
