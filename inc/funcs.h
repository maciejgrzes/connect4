#include "vars.h"
#include <vector>

#ifndef FUNCS
#define FUNCS

void DrawStar(Vector2 center, float outerRadius, float innerRadius, int points, Color color);

bool CheckWin(int board[ROWS][COLS], int player);

bool FindWinningLine(int board[ROWS][COLS], int player, std::vector<Cell> (&winningCells));

void DrawHoles();

void DrawPreview(Vector2 pos, Rectangle columns[7], int heights[7], int turn);

void DrawChips(int board[ROWS][COLS]);

#endif
